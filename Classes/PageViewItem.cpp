/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	PageViewItem.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/15
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "PageViewItem.h"
#include "cocostudio/CocoStudio.h"
#include "GameData.h"
#include "HeroCard.h"
#include "Equipment.h"
#include "AudioManager.h"
#include "DisplayEquipmentByType.h"

USING_NS_CC;
using namespace ui;
using namespace std;

PageViewItem::PageViewItem()
	:_heroCard(nullptr)
{
	//初始化为nullptr
	initBtnArrayToNullptr();
}

PageViewItem::~PageViewItem()
{
	//Button使用引用计数，所以在不使用的时候置空引用
	initBtnArrayToNullptr();
}

bool PageViewItem::init()
{
	if (!Layout::init())
	{
		return false;
	}

	loadUI();

	return true;
}

void PageViewItem::initWithHeroId(int heroId)
{
	_heroId = heroId;

	updateData();
}

void PageViewItem::setBtnCallBack(std::function<void(cocos2d::Ref*pSender)> func)
{
	_func = func;
}

void PageViewItem::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//设置大小
	setContentSize(node->getContentSize());
	
	_title = node->getChildByName<Text*>(titleLabelName);
	_apLabel = node->getChildByName<Text*>(apLabelName);
	_hpLabel = node->getChildByName<Text*>(hpLabelName);
	_heroImg = node->getChildByName<ImageView*>(heroImgName);
	_apIco = node->getChildByName<ImageView*>(apIcoName);

	//寻找装备按钮，并绑定回调
	for (int pos = 0; pos < max_equipment_num; ++pos)
	{
		_btnArray[pos] = node->getChildByTag<Button*>(pos);
		_btnArray[pos]->addTouchEventListener(CC_CALLBACK_2(PageViewItem::BtnCallBack, this));
	}

}

void PageViewItem::initBtnArrayToNullptr()
{
	for (int pos = 0; pos < max_equipment_num; ++pos)
	{
		_btnArray[pos] = nullptr;
	}
}

void PageViewItem::updateData(int id)
{
	auto data = GameData::getInstance();

	if (id != none)
	{
		_heroId = id;
	}

	_heroCard = data->getHeroCardById(_heroId);

	auto property = _heroCard->getProperty();

	//英雄名字
	_title->setString(*(property->name));
	_title->setTextColor(getColorByStar(property->star));

	//更新英雄图片
	_heroImg->loadTexture(*(property->textureName));

	//更新血量和攻击标签
	_hpLabel->setString(StringUtils::format("%d", property->hp));

	if (property->isMagic)
	{
		//魔法攻击图标
		_apIco->loadTexture(mpIco, TextureResType::PLIST);
		_apLabel->setString(StringUtils::format("%d", property->mp));
	}
	else
	{
		//物理攻击图标
		_apIco->loadTexture(apIco, TextureResType::PLIST);
		_apLabel->setString(StringUtils::format("%d", property->ap));
	}
	
	//更新按钮图片纹理
	updateEquipmentBtnTexture();
}

void PageViewItem::updateEquipmentBtnTexture()
{
	auto data = GameData::getInstance();
	auto heroEquipment = _heroCard->getProperty()->equipmentId;

	for (int pos = 0; pos < max_equipment_num; ++pos)
	{
		int id = heroEquipment[pos];
		
		if (id != none)
		{
			//获取当前位置的装备属性
			auto property = data->getEquipmentById(id)->getProperty();
			_btnArray[pos]->loadTextureNormal(*(property->textureName), Widget::TextureResType::PLIST);
		}
		else
		{
			//英雄该部位没有装备装备，加载默认的图片
			_btnArray[pos]->loadTextureNormal(addIco, Widget::TextureResType::PLIST);
		}
	}
}

Color4B PageViewItem::getColorByStar(int star)
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
		color = Color4B::YELLOW;
		break;
	default:
		break;
	}

	return color;
}

void PageViewItem::BtnCallBack(Ref * pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN)
	{
		AudioManager::getInstance()->playClickEffect();
	}

	if (type == Widget::TouchEventType::ENDED)
	{
		if(_func)
		{
			_func(pSender);
		}
	}
}
