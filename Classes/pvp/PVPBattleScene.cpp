/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	PVPBattleScene.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/12/04
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "PVPBattleScene.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "McLog.h"
#include "AudioManager.h"
#include "Tools.h"
#include "TypeStruct.h"
#include "GameData.h"
#include "HeroCard.h"
#include "GameOverLayer.h"

USING_NS_CC;
using namespace std;

PVPBattleScene::PVPBattleScene()
	:_isGameOver(false),
	_gold(0),
	_currentIndex(0),
	_isWin(false)
{
	AudioManager::getInstance()->preLoadBattleSceneAudio();
	loadAnimation();
}

PVPBattleScene::~PVPBattleScene()
{
	AudioManager::getInstance()->unLoadBattleSceneAudio();
	unLoadAnimation();

	//释放战斗流程数据
	for (int index = _battleVector.size() - 1; index >= 0; --index)
	{
		CC_SAFE_DELETE(_battleVector.at(index));
		_battleVector.pop_back();
	}

	//释放地方玩家数据
	for (int index = _enemyData.size() - 1; index >= 0; --index)
	{
		CC_SAFE_DELETE(_enemyData.at(index));
		_enemyData.pop_back();
	}
}

bool PVPBattleScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	createBgAndMusic();

	return true;
}

cocos2d::Scene * PVPBattleScene::createScene(PVPData * pvpData)
{
	auto layer = PVPBattleScene::create();
	layer->_gold = pvpData->gold;

	if (pvpData->playerId == 3)
	{
		layer->_isGameOver = true;
	}
	else
	{
		//解析json数据
		layer->getBattleProgressFromeString(pvpData->battleData);
		layer->getEnemyData(pvpData->enemyData);
		layer->_playerId = pvpData->playerId;
	}

	auto scene = Scene::create();
	scene->addChild(layer);

	return scene;
}

void PVPBattleScene::getBattleProgressFromeString(const std::string & battle)
{
	rapidjson::Document doc;
	doc.Parse<0>(battle.c_str());

	if (doc.HasParseError())
	{
		CCASSERT(false, "json can not parse!");
		//错误记录
		McLog mc;
		mc.addError("json can not parse!", __FILE__, __LINE__);
		return;
	}

	for (unsigned int index = 0; index < doc["battle"].Size(); ++index)
	{
		rapidjson::Value & json = doc["battle"][index];

		auto bp = new BattleProgress();

		bp->player = json["ap"].GetInt();
		bp->attackHeroPos = json["rp"].GetInt();
		bp->skillId = json["ai"].GetInt();
		
		for (int pos = 0; pos < max_battle_hero_num; ++pos)
		{
			string key = StringUtils::format("d%d", pos);
			bp->dmg[pos] = json[key.c_str()].GetInt();
		}

		_battleVector.push_back(bp);
	}
}

void PVPBattleScene::getEnemyData(const std::string & data)
{
	rapidjson::Document doc;
	doc.Parse<0>(data.c_str());

	if (doc.HasParseError())
	{
		CCASSERT(false, "json can not parse!");
		//错误记录
		McLog mc;
		mc.addError("json can not parse!", __FILE__, __LINE__);
		return;
	}

	for (unsigned int index = 0; index < doc["role"].Size(); ++index)
	{
		rapidjson::Value & json = doc["role"][index];

		auto enemy = new PVPEnemy();

		enemy->type = json["t"].GetInt();

		//type != none才说明有角色存在
		if (enemy->type != none)
		{
			enemy->pos = json["p"].GetInt();
			enemy->ap = json["ap"].GetInt();
			enemy->mp = json["mp"].GetInt();
			enemy->hp = json["hp"].GetInt();
			enemy->attackId = json["ai"].GetInt();
			enemy->skillId = json["si"].GetInt();
			enemy->defend = json["df"].GetInt();
			enemy->magicDefend = json["mdf"].GetInt();
			enemy->blockRate = json["br"].GetInt();
			enemy->critRate = json["cr"].GetInt();
			enemy->critDamage = json["cd"].GetInt();
			enemy->speed = json["sp"].GetInt();
			enemy->isMagic = json["im"].GetBool();

			_enemyData.push_back(enemy);
		}
	}
}

void PVPBattleScene::createRole()
{
	auto gameData = GameData::getInstance();

	//创建英雄角色
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int heroId = gameData->getBattleHeroId(pos);

		if (heroId != none)
		{
			//该位置有英雄，创建英雄
			auto property = gameData->getHeroCardById(heroId)->getProperty();
			_heroArray[pos].initRole(property, pos);
		}
	}

	//创建敌方角色
	for (unsigned int pos = 0; pos < _enemyData.size(); ++pos)
	{
		auto enemy = _enemyData.at(pos);

		//创建敌方角色,并将角色放到正确的位置上
		_enemyArray[enemy->pos].initRole(enemy);
	}
}

void PVPBattleScene::displayRole()
{
	//显示英雄
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		PVPRole* role = &_heroArray[pos];

		if (role->getIsAlive())
		{
			role->setPosition(HeroPosition[pos]);
			addChild(role);
		}
	}

	//显示怪物
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		PVPRole* role = &_enemyArray[pos];

		if (role->getIsAlive())
		{
			role->setPosition(EnemyPosition[pos]);
			addChild(role);
		}
	}
}

void PVPBattleScene::battle(float dt)
{
	if (_isGameOver)
	{
		//取消定时器
		unschedule(schedule_selector(PVPBattleScene::battle));

		//延时启动游戏结算场景
		auto delay = DelayTime::create(_delay);
		auto gameOverCallback = CallFunc::create([&]() {
			auto data = GameData::getInstance();

			//计算奖励
			int rewardGold = _gold * 2 * 0.9f;
			int level = data->getLevel();
			int rewardExp = data->getExpLimit(start_exp_rate, level) / level;

			//创建游戏结束创景
			auto overLayer = GameOverLayer::create();
			overLayer->initLayer(_isWin, ERT_NONE, none, rewardGold, rewardExp);
			addChild(overLayer);
		});

		runAction(Sequence::createWithTwoActions(delay, gameOverCallback));
	}
	else
	{
		auto battle = _battleVector.at(_currentIndex++);

		if (_playerId == battle->player)
		{
			auto hero = &_heroArray[battle->attackHeroPos];
			hero->attack(_enemyArray, battle->skillId, battle->dmg);
		}
		else
		{
			auto enemy = &_enemyArray[battle->attackHeroPos];
			enemy->attack(_heroArray, battle->skillId, battle->dmg);
		}

		if (_currentIndex >= _battleVector.size())
		{
			//已经达到数据最后，结束游戏
			_isGameOver = true;

			//设置延时启动的时间
			_delay = 1.2f;

			auto last = _battleVector.at(_battleVector.size() - 1);

			//判断游戏胜负
			if (last->player == _playerId)
			{
				_isWin = true;
			}
			else
			{
				_isWin = false;
			}
		}
	}
}

void PVPBattleScene::createBgAndMusic()
{
	//创建背景
	int randomIndex = Tools::getRandomInt(0, 2);
	Sprite * bg = Sprite::create(StringUtils::format("warbg%d.png", randomIndex));
	bg->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
	bg->setPosition(Point::ZERO);
	addChild(bg);

	//播放背景音乐
	AudioManager::getInstance()->playBattleSceneBgMusic();
}

void PVPBattleScene::loadAnimation()
{
	//加载动画缓存
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect11.plist", "effect11.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect14.plist", "effect14.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect15.plist", "effect15.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect16.plist", "effect16.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect17.plist", "effect17.png");
}

void PVPBattleScene::unLoadAnimation()
{
	//释放动画缓存
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect11.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect14.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect15.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect16.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect17.plist");
}

void PVPBattleScene::onEnter()
{
	Layer::onEnter();

	//扣除开始本局游戏的金币
	GameData::getInstance()->setGold(-_gold);

	if (!_isGameOver)
	{
		//创建角色
		createRole();

		//显示角色
		displayRole();
	}
	else
	{
		//游戏获胜
		_isWin = true;
	}
}

void PVPBattleScene::onExit()
{
	Layer::onExit();

	//取消定时器
	unschedule(schedule_selector(PVPBattleScene::battle));

	//关闭背景音乐
	AudioManager::getInstance()->playGamingSceneBgMusic();
}

void PVPBattleScene::onEnterTransitionDidFinish()
{
	//启动定时器
	schedule(schedule_selector(PVPBattleScene::battle), frequent);
}
