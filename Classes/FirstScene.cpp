#include "FirstScene.h"
#include "cocostudio/CocoStudio.h"
#include "HelloWorldScene.h"

USING_NS_CC;
using namespace ui;

FirstScene::FirstScene()
	:_currentIndex(0)
{
	readChatData();
}

FirstScene::~FirstScene()
{
}

bool FirstScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//背景音乐
	//CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/wellcomeBg.mp3");

	//注册触摸事件
	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = [](Touch* touch, Event* event) {

		return true;
	};

	listen->onTouchMoved = [](Touch* touch, Event* event) {

	};

	listen->onTouchEnded = [&](Touch* touch, Event* event) {
		nextMsg();
	};

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);

	//创建层
	auto root = CSLoader::createNode(csbName_firstScene);
	addChild(root);

	_heroName = dynamic_cast<Text*>(root->getChildByTag(heroNameTextTag_firstScene));
	_chatContent = dynamic_cast<Text*>(root->getChildByTag(chatContentTextTag_firstScene));
	_heroImage = dynamic_cast<ImageView*>(root->getChildByTag(heroImageTag_firstScene));


	//显示初始对白
	nextMsg();

	return true;
}

cocos2d::Scene * FirstScene::createScene()
{
	auto scene = Scene::create();
	auto layer = FirstScene::create();

	scene->addChild(layer);

	return scene;
}

void FirstScene::setContent(std::string heroName, std::string chatContent, std::string heroImage)
{
	_heroName->setString(heroName);
	_chatContent->setString(chatContent);
	_heroImage->loadTexture(heroImage);
}

void FirstScene::readChatData()
{
	std::string jsonpath = FileUtils::getInstance()->fullPathForFilename(storyFileName_firstScene);
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
	_doc.Parse<0>(contentStr.c_str());

	if (_doc.HasParseError()) {
		return;
	}
}

void FirstScene::nextMsg()
{
	if (_currentIndex < _doc["datas"].Size())
	{
		rapidjson::Value & json = _doc["datas"][_currentIndex];
		setContent(json["name"].GetString(), json["msg"].GetString(), json["picture"].GetString());
		++_currentIndex;
	}
	else
	{
		UserDefault::getInstance()->setBoolForKey("isFirst", false);
		//对白完成跳转到游戏主页
		Director::getInstance()->replaceScene(TransitionPageTurn::create(1.0f, HelloWorld::createScene(), false));
	}
}
