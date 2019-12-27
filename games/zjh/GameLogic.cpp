#include "GameLogic.h"

#include <vector>
#include <map>

//扑克数据
WORD CzjhGameLogic::m_wCardListData[IDD_MAX_CARD_COUNT]=
{
	0x0001,0x0002,0x0003,0x0004,0x0005,0x0006,0x0007,0x0008,0x0009,0x000A,0x000B,0x000C,0x000D,									//方块 A - K
	0x0101,0x0102,0x0103,0x0104,0x0105,0x0106,0x0107,0x0108,0x0109,0x010A,0x010B,0x010C,0x010D,									//梅花 A - K
	0x0201,0x0202,0x0203,0x0204,0x0205,0x0206,0x0207,0x0208,0x0209,0x020A,0x020B,0x020C,0x020D,									//红桃 A - K
	0x0301,0x0302,0x0303,0x0304,0x0305,0x0306,0x0307,0x0308,0x0309,0x030A,0x030B,0x030C,0x030D,									//黑桃 A - K
};



CzjhGameLogic::CzjhGameLogic()
{

}
CzjhGameLogic::~CzjhGameLogic()
{

}



////排列扑克
//void CzjhGameLogic::SortCard(WORD wCardData[IDD_MAX_HAND_COUNT])
//{
//	//转换数值
//	WORD wLogicValue[IDD_MAX_HAND_COUNT];
//	for (WORD i=0;i<IDD_MAX_HAND_COUNT;i++) wLogicValue[i]=GetCardValue(wCardData[i]);
//
//	//排序操作
//	bool bSorted=true;
//	WORD wTempData,wLast=IDD_MAX_HAND_COUNT-1;
//	do
//	{
//		bSorted=true;
//		for (BYTE i=0;i<wLast;i++)
//		{
//			if ((wLogicValue[i]<wLogicValue[i+1])||
//				((wLogicValue[i]==wLogicValue[i+1])&&(wCardData[i]<wCardData[i+1])))
//			{
//				//交换位置
//				wTempData=wCardData[i];
//				wCardData[i]=wCardData[i+1];
//				wCardData[i+1]=wTempData;
//				wTempData=wLogicValue[i];
//				wLogicValue[i]=wLogicValue[i+1];
//				wLogicValue[i+1]=wTempData;
//				bSorted=false;
//			}
//		}
//		wLast--;
//	} while(bSorted==false);
//
//	return;
//}


//排列扑克（AKQ--432）
void CzjhGameLogic::SortCardByLogicValue(WORD wCardData[IDD_MAX_HAND_COUNT])
{
	//转换数值
	WORD wLogicValue[IDD_MAX_HAND_COUNT];
	for (WORD i=0;i<IDD_MAX_HAND_COUNT;i++) wLogicValue[i]=GetCardLogicValue(wCardData[i]);

	//排序操作
	bool bSorted=true;
	WORD wTempData,wLast=IDD_MAX_HAND_COUNT-1;
	do
	{
		bSorted=true;
		for (BYTE i=0;i<wLast;i++)
		{
			if ((wLogicValue[i]<wLogicValue[i+1])||
				((wLogicValue[i]==wLogicValue[i+1])&&(wCardData[i]<wCardData[i+1])))
			{
				//交换位置
				wTempData=wCardData[i];
				wCardData[i]=wCardData[i+1];
				wCardData[i+1]=wTempData;
				wTempData=wLogicValue[i];
				wLogicValue[i]=wLogicValue[i+1];
				wLogicValue[i+1]=wTempData;
				bSorted=false;
			}
		}
		wLast--;
	} while(bSorted==false);

	return;
}
//获取逻辑数值
WORD CzjhGameLogic::GetCardLogicValue(WORD wCardData)
{
	WORD wCardValue=GetCardValue(wCardData);
	if (wCardValue==1)
	{
		wCardValue+=13;
	}
	return wCardValue;
}


//混乱扑克(洗牌)
void CzjhGameLogic::RandCardList(WORD wCardBuffer[IDD_MAX_ROOM_USERCOUNT][IDD_MAX_HAND_COUNT])
{
	WORD AllCount=IDD_MAX_ROOM_USERCOUNT*IDD_MAX_HAND_COUNT;
	//混乱准备
	WORD wCardData[CountArray(m_wCardListData)];
	memcpy(wCardData,m_wCardListData,sizeof(m_wCardListData));

	//混乱扑克
	BYTE cbRandCount=0,cbPosition=0;
	do
	{
		cbPosition=rand()%(CountArray(wCardData)-cbRandCount);
		wCardBuffer[cbRandCount/IDD_MAX_HAND_COUNT][cbRandCount%IDD_MAX_HAND_COUNT]=wCardData[cbPosition];
		cbRandCount++;
		wCardData[cbPosition]=wCardData[CountArray(wCardData)-cbRandCount];
	} while (cbRandCount<AllCount);

	return;
}




WORD CzjhGameLogic::GetCardType(WORD wCardData[IDD_MAX_HAND_COUNT])
{
	//变量定义
	bool bJingHua=true,bSunZi=true,bBaoZi=true;
	WORD wFirstColor=GetCardColor(wCardData[0]);
	WORD wFirstValue=GetCardLogicValue(wCardData[0]);

	//牌形分析
	for (BYTE i=1;i<IDD_MAX_HAND_COUNT;i++)
	{
		//数据分析
		if (GetCardColor(wCardData[i])!=wFirstColor) bJingHua=false;			//花色不同，不是金花
		if (wFirstValue!=(GetCardLogicValue(wCardData[i])+i)) bSunZi=false;		//不连续，不是顺子
		if (wFirstValue!=(GetCardLogicValue(wCardData[i]))) bBaoZi=false;			//不相同，不是豹子

		//结束判断
		if ((bJingHua==false)&&(bSunZi==false)&&(bBaoZi==false)) break;
	}
	if (bSunZi==false)
	{
		//特殊顺子  123 其他均为AKQ---432
		if (GetCardValue(wCardData[0])==1 && GetCardValue(wCardData[1])==3 && GetCardValue(wCardData[2])==2)
		{
			//对特殊顺子进行排序，以方便后面的比牌。
			WORD temp=wCardData[0];
			wCardData[0]=wCardData[1];
			wCardData[1]=wCardData[2];
			wCardData[2]=temp;
			bSunZi=true;
		}
	}
	if (bBaoZi)
	{
		return CT_BAO_ZI;
	}
	if (bSunZi&&bJingHua)
	{
		return CT_TONG_HUA_SHUN;
	}
	if (bJingHua)
	{
		return CT_JING_HUA;
	}
	if (bSunZi)
	{
		return CT_SHUN_ZI;
	}
	if (GetCardValue(wCardData[0])==GetCardValue(wCardData[1])||GetCardValue(wCardData[1])==GetCardValue(wCardData[2]))
	{
		return CT_DOUBLE;
	}
	if (GetCardValue(wCardData[0])==5 && GetCardValue(wCardData[1])==3 && GetCardValue(wCardData[2])==2)
	{
		return CT_TE_SHU;
	}
	return CT_SINGLE;
}


//对比扑克
bool CzjhGameLogic::CompareCard(WORD wFirstCardData[IDD_MAX_HAND_COUNT], WORD wNextCardData[IDD_MAX_HAND_COUNT])
{
	//由大到小排序
	SortCardByLogicValue(wFirstCardData);
	SortCardByLogicValue(wNextCardData);

	WORD FirstType=GetCardType(wFirstCardData);
	WORD NextType=GetCardType(wNextCardData);

	//不同类型快速判断
	if (FirstType!=NextType)
	{
		if (FirstType== CT_TE_SHU && NextType== CT_BAO_ZI)
		{
			return true;
		}
		if (FirstType== CT_BAO_ZI && NextType== CT_TE_SHU)
		{
			return false;
		}
		return FirstType>NextType;
	}

	//相同类型详细判断
	WORD FirstValue=GetCardLogicValue(wFirstCardData[0]);
	WORD NextValue=GetCardLogicValue(wNextCardData[0]);

	if (FirstType==CT_TONG_HUA_SHUN || FirstType==CT_SHUN_ZI || FirstType==CT_BAO_ZI)//同花顺，顺子 豹子 只要判断第一个牌就好了（特殊顺子在获取类型时排序，第一位是3）
	{
		return	FirstValue>NextValue;
	}

	if (FirstType==CT_DOUBLE)//对子先看对子，再看单牌
	{
		//排序后，中间的牌必定是对子中的一个
		if (GetCardValue(wFirstCardData[1])!=GetCardValue(wNextCardData[1]))
		{
			return GetCardLogicValue(wFirstCardData[1])>GetCardLogicValue(wNextCardData[1]);
		}

		//对子相同，获取最后的单牌
		if (FirstValue==GetCardLogicValue(wFirstCardData[1]))
		{
			FirstValue=GetCardLogicValue(wFirstCardData[2]);
		}
		if (NextValue==GetCardLogicValue(wNextCardData[1]))
		{
			NextValue=GetCardLogicValue(wNextCardData[2]);
		}

		return FirstValue>NextValue;
	}

	//单牌，金花，挨个比较
	for (WORD i=0; i<IDD_MAX_HAND_COUNT; i++)
	{
		FirstValue=GetCardLogicValue(wFirstCardData[i]);
		NextValue=GetCardLogicValue(wNextCardData[i]);

		if (FirstValue!=NextValue)
		{
			return	FirstValue>NextValue;
		}
	}
	//单牌，金花中，所有牌相同，开牌者输
	return false;
}
