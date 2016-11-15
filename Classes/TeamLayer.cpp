/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	TeamLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/14
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "TeamLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "GameData.h"
#include "HeroCard.h"
#include "DisplayLayer.h"
#include "AudioManager.h"
#include "DisplayListItem.h"
#include "DialogManager.h"
#include "I18N.h"

USING_NS_CC;
using namespace ui;
using namespace std;

TeamLayer::TeamLayer()
	:_selectTag(none),
	_displayLayer(nullptr)
{
}

TeamLayer::~TeamLayer()
{
}

bool TeamLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	loadUI();

	addTouchEventListener();

	updateData();

	return true;
}

void TeamLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);
	
	for (int index = 0; index < max_battle_hero_num; ++index)
	{
		_nameLabel[index] = node->getChildByName<Text*>(StringUtils::format(nameLabel.c_str(), index));
		_heroIco[index] = node->getChildByName<Sprite*>(StringUtils::format(heroIcoSprite.c_str(), index));
	}
}

void TeamLayer::updateData()
{
	auto battleArray = GameData::getInstance()->getBattleHeroArray();

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int id = battleArray[pos];
		
		if (id < init_unique_num)
		{
			//如果id == -1， 表示该位置没有英雄,显示默认图片
			if (id == -1)
			{
				_heroIco[pos]->setSpriteFrame(addIco);
				_nameLabel[pos]->setString("");
			}
		}
		else
		{
			//获得该位置上的英雄信息
			auto property = GameData::getInstance()->getHeroCardById(id)->getProperty();

			string icoName = StringUtils::format(heroName.c_str(), property->type);
			
			//设置显示头像
			_heroIco[pos]->setSpriteFrame(icoName);
			//设置显示的名字
			_nameLabel[pos]->setString(*(property->name));
		}
	}
}

void TeamLayer::setUnBattleBtn()
{
	auto objIdVector = _displayLayer->getObjectIdVector();

	for (unsigned int index = 0; index < objIdVector->size(); ++index)
	{
		int id = objIdVector->at(index);
		
		if (GameData::getInstance()->isBattleHero(id))
		{
			//如果英雄已经上阵了，则换成下阵按钮图片
			auto item = _displayLayer->getObjectItemByIndex(index);
			item->setBtnTexture(btnUnbattleHero1, btnUnbattleHero2);
		}
	}
}

int TeamLayer::getHeroIcoisContainPoint(cocos2d::Point & point)
{
	int result = none;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		auto sp = _heroIco[pos];

		//如果点在任何一个英雄图标里面，则返回所在的标签
		if(sp->getBoundingBox().containsPoint(point))
		{
			int tag = sp->getTag();
			
			//判断选中的是不是自己
			if (tag != _selectTag)
			{
				result = tag;
				break;
			}
			else
			{
				//如果是自己，则忽略继续向下查找
				continue;
			}
		}
	}

	return result;
}

void TeamLayer::addTouchEventListener()
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(TeamLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(TeamLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(TeamLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool TeamLayer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Point touchPos = touch->getLocation();

	_selectTag = getHeroIcoisContainPoint(touchPos);

	if (_selectTag < 0)
	{
		//如果没有选中英雄图标，则忽略本次触摸，不在执行后续操作
		return false;
	}

	//选中后缩小精灵
	_heroIco[_selectTag]->setScale(0.8f);
	_nameLabel[_selectTag]->setScale(0.8f);

	//设置英雄图标和名字的z顺序，防止被遮挡
	_heroIco[_selectTag]->setLocalZOrder(select_z_order);
	_nameLabel[_selectTag]->setLocalZOrder(select_z_order);

	return true;
}

void TeamLayer::onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Point pos = touch->getLocation();

	//移动英雄图标和英雄名字
	_heroIco[_selectTag]->setPosition(pos);
	_nameLabel[_selectTag]->setPosition(pos - distanceNameToIco);
}

void TeamLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	Point pos = touch->getLocation();
	float distance = pos.distance(touch->getStartLocation());

	//如果移动的距离小于5.0f则忽略移动,认为是点击事件
	if (distance > 5.0f)
	{
		//更换英雄位置
		int endTag = getHeroIcoisContainPoint(pos);

		if (endTag < 0)
		{
			//移动结束，还原大小
			_heroIco[_selectTag]->setScale(1.0f);
			_nameLabel[_selectTag]->setScale(1.0f);

			//拖拽结束的位置非法，放回原来的位置
			_heroIco[_selectTag]->setPosition(HeroIcoPos[_selectTag]);
			_nameLabel[_selectTag]->setPosition(HeroNamePos[_selectTag]);
		}
		else
		{
			//更换位置成功,将托拽的英雄图标设置到新的位置
			_heroIco[_selectTag]->setPosition(HeroIcoPos[endTag]);
			_nameLabel[_selectTag]->setPosition(HeroNamePos[endTag]);

			//将结束位置的英雄放到托拽英雄的原始位置
			_heroIco[endTag]->setPosition(HeroIcoPos[_selectTag]);
			_nameLabel[endTag]->setPosition(HeroNamePos[_selectTag]);

			//移动结束，还原大小
			_heroIco[_selectTag]->setScale(1.0f);
			_nameLabel[_selectTag]->setScale(1.0f);
			
			//交换精灵的指针引用
			auto sp = _heroIco[_selectTag];
			_heroIco[_selectTag] = _heroIco[endTag];
			_heroIco[endTag] = sp;

			//交换名字的指针引用
			auto lb = _nameLabel[_selectTag];
			_nameLabel[_selectTag] = _nameLabel[endTag];
			_nameLabel[endTag] = lb;

			//更换英雄的标签
			_heroIco[_selectTag]->setTag(_selectTag);
			_heroIco[endTag]->setTag(endTag);

			//交换出战数组里面的位置
			auto battleArray = GameData::getInstance()->getBattleHeroArray();
			int id = battleArray[_selectTag];
			battleArray[_selectTag] = battleArray[endTag];
			battleArray[endTag] = id;

		}

	}
	else
	{
		//播放点击音效
		AudioManager::getInstance()->playClickEffect();

		//更换英雄，第一步是将移动过的英雄复位
		_heroIco[_selectTag]->setPosition(HeroIcoPos[_selectTag]);
		_nameLabel[_selectTag]->setPosition(HeroNamePos[_selectTag]);

		//移动结束，还原大小
		_heroIco[_selectTag]->setScale(1.0f);
		_nameLabel[_selectTag]->setScale(1.0f);

		//创建英雄显示列表层
		_displayLayer = DisplayLayer::create();
		_displayLayer->setDisplayType(OT_HERO);

		_displayLayer->setBtnTexture(btnBattleHero1, btnBattlehero2);

		int selectTag = _selectTag;

		_displayLayer->setBtnCallBack([&, selectTag](Ref* pSender) {
			
			auto btn = dynamic_cast<Button*>(pSender);
			int tag = btn->getTag();
			auto data = GameData::getInstance();

			//取得选中英雄的Id
			int id = _displayLayer->getObjectIdByIndex(tag);
			auto item = _displayLayer->getObjectItemByIndex(tag);

			if (data->isBattleHero(id))
			{
				//如果当前只有一个英雄，则不允许下阵。(需要保证至少有一个英雄上阵)
				if (data->getBattleHeroNum() < 2)
				{
					//提示下阵
					DialogManager::getInstance()->showTips(I18N::getInstance()->getStringByKey(atleastOneHero), Color4B::RED);
				}
				else
				{
					//英雄已经上阵，所以现在是下阵
					data->unbattleHero(id);

					//将下阵的按钮换换成上阵按钮
					item->setBtnTexture(btnBattleHero1, btnBattlehero2);
					
					//提示下阵
					DialogManager::getInstance()->showTips(I18N::getInstance()->getStringByKey(battleSuc), Color4B::GREEN);
					
					//更新数据
					updateData();

					//关闭列表框
					_displayLayer->endAnimation();
				}
			}
			else
			{
				//英雄没有上阵，所以现在是上阵
				data->setBattleHero(selectTag, id);

				//将上阵的按钮换换成下阵按钮
				item->setBtnTexture(btnUnbattleHero1, btnUnbattleHero2);

				//提示上阵
				DialogManager::getInstance()->showTips(I18N::getInstance()->getStringByKey(unbattleSuc), Color4B::GREEN);
				
				//更新数据
				updateData();

				//关闭列表框
				_displayLayer->endAnimation();
			}

		});

		addChild(_displayLayer);
		
		setUnBattleBtn();
	}

	//移动完毕，设置英雄图标和名字的z顺序为默认值
	_heroIco[_selectTag]->setLocalZOrder(default_z_order);
	_nameLabel[_selectTag]->setLocalZOrder(default_z_order);

	//取消选中项
	_selectTag = none;
}