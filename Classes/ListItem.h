/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	ListItem.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __LISTITEM_H__
#define __LISTITEM_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

enum ItemLabelType{
	LEVEL,
	NAME,
	TEXT1,
	TEXT2
};


class ListItem : public cocos2d::ui::Layout{
public:
	ListItem();
	~ListItem();
	CREATE_FUNC(ListItem);
	virtual bool init() override;

	//设置按钮回调
	void setBtnCallBack(std::function<void(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)> func);

	//设置按钮的标签
	void setBtnTag(const int tag);

	//设置按钮图片
	void setBtnTexture(const std::string normal, const std::string pressed);

	//设置对应类型的标签内容
	void setLabelText(ItemLabelType type, const std::string content);
	
	//设置对应类型的标签颜色
	void setLabelColor(ItemLabelType type, cocos2d::Color4B color);
	
	//设置Item的图标
	void setIco(const std::string icoName, const std::string img1Name, const std::string img2Name);
	
	//设置品质星级
	void setStarNum(const int n);

private:
	const std::string csbName = "layers/homeLayer/listItem.csb";
	const std::string icoName = "ico";
	const std::string levelLabelName = "levelLabel";
	const std::string nameLabelName = "nameLabel";
	const std::string starName = "star";
	const std::string img1Name = "img1";
	const std::string img2Name = "img2";
	const std::string text1Name = "text1";
	const std::string text2Name = "text2";
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
	cocos2d::ui::Text*		_text1;		//属性标签
	cocos2d::ui::Text*		_text2;		//属性标签
	cocos2d::ui::Button*	_btn;		//按钮
};

#endif // LISTITEM_H_
