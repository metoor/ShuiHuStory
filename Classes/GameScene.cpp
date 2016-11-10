/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	GameScene.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "GameScene.h"
#include "ParticleLayer.h"
#include "cocostudio/CocoStudio.h"
#include "GameData.h"
#include "HomeLayer.h"
#include "AudioManager.h"
#include "ConstantDefine.h"
#include "I18N.h"

USING_NS_CC;
using namespace ui;
using namespace std;


GameScene::GameScene()
	:_preMenu(nullptr)
{
	//初始化游戏数据
	GameData::getInstance();

	AudioManager::getInstance()->preLoadGlobalAudio();

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("head.plist", "head.pvr.ccz");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("equipment.plist", "equipment.pvr.ccz");
	
	I18N::getInstance()->loadStringFile("string.plist");
}

GameScene::~GameScene()
{
	auto gameData = GameData::getInstance();
	gameData->saveUniqueIdentifierNumToFile();
	gameData->saveEquipment();
	gameData->saveHeroCard();
	gameData->saveBattleHero();

	auto audio = AudioManager::getInstance();
	audio->unLoadGlobalEffect();

	gameData->destoryInstance();
	audio->destoryInstance();

	//因为Button是引用计数，所以将引用全部置空
	initArrayToNullptr();
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
	if (!Layer::init())
	{
		return false;
	}

	loadUI();

	initArrayToNullptr();

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
	_preMenu->setEnabled(false);

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

void GameScene::initArrayToNullptr()
{
	//初始化数组为nullptr
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		_layerPointer[index] = nullptr;
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

		//因为开始默认选中第一个按钮
		if (index == 0)
		{
			_preMenu = menu;
		}
	}
}

void GameScene::menuCallBack(Ref * pSender, Widget::TouchEventType type)
{

	if (type == Widget::TouchEventType::BEGAN)
	{
		//播放点击音效
		AudioManager::getInstance()->playClickEffect();
	}

	if (type == Widget::TouchEventType::ENDED)
	{
		//获取当前的点击菜单的tag和上次点击的Tag
		auto currentMenu = dynamic_cast<Button*>(pSender);
		int currentTag = currentMenu->getTag();
		int preTag = _preMenu->getTag();

		if (preTag != currentTag && preTag != none)
		{
			//如果上次的索引和这次的不一样，且不等于none，则需要创建当前索引层，然后释放上次的索引层
			log("--current:%d--pre:%d", currentTag, preTag);
		}

		//不能连续点击相同的菜单，禁用当前的菜单，解禁上一次的菜单
		_preMenu->setEnabled(true);
		currentMenu->setEnabled(false);

		//保存这次点击的菜单指针引用
		_preMenu = currentMenu;

	}
}
