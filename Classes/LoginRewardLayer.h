/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	LoginRewardLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/11
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef LOGINREWARDLAYER_H_  
#define LOGINREWARDLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"


class LoginRewardLayer : public cocos2d::Layer
{
public:
	LoginRewardLayer();
	~LoginRewardLayer();
	virtual bool init() override;
	CREATE_FUNC(LoginRewardLayer);

private:
	void loadUI();

	void initData();

	void startAnimation();
	void endAnimation();

	void onEnterTransitionDidFinish() override;

	void updateTime(float dt);

private:
	const std::string csbName = "layers/homeLayer/loginLayer.csb";
	const std::string timeLabelName = "timeLabel";
	const std::string contentLabelName = "contentLabel";
	const std::string btnCloseName = "btnClose";
	const std::string btnOkName = "btnOk";
	const unsigned int intervalTime = 43200;	//12小时

private:
	//记录还差多少时间可以领取奖励
	unsigned int _remainTime;

	cocos2d::ui::Button *_btnClose, *_btnOk;
	cocos2d::ui::Text *_timeTips, *_contentLabel;
};
  
#endif // LOGINREWARDLAYER_H_ 