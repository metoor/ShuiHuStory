/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	GameData.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "GameData.h"
#include "cocos2d.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "Equipment.h"
#include "HeroCard.h"
#include "Tools.h"
#include "McLog.h"

USING_NS_CC;

GameData* GameData::_gameData = nullptr;

GameData::GameData()
{
}

GameData::~GameData()
{
	saveData();

	destoryEquipmentMapElement();
	destoryHeroMapElement();
}

void GameData::destoryEquipmentMapElement()
{
	for (auto iter = _equipmentMap.begin(); iter != _equipmentMap.end(); ++iter)
	{
		CC_SAFE_DELETE(iter->second);
	}

	_equipmentMap.clear();
}

void GameData::destoryHeroMapElement()
{
	for (auto iter = _heroCardMap.begin(); iter != _heroCardMap.end(); ++iter)
	{
		CC_SAFE_DELETE(iter->second);
	}

	_heroCardMap.clear();
}

GameData * GameData::getInstance()
{
	if (!_gameData)
	{
		_gameData = new (std::nothrow)GameData();

		if (!_gameData)
		{
			//错误日志记录
			McLog mLog;
			mLog.addError("_gameData create failed...", __FILE__, __LINE__);
		}
	}

	return _gameData;
}

bool GameData::isNullptr()
{
	bool result = false;

	if (_gameData == nullptr)
	{
		result = true;
	}

	return result;
}

void GameData::readData()
{
	readUniqueIdentifierNumFromFile();
	readBattleHero();
	readUserData();
	readEquipment();
	readHeroCard();
}

void GameData::saveData()
{
	saveUserData();
	saveBattleHero();
	saveEquipment();
	saveHeroCard();
	saveUniqueIdentifierNumToFile();
}

bool GameData::setGold(const int gold)
{
	bool result = true;

	if ((_gold + gold) < 0)
	{
		result = false;
	}
	else
	{
		_gold = Tools::minInt(_gold + gold, max_gold);
	}

	return result;
}

bool GameData::setDiamond(const int diamond)
{
	bool result = true;

	if ((_diamond + diamond) < 0)
	{
		result = false;
	}
	else
	{
		_diamond = Tools::minInt(_diamond + diamond, max_diamond);
	}

	return result;
}

bool GameData::setExp(const int exp)
{
	bool result = true;
	int totalxp = getExpLimit(start_exp_rate, _level);

	_exp += exp;

	if (_level >= 100)
	{
		_exp = totalxp;

		result = false;
	}

	while (_exp >= totalxp && _level < max_level)
	{
		_exp -= totalxp;
		++_level;
	}

	return result;
}

int GameData::getGold()
{
	return _gold;
}

int GameData::getDiamond()
{
	return _diamond;
}

int GameData::getExp()
{
	return _exp;
}

int GameData::getLevel()
{
	return _level;
}

int GameData::getExpLimit(float x, int level)
{
	if (level == 0)
	{
		return start_exp;
	}

	return x * getExpLimit(x, level - 1);
}

void GameData::saveUserData()
{
	auto db = UserDefault::getInstance();

	db->setIntegerForKey(goldName.c_str(), _gold);
	db->setIntegerForKey(diamondName.c_str(), _diamond);
	db->setIntegerForKey(levelName.c_str(), _level);
	db->setIntegerForKey(expName.c_str(), _exp);

	db->flush();
}

void GameData::readUserData()
{
	auto db = UserDefault::getInstance();

	_gold = Tools::maxInt(0, Tools::minInt(db->getIntegerForKey(goldName.c_str(), init_glod), max_gold));
	_diamond = Tools::maxInt(0, Tools::minInt(db->getIntegerForKey(diamondName.c_str(), init_diamond), max_diamond));
	_level = Tools::maxInt(0, Tools::minInt(db->getIntegerForKey(levelName.c_str(), init_level), max_level));
	_exp = Tools::maxInt(0, db->getIntegerForKey(expName.c_str(), 0));
}

void GameData::destoryInstance()
{
	CC_SAFE_DELETE(_gameData);
}

int GameData::getUniqueIdentifierNum()
{
	return _uniqueIdentifierNum++;
}

void GameData::saveUniqueIdentifierNumToFile()
{
	UserDefault::getInstance()->setIntegerForKey(uniqueNumName.c_str(), _uniqueIdentifierNum);
}

void GameData::readUniqueIdentifierNumFromFile()
{
	_uniqueIdentifierNum = UserDefault::getInstance()->getIntegerForKey(uniqueNumName.c_str(), init_unique_num);
}

void GameData::readBattleHero()
{
	bool isFirst = true;

	//从文件中依次读取出出战英雄的id
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		_battleHero[pos] = UserDefault::getInstance()->getIntegerForKey(battleSaveKey[pos].c_str(), none);
		
		if (isFirst && _battleHero[pos] > 1000)
		{
			//如果有出战英雄则不是第一次登陆
			isFirst = false;
		}
	}

	if (isFirst)
	{
		//设置默认出战的两个英雄
		_battleHero[0] = 1005;
		_battleHero[1] = 1006;
	}
}

void GameData::saveBattleHero()
{
	//从文件中依次保存出出战英雄的id
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		UserDefault::getInstance()->setIntegerForKey(battleSaveKey[pos].c_str(), _battleHero[pos]);
	}
}

void GameData::readEquipmentFromJson(string fileName)
{
	rapidjson::Document doc;

	std::string contentStr = FileUtils::getInstance()->getStringFromFile(fileName);
	doc.Parse<0>(contentStr.c_str());

	if (doc.HasParseError())
	{
		//error
		CCAssert(false, "Json::cloth.json Reader Parse error!");

		McLog log;
		log.addError("Json Reader parse error...", __FILE__, __LINE__);
	}

	for (unsigned int i = 0; i < doc["data"].Size(); ++i)
	{
		rapidjson::Value & json = doc["data"][i];

		SaveEquipmentData sed;

		sed.exLevel = json["exLevel"].GetInt();
		sed.id = json["id"].GetInt();
		sed.level = json["level"].GetInt();
		sed.type = json["type"].GetInt();
		sed.rate = json["rate"].GetInt();
		sed.star = json["star"].GetInt();
		sed.user = json["user"].GetInt();

		Equipment* equipment = new Equipment();

		equipment->init(&sed);

		_equipmentMap.insert(std::pair<int, Equipment*>(sed.id, equipment));
	}
}

void GameData::readHeroCardFromJson(std::string fileName)
{
	rapidjson::Document doc;

	std::string contentStr = FileUtils::getInstance()->getStringFromFile(fileName);
	doc.Parse<0>(contentStr.c_str());

	if (doc.HasParseError())
	{
		//error
		CCAssert(false, "Json::cloth.json Reader Parse error!");

		McLog log;
		log.addError("Json Reader parse error...", __FILE__, __LINE__);
	}

	for (unsigned int i = 0; i < doc["data"].Size(); ++i)
	{
		rapidjson::Value & json = doc["data"][i];

		SaveHeroCardData shcd;

		shcd.exLevel = json["exLevel"].GetInt();
		shcd.id = json["id"].GetInt();
		shcd.level = json["level"].GetInt();
		shcd.type = json["type"].GetInt();
		shcd.rate = json["rate"].GetInt();
		shcd.star = json["star"].GetInt();

		//装备信息
		shcd.equipmentId[0] = json["e0"].GetInt();
		shcd.equipmentId[1] = json["e1"].GetInt();
		shcd.equipmentId[2] = json["e2"].GetInt();
		shcd.equipmentId[3] = json["e3"].GetInt();
		shcd.equipmentId[4] = json["e4"].GetInt();
		shcd.equipmentId[5] = json["e5"].GetInt();

		HeroCard* heroCard = new HeroCard();

		heroCard->init(&shcd);

		_heroCardMap.insert(std::pair<int, HeroCard*>(shcd.id, heroCard));
	}
}

void GameData::saveEquipmentToFile(std::string fileName)
{
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);

	for (auto iter = _equipmentMap.begin(); iter != _equipmentMap.end(); ++iter)
	{
		auto sed = iter->second->getSaveData();
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("id", sed.id, allocator);
		object.AddMember("type", sed.type, allocator);
		object.AddMember("exLevel", sed.exLevel, allocator);
		object.AddMember("level", sed.level, allocator);
		object.AddMember("rate", sed.rate, allocator);
		object.AddMember("star", sed.star, allocator);
		object.AddMember("user", sed.user, allocator);

		array.PushBack(object, allocator);
	}

	document.AddMember("data", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	FILE* file = fopen(fileName.c_str(), "wb");

	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
	else
	{
		//error
		CCAssert(false, "can not create file...");

		McLog log;
		log.addError("can not create file: " + fileName, __FILE__, __LINE__);
	}
}

void GameData::saveHeroCardToFile(std::string fileName)
{
	rapidjson::Document document;
	document.SetObject();

	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value array(rapidjson::kArrayType);

	for (auto iter = _heroCardMap.begin(); iter != _heroCardMap.end(); ++iter)
	{
		auto shd = iter->second->getSaveData();
		rapidjson::Value object(rapidjson::kObjectType);

		object.AddMember("id", shd.id, allocator);
		object.AddMember("type", shd.type, allocator);
		object.AddMember("exLevel", shd.exLevel, allocator);
		object.AddMember("level", shd.level, allocator);
		object.AddMember("rate", shd.rate, allocator);
		object.AddMember("star", shd.star, allocator);

		object.AddMember("e0", shd.equipmentId[0], allocator);
		object.AddMember("e1", shd.equipmentId[1], allocator);
		object.AddMember("e2", shd.equipmentId[2], allocator);
		object.AddMember("e3", shd.equipmentId[3], allocator);
		object.AddMember("e4", shd.equipmentId[4], allocator);
		object.AddMember("e5", shd.equipmentId[5], allocator);

		array.PushBack(object, allocator);
	}

	document.AddMember("data", array, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	FILE* file = fopen(fileName.c_str(), "wb");

	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}
	else
	{
		//error
		CCAssert(false, "can not create file...");

		McLog log;
		log.addError("can not create file: " + fileName, __FILE__, __LINE__);

	}
}

void GameData::saveEquipment()
{
	std::string jsonpath = FileUtils::getInstance()->getWritablePath();
	jsonpath.append(save_equipment_file_name);

	saveEquipmentToFile(jsonpath);
}

void GameData::saveHeroCard()
{
	std::string jsonpath = FileUtils::getInstance()->getWritablePath();
	jsonpath.append(save_hero_file_name);

	saveHeroCardToFile(jsonpath);
}

void GameData::readEquipment()
{
	std::string jsonpath = FileUtils::getInstance()->getWritablePath();
	jsonpath.append(save_equipment_file_name);

	//判断是否是第一次登陆
	if (!FileUtils::getInstance()->isFileExist(jsonpath))
	{
		//第一次登陆创建默认装备
		for (int i = 1; i < 6; ++i)
		{
			Equipment* eq = new Equipment();
			eq->init(i * 100);
			addEquipmentToMap(eq);
		}
	}
	else
	{
		//不是第一次登陆，读取保存的装备数据
		readEquipmentFromJson(jsonpath);
	}
}

void GameData::readHeroCard()
{
	std::string jsonpath = FileUtils::getInstance()->getWritablePath();
	jsonpath.append(save_hero_file_name);

	//判断是否是第一次登陆
	if (!FileUtils::getInstance()->isFileExist(jsonpath))
	{
		//第一次登陆创建默英雄卡牌
		for (int i = 0; i < 2; i++)
		{
			HeroCard* hc = new (std::nothrow) HeroCard();
			//创建默认的英雄：公孙胜和水浒迷
			hc->init(3 + i * 105);
			addHeroCardToMap(hc);
		}
	}
	else
	{
		//不是第一次登陆，读取保存的英雄卡牌数据
		readHeroCardFromJson(jsonpath);
	}
}

void GameData::addEquipmentToMap(Equipment * equipment)
{
	int key = equipment->getProperty()->id;
	_equipmentMap.insert({ key, equipment });
}

void GameData::addHeroCardToMap(HeroCard * heroCard)
{
	int key = heroCard->getProperty()->id;
	_heroCardMap.insert({ key, heroCard });
}

void GameData::deleteHeroCardById(int id)
{
	_heroCardMap.erase(id);
}

void GameData::deleteEquipmentById(int id)
{
	_equipmentMap.erase(id);
}

Equipment* GameData::getEquipmentById(int id)
{
	auto iter = _equipmentMap.find(id);
	Equipment* result = nullptr;

	if (iter != _equipmentMap.end())
	{
		//成功找到
		result = iter->second;
	}

	return result;
}

HeroCard * GameData::getHeroCardById(int id)
{
	auto iter = _heroCardMap.find(id);
	HeroCard* result = nullptr;

	if (iter != _heroCardMap.end())
	{
		//成功找到
		result = iter->second;
	}

	return result;
}

int GameData::getBattleHeroId(int pos)
{
	if (Tools::betweenAnd(pos, 0, max_battle_hero_num - 1))
	{
		return _battleHero[pos];
	}

	return 100;
}

int * GameData::getBattleHeroArray()
{
	return _battleHero;
}

bool GameData::unbattleHero(int id)
{
	bool result = false;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		if (_battleHero[pos] == id)
		{
			//查找到英雄
			auto hero = getHeroCardById(id);
			auto equipmentList = hero->getProperty()->equipmentId;

			for (int pos = 0; pos < max_equipment_num; ++pos)
			{
				int equipmentId = equipmentList[pos];

				//如果将英雄有装备则将装备卸下
				if (equipmentId != none)
				{
					auto equipment = getEquipmentById(equipmentId);
					equipment->setUserId(none);
					hero->wearEquipment(pos, none);
				}
			}


			_battleHero[pos] = none;
			result = true;
			break;
		}
	}

	return result;
}

bool GameData::isBattleHero(int id)
{
	bool result = false;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		if (id == _battleHero[pos])
		{
			result = true;
			break;
		}
	}

	return result;
}

int GameData::getBattleHeroNum()
{
	int num = 0;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		if (_battleHero[pos] > 1000)
		{
			++num;
		}
	}

	return num;
}

int GameData::getPosById(int id)
{
	int result = none;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		if (_battleHero[pos] == id)
		{
			result = pos;
		}
	}

	return result;
}

void GameData::setBattleHero(int pos, int id)
{
	if (pos < 0 || pos >= max_battle_hero_num)
	{
		McLog mc;
		mc.addWaring("'pos' out of range...", __FILE__, __LINE__);
	}
	else
	{
		_battleHero[pos] = id;
	}
}

const std::unordered_map<int, Equipment*>* GameData::getEquipmentMap()
{
	return &_equipmentMap;
}

const std::unordered_map<int, HeroCard*>* GameData::getHeroCardMap()
{
	return &_heroCardMap;
}
