/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	BattleScene.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/01
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef BATTLESCENE_H_  
#define BATTLESCENE_H_  
  
#include "cocos2d.h"
#include "vector"
#include "ConstantDefine.h"

//前置声明
struct MapData;
struct EnemyProperty;
class Role;

class BattleScene : public cocos2d::Layer
{
public:
	BattleScene();
	~BattleScene();
	virtual bool init() override;
	CREATE_FUNC(BattleScene);

	static cocos2d::Scene* createScene(MapData* mapData);

	//设置关卡相关的数据，如：挑战等级，获胜奖励等
	void setMapData(MapData* mapData);

private:
	//战斗流程
	void battle(float dt);

	//获得出战位置一行中第一个不为空的角色索引，如果返回none则表示都为空 row【0， 1】
	int getRowRole(int* list, int row);

	//获得出战位置，某一列不为空的角色，如果都为空则返回none， col【0， 2】
	int getColRole(int* list, int col);

	//获得role的攻击对象
	cocos2d::Vector<Role*>* getEnemyList(Role* role);

	void createBgAndMusic();

	//创建_enemyIndex和_heroIndex到_roleList的映射。
	void initRoleIndex();

	//创建我方英雄和地方角色
	void createRole();

	//将我方角色和敌方角色显示的场景中
	void displayRole();

	//读取敌方的英雄的数据模板
	void readEnemyMould();

	//根据模板计算出敌人的实际属性
	void calEnemyRealProperty();

	//将roleList里的元数按照role->speed从大到小排序
	void sortBySpeed();
	
	//通的对应类型的敌人数据
	EnemyProperty* getEnemyProperty(int type);

	//移除roleList中死亡的角色
	void removeDeadRoleFormRoleList();

	//游戏没有结束返回none， 游戏结数，玩家获胜返回0，电脑获胜返回1
	int isGameOver();

	//判断list中是否还有活着的角色
	bool isAlive(int * list);

	void loadAnimation();
	void unLoadAnimation();


	void onEnter() override;
	void onExit() override;
	void onEnterTransitionDidFinish() override;

	void update(float dt);

private:
	const cocos2d::Point HeroPosition[max_battle_hero_num] = { cocos2d::Point(120, 350), cocos2d::Point(320, 350), cocos2d::Point(520, 350),
		cocos2d::Point(120, 120), cocos2d::Point(320, 120), cocos2d::Point(520, 120) };

	const cocos2d::Point EnemyPosition[max_battle_hero_num] = { cocos2d::Point(120, 610), cocos2d::Point(320, 610), cocos2d::Point(520, 610),
		cocos2d::Point(120, 840), cocos2d::Point(320, 840), cocos2d::Point(520, 840) };

	//当前正在进行攻击行为的角色索引
	const float frequent = 1.5f;

private:
	//当前正在进行攻击行为的角色索引
	int _currentAttackIndex;

	//用来暂存攻击对象，避免每次都度创建容器
	cocos2d::Vector<Role*> _attackList;

	MapData* _mapData;
	cocos2d::Vector<Role*> _roleList;

	//记录角色在_roleList中的索引，none表示不存在
	int _enemyIndex[max_battle_hero_num];
	int _heroIndex[max_battle_hero_num];

	std::vector<EnemyProperty*> _enemyMouldVector;
};
  
#endif // BATTLESCENE_H_ 
