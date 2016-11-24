/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	HeroCard.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "HeroCard.h"
#include "Tools.h"
#include "GameData.h"
#include "Config.h"
#include "Equipment.h"
#include "cmath"
#include "ConstantDefine.h"


HeroCard::HeroCard()
	:_property(nullptr)
{
}

HeroCard::~HeroCard()
{
	delete _property;
	_property = nullptr;
}

void HeroCard::init(const int index)
{
	auto heroCardType = Config::getInstance()->getHeroCardByIndex(index);
	init(heroCardType);
}

void HeroCard::init(const HeroCardType * heroCardType)
{
	if (!_property)
	{
		_property = new HeroCardProperty();
	}

	//产生随机等级1~5（最低等级由模板的星级数据决定）
	switch (heroCardType->star)
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

	//根据等级产生模板比率。公式为：根据等级产生模板比率。公式为：星级*0.1f + sin(90, 360) * 0.2f;
	_property->rate = Tools::keepTwoEffectNum(1 + _property->star * 0.1f+ sin(Tools::getRandomInt(90, 360)) * 0.1f);

	_property->id = GameData::getInstance()->getUniqueIdentifierNum();
	_property->type = heroCardType->type;

	_property->level = 1;
	_property->exLevel = 0;
	_property->isMagic = heroCardType->isMagic;
	_property->attackId = heroCardType->attackId;
	_property->skillId = heroCardType->skillId;

	_property->textureName = &(heroCardType->textureName);
	_property->name = &(heroCardType->name);
	_property->exName = &(heroCardType->exName);
	_property->skillName = &(heroCardType->skillName);
	_property->starName = &(heroCardType->starName);
	_property->des = &(heroCardType->des);

	//初始化卡牌角色的装备
	for (int pos = 0; pos < max_equipment_num; ++pos)
	{
		wearEquipment(pos, none);
	}

	calculateHeroCardPropery(heroCardType);
}

void HeroCard::init(const SaveHeroCardData* saveHeroData)
{
	if (!_property)
	{
		_property = new HeroCardProperty();
	}

	_property->id = saveHeroData->id;
	_property->type = saveHeroData->type;
	_property->star = saveHeroData->star;
	_property->rate = Tools::intToPercent(saveHeroData->rate);
	_property->level = saveHeroData->level;
	_property->exLevel = saveHeroData->exLevel;

	//获取英雄装备穿戴信息
	for (int pos = 0; pos < max_equipment_num; ++pos)
	{
		_property->equipmentId[pos] = saveHeroData->equipmentId[pos];
	}

	auto heroCardType = Config::getInstance()->getHeroCardByIndex(_property->type);

	_property->attackId = heroCardType->attackId;
	_property->skillId = heroCardType->skillId;
	_property->isMagic = heroCardType->isMagic;

	_property->textureName = &(heroCardType->textureName);
	_property->name = &(heroCardType->name);
	_property->exName = &(heroCardType->exName);
	_property->skillName = &(heroCardType->skillName);
	_property->starName = &(heroCardType->starName);
	_property->des = &(heroCardType->des);

	calculateHeroCardPropery(heroCardType);
	
	//将装备的属性附加到英雄属性上
	calculateEquipmentPropery();
}

const SaveHeroCardData HeroCard::getSaveData()
{
	SaveHeroCardData shd;
	shd.exLevel = _property->exLevel;
	shd.id = _property->id;
	shd.level = _property->level;
	shd.star = _property->star;
	shd.rate = Tools::percentToInt(_property->rate);
	shd.type = _property->type;
	
	Tools::arrayAssignment(max_equipment_num, shd.equipmentId, _property->equipmentId);

	return shd;
}

const HeroCardProperty * HeroCard::getProperty()
{
	return _property;
}

void HeroCard::wearEquipment(int pos, int equipmentId)
{
	//越界检查
	if (Tools::betweenAnd(pos, 0, max_equipment_num - 1))
	{
		_property->equipmentId[pos] = equipmentId;
	}
	else
	{
		//error
	}
}

int HeroCard::getEquipmentByPos(int pos)
{
	int result = none;

	if (Tools::betweenAnd(pos, 0, max_equipment_num - 1))
	{
		result = _property->equipmentId[pos];
	}
	else
	{
		//error
	}

	return result;
}

void HeroCard::updatetePropery()
{
	//重新计算英雄本身的属性加成和装备加成
	calculateHeroCardPropery(Config::getInstance()->getHeroCardByIndex(_property->type));
	calculateEquipmentPropery();
}

void HeroCard::levelup()
{
	++(_property->level);
	updatetePropery();
}

void HeroCard::intensify()
{
	++(_property->exLevel);
	updatetePropery();
}

void HeroCard::calculateHeroCardPropery(const HeroCardType * heroCardType)
{
	//计算总体的比率，例如：卡牌等级和卡牌强化等级的加成比率
	float strengthRate = (((_property->level - 1) + (_property->exLevel) * 2) / 10.0f) + 1.0f;

	//总的加成比率
	float rate = Tools::keepTwoEffectNum(_property->rate * strengthRate);

	_property->ap = heroCardType->ap * rate;
	_property->mp = heroCardType->mp * rate;
	_property->blockRate = heroCardType->blockRate * rate;
	_property->critDamage = Tools::intToPercent(heroCardType->critDamage) * rate;
	_property->critRate = heroCardType->critRate * rate;
	_property->defend = heroCardType->defend * rate;
	_property->magicDefend = heroCardType->magicDefend * rate;
	_property->hp = heroCardType->hp * rate;
	_property->speed = heroCardType->speed * rate;

	_property->buyMoney = heroCardType->buyMoney * rate;
	_property->sellMoney = heroCardType->sellMoney * rate;
}

void HeroCard::calculateEquipmentPropery()
{
	//临时保存计算出来的装备属性之和。
	EquipmentProperty ep{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, nullptr, nullptr, nullptr};

	//遍历佩戴装备的属性并分别求出个种类属性之和
	for (int pos = 0; pos < max_equipment_num; ++pos)
	{
		int equipmentId = getEquipmentByPos(pos);

		if (equipmentId != none)
		{
			auto equipmentProperty = GameData::getInstance()->getEquipmentById(equipmentId)->getProperty();

			ep.ap += equipmentProperty->ap;
			ep.blockRate += equipmentProperty->blockRate;
			ep.magicDefend += equipmentProperty->magicDefend;
			ep.critDamage += equipmentProperty->critDamage;
			ep.critRate += equipmentProperty->critRate;
			ep.defend += equipmentProperty->defend;
			ep.mp += equipmentProperty->mp;
			ep.hp += equipmentProperty->hp;
			ep.speed += equipmentProperty->speed;
		}
	}

	//将装备附加的属性更新到英雄卡牌属性上
	_property->ap += ep.ap;
	_property->mp += ep.mp;
	_property->blockRate += ep.blockRate;
	_property->critDamage += ep.critDamage;
	_property->critRate += ep.critRate;
	_property->defend += ep.defend;
	_property->magicDefend += ep.magicDefend;
	_property->hp += ep.hp;
	_property->speed += ep.speed;
}
