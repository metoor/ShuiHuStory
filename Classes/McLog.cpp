#include "McLog.h"
#include "cocos2d.h"
#include <fstream>

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

void McLog::addWaring(std::string msg, std::string fileName, int line)
{
	addLog(msg, true, fileName, line);
}

void McLog::addError(std::string msg, std::string fileName, int line)
{
	//记录是否发生错误
	_isError = true;

	addLog(msg, false, fileName, line);

}

void McLog::addLog(std::string msg, bool isWaring, std::string fileName, int line)
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
		cocos2d::Director::getInstance()->end();
	}
}
