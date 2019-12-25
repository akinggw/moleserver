#include "../../include/MahJong/MahJongAlg.h"

//万饼条按从小到大排序
void CMahJongAlg::SortHandMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	MJBYTE bTmp = -1;
	for (int i=0;i<iAllMJCount;i++)
	{
		for (int j=i+1;j<iAllMJCount;j++)
		{
			if (bAllMJ[i] > bAllMJ[j])
			{
				bTmp = bAllMJ[i];
				bAllMJ[i] = bAllMJ[j];
				bAllMJ[j] = bTmp;
			}
		}
	}
}

//打乱排序
void CMahJongAlg::RandomMJ(MJBYTE bAllMJ[], int iAllMJCount)
{
	int index=0 ,iTmp=0;
	int iRandCount = rand() % 6 + 5;
	for (int i = 0;i < iRandCount; i++)
	{
		for( int j=0; j<iAllMJCount; j++ )
		{
			index = GetRandomNum(0, iAllMJCount-1);
			if( j!=index )
			{
				iTmp = bAllMJ[j];
				bAllMJ[j] = bAllMJ[index];
				bAllMJ[index] = iTmp;
			}
		}
	}
}

//校验牌是否存在
bool CMahJongAlg::CheckMJExist(const MJBYTE bAllMJ[], int iAllMJCount, MJBYTE bMJ)
{
	for (int i=0;i<iAllMJCount;i++)
	{
		if (bAllMJ[i] == bMJ)
		{
			return true;
		}
	}
	return false;
}

//删除麻将
bool CMahJongAlg::DeleteMJ(MJBYTE bAllMJ[], int &iAllMJCount, MJBYTE bMJ)
{
	if (iAllMJCount <= 0)
	{
		return false;
	}
	if (bAllMJ[0] == CONST_MJ_HIDE)
	{
		for (int i=0;i<iAllMJCount;i++)
		{
			if (bAllMJ[i] == CONST_MJ_HIDE)
			{
				for(int j=i;j<iAllMJCount-1;j++)
				{
					bAllMJ[j] = bAllMJ[j+1];
				}
				iAllMJCount -= 1;
				return true;
			}
		}
		//iAllMJCount --;
		return true;
	}
	else
	{
		for (int i=0;i<iAllMJCount;i++)
		{
			if (bAllMJ[i] == bMJ)
			{
				for(int j=i;j<iAllMJCount-1;j++)
				{
					bAllMJ[j] = bAllMJ[j+1];
				}
				iAllMJCount -= 1;
				return true;
			}
		}
	}
	return false;
}

//获得指定麻将的数量
int CMahJongAlg::GetMJCount(MJBYTE bMJ[], int &iMJCount, MJBYTE bM)
{
	int iCount = 0;
	for (int i=0; i<iMJCount;i++)
	{
		if (bMJ[i] == bM)
		{
			iCount++;
		}
	}
	return iCount;
}

//////////////////////////////////////////////////////////////////////////
bool CMahJongAlg::IsNormalHu(MJBlock bSelfHUMJ[], int iSelfHUMJCount, MJBYTE bSelfHandMJ[], int iSelfHandMJCount)
{
	return CheckRestHu(bSelfHandMJ, iSelfHandMJCount);
}

//检测剩余的牌是否能胡，以数组表示
bool CMahJongAlg::CheckRestHu(MJBYTE bHandMJ[], int iHandMJCount)
{
	MJBYTE bCheckMJ[MAX_ARRAY_MJ] = {0};
	for (int i=0; i<iHandMJCount; i++)
	{
		if (m_bMahJongColor & MahJong_Flower)
		{
			if (MASK_MAHJONGKIND(bHandMJ[i]) == MahJong_Flower)
			{
				return false;
			}
		}

		bCheckMJ[MASK_MAHJONGKIND(bHandMJ[i])*10 + MASK_MAHJONGNUM(bHandMJ[i])]++;
	}

	return CheckIsHu(bCheckMJ, MAX_ARRAY_MJ);
}

//检查牌是否能胡，将各牌的个数保存在38大小的数组中
bool CMahJongAlg::CheckIsHu(MJBYTE bMJ[], int iMaxCount)
{
	//static int JIANG = 0;						// 将牌标志,即牌型“三三三三二”中的“二” ,放到第0号位置
	if( !Remain(bMJ, iMaxCount) ) return 1;     // 递归退出条件：如果没有剩牌,则和牌返回。
	int i=0;
	for(i=1; !bMJ[i] && i<iMaxCount; i++);  // 找到有牌的地方,i就是当前牌, PAI[i]是个数
	//printf("i = %d\n",i);						// 跟踪信息

	// 4张组合(杠子) 手牌里不存在杠
	//if ( bMJ[i] == 4 )							// 如果当前牌数等于4张
	//{
	//	bMJ[i] = 0;								// 除开全部4张牌
	//	if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // 如果剩余的牌组合成功,和牌
	//	bMJ[i] = 4;								// 否则,取消4张组合
	//}

	// 3张组合(大对)
	if ( bMJ[i] >= 3 )							// 如果当前牌不少于3张
	{
		bMJ[i] -= 3;							// 减去3张牌
		if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // 如果剩余的牌组合成功,和牌
		bMJ[i] += 3;							// 取消3张组合
	}

	// 2张组合(将牌)
	if ( !bMJ[0] && bMJ[i] >= 2 )				// 如果之前没有将牌,且当前牌不少于2张
	{
		bMJ[0] = 1;								// 设置将牌标志
		bMJ[i] -= 2;							// 减去2张牌
		if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // 如果剩余的牌组合成功,和牌
		bMJ[i] += 2;							// 取消2张组合
		bMJ[0] = 0;								// 清除将牌标志
	}

	if ( i > 30 )    return 0;					// “东南西北中发白”没有顺牌组合,不和

	// 顺牌组合,注意是从前往后组合！
	if( i%10 != 8 && i%10 != 9  &&				// 排除数值为8和9的牌
		bMJ[i+1] && bMJ[i+2] )					// 如果后面有连续两张牌
	{
		bMJ[i]--;
		bMJ[i+1]--;
		bMJ[i+2]--;								// 各牌数减1
		if( CheckIsHu(bMJ, iMaxCount) ) return 1;      // 如果剩余的牌组合成功,和牌
		bMJ[i]++;
		bMJ[i+1]++;
		bMJ[i+2]++;								// 恢复各牌数
	}
	// 无法全部组合,不和！
	return 0;
}

//判定和辅助函数:检查牌是否还剩余牌，将各牌的个数保存在38大小的数组中
int CMahJongAlg::Remain(MJBYTE bMJ[], int iMaxCount)
{
	int sum = 0;
	for(int i=1; i<iMaxCount; i++)
	{
		sum += bMJ[i];
	}
	return sum;
}

//获得手牌所有可能的胡牌组合
bool CMahJongAlg::GetAllCombination(MJBYTE bMJ[], int iMaxCount, HuTree huTree)
{
	//static int JIANG = 0;						// 将牌标志,即牌型“三三三三二”中的“二” ,放到第0号位置
	if( !Remain(bMJ, iMaxCount) ) return true;     // 递归退出条件：如果没有剩牌,则和牌返回。
	int i=0;
	for(i=1; !bMJ[i] && i<iMaxCount; i++);  // 找到有牌的地方,i就是当前牌, PAI[i]是个数

	// 3张组合(刻子)
	if ( bMJ[i] >= 3 )							// 如果当前牌不少于3张
	{
		HuNode *huKe = new HuNode;
		huKe->mjBlock.first = MASK_MAHJONG(i/10, i%10);
		huKe->mjBlock.style = BlockStyle_Ke;
		huKe->tJiang = 0;
		huKe->tShun = 0;
		huKe->tKe = 0;
		huTree->tKe = huKe;

		bMJ[i] -= 3;							// 减去3张牌
		GetAllCombination(bMJ, iMaxCount, huTree->tKe);
		bMJ[i] += 3;							// 取消3张组合
	}

	// 2张组合(将牌)
	if ( !bMJ[0] && bMJ[i] >= 2 )				// 如果之前没有将牌,且当前牌不少于2张
	{
		HuNode *huJiang = new HuNode;
		huJiang->mjBlock.first = MASK_MAHJONG(i/10, i%10);
		huJiang->mjBlock.style = BlockStyle_Jiang;
		huJiang->tJiang = 0;
		huJiang->tShun = 0;
		huJiang->tKe = 0;
		huTree->tJiang = huJiang;

		bMJ[0] = 1;								// 设置将牌标志
		bMJ[i] -= 2;							// 减去2张牌
		GetAllCombination(bMJ, iMaxCount, huTree->tJiang);
		bMJ[i] += 2;							// 取消2张组合
		bMJ[0] = 0;								// 清除将牌标志
	}

	// 顺牌组合,注意是从前往后组合！
	if( i < 30 && i%10 != 8 && i%10 != 9  && bMJ[i+1] && bMJ[i+2] )		// 排除数值为8和9的牌 如果后面有连续两张牌
	{
		HuNode *huShun = new HuNode;
		huShun->mjBlock.first = MASK_MAHJONG(i/10, i%10);
		huShun->mjBlock.style = BlockStyle_Shun;
		huShun->tJiang = 0;
		huShun->tShun = 0;
		huShun->tKe = 0;
		huTree->tShun = huShun;

		// 各牌数减1
		bMJ[i]--;
		bMJ[i+1]--;
		bMJ[i+2]--;
		GetAllCombination(bMJ, iMaxCount, huTree->tShun);

		// 恢复各牌数
		bMJ[i]++;
		bMJ[i+1]++;
		bMJ[i+2]++;
	}
	//不能胡牌，可以不用往下展开了
	return false;
}

//从树中提取出胡牌结构，保存在数组中
void CMahJongAlg::ExtractHuBlock(HuNode *huTree, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount, MJBlock bTmpData[MAX_BLOCK_COUNT], int &iCombinaIndex, MJBYTE bMaxBlock)
{
	if (huTree->tJiang == 0 && huTree->tKe == 0 && huTree->tShun == 0)
	{
		//遍历完了，看数目对不对
		if (iCombinaIndex == bMaxBlock)
		{
			//完整的，是合法牌型,组合计数器+1
			memcpy(bData[iCombinaCount], bTmpData, sizeof(MJBlock) * (MAX_HAND_MJ / 3 +1));
			iCombinaCount++;
		}
		else
		{
			//不是完整的，组合失败,重新计数
		}
		//序数归0
		iCombinaIndex = 0;
		return;
	}
	else
	{
		//某一个方法可以成功
		if (huTree->tKe != 0)
		{
			MJBYTE bBackIndex = iCombinaIndex;
			bTmpData[iCombinaIndex].first = huTree->tKe->mjBlock.first;
			bTmpData[iCombinaIndex].style = huTree->tKe->mjBlock.style;
			iCombinaIndex++;

			ExtractHuBlock(huTree->tKe, bData, iCombinaCount, bTmpData, iCombinaIndex, bMaxBlock);

			//还原,换另种方式组合
			iCombinaIndex = bBackIndex;
		}
		if (huTree->tShun != 0)
		{
			MJBYTE bBackIndex = iCombinaIndex;
			bTmpData[iCombinaIndex].first = huTree->tShun->mjBlock.first;
			bTmpData[iCombinaIndex].style = huTree->tShun->mjBlock.style;
			iCombinaIndex++;

			ExtractHuBlock(huTree->tShun, bData, iCombinaCount, bTmpData, iCombinaIndex, bMaxBlock);

			//还原,换另种方式组合
			iCombinaIndex = bBackIndex;
		}
		if (huTree->tJiang != 0)
		{
			MJBYTE bBackIndex = iCombinaIndex;
			bTmpData[iCombinaIndex].first = huTree->tJiang->mjBlock.first;
			bTmpData[iCombinaIndex].style = huTree->tJiang->mjBlock.style;
			iCombinaIndex++;

			ExtractHuBlock(huTree->tJiang, bData, iCombinaCount, bTmpData, iCombinaIndex, bMaxBlock);

			//还原,换另种方式组合
			iCombinaIndex = bBackIndex;
		}
		return;
	}
}

void CMahJongAlg::ReleaseHuTree(HuNode *huTree)
{
	if (huTree == 0)
	{
		return;
	}
	if (huTree->tJiang != 0)
	{
		ReleaseHuTree(huTree->tJiang);
		huTree->tJiang = 0;
	}
	if (huTree->tKe != 0)
	{
		ReleaseHuTree(huTree->tKe);
		huTree->tKe = 0;
	}
	if (huTree->tShun != 0)
	{
		ReleaseHuTree(huTree->tShun);
		huTree->tShun = 0;
	}
	//删除
	try
	{
		delete huTree;
	}
	catch (...)
	{
	}
	huTree=0;
}


//获得块的类型
enBlockStyle CMahJongAlg::GetBlockStyle(MJBYTE bMJ1, MJBYTE bMJ2, MJBYTE bMJ3)
{
	//先排序
	MJBYTE bTemp = 0;
	if (bMJ1 > bMJ2 && bMJ1 > bMJ3)
	{
		//bmj1最大
		bTemp = bMJ1;
		bMJ1 = bMJ3;
		bMJ3 = bTemp;

		if (bMJ1 > bMJ2)
		{
			bTemp = bMJ1;
			bMJ1 = bMJ2;
			bMJ2 = bTemp;
		}
	}
	else if (bMJ1 > bMJ2 || bMJ1 > bMJ3)
	{
		//bmj1中间
		bTemp = bMJ1;
		bMJ1 = bMJ2;
		bMJ2 = bTemp;

		if (bMJ1 > bMJ3)
		{
			bTemp = bMJ1;
			bMJ1 = bMJ3;
			bMJ3 = bTemp;
		}
	}
	else
	{
		//bmj1最小
		if (bMJ2 > bMJ3)
		{
			bTemp = bMJ2;
			bMJ3 = bMJ2;
			bMJ2 = bTemp;
		}
	}

	if (bMJ1 == bMJ2 && bMJ2 == bMJ3)
	{
		return BlockStyle_Ke;
	}
	else if (bMJ1 == bMJ2 -1 && bMJ2 == bMJ3 -1)
	{
		return BlockStyle_Shun;
	}
	else
	{
		return BlockStyle_Null;
	}
}

//将得到(摸、别人打出)的那张牌与手牌合在一起，然后排序
void CMahJongAlg::CombineHandMJ(MJBYTE bHandMJ[], int iHandMJCount, MJBYTE bMJ, MJBYTE bCombineMJ[], int &iCombineMJCount)
{
	iCombineMJCount = iHandMJCount;
	memcpy(bCombineMJ, bHandMJ, sizeof(MJBYTE) * iCombineMJCount);
	bCombineMJ[iCombineMJCount++] = bMJ;
	SortHandMJ(bCombineMJ, iCombineMJCount);
}

//获得手牌所有可能的胡牌组合
void CMahJongAlg::GetHuCombination(MJBYTE bHandMJ[], int iHandMJCount, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount)
{
	MJBYTE bMJ[MAX_ARRAY_MJ] = {0};
	for (int i=0;i<iHandMJCount;i++)
	{
		bMJ[MASK_MAHJONGKIND(bHandMJ[i])*10 + MASK_MAHJONGNUM(bHandMJ[i])]++;
	}

	HuNode* root = new HuNode;
	root->mjBlock.first = 0;
	root->mjBlock.style = BlockStyle_Null;
	root->tJiang = 0;
	root->tKe = 0;
	root->tShun = 0;
	GetAllCombination(bMJ, MAX_ARRAY_MJ, root);

	HuNode* pNode = root;
	int iIndex = 0;
	MJBlock bTmpData[MAX_BLOCK_COUNT];
	ExtractHuBlock(pNode, bData, iCombinaCount, bTmpData, iIndex, (iHandMJCount+2)/3);
	pNode = 0;
	ReleaseHuTree(root);
}
