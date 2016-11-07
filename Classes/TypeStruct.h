/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	TypeStruct.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __TYPESTRUCT_H__
#define __TYPESTRUCT_H__

#include "string"
#include "ConstantDefine.h"

using std::string;


//用来保存装备信息到文件的结构，为了方便保存，所有浮点型都会转换为整型保存
struct SaveEquipmentData
{
	int id;				//唯一标识符
	int type;			//初始化的模板编号
	int level;			//等级
	int exLevel;		//强化等级
	int user;			//使用者
	int rate;			//相对于模板的属性比率，例如：1.2表示所有属性为模板属性的1.2倍
	int star;			//星级
};

//装备类属性结构
struct EquipmentProperty
{
	int id;				//唯一标识符
	int type;			//初始化的模板编号
	int level;			//等级
	int exLevel;		//强化等级
	int user;			//使用者
	float rate;			//相对于模板的属性比率，例如：1.2表示所有属性为模板属性的1.2倍
	int star;			//星级

	int defend;		//防御
	int magicDefend;	//魔法防御
	int ap;				//物理攻击
	int mp;				//魔法攻击
	int hp;				//生命值
	int speed;			//攻击时的优先级，值越大优先级越高
	float critRate;		//暴击
	float blockRate;		//格挡
	float critDamage;	//暴击伤害加成
	
	int buyMoney;		//购买的金币
	int sellMoney;		//卖出的金币

	const string * name;		//名字
	const string * textureName;	//纹理名字
	const string * des;			//描述

};

//装备模板属性结构
struct EquipmentType
{
	string name;		//名字
	int type;			//初始化的模板编号
	int defend;		//防御
	int magicDefend;	//魔法防御
	int ap;				//物理攻击
	int mp;				//魔法攻击
	int hp;				//生命值
	int speed;			//攻击时的优先级，值越大优先级越高
	int star;			//星级
	int critRate;		//暴击
	int blockRate;		//格挡
	int buyMoney;		//购买的金币
	int sellMoney;		//卖出的金币
	int critDamage;		//暴击伤害加成
	string textureName;	//纹理名字
	string des;			//描述
};

struct HeroCardType
{
	int type;				//类型
	int star;				//品质
	int hp;					//卡牌的血
	int ap;					//卡牌物理攻击
	int mp;					//卡牌魔法攻击
	int defend;				//卡牌的物理防御
	int magicDefend;		//卡牌的魔法防御
	int attackId;			//普通攻击方式
	int skillId;			//技能攻击方式
	int speed;				//速度
	int critRate;			//暴击
	int blockRate;			//格挡
	int critDamage;			//暴击伤害加成
	int buyMoney;			//购买的金币
	int sellMoney;			//卖出的金币
	bool isMagic;			//是否魔法攻击

	string textureName;		//纹理
	string name;			//名字
	string exName;			//别名
	string starName;		//星君名字
	string skillName;		//技能名称
	string des;
};

//用来保存英雄信息到文件的结构，为了方便保存，所有浮点型都会转换为整型保存。例如1.2->120
struct SaveHeroCardData
{
	int id;				//唯一标识符
	int type;			//初始化的模板编号
	int level;			//等级
	int exLevel;		//强化等级
	int rate;			//相对于模板的属性比率，例如：120表示所有属性为模板属性的1.2倍
	int star;			//星级
	
	//装备情况
	int equipmentId[max_equipment_num];	//-1表示没有佩戴该类型的装备，大于1000表示装备Id
};

struct HeroCardProperty
{
	int id;				//唯一标识符
	int type;			//初始化的模板编号
	int level;			//等级
	int exLevel;		//强化等级
	float rate;			//相对于模板的属性比率，例如：1.2表示所有属性为模板属性的1.2倍
	int star;			//品质/星级

	const string* textureName;		//纹理
	const string* name;				//名字
	const string* exName;			//别名
	const string* starName;			//星君名字
	const string* skillName;		//技能名称
	const string* des;				//描述

	int hp;					//卡牌的血
	int ap;					//卡牌物理攻击
	int mp;					//卡牌物理攻击
	int defend;				//卡牌的物理防御
	int magicDefend;		//卡牌的魔法防御
	int attackId;			//普通攻击方式
	int skillId;			//技能攻击方式
	int speed;				//速度
	float critRate;			//暴击
	float blockRate;			//格挡
	float critDamage;		//暴击伤害加成
	int buyMoney;		//购买的金币
	int sellMoney;		//卖出的金币
	bool isMagic;			//是否魔法攻击
	

	int equipmentId[max_equipment_num];	//-1表示没有佩戴该类型的装备，大于1000表示装备Id

};

struct EnemyType {
	int type;
	string textureName;
	string name;
	string exname;
	string starname;
	string skillsname;
	int hp;
	int ap;
	int attackId;
	int skillId;
	int level;
	int magic;
	string des;
	int defend;
	int magicDefend;
};


struct MapData
{
	int startData[6];
	string name;
	int coin;
	int exp;
	int type;
	int chalevel;
	int card;
	int sword;

	string cardName;
	string cardTexture;
	string equipmentName;
	string equipmentTexture;
};

#endif // TYPESTRUCT_H_
