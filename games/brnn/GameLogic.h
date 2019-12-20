#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"

//-----------------------------------------------
//牌型
enum emCardType
{
	CT_ERROR			=	0,		//错误类型
	CT_POINT			=	1,		//点数
	CT_SPECIAL_NIU1		=	3,		//牛1
	CT_SPECIAL_NIU2		=	4,		//牛2
	CT_SPECIAL_NIU3		=	5,		//牛3
	CT_SPECIAL_NIU4		=	6,		//牛4
	CT_SPECIAL_NIU5		=	7,		//牛5
	CT_SPECIAL_NIU6		=	8,		//牛6
	CT_SPECIAL_NIU7		=	9,		//牛7
	CT_SPECIAL_NIU8		=	10,		//牛9
	CT_SPECIAL_NIU9		=	11,		//牛9
	CT_SPECIAL_NIUNIUXW	=	12,		//小牛
	CT_SPECIAL_NIUNIUDW	=	13,		//大牛
	CT_SPECIAL_NIUNIU	=	14,		//牛牛
	CT_SPECIAL_NIUYING	=	15,		//银牛
	CT_SPECIAL_NIUKING	=	16,		//金牛
	CT_SPECIAL_BOMEBOME	=	17,		//炸弹

};
//牌型
//enum emCardType
//{
//	CT_ERROR			=	0,		//错误类型
//	CT_POINT			=	1,		//点数类型1
//	CT_SPECIAL_NIU1		=	2,		//特殊类型1
//	CT_SPECIAL_NIU2		=	3,		//特殊类型1
//	CT_SPECIAL_NIU3		=	4,		//特殊类型1
//	CT_SPECIAL_NIU4		=	5,		//特殊类型1
//	CT_SPECIAL_NIU5		=	6,		//特殊类型 1
//	CT_SPECIAL_NIU6		=	7,		//特殊类型 1
//	CT_SPECIAL_NIU7		=	8,		//特殊类型 2
//	CT_SPECIAL_NIU8		=	9,		//特殊类型 2
//	CT_SPECIAL_NIU9		=	10,		//特殊类型 3
//	CT_SPECIAL_NIUNIUXW	=	11,		//特殊类型 4
//	CT_SPECIAL_NIUNIUDW	=	12,		//特殊类型 5
//	CT_SPECIAL_NIUNIU	=	13,		//特殊类型 5
//	CT_SPECIAL_NIUYING	=	14,		//特殊类型 5
//	CT_SPECIAL_NIUKING	=	15,		//特殊类型 5
//	CT_SPECIAL_BOMEBOME	=	16,		//特殊类型 5
//
//};

//数值掩码
#define	LOGIC_MASK_COLOR	0xF0								//花色掩码
#define	LOGIC_MASK_VALUE	0x0F								//数值掩码

//排序类型
#define	ST_VALUE			1									//数值排序
#define	ST_NEW				3									//数值排序
#define	ST_LOGIC			2									//逻辑排序

//扑克数目
#define CARD_COUNT			54									//扑克数目

struct tagCardData
{
	tagCardData(){this->Init();}
	void Init(void){memset(this,0,sizeof(*this));}
	uint8		cbCard_[5];
	uint8		cbCardCnt;
};

//-----------------------------------------------
//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const uint8		s_cbCardListData[CARD_COUNT];		//扑克定义

	//函数定义
public:
	CGameLogic();
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	uint8 GetCardValue(uint8 cbCardData)
	{
		return cbCardData&LOGIC_MASK_VALUE;
	}
	//获取花色
	uint8 GetCardColor(uint8 cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//控制函数
public:
	//混乱扑克
	void RandCardList(uint8 cbCardBuffer[], uint8 cbBufferCount);
	//排列扑克
	void SortCardList(uint8 cbCardData[], uint8 cbCardCount, uint8 cbSortType);

	//排列扑克m_cbTableCardArray[CARD_LOCATION_COUNT][HAND_CARD_CNT]
	void SortCardList_Big(uint8 cbCardDataArray[5][5]);

	int RetType(int itype);

	//逻辑函数
public:
	//获取牌点
	uint8 GetCardListPip(const uint8 cbCardData[], uint8 cbCardCount);
	//获取牌型
	uint8 GetCardType(const uint8 cbCardData[], uint8 cbCardCount,uint8 *bcOutCadData = NULL);
	//大小比较
	int CompareCard(const uint8 cbFirstCardData[], uint8 cbFirstCardCount,const uint8 cbNextCardData[], uint8 cbNextCardCount,uint8 &Multiple);
	//逻辑大小
	uint8 GetCardLogicValue(uint8 cbCardData);

	uint8 GetCardNewValue(uint8 cbCardData);

			//是否有效的区域
		//根据牌型得到倍数
	uint8 GetMultiple(emCardType CardType);

};


#endif
