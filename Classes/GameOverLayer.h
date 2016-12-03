/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	GameOverLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/02
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef GAMEOVERLAYER_H_  
#define GAMEOVERLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"

enum ExtraRewardType
{
	ERT_NONE,
	ERT_CARD,
	ERT_EQUIPMENT
};

class GameOverLayer : public cocos2d::Layer
{
public:
	GameOverLayer();
	~GameOverLayer();
	virtual bool init() override;
	CREATE_FUNC(GameOverLayer);

	void initLayer(bool isWin, ExtraRewardType rewardType, int type, int gold, int exp);

private:

	void failed();
	void win();

	void loadUI();

	//创建额外的奖励，如果有额外奖励返回奖励的id，否则返回none
	std::string createExtraReward();

	void onEnter() override;
	
	void startAnimation();
	void endAnimation();

private:
	const std::string csbName = "layers/warLayer/gameOverLayer.csb";
	const std::string btnOkName = "btnOk";
	const std::string expName = "exp";
	const std::string goldName = "gold";
	const std::string icoName = "ico";
	const std::string icoBgName = "icobg";

	//获得额外奖励的概率十分之一
	const int extraRewardProbability = 10;

private:
	int _gold;
	int _exp;
	int _type;
	bool _isWin;
	ExtraRewardType _rewardType;

	cocos2d::ui::Text *_expLabel, *_goldLabel;
	cocos2d::ui::Button* _btnOk;
	cocos2d::ui::ImageView *_ico, *_icoBg;
};
  
#endif // GAMEOVERLAYER_H_ 
