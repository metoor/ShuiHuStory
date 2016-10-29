#include "GameData.h"
#include "cocos2d.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "Equipment.h"
#include "HeroCard.h"

USING_NS_CC;

GameData* GameData::_gameData = nullptr;

//保存已获得英雄卡牌数据的文件名字
#define SAVE_HERO_CARD_FILE_NAME "myHeroCard.json"	

//保存已获得装备数据的文件名字
#define SAVE_EQUIPMENT_FILE_NAME "myEquipment.json"		

GameData::~GameData()
{
	saveUniqueIdentifierNumToFile();
	destoryEquipmentMapElement();
	destoryHeroMapElement();
}

GameData::GameData()
{
	readUniqueIdentifierNumFromFile();
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
	UserDefault::getInstance()->setIntegerForKey("uniqueIdentifierNum", _uniqueIdentifierNum);
}

void GameData::readUniqueIdentifierNumFromFile()
{
	_uniqueIdentifierNum = UserDefault::getInstance()->getIntegerForKey("uniqueIdentifierNum", 1000);
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
	jsonpath.append(SAVE_EQUIPMENT_FILE_NAME);

	saveEquipmentToFile(jsonpath);
}

void GameData::saveHeroCard()
{
	std::string jsonpath = FileUtils::getInstance()->getWritablePath();
	jsonpath.append(SAVE_HERO_CARD_FILE_NAME);

	saveHeroCardToFile(jsonpath);
}

void GameData::readEquipment()
{
	std::string jsonpath = FileUtils::getInstance()->getWritablePath();
	jsonpath.append(SAVE_EQUIPMENT_FILE_NAME);

	//判断是否是第一次登陆
	if (!FileUtils::getInstance()->isFileExist(jsonpath))
	{
		//第一次登陆创建默认装备
		for (int i = 1; i < 7; ++i)
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
	jsonpath.append(SAVE_HERO_CARD_FILE_NAME);

	//判断是否是第一次登陆
	if (!FileUtils::getInstance()->isFileExist(jsonpath))
	{
		//第一次登陆创建默英雄卡牌
		for (int i = 0; i < 3; ++i)
		{
			HeroCard* hc = new HeroCard();
			hc->init(i);
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
