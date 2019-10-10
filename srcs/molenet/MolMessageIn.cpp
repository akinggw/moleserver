#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#include "../../include/molnet/MolMessageIn.h"

/**
 * 初始的构造函数
 */
CMolMessageIn::CMolMessageIn() :
	mLength(0),
	mPos(0)
{

}

/**
 * 构造函数
 *
 * @param data 相应的包数据
 * @param length 包数据的长度
 */
CMolMessageIn::CMolMessageIn(const char *data, int length) :
    mLength(0),
    mPos(0)
{
	if(mLength < MOL_REV_BUFFER_SIZE_TWO)
	{
		memcpy((void*)mData,data,length);
		mLength = length;
		mId = read16();
	}
}

/**
 * 析构函数
 */
CMolMessageIn::~CMolMessageIn()
{

}

/**
 * 读取一个BYTE数据
 */
//uint8 CMolMessageIn::readByte()
//{
//    uint8 value = -1;
//    if (mPos < mLength)
//    {
//        value = (uint8) mData[mPos];
//    }
//    mPos += 1;
//    return value;
//}

/**
 * 读取一个short数据
 */
int16 CMolMessageIn::read16()
{
    int16 value = -1;
    if (mPos + 2 <= mLength)
    {
        int16 t;
        memcpy(&t, mData + mPos, 2);
        value = (int16)/*MOL_NET_TO_HOST_16*/(t);
    }
    mPos += 2;
    return value;
}

/**
 * 读取一个long数据
 */
int32 CMolMessageIn::read32()
{
    int32 value = -1;
    if (mPos + 4 <= mLength)
    {
        int32 t;
        memcpy(&t, mData + mPos, 4);
        value = (int32)/*MOL_NET_TO_HOST_32*/(t);
    }
    mPos += 4;
    return value;
}

/// 读取一个float数据
//float CMolMessageIn::readFloat()
//{
//	float value = -1;
//	if (mPos + 4 <= mLength)
//	{
//		float t;
//		memcpy(&t, mData + mPos, 4);
//		value = (float)(t);
//	}
//	mPos += 4;
//	return value;
//}

/// 读取一个64数据
int64 CMolMessageIn::read64()
{
	int64 value = -1;
	if (mPos + 8 <= mLength)
	{
		int64 t;
		memcpy(&t, mData + mPos, 8);
		value = (int64)/*MOL_NET_TO_HOST_64*/(t);
	}
	mPos += 8;
	return value;
}

/**
 * 读取一个字符串
 *
 * @param length 如果长度为-1的话，长度就存储在字符串开始的第一个short中
 *
 * @return 返回读取的字符串
 */
CMolString CMolMessageIn::readString(int length)
{
    if (length < 0)
    {
        length = read16();
    }

    if (length < 0 || mPos + length > mLength)
    {
        mPos = mLength + 1;
        return CMolString();
    }

    char const *stringBeg = mData + mPos;
    char const *stringEnd = (char const *)memchr(stringBeg, '\0', length);
    CMolString readString(stringBeg,
            stringEnd ? (int)(stringEnd - stringBeg) : length);
    mPos += length;

    return readString;
}
