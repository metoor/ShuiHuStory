#include "Equipment.h"
#include "GameData.h"
#include "Tools.h"
#include "Config.h"
#include "cmath"

#define NUMBER_100 100
#define NUMBER_100F 100.0f

#define NUMBER_10 10
#define NUMBER_10F 10.0f

Equipment::Equipment()
	:_property(nullptr)
{
}

Equipment::~Equipment()
{
	delete _property;
	_property = nullptr;
}

void Equipment::init(const int index)
{
	auto equipment = Config::getInstance()->getEquipmentByIndex(index);
	init(equipment);
}

void Equipment::init(const EquipmentType* equipmentType)
{
	if (!_property)
	{
		_property = new EquipmentProperty();
	}

	//产生随机等级1~5（最低等级由模板的星级数据决定）
	switch (equipmentType->star)
	{
	case  1:
		_property->star = Tools::randomBetweenOneAndFive(1, 40, 30, 15, 10, 5);
		break;
	case 2:
		_property->star = Tools::randomBetweenOneAndFive(2, 50, 30, 15, 5);
		break;
	case 3:
		_property->star = Tools::randomBetweenOneAndFive(3, 80, 15, 5);
		break;
	case 4:
		_property->star = Tools::randomBetweenOneAndFive(4, 95, 5);
		break;
	case 5:
		_property->star = 5;
		break;
	default:
		break;
	}

	//根据等级产生模板比率。公式为：星级*0.1f + sin(90, 360) * 0.2f;
	_property->rate = Tools::keepTwoEffectNum(1 + _property->star * 0.1f + sin(Tools::getRandomInt(90, 360)) * 0.1f);
	
	_property->id = GameData::getInstance()->getUniqueIdentifierNum();
	_property->type = equipmentType->type;

	_property->level = 1;
	_property->exLevel = 1;
	_property->user = NONE_USER;			//NONE_USER 表示该装备没有被角色使用
	
	_property->textureName = &(equipmentType->textureName);
	_property->name = &(equipmentType->name);
	_property->des = &(equipmentType->des);

	calculatePropery(equipmentType);

}

void Equipment::init(const SaveEquipmentData * saveEquipmentData)
{
	if (!_property)
	{
		_property = new EquipmentProperty();
	}
	
	_property->id = saveEquipmentData->id;
	_property->type = saveEquipmentData->type;
	_property->star = saveEquipmentData->star;
	_property->rate = Tools::intToPercent(saveEquipmentData->rate);
	_property->level = saveEquipmentData->level;
	_property->exLevel = saveEquipmentData->exLevel;
	_property->user = saveEquipmentData->user;			//NONE_USER表示该装备没有被角色使用

	auto equipment = Config::getInstance()->getEquipmentByIndex(_property->type);

	_property->textureName = &(equipment->textureName);
	_property->name = &(equipment->name);
	_property->des = &(equipment->des);

	calculatePropery(equipment);
}

const SaveEquipmentData Equipment::getSaveData()
{
	SaveEquipmentData sed;
	sed.exLevel = _property->exLevel;
	sed.id = _property->id;
	sed.level = _property->level;
	sed.star = _property->star;
	sed.rate = Tools::percentToInt(_property->rate);
	sed.type = _property->type;
	sed.user = _property->user;

	return sed;
}

const EquipmentProperty * Equipment::getProperty()
{
	return _property;
}

void Equipment::calculatePropery(const EquipmentType* equipmentType)
{
	//计算总体的比率，例如：装备等级和装备强化等级的加成比率
	float strengthRate = (((_property->level - 1) + (_property->exLevel - 1) * 2) / NUMBER_10F) + 1.0f;
	
	//总的加成比率
	float rate = Tools::keepTwoEffectNum(_property->rate * strengthRate);

	_property->ap = equipmentType->ap * rate;
	_property->mp = equipmentType->mp * rate;
	_property->blockRate = Tools::intToPercent(equipmentType->blockRate) * rate;
	_property->critDamage = Tools::intToPercent(equipmentType->critDamage) * rate;
	_property->critRate = Tools::intToPercent(equipmentType->critRate) * rate;
	_property->buyMoney = equipmentType->buyMoney * rate;
	_property->sellMoney = equipmentType->sellMoney * rate;
	_property->defend = equipmentType->defend * rate;
	_property->magicDefend = equipmentType->magicDefend * rate;
	_property->hp = equipmentType->hp * rate;
	_property->speed = equipmentType->speed * rate;
}
