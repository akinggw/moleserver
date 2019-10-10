#ifndef MOLE2D_MESSAGE_OUT_H
#define MOLE2D_MESSAGE_OUT_H

/**
 * Mole2D 游戏引擎
 *
 * 这个文件属于整个引擎的一部分
 *
 * 作者：akinggw
 * 建立时间： 2008.6.24
 */

#include <string>
#include <iosfwd>

#include "common.h"

/**
* 用于建立输出信息
*/
class CMolMessageOut : public NedAllocatedObject
{
public:
	/// 构造函数
	CMolMessageOut();
	/// 带消息ID的构造函数
	CMolMessageOut(uint16 id);
	/// 析构函数
	~CMolMessageOut();

	/// 清除当前信息
	void clear();
	/// 写一个BYTE进去
	//void write8(uint8 value);
	/// 写入一堆BYTE进去
	void writeBytes(uint8 *data,uint32 length);
	/// 写一个SHORT进去
	void write16(int16 value);
	/// 写一个LONG进去
	void write32(int32 value);
	/// 写一个FLOAT进去
	//void writeFloat(float value);
	/// 写一个64位进去
	void write64(int64 value);
	/// 写一个字符串进去
	void writeString(const std::string &str, int length = -1);
	/// 得到消息
	char* getData() const { return mData; }
	/// 得到消息长度
	unsigned int getLength() const { return mPos; }

private:
	/// 确定缓冲区的容量很大
	void expand(size_t bytes);

	char *mData;
	unsigned int mPos;
	unsigned int mDataSize;
};

#endif
