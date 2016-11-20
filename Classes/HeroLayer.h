/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	HeroLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/18
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef HEROLAYER_H_  
#define HEROLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ConstantDefine.h"

class DisplayLayer;

class HeroLayer : public cocos2d::Layer
{
public:
	HeroLayer();
	~HeroLayer();
	virtual bool init() override;
	CREATE_FUNC(HeroLayer);

private:
	void loadUI();

	void loadItem();
	
	//禁用已经上阵的英雄，如果上阵的英雄刚好是当前位置，则显示下阵
	void setEnableBtn(DisplayLayer* display);

	//换装备后更新数据
	void addEquipmentUpateEventListener();
	
	//英雄升级等更新数据
	void addHeroUpateEventListener();

	void onEnterTransitionDidFinish() override;

	void updateEnentListern(cocos2d::EventCustom* event);

private:
	const std::string csbName = "layers/heroLayer/heroLayer.csb";
	const std::string pageViewName = "pageView";
	const std::string btnChangeName = "btnChange";
	const std::string btnDetailsName = "btnDetails";
	const int displayLayerTag = 1432;

private:
	cocos2d::ui::Button *_btnChange, *_btnDetails;
	cocos2d::ui::PageView *_pageView;

	int _itemId[max_battle_hero_num];

};
  
#endif // HEROLAYER_H_ 
