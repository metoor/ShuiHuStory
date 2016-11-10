/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DialogManager.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "cocos2d.h"
#include <functional>


class DialogManager{
public:
	~DialogManager();

	//获得实例
	static DialogManager* getInstance();

	//显示一个非模态提示框
	void showTips(const std::string& content, cocos2d::Color4B color = cocos2d::Color4B::RED, int size = 35);

	//显示模态对话框
	void showDialog(const std::string& title, const std::string& content, std::function<void(cocos2d::Ref * pSender)> func, bool isDisplayCancelBtn = true, cocos2d::Color4B color = cocos2d::Color4B::RED, int contentSize = 35);

	//销毁实例
	void destoryInstance();

private:
	DialogManager();

private:
	//实例
	static DialogManager* _instance;
};

#endif // DIALOGMANAGER_H_
