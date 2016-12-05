/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	PVPBattleScene.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/04
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef PVPBATTLESCENE_H_  
#define PVPBATTLESCENE_H_  
  
#include "cocos2d.h"
#include "ConstantDefine.h"
#include "PVPRole.h"
#include <vector>

struct PVPData;
class PVPRole;

struct BattleProgress
{
	int player;
	int attackHeroPos;
	int dmg[max_battle_hero_num];
	int skillId;
};

class PVPBattleScene : public cocos2d::Layer
{
public:
	PVPBattleScene();
	~PVPBattleScene();
	virtual bool init() override;
	CREATE_FUNC(PVPBattleScene);

	static cocos2d::Scene* createScene(PVPData * pvpData);

private:
	//将字符串解析，还原成战斗数据
	void getBattleProgressFromeString(const std::string & battle);

	void getEnemyData(const std::string& data);

	//创建我方英雄和地方角色
	void createRole();

	//将我方角色和敌方角色显示的场景中
	void displayRole();

	//战斗流程
	void battle(float dt);

	void createBgAndMusic();

	void loadAnimation();
	void unLoadAnimation();

	void onEnter() override;
	void onExit() override;
	void onEnterTransitionDidFinish() override;

private:
	const cocos2d::Point HeroPosition[max_battle_hero_num] = { cocos2d::Point(120, 350), cocos2d::Point(320, 350), cocos2d::Point(520, 350),
		cocos2d::Point(120, 120), cocos2d::Point(320, 120), cocos2d::Point(520, 120) };

	const cocos2d::Point EnemyPosition[max_battle_hero_num] = { cocos2d::Point(120, 610), cocos2d::Point(320, 610), cocos2d::Point(520, 610),
		cocos2d::Point(120, 840), cocos2d::Point(320, 840), cocos2d::Point(520, 840) };

	//每次英雄攻击战斗的时间
	const float frequent = 1.3f;

private:
	int _playerId;
	
	//游戏是否结束
	bool _isGameOver;
	
	//是否胜利
	bool _isWin;

	//开启游戏消耗的金币
	int _gold;

	//当前攻击的索引
	unsigned int _currentIndex;

	PVPRole _enemyArray[max_battle_hero_num];
	PVPRole _heroArray[max_battle_hero_num];

	std::vector<BattleProgress*> _battleVector;
	std::vector<PVPEnemy*> _enemyData;
};
  
#endif // PVPBATTLESCENE_H_ 
