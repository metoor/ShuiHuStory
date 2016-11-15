/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	McLog.h
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __MCLOG_H__
#define __MCLOG_H__

#include <string>
#include <cstdio>

class McLog {
public:
	McLog(std::string fileName = "log.mc");
	~McLog();

	//msg:错误描述，fileName:出错的文件名，line:出错的行数
	void addWaring(const std::string& msg, const std::string& fileName, int line);
	void addError(const std::string& msg, const std::string& fileName, int line);

private:
	void addLog(const std::string& msg, bool isWaring, const std::string& fileName, int line);
	void saveLog();

private:
	bool _isError;

	//保存错误信息
	std::string _log;
	std::string _fileName;
};
#endif // MCLOG_H_
