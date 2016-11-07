/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	Config.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/10/20
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <vector>
#include "TypeStruct.h"

class Config{
public:
	~Config();

	static Config* getInstance();


	//通过type获取HeroType
	const HeroCardType* getHeroCardByIndex(int type);

	//通过type或取EquipmentType-（需要根据type<101>转换成相应的索引）
	const EquipmentType* getEquipmentByIndex(int type);

private:
	Config();

	//销毁模Vector里面的元数
	void destoryHeroVectorElement();
	void destoryEquipmentVectorElement();

	//从Json文件中读取英雄模板表数据
	void readHeroJson(string fileName);

	//从Json文件中读取装备模板表数据
	void readEquipmentJson(string fileName);

private:
	//保存英雄模板[0~108]
	std::vector<HeroCardType*> _heroCardVector;

	//保存装备模板
	std::vector<EquipmentType*> _equipmentVector;

	//单例
	static Config* _config;
};

#endif // CONFIG_H_
