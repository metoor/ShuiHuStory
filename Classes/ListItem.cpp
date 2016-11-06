#include "ListItem.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"

USING_NS_CC;
using namespace ui;
using namespace std;

ListItem::ListItem()
{
}

ListItem::~ListItem()
{
}

bool ListItem::init()
{
	if (!Layout::init())
	{
		return false;
	}

	loadUI();

	return true;
}

void ListItem::setBtnCallBack(function<void(Ref*pSender, Widget::TouchEventType type)> func)
{
	_btn->addTouchEventListener([func](Ref*pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			//播放点击音效
			auto audio = AudioManager::getInstance();
			audio->playEffect(audio->clickEffect);
		}

		//执行用户自定义事件
		func(pSender, type);
	});
}

void ListItem::setBtnTag(const int tag)
{
	_btn->setTag(tag);
}

void ListItem::setBtnTexture(const std::string nor, const std::string pressed)
{
	_btn->loadTextureNormal(nor, Widget::TextureResType::PLIST);
	_btn->loadTexturePressed(pressed, Widget::TextureResType::PLIST);
	_btn->loadTextureDisabled(pressed, Widget::TextureResType::PLIST);
}

void ListItem::setLabelText(ItemLabelType type, const std::string content)
{
	switch (type)
	{
	case LEVEL:
		_level->setString(content);
		break;
	case NAME:
		_name->setString(content);
		break;
	case TEXT1:
		_text1->setString(content);
		break;
	case TEXT2:
		_text2->setString(content);
		break;
	default:
		break;
	}
}

void ListItem::setLabelColor(ItemLabelType type, cocos2d::Color4B color)
{
	switch (type)
	{
	case LEVEL:
		_level->setTextColor(color);
		break;
	case NAME:
		_name->setTextColor(color);
		break;
	case TEXT1:
		_text1->setTextColor(color);
		break;
	case TEXT2:
		_text2->setTextColor(color);
		break;
	default:
		break;
	}
}

void ListItem::setIco(const std::string icoName, const std::string img1Name, const std::string img2Name)
{
	_ico->loadTexture(icoName, Widget::TextureResType::PLIST);
	_img1->loadTexture(img1Name, Widget::TextureResType::PLIST);
	_img2->loadTexture(img2Name, Widget::TextureResType::PLIST);
}

void ListItem::setStarNum(const int n)
{
	for (int i = 0; i < n - 1; ++i)
	{
		auto addStar = _star->clone();
		addStar->setPosition(Vec2(_star->getPositionX() + _star->getContentSize().width * (i + 1) * 0.75f, _star->getPositionY()));
		addChild(addStar);
	}
}

void ListItem::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);
	
	//设置大小
	setContentSize(node->getContentSize());

	//寻找控件
	_ico = node->getChildByName<ImageView*>(icoName);
	_level = node->getChildByName<Text*>(levelLabelName);
	_name = node->getChildByName<Text*>(nameLabelName);
	_star = node->getChildByName<ImageView*>(starName);
	_img1 = node->getChildByName<ImageView*>(img1Name);
	_img2 = node->getChildByName<ImageView*>(img2Name);
	_text1 = node->getChildByName<Text*>(text1Name);
	_text2 = node->getChildByName<Text*>(text2Name);
	_btn = node->getChildByName<Button*>(btnName);

	
}
