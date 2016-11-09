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
#include "ConstantDefine.h"
#include "GameData.h"
#include "AudioManager.h"
#include "Equipment.h"
#include "HeroCard.h"
#include "I18N.h"
#include "Tools.h"
#include "McLog.h"
#include "AudioManager.h"

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
			dynamic_cast<Button*>(pSender)->setUserData((void*)_objectId);

			intensify();
		}
	});

	_btnLevelup->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED)
		{
			//点击音效
			AudioManager::getInstance()->playClickEffect();
			dynamic_cast<Button*>(pSender)->setUserData((void*)_objectId);

			levelup();
		}
	});
}

string DetialsLayer::propertyToString(const EquipmentProperty * property)
{
	auto i18n = I18N::getInstance();

	string res = StringUtils::format(i18n->getStringByKey(level_d)->c_str(), property->level);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(intensify_d)->c_str(), "+", property->exLevel);
	res += "\n";

	//获得装备的类型
	int type = property->type / 100;

	switch (type)
	{
	case 1:
	case 2:
	{
		//衣服
		res += StringUtils::format(i18n->getStringByKey(define_d)->c_str(), "+", property->defend);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(mDefine_d)->c_str(), "+", property->magicDefend);
		break;
	}
	case 3:
	{
		//武器
		res += StringUtils::format(i18n->getStringByKey(ap_d)->c_str(), "+", property->ap);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(mp_d)->c_str(), "+", property->mp);
		res += "\n";
		res = StringUtils::format(i18n->getStringByKey(crit_d)->c_str(), "+", Tools::percentToInt(property->critRate));
		break;
	}
	case 4:
	{
		//佩戴
		res += StringUtils::format(i18n->getStringByKey(hp_d)->c_str(), "+", property->hp);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(critDmg_d)->c_str(), "+", Tools::percentToInt(property->critDamage));
		break;
	}
	case 5:
	{
		//鞋子
		res += StringUtils::format(i18n->getStringByKey(speed_d)->c_str(), "+", property->speed);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(block_d)->c_str(), "+", Tools::percentToInt(property->blockRate));
		break;
	}
	case 6:
	{
		//坐骑
		res += StringUtils::format(i18n->getStringByKey(speed_d)->c_str(), "+", property->speed);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(hp_d)->c_str(), "+", property->hp);
		res += "\n";
		res = StringUtils::format(i18n->getStringByKey(define_d)->c_str(), "+", property->defend);
		res += "\n";
		res += StringUtils::format(i18n->getStringByKey(mDefine_d)->c_str(), "+", property->magicDefend);
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
	string res = StringUtils::format(i18n->getStringByKey(level_d)->c_str(), property->level);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(intensify_d)->c_str(), "", property->exLevel);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(star_d)->c_str(), property->star);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(crit_d)->c_str(), "", Tools::percentToInt(property->critRate));
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(ap_d)->c_str(), "", property->ap);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(mp_d)->c_str(), "", property->mp);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(define_d)->c_str(), "", property->defend);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(mDefine_d)->c_str(), "", property->magicDefend);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(speed_d)->c_str(), "", property->speed);
	res += "\t";
	res += StringUtils::format(i18n->getStringByKey(block_d)->c_str(), "", Tools::percentToInt(property->blockRate));
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(hp_d)->c_str(), "", property->hp);
	res += "\n";
	res += StringUtils::format(i18n->getStringByKey(critDmg_d)->c_str(), "", Tools::percentToInt(property->critDamage));

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
	auto data = GameData::getInstance();

	switch (_type)
	{
	case DT_EQUIPMENT:
	{
		auto equipment = data->getEquipmentById(_objectId);
		equipment->intensify();
		break;
	}
	case DT_HERO:
	{
		auto hero = data->getHeroCardById(_objectId);
		hero->intensify();
		break;
	}
	default:
		break;
	}

	updateAttribute();
}

void DetialsLayer::levelup()
{
	auto data = GameData::getInstance();

	switch (_type)
	{
	case DT_EQUIPMENT:
	{
		auto equipment = data->getEquipmentById(_objectId);
		equipment->levelup();
		break;
	}
	case DT_HERO:
	{
		auto hero = data->getHeroCardById(_objectId);
		hero->levelup();
		break;
	}
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
