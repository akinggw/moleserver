#ifndef MOLE2D_MESSAGE_IN_H
#define MOLE2D_MESSAGE_IN_H

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

//#include "VMemPool.h"
#include "MolString.h"

/**
* 用于处理输入信息
*/
class CMolMessageIn : public NedAllocatedObject
{
public:
	/// 初始的构造函数
	CMolMessageIn();
	/// 构造函数
	CMolMessageIn(const char *data, int length);
	/// 析构函数
	~CMolMessageIn();

	/// 得到消息ID
	uint16 getId() const { return mId; }
	/// 得到消息的总长度
	int getLength() const { return mLength; }
	/// 读取一个BYTE数据
	//uint8 readByte();
	/// 读取一个short数据
	int16 read16();
	/// 读取一个long数据
	int32 read32();
	/// 读取一个float数据
	//float readFloat();
	/// 读取一个64数据
	int64 read64();
	/// 读取一个字符串，如果长度为-1的话，长度就存储在字符串开始的第一个short中
	CMolString readString(int length = -1);
	/// 返回没有读取的数据的长度
	int getUnreadLength() const { return mLength - mPos; }
	/// 得到消息
	const char* getData() const { return mData; }

private:
	char mData[MOL_REV_BUFFER_SIZE_TWO];         /**< 数据包数据 */
	unsigned short mLength;    /**< 数据包的总长度 */
	unsigned short mId;        /**< 数据包的ID */

	unsigned short mPos;       /**< 数据实际读取的位置 */
};

#endif
