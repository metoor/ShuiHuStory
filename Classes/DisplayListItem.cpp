/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DisplayListItem.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "DisplayListItem.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

DisplayListItem::DisplayListItem()
{
}

DisplayListItem::~DisplayListItem()
{
}

bool DisplayListItem::init()
{
	if (!Layout::init())
	{
		return false;
	}

	loadUI();

	return true;
}

void DisplayListItem::setBtnCallBack(function<void(Ref*pSender)> func)
{
	_btn->addTouchEventListener([func](Ref*pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			//播放点击音效
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			func(pSender);
		}
		
	});
}

void DisplayListItem::setBtnTag(const int tag)
{
	_btn->setTag(tag);
}

void DisplayListItem::setBtnTexture(const std::string& nor, const std::string& pressed)
{
	_btn->loadTextureNormal(nor, Widget::TextureResType::PLIST);
	_btn->loadTexturePressed(pressed, Widget::TextureResType::PLIST);
	_btn->loadTextureDisabled(pressed, Widget::TextureResType::PLIST);
}

void DisplayListItem::setLabelText(ItemLabelType type, const std::string& content)
{
	switch (type)
	{
	case ILT_LEVEL:
		_level->setString(content);
		break;
	case ILT_NAME:
		_name->setString(content);
		break;
	case ILT_ATTRIBUTE1:
		_attribute1->setString(content);
		break;
	case ILT_ATTRIBUTE2:
		_attribute2->setString(content);
		break;
	default:
		break;
	}
}

void DisplayListItem::setLabelColor(ItemLabelType type, cocos2d::Color4B color)
{
	switch (type)
	{
	case ILT_LEVEL:
		_level->setTextColor(color);
		break;
	case ILT_NAME:
		_name->setTextColor(color);
		break;
	case ILT_ATTRIBUTE1:
		_attribute1->setTextColor(color);
		break;
	case ILT_ATTRIBUTE2:
		_attribute2->setTextColor(color);
		break;
	default:
		break;
	}
}

void DisplayListItem::setIco(const std::string& icoName, const std::string& img1Name, const std::string& img2Name)
{
	_ico->loadTexture(icoName, Widget::TextureResType::PLIST);
	_img1->loadTexture(img1Name, Widget::TextureResType::PLIST);
	_img2->loadTexture(img2Name, Widget::TextureResType::PLIST);
}

void DisplayListItem::setStarNum(const int n)
{
	for (int i = 0; i < n - 1; ++i)
	{
		auto addStar = _star->clone();
		addStar->setPosition(Vec2(_star->getPositionX() + _star->getContentSize().width * (i + 1) * 0.75f, _star->getPositionY()));
		addChild(addStar);
	}
}

void DisplayListItem::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//设置大小
	setContentSize(node->getContentSize());

	//寻找控件
	_ico = node->getChildByName<ImageView*>(icoName);
	_level = node->getChildByName<Text*>(levelLabelName);
	_name = node->getChildByName<Text*>(nameLabelName);
	_star = node->getChildByName<ImageView*>(starName);
	_img1 = node->getChildByName<ImageView*>(img1Name);
	_img2 = node->getChildByName<ImageView*>(img2Name);
	_attribute1 = node->getChildByName<Text*>(text1Name);
	_attribute2 = node->getChildByName<Text*>(text2Name);
	_btn = node->getChildByName<Button*>(btnName);
}
