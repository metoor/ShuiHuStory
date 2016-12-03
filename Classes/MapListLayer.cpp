/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	MapListLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/27
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "MapListLayer.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "cocostudio/CocoStudio.h"
#include "BlockLayer.h"
#include "AudioManager.h"
#include "GameData.h"
#include "Config.h"
#include "TypeStruct.h"
#include "I18N.h"
#include "DialogManager.h"
#include "BattleScene.h"

USING_NS_CC;
using namespace ui;
using namespace std;

MapListLayer::MapListLayer()
{
}

MapListLayer::~MapListLayer()
{
}

bool MapListLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//触摸锁定层
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockTagName);
	addChild(blockLayer);

	loadUI();

	return true;
}

void MapListLayer::initWithMapIndex(int index)
{
	loadMap(index);
	loadItem();
}

void MapListLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	_btnClose = node->getChildByName<Button*>(btnCloseName);
	_listView = node->getChildByName<ListView*>(listViewName);
	_tips = node->getChildByName<Text*>(tipsName);

	_btnClose->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			endAnimation();
		}
	});

	//为listView添加点击事件
	_listView->addEventListener(ListView::ccListViewCallback(CC_CALLBACK_2(MapListLayer::onItemSelected, this)));
}

void MapListLayer::loadItem()
{
	//根据mapData里面的数据生成Item
	for (unsigned int index = 0; index < _mapDataVector.size(); ++index)
	{
		auto item = MapListItem::create();
		MapData* mapData = _mapDataVector.at(index);
		auto config = Config::getInstance();
		auto i18n = I18N::getInstance();

		//获取该关卡是否有额外的奖励
		if (mapData->sword != none)
		{
			//该关卡有额外的装备奖励,获取额外奖励的名字
			auto eq = config->getEquipmentByIndex(mapData->sword);
			item->setString(mapData->name, mapData->exp, mapData->gold, StringUtils::format(i18n->getStringByKey(rreward).c_str(), eq->name.c_str()));
		}
		else if (mapData->card != none)
		{
			//该关卡有额外的英雄奖励，获取额外奖励的名字
			auto hero = config->getHeroCardByIndex(mapData->card);
			item->setString(mapData->name, mapData->exp, mapData->gold, StringUtils::format(i18n->getStringByKey(rreward).c_str(), hero->name.c_str()));
		}
		else
		{
			//没有额外的奖励
			item->setString(mapData->name, mapData->exp, mapData->gold, "");
		}

		//将创建好的item添加到ListView
		_listView->pushBackCustomItem(item);
	}

	setTips();
}

void MapListLayer::loadMap(int index)
{
	rapidjson::Document doc;
	std::string jsonpath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("map_%d.json", index));
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
	doc.Parse<0>(contentStr.c_str());

	if (doc.HasParseError()) {
		return;
	}

	for (unsigned int index = 0; index < doc["data"].Size(); ++index)
	{
		rapidjson::Value & json = doc["data"][index];

		auto mapData = new MapData();

		for (int pos = 0; pos < max_battle_hero_num; ++pos)
		{

			mapData->startData[pos] = json["startData"][StringUtils::format("chaPos%d", pos + 1).c_str()].GetInt();
		}

		mapData->name = json["name"].GetString();
		mapData->gold = json["coin"].GetInt();
		mapData->exp = json["xp"].GetInt();
		mapData->type = json["type"].GetInt();
		mapData->chalevel = json["chalevel"].GetInt();
		mapData->card = json["card"].GetInt();
		mapData->sword = json["sword"].GetInt();

		_mapDataVector.push_back(mapData);
	}
}

void MapListLayer::setTips()
{
	int num = _mapDataVector.size();
	_tips->setString(StringUtils::format("%d/%d", num, num));
}

void MapListLayer::startAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//设置位置在屏幕外面
	this->setPosition(visibleSize.width, 0);

	//移动至屏幕中央
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(0, 0)), 0.2f);

	//在移动完成后，显示触摸锁定层
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		//显示触摸锁定层
		dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(160.0f);
	}));

	this->runAction(ani);
}

void MapListLayer::endAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//先隐藏触摸锁层
	dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(0.0f);

	//移动到屏幕外面
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(visibleSize.width, 0)), 0.2f);

	//在完成移动后释放资源
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}

void MapListLayer::onItemSelected(cocos2d::Ref * pSender, cocos2d::ui::ListView::EventType type)
{
	if (type == ListView::EventType::ON_SELECTED_ITEM_END)
	{
		AudioManager::getInstance()->playClickEffect();

		int index = _listView->getCurSelectedIndex();

		//检查是否满足挑战的等级
		auto mapData = _mapDataVector.at(index);
		
		int level = GameData::getInstance()->getLevel();
		
		if (level < mapData->chalevel)
		{
			//玩家等级小于挑战等级，弹出提示框等级不足
			auto i18n = I18N::getInstance();

			DialogManager::getInstance()->showTips(StringUtils::format(i18n->getStringByKey(lvNoEnough).c_str(), mapData->chalevel));
		}
		else
		{
			//启动战斗场景
			auto scene = BattleScene::createScene(mapData);
			Director::getInstance()->pushScene(scene);
		}
	}
}

void MapListLayer::onEnter()
{
	Layer::onEnter();

	startAnimation();
}



/************************************************************************/
/* listview 的item                                                      */
/************************************************************************/

MapListItem::MapListItem()
{
}

MapListItem::~MapListItem()
{
}

bool MapListItem::init()
{
	if (!Layout::init())
	{
		return false;
	}

	loadUI();

	return true;
}

void MapListItem::setString(const std::string & title, const int exp, const int gold, const std::string & reward)
{
	auto i18n = I18N::getInstance();

	_title->setString(title);
	_exp->setString(StringUtils::format(i18n->getStringByKey(rexp).c_str(), exp));
	_gold->setString(StringUtils::format(i18n->getStringByKey(rgold).c_str(), gold));
	_reward->setString(reward);
}

void MapListItem::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//设置大小
	setContentSize(node->getContentSize());

	//设置点击可用
	setTouchEnabled(true);

	_exp = node->getChildByName<Text*>(expName);
	_gold = node->getChildByName<Text*>(goldName);
	_title = node->getChildByName<Text*>(titleName);
	_reward = node->getChildByName<Text*>(rewardName);
}
