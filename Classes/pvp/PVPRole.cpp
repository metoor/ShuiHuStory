/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	PVPRole.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/04
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "PVPRole.h"
#include "Tools.h"
#include "TypeStruct.h"
#include "McLog.h"
#include "AudioManager.h"
#include "PVPBattleScene.h"

USING_NS_CC;
using namespace ui;
using std::string;

PVPRole::PVPRole()
	:_isAlive(false),
	_audioIndex(none),
	_isNeedPlayEffect(false)
{
	
}

PVPRole::~PVPRole()
{
	AudioManager::getInstance()->unLoadBattleSceneAudio();
}

bool PVPRole::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	
	return true;
}

void PVPRole::initRole(const HeroCardProperty * hero, int pos)
{
	if (!Tools::betweenAnd(pos, 0, 5))
	{
		McLog mc;
		mc.addError("'pos' out of range...", __FILE__, __LINE__);
		return;
	}

	_roleType = RT_HERO;
	_pos = pos;

	_hp = hero->hp;
	_currentHp = _hp;

	_isAlive = true;

	//创建角色纹理和血条
	initWithFile(*(hero->textureName));
	this->setScale(sprite_scale);
	createHpBar();
}

void PVPRole::initRole(const PVPEnemy * enemy)
{
	_roleType = RT_ENEMY;
	_pos = enemy->pos;

	_hp = enemy->hp;
	_currentHp = _hp;

	_isAlive = true;
	
	//创建角色纹理和血条
	initWithFile(StringUtils::format("car%d.png", enemy->type));
	this->setScale(sprite_scale);
	createHpBar();
}

ActionInterval* PVPRole::createBulletAnimation(int attackId, Point pos)
{
	ActionInterval* action = nullptr;

	if (0 < attackId && attackId < 5)
	{
		//第一种攻击动画
		//计算动画所需要的时间
		float distance = getPosition().distance(pos);
		
		//保证动画的时间不会太长也不会太短在【0.5f， 0.8】之间
		float animationTime = Tools::maxFloat(0.35f, Tools::minFloat(0.8f, distance / bullet_speed));

		action = MoveTo::create(animationTime, pos);

		//音效的名字是从0开始，attackId是从1开始，所以要减一
		_audioIndex = attackId - 1;
	}
	else if (attackId <= 100)
	{
		action = createAnimationWithName("140", animation_time);
		_audioIndex = 8;
	}
	else if (100 < attackId && attackId <= 200)
	{
		action = createAnimationWithName("160", animation_time);
		_audioIndex = 4;
	}
	else if (200 < attackId && attackId <= 300)
	{
		action = createAnimationWithName("150", animation_time);
		_audioIndex = 5;
	}
	else if (300 < attackId && attackId <= 400)
	{
		action = createAnimationWithName("110", animation_time);
		_audioIndex = 6;
	}
	else if (400 < attackId && attackId <= 500)
	{
		action = createAnimationWithName("170", animation_time);
		_audioIndex = 7;
	}

	return action;
}

cocos2d::Sprite * PVPRole::createBllet(int attackId, Point pos)
{
	Sprite* bullet = nullptr;

	if (0 < attackId && attackId < 5)
	{
		//加载子弹贴图
		float angleRotate = calDirection(pos);
		bullet = Sprite::create(StringUtils::format("effect%d.png", attackId));
		bullet->setPosition(this->getPosition());
		bullet->setRotation(angleRotate);
	}else if (attackId <= 100)
	{
		bullet = Sprite::createWithSpriteFrameName("1400.png");
		bullet->setPosition(pos);
	}
	else if (100 < attackId && attackId <= 200)
	{
		bullet = Sprite::createWithSpriteFrameName("1600.png");
		bullet->setPosition(pos);
	}
	else if (200 < attackId && attackId <= 300)
	{
		bullet = Sprite::createWithSpriteFrameName("1500.png");
		bullet->setPosition(pos);
	}
	else if (300 < attackId && attackId <= 400)
	{
		bullet = Sprite::createWithSpriteFrameName("1100.png");
		bullet->setPosition(pos);
	}
	else if (400 < attackId && attackId <= 500)
	{
		bullet = Sprite::createWithSpriteFrameName("1700.png");
		bullet->setPosition(pos);
	}

	//添加到场景中,
	Director::getInstance()->getRunningScene()->addChild(bullet, 100);

	return bullet;
}

cocos2d::ActionInterval * PVPRole::createAnimationWithName(const std::string & name, float delay)
{
	SpriteFrameCache* cache = SpriteFrameCache::getInstance();

	Vector<SpriteFrame*> frameVec;
	SpriteFrame* frame = NULL;
	int index = 0;

	do
	{
		frame = cache->getSpriteFrameByName(StringUtils::format("%s%d.png", name.c_str(), index++));

		//不断的获取SpriteFrame对象，直到获取的值为空是停止
		if (frame == NULL)
		{
			break;
		}

		frameVec.pushBack(frame);
	}while (true);

	auto animation = Animation::createWithSpriteFrames(frameVec);
	animation->setDelayPerUnit(delay / (index - 1));
	animation->setRestoreOriginalFrame(true);
	animation->setLoops(1);
	auto action = Animate::create(animation);

	return action;
}

void PVPRole::attack(int attackId, int dmg, PVPRole * enemy)
{
	Point enemyPos = enemy->getPosition();

	auto bullet = createBllet(attackId, enemyPos);
	auto bulletAction = createBulletAnimation(attackId, enemyPos);

	//扣血动画
	auto displayText = CallFunc::create([&, dmg, enemy]() {
		if(_isNeedPlayEffect)
		{
			_isNeedPlayEffect = false;

			//播放攻击音效
			AudioManager::getInstance()->playAttackEffect(_audioIndex);
		}

		enemy->flyWords(dmg);
		enemy->hurt(dmg);
	});

	//移除子弹动画
	auto removeBullet = CallFunc::create([bullet]() {
		bullet->removeFromParentAndCleanup(true);
	});

	auto action = Sequence::create(bulletAction, displayText, removeBullet, NULL);
	
	//角色攻击前摇
	attackAnimation();

	bullet->runAction(action);
}

void PVPRole::attackAnimation()
{
	auto rotate1 = RotateTo::create(attack_animation_time, 30);

	EaseBounceInOut* bounce1 = EaseBounceInOut::create(rotate1);
	auto *scal = ScaleTo::create(attack_animation_time, 0.5f);

	RotateTo* rotate2 = RotateTo::create(attack_animation_time, 0);
	ScaleTo *scal2 = ScaleTo::create(attack_animation_time, sprite_scale);

	EaseBounceInOut* bounce2 = EaseBounceInOut::create(rotate2);

	Sequence* rotateSequence = Sequence::create(bounce1, scal, bounce2, scal2, NULL);
	this->runAction(rotateSequence);
}

void PVPRole::createHpBar()
{
	auto hpBarBg = Sprite::createWithSpriteFrameName(hp_bar_bg_name);
	hpBarBg->setPosition(this->getContentSize().width / 2, hp_bar_height_pos);
	hpBarBg->setScaleX(hp_bar_scale);
	addChild(hpBarBg);

	m_hpBar = LoadingBar::create(hp_bar_name, Widget::TextureResType::PLIST, 100.0f);
	m_hpBar->setPosition(Point(this->getContentSize().width / 2, hp_bar_height_pos));
	m_hpBar->setScaleX(hp_bar_scale);
	addChild(m_hpBar, 10);
}

void PVPRole::flyWords(int damg, float scale)
{
	auto label = LabelAtlas::create("0123456789", "fonts/fonts.png", 42, 60, '0');
	auto visibleSize = getContentSize();
	
	label->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	label->setAnchorPoint(Point::ANCHOR_MIDDLE);
	label->setString(StringUtils::format("%d", damg));
	label->setScale(scale);
	addChild(label, 100);

	ActionInterval* fade = FadeOut::create(1.2f);

	auto callFun = CallFunc::create([label]() {
		label->removeFromParentAndCleanup(true);
	});

	Sequence* seq = Sequence::createWithTwoActions(fade, callFun);
	label->runAction(seq);
}

//计算两点间的角度
float PVPRole::calDirection(Point pos)
{
	float accDir = 0;
	float vecX = pos.x - getPositionX();
	float vecY = pos.y - getPositionY();

	if (abs(vecX) < 0.00001f)
	{
		if (vecY > 0)
			accDir = 0;
		else if (vecY < 0)
			accDir = PI;
	}
	else
	{
		accDir = atan(vecY / vecX);

		if (vecX >= 0)
			accDir = PI * 0.5 - accDir;
		else
			accDir = PI * 1.5 - accDir;
	}

	return (float)(accDir * 180.0f / PI);
}

void PVPRole::attack(PVPRole attackList[], int attackId, int dmgList[])
{
	_isNeedPlayEffect = true;

	for (int pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int dmg = dmgList[pos];

		if (dmg != 0)
		{
			attack(attackId, dmg, &(attackList[pos]));
		}
	}
}

void PVPRole::hurt(int dmg)
{
	if (_isAlive)
	{
		_currentHp -= Tools::maxInt(0, dmg);
		
		//更新血条
		updateHpBar();
		
		if (_currentHp <= 0)
		{
			//角色已经死亡
			_isAlive = false;
			_currentHp = 0;

			auto fadeout = FadeOut::create(0.5f);
			auto remove = CallFunc::create([&]() {
				this->removeFromParentAndCleanup(true);
			});

			runAction(Sequence::create(fadeout, remove, NULL));
		}
	}
}

void PVPRole::updateHpBar()
{
	m_hpBar->setPercent((_currentHp * 1.0f / _hp) * 100);
}