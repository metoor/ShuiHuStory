/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	BattleScene.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/12/01
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "BattleScene.h"
#include "Role.h"
#include "GameData.h"
#include "json/stringbuffer.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "TypeStruct.h"
#include "HeroCard.h"
#include "Tools.h"
#include "AudioManager.h"
#include "GameOverLayer.h"
#include "DialogManager.h"
#include "AboutLayer.h"
#include "I18N.h"

USING_NS_CC;
using namespace std;

BattleScene::BattleScene()
	:_mapData(nullptr),
	_currentAttackIndex(0)
{
	AudioManager::getInstance()->preLoadBattleSceneAudio();
	loadAnimation();
	_mapData = new MapData();
	readEnemyMould();
}

BattleScene::~BattleScene()
{
	AudioManager::getInstance()->unLoadBattleSceneAudio();
	unLoadAnimation();

	CC_SAFE_DELETE(_mapData);

	//释放模板占用的资源
	for (int index = _enemyMouldVector.size() - 1; index >= 0; --index)
	{
		CC_SAFE_DELETE(_enemyMouldVector.at(index));
		_enemyMouldVector.pop_back();
	}
}

bool BattleScene::init()
{
	if (!Layer::init())
	{
		return false;;
	}

	//添加手机按键事件监听
	addPhoneEventListener();

	createBgAndMusic();

	return true;
}

cocos2d::Scene * BattleScene::createScene(MapData* mapData)
{
	auto layer = BattleScene::create();
	layer->setMapData(mapData);

	auto scene = Scene::create();
	scene->addChild(layer);

	return scene;
}

void BattleScene::setMapData(MapData * mapData)
{
	_mapData->card = mapData->card;
	_mapData->chalevel = mapData->chalevel;
	_mapData->exp = mapData->exp;
	_mapData->gold = mapData->gold;
	_mapData->sword = mapData->sword;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int type = mapData->startData[pos];

		if (type > 0)
		{
			_mapData->startData[pos] = type;
		}
		else
		{
			_mapData->startData[pos] = none;
		}
	}

	//根据挑战等级计算实际的怪物属性
	calEnemyRealProperty();
}

void BattleScene::battle(float dt)
{
	if (_currentAttackIndex >= _roleList.size())
	{
		//因为删除了角色所以造成了角色会往前移动，
		_currentAttackIndex = _roleList.size() - 1;
	}

	auto role = _roleList.at(_currentAttackIndex);
	if (role->getIsAlive())
	{
		//英雄还活着
		auto enemyList = getEnemyList(role);
		role->attack(enemyList);

		//索引循环
		_currentAttackIndex = (++_currentAttackIndex) % _roleList.size();
	}

}

int BattleScene::getRowRole(int * list, int row)
{
	CCAssert(row < 2, "col must be less than 2");

	int result = none;

	//从左至右，返回第一个活着的角色的索引，如果都已经死亡，则返回none
	for (int pos = 0 + row * 3; pos < 3 + 3 * row; ++pos)
	{
		int index = list[pos];
		if (index != none)
		{
			result = index;
			break;
		}
	}

	return result;
}

int BattleScene::getColRole(int * list, int col)
{
	CCAssert(col < 3, "col must be less than 3");

	int result = none;

	int row1 = list[col];
	int row2 = list[col + 3];

	//从col的第一排到第二排，返回第一个活着的角色的索引，如果都已经死亡则返回none
	if (row1 != none)
	{
		result = row1;
	}
	else if (row2 != none)
	{
		result = row2;
	}

	return result;
}

void BattleScene::createBgAndMusic()
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

void BattleScene::initRoleIndex()
{
	//初始化列表为none
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		_enemyIndex[pos] = none;
		_heroIndex[pos] = none;
	}

	for (int index = 0; index < _roleList.size(); ++index)
	{
		auto role = _roleList.at(index);

		if (role->getRoleType() == RT_HERO)
		{
			//角色类型为Hero
			_heroIndex[role->getPos()] = index;
		}
		else
		{
			//角色类型为怪物
			_enemyIndex[role->getPos()] = index;
		}
	}
}

void BattleScene::createRole()
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
			auto heroRole = Role::create();
			heroRole->initRole(property, pos);

			_roleList.pushBack(heroRole);
		}
	}

	//创建敌方角色
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int enemyType = _mapData->startData[pos];

		if (enemyType != none)
		{
			//该位置有英雄，创建英雄
			auto property = getEnemyProperty(enemyType);
			auto enemyRole = Role::create();
			enemyRole->initRole(property, pos);

			_roleList.pushBack(enemyRole);
		}
	}

	//按照speed进行排序，因为后面速度高的优先攻击
	sortBySpeed();

	//根据创建好的角色表，创建索引
	initRoleIndex();
}

void BattleScene::displayRole()
{
	//显示英雄
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int index = _heroIndex[pos];

		if (index != none)
		{
			auto role = _roleList.at(index);
			role->setPosition(HeroPosition[pos]);
			addChild(role);
		}
	}

	//显示怪物
	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int index = _enemyIndex[pos];

		if (index != none)
		{
			auto role = _roleList.at(index);
			role->setPosition(EnemyPosition[pos]);
			addChild(role);
		}
	}
}

void BattleScene::readEnemyMould()
{
	string filepath = FileUtils::getInstance()->fullPathForFilename("datas/enemy.json");
	rapidjson::Document doc;

	string content = FileUtils::getInstance()->getStringFromFile(filepath);

	doc.Parse<0>(content.c_str());

	if (doc.HasParseError()) {
		CCAssert(false, "Json::cloth.json Reader Parse error!");
	}

	if (!doc["data"].IsNull() && doc["data"].IsArray())
	{

		for (unsigned int inex = 0; inex < doc["data"].Size(); ++inex)
		{
			rapidjson::Value & json = doc["data"][inex];

			auto enemy = new EnemyProperty();

			enemy->ap = json["ap"].GetInt();
			enemy->mp = json["mp"].GetInt();
			enemy->hp = json["hp"].GetInt();
			enemy->type = json["type"].GetInt();
			enemy->attackId = json["attackid"].GetInt();
			enemy->skillId = json["skillsid"].GetInt();
			enemy->defend = json["defend"].GetInt();
			enemy->mDefend = json["mDefine"].GetInt();
			enemy->block = json["block"].GetInt();
			enemy->crit = json["crit"].GetInt();
			enemy->critDmg = json["critDmg"].GetInt();
			enemy->speed = json["speed"].GetInt();
			enemy->isMagic = json["isMagic"].GetBool();

			_enemyMouldVector.push_back(enemy);
		}
	}
}

void BattleScene::calEnemyRealProperty()
{
	for (unsigned int index = 0; index < _enemyMouldVector.size(); ++index)
	{
		auto enemyProerty = _enemyMouldVector.at(index);

		//每提升一级，所增加的属性
		int hp = 50;
		int ap = 20;
		int df = 10;
		int speed = 2;
		int critDmg = 1;
		float rate = 0.5;

		int chalevel = _mapData->chalevel;

		enemyProerty->ap += chalevel * ap;
		enemyProerty->mp += chalevel * ap;
		enemyProerty->hp += chalevel * hp;
		enemyProerty->defend += chalevel * df;
		enemyProerty->mDefend += chalevel * df;
		enemyProerty->block += (int)(chalevel * rate);
		enemyProerty->crit += (int)(chalevel * rate);
		enemyProerty->critDmg += chalevel * critDmg;
		enemyProerty->speed += chalevel * speed;
	}
}

void BattleScene::sortBySpeed()
{
	int length = _roleList.size();

	for (int i = 0; i < length - 1; ++i)
	{
		int swapIndex = none;
		int speed1 = _roleList.at(i)->getSpeed();

		for (int j = i + 1; j < length; ++j)
		{
			int speed2 = _roleList.at(j)->getSpeed();

			if (speed1 < speed2)
			{
				//发现更大的数，记录这个更大的数的索引，以便在完成本次查找后交换位置
				swapIndex = j;
				speed1 = speed2;
			}
		}

		if (swapIndex != none)
		{
			//和本次找到的最大的数交换位置
			_roleList.swap(i, swapIndex);
		}
	}
}

EnemyProperty * BattleScene::getEnemyProperty(int type)
{
	//type是从201开始，所以需要减去200，转化成容器的索引
	int index = type - 200;

	return _enemyMouldVector.at(index);
}

void BattleScene::removeDeadRoleFormRoleList()
{
	for (int index = 0; index < _roleList.size(); ++index)
	{
		auto role = _roleList.at(index);
		int pos = role->getPos();

		if (!role->getIsAlive())
		{
			//移除死亡的角色
			if (role->getRoleType() == RT_HERO)
			{
				//死亡的是我方英雄
				_heroIndex[pos] = none;
			}
			else
			{
				//死亡的是敌方的英雄
				_enemyIndex[pos] = none;
			}

			_roleList.erase(index);

			//因为移除了角色，所以索引表也需要更新
			initRoleIndex();
		}
	}
}


void BattleScene::onEnter()
{
	Layer::onEnter();

	//创建角色
	createRole();

	//显示角色
	displayRole();
}

void BattleScene::onExit()
{
	Layer::onExit();

	//取消定时器
	unschedule(schedule_selector(BattleScene::battle));
	this->unscheduleUpdate();

	//关闭背景音乐
	AudioManager::getInstance()->playGamingSceneBgMusic();
}

void BattleScene::onEnterTransitionDidFinish()
{
	//启动定时器
	schedule(schedule_selector(BattleScene::battle), frequent);
	this->scheduleUpdate();
}

void BattleScene::update(float dt)
{
	int gameover = isGameOver();

	switch (gameover)
	{
	case 0:
	case 1:
	{
		//先停止定时器
		unschedule(schedule_selector(BattleScene::battle));
		this->unscheduleUpdate();

		//创建游戏结算场景
		auto delay = DelayTime::create(1.2f);
		auto gameOverCallback = CallFunc::create([&, gameover]() {
			
			auto overLayer = GameOverLayer::create();
			bool isWin = gameover == 0;
			ExtraRewardType rewardType = ERT_NONE;
			int type = none;
			
			//获取额外奖励的类型
			if (_mapData->card != none)
			{
				rewardType = ERT_CARD;
				type = _mapData->card;
			}
			else if (_mapData->sword != none)
			{
				rewardType = ERT_EQUIPMENT;
				type = _mapData->sword;
			}
			
			overLayer->initLayer(isWin, rewardType, type, _mapData->gold, _mapData->exp);
			addChild(overLayer);
		});

		runAction(Sequence::createWithTwoActions(delay, gameOverCallback));

		break;
	}
	case -1:
		//游戏还没有结束
		removeDeadRoleFormRoleList();
		break;
	default:
		break;
	}
}

cocos2d::Vector<Role*>* BattleScene::getEnemyList(Role * role)
{
	//清空上次的数据
	_attackList.clear();

	int * indexList = nullptr;
	int attackId = role->getAttackId();

	if (role->getRoleType() == RT_HERO)
	{
		//攻击的目标是怪物
		indexList = _enemyIndex;
	}
	else
	{
		//攻击的目标是英雄
		indexList = _heroIndex;
	}

	if (attackId >= 0 && attackId <= 100)	//攻击单体目标
	{
		int currentPos = role->getPos();

		//优先攻击前排
		if (currentPos > 2)
		{
			currentPos -= 3;
		}

		int attackIndex = getColRole(indexList, currentPos);
		if (attackIndex != none)
		{
			//和自己同一列有角色，优先攻击第一排，如果第一排没角色，则攻击第二排
			_attackList.pushBack(_roleList.at(attackIndex));
		}
		else if ((attackIndex = getRowRole(indexList, 0)) != none)
		{
			//和攻击英雄相同的一列没有角色，第一排其他位置有角色，优先攻击第一排，
			_attackList.pushBack(_roleList.at(attackIndex));
		}
		else if ((attackIndex = getRowRole(indexList, 1)) != none)
		{
			//和攻击英雄相同的一列没有角色，第一排也没有角色，攻击第二排角色，
			_attackList.pushBack(_roleList.at(attackIndex));
		}
	}
	else if (100 < attackId && attackId <= 200)	//攻击全体目标
	{
		for (int pos = 0; pos < max_battle_hero_num; ++pos)
		{
			int attackIndex = indexList[pos];
			if (attackIndex != none)
			{
				//只要有英雄救添加到攻击列表
				_attackList.pushBack(_roleList.at(attackIndex));
			}
		}
	}
	else if (200 < attackId && attackId <= 300)	//攻击前排目标
	{
		if (getRowRole(indexList, 0) != none)
		{
			//第一排有英雄，优先攻击第一排
			for (int pos = 0; pos < 3; ++pos)
			{
				int attackIndex = indexList[pos];
				if (attackIndex != none)
				{
					_attackList.pushBack(_roleList.at(attackIndex));
				}
			}
		}
		else if (getRowRole(indexList, 1) != none)
		{
			//第一排没有英雄，但是第二排有英雄
			for (int pos = 3; pos < max_battle_hero_num; ++pos)
			{
				int attackIndex = indexList[pos];
				if (attackIndex != none)
				{
					_attackList.pushBack(_roleList.at(attackIndex));
				}
			}
		}
	}
	else if (300 < attackId && attackId <= 400)	//攻击后排目标
	{
		if (getRowRole(indexList, 1) != none)
		{
			//第二排存在英雄，优先攻击第二排
			for (int pos = 3; pos < max_battle_hero_num; ++pos)
			{
				int attackIndex = indexList[pos];
				if (attackIndex != none)
				{
					_attackList.pushBack(_roleList.at(attackIndex));
				}
			}
		}
		else if (getRowRole(indexList, 0) != none)
		{
			//第二排没有英雄，但是第一排有英雄，攻击第一排
			for (int pos = 0; pos < 3; ++pos)
			{
				int attackIndex = indexList[pos];
				if (attackIndex != none)
				{
					_attackList.pushBack(_roleList.at(attackIndex));
				}
			}
		}
	}
	else if (400 < attackId && attackId <= 500)	//攻击竖排目标
	{
		//优先攻击和自己同一列的英雄
		int col = role->getPos();

		if (col > 2)
		{
			//英雄的位置在第二行，计算其对应的列
			col -= 3;
		}

		if (getColRole(indexList, col) != none)
		{
			//和自己同一列的敌方有英雄
			for (int pos = col; pos < max_battle_hero_num; pos += 3)
			{
				int attackIndex = indexList[pos];
				if (attackIndex != none)
				{
					_attackList.pushBack(_roleList.at(attackIndex));
				}
			}
		}
		else
		{
			//和自己同一列的敌方没有英雄，则优先攻击下一列
			int nextCol = (col + 1) % 3;
			while (nextCol != col)
			{
				if (getColRole(indexList, nextCol) != none)
				{
					for (int pos = nextCol; pos < max_battle_hero_num; pos += 3)
					{
						int attackIndex = indexList[pos];
						if (attackIndex != none)
						{
							_attackList.pushBack(_roleList.at(attackIndex));
						}
					}
					
					//已经获取到攻击对象，跳出循环
					break;
				}
				nextCol = (++nextCol) % 3;
			}
		}
	}

	return &_attackList;
}

int BattleScene::isGameOver()
{
	int result = none;

	int heroIsAlive = isAlive(_heroIndex);
	int enemyIsAlive = isAlive(_enemyIndex);

	if (heroIsAlive && enemyIsAlive)
	{
		result = none;
	}
	else if (heroIsAlive)
	{
		result = 0;
	}
	else
	{
		result = 1;
	}

	return result;
}

bool BattleScene::isAlive(int * list)
{
	bool isAlive = false;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int index = list[pos];
		if (index != none)
		{
			isAlive = true;
			break;
		}
	}

	return isAlive;
}

void BattleScene::loadAnimation()
{
	//加载动画缓存
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect11.plist", "effect11.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect14.plist", "effect14.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect15.plist", "effect15.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect16.plist", "effect16.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("effect17.plist", "effect17.png");
}

void BattleScene::unLoadAnimation()
{
	//释放动画缓存
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect11.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect14.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect15.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect16.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("effect17.plist");
}

void BattleScene::addPhoneEventListener()
{
	//对手机返回键的监听
	auto listener = EventListenerKeyboard::create();

	//和回调函数绑定
	listener->onKeyReleased = CC_CALLBACK_2(BattleScene::onKeyReleased, this);

	//添加到事件分发器中
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void BattleScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * pEvent)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		//弹出退出游戏提醒
		auto i18n = I18N::getInstance();
		DialogManager::getInstance()->showDialog(i18n->getStringByKey(qtitle), i18n->getStringByKey(back), [](Ref* psender) {
			Director::getInstance()->popScene();
		});
	}
}