#include "DialogManager.h"
#include "McLog.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

DialogManager* DialogManager::_instance = nullptr;

USING_NS_CC;


/************************************************************************/
/*         触摸锁定层                                                     */
/************************************************************************/
class BlockLayer : public Layer
{
public:
	CREATE_FUNC(BlockLayer);

	bool init() override
	{
		if (!Layer::init())
			return false;

		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(true);
		listener->onTouchBegan = CC_CALLBACK_2(BlockLayer::onTouchBegan, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
		return true;
	}

	virtual bool onTouchBegan(Touch* touch, Event* event) override
	{
		return true;
	}
};



/************************************************************************/
/*    对话框管理类                                                        */
/************************************************************************/
DialogManager::DialogManager()
{
}

cocos2d::Node * DialogManager::createTip(std::string text, cocos2d::Color4B color, int size)
{
	//获取cocos stuido编辑好的ui文件
	auto tipLayer = (CSLoader::createNode(csbName_TipLayer));

	//获取显示文本内容的Text控件
	auto label = tipLayer->getChildByName<ui::Text*>("text");

	Size visibleSize = Director::getInstance()->getVisibleSize();

	label->setString(text);
	label->setTextColor(color);
	label->setFontSize(size);
	tipLayer->setAnchorPoint(Point::ANCHOR_MIDDLE);
	tipLayer->setPosition(visibleSize / 2);
	
	return tipLayer;
}

DialogManager::~DialogManager()
{
}

DialogManager * DialogManager::getInstance()
{
	if (_instance == nullptr)
	{
		_instance = new DialogManager();

		if (!_instance)
		{
			//错误日志记录
			McLog mLog;
			mLog.addError("_instance create failed...", __FILE__, __LINE__);
		}
	}

	return _instance;
}

void DialogManager::showTipDialog(std::string text, float second, cocos2d::Color4B color, int size)
{
	auto blockLayer = BlockLayer::create();
	
	//锁定屏幕触摸
	Director::getInstance()->getRunningScene()->addChild(blockLayer, 9999);

	blockLayer->addChild(createTip(text, color, size));
}

void DialogManager::destoryInstance()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}
