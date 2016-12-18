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
#include "AudioManager.h"
#include "ConstantDefine.h"
#include "I18N.h"
#include "Config.h"
#include "HomeLayer.h"
#include "TeamLayer.h"
#include "HeroLayer.h"
#include "PictureLayer.h"
#include "StoreLayer.h"
#include "WarMenuLayer.h"
#include "DialogManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;


GameScene::GameScene()
	:_preMenu(nullptr),
	_preLayer(nullptr),
	_currentLayer(nullptr)
{
	//初始化游戏数据管理类
	if (GameData::isNullptr())
	{
		Config::getInstance();
		GameData::getInstance()->readData();
		I18N::getInstance()->loadData();
		AudioManager::getInstance()->preLoadGlobalAudio();
	}

	//异步加载游戏ui
	Director::getInstance()->getTextureCache()->addImageAsync("ui/ui_1", [&](Texture2D* texture) {
		log("ui_1.png load finsh..");
	});
	
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("head.plist", "head.pvr.ccz");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("equipment.plist", "equipment.pvr.ccz");
}

GameScene::~GameScene()
{
	//释放游戏数据
	I18N::getInstance()->loadStringFile("string.plist");
	GameData::getInstance()->destoryInstance();
	Config::getInstance()->destoryInstance();
	AudioManager::getInstance()->destoryInstance();
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

	//处理手机返回键的点击事件
	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//创建粒子特效
	auto paricle = ParticleLayer::create();
	addChild(paricle, particle_z_order);

	//启动默认转到Home场景
	_preLayer = HomeLayer::create();
	addChild(_preLayer);
	_preMenu->setEnabled(false);

	//游戏背景音乐
	AudioManager::getInstance()->playGamingSceneBgMusic();

	return true;
}

cocos2d::Layer * GameScene::createLayer(GameMenuType type)
{
	Layer* layer = nullptr;

	switch (type)
	{
	case GMT_HOME:
		layer = HomeLayer::create();
		break;
	case GMT_TEAM:
		layer = TeamLayer::create();
		break;
	case GMT_HERO:
		layer = HeroLayer::create();
		break;
	case GMT_WAR:
		layer = WarMenuLayer::create();
		break;
	case GMT_PHOTO:
		layer = PictureLayer::create();
		break;
	case GMT_STORE:
		layer = StoreLayer::create();
		break;
	default:
		break;
	}

	return layer;
}

//手机返回键处理回调
void GameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event * pEvent)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		//弹出退出游戏提醒
		auto i18n = I18N::getInstance();
		DialogManager::getInstance()->showDialog(i18n->getStringByKey(qtitle), i18n->getStringByKey(quit), [](Ref* psender) {
			Director::getInstance()->end();
		});
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
			//释放上一菜单层
			_preLayer->removeFromParentAndCleanup(true);

			//创建当前点击菜单对应的场景
			_currentLayer = createLayer(static_cast<GameMenuType>(currentTag));
			addChild(_currentLayer);
		}

		//不能连续点击相同的菜单，禁用当前的菜单，解禁上一次的菜单
		_preMenu->setEnabled(true);
		currentMenu->setEnabled(false);

		//保存这次点击的菜单和场景指针引用
		_preMenu = currentMenu;
		_preLayer = _currentLayer;

		//将当前场景指正置空
		_currentLayer = nullptr;

	}
}
