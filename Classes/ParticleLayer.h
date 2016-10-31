#ifndef __PARTICLELAYER_H__
#define __PARTICLELAYER_H__

#include "cocos2d.h"

class ParticleLayer : public cocos2d::Layer {
public:
	ParticleLayer();
	~ParticleLayer();

	bool init() override;
	CREATE_FUNC(ParticleLayer);

	void changeToBgParticle();
	void changeToClickParticle();

private:
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
	virtual void onToucheMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

private:
	cocos2d::ParticleSystemQuad* _particle;
};

#endif // PARTICLELAYER_H_