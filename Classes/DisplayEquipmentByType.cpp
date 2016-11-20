/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DisplayEquipmentByType.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/19
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "DisplayEquipmentByType.h"
#include "cocostudio/CocoStudio.h"
#include "BlockLayer.h"
#include "GameData.h"
#include "AudioManager.h"
#include "Equipment.h"
#include "HeroCard.h"
#include "DisplayListItem.h"
#include "Tools.h"
#include "DialogManager.h"
#include "I18N.h"

USING_NS_CC;
using namespace ui;
using namespace std;

DisplayEquipmentByType::DisplayEquipmentByType()
{
}

DisplayEquipmentByType::~DisplayEquipmentByType()
{
}

bool DisplayEquipmentByType::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//触摸屏蔽，防止穿透点击到下一层
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockTagName);
	addChild(blockLayer);

	loadUI();

	return true;
}

void DisplayEquipmentByType::setDisPlayEquipmentType(ET et, int heroid)
{
	_et = et;
	_heroId = heroid;
}

void DisplayEquipmentByType::setBtnCallBack(std::function<void(cocos2d::Ref*pSender, int equipmentPos, int equipmentId, bool isAdd)> func)
{
	_func = func;
}

void DisplayEquipmentByType::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//查找控件
	_listView = node->getChildByName<ListView*>(listViewName);
	_btnClose = node->getChildByName<Button*>(btnCloseName);
	_tipLabel = node->getChildByName<Text*>(tipLabelName);

	//绑定事件回调
	_btnClose->addTouchEventListener(CC_CALLBACK_2(DisplayEquipmentByType::btnCloseCallBack, this));
}

void DisplayEquipmentByType::loadItem()
{
	auto equipmentMap = GameData::getInstance()->getEquipmentMap();

	for (auto iter = equipmentMap->begin(); iter != equipmentMap->end(); ++iter)
	{
		int type = iter->second->getProperty()->type / 100;
		
		//和当前选中的装备类型是否相同
		if (type == _et)
		{
			auto item = DisplayListItem::create();
			
			//设置item显示的属性
			auto property = (iter->second)->getProperty();
			setItemAttribute(property, item);
			
			int userId = property->user;

			//装备是否已经装备上
			if (userId != none)
			{
				//如果是当前位置装备的英雄则显示图片为卸下
				item->setBtnTexture(btnUnEquipment1, btnUnEquipment2);

				if (_heroId != userId)
				{
					//如果装备已经使用上,但不是装备在当前选中的位置
					item->setBtnEnable(false);
				}
			}
			else
			{
				item->setBtnTexture(btnEquipment1, btnEquipment2);
			}
			
			item->setBtnTag(_objectIdVector.size());

			item->setBtnCallBack([&, userId](Ref* pSender) {
				//调用用户没有设置的回调则结束
				if (!_func)
				{
					return;
				}

				auto btn = dynamic_cast<Button*>(pSender);
				_clickedIndex = btn->getTag();

				int equipmentId = _objectIdVector.at(_clickedIndex);

				if (userId != none)
				{
					//将按钮卸下图片改成装备图片
					btn->loadTextures(btnEquipment1, btnEquipment2, btnEquipment2, Widget::TextureResType::PLIST);
					
					_func(pSender, (int)_et, equipmentId, false);
					
					//提示
					DialogManager::getInstance()->showTips(I18N::getInstance()->getStringByKey(unEquipmentSuc), Color4B::GREEN);
				}
				else
				{
					//将按钮装备图片改成卸下图片
					btn->loadTextures(btnUnEquipment1, btnUnEquipment2, btnUnEquipment2, Widget::TextureResType::PLIST);
					
					_func(pSender, (int)_et, equipmentId, true);

					//提示
					DialogManager::getInstance()->showTips(I18N::getInstance()->getStringByKey(equipmentSuc), Color4B::GREEN);
				}

				//发送更新消息，并关闭显示的装备列表
				_eventDispatcher->dispatchCustomEvent(msg_update_hero_pageview_item, (void*)none);
				endAnimation();
			});

			//将item添加到listView，并保存item装备对应的Id
			_listView->pushBackCustomItem(item);
			_objectIdVector.push_back(iter->first);

		}
	}

	//容量使用提示
	_tipLabel->setString(StringUtils::format("%d/%d", _objectIdVector.size(), equipment_map_max_size));
}

void DisplayEquipmentByType::setItemColor(DisplayListItem * item, int star)
{
	Color4B color;

	//获得对应星级的颜色值
	switch (star)
	{
	case 1:
		color = Color4B::WHITE;
		break;
	case 2:
		color = Color4B::BLUE;
		break;
	case 3:
		color = Color4B::GREEN;
		break;
	case 4:
		color = Color4B::MAGENTA;
		break;
	case 5:
		color = Color4B::ORANGE;
		break;
	default:
		break;
	}

	for (int i = 0; i < 4; ++i)
	{
		item->setLabelColor(static_cast<ItemLabelType>(i), color);
	}
}

void DisplayEquipmentByType::onEnter()
{
	Layer::onEnter();

	loadItem();
	startAnimation();
}

void DisplayEquipmentByType::setItemAttribute(const EquipmentProperty * property, DisplayListItem * item)
{
	//设置文本标签
	item->setLabelText(ILT_LEVEL, StringUtils::format("Lv:%d", property->level));
	item->setLabelText(ILT_NAME, *(property->name));
	item->setStarNum(property->star);

	//设置颜色
	setItemColor(item, property->star);

	int type = property->type;

	if (Tools::betweenAnd(type, 100, 108))
	{
		//帽子
		item->setIco(*(property->textureName), defineIco, mDefineIco);
		item->setLabelText(ILT_ATTRIBUTE1, StringUtils::format("%d", property->defend));
		item->setLabelText(ILT_ATTRIBUTE2, StringUtils::format("%d", property->magicDefend));
	}
	else if (Tools::betweenAnd(type, 200, 207))
	{
		//衣服
		item->setIco(*(property->textureName), defineIco, mDefineIco);
		item->setLabelText(ILT_ATTRIBUTE1, StringUtils::format("%d", property->defend));
		item->setLabelText(ILT_ATTRIBUTE2, StringUtils::format("%d", property->magicDefend));
	}
	else if (Tools::betweenAnd(type, 300, 324))
	{
		//武器
		item->setIco(*(property->textureName), apIco, mpIco);
		item->setLabelText(ILT_ATTRIBUTE1, StringUtils::format("%d", property->ap));
		item->setLabelText(ILT_ATTRIBUTE2, StringUtils::format("%d", property->mp));
	}
	else if (Tools::betweenAnd(type, 400, 405))
	{
		//佩戴
		item->setIco(*(property->textureName), hpIco, critIco);
		item->setLabelText(ILT_ATTRIBUTE1, StringUtils::format("%d", property->hp));
		item->setLabelText(ILT_ATTRIBUTE2, StringUtils::format("%d%%", Tools::percentToInt(property->critDamage)));
	}
	else if (Tools::betweenAnd(type, 500, 504))
	{
		//鞋子
		item->setIco(*(property->textureName), speedIco, blockIco);
		item->setLabelText(ILT_ATTRIBUTE1, StringUtils::format("%d", property->speed));
		item->setLabelText(ILT_ATTRIBUTE2, StringUtils::format("%d%%", Tools::percentToInt(property->blockRate)));
	}
	else if (Tools::betweenAnd(type, 600, 604))
	{
		//坐骑
		item->setIco(*(property->textureName), hpIco, speedIco);
		item->setLabelText(ILT_ATTRIBUTE1, StringUtils::format("%d", property->hp));
		item->setLabelText(ILT_ATTRIBUTE2, StringUtils::format("%d", property->speed));
	}
}

void DisplayEquipmentByType::btnCloseCallBack(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		//点击音效
		AudioManager::getInstance()->playClickEffect();

		endAnimation();
	}
}

void DisplayEquipmentByType::startAnimation()
{
	//初始为0
	this->setScale(0.0f);

	auto ani = Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.0f), RotateTo::create(0.5f, 720)), CallFunc::create([&]() {
		//显示触摸锁定层
		this->getChildByName(blockTagName)->setOpacity(150);
	}));

	this->runAction(ani);
}

void DisplayEquipmentByType::endAnimation()
{
	//隐藏触摸屏蔽层
	this->getChildByName(blockTagName)->setOpacity(0.0f);

	auto ani = Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.0f), RotateTo::create(0.5f, 720)), CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}
