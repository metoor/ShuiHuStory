/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	PVPStartLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/03
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef PVPSTARTLAYER_H_  
#define PVPSTARTLAYER_H_  

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "TypeStruct.h"

class PVPStartLayer : public cocos2d::Layer
{
public:
	PVPStartLayer();
	~PVPStartLayer();
	virtual bool init() override;
	CREATE_FUNC(PVPStartLayer);

private:
	void loadUI();

	void ballRunAction();
	void stopBallAction();

	//获得我方上阵的英雄数据（json）
	std::string getBattleHeroData();

	//显示文本
	void disPlayTips(const std::string &content, cocos2d::Color4B color = cocos2d::Color4B::MAGENTA);

	//添加创建战斗场景的消息
	void addCreteSceneMessageListener();

	//添加连接断开消息
	void addDisconnectMessageListener();

	//添加socket连接消息
	void addConnectMessageListener();

	void onEnter() override;

	void startAnimation();
	void endAnimation();

private:
	const std::string csbName = "layers/warLayer/pvpLayer.csb";
	const std::string btnCloseName = "btnClose";
	const std::string btnStartName = "btnStart";
	const std::string ballName = "ball";
	const std::string tipsName = "tips";

	const int actionTag = 1236;	//小球的动画标签
	const int socketLayerTag = 3216;	//发送数据给服务器的层的标签
	const int base_gold = 1000;	//匹配基础消耗金币

	const int ball_start_pos_x = 100;
	const int ball_end_pos_x = 550;
	const int ball_pos_y = 620;

private:
	//开启比赛需要的金币
	int _gold;

	//是否是开始匹配按钮
	bool _isStartBtn;

	//保存从服务器获取的数据
	PVPData _pvpData;

	cocos2d::ActionInterval* _action;
	cocos2d::ui::Text* _tips;
	cocos2d::Sprite* _ball;
	cocos2d::ui::Button* _btnClose, *_btnStart;
};
  
#endif // PVPSTARTLAYER_H_ 