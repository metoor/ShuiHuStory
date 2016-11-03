#ifndef __AUDIOMANAGER_H__
#define __AUDIOMANAGER_H__

#include <string>
#include "SimpleAudioEngine.h"

class AudioManager{
public:
	~AudioManager();

	static AudioManager* getInstance();
	void destoryInstance();

	//通过名字播放音效
	void playEffect(std::string name);

	//通过名字播放背景音乐
	void playBackgroundMusic(std::string name, bool isLoop = true);

	//获得是否静音标志
	bool getIsMute();
	
	//设置是否静音
	void setIsMute(bool isMute);

public:
	//音乐/音效的名字
	const std::string bgFirstScene = "audios/wellcomeBg.mp3";
	const std::string clickEffect = "audios/click.mp3";

	//预加载全局音效和背景音乐
	void preLoadGlobalAudio();

	//卸载全局音效和背景音乐
	void unLoadGlobalEffect();

	//音乐播放引擎实例
	CocosDenshion::SimpleAudioEngine* _simpleAudioEngine;

private:
	AudioManager();

private:
	bool _isMute;
	static AudioManager* _instance;
};

#endif // AUDIOMANAGER_H_
