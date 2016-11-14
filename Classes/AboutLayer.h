/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	AboutLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/14
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef ABOUTLAYER_H_  
#define ABOUTLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class AboutLayer : public cocos2d::Layer
{
public:
	AboutLayer();
	~AboutLayer();
	virtual bool init() override;
	CREATE_FUNC(AboutLayer);

private:
	void loadUI();

	void startAnimation();
	void endAnimation();
	void onEnterTransitionDidFinish() override;

private:
	const std::string csbName = "layers/homeLayer/aboutLayer.csb";
	const std::string btnCloseName = "btnClose";

private:
	cocos2d::ui::Button* _btnClose;
};
  
#endif // ABOUTLAYER_H_ 
