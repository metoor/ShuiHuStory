/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	WarMenuLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/26
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef WARMENULAYER_H_  
#define WARMENULAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"

class WarMenuLayer : public cocos2d::Layer
{
public:
	WarMenuLayer();
	~WarMenuLayer();
	virtual bool init() override;
	CREATE_FUNC(WarMenuLayer);

private:
	void loadUI();

private:
	const std::string csbName = "layers/warLayer/warMenuLayer.csb";
	const std::string btnLocalName = "btnLocal";
	const std::string btnPvpName = "btnPvp";

private:
	cocos2d::ui::Button *_btnLocal, *_btnPvp;

	//保存战役模式的背景图片,用来在退出本场景的时候移除纹理，释放空间
	cocos2d::Texture2D* _texture;
};
  
#endif // WARMENULAYER_H_ 
