/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	StoreLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/24
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef STORELAYER_H_  
#define STORELAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"

//前置声明
class DisplayListItem;
class HeroCard;
class Equipment;
struct EquipmentProperty;
struct HeroCardProperty;
struct SaveEquipmentData;
struct SaveHeroCardData;


class StoreLayer : public cocos2d::Layer
{
public:
	StoreLayer();
	~StoreLayer();
	virtual bool init() override;
	CREATE_FUNC(StoreLayer);

private:
	void loadUI();

	int getRandomHeroType();
	int getRandomEquipment();

	//保存装备和英雄数据
	void saveEquipment();
	void saveHeroCard();

	//获得保存的装备和英雄数据
	void getSaveEquipment(SaveEquipmentData& sd);
	void getSaveHeroCard(SaveHeroCardData& sd);

	//创建动态的item（装备和英雄），如果不给定参数或者给定任意一个，则表示不需要更新数据，加载保存的数据
	void createRandomItem(int heroType = -1, int equipmentType = -1);

	//设置Item的属性
	void setItemAttribute(const HeroCardProperty* property, DisplayListItem* item);

	//设置Item的属性
	void setItemAttribute(const EquipmentProperty* property, DisplayListItem* item);

	void setItemColor(DisplayListItem* item, int star);

	//显示玩家当前拥有的钻石数
	void updateDiamond();

	//显示玩家当前拥有的金币数
	void updateGold();

	//listView默认三个item里面的按钮回调
	void btnOkCallBack(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);

	void onEnter() override;

private:
	const std::string csbName = "layers/storeLayer/storeLayer.csb";
	const std::string goldLabelName = "gold";
	const std::string diamondLabelName = "diamond";
	const std::string listViewName = "listView";
	const std::string itemName = "item_%d";
	const std::string btnOkName = "btnOk";

	//保存商店更新的时间key
	const std::string preTimeKey = "pst";

	//保存玩家是否购买过此时段装备
	const std::string isBuyHero = "ibys";
	const std::string isBuyEquipment = "ibes";

	//保存装备数据的key
	const std::string eId = "eds";
	const std::string eStar = "ess";
	const std::string eType = "ets";
	const std::string eRate = "ers";

	//保存英雄数据的key
	const std::string hId = "hds";
	const std::string hStar = "hss";
	const std::string hType = "hts";
	const std::string hRate = "hrs";

	//商店界面的常量定义
	const int tian_d = 300; //招募天罡需要消耗的钻石
	const int di_d = 120;	//招募地煞需要消耗的钻石
	const int gold = 10000;	//购买金币时获得的金币数
	const int gold_d = 10;	//购买金币时消耗的钻石
	const int diamond = 100;	//购买钻石时获得的钻石数
	const int diamond_g = 120000;	//购买钻石时消耗的金币数

	const unsigned int intervalTime = 21600;	//6小时
	const int numCount = 2;	//动态创建的随机物品总数

private:
	HeroCard* _hero;
	Equipment* _equipment;

	bool _isBuyHero;
	bool _isBuyEquipment;

	cocos2d::ui::ListView* _listView;
	cocos2d::ui::Text *_goldLabel, *_diamondLabel;
};
  
#endif // STORELAYER_H_ 
