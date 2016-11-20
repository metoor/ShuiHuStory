/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	Equipment.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __EQUIPMENT_H__
#define __EQUIPMENT_H__

#include "TypeStruct.h"

class Equipment
{
public:
	Equipment();

	~Equipment();
	
	void setUserId(int id);

	//根据config中的EquipmentVector的索引创建装备
	void init(const int index);

	//根据模板生成新的装备，参数为初始装备的模板
	void init(const EquipmentType* equipmentType);

	//生成已有的装备模板生成装备，参数为保存的装备数据
	void init(const SaveEquipmentData* saveEquipmentData);

	//获得装备需要保存的数据
	const SaveEquipmentData getSaveData();

	//获得装备属性
	const EquipmentProperty* getProperty();

	//升级和强化装备
	void levelup();
	void intensify();

	//计算装备所有加成之后的属性，参数为初始装备的模板
	void calculatePropery(const EquipmentType* equipmentType);

private:
	//装备的所有属性信息
	EquipmentProperty* _property;
};

#endif // EQUIPMENT_H_
