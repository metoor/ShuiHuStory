/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	I18N.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "I18N.h"
#include "McLog.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

I18N* I18N::_instance = nullptr;


I18N::I18N()
	:_map(nullptr)
{
}

I18N::~I18N()
{
	CC_SAFE_DELETE(_map);
}

I18N * I18N::getInstance()
{
	if (!_instance)
	{
		_instance = new (std::nothrow)I18N();

		if (!_instance)
		{
			//错误日志记录
			McLog mLog;
			mLog.addError("_instance create failed...", __FILE__, __LINE__);
		}
	}

	return _instance;
}

bool I18N::isNullptr()
{
	bool result = false;

	if (_instance == nullptr)
	{
		result = true;
	}

	return result;
}

void I18N::loadData()
{
	loadStringFile("datas/string.plist");
}

void I18N::loadStringFile(string fileName)
{
	if (!_map)
	{
		_map = new (nothrow)unordered_map<string, string>();
	}
	else
	{
		_map->clear();
	}

	auto fileUtils = FileUtils::getInstance();
	string fullPath = fileUtils->fullPathForFilename(fileName);

	//从plist文件中读取ValueMap
	auto valueMap = fileUtils->getValueMapFromFile(fullPath);

	//将value转换成string
	for (auto iter = valueMap.begin(); iter != valueMap.end(); ++iter)
	{
		_map->insert({ iter->first, iter->second.asString() });
	}
}

const string & I18N::getStringByKey(const std::string& key)
{

	auto iter = _map->find(key);

	if (iter != _map->end())
	{
		return iter->second;
	}
	else
	{
		//没有匹配到相同的键值
		{
			//错误日志记录
			McLog mLog;
			mLog.addWaring("'key' can not find...", __FILE__, __LINE__);
		}
		return nullptr;
	}
}

void I18N::dstoryInstance()
{
	CC_SAFE_DELETE(_instance);
}
