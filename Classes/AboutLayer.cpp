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
#include "Tools.h"

USING_NS_CC;
using namespace ui;

AboutLayer::AboutLayer()
	:_isSelect(false),
	_currentPos(0.0f)
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
	_scrollView = node->getChildByName<ScrollView*>(scrollViewName);

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

	//添加ScrollView的事件监听
	_scrollView->addEventListener((ui::ListView::ccScrollViewCallback)CC_CALLBACK_2(AboutLayer::selectedItemEventScrollView, this));
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
		dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(bl_opacity_on);
	}));

	this->runAction(ani);
}

void AboutLayer::endAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//先隐藏触摸锁层
	dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(bl_opacity_off);

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

	//开启自动滚动
	scheduleUpdate();
}

void AboutLayer::onExitTransitionDidStart()
{
	unscheduleUpdate();
}

void AboutLayer::update(float dt)
{
	//log("isSelect:%d, curPos:%f", _isSelect, _currentPos);
	if (!_isSelect && _currentPos < 100)
	{
		//如果没有滑动到最底部，则自动向下滑动
		_scrollView->jumpToPercentVertical(_currentPos);
		_currentPos += dt * 1.5f;
	}
}

void AboutLayer::selectedItemEventScrollView(Ref* pSender, ui::ScrollView::EventType type)
{
	switch (type)
	{
	case cocos2d::ui::ScrollView::EventType::CONTAINER_MOVED:
	{
		if (_isSelect)
		{
			auto startpos = _scrollView->getTouchBeganPosition();
			auto endPos = _scrollView->getTouchEndPosition();
			
			float height = _scrollView->getInnerContainer()->getContentSize().height;

			//转换成Y轴距离的百分比
			float disY = (endPos.y - startpos.y) * 10 / height;

			//log("dis:%f,--star:(%f, %f), end:(%f, %f)", disY, startpos.x, startpos.y, endPos.x, endPos.y);


			_currentPos = Tools::maxFloat(0, Tools::minFloat(99, _currentPos + disY));
			_isSelect = false;
		}

		
	}
		break;
	case ScrollView::EventType::SCROLLING:
		_isSelect = true;
		break;
	default:
		break;
	}
}