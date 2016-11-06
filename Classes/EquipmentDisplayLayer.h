#ifndef __EQUIPMENTDISPLAYLAYER_H__
#define __EQUIPMENTDISPLAYLAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>

//前置声明
class ListItem;
struct EquipmentProperty;

class EquipmentDisplayLayer : public cocos2d::Layer{
public:
	EquipmentDisplayLayer();
	~EquipmentDisplayLayer();
	CREATE_FUNC(EquipmentDisplayLayer);
	virtual bool init() override;


private:
	void loadUI();
	
	//设置Item的属性
	void setItemAttribute(const EquipmentProperty* property, ListItem* item);

	//更新指定id的item的属性
	void updateItemAttribute(const int equipmentId, const int itenId);

	//初始化5个Item
	void loadItem(int n = 5);

	void setItemColor(ListItem* item, int star);

	virtual void onEnterTransitionDidFinish() override;

	//按钮回调方法
	void btnCloseCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	//进场动画
	void startAnimation();

	//结束动画，并清理资源
	void endAnimation();

private:
	const std::string csbName = "layers/homeLayer/equipmentDisplayLayer.csb";
	const std::string listViewName = "listView";
	const std::string btnCloseName = "btnClose";
	const std::string blockName = "blockName";
	const int defaultLoadNum = 5;

private:
	//记录上一次详情按钮点击的索引
	int _preClicked;

	cocos2d::ui::ListView* _listView;
	cocos2d::ui::Button*	_btnClose;
	cocos2d::Vector<ListItem*> _itemVector;
};

#endif // EQUIPMENTDISPLAYLAYER_H_
