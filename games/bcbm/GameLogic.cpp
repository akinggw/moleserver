#include "GameLogic.h"

#include <vector>
#include <map>

#define CountArray(Array) (sizeof(Array)/sizeof(Array[0]))
int m_curZhongJiangPos = 0;

//////////////////////////////////////////////////////////////////////////

//构造函数
CGameLogic::CGameLogic()
{
	m_cbCardListData[0] = CARTYPE_FALALI;
	m_cbCardListData[1] = CARTYPE_BENCI;
	m_cbCardListData[2] = CARTYPE_LANBOJILI;
	m_cbCardListData[3] = CARTYPE_BAOMA;
	m_cbCardListData[4] = CARTYPE_MASAILADI;
	m_cbCardListData[5] = CARTYPE_AODI;
	m_cbCardListData[6] = CARTYPE_BAOSHIJIE;
	m_cbCardListData[7] = CARTYPE_DAZONG;
	m_cbCardListData[8] = CARTYPE_FALALI;

	m_cbCardListData[9] = CARTYPE_BENCI;
	m_cbCardListData[10] = CARTYPE_LANBOJILI;
	m_cbCardListData[11] = CARTYPE_BAOMA;
	m_cbCardListData[12] = CARTYPE_MASAILADI;
	m_cbCardListData[13] = CARTYPE_AODI;
	m_cbCardListData[14] = CARTYPE_BAOSHIJIE;
	m_cbCardListData[15] = CARTYPE_DAZONG;

	m_cbCardListData[16] = CARTYPE_FALALI;
	m_cbCardListData[17] = CARTYPE_BENCI;
	m_cbCardListData[18] = CARTYPE_LANBOJILI;
	m_cbCardListData[19] = CARTYPE_BAOMA;
	m_cbCardListData[20] = CARTYPE_MASAILADI;
	m_cbCardListData[21] = CARTYPE_AODI;
	m_cbCardListData[22] = CARTYPE_BAOSHIJIE;
	m_cbCardListData[23] = CARTYPE_DAZONG;
	m_cbCardListData[24] = CARTYPE_FALALI;

	m_cbCardListData[25] = CARTYPE_BENCI;
	m_cbCardListData[26] = CARTYPE_LANBOJILI;
	m_cbCardListData[27] = CARTYPE_BAOMA;
	m_cbCardListData[28] = CARTYPE_MASAILADI;
	m_cbCardListData[29] = CARTYPE_AODI;
	m_cbCardListData[30] = CARTYPE_BAOSHIJIE;
	m_cbCardListData[31] = CARTYPE_DAZONG;
}

//析构函数
CGameLogic::~CGameLogic()
{
}

/// 根据区域类型得到位置
int CGameLogic::GetPostionByCarType(tagCarType pCarType)
{
	int pTmpCarType[32];
	int pTmpCarTypeCount = 0;

	for(int i=0;i<32;i++)
	{
		if(m_cbCardListData[i] == pCarType)
		{
			pTmpCarType[pTmpCarTypeCount] = i;
			pTmpCarTypeCount+=1;
		}
	}

	int ptmpPos = 0;
	int pcount = 0;

	srand((uint32)time(0));
	while(true)
	{
		ptmpPos = pTmpCarType[rand()%pTmpCarTypeCount];

		if(ptmpPos != m_curZhongJiangPos)
			break;

		if(pcount > 100)
			break;

		pcount += 1;
	}

	m_curZhongJiangPos = ptmpPos;

	return m_curZhongJiangPos;
}

/// 得到押租类型的名称
std::string CGameLogic::GetCarTypeName(tagCarType pCarType)
{
	switch(pCarType)
	{
	case CARTYPE_FALALI:
		{
			return "法拉利";
		}
		break;
	case CARTYPE_LANBOJILI:
		{
			return "兰博基尼";
		}
		break;
	case CARTYPE_MASAILADI:
		{
			return "玛莎拉蒂";
		}
		break;
	case CARTYPE_BAOSHIJIE:
		{
			return "保时捷";
		}
		break;
	case CARTYPE_BENCI:
		{
			return "奔驰";
		}
		break;
	case CARTYPE_BAOMA:
		{
			return "宝马";
		}
		break;
	case CARTYPE_AODI:
		{
			return "奥迪";
		}
		break;
	case CARTYPE_DAZONG:
		{
			return "大众";
		}
		break;
	default:
		break;
	}

	return "";
}
