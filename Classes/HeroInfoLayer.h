/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	HeroInfoLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/22
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef HEROINFOLAYER_H_  
#define HEROINFOLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define ARRAY_SIZE 14

enum TextType{
	TT_NAME,
	TT_STAR,
	TT_HP,
	TT_AP,
	TT_MP,
	TT_CRIT,
	TT_BLOCK,
	TT_SPEED,
	TT_DEFIME,
	TT_MDEFIME,
	TT_CRITDMG,
	TT_SKILL,
	TT_CONTENT,
	TT_STARNAME
};

class HeroInfoLayer : public cocos2d::Layer
{
public:
	HeroInfoLayer();
	~HeroInfoLayer();
	virtual bool init() override;
	CREATE_FUNC(HeroInfoLayer);

	void initWithHerotype(const int type);

private:
	void loadUI();

	void setString(TextType type,  const std::string& str);
	void setString(TextType type, int  num);

	//初始化成员数组成nullpter
	void initArrayToNulptr();

	void onEnter() override;

	void startAnimation();
	void endAnimation();

private:
	const std::string csbName = "layers/pictureLayer/heroInfoLayer.csb";
	const int textStartTag = 0;
	const int startEndTag = 14;
	const int heroImgTag = 14;
	const int btnCloseTag = 15;
	const int scrollViewTag = 16;

private:
	cocos2d::ui::Text* _textList[ARRAY_SIZE];
	cocos2d::ui::Button* _btnClose;
	cocos2d::ui::ImageView* _heroImg;
	cocos2d::ui::ScrollView* _scrollView;
};
  
#endif // HEROINFOLAYER_H_ 
