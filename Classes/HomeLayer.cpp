#include "HomeLayer.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;
using namespace std;

HomeLayer::HomeLayer()
	:_preTag(start)
{
}

HomeLayer::~HomeLayer()
{
}

bool HomeLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	loadUI();

	//初始化数组为nullptr
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		_layerPointer[index] = nullptr;
	}

	return true;
}

void HomeLayer::loadUI()
{
	//加载编辑好的主页场景文件
	auto homeLayer = CSLoader::createNode(csbName);
	addChild(homeLayer);

	//绑定菜单的回调方法
	for (int index = 0; index < ARRAY_SIZE; ++index)
	{
		auto menu = homeLayer->getChildByTag<Button*>(index);
		menu->addTouchEventListener(CC_CALLBACK_2(HomeLayer::menuCallBack, this));
	}
}

void HomeLayer::menuCallBack(Ref* pSender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		int currentTag = dynamic_cast<Button*>(pSender)->getTag();
		
		if (start == _preTag)
		{
			//第一启动，只需要创建对应的层
			log("----%d", currentTag);
		}
		else if (_preTag != currentTag)
		{
			//不是第一次启动，要销毁上次点击的对应Tag的层
			log("--current:%d--pre:%d", currentTag, _preTag);
		}

		
		_preTag = currentTag;
	}
}
