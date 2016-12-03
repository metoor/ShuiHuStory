/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	AudioManager.h
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/10/20
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include <string>
#include "SimpleAudioEngine.h"

class AudioManager{
public:
	~AudioManager();

	static AudioManager* getInstance();
	
	//判断是否已经生成该单例类
	static bool isNullptr();

	//销毁该单例类
	void destoryInstance();

	//播放第一次登陆时的背景音乐
	unsigned int playFirstSceneBgMusic();
	
	//播放游戏主页面背景音乐
	unsigned int playGamingSceneBgMusic();
	
	//播放游戏主页面背景音乐
	unsigned int playBattleSceneBgMusic();

	//停止播放背景音乐，并清理资源
	void stopBackgroundMusic();

	//播放菜单点击音效
	unsigned int playClickEffect();

	//播放战斗音效，index属于【0， 8】，【0， 3】普通技能攻击，【4， 8】特技攻击音效
	unsigned int playAttackEffect(int index);

	//获得金币音效
	unsigned int playGetGoldEffect();

	//播放游戏胜利或失败音效
	unsigned int playGameOverEffect(bool isWin);
	
	//加载和卸载战斗场景的音效
	void preLoadBattleSceneAudio();
	void unLoadBattleSceneAudio();

	//预加载全局音效和背景音乐
	void preLoadGlobalAudio();

	//卸载全局音效和背景音乐
	void unLoadGlobalEffect();

	//获得是否静音标志
	bool getIsEffectMute();
	bool getIsBgMute();
	
	//设置是否静音
	void setEffectMute(bool isEffectMute);
	void setBgMute(bool isBgMute);

public:

	//音乐播放引擎实例
	CocosDenshion::SimpleAudioEngine* _simpleAudioEngine;

private:
	AudioManager();

	//通过名字播放音效
	unsigned int playEffect(std::string name);

	//通过名字播放背景音乐
	unsigned int playBackgroundMusic(std::string name, bool isLoop = true);

private:
	//战斗时使用背景音乐
	int _randomIndex;

	//是否启用音效
	bool _isEffectMute;
	//是否启用背景音乐
	bool _isbgMusicMute;

	static AudioManager* _instance;
};

#endif // AUDIOMANAGER_H_
