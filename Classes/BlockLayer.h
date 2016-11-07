/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	BlockLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	 create by vs2015pro
*************************************************/

#include "cocos2d.h"

//´¥ÃþÍÌÊÉ²ã£¬·ÀÖ¹µã»÷ÊÂ¼þ´©Í¸
class BlockLayer : public cocos2d::LayerColor
{
public:
	CREATE_FUNC(BlockLayer);

	bool init() override
	{
		if (!cocos2d::Layer::init())
			return false;

		initWithColor(cocos2d::Color4B(168, 168, 168, 0));

		auto listener = cocos2d::EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(BlockLayer::onTouchBegan, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		return true;
	}

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override
	{
		return true;
	}

};
