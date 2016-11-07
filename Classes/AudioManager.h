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
	void destoryInstance();

	//播放第一次登陆时的背景音乐
	unsigned int playFirstSceneBgMusic();

	//播放菜单点击音效
	unsigned int playClickEffect();

	//预加载全局音效和背景音乐
	void preLoadGlobalAudio();

	//卸载全局音效和背景音乐
	void unLoadGlobalEffect();

	//获得是否静音标志
	bool getIsMute();
	
	//设置是否静音
	void setIsMute(bool isMute);

public:
	//音乐/音效的名字
	const std::string bgFirstSceneBgMusicName = "audios/wellcomeBg.mp3";
	const std::string clickEffectName = "audios/click.mp3";

	
	//音乐播放引擎实例
	CocosDenshion::SimpleAudioEngine* _simpleAudioEngine;

private:
	AudioManager();

	//通过名字播放音效
	unsigned int playEffect(std::string name);

	//通过名字播放背景音乐
	unsigned int playBackgroundMusic(std::string name, bool isLoop = true);

private:
	bool _isMute;
	static AudioManager* _instance;
};

#endif // AUDIOMANAGER_H_
