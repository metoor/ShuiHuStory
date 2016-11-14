/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	LevelupRewardLayer.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/11
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "LevelupRewardLayer.h"
#include "cocostudio/CocoStudio.h"
#include "AudioManager.h"
#include "BlockLayer.h"
#include "GameData.h"
#include "LevelupRewardItem.h"
#include "DialogManager.h"
#include "I18N.h"

USING_NS_CC;
using namespace ui;
using namespace std;

LevelupRewardLayer::LevelupRewardLayer()
{
	readData();
}

LevelupRewardLayer::~LevelupRewardLayer()
{
	saveData();
}

bool LevelupRewardLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//触摸锁定层
	auto blockLayer = BlockLayer::create();
	blockLayer->setName(blockTagName);
	addChild(blockLayer);

	loadUI();


	return true;
}

void LevelupRewardLayer::loadUI()
{
	auto node = CSLoader::createNode(csbName);
	addChild(node);

	_btnClose = node->getChildByName<Button*>(btnCloseName);
	_listView = node->getChildByName<ListView*>(listViewName);

	_btnClose->addTouchEventListener([&](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::BEGAN)
		{
			AudioManager::getInstance()->playClickEffect();
		}

		if (type == Widget::TouchEventType::ENDED)
		{
			endAnimation();
		}
	});
}

void LevelupRewardLayer::loadItem()
{
	for (int pos = 0; pos < itemCount; ++pos)
	{

		if (_isGetGift[pos])
		{
			//如果已经领取过了，则不再创建
			continue;
		}

		auto item = LevelupRewardItem::create();

		//计算奖励信息
		int index = pos + 1;
		int level = item_level_base + index * item_level_increaseNum;
		int gold = gift_base_diamond + index * item_gold_increaseNum;
		int diamond = gift_base_diamond + index * item_diamond_increaseNum;

		//如果当前等级小于领取要求，则禁用领取按钮
		if (GameData::getInstance()->getLevel() < level)
		{
			item->setBtnEnable(false);
		}

		//设置item显示属性
		item->setRewardString(level, diamond, gold);
		item->setBtnCallback([&, pos, level, gold, diamond](Ref* pSender) {
			int tag = dynamic_cast<Button*>(pSender)->getTag();

			//记录用户领取情况
			_isGetGift[pos] = true;

			//获得奖励
			auto data = GameData::getInstance();
			data->setGold(gold);
			data->setDiamond(diamond);
			dynamic_cast<LevelupRewardItem*>(_listView->getItem(pos))->setBtnEnable(false);
			
			//通知更新用户数据
			_eventDispatcher->dispatchCustomEvent(msg_update_user_data);

			//显示提示
			DialogManager::getInstance()->showTips(StringUtils::format(I18N::getInstance()->getStringByKey(loginRewardTips).c_str(), gold, diamond), Color4B::GREEN);
		});


		_listView->pushBackCustomItem(item);
	}
}

void LevelupRewardLayer::readData()
{
	for (int pos = 0; pos < itemCount; ++pos)
	{
		_isGetGift[pos] = UserDefault::getInstance()->getBoolForKey(StringUtils::format(is_get_gift_key.c_str(), pos).c_str(), false);
	}
}

void LevelupRewardLayer::saveData()
{
	for (int pos = 0; pos < itemCount; ++pos)
	{
		UserDefault::getInstance()->setBoolForKey(StringUtils::format(is_get_gift_key.c_str(), pos).c_str(), _isGetGift[pos]);
	}
}

void LevelupRewardLayer::startAnimation()
{
	//y轴从0增加到1
	setScaleY(0.0f);

	auto ani = Sequence::createWithTwoActions(ScaleTo::create(0.4f, 1.0f), CallFunc::create([&]() {
		//显示触摸锁定层
		dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(160.0f);
	}));

	this->runAction(ani);
}

void LevelupRewardLayer::endAnimation()
{
	//先隐藏触摸锁层
	dynamic_cast<BlockLayer*>(this->getChildByName(blockTagName))->setOpacity(0.0f);

	//y轴从1缩小到0
	auto ani = Sequence::createWithTwoActions(ScaleTo::create(0.4f, 1.0f, 0.0f), CallFunc::create([&]() {
		this->removeFromParentAndCleanup(true);
	}));

	this->runAction(ani);
}

void LevelupRewardLayer::onEnterTransitionDidFinish()
{
	loadItem();
	startAnimation();
}
