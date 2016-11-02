#include "ParticleLayer.h"

USING_NS_CC;


ParticleLayer::ParticleLayer()
	:_particle(nullptr)
{
}

ParticleLayer::~ParticleLayer()
{
}

bool ParticleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//触摸事件监听
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ParticleLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ParticleLayer::onToucheMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ParticleLayer::onTouchEnded, this);
	
	listener->setSwallowTouches(false);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建粒子特效
	_particle = ParticleSystemQuad::create("particles/stars.plist");
	addChild(_particle);
	changeToBgParticle();


	return true;
}

void ParticleLayer::changeToBgParticle()
{
	Size winSize = Director::getInstance()->getVisibleSize();
	_particle->setPosVar(Vec2(250, 450));
	_particle->setEmissionRate(5.0f);
	_particle->setLife(5.0f);
	_particle->setLifeVar(2.0f);
	_particle->setSpeed(20.0f);
	_particle->setPosition(winSize / 2);
}

void ParticleLayer::changeToClickParticle()
{
	_particle->setPosVar(Vec2(0, 0));
	_particle->setEmissionRate(50.0f);
	_particle->setLife(2.0f);
	_particle->setLifeVar(1.0f);
	_particle->setSpeed(35.0f);
}

bool ParticleLayer::onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event)
{
	changeToClickParticle();
	_particle->setPosition(touch->getLocation());
	return true;
}

void ParticleLayer::onToucheMoved(cocos2d::Touch * touch, cocos2d::Event * event)
{
	_particle->setPosition(touch->getLocation());
}

void ParticleLayer::onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event)
{
	changeToBgParticle();
}
