/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	PVPStartLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/12/03
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "PVPStartLayer.h"
#include "cocostudio/CocoStudio.h"
#include "BlockLayer.h"
#include "GameData.h"
#include "AudioManager.h"
#include "I18N.h"
#include "TypeStruct.h"
#include "ClientLayer.h"
#include "HeroCard.h"
#include "McLog.h"
#include "PVPBattleScene.h"
#include "Tools.h"

USING_NS_CC;
using namespace ui;

PVPStartLayer::PVPStartLayer()
	:_isStartBtn(true),
	_ball(nullptr)
{
}

PVPStartLayer::~PVPStartLayer()
{
}

bool PVPStartLayer::init()
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

	//注册消息
	addCreteSceneMessageListener();
	addDisconnectMessageListener();
	addConnectMessageListener();

	auto data = GameData::getInstance();
	auto i18n = I18N::getInstance();

	//计算本次比赛需要消耗的金币数，公式：（1 + 等级/10）*base_gold
	_gold = (1 + data->getLevel() / 10) * base_gold;

	if (data->getGold() < _gold)
	{
		//金币不够，禁用按钮，显示提示语
		_btnStart->setEnabled(false);
		disPlayTips(StringUtils::format(i18n->getStringByKey(pvpNoMoney).c_str(), _gold), Color4B::RED);
	}
	else
	{
		//显示提示语
		disPlayTips(StringUtils::format(i18n->getStringByKey(pvpTips).c_str(), _gold, (int)(_gold * 2 * 0.9f)), Color4B::BLUE);
	}

	return true;
}

void PVPStartLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	_btnClose = node->getChildByName<Button*>(btnCloseName);
	_btnStart = node->getChildByName<Button*>(btnStartName);
	_tips = node->getChildByName<Text*>(tipsName);
	_ball = node->getChildByName<Sprite*>(ballName);

	//关闭按钮事件回调
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

	//开始匹配按钮回调
	_btnStart->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (Widget::TouchEventType::BEGAN == type)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (Widget::TouchEventType::ENDED == type)
		{
			auto i18n = I18N::getInstance();

			if (_isStartBtn)
			{
				auto layer = ClientLayer::create();
				layer->setTag(socketLayerTag);
				layer->initSocket(6010);
				addChild(layer);

				//将纹理改成“取消”
				_btnStart->loadTextures(btnCancel1, btnCancel2, btnCancel2, Widget::TextureResType::PLIST);

				//显示一时语“匹配对手中”
				disPlayTips(i18n->getStringByKey(connecting), Color4B::MAGENTA);
				ballRunAction();
			}
			else
			{
				//如果连接层已经添加到当前层则移除
				auto layer = dynamic_cast<ClientLayer*>(getChildByTag(socketLayerTag));
				if (layer != nullptr)
				{
					layer->removeFromParentAndCleanup(true);
				}

				//将纹理改成“开始匹配”
				_btnStart->loadTextures(btnStart1, btnStart2, btnStart2, Widget::TextureResType::PLIST);

				stopBallAction();

				//显示一时语“用户取消”
				disPlayTips(i18n->getStringByKey(userCancel), Color4B::BLUE);
			}

			_isStartBtn = !_isStartBtn;
		}
	});
}

void PVPStartLayer::ballRunAction()
{
	//创建移动动画
	auto move = MoveBy::create(3.0f, Vec2(ball_end_pos_x - ball_start_pos_x, 0));
	auto move_back = move->reverse();

	//创建变速运动
	auto move_ease = EaseBounceInOut::create(move);
	auto move_ease_back = move_ease->reverse();

	//延时
	auto delay = DelayTime::create(0.25f);

	auto seq = Sequence::create(move_ease, delay, move_ease_back, delay->clone(), NULL);
	auto action = RepeatForever::create(seq);
	action->setTag(actionTag);

	_ball->setVisible(true);
	_ball->runAction(action);
}

void PVPStartLayer::stopBallAction()
{
	_ball->stopActionByTag(actionTag);
	_ball->setVisible(false);
	_ball->setPosition(Point(ball_start_pos_x, ball_pos_y));
}

string PVPStartLayer::getBattleHeroData()
{
	std::string json = "{\"role\":[";
	char buffer[255];

	memset(buffer, 0, sizeof(buffer));

	//英雄出战列表
	auto data = GameData::getInstance();

	for (unsigned pos = 0; pos < max_battle_hero_num; ++pos)
	{
		int heroId = data->getBattleHeroId(pos);

		if (heroId != -1)
		{
			auto property = data->getHeroCardById(heroId)->getProperty();

			//将布尔类型的值转换成字符串
			string isMagic = "false";
			if (property->isMagic)
			{
				isMagic = "true";
			}

			std::sprintf(buffer, "{\"t\":%d,\"p\":%d,\"hp\":%d,\"ap\":%d,\"mp\":%d,\"df\":%d,\"mdf\":%d,\"ai\":%d,\"si\":%d,\"sp\":%d,\"cr\":%d,\"br\":%d,\"cd\":%d,\"im\":%s},",
				property->type, pos, property->hp, property->ap, property->mp,
				property->defend, property->magicDefend, property->attackId, property->skillId, property->speed, 
				property->critRate, property->blockRate, Tools::percentToInt(property->critDamage), isMagic.c_str());
		}
		else
		{
			std::sprintf(buffer, "{\"t\":%d,\"p\":%d,\"hp\":%d,\"ap\":%d,\"mp\":%d,\"df\":%d,\"mdf\":%d,\"ai\":%d,\"si\":%d,\"sp\":%d,\"cr\":%d,\"br\":%d,\"cd\":%d,\"im\":%s},",
				none, pos, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "false");
		}

		json += buffer;
	}

	json.pop_back();

	json += "]}";

	return json;
}

void PVPStartLayer::disPlayTips(const std::string & content, cocos2d::Color4B color)
{
	_tips->setString(content);
	_tips->setTextColor(color);
}

void PVPStartLayer::addCreteSceneMessageListener()
{
	//注册启动pvp战斗事件
	auto listen = EventListenerCustom::create(msg_create_pvp_scene, [&](EventCustom* event) {
		_pvpData = *(PVPData*)(event->getUserData());
		_pvpData.gold = _gold;

		auto i18n = I18N::getInstance();
		
		//将纹理改成“开始匹配”
		_btnStart->loadTextures(btnStart1, btnStart2, btnStart2, Widget::TextureResType::PLIST);
		
		stopBallAction();
		
		disPlayTips(StringUtils::format(i18n->getStringByKey(pvpTips).c_str(), _gold, (int)(_gold * 2 * 0.9f)), Color4B::BLUE);
		
		_isStartBtn = !_isStartBtn;

		//启动战斗场景
		auto scene = PVPBattleScene::createScene(&_pvpData);
		Director::getInstance()->pushScene(scene);
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
}

void PVPStartLayer::addDisconnectMessageListener()
{
	//注册连接断开事件
	auto listen = EventListenerCustom::create(msg_socket_disconnect, [&](EventCustom* event) {
		auto i18n = I18N::getInstance();
		
		//将纹理改成“开始匹配”
		_btnStart->loadTextures(btnStart1, btnStart2, btnStart2, Widget::TextureResType::PLIST);

		stopBallAction();

		disPlayTips(i18n->getStringByKey(netError), Color4B::RED);

		_isStartBtn = !_isStartBtn;
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
}

void PVPStartLayer::addConnectMessageListener()
{
	//注册，socket初始话化状态消息，0表示成功，1表示失败
	auto listen = EventListenerCustom::create(msg_socket_connect, [&](EventCustom* event) {
		int result = ((int)event->getUserData());
		
		if (result == 0)
		{
			//连接成功，发送数据给服务器
			auto layer = dynamic_cast<ClientLayer*>(getChildByTag(socketLayerTag));
			if (layer != nullptr)
			{
				auto data = getBattleHeroData();
				layer->sendMessage(data, 0);
			}
			else
			{
				//连接成功，但是没有获取到socket所在Layer对象
				McLog mc;
				mc.addError("socket ready, but can not get socket layer...", __FILE__, __LINE__);
			}
		}
		else
		{
			//连接失败
			auto i18n = I18N::getInstance();

			//将纹理改成“开始匹配”
			_btnStart->loadTextures(btnStart1, btnStart2, btnStart2, Widget::TextureResType::PLIST);

			stopBallAction();

			disPlayTips(i18n->getStringByKey(netError), Color4B::RED);

			_isStartBtn = !_isStartBtn;
		}
	});

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
}

void PVPStartLayer::onEnter()
{
	Layer::onEnter();

	startAnimation();
}

void PVPStartLayer::startAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//设置位置在屏幕外面
	this->setPosition(visibleSize.width, 0);

	//移动至屏幕中央
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(0, 0)), 0.2f);

	//在移动完成后，显示触摸锁定层
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		//显示触摸锁定层
		dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(bl_opacity_on);
	}));

	this->runAction(ani);
}

void PVPStartLayer::endAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//先隐藏触摸锁层
	dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(bl_opacity_off);

	//移动到屏幕外面
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(visibleSize.width, 0)), 0.2f);

	//在完成移动后释放资源
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}
