/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	GameData.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/10/20
// Contact: 	caiufen@qq.com
// Description:		create by vs2015pro
*************************************************/


#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include <unordered_map>
#include "ConstantDefine.h"

class Equipment;
class HeroCard;

class GameData{
public:
	~GameData();

	//获得实例
	static GameData* getInstance();
	
	static bool isNullptr();

	//加载数据
	void readData();

	//释放加载的数据
	void saveData();

	//设置用户数据，金币，钻石，经验 ,等级，如果设置成功返回true，失败则返回false，
	bool setGold(const int gold);
	bool setDiamond(const int diamond);
	//如果角色达到最大等级则返回false
	bool setExp(const int exp);
	
	//获得用户数据方法
	int getGold();
	int getDiamond();
	int getExp();
	int getLevel();

	//获得级别level升级所需要的经验值，X是比例系数
	int getExpLimit(float x, int level);

	//保存用户数据 金币，钻石，经验 ,等级
	void saveUserData();
	
	//读取用户数据 金币，钻石，经验 ,等级
	void readUserData();
 
	//销毁实例
	void destoryInstance();

	//获取唯一的标识号
	int getUniqueIdentifierNum();

	//保存当前已经使用到的标识号
	void saveUniqueIdentifierNumToFile();

	//读取文件中的标识号
	void readUniqueIdentifierNumFromFile();

	//读取出战英雄
	void readBattleHero();

	//保存出战英雄
	void saveBattleHero();

	//从指定文件中获得已获得的装备数据
	void readEquipmentFromJson(std::string fileName);

	//从指定文件中获得已获得的英雄数据
	void readHeroCardFromJson(std::string fileName);

	//将已获得的装备数据写指定文件入文件
	void saveEquipmentToFile(std::string fileName);

	//将已获得英雄卡牌数据写入指定文件
	void saveHeroCardToFile(std::string fileName);

	//保存已获得装备，是对saveEquipmentToFile的进一步封装
	void saveEquipment();

	//保存已获得装备，是对saveHeroCardToFile的进一步封装
	void saveHeroCard();
	
	//读取已获得的装备， 是对readEquipmentFromJson的进一步封装
	void readEquipment();

	//读取已获得英雄卡牌， 是对readHeroCardFromJson的进一步封装
	void readHeroCard();

	//添加到一个元素到_quipmentMap
	void addEquipmentToMap(Equipment* equipment);
	
	//添加一个元数到_heroCardMap
	void addHeroCardToMap(HeroCard* heroCard);

	//通过id删除玩家已有列表中英雄卡牌
	void deleteHeroCardById(int id);

	//通过id玩家已有列表中的删除装备
	void deleteEquipmentById(int id);

	//通过Id获取已经获得的装备
	Equipment* getEquipmentById(int id);

	//通过Id获取已经获得的英雄卡牌
	HeroCard* getHeroCardById(int id);

	//获取对应位置的出战英雄id
	int getBattleHeroId(int pos);

	//获得出战英雄数组
	int* getBattleHeroArray();

	//如果英雄已经上阵，则下阵，返回true，否则返回false
	bool unbattleHero(int id);

	//判断id是否出战
	bool isBattleHero(int id);

	//返回上阵英雄的个数
	int getBattleHeroNum();

	void setBattleHero(int pos, int id);

	//获得装备容器
	const std::unordered_map<int, Equipment*>* getEquipmentMap();

	//获得英雄卡牌容器
	const std::unordered_map<int, HeroCard*>* getHeroCardMap();

private:
	GameData();

	//销毁_equipmentMap里面的元数
	void destoryEquipmentMapElement();
	
	//销毁_heroCardMap里面的元数
	void destoryHeroMapElement();

private:
	//用户数据
	int _gold;
	int _diamond;
	int _level;
	int _exp;

	//保存出战英雄
	int _battleHero[max_battle_hero_num];

	//保存已获得的装备
	std::unordered_map<int, Equipment*> _equipmentMap;

	//保存已获得的英雄卡牌
	std::unordered_map<int, HeroCard*> _heroCardMap;

	int _uniqueIdentifierNum;	//记录物品唯一的标识符

	//单例
	static GameData* _gameData;
};

#endif // GAMEDATA_H_
