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
const int init_unique_num = 1000;			//唯一数的起始大小

const int none = -1;						//表示没有或者英雄装备位置没有装备或者表示装备没有使用者
const int max_equipment_num = 6;			//角色可以佩戴的装备数量（6种类型）
const int max_battle_hero_num = 6;			//最多能有6个英雄上阵

const int init_glod = 5000;					//玩家初始获得的金币
const int init_level = 1;					//玩家初始的等级
const int init_diamond = 1200;				//玩家初始获得的钻石

const int intensify_base_diamond = 25;		//强化装备和卡牌的时候需要的基础钻石
const int levelup_base_gold = 300;		//升级装备和强化的时候需要的基础金币

const int login_gold_base = 1000;		//每日奖励的基础奖励金币
const int login_diamond_base = 10;		//每日奖励的基础奖励钻石

const std::string goldName = "gold";		//用来保存金币数是的key
const std::string diamondName = "diamond";	//用来保存钻石数的key
const std::string levelName = "level";		//保存等级的key
const std::string expName = "exp";			//保存经验的key
const std::string uniqueNumName = "uniqueIdentifierNum"; //用来生成Id的唯一数

//自定义消息
const string msg_update_user_data = "MSG_UPDATE_USER_DATA";	//更新用户数据的消息名
const string msg_update_display_item = "MSG_UPDATE_DISPLAY_ITEM";	//用于强化或升级后更新DisplayLayer列表的item属性显示


//发送自定义消息时的参数
const int msg_update_display_item_parameter_update = 0; //升级或强化
const int msg_update_display_item_parameter_abandon = 1; //丢弃
const int msg_update_display_item_parameter_sell = 2; //丢弃

//出战英雄保存时的key
const std::string battleSaveKey[max_battle_hero_num] = { "0", "1", "2", "3", "4", "5" };

//每日登陆奖励
const std::string preGetTime = "pgt"; //上次领取奖励的时间

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
const std::string critDmg = "icos/critDmg.png";
const std::string blockIco = "icos/block.png";


//--------string.plist文件种的中文字符资源名称-------------------------------------------------------------------
const std::string level_d = "level_d";
const std::string intensify_d = "intensify_d";
const std::string star_d = "star_d";
const std::string speed_d = "speed_d";
const std::string block_d = "block_d";
const std::string hp_d = "hp_d";
const std::string critDmg_d = "critDmg_d";
const std::string crit_d = "crit_d";
const std::string ap_d = "ap_d";
const std::string mp_d = "mp_d";
const std::string define_d = "define_d";
const std::string mDefine_d = "mDefine_d";

//错误对话框标题和提示内容资源名称
const std::string error = "error";
const std::string errorContent = "errorContent";

const std::string levelupTitle = "levelupTitle";		//中文字符'升级'资源名称
const std::string intensifyTitle = "intensifyTitle";	//中文字符'强化'资源名称
const std::string levelTips = "levelTips";				//升级提示费用
const std::string intensifyTips = "intensifyTips";	//强化提示费用
const std::string notEnoughGold = "notEnoughGold";		//金币不够
const std::string notEnougMaterial = "notEnougMaterial";//材料不够
const std::string notEnougDiamond = "notEnoughDiamond";	//钻石不够
const std::string succed = "succed";					//成功
const std::string failed = "failed";					//失败


const std::string sellTitle = "sellTitle";					//出售对话框标题
const std::string abandonTitle = "abandonTitle";			//丢弃对话框标题
const std::string sellTips = "sell";					//出售对话框提示内容
const std::string abandonTips = "abandon";			//丢弃对框提示内容
const std::string sellSucced = "sellSucced";			//出售成功提示
const std::string abandonSucced = "abandonSucced";		//丢弃成功提示

//每日登陆奖励
const std::string loginRewardTips = "loginRewardTips"; //登陆奖励领取提示
const std::string loginRewardTimeTips1 = "loginRewardTimeTips1"; //还没到领取时间提示
const std::string loginRewardTimeTips2 = "loginRewardTimeTips2"; //到达领取时间提示
const std::string rewardGold = "rewardGold"; //奖励的金币字符串
const std::string rewardDiamond = "rewardDiamond"; //奖励的钻石数字符串


//---------------给控件设置的标签名字（node->setName()）-----------------------------------
const std::string blockTagName = "blockName";		//BlockLayer的标签名字，用来通过标签名字获取实例对象
const std::string tipsTagName = "tips";				//提示框的标签名字
const std::string dialogTagName = "dialog";			//模态对话框的标签名字

#endif // __CONSTANTDEFINE_H_
