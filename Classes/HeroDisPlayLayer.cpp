#include "HeroDisplayLayer.h"
#include "cocostudio/CocoStudio.h"
#include "BlockLayer.h"
#include "AudioManager.h"
#include "ListItem.h"
#include "GameData.h"
#include "Tools.h"
#include "HeroCard.h"

USING_NS_CC;
using namespace ui;
using namespace std;


HeroDisplayLayer::HeroDisplayLayer()
{
}

HeroDisplayLayer::~HeroDisplayLayer()
{
}

bool HeroDisplayLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//创建触摸屏蔽层
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockName);
	addChild(blockLayer);

	loadUI();
	
	//加载item
	auto heroCardMap = GameData::getInstance()->getHeroCardMap();
	loadItem(heroCardMap->size());

	return true;
}

void HeroDisplayLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	//查找控件
	_listView = node->getChildByName<ListView*>(listViewName);
	_btnClose = node->getChildByName<Button*>(btnCloseName);


	//绑定事件回调
	_btnClose->addTouchEventListener(CC_CALLBACK_2(HeroDisplayLayer::btnCloseCallBack, this));

}

void HeroDisplayLayer::loadItem(int n)
{
	auto heroCardMap = GameData::getInstance()->getHeroCardMap();

	//开始加载的索引
	int startIndex = _itemVector.size();
	//结束的索引，实际加载到endIndex - 1
	int endIndex = startIndex + n;
	//当前的索引
	int index = 0;

	for (auto iter = heroCardMap->begin(); iter != heroCardMap->end(); ++iter)
	{
		if (Tools::betweenAnd(index, startIndex, endIndex - 1))
		{
			auto item = ListItem::create();

			//设置item显示的属性
			auto property = iter->second->getProperty();
			setItemAttribute(property, item);
			item->setBtnTexture(btnDetails1, btnDetails2);
			item->setBtnTag(index);
			item->setBtnCallBack([&](Ref* pSender, Widget::TouchEventType type) {
				if (type == Widget::TouchEventType::ENDED)
				{
					int tag = dynamic_cast<Button*>(pSender)->getTag();

					log("----%d", tag);
				}
			});


			//添加到listView和itemVector
			_listView->pushBackCustomItem(item);
			_itemVector.pushBack(item);
		}
		else if (static_cast<unsigned>(startIndex) >= heroCardMap->size() || index >= endIndex)
		{
			//如果当前的索引已经超过了要加载的最大索引，或者所有的item已经全部加载则结束循环
			break;
		}

		++index;
	}
}

void HeroDisplayLayer::setItemAttribute(const HeroCardProperty * property, ListItem * item)
{
	//设置文本标签
	item->setLabelText(LEVEL, StringUtils::format("Lv:%d", property->level));
	item->setLabelText(NAME, *(property->name));
	item->setStarNum(property->star);
	item->setLabelText(TEXT1, StringUtils::format("%d", property->hp));
	item->setLabelText(TEXT2, StringUtils::format("%d", property->ap));

	//设置图标
	item->setIco(StringUtils::format("head%d.png", property->type), hpIco, apIco);
	

	setItemColor(item, property->star);
}

void HeroDisplayLayer::updateItemAttribute(const int heroId, const int itenId)
{
	auto hero = GameData::getInstance()->getHeroCardById(heroId);

	//更新单个item的数据
	setItemAttribute(hero->getProperty(), _itemVector.at(itenId));
}

void HeroDisplayLayer::setItemColor(ListItem * item, int star)
{
	Color4B color;

	//设置颜色
	switch (star)
	{
	case 1:
		color = Color4B::WHITE;
		break;
	case 2:
		color = Color4B::BLUE;
		break;
	case 3:
		color = Color4B::GREEN;
		break;
	case 4:
		color = Color4B(255, 0, 255, 255);
		break;
	case 5:
		color = Color4B(255, 102, 0, 255);
		break;
	default:
		break;
	}

	for (int i = 0; i < 4; ++i)
	{
		item->setLabelColor(static_cast<ItemLabelType>(i), color);
	}
}

void HeroDisplayLayer::btnCloseCallBack(cocos2d::Ref * pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		//点击音效
		auto audio = AudioManager::getInstance();
		audio->playEffect(audio->clickEffect);

		endAnimation();
	}
}

void HeroDisplayLayer::onEnterTransitionDidFinish()
{
	startAnimation();
}

void HeroDisplayLayer::startAnimation()
{
	//初始为0
	this->setScale(0.0f);

	auto ani = Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 1.0f), RotateTo::create(0.5f, 720)), CallFunc::create([&]() {
		//显示触摸锁定层
		this->getChildByName(blockName)->setOpacity(150);
	}));

	this->runAction(ani);
}

void HeroDisplayLayer::endAnimation()
{
	//隐藏触摸屏蔽层
	this->getChildByName(blockName)->setOpacity(0.0f);

	auto ani = Sequence::createWithTwoActions(Spawn::createWithTwoActions(ScaleTo::create(0.5f, 0.0f), RotateTo::create(0.5f, 720)), CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}
