#ifndef MAHJONG_ALG_HEAD_FILE
#define MAHJONG_ALG_HEAD_FILE

#include "MahJongStruct.h"

//! 麻将花色
/*!
	麻将花色,用于指定整副牌所有的花色
*/
enum enMahJongColor
{
	MahJongColor_Invalid	= 0x00,											/*!< 无效值 */
	MahJongColor_Wan		= 0x01,											/*!< 万子 */
	MahJongColor_Tiao		= 0x02,											/*!< 条子 */
	MahJongColor_Bing		= 0x04,											/*!< 筒子 */
	MahJongColor_Word		= 0x08,											/*!< 字牌 */
	MahJongColor_Flower		= 0x10											/*!< 花牌 */
};


//! 麻将通用算法
/*! 麻将通用算法集合，具体的麻将算法只需继承该类即可。
	\n 如血战麻将的算法类：CXZMahJongAlg
	\n class CXZMahJongAlg : public CMahJongAlg
	\n {
	\n 	//额外的变量...
	\n
	\n 	//扩展的方法...
	\n }
	\n 在使用时，即可使用基类的通用方法和已扩展的方法。
	\n CXZMahJongAlg* xzAlg = new CXZMahJongAlg(MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing);
	\n xzAlg->基类的方法();
	\n xzAlg->扩展的方法();
	\note初始化时需要设置麻将花色
	\sa CMahJongLogic
*/
class CMahJongAlg
{
private:
	MJBYTE				m_bMahJongColor;									/*!< 整副麻将所包含的颜色(万条饼字花等) */

public:
	//! 构造函数
	/*!
		设置随机数种子,设置默认的麻将颜色(万条饼字花)
		\param bMahJongColor 麻将颜色,每一位代表一个颜色,由enMahJongColor定义.默认颜色:万条饼字花
	*/
	CMahJongAlg(MJBYTE bMahJongColor = (MahJongColor_Wan|MahJongColor_Tiao|MahJongColor_Bing|MahJongColor_Word|MahJongColor_Flower)){ srand((uint32)time(NULL)); m_bMahJongColor = bMahJongColor; }

	//! 设置麻将颜色
	/*!
		设置麻将颜色,用MJBYTE表示,每一位代表一个颜色
		\param bMahJongColor 麻将颜色,每一位代表一个颜色,由enMahJongColor定义
		\sa enMahJongColor, GetMahJongColor()
	*/
	inline void SetMahJongColor(MJBYTE bMahJongColor){ m_bMahJongColor = bMahJongColor; }

	//! 获取麻将颜色
	/*!
		获取麻将颜色,用MJBYTE表示,每一位代表一个颜色
		\return 麻将颜色,每一位代表一个颜色,由enMahJongColor定义
		\sa enMahJongColor, SetMahJongColor()
	*/
	inline MJBYTE GetMahJongColor(){ return m_bMahJongColor; }

	//辅助
public:
	//! 获取随机数
	/*!
		获取指定范围的随机数
		\param iMin 整数,随机范围左边界
		\param iMax 整数,随机范围右边界
		\return iMin到iMax之间的随机数
	*/
	virtual inline int GetRandomNum(int iMin, int iMax)
	{
		srand((uint32)time(NULL));
		return rand() % (iMax + 1 - iMin) + iMin;
	}

	//! 打乱排序
	/*!
		将传入的数组打乱顺序,打乱的次数在RANDOM_TIMES定义
		\param bAllMJ 牌数组
		\param bAllMJCount 牌数组大小
		\sa RANDOM_TIMES
	*/
	virtual void RandomMJ(MJBYTE bAllMJ[], int bAllMJCount);

	//! 将牌数据排序
	/*!
		万饼条按从小到大排序
		\param bAllMJ 牌数组
		\param bAllMJCount 牌数组大小
	*/
	virtual void SortHandMJ(MJBYTE bAllMJ[], int bAllMJCount);

	//! 校验牌是否存在
	/*!
		校验牌在指定牌数组中是否存在
		\param bAllMJ 牌数组
		\param bAllMJCount 牌数组大小
		\param bMJ 指定麻将牌
		\return true-存在;false-不存在
	*/
	virtual bool CheckMJExist(const MJBYTE bAllMJ[], int bAllMJCount, MJBYTE bMJ);

	//! 删除指定麻将
	/*!
		从牌数组中删除指定麻将,如果牌时隐藏的(牌数据为0),牌数量减1
		\param bAllMJ 牌数组
		\param bAllMJCount 牌数组大小
		\param bMJ 指定麻将牌
		\return true-删除成功;false-删除失败
	*/
	virtual bool DeleteMJ(MJBYTE bAllMJ[], int &bAllMJCount, MJBYTE bMJ);

	//! 获得指定麻将的数量
	/*!
		\param bMJ 牌数组
		\param bMJCount 牌数组大小
		\param bM 指定麻将牌
		\return 指定麻将在麻将数组中的数量
	*/
	virtual int GetMJCount(MJBYTE bMJ[], int &bMJCount, MJBYTE bM);

	//////////////////////////////////////////////////////////////////////////

	//胡牌牌型判定
public:
	//! 一般胡牌判定
	/*!
		判定是否能一般胡牌，即3*N+2张牌是否能凑成 1对将牌N个麻将块的组合
		\param bSelfHUMJ 胡牌区数组
		\param bSelfHUMJCount 胡牌区数组大小
		\param bSelfHandMJ 手牌区数组
		\param bSelfHandMJCount 手牌区数组大小
		\return true-能胡;FLASE-不能胡
		\note 判定一般的胡牌使用这个方法即可
	*/
	virtual bool IsNormalHu(MJBlock bSelfHUMJ[], int bSelfHUMJCount, MJBYTE bSelfHandMJ[], int bSelfHandMJCount);

	//辅助
protected:
	//! 一般胡牌判定
	/*!
		检测剩余的手牌是否能胡，以数组表示
		\param bHandMJ 手牌区数组
		\param bHandMJCount 手牌区数组大小
		\return true-能胡;FLASE-不能胡
		\note 判定一般的胡牌使用 IsNormalHu() 即可
		\sa IsNormalHu(), CheckIsHu()
	*/
	virtual bool CheckRestHu(MJBYTE bHandMJ[], int bHandMJCount);

	//! 一般胡牌判定
	/*!
		检查牌是否能胡，将各牌的个数保存在MAX_ARRAY_MJ大小的数组中
		\param bMJ 麻将牌数组
		\param bMaxCount 麻将数组最大大小
		\return true-能胡;FLASE-不能胡
		\note 判定一般的胡牌使用 IsNormalHu() 即可
		\sa CheckRestHu(), MAX_ARRAY_MJ
	*/
	virtual bool CheckIsHu(MJBYTE bMJ[], int bMaxCount);

	//! 胡牌组合过程中是否还有剩余牌
	/*!
		检查牌是否还剩余牌，将各牌的个数保存在MAX_ARRAY_MJ大小的数组中
		\param bMJ 麻将牌数组
		\param bMaxCount 麻将数组最大大小
		\return 余下牌的个数
		\note 判定一般的胡牌使用 IsNormalHu() 即可
		\sa MAX_ARRAY_MJ
	*/
	virtual int Remain(MJBYTE bMJ[], int bMaxCount);

	//! 获取胡牌组合
	/*!
		获得手牌所有可能的胡牌组合,如：333 444 555 123 77也可以为345 345 345 123 7738
		\param bMJ 麻将牌数组
		\param bMaxCount 麻将数组最大大小
		\param huTree 麻将组合树
		\return 是否能凑成组合
		\note 获取胡牌组合使用 GetHuCombination() 即可
	*/
	virtual bool GetAllCombination(MJBYTE bMJ[], int bMaxCount, HuTree huTree);

	//! 从树中提取出胡牌结构
	/*!
		从树中提取出胡牌结构，保存在数组中(树，胡牌块数组，临时块(用于递归暂存上一步结果)，组合数目，组合中块序数，组合中最大块)
		\param huTree 麻将组合树
		\param bData 麻将组合数组
		\param iCombinaCount 麻将组合数组大小
		\param bTmpData 临时块,用于递归暂存上一步结果
		\param iCombinaIndex 当前麻将组合中麻将块的序数,如果和bMaxBlock相等则这一组合完成
		\param bMaxBlock 麻将组合中麻将块最大大小
		\return 是否能凑成至少1个组合
		\note 获取胡牌组合使用 GetHuCombination() 即可
	*/
	virtual void ExtractHuBlock(HuNode *huTree, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount, MJBlock bTmpData[MAX_BLOCK_COUNT], int &iCombinaIndex, MJBYTE bMaxBlock);

	//! 释放树的内存
	/*!
		释放树的内存释放胡牌树的内存
		\param huTree 麻将组合树的指针
	*/
	virtual void ReleaseHuTree(HuNode *huTree);

	//辅助
public:
	//! 获得块的类型
	/*!
		获得给定3张牌的类型
		\param bMJ1 第一张牌数据
		\param bMJ2 第二张牌数据
		\param bMJ3 第三张牌数据
		\return 返回牌类型enBlockStyle
		\sa enBlockStyle
	*/
	virtual enBlockStyle GetBlockStyle(MJBYTE bMJ1, MJBYTE bMJ2, MJBYTE bMJ3);

	//! 将牌加到手牌中
	/*!
		将得到(摸、别人打出)的那张牌与手牌合在一起，然后排序,保存到bCombineMJ中
		\param bHandMJ 手牌数组
		\param iHandMJCount 手牌数组大小
		\param bMJ 第三张牌数据
		\param bCombineMJ 结果牌数组
		\param iCombineMJCount 结果牌数组大小
	*/
	virtual void CombineHandMJ(MJBYTE bHandMJ[], int iHandMJCount, MJBYTE bMJ, MJBYTE bCombineMJ[], int &iCombineMJCount);

	//! 获得手牌所有可能的胡牌组合
	/*!
		获得手牌所有可能的胡牌组合,(手牌,手牌个数，组合块(如果是空则结束)，组合个数)
		\param bHandMJ 手牌数组
		\param iHandMJCount 手牌数组大小
		\param bData 组合结构数组
		\param iCombinaCount 组合结构数组大小
	*/
	virtual void GetHuCombination(MJBYTE bHandMJ[], int iHandMJCount, MJBlock bData[][MAX_BLOCK_COUNT], int &iCombinaCount);
};

#endif
