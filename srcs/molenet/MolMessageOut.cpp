#include <cstring>
#include <iomanip>
#include <iostream>
#include <string>

#include "../../include/molnet/MolMessageOut.h"

#if defined(__cplusplus)
extern "C" {
#endif
    #include "../../include/molnet/netallocator/NedAllocatorImpl.h"
 #if defined(__cplusplus)
}
#endif

/// 初始的消息缓冲区容量
const unsigned int INITIAL_DATA_CAPACITY = 16;
/// 容量增长因子
const unsigned int CAPACITY_GROW_FACTOR = 2;

/**
 * 构造函数
 */
CMolMessageOut::CMolMessageOut() :
     mPos(0)
{
    mData = (char*) MemoryMalloc(INITIAL_DATA_CAPACITY);
    mDataSize = INITIAL_DATA_CAPACITY;
}

/**
 * 带消息ID的构造函数
 *
 * @param id 消息ID
 */
CMolMessageOut::CMolMessageOut(uint16 id):
    mPos(0)
{
    mData = (char*) MemoryMalloc(INITIAL_DATA_CAPACITY);
    mDataSize = INITIAL_DATA_CAPACITY;

    write16(id);
}

/**
 * 析构函数
 */
CMolMessageOut::~CMolMessageOut()
{
    if(mData) MemoryFree(mData);
}

/**
 * 清除当前信息
 */
void CMolMessageOut::clear()
{
    mData = (char *) Memoryrealloc(mData, INITIAL_DATA_CAPACITY);
    mDataSize = INITIAL_DATA_CAPACITY;
    mPos = 0;
}

/**
 * 写一个BYTE进去
 *
 * @param value 要写入的BYTE值
 */
//void CMolMessageOut::write8(uint8 value)
//{
//    expand(mPos + 1);
//    mData[mPos] = value;
//    mPos += 1;
//}

/**
 * 写入一堆BYTE进去
 *
 * @param data 要写入的数据
 * @param length 要写入的数据长度
 */
void CMolMessageOut::writeBytes(uint8 *data,uint32 length)
{
	if(data == NULL || length <= 0) return;

	expand(mPos + length);
	memcpy(mData+mPos,data,length);
	mPos += length;
}

/**
 * 确定缓冲区的容量很大
 *
 * @param size 要申请的缓冲区大小
 */
void CMolMessageOut::expand(size_t bytes)
{
    if (bytes > mDataSize)
    {
        do
        {
            mDataSize *= CAPACITY_GROW_FACTOR;
        }
        while (bytes > mDataSize);

        mData = (char*) Memoryrealloc(mData, mDataSize);
    }
}

/**
 * 写一个SHORT进去
 *
 * @param value 要写入的SHORT值
 */
void CMolMessageOut::write16(int16 value)
{
    expand(mPos + 2);
    int16 t = (int16)/*MOL_HOST_TO_NET_16*/(value);
    memcpy(mData + mPos, &t, 2);
    mPos += 2;
}

/**
 * 写一个LONG进去
 *
 * @param value 要写入的LONG值
 */
void CMolMessageOut::write32(int32 value)
{
    expand(mPos + 4);
    int32 t = (int32)/*MOL_HOST_TO_NET_32*/(value);
    memcpy(mData + mPos, &t, 4);
    mPos += 4;
}

/**
 * 写一个FLOAT进去
 *
 * @param value 要写入的值
 */
//void CMolMessageOut::writeFloat(float value)
//{
//	expand(mPos + 4);
//	memcpy(mData + mPos, &value, 4);
//	mPos += 4;
//}

/**
 * 写一个64进去
 *
 * @param value 要写入的值
 */
void CMolMessageOut::write64(int64 value)
{
	expand(mPos + 8);
	int64 t = (int64)/*MOL_HOST_TO_NET_64*/(value);
	memcpy(mData + mPos, &t, 8);
	mPos += 8;
}

/**
 * 写一个字符串进去
 *
 * @param string 要写入的字符串
 * @param length 字符串长度
 */
void CMolMessageOut::writeString(const std::string &str, int length)
{
    int stringLength = (int)str.length();
    if (length < 0)
    {
        write16(stringLength);
        length = stringLength;
    }
    else if (length < stringLength)
    {
        stringLength = length;
    }
    expand(mPos + length);

    memcpy(mData + mPos, str.c_str(), stringLength);

    if (length > stringLength)
    {
        memset(mData + mPos + stringLength, '\0', length - stringLength);
    }
    mPos += length;
}
