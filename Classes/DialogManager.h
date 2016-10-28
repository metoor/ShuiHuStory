#ifndef __DIALOGMANAGER_H__
#define __DIALOGMANAGER_H__

#include "cocos2d.h"

class DialogManager{
public:
	~DialogManager();

	//获得实例
	static DialogManager* getInstance();

	//显示一个提示框
	void showTipDialog(std::string text, float second = 3.0f, cocos2d::Color4B color = cocos2d::Color4B(255, 0, 0, 255), int size = 35);

	//销毁实例
	void destoryInstance();

private:
	DialogManager();

	//创建一个提示对话框
	cocos2d::Node* createTip(std::string text, cocos2d::Color4B color, int size);

private:
	//常量名称
	const std::string csbName_TipLayer = "layers/Tools/tipDialog.csb";

private:
	static DialogManager* _instance;
};

#endif // DIALOGMANAGER_H_
