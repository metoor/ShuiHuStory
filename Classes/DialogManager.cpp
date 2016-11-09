/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DialogManager.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "DialogManager.h"
#include "McLog.h"
#include "ConstantDefine.h"
#include "Tips.h"
#include "Dialog.h"


DialogManager* DialogManager::_instance = nullptr;

USING_NS_CC;
using namespace std;
using namespace ui;

DialogManager::DialogManager()
{
}

DialogManager::~DialogManager()
{
}

DialogManager * DialogManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new (std::nothrow)DialogManager();

		if (!_instance)
		{
			//错误日志记录
			McLog mLog;
			mLog.addError("_instance create failed...", __FILE__, __LINE__);
		}
	}

	return _instance;
}

void DialogManager::showTips(const std::string& content, cocos2d::Color4B color, int size)
{
	auto currentScene = Director::getInstance()->getRunningScene();
	auto tips = dynamic_cast<Tips*>(currentScene->getChildByName(tipsTagName));

	if (tips)
	{
		//场景里面有正在显示tips,不用再创建直接修改当前正在显示的tips显示内容
		tips->initTips(content, color, size);
	}
	else
	{
		//场景里面没有正在显示的tips，创建一个
		tips = Tips::create();
		tips->initTips(content, color, size);
		currentScene->addChild(tips, tip_z_order, tipsTagName);
	}
}

void DialogManager::showDialog(std::string title, std::string content, std::function<void(cocos2d::Ref*pSender)> func)
{
	auto currentScene = Director::getInstance()->getRunningScene();
	auto dialog = dynamic_cast<Dialog*>(currentScene->getChildByName(dialogTagName));

	if (!dialog)
	{
		//当前场景没有正在显示的会话框，创建对话框
		auto dialog = Dialog::create();
		dialog->initDialog(title, content, func);
		currentScene->addChild(dialog, dialog_z_order, dialogTagName);
	}
	else
	{
		CCASSERT(false, "scene has a tips displaying...");

		McLog mc;
		mc.addWaring("scene has a tips displaying", __FILE__, __LINE__);
	}
	
}

void DialogManager::showDialog(std::string title, std::string content, cocos2d::Color4B color, int contentSize, std::function<void(cocos2d::Ref*pSender)> func)
{
	auto currentScene = Director::getInstance()->getRunningScene();
	auto dialog = dynamic_cast<Dialog*>(currentScene->getChildByName(dialogTagName));

	if (!dialog)
	{
		//当前场景没有正在显示的会话框，创建对话框
		auto dialog = Dialog::create();
		dialog->initDialog(title, content, func);
		dialog->setContentColor(color);
		dialog->setFontSize(contentSize);
		currentScene->addChild(dialog, dialog_z_order, dialogTagName);
	}
	else
	{
		CCASSERT(false, "scene has a tips displaying...");

		McLog mc;
		mc.addWaring("scene has a tips displaying", __FILE__, __LINE__);
	}
}

void DialogManager::destoryInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}
