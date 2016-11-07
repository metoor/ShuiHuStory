/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	HeroDisplayLayer.h
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/10/20
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __HERODISPLAYLAYER_H__
#define __HERODISPLAYLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

//前置声明
class ListItem;
struct HeroCardProperty;

class HeroDisplayLayer : public cocos2d::Layer{
public:
	HeroDisplayLayer();
	~HeroDisplayLayer();
	virtual bool init() override;
	CREATE_FUNC(HeroDisplayLayer);

private:
	void loadUI();

	//设置列表显示的当前已用数量和总得容量，如:30/50
	void setTipLabel();

	// 初始化5个Item
	void loadItem(int n = 5);

	//设置Item的属性
	void setItemAttribute(const HeroCardProperty* property, ListItem* item);
	
	//更新指定id的item的属性
	void updateItemAttribute(const int heroId, const int itenId);
	
	void setItemColor(ListItem* item, int star);

	//按钮回调方法
	void btnCloseCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	virtual void onEnterTransitionDidFinish() override;

	//进场动画
	void startAnimation();

	//结束动画，并清理资源
	void endAnimation();

private:
	const std::string csbName = "layers/homeLayer/heroDisplayLayer.csb";
	const std::string listViewName = "listView";
	const std::string tipLabelName = "tip";
	const std::string btnCloseName = "btnClose";
	const std::string blockName = "blockName";
	const int defaultLoadNum = 5;

private:
	//记录上一次详情按钮点击的索引
	int _preClicked;

	cocos2d::ui::ListView* _listView;
	cocos2d::ui::Button*	_btnClose;
	cocos2d::ui::Text*		_tipLabel;
	cocos2d::Vector<ListItem*> _itemVector;
};

#endif // HERODISPLAYLAYER_H_
