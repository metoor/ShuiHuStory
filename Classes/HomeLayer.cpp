#include "HomeLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "EquipmentDisplayLayer.h"
#include "HeroDisplayLayer.h"

USING_NS_CC;
using namespace ui;
using namespace std;

HomeLayer::HomeLayer()
	:_preMenu(nullptr)
{
}

HomeLayer::~HomeLayer()
{
}

bool HomeLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	loadUI();

	//初始化数组为nullptr
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		_layerPointer[index] = nullptr;
	}

	return true;
}

void HomeLayer::loadUI()
{
	//加载编辑好的主页场景文件
	auto homeLayer = CSLoader::createNode(csbName);
	addChild(homeLayer);

	//绑定菜单的回调方法
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		auto menu = homeLayer->getChildByTag<Button*>(index);
		menu->addTouchEventListener(CC_CALLBACK_2(HomeLayer::menuCallBack, this));
	}
}

Layer* HomeLayer::createMenuLayer(HomeMenuType type)
{
	Layer* layer = nullptr;

	switch (type)
	{
	case EQUIPMENT:
		layer = EquipmentDisplayLayer::create();
		break;
	case HERO:
		layer = HeroDisplayLayer::create();
		break;
	case LOGIN:
		break;
	case LEVEL_UP:
		break;
	case ABOUT:
		break;
	case AUDIO:
		break;
	default:
		break;
	}

	return layer;
}

void HomeLayer::menuCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::BEGAN)
	{
		//播放点击音效
		auto audio = AudioManager::getInstance();
		audio->playEffect(audio->clickEffect);
	}

	if (type == Widget::TouchEventType::ENDED)
	{
		int currentTag = dynamic_cast<Button*>(pSender)->getTag();
		auto layer = createMenuLayer(static_cast<HomeMenuType>(currentTag));
		addChild(layer);
	}
}
