/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	Role.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/28
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef ROLE_H_  
#define ROLE_H_  

#include "cocos2d.h"
#include "ConstantDefine.h"
#include "ui/CocosGUI.h"

//前置声明
struct HeroCardProperty;
struct EnemyProperty;

enum RoleType{
	RT_HERO,
	RT_ENEMY
};

class Role : public cocos2d::Sprite
{
public:
	Role();
	~Role();
	virtual bool init();
	CREATE_FUNC(Role);

	void initRole(const HeroCardProperty* hero, int pos);
	void initRole(const EnemyProperty* enemy, int pos);

	void attack(cocos2d::Vector<Role*> * attackList);

	//获得攻击的方式Id
	int getAttackId();

private:
	//获得角色实际的攻击力
	int getDamage();

	//扣血,返回值为受到的真实伤害
	void hurt(int dmg);

	//攻击单个敌人
	void attack(Role* enemy);

	//计算子弹旋转的角度
	float calDirection(cocos2d::Point pos);

	//角色自身的攻击动画
	void attackAnimation();

	cocos2d::Sprite* createBllet(cocos2d::Point pos);

	//创建子弹运动的动画
	cocos2d::ActionInterval* createBulletAnimation(cocos2d::Point pos);

	cocos2d::ActionInterval* createAnimationWithName(const std::string &name, float delay);

	void flyWords(int damg, float scale = 2.0f);

	void createHpBar();

	void updateHpBar();

	//获得受到的真实伤害 敌方攻击 - 我方防御(包括触发格挡效果)
	int getRealDmg(int dmg, bool isMagic);

private:
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

	CC_SYNTHESIZE(bool, _isMagic, IsMagic);
	CC_SYNTHESIZE(int, _ap, Ap);
	CC_SYNTHESIZE(int, _mp, Mp);
	CC_SYNTHESIZE(int, _define, Define);
	CC_SYNTHESIZE(int, _mDefine, mDefine);
	CC_SYNTHESIZE(int, _block, Block);
	CC_SYNTHESIZE(int, _crit, Crit);
	CC_SYNTHESIZE(float, _critDmg, CritDmg);
	CC_SYNTHESIZE(int, _speed, Speed);
	CC_SYNTHESIZE(int, _hp, Hp);
	CC_SYNTHESIZE(int, _skillId, SkillId);	//技能攻击Id
	CC_SYNTHESIZE(int, _norAtId, NorAtId);	//普通攻击Id

	//记录是第几次攻击
	int _attickNum;

	//是否需要播放攻击音效，避免在攻击多个目标是音效被重复播放
	bool _isNeedPlayEffect;

	//音效的索引
	int _audioIndex;

	//血条
	cocos2d::ui::LoadingBar* m_hpBar;
};

#endif // ROLE_H_
