/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	ConstantDefine.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/10/20
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __CONSTANTDEFINE_H_
#define __CONSTANTDEFINE_H_

#include "string"

using std::string;


//------------用于添加层是的zorder排序-----------------------------------------------

//游戏Layer的Zorder，决定了游戏显示的先后顺序，值越大则显示在越上层
const int max_z_order = 9999;
const int particle_z_order = 9990;
const int dialog_z_order = 9980;
const int menu_z_order = 9970;
const int tip_z_order = 9960;


//-----------------------------------------------------

const std::string save_hero_file_name = "myHeroCard.json";			//用来保存玩家获得英雄卡牌数据的文件名称
const std::string save_equipment_file_name = "myEquipment.json";	//用来保存玩家获得装备数据的文件名称
const std::string equipment_file_name = "datas/equipment.data";		//用来生成装备的基础数据的文件名称
const std::string hero_file_name = "datas/hero.data";				//用来生成英雄卡牌基础数据的文件名称

const int equipment_map_max_size = 50;		//玩家拥有的最大装备数
const int hero_card_map_max_size = 50;		//玩家拥有英雄卡牌最大张数

const int max_gold = 1000000;				//玩家能同时拥有的最大金币数
const int max_diamond = 10000;				//玩家能同时拥有的最大专事数

const int max_level = 100;					//玩家最大等级
const int start_exp = 300;					//玩家最开始获得的经验值
const float start_exp_rate = 1.1f;			//计算当前等级升级所需经验时的比率

const int none = -1;						//表示没有或者英雄装备位置没有装备或者表示装备没有使用者
const int max_equipment_num = 6;			//角色可以佩戴的装备数量（6种类型）
const int max_battle_hero_num = 6;			//最多能有6个英雄上阵


const std::string goldName = "gold";		//用来保存金币数是的key
const std::string diamondName = "diamond";	//用来保存钻石数的key
const std::string levelName = "level";		//保存等级的key
const std::string expName = "exp";			//保存经验的key


//-------------------------------------------------------------

//详细按钮的纹理图片名称
const std::string btnDetails1 = "btns/details1.png";
const std::string btnDetails2 = "btns/details2.png";

//属性图标，如：防御，魔防，生命等....
const std::string hpIco = "icos/hp.png";
const std::string apIco = "icos/ap.png";
const std::string mpIco = "icos/mp.png";
const std::string defineIco = "icos/define.png";
const std::string mDefineIco = "icos/mdefine.png";
const std::string speedIco = "icos/speed.png";
const std::string critIco = "icos/crit.png";
const std::string blockIco = "icos/block.png";


//---------------给控件设置的标签名字（node->setName()）-----------------------------------
const std::string blockTagName = "blockName";		//BlockLayer的标签名字，用来通过标签名字获取实例对象


#endif // __CONSTANTDEFINE_H_
