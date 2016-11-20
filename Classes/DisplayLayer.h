/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DisplayLayer.h
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __DISPLAYLAYER_H__
#define __DISPLAYLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include <functional>
#include <unordered_map>

enum ObjectType
{
	OT_NONE,
	OT_EQUIPMENT,
	OT_HERO,
};

//前置声明
class DisplayListItem;
class Equipment;
class HeroCard;
struct EquipmentProperty;
struct HeroCardProperty;


class DisplayLayer : public cocos2d::Layer {
public:
	DisplayLayer();
	~DisplayLayer();
	CREATE_FUNC(DisplayLayer);
	virtual bool init() override;

	int getObjectIdByIndex(int index);

	std::vector<int>* getObjectIdVector();

	DisplayListItem* getObjectItemByIndex(int index);

	//设置要显示的类型，显示装备或英雄卡牌
	void setDisplayType(ObjectType type);

	//设置item里面的按钮点击事件回调
	void setBtnCallBack(std::function<void(cocos2d::Ref * pSender)> func);

	//设置item里面按钮的图片
	void setBtnTexture(const std::string& normal, const std::string& pressed);

	//结束动画，并清理资源
	void endAnimation();

private:
	void loadUI();

	void load();

	//通过map来生成item
	void loadItem(const std::unordered_map<int, Equipment*>* equipmentMap);
	void loadItem(const std::unordered_map<int, HeroCard*>* heroMap);

	//设置列表显示的当前已用数量和总得容量，如:30/50
	void setTipLabel();

	//注册更新列表某一项的数据
	void addUpdateDisplayItemEventListener();

	//删除item
	void deleteItem();

	//设置Item的属性
	void setItemAttribute(const HeroCardProperty* property, DisplayListItem* item);

	//设置Item的属性
	void setItemAttribute(const EquipmentProperty* property, DisplayListItem* item);

	//更新指定物品(装备或英雄)id的item的属性
	void updateItemAttribute();

	void setItemColor(DisplayListItem* item, int star);

	virtual void onEnter() override;

	//按钮回调方法
	void btnCloseCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	//进场动画
	void startAnimation();

private:
	const std::string csbName = "layers/homeLayer/displayLayer.csb";
	const std::string listViewName = "listView";
	const std::string tipLabelName = "tip";
	const std::string btnCloseName = "btnClose";

private:
	//是否需要更新用户数据
	bool _isNeedUpadateUserData;

	//记录点击按钮点击的索引
	int _clickedIndex;

	//保存要显示的物品类型
	ObjectType _type;

	//保存item里面的按钮纹理图片
	std::string _normalTexture;
	std::string _pressedTexture;

	cocos2d::ui::ListView*	_listView;
	cocos2d::ui::Button*	_btnClose;
	cocos2d::ui::Text*		_tipLabel;

	//容器和listView对应，比如：_equipmendIdVector[0], 保存的是_listview中第一个item对应的装备唯一Id
	std::vector<int> _objectIdVector;

	//保存item按钮的点击事件回调
	std::function<void(cocos2d::Ref * pSender)> _func;
};

#endif // DISPLAYLAYER_H_
