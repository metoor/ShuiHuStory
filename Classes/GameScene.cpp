#include "GameScene.h"
#include "ParticleLayer.h"
#include "cocostudio/CocoStudio.h"
#include "GameData.h"
#include "HomeLayer.h"


#include "TypeStruct.h"

USING_NS_CC;
using namespace ui;
using namespace std;


GameScene::GameScene()
	:_preTag(0)
{
	GameData::getInstance()->readHeroCard();
	GameData::getInstance()->readEquipment();
}

GameScene::~GameScene()
{
	GameData::getInstance()->saveUniqueIdentifierNumToFile();
	GameData::getInstance()->saveEquipment();
	GameData::getInstance()->saveHeroCard();
}

Scene * GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();

	scene->addChild(layer);

	return scene;
}

bool GameScene::init()
{
	if(!Layer::init())
	{
		return false;
	}

	loadUI();

	//初始化数组为nullptr
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		_layerPointer[index] = nullptr;
	}

	//处理手机返回键的点击事件
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建粒子特效
	auto paricle = ParticleLayer::create();
	addChild(paricle, particle_z_order);

	//启动默认转到Home场景
	_layerPointer[0] = HomeLayer::create();
	addChild(_layerPointer[0]);

	return true;
}

//手机返回键处理回调
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		Director::getInstance()->end();
	}
}

void GameScene::loadUI()
{
	//加载游戏背景
	auto bgLayer = CSLoader::createNode(csbName);
	addChild(bgLayer);

	//加载菜单层
	auto menuLayer = CSLoader::createNode(csbMenuName);
	addChild(menuLayer, menu_z_order);

	//为菜单绑定回调方法
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		auto menu = menuLayer->getChildByTag<Button*>(index);
		menu->addTouchEventListener(CC_CALLBACK_2(GameScene::menuCallBack, this));
	}
}

void GameScene::menuCallBack(Ref * pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		int currentTag = dynamic_cast<Button*>(pSender)->getTag();

		if(_preTag != currentTag && _preTag != none)
		{
			//如果上次的索引和这次的不一样，且不等于none，则需要创建当前索引层，然后释放上次的索引层
			log("--current:%d--pre:%d", currentTag, _preTag);
		}

		_preTag = currentTag;

	}
}
