/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	HeroLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/18
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "HeroLayer.h"
#include "cocostudio/CocoStudio.h"
#include "BlockLayer.h"
#include "GameData.h"
#include "AudioManager.h"
#include "PageViewItem.h"
#include "DisplayEquipmentByType.h"
#include "HeroCard.h"
#include "Equipment.h"
#include "DetailsLayer.h"
#include "McLog.h"
#include "DisplayLayer.h"
#include "DisplayListItem.h"
#include "DialogManager.h"
#include "I18N.h"

USING_NS_CC;
using namespace ui;
using namespace std;

HeroLayer::HeroLayer()
{
	//初始化数组
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		_itemId[pos] = none;
	}
}

HeroLayer::~HeroLayer()
{
}

bool HeroLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//吞噬点击事件
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockTagName);
	addChild(blockLayer);

	loadUI();

	addEquipmentUpateEventListener();
	addHeroUpateEventListener();

	return true;
}

void HeroLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	_pageView = node->getChildByName<PageView*>(pageViewName);
	_btnChange = node->getChildByName<Button*>(btnChangeName);
	_btnDetails = node->getChildByName<Button*>(btnDetailsName);

	//绑定点击回调
	_btnChange->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			//创建可以选择的英雄列表
			auto layer = DisplayLayer::create();
			layer->setTag(displayLayerTag);
			layer->setDisplayType(OT_HERO);
			layer->setBtnCallBack([&](Ref* pSender) {
				int pos = _pageView->getCurrentPageIndex();
				int currentId = _itemId[pos];

				auto btn = dynamic_cast<Button*>(pSender);
				int id = (int)btn->getUserData();
				auto data = GameData::getInstance();

				//将已经上阵的英雄装备换给即将上阵的英雄
				auto currentHero = data->getHeroCardById(currentId);
				//即将上阵的英雄
				auto hero = data->getHeroCardById(id);

				for (int pos = 0; pos < max_equipment_num; ++pos)
				{
					int currentEquipmentId = currentHero->getEquipmentByPos(pos);
					
					if (currentEquipmentId != none)
					{
						auto equipment = data->getEquipmentById(currentEquipmentId);

						//将当前装备的使用者设置为即将上阵的英雄
						equipment->setUserId(id);
						hero->wearEquipment(pos, currentEquipmentId);
						currentHero->wearEquipment(pos, none);
					}
				}

				//更新出战列表的英雄Id
				int heroPos = data->getPosById(currentId);

				if (heroPos != none)
				{
					//将当前位置的英雄下阵
					data->setBattleHero(heroPos, id);
					
					//更新保存pageview的item的id
					_itemId[heroPos] = id;
				}

				//换将成功提示
				DialogManager::getInstance()->showTips(I18N::getInstance()->getStringByKey(unbattleSuc), Color4B::GREEN);
				
				//发送更新数据的消息
				_eventDispatcher->dispatchCustomEvent(msg_update_hero_pageview_item, (void*)id);
				
				//通过Tag获取到列表的指针，然后关闭列表
				dynamic_cast<DisplayLayer*>(getChildByTag(displayLayerTag))->endAnimation();
			});
			addChild(layer);

			setEnableBtn(layer);
		}
	});

	_btnDetails->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			int pos = _pageView->getCurrentPageIndex();
			int id = _itemId[pos];

			if (id != none)
			{
				//id有效，创建详细属性界面
				auto layer = DetailsLayer::create();
				layer->initUiWithId(id, DT_HERO);
				addChild(layer);
			}
			else
			{
				McLog mc;
				mc.addWaring("'id' value is error...", __FILE__, __LINE__);
			}
			
		}
	});

}

void HeroLayer::loadItem()
{
	int itemIndex = 0;

	auto data = GameData::getInstance();

	auto battleHero = data->getBattleHeroArray();

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int id = battleHero[pos];
		
		if (id != none)
		{
			auto item = PageViewItem::create();
			item->initWithHeroId(id);

			//设置pageview中装备item选中回调
			item->setBtnCallBack([&, pos, battleHero](Ref* pSender) {
				int heroId = battleHero[pos];
				auto btn = dynamic_cast<Button*>(pSender);
				int tag = btn->getTag();

				auto layer = DisplayEquipmentByType::create();
				layer->setDisPlayEquipmentType(ET(tag), heroId);

				//设置listview中item按钮点击事件
				layer->setBtnCallBack([&, heroId](Ref* pSender, int equipmentPos, int equipmentId, bool isAdd) {
					
					auto data = GameData::getInstance();
					auto hero = data->getHeroCardById(heroId);

					//如果该位置已经有装备，先将已有装备移除
					int preEquipmentId = hero->getEquipmentByPos(equipmentPos);
					
					if (preEquipmentId != none)
					{
						auto equipment = data->getEquipmentById(preEquipmentId);
						equipment->setUserId(none);
					}

					if (isAdd)
					{
						//为英雄添加装备
						auto curEquipment = data->getEquipmentById(equipmentId);
						curEquipment->setUserId(heroId);
						hero->wearEquipment(equipmentPos, equipmentId);	
					}
					else
					{
						//卸下英雄的装备
						hero->wearEquipment(equipmentPos, none);
					}
					
					hero->updatetePropery();
				});

				addChild(layer);
			});

			_pageView->addPage(item);
			_itemId[itemIndex++] = id;
		}
	}

	//设置默认显示的页面
	_pageView->setCurrentPageIndex(0);
}

void HeroLayer::setEnableBtn(DisplayLayer* display)
{
	auto objectVector = display->getObjectIdVector();
	auto data = GameData::getInstance();

	for (unsigned int index = 0; index < objectVector->size(); ++index)
	{
		int id = objectVector->at(index);
		auto item = display->getObjectItemByIndex(index);

		//判断是否已经上阵了
		if (data->isBattleHero(id))
		{
			//英雄已经上阵，不能在被选择上阵
			item->setBtnTexture(btnUnbattleHero1, btnUnbattleHero2);
			item->setBtnEnable(false);
		}
		else
		{
			//英雄没有上阵，正常上阵
			item->setBtnTexture(btnBattleHero1, btnBattleHero2);
		}
	}
}

void HeroLayer::addEquipmentUpateEventListener()
{
	//注册更新数据事件
	auto listen = EventListenerCustom::create(msg_update_hero_pageview_item, CC_CALLBACK_1(HeroLayer::updateEnentListern, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
}

void HeroLayer::addHeroUpateEventListener()
{
	//注册更新数据事件
	auto listen = EventListenerCustom::create(msg_update_display_item, CC_CALLBACK_1(HeroLayer::updateEnentListern, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
}

void HeroLayer::onEnterTransitionDidFinish()
{
	loadItem();
}

void HeroLayer::updateEnentListern(cocos2d::EventCustom* event)
{
	int id = (int)event->getUserData();

	int index = _pageView->getCurrentPageIndex();
	auto item = dynamic_cast<PageViewItem*>(_pageView->getItem(index));
	item->updateData();

	//只有在更换英雄的时候会传递有意义的Id
	if (id > init_unique_num)
	{
		item->updateData(id);
	}
	else
	{
		item->updateData();
	}
}
