/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	AudioLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/13
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef AUDIOLAYER_H_  
#define AUDIOLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"


class AudioLayer : public cocos2d::Layer
{
public:
	AudioLayer();
	~AudioLayer();
	virtual bool init() override;
	CREATE_FUNC(AudioLayer);

private:
	void loadUI();

	//设置根据保存的数据设置按钮图标初始状态
	void initBtnIco();

	//音乐按钮点击逻辑处理。如果当前音乐可用，则点击后禁用。 不可用，则点击后可用
	void music();
	
	//音效按钮点击处理。如果当前音乐可用，则点击后禁用。不可用，则点击后可用
	void effect();

	void startAnimation();
	void endAnimation();
	void onEnterTransitionDidFinish() override;

private:
	const std::string csbName = "layers/homeLayer/audioLayer.csb";
	const std::string btnCloseName = "btnClose";
	const std::string btnMusicName = "btnMusic";
	const std::string btnEffectName = "btnEffect";

private:
	cocos2d::ui::Button *_btnClose, *_btnMusic, *_btnEffect;
};
  
#endif // AUDIOLAYER_H_ 
