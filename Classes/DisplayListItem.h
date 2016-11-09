/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DisplayListItem.h
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __DISPLAYLISTITEM_H__
#define __DISPLAYLISTITEM_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

enum ItemLabelType {
	ILT_LEVEL,
	ILT_NAME,
	ILT_ATTRIBUTE1,
	ILT_ATTRIBUTE2
};


class DisplayListItem : public cocos2d::ui::Layout {
public:
	DisplayListItem();
	~DisplayListItem();
	CREATE_FUNC(DisplayListItem);
	virtual bool init() override;

	//设置按钮回调
	void setBtnCallBack(std::function<void(cocos2d::Ref * pSender)> func);

	//设置按钮的标签
	void setBtnTag(const int tag);

	//设置按钮图片
	void setBtnTexture(const std::string& normal, const std::string& pressed);

	//设置对应类型的标签内容
	void setLabelText(ItemLabelType type, const std::string& content);

	//设置对应类型的标签颜色
	void setLabelColor(ItemLabelType type, cocos2d::Color4B color);

	//设置Item的图标
	void setIco(const std::string& icoName, const std::string& img1Name, const std::string& img2Name);

	//设置品质星级
	void setStarNum(const int n);

private:
	const std::string csbName = "layers/homeLayer/displayListItem.csb";
	const std::string icoName = "ico";
	const std::string levelLabelName = "levelLabel";
	const std::string nameLabelName = "nameLabel";
	const std::string starName = "star";
	const std::string img1Name = "img1";
	const std::string img2Name = "img2";
	const std::string text1Name = "attribute1";
	const std::string text2Name = "attribute2";
	const std::string btnName = "btn";

private:
	void loadUI();


private:
	cocos2d::ui::ImageView* _ico;		//物品图标
	cocos2d::ui::ImageView* _star;		//星级图标
	cocos2d::ui::ImageView* _img1;		//属性图标
	cocos2d::ui::ImageView* _img2;		//属性图标
	cocos2d::ui::Text*		_level;		//等级标签
	cocos2d::ui::Text*		_name;		//名字标签
	cocos2d::ui::Text*		_attribute1;//属性标签
	cocos2d::ui::Text*		_attribute2;//属性标签
	cocos2d::ui::Button*	_btn;		//按钮
};

#endif // DisplayListItem_H_
