#include "GameLogic.h"

#include <vector>
#include <map>


//数组维数
#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))

//扑克数据
const uint8 CGameLogic::s_cbCardListData[CARD_COUNT]=
{
	0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,	//方块 A - K
	0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,	//梅花 A - K
	0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,	//红桃 A - K
	0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,	//黑桃 A - K
	0x4E,0x4F,
};

//-----------------------------------------------
CGameLogic::CGameLogic()
{
}

CGameLogic::~CGameLogic()
{
}

uint8 CGameLogic::GetCardByColor(uint8 cardcolor)
{
	if(cardcolor != 4)
		return s_cbCardListData[rand()%13+cardcolor*13];

	return s_cbCardListData[rand()%2+cardcolor*13];
}

//混乱扑克
void CGameLogic::RandCardList(uint8 cbCardBuffer[], uint8 cbBufferCount)
{
	//混乱准备
	uint8 cbCardData[CountArray(s_cbCardListData)];
	memcpy(cbCardData,s_cbCardListData,sizeof(s_cbCardListData));

	//混乱扑克
	uint8 cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(cbCardData)-cbRandCount);
		cbCardBuffer[cbRandCount++]=cbCardData[cbPosition];
		cbCardData[cbPosition]=cbCardData[CountArray(cbCardData)-cbRandCount];
	} while (cbRandCount<cbBufferCount);

	return;
}
