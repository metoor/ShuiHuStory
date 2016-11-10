/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	HomeLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "HomeLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "DisplayLayer.h"
#include "DetialsLayer.h"
#include "GameData.h"

USING_NS_CC;
using namespace ui;
using namespace std;

HomeLayer::HomeLayer()
	:_preMenu(nullptr)
{
}

HomeLayer::~HomeLayer()
{
}

bool HomeLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	loadUI();

	//初始化数组为nullptr
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		_layerPointer[index] = nullptr;
	}

	updateUserData();

	addUpdateUserDataEventListener();

	return true;
}

void HomeLayer::loadUI()
{
	//加载编辑好的主页场景文件
	auto homeLayer = CSLoader::createNode(csbName);
	addChild(homeLayer);

	_dianmodLabel = homeLayer->getChildByName<Text*>(diamodNumName);
	_goldLabel = homeLayer->getChildByName<Text*>(goldNumName);
	_levelLabel = homeLayer->getChildByName<Text*>(levelNumName);
	_expLabel = homeLayer->getChildByName<Text*>(expNumName);
	_expBar = homeLayer->getChildByName<LoadingBar*>(expBarName);

	//绑定菜单的回调方法
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		auto menu = homeLayer->getChildByTag<Button*>(index);
		menu->addTouchEventListener(CC_CALLBACK_2(HomeLayer::menuCallBack, this));
	}
}

void HomeLayer::addUpdateUserDataEventListener()
{
	//注册更新数据事件
	auto listen = EventListenerCustom::create(msg_update_user_data, [&](EventCustom* event) {
		updateUserData();
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
}

void HomeLayer::updateUserData()
{
	auto data = GameData::getInstance();

	_levelLabel->setString(StringUtils::format("%d", data->getLevel()));
	_goldLabel->setString(StringUtils::format("%d", data->getGold()));
	_dianmodLabel->setString(StringUtils::format("%d", data->getDiamond()));
	
	//设置经验条的当前经验和总得经验
	int maxExp = data->getExpLimit(start_exp_rate, data->getLevel());
	int currentExp = data->getExp();

	_expLabel->setString(StringUtils::format("%d/%d", currentExp, maxExp));
	_expBar->setPercent(currentExp * 1.0f / maxExp);
}

Layer* HomeLayer::createMenuLayer(HomeMenuType type)
{
	Layer* layer = nullptr;

	switch (type)
	{
	case HMT_EQUIPMENT:
	{
		auto displayLayer = DisplayLayer::create();
		displayLayer->setBtnTxture(btnDetails1, btnDetails2);
		displayLayer->setDisplayType(OT_EQUIPMENT);

		//列表里item里的按钮回调
		displayLayer->setBtnCallBack([&](Ref* pSender) {
			auto btn = static_cast<Button*>(pSender);
			int id = (int)btn->getUserData();

			//创建装备详情层并显示
			auto detailsLayer = DetialsLayer::create();
			detailsLayer->initUiWithId(id, DT_EQUIPMENT);
			
			addChild(detailsLayer);
		});

		layer = displayLayer;
		break;
	}
	case HMT_HERO:
	{
		auto displayLayer = DisplayLayer::create();
		displayLayer->setBtnTxture(btnDetails1, btnDetails2);
		displayLayer->setDisplayType(OT_HERO);

		//列表里item里的按钮回调
		displayLayer->setBtnCallBack([&](Ref* pSender) {
			auto btn = static_cast<Button*>(pSender);
			int id = (int)btn->getUserData();

			//创建英雄卡牌详情层并显示
			auto detailsLayer = DetialsLayer::create();
			detailsLayer->initUiWithId(id, DT_HERO);
			addChild(detailsLayer);
		});

		layer = displayLayer;
		break;
	}
	case HMT_LOGIN:
		break;
	case HMT_LEVEL_UP:
		break;
	case HMT_ABOUT:
		break;
	case HMT_AUDIO:
		break;
	default:
		break;
	}

	return layer;
}

void HomeLayer::menuCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN)
	{
		//播放点击音效
		AudioManager::getInstance()->playClickEffect();
	}

	if (type == Widget::TouchEventType::ENDED)
	{
		int currentTag = dynamic_cast<Button*>(pSender)->getTag();
		auto layer = createMenuLayer(static_cast<HomeMenuType>(currentTag));
		addChild(layer);
	}
}
