#include "../../include/MahJong/MahJongLogic.h"

void CMahJongLogic::Release()
{
	if (m_PMJAlgorithm)
	{
		delete m_PMJAlgorithm;
		m_PMJAlgorithm = 0;
	}
}
void CMahJongLogic::SetMahJongAlg(CMahJongAlg *PMJAlgorithm)
{
	if (m_PMJAlgorithm)
	{
		delete m_PMJAlgorithm;
		m_PMJAlgorithm = 0;
	}

	m_PMJAlgorithm = PMJAlgorithm;
}

CMahJongAlg * CMahJongLogic::GetMahJongAlg()
{
	return m_PMJAlgorithm;
}

//出牌操作
bool CMahJongLogic::OutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ)
{
	if (GetRequiredCheck())
	{
		if (iSelfHandMJCount <= 0)
		{
			return false;
		}
	}
	if (DeleteMJ(bSelfHandMJ, iSelfHandMJCount, bOutMJ))
	{
		bSelfOutMJ[iSelfOutMJCount++] = bOutMJ;
		//SortHandMJ(bSelfHandMJ, bSelfHandMJCount);
		return true;
	}
	return false;
}


//撤销出牌操作
bool CMahJongLogic::UnDoOutMJ(MJBYTE bSelfHandMJ[], int &iSelfHandMJCount, MJBYTE bSelfOutMJ[], int &iSelfOutMJCount, MJBYTE bOutMJ)
{
	//判定当前手牌应是3N+1而且出牌区最后一张牌为指定牌
	if(bSelfOutMJ[iSelfOutMJCount-1] == bOutMJ && iSelfHandMJCount %3 ==1)
	{
		iSelfOutMJCount--;
		bSelfHandMJ[iSelfHandMJCount++] = bOutMJ;
		return true;
	}
	else
	{
		return false;
	}
}

//吃操作
bool CMahJongLogic::Chi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType)
{
	//先检测这个是不是合法操作
	MJBYTE bMjColor = m_PMJAlgorithm->GetMahJongColor();
	if (bMjColor & MahJongColor_Word)
	{
		if (MASK_MAHJONGKIND(bChi) >= MahJong_Word)
		{
			//如果是字牌或花牌,则出错
			return false;
		}
	}
	if (GetRequiredCheck())
	{
		if ((bChiType == Operation_Chi_Type_Left && MASK_MAHJONGNUM(bChi) > 7) || (bChiType == Operation_Chi_Type_Midd && MASK_MAHJONGNUM(bChi) > 8))
		{
			//*00吃法,吃的那个大于7出错；0*0吃法,吃的那个大于8出错
			return false;
		}
		//检查是不是对的。如左边吃7W，则应该存在89W
		switch(bChiType)
		{
			case Operation_Chi_Type_Left:
				{
					if (!CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi+1) || !CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi+2))
					{
						return false;
					}
				}
				break;
			case Operation_Chi_Type_Midd:
				{
					if (!CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi-1) || !CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi+1))
					{
						return false;
					}
				}
				break;
			case Operation_Chi_Type_Right:
				{
					if (!CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi-2) || !CheckMJExist(bSelfHandMJ, iSelfHandCount, bChi-1))
					{
						return false;
					}
				}
				break;
		}
	}

	iLastOutMJCount--;
	switch(bChiType)
	{
	case Operation_Chi_Type_Left:
		{
			//从手牌中删除 bChi+1 bChi+2各一张牌
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi+1);
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi+2);
			bSelfHuMJ[iSelfHuCount].first = bChi;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Chi;
			iSelfHuCount++;
		}
		break;
	case Operation_Chi_Type_Midd:
		{
			//从手牌中删除 bChi-1 bChi+1各一张牌
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi-1);
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi+1);
			bSelfHuMJ[iSelfHuCount].first = bChi-1;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Chi;
			iSelfHuCount++;
		}
		break;
	case Operation_Chi_Type_Right:
		{
			//从手牌中删除 bChi-2 bChi-1各一张牌
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi-2);
			DeleteMJ(bSelfHandMJ, iSelfHandCount, bChi-1);
			bSelfHuMJ[iSelfHuCount].first = bChi-2;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Chi;
			iSelfHuCount++;
		}
		break;
	}
	return true;
}

//撤销吃操作
bool CMahJongLogic::UnDoChi(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bChi, enOperationChiType bChiType)
{
	//胡牌区的最后一个为指定吃牌
	if (bSelfHuMJ[iSelfHuCount-1].style == BlockStyle_Chi && iSelfHandCount % 3 == 2)
	{
		switch(bChiType)
		{
		case Operation_Chi_Type_Left:
			{
				//*00
				if (bSelfHuMJ[iSelfHuCount-1].first != bChi)
				{
					return false;
				}
			}
			break;
		case Operation_Chi_Type_Midd:
			{
				//0*0
				if (bSelfHuMJ[iSelfHuCount-1].first != bChi-1)
				{
					return false;
				}
			}
			break;
		case Operation_Chi_Type_Right:
			{
				//00*
				if (bSelfHuMJ[iSelfHuCount-1].first != bChi-2)
				{
					return false;
				}
			}
			break;
		}
	}
	else
	{
		return false;
	}

	bLastOutMJ[iLastOutMJCount++] = bChi;	//补回吃掉的出牌
	iSelfHuCount--;	//删除吃的胡牌结构

	//补回吃牌后去掉的手牌部分
	switch(bChiType)
	{
	case Operation_Chi_Type_Left:
		{
			//从手牌中补回 bChi+1 bChi+2各一张牌
			bSelfHandMJ[iSelfHandCount++] = bChi+1;
			bSelfHandMJ[iSelfHandCount++] = bChi+2;
		}
		break;
	case Operation_Chi_Type_Midd:
		{
			//从手牌中补回 bChi-1 bChi+1各一张牌
			bSelfHandMJ[iSelfHandCount++] = bChi-1;
			bSelfHandMJ[iSelfHandCount++] = bChi+1;
		}
		break;
	case Operation_Chi_Type_Right:
		{
			//从手牌中补回 bChi-2 bChi-1各一张牌
			bSelfHandMJ[iSelfHandCount++] = bChi-2;
			bSelfHandMJ[iSelfHandCount++] = bChi-1;
		}
		break;
	}
	//将手牌排序
	m_PMJAlgorithm->SortHandMJ(bSelfHandMJ, iSelfHandCount);
	return true;
}

//碰操作
bool CMahJongLogic::Peng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng)
{
	//检验下是不是合法的
	if (GetRequiredCheck())
	{
		if (GetMJCount(bSelfHandMJ, iSelfHandCount, bPeng) < 2)
		{
			return false;
		}
	}

	iLastOutMJCount--;
	bSelfHuMJ[iSelfHuCount].first = bPeng;
	bSelfHuMJ[iSelfHuCount].style = BlockStyle_Peng;
	iSelfHuCount++;
	DeleteMJ(bSelfHandMJ, iSelfHandCount, bPeng);
	DeleteMJ(bSelfHandMJ, iSelfHandCount, bPeng);//删2次

	return true;
}

//撤销碰操作
bool CMahJongLogic::UnDoPeng(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bPeng)
{
	//胡牌区最后一个应该是碰的牌
	if (bSelfHuMJ[iSelfHuCount-1].style == BlockStyle_Peng && bSelfHuMJ[iSelfHuCount-1].first == bPeng && iSelfHandCount % 3 == 2)
	{
		iSelfHuCount--;	//删除碰的牌
		bSelfHandMJ[iSelfHandCount++] = bPeng;
		bSelfHandMJ[iSelfHandCount++] = bPeng;
		bLastOutMJ[iLastOutMJCount++] = bPeng;
		m_PMJAlgorithm->SortHandMJ(bSelfHandMJ, iSelfHandCount);
		return true;
	}
	else
	{
		return false;
	}
}

//杠操作
bool CMahJongLogic::Gang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType)
{
	//检验下是不是合法的
	if (GetRequiredCheck())
	{
		switch(bGangType)
		{
		case Operation_Gang_Type_Ming:
			{
				//自己手牌有3张一样的
				int iCount = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
				if (iCount < 3 && iLastOutMJCount > 0)
				{
					return false;
				}
			}
			break;
		case Operation_Gang_Type_An:
			{
				//自己手牌有4张一样的
				int iCount = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
				if (iCount < 4)
				{
					return false;
				}
			}
			break;
		case Operation_Gang_Type_ZiMoMing:
			{
				//胡牌区有一个碰,手牌最后一张是指定牌
				if (bSelfHandMJ[iSelfHandCount-1] != bGang)
				{
					return false;
				}
				bool bExistFlag = false;
				for (int i=0;i<iSelfHuCount;i++)
				{
					if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bGang)
					{
						bExistFlag = true;
						break;
					}
				}
				if (!bExistFlag)
				{
					return false;
				}
			}
			break;
		case Operation_Gang_Type_XF:
			{
				if (bGang == 0x31)
				{
					int bGang01 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
					int bGang02 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x01);
					int bGang03 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x02);
					int bGang04 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x03);
					if(!(bGang01 && bGang02 && bGang03 && bGang04))
					{
						return false;
					}
				}
				if (bGang == 0x35)
				{
					int bGang05 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang);
					int bGang06 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x01);
					int bGang07 = m_PMJAlgorithm->GetMJCount(bSelfHandMJ, iSelfHandCount, bGang+0x02);
					if(!(bGang05 && bGang06 &&bGang07))
					{
						return false;
					}
				}
			}
			break;
		default:
			{
				return false;
			}
		}
	}

	//验证成功,操作
	switch(bGangType)
	{
	case Operation_Gang_Type_Ming:
		{
			//去除3张一样的手牌和出牌玩家的最后一张出牌,添加明杠到胡牌区中
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			iLastOutMJCount--;

			bSelfHuMJ[iSelfHuCount].first = bGang;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_Ming;
			iSelfHuCount++;
		}
		break;
	case Operation_Gang_Type_An:
		{
			//去除4张一样的手牌
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);

			bSelfHuMJ[iSelfHuCount].first = bGang;
			bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_An;
			iSelfHuCount++;
		}
		break;
	case Operation_Gang_Type_ZiMoMing:
		{
			//将碰牌变杠牌,删除指定手牌1次
			m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);

			for (int i=0;i<iSelfHuCount;i++)
			{
				if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bGang)
				{
					bSelfHuMJ[i].style = BlockStyle_Gang_ZiMoMing;
					break;
				}
			}
		}
		break;
	case Operation_Gang_Type_XF:
		{
			//去除4张东南西北的手牌
			if (bGang == 0x31)
			{
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x01);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x02);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x03);

				bSelfHuMJ[iSelfHuCount].first = bGang;
				bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_XF;
				iSelfHuCount++;
			}
			else if (bGang == 0x35)
			{
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x01);
				m_PMJAlgorithm->DeleteMJ(bSelfHandMJ, iSelfHandCount, bGang+0x02);

				bSelfHuMJ[iSelfHuCount].first = bGang;
				bSelfHuMJ[iSelfHuCount].style = BlockStyle_Gang_XF;
				iSelfHuCount++;
			}
		}
		break;
	}
	return true;
}


//撤销杠操作
bool CMahJongLogic::UnDoGang(MJBlock bSelfHuMJ[], int &iSelfHuCount, MJBYTE bSelfHandMJ[], int &iSelfHandCount, MJBYTE bLastOutMJ[], int &iLastOutMJCount, MJBYTE bGang, enOperationGangType bGangType)
{
	if (bSelfHuMJ[iSelfHuCount-1].first != bGang || iSelfHandCount % 3 != 1 || iSelfHuCount <= 0)
	{
		return false;
	}
	if (bGangType == Operation_Gang_Type_An && bSelfHuMJ[iSelfHuCount-1].style != BlockStyle_Gang_An)
	{
		return false;
	}
	else if (bGangType == Operation_Gang_Type_Ming && bSelfHuMJ[iSelfHuCount-1].style != BlockStyle_Gang_Ming)
	{
		return false;
	}
	else if (bGangType == Operation_Gang_Type_ZiMoMing && bSelfHuMJ[iSelfHuCount-1].style != BlockStyle_Gang_ZiMoMing)
	{
		return false;
	}

	switch(bGangType)
	{
	case Operation_Gang_Type_Ming:
		{
			//补回3张一样的手牌和出牌玩家的最后一张出牌,删除胡牌区明杠
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bLastOutMJ[iLastOutMJCount++] = bGang;
			iSelfHuCount--;
		}
		break;
	case Operation_Gang_Type_An:
		{
			//去除4张一样的手牌
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			bSelfHandMJ[iSelfHandCount++] = bGang;
			iSelfHuCount--;
		}
		break;
	case Operation_Gang_Type_ZiMoMing:
		{
			//将碰牌变杠牌,删除指定手牌1次
			bSelfHandMJ[iSelfHandCount++] = bGang;
			for (int i=0;i<iSelfHuCount;i++)
			{
				if (bSelfHuMJ[i].style == BlockStyle_Gang_ZiMoMing && bSelfHuMJ[i].first == bGang)
				{
					bSelfHuMJ[i].style = BlockStyle_Peng;
					break;
				}
			}
		}
		break;
	}
	m_PMJAlgorithm->SortHandMJ(bSelfHandMJ, iSelfHandCount);
	return true;
}

//杠操作(杠别人的牌)
//bool CMahJongLogic::NormalGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bLastOutMJ[], MJBYTE &bLastOutMJCount, MJBYTE bGang)
//{
//	//检测合法性,要有3张相同手牌才能杠别人出的牌
//	if (GetRequiredCheck())
//	{
//		if (GetMJCount(bSelfHandMJ, bSelfHandCount, bGang) < 3)
//		{
//			return false;
//		}
//	}
//
//	bLastOutMJCount--;
//
//	//删除手牌3次，添加胡牌区
//	for (int i = 3;i > 0;i--)
//	{
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bGang);
//	}
//	bSelfHuMJ[bSelfHuCount].first = bGang;
//	bSelfHuMJ[bSelfHuCount].style = BlockStyle_Gang;
//	bSelfHuCount++;
//	return true;
//}

//杠操作(杠自己摸的牌)
//bool CMahJongLogic::ZMGang(MJBlock bSelfHuMJ[], MJBYTE &bSelfHuCount, MJBYTE bSelfHandMJ[], MJBYTE &bSelfHandCount, MJBYTE bTouch)
//{
//	//判定是否自摸明杠
//	bool isMing = false;
//	for (int i = 0;i<bSelfHuCount;i++)
//	{
//		if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bTouch)
//		{
//			isMing = true;
//			break;
//		}
//	}
//
//	bool isAnGang = true;
//
//	if(GetRequiredCheck())
//	{
//		isAnGang = GetMJCount(bSelfHandMJ, bSelfHandCount, bTouch) == 4;
//	}
//
//	if (isMing)
//	{
//		//删除手牌1次
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		for (int i = 0;i<bSelfHuCount;i++)
//		{
//			if (bSelfHuMJ[i].style == BlockStyle_Peng && bSelfHuMJ[i].first == bTouch)
//			{
//				bSelfHuMJ[i].style = BlockStyle_Gang;
//				break;
//			}
//		}
//	}
//	else if (isAnGang)
//	{
//		//删除手牌4次，添加暗杠到胡牌区
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//		DeleteMJ(bSelfHandMJ, bSelfHandCount, bTouch);
//
//		bSelfHuMJ[bSelfHuCount].first = bTouch;
//		bSelfHuMJ[bSelfHuCount].style = BlockStyle_An_Gang;
//		bSelfHuCount++;
//	}
//	else
//	{
//		return false;
//	}
//
//	return true;
//}

int CMahJongLogic::GetRandomNum(int iMin, int iMax)
{
	return m_PMJAlgorithm->GetRandomNum(iMin, iMax);
}

//打乱排序
void CMahJongLogic::RandomMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	m_PMJAlgorithm->RandomMJ(bAllMJ, iAllMJCount);
}

//万饼条按从小到大排序
void CMahJongLogic::SortHandMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	m_PMJAlgorithm->SortHandMJ(bAllMJ, iAllMJCount);
}

//校验牌是否存在
bool CMahJongLogic::CheckMJExist(const MJBYTE bAllMJ[], int iAllMJCount, MJBYTE bMJ)
{
	return m_PMJAlgorithm->CheckMJExist(bAllMJ, iAllMJCount, bMJ);
}

//删除麻将
bool CMahJongLogic::DeleteMJ(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ)
{
	return m_PMJAlgorithm->DeleteMJ(bAllMJ, iAllMJCount, bMJ);
}
//获得指定麻将的数量
int CMahJongLogic::GetMJCount(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ)
{
	return m_PMJAlgorithm->GetMJCount(bAllMJ, iAllMJCount, bMJ);
}
//////////////////////////////////////////////////////////////////////////

//判定能否吃:上家打的牌才能吃
bool CMahJongLogic::IsChi(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut, MJBYTE &bChiType)
{
	MJFLAG bMjColor = m_PMJAlgorithm->GetMahJongColor();
	if (bMjColor & MahJongColor_Word)
	{
		if (MASK_MAHJONGKIND(bMJOut) >= MahJong_Word)
		{
			//如果是字牌或花牌,则出错
			return false;
		}
	}
	bChiType = 0;
	//bool bPosLeft = true, bPosMidd = true, bPosRight = true;
	//for (int i = 0 ;i < bSelfMJCount - 1; i++)
	//{
	//	//@**
	//	if( bSelfMJ[i] == bMJOut+1 && bPosLeft )
	//	{
	//		for( int j=i; j<bSelfMJCount; j++ )
	//		{
	//			if( bSelfMJ[j] == bMJOut+2 )
	//			{
	//				bChiType = bChiType | Operation_Chi_Type_Left;
	//				break;
	//			}
	//		}
	//		bPosLeft = false;
	//	}
	//	//*@*
	//	else if( bSelfMJ[i] == bMJOut-1 && bPosMidd )
	//	{
	//		for( int j=i; j<bSelfMJCount; j++ )
	//		{
	//			if( bSelfMJ[j] == bMJOut+1 )
	//			{
	//				bChiType = bChiType | Operation_Chi_Type_Midd;
	//				break;
	//			}
	//		}
	//		bPosMidd = false;
	//	}
	//	//**@
	//	else if( bSelfMJ[i] == bMJOut-2 && bPosRight )
	//	{
	//		for( int j=i; j<bSelfMJCount; j++ )
	//		{
	//			if( bSelfMJ[j] == bMJOut-1 )
	//			{
	//				bChiType = bChiType | Operation_Chi_Type_Right;
	//				break;
	//			}
	//		}
	//		bPosRight = false;
	//	}
	//}
	int iLLMJCount = MASK_MAHJONGNUM(bMJOut) > 2 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut-2) : 0;
	int iLMJCount = MASK_MAHJONGNUM(bMJOut) > 1 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut-1) : 0;
	int iRMJCount = MASK_MAHJONGNUM(bMJOut) < 9 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut+1) : 0;
	int iRRMJCount = MASK_MAHJONGNUM(bMJOut) < 8 ? GetMJCount(bSelfMJ, iSelfMJCount, bMJOut+2) : 0;
	if (iLLMJCount > 0 && iLMJCount > 0)
	{
		bChiType = bChiType | Operation_Chi_Type_Right;
	}
	if (iLMJCount > 0 && iRMJCount > 0)
	{
		bChiType = bChiType | Operation_Chi_Type_Midd;
	}
	if (iRMJCount > 0 && iRRMJCount > 0)
	{
		bChiType = bChiType | Operation_Chi_Type_Left;
	}
	return bChiType > 0;
}

//判定能否碰:除出牌者以外才能碰
bool CMahJongLogic::IsPeng(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut)
{
	return m_PMJAlgorithm->GetMJCount(bSelfMJ, iSelfMJCount, bMJOut) >= 2;
}

//判定能否明杠:别人出牌
bool CMahJongLogic::IsOutMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJOut)
{
	return m_PMJAlgorithm->GetMJCount(bSelfMJ, iSelfMJCount, bMJOut) >= 3;
}

//判定是否自摸明杠
bool CMahJongLogic::IsSelfMingGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bMJTouch)
{
	for(int i = 0; i < iSelfHUMJCount; i++)
	{
		if (bSelfHUMJ[i].style == BlockStyle_Peng && bSelfHUMJ[i].first == bMJTouch)
		{
			return true;
		}
	}
	return false;
}

//判定能否暗杠:自己摸的牌才能暗杠,将可杠的牌放入bGang
bool CMahJongLogic::IsAnGang(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfMJ[], int bSelfMJCount, MJBYTE bGang[], int &bGangCount)
{
	bGangCount = 0;

	int iCount[MAX_ARRAY_MJ] = {0};//每一种牌的数量
	for (int i=0; i < bSelfMJCount; i++)
	{
		iCount[MASK_MAHJONGKIND(bSelfMJ[i]) * 10 + MASK_MAHJONGNUM(bSelfMJ[i])]++;
	}
	//有4个的就是暗杠
	for (int i=0; i < MAX_ARRAY_MJ; i++)
	{
		if (iCount[i] >= 4)
		{
			bGang[bGangCount++] = MASK_MAHJONG(i/10, i%10);
		}
	}
	return bGangCount > 0 ? true:false;
}

//判定能否旋风杠:自己摸的牌才能旋风杠,将可杠的牌放入bGang
bool CMahJongLogic::IsXFGang(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfMJ[], int iSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &iGangCount)
{
	iGangCount = 0;

	int iCount[MAX_ARRAY_MJ] = {0};//每一种牌的数量
	iCount[MASK_MAHJONGKIND(bMJTouch) * 10 + MASK_MAHJONGNUM(bMJTouch)]++;
	for (int i=0; i < iSelfMJCount; i++)
	{
		iCount[MASK_MAHJONGKIND(bSelfMJ[i]) * 10 + MASK_MAHJONGNUM(bSelfMJ[i])]++;
	}
	//最多有四个旋风杠
	for (int i=0; i < 4; i++)
	{
		//31,32,33,34代表东南西北  35,36,37代表中发白
		if (iCount[31] >= 1 && iCount[32]>=1 && iCount[33]>=1 && iCount[34]>=1)
		{
			iCount[31]--;iCount[32]--;
			iCount[33]--;iCount[34]--;
			bGang[iGangCount++] = 0x31;
			continue;
		}
		else if (iCount[35]>=1 && iCount[36]>=1 && iCount[37]>=1)
		{
			iCount[35]--;iCount[36]--;iCount[37]--;
			bGang[iGangCount++] = 0x35;
			continue;
		}
		break;
	}
	return iGangCount > 0 ? true:false;

}
//判定能否暗杠:自己摸的牌才能暗杠,将可杠的牌放入bGang
bool CMahJongLogic::IsAnGang(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfMJ[], int bSelfMJCount, MJBYTE bMJTouch, MJBYTE bGang[], int &bGangCount)
{
	bGangCount = 0;

	int iCount[MAX_ARRAY_MJ] = {0};//每一种牌的数量
	iCount[MASK_MAHJONGKIND(bMJTouch) * 10 + MASK_MAHJONGNUM(bMJTouch)]++;
	for (int i=0; i < bSelfMJCount; i++)
	{
		iCount[MASK_MAHJONGKIND(bSelfMJ[i]) * 10 + MASK_MAHJONGNUM(bSelfMJ[i])]++;
	}
	//有4个的就是暗杠
	for (int i=0; i < MAX_ARRAY_MJ; i++)
	{
		if (iCount[i] >= 4)
		{
			bGang[bGangCount++] = MASK_MAHJONG(i/10, i%10);
		}
	}
	return bGangCount > 0 ? true:false;
}

//判定能否胡牌
bool CMahJongLogic::IsHu(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfHandMJ[], int bSelfHandMJCount, MJBYTE bMJ)
{
	MJBYTE bHandMJ[MAX_HAND_MJ] = {0};
	memcpy(bHandMJ, bSelfHandMJ, sizeof(bHandMJ));
	MJBYTE bHandMJCount = bSelfHandMJCount;
	bHandMJ[bHandMJCount++] = bMJ;

	SortHandMJ(bHandMJ, bHandMJCount);

	//判定是否胡牌
	if (m_PMJAlgorithm->IsNormalHu(bSelfHUMJ, bSelfHUMJCount, bHandMJ, bHandMJCount))
	{
		return true;
	}

	return false;
}
