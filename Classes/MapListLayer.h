/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	MapListLayer.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/27
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef MAPLISTLAYER_H_  
#define MAPLISTLAYER_H_  
  
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>
#include <functional>

//前置声明
struct MapData;

class MapListLayer : public cocos2d::Layer
{
public:
	MapListLayer();
	~MapListLayer();
	virtual bool init() override;
	CREATE_FUNC(MapListLayer);

	void initWithMapIndex(int index);

private:
	void loadUI();

	void loadItem();

	//加载指定索引的地图信息
	void loadMap(int index);

	//设置item的数目，如：5/12
	void setTips();

	void startAnimation();
	void endAnimation();

	//listview item点击回调
	void onItemSelected(cocos2d::Ref* pSender, cocos2d::ui::ListView::EventType type);

	void onEnter() override;

private:
	const std::string csbName = "layers/warLayer/mapListLayer.csb";
	const std::string listViewName = "listView";
	const std::string btnCloseName = "btnClose";
	const std::string tipsName = "tips";

private:
	cocos2d::ui::ListView* _listView;
	cocos2d::ui::Button* _btnClose;
	cocos2d::ui::Text* _tips;

	std::vector<MapData*> _mapDataVector;
};
  

/************************************************************************/
/* listview 的item                                                      */
/************************************************************************/
class MapListItem : public cocos2d::ui::Layout
{
public:
	MapListItem();
	~MapListItem();
	virtual bool init() override;
	CREATE_FUNC(MapListItem);

	//设置item的内容
	void setString(const std::string& title, const int exp, const int gold, const std::string& reward);

private:
	void loadUI();

private:
	const std::string csbName = "layers/warLayer/mapListItemLayer.csb";
	const std::string titleName = "title";
	const std::string expName = "exp";
	const std::string goldName = "gold";
	const std::string rewardName = "reward";

private:
	cocos2d::ui::Text *_title, *_exp, *_gold, *_reward;
};
#endif // MAPLISTLAYER_H_ 
