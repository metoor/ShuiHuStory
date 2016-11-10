/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	McLog.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "McLog.h"
#include "cocos2d.h"
#include <fstream>
#include "DialogManager.h"
#include "I18N.h"
#include "ConstantDefine.h"

using namespace std;

McLog::McLog(std::string fileName)
	:_isError(false)
{
	_fileName = fileName;
}

McLog::~McLog()
{
	saveLog();
}

void McLog::addWaring(const ::string& msg, const std::string& fileName, int line)
{
	addLog(msg, true, fileName, line);
}

void McLog::addError(const std::string& msg, const std::string& fileName, int line)
{
	//记录是否发生错误
	_isError = true;

	addLog(msg, false, fileName, line);

}

void McLog::addLog(const std::string& msg, bool isWaring, const std::string& fileName, int line)
{
	string flag = "---Error---";
	if (isWaring)
	{
		flag = "---Waring---";
	}

	_log.append(__TIMESTAMP__).append(flag).append("file:").append(fileName)
		.append("---line: ").append(cocos2d::StringUtils::format("%d", line)).append("---des:")
		.append(msg).append("\n");
}

void McLog::saveLog()
{
	if (!_log.empty())
	{
		string path = cocos2d::FileUtils::getInstance()->getWritablePath();
		path.append(_fileName);

		ofstream out(path, ios::app);

		if (out.is_open())
		{
			out << _log;
			out.flush();
			out.close();
		}

	}

	//如果发生错误结束游戏
	if (_isError)
	{
		auto i18n = I18N::getInstance();

		DialogManager::getInstance()->showDialog(i18n->getStringByKey(error), i18n->getStringByKey(errorContent), [](cocos2d::Ref* pSender) {
			cocos2d::Director::getInstance()->end();
		}, false);
	}
}
