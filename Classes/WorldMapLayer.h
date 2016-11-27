/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	WorldMapLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/26
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef WORLDMAPLAYER_H_  
#define WORLDMAPLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

class WorldMapLayer : public cocos2d::Layer
{
public:
	WorldMapLayer();
	~WorldMapLayer();
	virtual bool init() override;
	CREATE_FUNC(WorldMapLayer);

private:
	void loadUI();
	
	void startAnimation();
	void endAnimation();

	void onEnter() override;

	//世界大关卡选择回调
	void menuCallFun(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

private:
	const std::string csbName = "layers/warLayer/worldMapLayer.csb";
	const std::string btnCancelName = "btnClose";
	const std::string scrollViewName = "scrollView";
	const int btnMenuEndTag = 20;

private:
	cocos2d::ui::Button* _btnClose;
	cocos2d::ui::ScrollView* _scrollView;

	std::function<void(int index)> _menuCallFunc;
};
  
#endif // WORLDMAPLAYER_H_ 
