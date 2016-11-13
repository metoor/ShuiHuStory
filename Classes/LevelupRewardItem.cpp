/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	LevelupRewardItem.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/12
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "LevelupRewardItem.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "I18N.h"
#include "ConstantDefine.h"

USING_NS_CC;
using namespace ui;
using namespace std;

LevelupRewardItem::LevelupRewardItem()
	:_func(nullptr)
{
}

LevelupRewardItem::~LevelupRewardItem()
{
}

bool LevelupRewardItem::init()
{
	if (!Layout::init())
	{
		return false;
	}

	loadUI();

	return true;
}

void LevelupRewardItem::setBtnTag(int tag)
{
	_btnGet->setTag(tag);
}

void LevelupRewardItem::setRewardString(int level, int diamond, const int gold)
{
	_titleLabel->setString(StringUtils::format(I18N::getInstance()->getStringByKey(levelup_gift).c_str(), level));
	_diamondLabel->setString(StringUtils::format("%d", diamond));
	_goldLabel->setString(StringUtils::format("%d", gold));
}

void LevelupRewardItem::setBtnCallback(std::function<void(cocos2d::Ref*pSender)> func)
{
	_func = func;
}

void LevelupRewardItem::setBtnEnable(bool isEnable)
{
	_btnGet->setEnabled(isEnable);
}

void LevelupRewardItem::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//ÉèÖÃ´óÐ¡
	setContentSize(node->getContentSize());

	_btnGet = node->getChildByName<Button*>(btnGetName);
	_diamondLabel = node->getChildByName<Text*>(diamondLabelName);
	_goldLabel = node->getChildByName<Text*>(goldLbaelName);
	_titleLabel= node->getChildByName<Text*>(titleLbaelName);

	_btnGet->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
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
	});
}
