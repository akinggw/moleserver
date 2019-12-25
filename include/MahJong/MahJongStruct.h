#ifndef MAHJONG_STRUCT_HEAD_FILE
#define MAHJONG_STRUCT_HEAD_FILE

#include <time.h>
#include "../molnet/common.h"

typedef int16 MJBYTE;										/*!< 定义麻将类型.至少需要8位,这里用16位 */
typedef int16 MJFLAG;										/*!< 定义库中使用的标记.至少需要16位 */

////////////////////////////////公共宏//////////////////////////////////////////

#define MASK_MAHJONGKIND(value)				((value)>>4)			/*!< 高4位是类型 */
#define MASK_MAHJONGNUM(value)				((value)&0x0F)			/*!< 低4位是数字 */
#define MASK_MAHJONG(kind,num)				(((kind)<<4)+num)		/*!< 类型和数字组合成麻将 */
//#define MASK_MAHJONGFENGORZI(value)			((value<0x35)?MahJongColor_Feng:MahJongColor_Word)			/*!< 0x31-0x34风牌 0x35-0x39字牌 */

////////////////////////////////公共常量//////////////////////////////////////////
#define MAX_ARRAY_MJ				38								/*!< 计算胡牌时存放各个麻将个数的数组0、10等位置不放。38为最大个数  */
#define RANDOM_TIMES				10								/*!< 打乱麻将的次数 */

#define	MAX_HAND_MJ					14								/*!< 用户手上最多牌数 */
#define MAX_BLOCK_COUNT				((MAX_HAND_MJ + 2) / 3)			/*!< 最多麻将块个数 */
#define CONST_MJ_HIDE				0								/*!< 牌对其他人隐藏时的数值 */

////////////////////////////////麻将结构//////////////////////////////////////////
//! 麻将类型
/*! 麻将块的类型,根据胡牌时块分类 */
enum enBlockStyle
{
	BlockStyle_Null	 = 0,											/*!< 无 */
	//////////////////////////////////胡牌中的结构////////////////////////////////////////
	BlockStyle_Chi,													/*!< 123吃牌结构 */
	BlockStyle_Peng,	 											/*!< 111碰牌结构 */
	BlockStyle_Gang_Ming,											/*!< 1111明杠结构 */
	BlockStyle_Gang_ZiMoMing,										/*!< 1111自摸明杠结构 */
	BlockStyle_Gang_An,												/*!< 1111暗杠结构 */
	BlockStyle_Gang_XF,												/*!< 东南西北 中发白 旋风杠结构 */
	BlockStyle_FengZiChi,											/*!< 东南西北 中发白 吃 */
	//////////////////////////////////手牌中的结构////////////////////////////////////////
	BlockStyle_Shun,	 											/*!< 顺序结构 */
	BlockStyle_Ke,													/*!< 刻子结构,3个相同,非碰 */
	BlockStyle_Jiang,												/*!< 将牌结构 */
};

//! 麻将块结构
/*! 麻将块,由几个麻将组合 */
typedef struct tagMJBlock
{
	MJBYTE					first;									/*!< 麻将块第一张牌值 */
	enBlockStyle			style;									/*!< 麻将块类型 */

	/////////////////////////////只有特殊的吃用(南昌麻将)/////////////////////////////////////////////
	MJBYTE					second;									/*!< 麻将块第二张牌值 */
	MJBYTE					last;									/*!< 麻将块第三张牌值 */
	//! 获得块里面的麻将
	/*!
		获得给定3张牌的类型
		\param mj 块里面的麻将数组
		\param iCount 块里面的麻将个数
		\return 是否正确获取数据
		\sa enBlockStyle
	*/
	bool getMJ(MJBYTE mj[], int &iCount)
	{
		iCount = 0;
		switch(style)
		{
		case BlockStyle_Chi:
		case BlockStyle_Shun:
			{
				mj[0] = first+0;
				mj[1] = first+1;
				mj[2] = first+2;
				iCount = 3;
			}
			break;
		case BlockStyle_Peng:
		case BlockStyle_Ke:
			{
				mj[0] = mj[1] = mj[2] = first;
				iCount = 3;
			}
			break;
		case BlockStyle_Gang_Ming:
		case BlockStyle_Gang_ZiMoMing:
			{
				mj[0] = mj[1] = mj[2] = mj[3] = first;
				iCount = 4;
			}
			break;
		case BlockStyle_Gang_An:
			{
				mj[0] =  mj[1] =  mj[2] = CONST_MJ_HIDE;
				mj[3] = first;
				iCount = 4;
			}
			break;
		case BlockStyle_Gang_XF:
			{
				//东南西北牌块
				if(first == 0x31)
				{
					mj[0] = first+0;
					mj[1] = first+1;
					mj[2] = first+2;
					mj[3] = first+3;
					iCount = 4;
				}
				//中发白牌块
				else if (first == 0x35)
				{

					mj[0] = first+0;
					mj[1] = first+1;
					mj[2] = first+2;
					iCount = 3;
				}
			}
			break;
		case BlockStyle_FengZiChi:
			{
				mj[0] = first;
				mj[1] = second;
				mj[2] = last;
				iCount = 3;
			}
			break;
		case BlockStyle_Jiang:
			{
				mj[0] = mj[1] = first;
				iCount = 2;
			}
			break;
		default:
			return false;
		}
		return true;
	}
}MJBlock,*PMJBlock;

//! 计算胡牌牌型的结构
/*! 用于存储胡牌组合 */
typedef struct tagHuNode
{
	MJBlock					mjBlock;								/*!< 当前麻将块 */
	struct tagHuNode		*tKe;									/*!< 下一个刻子 */
	struct tagHuNode		*tShun;									/*!< 下一个顺子 */
	struct tagHuNode		*tJiang;								/*!< 下一个将 */
}HuNode, *HuTree;

/////////////////////////////麻将相关定义/////////////////////////////////////////////

//! 麻将种类
/*! 麻将种类: 万(1-9) 条(1-9) 饼(1-9),字,花如果有字牌或者其他自行添加 */
enum enMahJongKind
{
	MahJong_Wan = 0,												/*!< 万子 1~9万 */
	MahJong_Tiao,													/*!< 条子 1~9条 */
	MahJong_Bing,													/*!< 筒子 1~9筒 */
	MahJong_Word,													/*!< 字牌 东南西北中发白 */
	MahJong_Flower,													/*!< 花牌 春夏秋冬梅兰竹菊 */
	MahJong_Invalid = 99											/*!< 非法值 */
};

//! 吃操作的类型
enum enOperationChiType
{
	Operation_Chi_Type_Null			= 0x00,							/*!< 非法值 */
	Operation_Chi_Type_Left			= 0x01,							/*!< 吃：0** */
	Operation_Chi_Type_Midd			= 0x02,							/*!< 吃：*0* */
	Operation_Chi_Type_Right		= 0x04							/*!< 吃：**0 */
};

//! 杠操作的类型
/*! 杠操作的类型 */
enum enOperationGangType
{
	Operation_Gang_Type_Null		= 0x00,							/*!< 无 */
	Operation_Gang_Type_ZiMoMing	= 0x01,							/*!< 面下杠(自摸明杠) */
	Operation_Gang_Type_Ming		= 0x02,							/*!< 直杠 */
	Operation_Gang_Type_An			= 0x04,							/*!< 暗杠 */
	Operation_Gang_Type_XF			= 0x08,							/*!< 旋风杠 */
};
//////////////////////////////////////////////////////////////////////////
#endif
