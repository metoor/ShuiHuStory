/*************************************************
// Copyright (C), 2016-2020, CS&S. Co., Ltd.
// File name: 	Tools.h
// Author:		 Metoor
// Version: 	1.0 
// Date: 		2016/11/07
// Contact: 	caiufen@qq.com
// Description: 	create by vs2015pro
*************************************************/

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <random>
#include <ctime>

class Tools
{
public:
	//获得范围[min, max]内的整型随机数 --注意：该函数在循环里面调用不能超过10次
	static int getRandomInt(int min, int max);

	//获取当前系统时间-秒级别
	static time_t getCurrentTimeSecond();

	//获取当前系统时间-年月日等---注意：年（tm_year = 当前年份 - 1900），月（tm_mon[0~11]）
	static tm getCurrentTimeTm();

	//产生一个随机数范围[1~5]其中各个数的概率为参,返回0则表示给出的概率出错(相加不等于100)。如：（2(产生[2~5]), 10（1的概率10%）， 20， 30， 30， 10），
	static int randomBetweenOneAndFive(int starNum, int onePercent, int twoPercent, int threePercent = 0, int fourPercent = 0, int fivePercent = 0);

	//把整数转换成百分比
	static float intToPercent(int x);

	//把百分比转换成整数
	static int percentToInt(float x);

	//bool类型转换成int
	static bool intToBool(int i);

	//int类型转换成bool
	//static int boolToInt(bool b);

	//保留小数点后两位(四舍五入)
	static float keepTwoEffectNum(float f);

	//betweenAnd（1， 2， 3）参数1是否在范围【2， 3】
	static bool betweenAnd(int p, int min, int max);

	//数组赋值（length数组长度,将src里面【0-length）的值赋值到des里面)--注意越界问题
	static void arrayAssignment(int length, int des[], int src[]);

	//比较大小
	static int maxInt(int a, int b);
	static int minInt(int a, int b);

private:
	Tools();

};

#endif // TOOLS_H_
