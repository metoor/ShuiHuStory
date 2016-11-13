/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	AudioManager.cpp
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "AudioManager.h"
#include "cocos2d.h"
#include "McLog.h"
#include "ConstantDefine.h"

AudioManager* AudioManager::_instance = nullptr;

USING_NS_CC;
using namespace std;

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
	//保存是否静音的标志
	UserDefault::getInstance()->setBoolForKey(is_effect_mute_name.c_str(), _isEffectMute);
	UserDefault::getInstance()->setBoolForKey(is_bg_mute_name.c_str(), _isbgMusicMute);
}

AudioManager * AudioManager::getInstance()
{
	if (!_instance)
	{
		_instance = new (nothrow)AudioManager();

		if (!_instance)
		{
			McLog log;
			log.addError("_instance create failed...", __FILE__, __LINE__);
		}
		else
		{
			//获取是否静音标志
			bool effectMute = UserDefault::getInstance()->getBoolForKey(is_effect_mute_name.c_str(), false);
			bool bgMute = UserDefault::getInstance()->getBoolForKey(is_bg_mute_name.c_str(), false);
			
			_instance->setEffectMute(effectMute);
			_instance->setBgMute(bgMute);

			//保存SimpleAudioEngine实例的引用
			_instance->_simpleAudioEngine = CocosDenshion::SimpleAudioEngine::getInstance();
		}
	}

	return _instance;
}

bool AudioManager::isNullptr()
{
	bool result = false;

	if (_instance == nullptr)
	{
		result = true;
	}

	return result;
}

void AudioManager::destoryInstance()
{
	if (_instance)
	{
		CC_SAFE_DELETE(_instance);
	}

	_simpleAudioEngine->end();
}

unsigned int AudioManager::playFirstSceneBgMusic()
{
	return playBackgroundMusic(bgFirstSceneBgMusicName);
}

unsigned int AudioManager::playClickEffect()
{
	return playEffect(clickEffectName);
}

unsigned int AudioManager::playEffect(std::string name)
{
	unsigned int result = 0;

	if (!_isEffectMute)
	{
		result = _simpleAudioEngine->playEffect(name.c_str());
	}

	return result;
}

unsigned int AudioManager::playBackgroundMusic(std::string name, bool isLoop)
{
	unsigned int result = 0;

	if (!_isbgMusicMute)
	{
		_simpleAudioEngine->playBackgroundMusic(name.c_str(), isLoop);
	}

	return result;
}

void AudioManager::setEffectMute(bool isEffectMute)
{
	_isEffectMute = isEffectMute;
}

void AudioManager::setBgMute(bool isBgMute)
{
	_isbgMusicMute = isBgMute;
}

void AudioManager::preLoadGlobalAudio()
{
	//预加载音效
	_simpleAudioEngine->preloadEffect(clickEffectName.c_str());
}

void AudioManager::unLoadGlobalEffect()
{
	//卸载音效
	_simpleAudioEngine->unloadEffect(clickEffectName.c_str());
}

bool AudioManager::getIsEffectMute()
{
	return _isEffectMute;
}

bool AudioManager::getIsBgMute()
{
	return _isbgMusicMute;
}
