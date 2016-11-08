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
#include "ConstantDefine.h"

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

	return true;
}

void HomeLayer::loadUI()
{
	//加载编辑好的主页场景文件
	auto homeLayer = CSLoader::createNode(csbName);
	addChild(homeLayer);

	//绑定菜单的回调方法
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		auto menu = homeLayer->getChildByTag<Button*>(index);
		menu->addTouchEventListener(CC_CALLBACK_2(HomeLayer::menuCallBack, this));
	}
}

Layer* HomeLayer::createMenuLayer(HomeMenuType type)
{
	Layer* layer = nullptr;

	switch (type)
	{
	case MENU_EQUIPMENT:
	{
		auto displayLayer = DisplayLayer::create();
		displayLayer->setBtnTxture(btnDetails1, btnDetails2);
		
		//列表里item里的按钮回调
		displayLayer->setBtnCallBack([](Ref* pSender, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				auto btn = static_cast<Button*>(pSender);
				int id = (int)btn->getUserData();

				log("----%d", id);
			}
		});
		displayLayer->setItemType(EQUIPMENT);
		layer = displayLayer;
		break;
	}
	case MENU_HERO:
	{
		auto displayLayer = DisplayLayer::create();
		displayLayer->setBtnTxture(btnDetails1, btnDetails2);

		//列表里item里的按钮回调
		displayLayer->setBtnCallBack([](Ref* pSender, Widget::TouchEventType type) {
			if (type == Widget::TouchEventType::ENDED)
			{
				auto btn = static_cast<Button*>(pSender);
				int id = (int)btn->getUserData();

				log("----%d", id);
			}
		});

		displayLayer->setItemType(HERO);
		layer = displayLayer;
		break;
	}
	case MENU_LOGIN:
		break;
	case MENU_LEVEL_UP:
		break;
	case MENU_ABOUT:
		break;
	case MENU_AUDIO:
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
