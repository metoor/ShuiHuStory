/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	DetialsLayer.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "DetialsLayer.h"
#include "cocostudio/CocoStudio.h"
#include "BlockLayer.h"
#include "GameData.h"
#include "AudioManager.h"
#include "Equipment.h"
#include "HeroCard.h"
#include "I18N.h"
#include "Tools.h"
#include "McLog.h"
#include "AudioManager.h"
#include "DialogManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

DetialsLayer::DetialsLayer()
	:_objectId(0),
	_intensifyFun(nullptr),
	_levelupFunc(nullptr)
{
}

DetialsLayer::~DetialsLayer()
{
}

bool DetialsLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//吞噬点击事件
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockTagName);
	addChild(blockLayer);

	loadUI();

	return true;
}

void DetialsLayer::initUiWithId(int id, DisplayType type)
{
	_objectId = id;
	_type = type;

	updateAttribute();
}

void DetialsLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//查找控件
	_nameLabel = node->getChildByName<Text*>(nameLabelName);
	_ico = node->getChildByName<ImageView*>(icoName);
	_attributeLabel = node->getChildByName<Text*>(attributeLabelName);
	_btnIntensify = node->getChildByName<Button*>(btnIntensifyName);
	_btnLevelup = node->getChildByName<Button*>(btnLevelupName);
	_btnClose = node->getChildByName<Button*>(btnCloseName);

	//关闭按钮回调
	_btnClose->addTouchEventListener(CC_CALLBACK_2(DetialsLayer::btnCloseCallBack, this));
	_btnIntensify->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			//点击音效
			AudioManager::getInstance()->playClickEffect();

			dynamic_cast<Button*>(pSender)->setUserData((void*)BT_INTENSIFY);

			intensify();
		}
	});

	_btnLevelup->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			//点击音效
			AudioManager::getInstance()->playClickEffect();
			dynamic_cast<Button*>(pSender)->setUserData((void*)BT_LEVEL_UP);

			levelup();
		}
	});
}



int DetialsLayer::calculateCost(BtnType type)
{
	if (BT_LEVEL_UP == type)
	{
		return calculateGold();
	}
	else
	{
		return calculateDiamond();
	}
}

int DetialsLayer::calculateGold()
{
	int cost = 0;
	auto data = GameData::getInstance();

	switch (_type)
	{
	case DT_EQUIPMENT:
	{
		//计算升级需要消耗金币
		auto property = data->getEquipmentById(_objectId)->getProperty();
		cost = property->level * levelup_base_gold * (1 + property->star * 0.1f);
		break;
	}
	case DT_HERO:
	{
		//计算升级需要消耗金币
		auto property = data->getHeroCardById(_objectId)->getProperty();
		cost = property->level * levelup_base_gold * (1 + property->star * 0.1f);
		break;
	}
	default:
		break;
	}
	return cost;
}

int DetialsLayer::calculateDiamond()
{
	int cost = 0;
	auto data = GameData::getInstance();

	switch (_type)
	{
	case DT_EQUIPMENT:
	{
		//计算强化所需要的钻石
		auto property = data->getEquipmentById(_objectId)->getProperty();
		cost = property->exLevel * intensify_base_diamond * (1 + property->star *0.1f);
		break;
	}
	case DT_HERO:
	{
		//计算强化所需要的钻石
		auto property = data->getHeroCardById(_objectId)->getProperty();
		cost = property->exLevel * intensify_base_diamond * (1 + property->star *0.1f);
		break;
	}
	default:
		break;
	}
	return cost;
}

int DetialsLayer::findSameTypeObject()
{
	int result = none;
	auto data = GameData::getInstance();

	switch (_type)
	{
	case DT_EQUIPMENT:
	{
		auto equimpentMap = data->getEquipmentMap();
		auto curProperty = data->getEquipmentById(_objectId)->getProperty();

		for (auto iter = equimpentMap->begin(); iter != equimpentMap->end(); ++iter)
		{
			auto property = iter->second->getProperty();

			if (property->type == curProperty->type && iter->first != _objectId && property->user == none)
			{
				//有多余的同类型装备，而且没有装备到英雄身上，则查找成功
				result = iter->first;
				break;
			}
		}
		break;
	}
	case DT_HERO:
	{
		auto heroMap = data->getHeroCardMap();
		auto curProperty = data->getHeroCardById(_objectId)->getProperty();

		for (auto iter = heroMap->begin(); iter != heroMap->end(); ++iter)
		{
			auto property = iter->second->getProperty();

			if (property->type == curProperty->type && iter->first != _objectId && _objectId)
			{
				//如果多余的英雄已经出战，则忽略改英雄
				if (!data->isBattleHero(iter->first))
				{
					result = iter->first;
				}
			}
		}
		break;
	}
	default:
		break;
	}

	return result;
}

void DetialsLayer::levelupEquiupment()
{
	auto data = GameData::getInstance();
	auto dm = DialogManager::getInstance();
	auto i18n = I18N::getInstance();
	auto equipment = data->getEquipmentById(_objectId);

	
	int cost = calculateCost(BT_LEVEL_UP);

	dm->showDialog(i18n->getStringByKey(levelupTitle), StringUtils::format(i18n->getStringByKey(levelTips).c_str(), cost,
		equipment->getProperty()->name->c_str()),
		[&, cost](Ref* pSender) {

		auto data = GameData::getInstance();
		auto equipment = data->getEquipmentById(_objectId);

		//判断是否有足够的金币， 因为是要消耗金币所以是负数
		if (data->setGold(-cost))
		{
			equipment->levelup();
			dm->showTips(StringUtils::format(I18N::getInstance()->getStringByKey(succed).c_str(), equipment->getProperty()->name->c_str()), Color4B::GREEN);
		}
		else
		{
			//金币不足
			dm->showTips(I18N::getInstance()->getStringByKey(notEnoughGold));
		}
	});
}

void DetialsLayer::levelupHero()
{
	auto data = GameData::getInstance();
	auto dm = DialogManager::getInstance();
	auto i18n = I18N::getInstance();
	auto hero = data->getHeroCardById(_objectId);

	int cost = calculateCost(BT_LEVEL_UP);

	dm->showDialog(i18n->getStringByKey(levelupTitle), StringUtils::format(i18n->getStringByKey(levelTips).c_str(), cost,
		hero->getProperty()->name->c_str()),
		[&, cost](Ref* pSender) {

		auto data = GameData::getInstance();
		auto hero = data->getHeroCardById(_objectId);

		//判断是否有足够的金币， 因为是要消耗金币所以是负数
		if (data->setGold(-cost))
		{
			hero->levelup();
			dm->showTips(StringUtils::format(I18N::getInstance()->getStringByKey(succed).c_str(), hero->getProperty()->name->c_str()), Color4B::GREEN);
		}
		else
		{
			//金币不足
			dm->showTips(I18N::getInstance()->getStringByKey(notEnoughGold));
		}
	});
}

void DetialsLayer::intensifyEquipment()
{
	auto data = GameData::getInstance();
	auto dm = DialogManager::getInstance();
	auto i18n = I18N::getInstance();
	auto equipment = data->getEquipmentById(_objectId);

	int cost = calculateCost(BT_INTENSIFY);

	dm->showDialog(i18n->getStringByKey(intensifyTitle), StringUtils::format(i18n->getStringByKey(intensifyTips).c_str(), cost,
		equipment->getProperty()->name->c_str(), equipment->getProperty()->name->c_str()),
		[&, cost](Ref* pSender) {

		auto data = GameData::getInstance();
		auto equipment = data->getEquipmentById(_objectId);

		//判断是否有足够的金币， 因为是要消耗金币所以是负数
		if (data->setGold(-cost))
		{
			//是否有相同的装备
			int id = findSameTypeObject();

			if (id != none)
			{
				//删除作为强化材料的装备
				data->deleteEquipmentById(id);

				equipment->intensify();
				dm->showTips(StringUtils::format(I18N::getInstance()->getStringByKey(succed).c_str(), equipment->getProperty()->name->c_str()), Color4B::GREEN);
			}
			else
			{
				//没有相同的材料
				dm->showTips(StringUtils::format(I18N::getInstance()->getStringByKey(notEnougMaterial).c_str(), equipment->getProperty()->name->c_str()));
			}
		}
		else
		{
			//没有金币
			dm->showTips(I18N::getInstance()->getStringByKey(notEnoughGold).c_str());
		}
	});
}

void DetialsLayer::intensifyHero()
{
	auto data = GameData::getInstance();
	auto dm = DialogManager::getInstance();
	auto i18n = I18N::getInstance();
	auto hero = data->getHeroCardById(_objectId);

	int cost = calculateCost(BT_INTENSIFY);

	dm->showDialog(i18n->getStringByKey(intensifyTitle), StringUtils::format(i18n->getStringByKey(intensifyTips).c_str(), cost,
		hero->getProperty()->name->c_str(), hero->getProperty()->name->c_str()),
		[&, cost](Ref* pSender) {

		auto data = GameData::getInstance();
		auto hero = data->getHeroCardById(_objectId);

		//判断是否有足够的金币， 因为是要消耗金币所以是负数
		if (data->setGold(-cost))
		{
			//是否有相同的卡牌
			int id = findSameTypeObject();

			if (id != none)
			{
				//删除作为强化材料的卡牌
				data->deleteHeroCardById(id);

				hero->intensify();
				dm->showTips(StringUtils::format(I18N::getInstance()->getStringByKey(succed).c_str(), hero->getProperty()->name->c_str()), Color4B::GREEN);
			}
			else
			{
				//没有相同的材料
				dm->showTips(StringUtils::format(I18N::getInstance()->getStringByKey(notEnougMaterial).c_str(), hero->getProperty()->name->c_str()));
			}
		}
		else
		{
			//没有金币
			dm->showTips(I18N::getInstance()->getStringByKey(notEnoughGold).c_str());
		}
	});
}

string DetialsLayer::propertyToString(const EquipmentProperty * property)
{
	auto i18n = I18N::getInstance();

	string res = StringUtils::format(i18n->getStringByKey(level_d).c_str(), property->level);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(intensify_d).c_str(), "+", property->exLevel);
	res += "\n";

	//获得装备的类型
	int type = property->type / 100;

	switch (type)
	{
	case 1:
	case 2:
	{
		//衣服
		res += StringUtils::format(i18n->getStringByKey(define_d).c_str(), "+", property->defend);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(mDefine_d).c_str(), "+", property->magicDefend);
		break;
	}
	case 3:
	{
		//武器
		res += StringUtils::format(i18n->getStringByKey(ap_d).c_str(), "+", property->ap);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(mp_d).c_str(), "+", property->mp);
		res += "\n";
		res = StringUtils::format(i18n->getStringByKey(crit_d).c_str(), "+", Tools::percentToInt(property->critRate));
		break;
	}
	case 4:
	{
		//佩戴
		res += StringUtils::format(i18n->getStringByKey(hp_d).c_str(), "+", property->hp);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(critDmg_d).c_str(), "+", Tools::percentToInt(property->critDamage));
		break;
	}
	case 5:
	{
		//鞋子
		res += StringUtils::format(i18n->getStringByKey(speed_d).c_str(), "+", property->speed);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(block_d).c_str(), "+", Tools::percentToInt(property->blockRate));
		break;
	}
	case 6:
	{
		//坐骑
		res += StringUtils::format(i18n->getStringByKey(speed_d).c_str(), "+", property->speed);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(hp_d).c_str(), "+", property->hp);
		res += "\n";
		res = StringUtils::format(i18n->getStringByKey(define_d).c_str(), "+", property->defend);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(mDefine_d).c_str(), "+", property->magicDefend);
		break;
	}
	default:
		break;
	}

	return res;
}

string DetialsLayer::propertyToString(const HeroCardProperty* property)
{
	auto i18n = I18N::getInstance();

	//将属性格式化到要显示的中文字符串中去
	string res = StringUtils::format(i18n->getStringByKey(level_d).c_str(), property->level);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(intensify_d).c_str(), "", property->exLevel);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(star_d).c_str(), property->star);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(crit_d).c_str(), "", Tools::percentToInt(property->critRate));
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(ap_d).c_str(), "", property->ap);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(mp_d).c_str(), "", property->mp);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(define_d).c_str(), "", property->defend);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(mDefine_d).c_str(), "", property->magicDefend);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(speed_d).c_str(), "", property->speed);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(block_d).c_str(), "", Tools::percentToInt(property->blockRate));
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(hp_d).c_str(), "", property->hp);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(critDmg_d).c_str(), "", Tools::percentToInt(property->critDamage));

	return res;
}

void DetialsLayer::updateAttribute()
{
	auto data = GameData::getInstance();

	switch (_type)
	{
	case DT_EQUIPMENT:
	{
		auto property = data->getEquipmentById(_objectId)->getProperty();
		setName(*(property->name));
		setNameColor(property->star);
		setIco(*(property->textureName));
		setAttributeString(propertyToString(property));
		break;
	}
	case DT_HERO:
	{
		auto property = data->getHeroCardById(_objectId)->getProperty();
		setName(*(property->name));
		setNameColor(property->star);
		setIco(StringUtils::format("head%d.png", property->type));
		setAttributeString(propertyToString(property));
		break;
	}
	default:
		break;
	}
}

void DetialsLayer::setName(const std::string& name)
{
	_nameLabel->setString(name);
}

void DetialsLayer::setNameColor(int star)
{
	Color4B color;

	//获得对应星级的颜色值
	switch (star)
	{
	case 1:
		color = Color4B::WHITE;
		break;
	case 2:
		color = Color4B::BLUE;
		break;
	case 3:
		color = Color4B::GREEN;
		break;
	case 4:
		color = Color4B::MAGENTA;
		break;
	case 5:
		color = Color4B::YELLOW;
		break;
	default:
		break;
	}

	_nameLabel->setTextColor(color);
}

void DetialsLayer::setAttributeString(const std::string& content)
{
	_attributeLabel->setString(content);
}

void DetialsLayer::setIco(const std::string& icoName)
{
	_ico->loadTexture(icoName, TextureResType::PLIST);
}

void DetialsLayer::intensify()
{
	switch (_type)
	{
	case DT_EQUIPMENT:
		intensifyEquipment();
		break;
	case DT_HERO:
		intensifyHero();
		break;
	default:
		break;
	}

	updateAttribute();
}

void DetialsLayer::levelup()
{
	switch (_type)
	{
	case DT_EQUIPMENT:
		levelupEquiupment();
		break;
	case DT_HERO:
		levelupHero();
		break;
	default:
		break;
	}

	updateAttribute();
}

void DetialsLayer::startAnimation()
{
	this->setScale(0.0f);

	auto ani = Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.0f), RotateTo::create(0.5f, 720));
	this->runAction(ani);
}

void DetialsLayer::endAnimation()
{
	auto ani = Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.0f), RotateTo::create(0.5f, 720)), CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}

void DetialsLayer::onEnterTransitionDidFinish()
{
	startAnimation();
}

void DetialsLayer::btnCloseCallBack(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		//点击音效
		AudioManager::getInstance()->playClickEffect();

		endAnimation();
	}
}

void DetialsLayer::setBtnIntensifyCallBack(std::function<void(cocos2d::Ref*pSender, cocos2d::ui::Widget::TouchEventType type)> func)
{
	_intensifyFun = func;
}

void DetialsLayer::setBtnLevelupCallBack(std::function<void(cocos2d::Ref*pSender, cocos2d::ui::Widget::TouchEventType type)> func)
{
	_levelupFunc = func;
}
