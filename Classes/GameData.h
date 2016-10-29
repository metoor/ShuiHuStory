/*
*单例类，主要保存玩家游戏相关的数据，例如，金币、元宝等等
*/

#ifndef __GAMEDATA_H__
#define __GAMEDATA_H__

#include <unordered_map>

class Equipment;
class HeroCard;

class GameData{
public:
	~GameData();

	//获得实例
	static GameData* getInstance();
	
	//销毁实例
	void destoryInstance();

	//获取唯一的标识号
	int getUniqueIdentifierNum();

	//保存当前已经使用到的标识号
	void saveUniqueIdentifierNumToFile();

	//读取文件中的标识号
	void readUniqueIdentifierNumFromFile();

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

	//通过Id获取已经获得的装备
	Equipment* getEquipmentById(int id);

	//通过Id获取已经获得的英雄卡牌
	HeroCard* getHeroCardById(int id);

private:
	GameData();

	//销毁_equipmentMap里面的元数
	void destoryEquipmentMapElement();
	
	//销毁_heroCardMap里面的元数
	void destoryHeroMapElement();

private:
	int _money;
	int _gold;
	int _level;
	int _exp;

	//保存已获得的装备
	std::unordered_map<int, Equipment*> _equipmentMap;

	//保存已获得的英雄卡牌
	std::unordered_map<int, HeroCard*> _heroCardMap;

	int _uniqueIdentifierNum;	//记录物品唯一的标识符

	//单例
	static GameData* _gameData;
};

#endif // GAMEDATA_H_
