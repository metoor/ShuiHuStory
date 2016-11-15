/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	AboutLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/14
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "AboutLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "BlockLayer.h"
#include "ConstantDefine.h"

USING_NS_CC;
using namespace ui;

AboutLayer::AboutLayer()
{
}

AboutLayer::~AboutLayer()
{
}

bool AboutLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//触摸锁定层
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockTagName);
	addChild(blockLayer);

	loadUI();

	return true;
}

void AboutLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	_btnClose = node->getChildByName<Button*>(btnCloseName);

	_btnClose->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (Widget::TouchEventType::ENDED == type)
		{
			endAnimation();
		}
	});
}

void AboutLayer::startAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//设置位置在屏幕外面
	this->setPosition(-visibleSize.width, 0);

	//移动至屏幕中央
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(0, 0)), 0.2f);

	//在移动完成后，显示触摸锁定层
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		//显示触摸锁定层
		dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(160.0f);
	}));

	this->runAction(ani);
}

void AboutLayer::endAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//先隐藏触摸锁层
	dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(0.0f);

	//移动到屏幕外面
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(-visibleSize.width, 0)), 0.2f);

	//在完成移动后释放资源
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}

void AboutLayer::onEnterTransitionDidFinish()
{
	startAnimation();
}
