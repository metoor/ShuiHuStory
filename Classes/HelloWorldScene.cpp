#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "Tools.h"
#include "Config.h"
#include "Equipment.h"
#include "GameData.h"
#include "HeroCard.h"
#include "McLog.h"
#include "DialogManager.h"
#include "I18N.h"
#include "AudioManager.h"
#include "BlockLayer.h"
#include "ParticleLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;

HelloWorld::HelloWorld()
{
	GameData::getInstance()->readHeroCard();
	GameData::getInstance()->readEquipment();
}

HelloWorld::~HelloWorld()
{
	GameData::getInstance()->saveUniqueIdentifierNumToFile();
	GameData::getInstance()->saveEquipment();
	GameData::getInstance()->saveHeroCard();
	AudioManager::getInstance()->destoryInstance();
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);

	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//¼ÓÃÜ²âÊÔ
	//string content = FileUtils::getInstance()->getStringFromFile("test.data");

	auto pl = ParticleLayer::create();
	addChild(pl, 900);

    return true;
}

void HelloWorld::menuCloseCallback(Ref * psender)
{
	/*Equipment* e = new Equipment();
	e->init(Tools::getRandomInt(1, 6) * 100 + Tools::getRandomInt(0, 5));
	GameData::getInstance()->addEquipmentToMap(e);

	HeroCard* h = new HeroCard();
	h->init(Tools::getRandomInt(0, 108));
	GameData::getInstance()->addHeroCardToMap(h);*/

	auto i18n = I18N::getInstance();
	i18n->loadStringFile("string.plist");
	auto hello = StringUtils::format(i18n->getStringByKey("two")->c_str(), 1000);


	//DialogManager::getInstance()->showTip(&hello);

}
