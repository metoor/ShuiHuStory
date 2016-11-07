/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	HeroCard.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/
 
#ifndef __HEROCARD_H__
#define __HEROCARD_H__

#include "TypeStruct.h"

class HeroCard{
public:
	HeroCard();
	~HeroCard();

	//根据config中的HeroVector的索引创建英雄卡牌
	void init(const int index);

	//根据模板生成新的英雄卡牌，参数为初始英雄卡牌的模板
	void init(const HeroCardType* heroCardType);

	//生成英雄卡牌，参数为保存的英雄卡牌数据
	void init(const SaveHeroCardData* saveHeroCardData);

	//获得英雄卡牌需要保存的数据
	const SaveHeroCardData getSaveData();

	//获得英雄卡牌属性
	const HeroCardProperty* getProperty();

	//穿戴装备(int pos(穿戴的位置【0~5】), int equipmentId（要穿上的装备Id）);
	//0：佩戴，1：帽子，2：鞋子，3：衣服，4：坐骑，5：武器
	void wearEquipment(int pos, int equipmentId);

	//通过位置序号【0~5】获得该位置装备的装备的Id
	int getEquipmentByPos(int pos);

	//更新卡牌的属性，用于在升级过/强化过卡牌或装备时调用
	void updatetePropery();

private:
	//计算卡牌英雄强化和升级的加成属性并更新属性
	void calculateHeroCardPropery(const HeroCardType * heroCardType);

	//计算英雄卡牌佩戴的装备加成属性并更新英雄卡牌属性
	void calculateEquipmentPropery();

private:
	//英雄卡牌的所有属性信息
	HeroCardProperty* _property;
};

#endif // HEROCARD_H_