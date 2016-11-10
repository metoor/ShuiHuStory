/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	Dialog.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/09
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef DIALOG_H_  
#define DIALOG_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <functional>

class Dialog : public cocos2d::Layer
{
public:
	Dialog();
	~Dialog();
	virtual bool init() override;
	CREATE_FUNC(Dialog);


	//设置显示文本的颜色
	void setContentColor(cocos2d::Color4B color);

	//设置是否显示取消按钮
	void setCanceBtnlDisplay(bool isDisplay);

	//设置显示文本
	void setContent(const std::string & content);

	//设置显示文本
	void setFontSize(int contentTextSize);

	//设置对话框的标题
	void setTitle(const std::string& title);

	//设置确认按钮回调
	void setBtnOkCallBack(std::function<void(cocos2d::Ref * pSender)> func);

	//三个参数的初始化，方便创建
	void initDialog(const std::string & title, const std::string & content, std::function<void(cocos2d::Ref * pSender)> func);

private:
	void loadUI();

	void startAnimation();

	void onEnterTransitionDidFinish() override;

	//移除动画并清理资源
	void endAnimation();

private:
	const std::string csbName = "layers/Tools/dialogLayer.csb";
	const std::string btnCloseName = "btnClose";
	const std::string btnOkName = "btnOk";
	const std::string titleName = "title";
	const std::string contentName = "content";

private:
	cocos2d::ui::Button* _btnClose;
	cocos2d::ui::Button* _btnOk;
	cocos2d::ui::Text* _title;
	cocos2d::ui::Text* _content;
};
  
#endif // DIALOG_H_ 
