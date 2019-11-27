#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"
#include <map>
#include <string>

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const uint8				m_cbCardListData[52*8];				//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	uint8 GetCardValue(uint8 cbCardData) { return cbCardData&LOGIC_MASK_VALUE; }
	//获取花色
	uint8 GetCardColor(uint8 cbCardData) { return cbCardData&LOGIC_MASK_COLOR; }
	uint8 GetCardLogicValue(uint8 cbCardData)
	{
		return GetCardValue(cbCardData);
		////扑克属性
		//uint8 cbCardColor=GetCardColor(cbCardData);
		//uint8 cbCardValue=GetCardValue(cbCardData);

		////转换数值
		//if (cbCardColor==4) return cbCardValue+2;
		//return (cbCardValue<=2)?(cbCardValue+13):cbCardValue;
	}

	//控制函数
public:
	//混乱扑克
	void RandCardList(uint8 cbCardBuffer[], int cbBufferCount);
	//根据类型得到牌
	tagCardType GetCardByResultType(tagCardType pResultType,uint8 *firstcard,uint8 *secondcard);
};

//////////////////////////////////////////////////////////////////////////

#endif
