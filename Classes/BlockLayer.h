#include "cocos2d.h"

/************************************************************************/
/*         ´¥ÃþËø¶¨²ã                                                     */
/************************************************************************/
class BlockLayer : public cocos2d::Layer
{
public:
	CREATE_FUNC(BlockLayer);

	bool init() override
	{
		if (!cocos2d::Layer::init())
			return false;

		auto listener = cocos2d::EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(BlockLayer::onTouchBegan, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		return true;
	}

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override
	{
		return true;
	}
};
