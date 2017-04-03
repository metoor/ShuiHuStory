/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	Tools.cpp
// Author:		 Metoor
// Version: 	1.0
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#include "Tools.h"
#include <random>
#include <assert.h>
#include "cocos2d.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

int Tools::getRandomInt(int min, int max)
{
	int result = 0;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo info;
	bool ret = JniHelper::getStaticMethodInfo(info, "org/cocos2dx/cpp/AppActivity", "randomInt", "(II)I");
	if (ret)
	{
		result = info.env->CallStaticIntMethod(info.classID, info.methodID,min,max);
	}
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	/************************************************************************/
	/*  因为现在电脑的运算能力很强，在循环里面连续获取时间时，会获取到相同的时间。因为随  */
	/*	机数种子（时间）相同，那么产生的随机数也会相同。为了避免在循环里获取时间后人为加	*/
	/*	上获取的总次数，这样既可避免产生相同的时间种子。	*/
	/************************************************************************/
	static unsigned randomCount = 0;

	time_t currentTimer = getCurrentTimeSecond();

	//人为加上获取的次数，避免产生相同的时间
	currentTimer += randomCount;
	++randomCount;

	std::default_random_engine e((unsigned int)currentTimer);

	std::uniform_int_distribution<unsigned> u(min, max);
	result = u(e);
#endif

	return result;
}

tm Tools::getCurrentTimeTm()
{
	time_t timer = getCurrentTimeSecond();
	tm *tmTemp = localtime(&timer);

	return *tmTemp;
}

float Tools::intToPercent(int x)
{
	return x / 100.0f;
}

int Tools::percentToInt(float x)
{
	return static_cast<int>(x * 100);
}

bool Tools::intToBool(int i)
{
	return (i != 0);
}

float Tools::keepTwoEffectNum(float f)
{
	int temp = static_cast<int>(f * 1000);
	int last = temp % 10;

	if (last > 4)
	{
		f = (temp + 10 - last) / 1000.0f;
	}
	else
	{
		f = (temp - last) / 1000.0f;
	}
	return f;
}

bool Tools::betweenAnd(int p, int min, int max)
{
	bool result = true;

	if (p < min || p > max)
	{
		result = false;
	}

	return result;
}

void Tools::arrayAssignment(int length, int des[], int src[])
{
	for (int index = 0; index < length; ++index)
	{
		des[index] = src[index];
	}
}

std::string Tools::getStringFromFile(const std::string & fileName)
{
	std::ifstream in;
	in.open(fileName, std::ios::binary | std::ios::in | std::ios::ate);

	if (in.is_open())
	{
		auto size = in.tellg();
		std::string str((unsigned int)size, '\0'); // construct string to stream size
		in.seekg(0);
		in.read(&str[0], size);

		in.close();

		return str;
	}
	else
	{
		assert(false);
		in.close();
		return "";
	}
}

bool Tools::writeStringToFile(const std::string & fileName, const std::string & content, std::ios_base::openmode mode)
{
	bool result = true;

	std::ofstream out;
	out.open(fileName, mode);

	if (out.is_open())
	{
		out << content;
	}
	else
	{
		result = false;
		assert(false);
	}

	out.close();

	return result;
}

std::string Tools::getCurrentTimeString()
{
	char buffer[25];
	memset(buffer, 0, sizeof(buffer));

	tm currentTime = getCurrentTimeTm();

	int year = currentTime.tm_year + 1900;
	int month = currentTime.tm_mon + 1;

	std::sprintf(buffer, "%dy_%dm_%dh_%dm_%ds", year, month, currentTime.tm_hour, currentTime.tm_min, currentTime.tm_sec);

	return std::string(buffer);
}

int Tools::randomBetweenOneAndFive(int starNum, int onePercent, int twoPercent, int threePercent, int fourPercent, int fivePercent)
{
	int result = 0;

	starNum = maxInt(minInt(starNum, 5), 1);

	if (onePercent + twoPercent + threePercent + fourPercent + fivePercent == 100)
	{
		int rd = getRandomInt(1, 100);

		if (rd < onePercent)
		{
			result = starNum;
		}
		else if (rd < twoPercent + onePercent)
		{
			result = starNum + 1;
		}
		else if (rd < threePercent + twoPercent + onePercent)
		{
			result = starNum + 2;
		}
		else if (rd < fourPercent + threePercent + twoPercent + onePercent)
		{
			result = starNum + 3;
		}
		else
		{
			result = starNum + 4;
		}
	}

	return result;
}

time_t Tools::getCurrentTimeSecond()
{
	return (time_t)time(NULL);
}

int Tools::maxInt(int a, int b)
{
	return (a > b ? a : b);
}

int Tools::minInt(int a, int b)
{
	return (a > b ? b : a);
}

float Tools::maxFloat(float a, float b)
{
	return (a > b ? a : b);
}

float Tools::minFloat(float a, float b)
{
	return (a > b ? b : a);
}
