#ifndef _MOL_STRING_H_INCLUDE_
#define _MOL_STRING_H_INCLUDE_

/**
 * Mole2D 游戏引擎
 *
 * 这个文件属于整个引擎的一部分
 *
 * 作者：akinggw
 * 建立时间： 2010.8.13
 */

#include "common.h"

/**
 * 自定义字符串处理类，主要用于解决字符串的网络传输中崩溃问题
 */
class CMolString
{
public:
	/// 构造函数
	CMolString()
		: count(0)
	{ }
	/// 带参数的构造函数
	CMolString(std::string str)
		: count(0)
	{
		memset(array,0,MOL_STR_BUFFER_SIZE);
		strcpy(array,str.c_str());
		array[(int)str.length()] = '\0';
		count = (int)str.length();
	}
	/// 带参数的构造函数2
	CMolString(const char* begin,int length)
		: count(0)
	{
		memset(array,0,MOL_STR_BUFFER_SIZE);
		memcpy((void*)array,begin,length);
		array[length] = '\0';
		count = length;
	}

	/// 得到字符串
	char* c_str(void) { return array; }
	/// 取得指定位置的字符串
	char at(int index)
	{
		if(index < 0 || index >= count)
			return -1;

		return array[index];
	}
	/// 清空字符串
	void clear(void)
	{
		count = 0;
	}
	/// 检测字符串是否为空
	bool empty(void)
	{
		return count > 0 ? false : true;
	}
	/// 得到字符串的长度
	int length(void)
	{
		return count;
	}
	/// 赋值操作
	CMolString& operator=(CMolString& other)
	{
		if(other.empty()) return *this;

		if(array) clear();

		memset(array,0,MOL_STR_BUFFER_SIZE);
		strcpy(array,other.c_str());
		array[other.length()] = '\0';
		count = other.length();

		return *this;
	}

private:
	char array[MOL_STR_BUFFER_SIZE];
	int count;
};

#endif
