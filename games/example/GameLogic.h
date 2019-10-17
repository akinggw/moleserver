#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"

//数值掩码
#define	LOGIC_MASK_COLOR	0xF0								//花色掩码
#define	LOGIC_MASK_VALUE	0x0F								//数值掩码

//扑克数目
#define CARD_COUNT			54									//扑克数目

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
	/// 根据花色得到一张牌
	uint8 GetCardByColor(uint8 cardcolor);
};

#endif
