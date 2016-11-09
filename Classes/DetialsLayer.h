/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DetialsLayer.h
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __DETIALSLAYER_H__
#define __DETIALSLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

//前置声明
struct EquipmentProperty;
struct HeroCardProperty;


enum DisplayType
{
	DT_EQUIPMENT,
	DT_HERO
};

//显示装备和英雄的详细信息
class DetialsLayer : public cocos2d::Layer
{
public:
	DetialsLayer();
	~DetialsLayer();
	virtual bool init() override;
	CREATE_FUNC(DetialsLayer);

	//通过装id初始化Ui数据
	void initUiWithId(int id, DisplayType type);

	//设置升级/强化按钮的回调
	void setBtnIntensifyCallBack(std::function<void(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> func);
	void setBtnLevelupCallBack(std::function<void(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> func);

private:
	void loadUI();

	//根据物品的属性创建描述字符
	std::string propertyToString(const EquipmentProperty* property);
	std::string propertyToString(const HeroCardProperty* property);

	//更新显示的数据
	void updateAttribute();

	//设置要显示的英雄或装备的名称
	void setName(const std::string& name);

	//根据星级设置名字的颜色
	void setNameColor(int star);

	//设置要显示的属性字符串内容
	void setAttributeString(const std::string& content);

	//设置英雄或装备图片
	void setIco(const std::string& icoName);

	void intensify();
	void levelup();

	//进场动画
	void startAnimation();

	//结束动画，并清理资源
	void endAnimation();

	void onEnterTransitionDidFinish() override;

	//按钮回调方法
	void btnCloseCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

private:
	const std::string csbName = "layers/homeLayer/detialsLayer.csb";
	const std::string icoName = "ico";
	const std::string nameLabelName = "nameLabel";
	const std::string attributeLabelName = "attributeLabel";
	const std::string btnIntensifyName = "btnIntensify";
	const std::string btnLevelupName = "btnLevelup";
	const std::string btnCloseName = "btnClose";

private:
	//九路显示的物品Id
	int _objectId;

	//记录显示的物品类型
	DisplayType _type;

	cocos2d::ui::ImageView* _ico;
	cocos2d::ui::Text* _nameLabel;
	cocos2d::ui::Text* _attributeLabel;
	cocos2d::ui::Button* _btnIntensify;
	cocos2d::ui::Button* _btnLevelup;
	cocos2d::ui::Button* _btnClose;

	std::function<void(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> _intensifyFun, _levelupFunc;
};

#endif // DETIALSLAYER_H_

