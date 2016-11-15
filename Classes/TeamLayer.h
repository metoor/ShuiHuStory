/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	TeamLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/14
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef TEAMLAYER_H_  
#define TEAMLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ConstantDefine.h"

class DisplayLayer;

class TeamLayer : public cocos2d::Layer
{
public:
	TeamLayer();
	~TeamLayer();
	virtual bool init() override;
	CREATE_FUNC(TeamLayer);

private:
	void loadUI();

	//显示出战英雄
	void updateData();

	//将已经上场的英雄item的按钮更换成下阵
	void setUnBattleBtn();

	//获得point选中了那个英雄头像，返回-1表示没有选中任何头像
	int getHeroIcoisContainPoint(cocos2d::Point& point);

	//注册触摸事件监听回调
	void addTouchEventListener();

	//触摸开始回调
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);

	//触摸移动回调
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);

	//触摸结束回调
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

private:
	//csb布局文件里面的控件名字
	const std::string csbName = "layers/teamLayer/teamLayer.csb";
	const std::string nameLabel = "name_%d";
	const std::string heroIcoSprite = "hero_%d";
	const std::string heroName = "head%d.png";
	const std::string btnSaveName = "btnSave";
	const int default_z_order = 10;
	const int select_z_order = 1000;

	//英雄图标的位置
	const cocos2d::Point HeroIcoPos[max_battle_hero_num] = { cocos2d::Point(160, 660), cocos2d::Point(320, 660), cocos2d::Point(480, 660),
		cocos2d::Point(160, 440) , cocos2d::Point(320, 440) , cocos2d::Point(480, 440) };

	//英雄名字的位置
	const cocos2d::Point HeroNamePos[max_battle_hero_num] = { cocos2d::Point(160, 550), cocos2d::Point(320, 550), cocos2d::Point(480, 550),
		cocos2d::Point(160, 330) , cocos2d::Point(320, 330) , cocos2d::Point(480, 330) };

	//英雄名字到英雄图标的距离
	const cocos2d::Point distanceNameToIco = cocos2d::Point(0, 110);

private:
	//记录当前选中的图标逻辑标签号
	int _selectTag;

	DisplayLayer* _displayLayer;
	cocos2d::ui::Text* _nameLabel[max_battle_hero_num];
	cocos2d::Sprite* _heroIco[max_battle_hero_num];

};
  
#endif // TEAMLAYER_H_ 
