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
#include "Tools.h"

AudioManager* AudioManager::_instance = nullptr;

USING_NS_CC;
using namespace std;

AudioManager::AudioManager()
	:_randomIndex(0)
{
}

AudioManager::~AudioManager()
{
	//保存是否静音的标志
	UserDefault::getInstance()->setBoolForKey(is_bg_mute_name.c_str(), _isbgMusicMute);
	UserDefault::getInstance()->setBoolForKey(is_effect_mute_name.c_str(), _isEffectMute);

	//不能在保存数据前释放，会造成数据我法保存，只有音效设置会保存，音乐设置不会保存
	_simpleAudioEngine->end();
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
	CC_SAFE_DELETE(_instance);
}

unsigned int AudioManager::playFirstSceneBgMusic()
{
	return playBackgroundMusic(firstSceneMusicName);
}

unsigned int AudioManager::playGamingSceneBgMusic()
{
	int randomIndex = Tools::getRandomInt(0, 1);
	return playBackgroundMusic(StringUtils::format(menuMusicName.c_str(), randomIndex));
}

unsigned int AudioManager::playBattleSceneBgMusic()
{
	return playBackgroundMusic(StringUtils::format(warBgMusicName.c_str(), _randomIndex));;
}

void AudioManager::stopBackgroundMusic()
{
	if (_simpleAudioEngine->isBackgroundMusicPlaying())
	{
		_simpleAudioEngine->stopBackgroundMusic(true);
	}
}

unsigned int AudioManager::playClickEffect()
{
	return playEffect(clickEffectName);
}

unsigned int AudioManager::playAttackEffect(int index)
{
	if (!Tools::betweenAnd(index, 0, 8))
	{
		CCAssert(false, "'index' out of rang...");
		McLog mc;
		mc.addError("'index' out of rang...", __FILE__, __LINE__);
	}
	
	return playEffect(StringUtils::format(attackEffectName.c_str(), index));
}

unsigned int AudioManager::playGetGoldEffect()
{
	return playEffect(rewardGoldEffectName);
}

unsigned int AudioManager::playGameOverEffect(bool isWin)
{
	if (isWin)
	{
		return playEffect(gameWineffectName);
	}
	else
	{
		return playEffect(gameFaileffectName);
	}
}

void AudioManager::preLoadBattleSceneAudio()
{
	for (int index= 0; index < 9; ++index)
	{
		//预加加载攻击音效
		_simpleAudioEngine->preloadEffect(StringUtils::format(attackEffectName.c_str(), index).c_str());
	}

	_simpleAudioEngine->preloadEffect(gameFaileffectName.c_str());
	_simpleAudioEngine->preloadEffect(gameWineffectName.c_str());

	_randomIndex = Tools::getRandomInt(0, 3);
	_simpleAudioEngine->preloadBackgroundMusic(StringUtils::format(warBgMusicName.c_str(), _randomIndex).c_str());
}

void AudioManager::unLoadBattleSceneAudio()
{
	for (int index = 0; index < 9; ++index)
	{
		//预加加载攻击音效
		_simpleAudioEngine->unloadEffect(StringUtils::format(attackEffectName.c_str(), index).c_str());
	}

	_simpleAudioEngine->unloadEffect(gameFaileffectName.c_str());
	_simpleAudioEngine->unloadEffect(gameWineffectName.c_str());
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
	_simpleAudioEngine->preloadEffect(rewardGoldEffectName.c_str());
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
