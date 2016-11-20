/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DisplayEquipmentByType.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/19
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef DISPLAYEQUIPMENTBYTYPE_H_  
#define DISPLAYEQUIPMENTBYTYPE_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"

//前置声明
class DisplayListItem;
class Equipment;
struct EquipmentProperty;

//装备的类型
enum ET
{
	ET_HAT = 1,
	ET_CLOTH,
	ET_WEAPON,
	ET_JADE,
	ET_SHOES,
	ET_HORSE
};

class DisplayEquipmentByType : public cocos2d::Layer
{
public:
	DisplayEquipmentByType();
	~DisplayEquipmentByType();
	virtual bool init() override;
	CREATE_FUNC(DisplayEquipmentByType);

	void setDisPlayEquipmentType(ET et, int heroid);

	//设置item里面的按钮点击事件回调
	void setBtnCallBack(std::function<void(cocos2d::Ref * pSender, int equipmentPos, int equipmentId, bool isAdd)> func);

private:
	void loadUI();

	void loadItem();

	void setItemColor(DisplayListItem* item, int star);

	//不知道原因是什么onEnterTransitionDidFinish在这里不会被调用因此改为onEnter
	void onEnter() override;
	
	void setItemAttribute(const EquipmentProperty* property, DisplayListItem* item);

	//按钮回调方法
	void btnCloseCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	//进场动画
	void startAnimation();

	//结束动画，并清理资源
	void endAnimation();

private:
	const std::string csbName = "layers/homeLayer/displayLayer.csb";
	const std::string listViewName = "listView";
	const std::string tipLabelName = "tip";
	const std::string btnCloseName = "btnClose";
private:
	//记录点击按钮点击的索引
	int _clickedIndex;
	//要装备装备的英雄id
	int _heroId;

	//示的类型
	ET _et;

	cocos2d::ui::ListView*	_listView;
	cocos2d::ui::Button*	_btnClose;
	cocos2d::ui::Text*		_tipLabel;

	//保存item按钮的点击事件回调
	std::function<void(cocos2d::Ref * pSender, int equipmentPos, int equipmentId, bool isAdd)> _func;

	//容器和listView对应，比如：_equipmendIdVector[0], 保存的是_listview中第一个item对应的装备唯一Id
	std::vector<int> _objectIdVector;
};
  
#endif // DISPLAYEQUIPMENTBYTYPE_H_ 
