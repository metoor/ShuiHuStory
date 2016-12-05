/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	PVPRole.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/04
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef PVPROLE_H_  
#define PVPROLE_H_  

#include "cocos2d.h"
#include "ConstantDefine.h"
#include "ui/CocosGUI.h"

//前置声明
struct HeroCardProperty;
struct EnemyProperty;
struct PVPEnemy;

enum RoleType{
	RT_HERO,
	RT_ENEMY
};

struct PVPEnemy
{
	bool isExist;
	int totalhp;
	int attackid;
	int skillsid;
	int totaldefend;
	int totalap;
	std::string textureName;
};

class PVPRole : public cocos2d::Sprite
{
public:
	PVPRole();
	~PVPRole();
	virtual bool init();
	CREATE_FUNC(PVPRole);

	void initRole(const HeroCardProperty* hero, int pos);

	void initRole(const PVPEnemy * enemy, int pos);

	void attack(PVPRole attackList[], int attackId, int dmgList[]);

private:
	//扣血
	void hurt(int dmg);

	//攻击单个敌人
	void attack(int attackId, int dmg, PVPRole* enemy);

	//计算子弹旋转的角度
	float calDirection(cocos2d::Point pos);

	//角色自身的攻击动画
	void attackAnimation();

	cocos2d::Sprite* createBllet(int attackId, cocos2d::Point pos);

	//创建子弹运动的动画
	cocos2d::ActionInterval* createBulletAnimation(int attackId, cocos2d::Point pos);

	cocos2d::ActionInterval* createAnimationWithName(const std::string &name, float delay);

	void flyWords(int damg, float scale = 2.0f);

	void createHpBar();

	void updateHpBar();

	//获得真实的伤害 敌方攻击 - 我方防御
	int getRealDmg(int dmg);

private:
	const float PI = 3.1415926f;
	const int sprite_height = 150;
	const int sprite_width = 200;
	const int hp_bar_height_pos = 30;

	//使用技能需要完成普通攻击的次数
	const int using_skill = 2;

	const float hp_bar_scale = 1.2f;
	const float sprite_scale = 0.6f;

	const float attack_animation_time = 0.15f;
	const float animation_time = 0.5f;
	const float bullet_speed = 900.0f;

	const std::string hp_bar_bg_name = "icos/hpBarBg.png";
	const std::string hp_bar_name = "icos/hpBar.png";

private:
	//get和set方法
	CC_SYNTHESIZE(int, _currentHp, CurrentHp);
	CC_SYNTHESIZE(bool, _isAlive, IsAlive);
	CC_SYNTHESIZE(RoleType, _roleType, RoleType);
	CC_SYNTHESIZE(int, _pos, Pos)

	CC_SYNTHESIZE(int, _define, Define);
	CC_SYNTHESIZE(int, _hp, Hp);

	//是否需要播放攻击音效，避免在攻击多个目标是音效被重复播放
	bool _isNeedPlayEffect;

	//音效的索引
	int _audioIndex;

	//血条
	cocos2d::ui::LoadingBar* m_hpBar;
};

#endif // PVPROLE_H_ 
