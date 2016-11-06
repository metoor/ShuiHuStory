#include "Config.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "cocos2d.h"
#include "Tools.h"
#include "McLog.h"


Config* Config::_config = nullptr;

USING_NS_CC;

Config::Config()
{
	readEquipmentJson(equipment_file_name);
	readHeroJson(hero_file_name);
}

Config::~Config()
{
	destoryHeroVectorElement();
	destoryEquipmentVectorElement();
}

void Config::destoryHeroVectorElement()
{
	for (int index = _heroCardVector.size() - 1; index >= 0; ++index)
	{
		delete _heroCardVector.at(index);
	}

	_heroCardVector.clear();
}

void Config::destoryEquipmentVectorElement()
{
	for (int index = _equipmentVector.size() - 1; index >= 0; ++index)
	{
		delete _equipmentVector.at(index);
	}

	_equipmentVector.clear();
}

void Config::readHeroJson(string fileName)
{
	string filepath = FileUtils::getInstance()->fullPathForFilename(fileName);
	rapidjson::Document doc;

	string content = FileUtils::getInstance()->getStringFromFile(filepath);

	doc.Parse<0>(content.c_str());

	if (doc.HasParseError()) 
	{
		//error
		CCAssert(false, "Json::cloth.json Reader Parse error!");

		McLog log;
		log.addError("json Reader Parse error... ", __FILE__, __LINE__);
	}

	if (!doc["datas"].IsNull() && doc["datas"].IsArray())
	{

		for (unsigned int i = 0; i < doc["datas"].Size(); ++i)
		{
			rapidjson::Value & json = doc["datas"][i];

			auto heroCardType = new HeroCardType();

			heroCardType->name = json["name"].GetString();
			heroCardType->exName = json["exName"].GetString();
			heroCardType->starName = json["starName"].GetString();
			heroCardType->textureName = json["textureName"].GetString();
			heroCardType->ap = json["ap"].GetInt();
			heroCardType->hp = json["hp"].GetInt();
			heroCardType->mp = json["mp"].GetInt();
			heroCardType->magicDefend = json["magicDefend"].GetInt();
			heroCardType->speed = json["speed"].GetInt();
			heroCardType->critDamage = json["critDamage"].GetInt();
			heroCardType->critRate = json["critRate"].GetInt();
			heroCardType->blockRate = json["blockRate"].GetInt();
			heroCardType->isMagic = json["isMagic"].GetBool();
			heroCardType->defend = json["defend"].GetInt();
			heroCardType->star = json["star"].GetInt();
			heroCardType->type = json["type"].GetInt();
			heroCardType->attackId = json["attackId"].GetInt();
			heroCardType->skillId = json["skillId"].GetInt();
			heroCardType->sellMoney = json["sellMoney"].GetInt();
			heroCardType->buyMoney = json["buyMoney"].GetInt();
			heroCardType->skillName = json["skillName"].GetString();
			heroCardType->des = json["des"].GetString();

			_heroCardVector.push_back(heroCardType);
		}
	}
}

void Config::readEquipmentJson(string fileName)
{
	string filepath = FileUtils::getInstance()->fullPathForFilename(fileName);
	rapidjson::Document doc;

	string content = FileUtils::getInstance()->getStringFromFile(filepath);

	doc.Parse<0>(content.c_str());

	if (doc.HasParseError()) 
	{
		//error
		CCAssert(false, "Json::cloth.json Reader Parse error!");

		McLog log;
		log.addError("json Reader Parse error... ", __FILE__, __LINE__);
	}

	if (!doc["datas"].IsNull() && doc["datas"].IsArray())
	{

		for (unsigned int i = 0; i < doc["datas"].Size(); ++i)
		{
			rapidjson::Value & json = doc["datas"][i];

			auto equipmentType = new EquipmentType();

			equipmentType->type = json["type"].GetInt();
			equipmentType->name = json["name"].GetString();
			equipmentType->textureName = json["textureName"].GetString();
			equipmentType->defend = json["defend"].GetInt();
			equipmentType->magicDefend = json["magicDefend"].GetInt();
			equipmentType->star = json["star"].GetInt();
			equipmentType->hp = json["hp"].GetInt();
			equipmentType->ap = json["ap"].GetInt();
			equipmentType->mp = json["mp"].GetInt();
			equipmentType->critDamage = json["critDamage"].GetInt();
			equipmentType->critRate = json["critRate"].GetInt();
			equipmentType->speed = json["speed"].GetInt();
			equipmentType->blockRate = json["blockRate"].GetInt();
			equipmentType->sellMoney = json["sellMoney"].GetInt();
			equipmentType->buyMoney = json["buyMoney"].GetInt();
			equipmentType->des = json["des"].GetString();

			_equipmentVector.push_back(equipmentType);

		}
	}
}


Config * Config::getInstance()
{
	if (!_config)
	{
		_config = new (std::nothrow)Config();

		if (!_config)
		{
			//错误日志记录
			McLog mLog;
			mLog.addError("_config create failed...", __FILE__, __LINE__);
		}
	}

	return _config;
}

const HeroCardType* Config::getHeroCardByIndex(int type)
{
	HeroCardType* result = nullptr;

	if (Tools::betweenAnd(type, 0, _heroCardVector.size()))
	{
		result = _heroCardVector.at(type);
	}
	else
	{
		//error
		CCAssert(false, "type out of index...");

		McLog log;
		log.addError("'type' out of range... ", __FILE__, __LINE__);
	}

	return result;
}

const EquipmentType* Config::getEquipmentByIndex(int type)
{
	EquipmentType* equiment = nullptr;

	//获得装备的类型，武器，帽子等...
	int first = type / 100;

	//获得装备在同种类里面的偏移量
	int pos = type - first * 100;

	//各个类型的装备索引号
	int equipmentNum[6] = { 0, 9, 17, 42, 48, 53 };

	//获得装备在Vector中的索引
	if (Tools::betweenAnd(first, 1, 6))
	{


		int index = equipmentNum[first - 1] + pos;

		if (Tools::betweenAnd(index, 0, _equipmentVector.size() - 1))
		{
			equiment = _equipmentVector.at(index);
		}
		else
		{
			//error
			CCAssert(false, "index out of index...");

			McLog log;
			log.addError("‘index’ out of range... ", __FILE__, __LINE__);
		}
	}
	else
	{
		//error
		CCAssert(false, "first out of index...");

		McLog log;
		log.addError("‘first’ out of range... ", __FILE__, __LINE__);
	}
	return equiment;
}
