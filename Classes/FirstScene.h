/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	FirstScene.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __FIRSTSCENE_H__
#define __FIRSTSCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/document.h"

class FirstScene : public cocos2d::Layer
{
public:
	FirstScene();
	~FirstScene();
	virtual bool init();
	CREATE_FUNC(FirstScene);

	static cocos2d::Scene* createScene();

private:
	void loadUI();

	void setContent(std::string name, std::string text, std::string imgName);
	
	//读取json中的剧情对话
	void readChatData();

	//读取下一条对白
	void nextMsg();

	//注册手机按键事件
	void addPhoneEventListener();

	//手机返回事件监听回调
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * pEvent);

private:
	//ui文件的常量名
	const std::string csbName = "layers/firstLogin/firstScene.csb";
	const unsigned heroNameTextTag = 33;
	const unsigned heroImageTag = 34;
	const unsigned chatContentTextTag = 35;
	const std::string storyFileName = "datas/story.data";

private:
	unsigned _currentIndex;
	rapidjson::Document  _doc;
	cocos2d::ui::Text* _heroName;
	cocos2d::ui::Text* _chatContent;
	cocos2d::ui::ImageView* _heroImage;
};

#endif // FIRSTSCENE_H_