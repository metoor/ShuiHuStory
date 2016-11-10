/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	Tips.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/09
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "Tips.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace std;
using namespace ui;

Tips::Tips()
{
}

Tips::~Tips()
{
}

bool Tips::init()
{
	if (!Layer::init())
	{
		return false;
	}

	loadUI();

	return true;
}

void Tips::setContent(const std::string& content)
{
	_content->setString(content);
}

void Tips::setContentFontSize(int contentSize)
{
	_content->setFontSize(contentSize);
}

void Tips::setContentColor(cocos2d::Color4B & color)
{
	_content->setTextColor(color);
}

void Tips::initTips(const std::string & content, cocos2d::Color4B color, int contentSize)
{
	setContent(content);
	setContentColor(color);
	setContentFontSize(contentSize);
}

void Tips::loadUI()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//获取cocos stuido编辑好的ui文件
	auto node = CSLoader::createNode(csbName);
	
	//设置提示框的位置在屏幕中心上一点
	node->setAnchorPoint(Point::ANCHOR_MIDDLE);
	node->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 1.3f));
	addChild(node);

	//获取显示文本内容的Text控件
	_content = node->getChildByName<Text*>(contenName);
}

void Tips::onEnterTransitionDidFinish()
{
	//淡入淡出动画，完成动画后然或自己销毁
	this->setOpacity(0.0f);

	auto ani = Sequence::create(FadeIn::create(1.0f), DelayTime::create(3.0f), FadeOut::create(1.0f), CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}), NULL);

	this->runAction(ani);
}
