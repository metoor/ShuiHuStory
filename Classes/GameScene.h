/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	GameScene.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"


//六个功能菜单对应的6个场景的数组大小
#define ARRAY_SIZE 6

class GameScene : public cocos2d::Layer{
public:
	GameScene();
	~GameScene();
	static cocos2d::Scene* createScene();
	virtual bool init() override;
	CREATE_FUNC(GameScene);


private:
	//手机返回键处理回调
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * pEvent);

	void initArrayToNullptr();

	void loadUI();

	void menuCallBack(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type);

private:
	//常量名称
	const std::string csbName = "layers/homeLayer/GameSceneBgLayer.csb";
	const std::string csbMenuName = "layers/homeLayer/menuLayer.csb";
	const std::string menuHomeName = "menuHero";
	const std::string menuTeamName = "menuTeam";
	const std::string menuHeroName = "menuHero";
	const std::string menuWarName = "menuWar";
	const std::string menuPictureName = "menuPicture";
	const std::string menuStoreName = "menuStore";

private:
	//保存上一次点击的菜单按钮的指针引用
	cocos2d::ui::Button* _preMenu;

	//用来存储6个功能层的指针引用
	cocos2d::Layer* _layerPointer[ARRAY_SIZE];

};

#endif // GAMESCENE_H_
