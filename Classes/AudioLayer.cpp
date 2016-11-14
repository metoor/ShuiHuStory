/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	AudioLayer.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/13
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "AudioLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "BlockLayer.h"
#include "ConstantDefine.h"
#include "AudioManager.h"

USING_NS_CC;
using namespace ui;
using namespace cocostudio::timeline;

AudioLayer::AudioLayer()
{
}

AudioLayer::~AudioLayer()
{
}

bool AudioLayer::init()
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

	initBtnIco();

	return true;
}

void AudioLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	_btnClose = node->getChildByName<Button*>(btnCloseName);
	_btnEffect = node->getChildByName<Button*>(btnEffectName);
	_btnMusic = node->getChildByName<Button*>(btnMusicName);

	//关闭按钮回调
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

	//音效按钮点击回调
	_btnEffect->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			effect();
		}
	});

	//音乐按钮点击回调
	_btnMusic->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			music();
		}
	});

}

void AudioLayer::initBtnIco()
{
	auto audio = AudioManager::getInstance();

	if (audio->getIsBgMute())
	{
		//设置成关闭状态图标
		_btnMusic->loadTextures(sound_off_1, sound_off_2, sound_off_2, Widget::TextureResType::PLIST);
	}
	else
	{
		//设置成打开状态图标
		_btnMusic->loadTextures(sound_on_1, sound_on_2, sound_on_2, Widget::TextureResType::PLIST);
	}

	if (audio->getIsEffectMute())
	{
		_btnEffect->loadTextures(sound_off_1, sound_off_2, sound_off_2, Widget::TextureResType::PLIST);
	}
	else
	{
		_btnEffect->loadTextures(sound_on_1, sound_on_2, sound_on_2, Widget::TextureResType::PLIST);
	}
}

void AudioLayer::music()
{
	auto audio = AudioManager::getInstance();

	//点击后，状态取反
	bool isBgMute = !audio->getIsBgMute();

	audio->setBgMute(isBgMute);

	if (isBgMute)
	{
		if (audio->_simpleAudioEngine->isBackgroundMusicPlaying())
		{
			audio->_simpleAudioEngine->stopBackgroundMusic();
		}
		//设置音乐关闭状态图标
		_btnMusic->loadTextures(sound_off_1, sound_off_2, sound_off_2, Widget::TextureResType::PLIST);
	}
	else
	{
		if (!audio->_simpleAudioEngine->isBackgroundMusicPlaying())
		{
			audio->playBackgroundMusic(firstSceneMusicName);
		}

		//设置背景音乐打开状态图标
		_btnMusic->loadTextures(sound_on_1, sound_on_2, sound_on_2, Widget::TextureResType::PLIST);
	}
}

void AudioLayer::effect()
{
	auto audio = AudioManager::getInstance();

	bool isEffectMute = !audio->getIsEffectMute();

	audio->setEffectMute(isEffectMute);

	if (isEffectMute)
	{
		//设置音效关闭状态图标
		_btnEffect->loadTextures(sound_off_1, sound_off_2, sound_off_2, Widget::TextureResType::PLIST);
	}
	else
	{
		//设置音效打开状态图标
		_btnEffect->loadTextures(sound_on_1, sound_on_2, sound_on_2, Widget::TextureResType::PLIST);
	}
}

void AudioLayer::startAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//设置位置在屏幕外面
	this->setPosition(visibleSize.width, 0);
	
	//移动至屏幕中央
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(0, 0)), 0.2f);

	//在移动完成后，显示触摸锁定层
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		//显示触摸锁定层
		dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(160.0f);
	}));

	this->runAction(ani);
}

void AudioLayer::endAnimation()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//先隐藏触摸锁层
	dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(0.0f);

	//移动到屏幕外面
	auto move = EaseIn::create(MoveTo::create(0.5f, Point(visibleSize.width, 0)), 0.2f);

	//在完成移动后释放资源
	auto ani = Sequence::createWithTwoActions(move, CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}

void AudioLayer::onEnterTransitionDidFinish()
{
	startAnimation();
}
