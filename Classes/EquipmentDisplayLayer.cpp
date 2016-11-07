/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	EquipmentDisplayLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "EquipmentDisplayLayer.h"
#include "cocostudio/CocoStudio.h"
#include "GameData.h"
#include "BlockLayer.h"
#include "Tools.h"
#include "Equipment.h"
#include "AudioManager.h"
#include "ListItem.h"

USING_NS_CC;
using namespace ui;
using namespace std;

EquipmentDisplayLayer::EquipmentDisplayLayer()
	:_preClicked(none)
{
}

EquipmentDisplayLayer::~EquipmentDisplayLayer()
{
}

bool EquipmentDisplayLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//触摸屏蔽，防止穿透点击到下一层
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockName);
	addChild(blockLayer);

	loadUI();

	//初始化Item
	auto equipment = GameData::getInstance()->getEquipmentMap();
	loadItem(equipment->size());

	return true;
}

void EquipmentDisplayLayer::onEnterTransitionDidFinish()
{
	startAnimation();
}

void EquipmentDisplayLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//查找控件
	_listView = node->getChildByName<ListView*>(listViewName);
	_btnClose = node->getChildByName<Button*>(btnCloseName);
	_tipLabel = node->getChildByName<Text*>(tipLabelName);

	//绑定事件回调
	_btnClose->addTouchEventListener(CC_CALLBACK_2(EquipmentDisplayLayer::btnCloseCallBack, this));
}

void EquipmentDisplayLayer::setTipLabel()
{
	_tipLabel->setString(StringUtils::format("%d/%d", _itemVector.size(), equipment_map_max_size));
}

void EquipmentDisplayLayer::setItemAttribute(const EquipmentProperty * property, ListItem* item)
{
	//设置文本标签
	item->setLabelText(LEVEL, StringUtils::format("Lv:%d", property->level));
	item->setLabelText(NAME, *(property->name));
	item->setStarNum(property->star);

	//设置颜色
	setItemColor(item, property->star);

	int type = property->type;

	if (Tools::betweenAnd(type, 100, 108))
	{
		//帽子
		item->setIco(*(property->textureName), defineIco, mDefineIco);

		item->setLabelText(TEXT1, StringUtils::format("%d", property->defend));
		item->setLabelText(TEXT2, StringUtils::format("%d", property->magicDefend));
	}
	else if (Tools::betweenAnd(type, 200, 207))
	{
		//衣服
		item->setIco(*(property->textureName), defineIco, mDefineIco);

		item->setLabelText(TEXT1, StringUtils::format("%d", property->defend));
		item->setLabelText(TEXT2, StringUtils::format("%d", property->magicDefend));
	}
	else if (Tools::betweenAnd(type, 300, 324))
	{
		//武器
		item->setIco(*(property->textureName), apIco, mpIco);

		item->setLabelText(TEXT1, StringUtils::format("%d", property->ap));
		item->setLabelText(TEXT2, StringUtils::format("%d", property->mp));
	}
	else if (Tools::betweenAnd(type, 400, 405))
	{
		//佩戴
		item->setIco(*(property->textureName), hpIco, critIco);

		item->setLabelText(TEXT1, StringUtils::format("%d", property->hp));
		item->setLabelText(TEXT2, StringUtils::format("%d%%", Tools::percentToInt(property->critDamage)));
	}
	else if (Tools::betweenAnd(type, 500, 504))
	{
		//鞋子
		item->setIco(*(property->textureName), speedIco, blockIco);

		item->setLabelText(TEXT1, StringUtils::format("%d", property->speed));
		item->setLabelText(TEXT2, StringUtils::format("%d%%", Tools::percentToInt(property->blockRate)));
	}
	else if (Tools::betweenAnd(type, 600, 604))
	{
		//坐骑
		item->setIco(*(property->textureName), hpIco, speedIco);

		item->setLabelText(TEXT1, StringUtils::format("%d", property->hp));
		item->setLabelText(TEXT2, StringUtils::format("%d", property->speed));
	}
}

void EquipmentDisplayLayer::updateItemAttribute(const int equipmentId, const int itenId)
{
	auto equipment = GameData::getInstance()->getEquipmentById(equipmentId);
	
	//更新单个item的数据
	setItemAttribute(equipment->getProperty(), _itemVector.at(itenId));
}

void EquipmentDisplayLayer::loadItem(int n)
{
	auto equipmentMap = GameData::getInstance()->getEquipmentMap();

	//开始加载的索引
	int startIndex = _itemVector.size();
	//结束的索引，实际加载到endIndex - 1
	int endIndex = startIndex + n;
	//当前的索引
	int index = 0;

	for (auto iter = equipmentMap->begin(); iter != equipmentMap->end(); ++iter)
	{
		if (Tools::betweenAnd(index, startIndex, endIndex - 1) && static_cast<unsigned>(startIndex) < equipmentMap->size())
		{
			auto item = ListItem::create();
			
			//设置item显示的属性
			auto property = (iter->second)->getProperty();
			setItemAttribute(property, item);
			item->setBtnTexture(btnDetails1, btnDetails2);
			item->setBtnTag(index);
			item->setBtnCallBack([&](Ref* pSender, Widget::TouchEventType type) { 
				if(type == Widget::TouchEventType::ENDED)
				{
					int tag = dynamic_cast<Button*>(pSender)->getTag();

					log("----%d", tag);
				}
			});
			
			//添加到listView和itemVector
			_listView->pushBackCustomItem(item);
			_itemVector.pushBack(item);

		}
		else if (static_cast<unsigned>(startIndex) >= equipmentMap->size() || index >= endIndex)
		{
			//如果当前的索引已经超过了要加载的最大索引，或者所有的item已经全部加载则结束循环
			break;
		}

		++index;
	}

	//设置列表上边容量使用信息
	setTipLabel();
}

void EquipmentDisplayLayer::setItemColor(ListItem * item, int star)
{
	Color4B color;

	//设置颜色
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
		color = Color4B(255, 0, 255, 255);
		break;
	case 5:
		color = Color4B(255, 102, 0, 255);
		break;
	default:
		break;
	}

	for (int i = 0; i < 4; ++i)
	{
		item->setLabelColor(static_cast<ItemLabelType>(i), color);
	}
}

void EquipmentDisplayLayer::btnCloseCallBack(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		//点击音效
		AudioManager::getInstance()->playClickEffect();

		endAnimation();
	}
}

void EquipmentDisplayLayer::startAnimation()
{
	//初始为0
	this->setScale(0.0f);

	auto ani = Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.0f), RotateTo::create(0.5f, 720)), CallFunc::create([&]() {
		//显示触摸锁定层
		this->getChildByName(blockName)->setOpacity(150);
	}));

	this->runAction(ani);
}

void EquipmentDisplayLayer::endAnimation()
{
	//隐藏触摸屏蔽层
	this->getChildByName(blockName)->setOpacity(0.0f);

	auto ani = Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.0f), RotateTo::create(0.5f, 720)), CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}
