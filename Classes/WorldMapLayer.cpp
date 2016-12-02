#include "WorldMapLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "BlockLayer.h"
#include "ConstantDefine.h"
#include "MapListLayer.h"

USING_NS_CC;
using namespace ui;
using namespace std;

WorldMapLayer::WorldMapLayer()
{
}

WorldMapLayer::~WorldMapLayer()
{
}

bool WorldMapLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//触摸锁定层
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockTagName);
	addChild(blockLayer);

	loadUI();

	return true;
}

void WorldMapLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	_scrollView = node->getChildByName<ScrollView*>(scrollViewName);
	_btnClose = dynamic_cast<Button*>(_scrollView->getChildByName(btnCancelName));

	_btnClose->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			endAnimation();
		}
	});

	for (int index = 0; index < btnMenuEndTag; ++index)
	{
		auto btn = dynamic_cast<Button*>(_scrollView->getChildByTag(index));
		btn->addTouchEventListener(CC_CALLBACK_2(WorldMapLayer::menuCallFun, this));
	}
}

void WorldMapLayer::startAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//设置位置在屏幕外面
	this->setPosition(-visibleSize.width, 0);

	//移动至屏幕中央
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(0, 0)), 0.2f);

	//在移动完成后，显示触摸锁定层
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		//显示触摸锁定层
		dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(160.0f);
	}));

	this->runAction(ani);
}

void WorldMapLayer::endAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//先隐藏触摸锁层
	dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(0.0f);

	//移动到屏幕外面
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(-visibleSize.width, 0)), 0.2f);

	//在完成移动后释放资源
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}

void WorldMapLayer::onEnter()
{
	Layer::onEnter();

	startAnimation();
}

void WorldMapLayer::menuCallFun(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	auto btn = dynamic_cast<Button*>(pSender);
	
	if (type == Widget::TouchEventType::BEGAN)
	{
		AudioManager::getInstance()->playClickEffect();
		
		//选中缩小
		btn->setScale(0.8f);
	}

	if (type == Widget::TouchEventType::CANCELED)
	{
		//取消恢复原始大小
		btn->setScale(1.0f);
	}

	if (type == Widget::TouchEventType::ENDED)
	{
		//结束恢复原始大小
		btn->setScale(1.0f);

		int tag = btn->getTag();
		
		auto layer = MapListLayer::create();
		layer->initWithMapIndex(tag);
		addChild(layer);
	}
}
