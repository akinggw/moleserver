#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#include "cdefines.h"

//////////////////////////////////////////////////////////////////////////

//排序类型
#define ST_ORDER					0									//大小排序
#define ST_COUNT					1									//数目排序

//////////////////////////////////////////////////////////////////////////
//数目定义

#define MAX_COUNT					20									//最大数目
#define FULL_COUNT					54									//全牌数目
#define GOOD_CARD_COUTN				38									//好牌数目
#define BACK_COUNT					3									//底牌数目
#define NORMAL_COUNT				17									//常规数目

//////////////////////////////////////////////////////////////////////////

//数值掩码
#define	MASK_COLOR					0xF0								//花色掩码
#define	MASK_VALUE					0x0F								//数值掩码

//扑克类型
#define CT_ERROR					0									//错误类型
#define CT_SINGLE					1									//单牌类型
#define CT_DOUBLE					2									//对牌类型
#define CT_THREE					3									//三条类型
#define CT_SINGLE_LINE				4									//单连类型
#define CT_DOUBLE_LINE				5									//对连类型
#define CT_THREE_LINE				6									//三连类型
#define CT_THREE_LINE_TAKE_ONE		7									//三带一单
#define CT_THREE_LINE_TAKE_TWO		8									//三带一对
#define CT_FOUR_LINE_TAKE_ONE		9									//四带两单
#define CT_FOUR_LINE_TAKE_TWO		10									//四带两对
#define CT_BOMB_CARD				11									//炸弹类型
#define CT_MISSILE_CARD				12									//火箭类型

//////////////////////////////////////////////////////////////////////////

//分析结构
struct tagAnalyseResult
{
	uint8 							cbFourCount;						//四张数目
	uint8 							cbThreeCount;						//三张数目
	uint8 							cbDoubleCount;						//两张数目
	uint8							cbSignedCount;						//单张数目
	uint8							cbFourCardData[MAX_COUNT];			//四张扑克
	uint8							cbThreeCardData[MAX_COUNT];			//三张扑克
	uint8							cbDoubleCardData[MAX_COUNT];		//两张扑克
	uint8							cbSignedCardData[MAX_COUNT];		//单张扑克
};

//出牌结果
struct tagOutCardResult
{
	uint8							cbCardCount;						//扑克数目
	uint8							cbResultCard[MAX_COUNT];			//结果扑克
};

#define MAX_TYPE_COUNT 254
struct tagOutCardTypeResult
{
	uint8							cbCardType;							//扑克类型
	uint8							cbCardTypeCount;					//牌型数目
	uint8							cbEachHandCardCount[MAX_TYPE_COUNT];//每手个数
	uint8							cbCardData[MAX_TYPE_COUNT][MAX_COUNT];//扑克数据
};


//扑克信息
struct tagHandCardInfo {
	uint8						cbHandCardData[ MAX_COUNT ];				//扑克数据
	uint8						cbHandCardCount;							//扑克数目
	tagOutCardTypeResult		CardTypeResult[ 12 + 1 ] ;					//分析数据

	//初始数据
	tagHandCardInfo( void ) {
		memset( cbHandCardData, 0,sizeof( cbHandCardData ) ) ;
		cbHandCardCount = 0;
		memset( &CardTypeResult,0, sizeof( CardTypeResult ) );
	}
};

//类型定义
typedef std::vector<tagHandCardInfo * >	    tagHandCardInfoArray;

//栈结构
class tagStackHandCardInfo
{

	//内联函数
public:

	//构造函数
	tagStackHandCardInfo( void )
	{
		m_HandCardInfoFreeArray.clear();
		m_HandCardInfoArray.clear();
	}

	//析构函数
	~tagStackHandCardInfo( void )
	{
		//清空栈
		ClearAll();
	}

	//元素压栈
	void Push( tagHandCardInfo * pHandCardInfo )
	{

		//是否还有空间
		if ( 0 < m_HandCardInfoFreeArray.size() )
		{
			//获取空间
			tagHandCardInfo * pHandCardInfoFree = *(m_HandCardInfoFreeArray.begin());
			m_HandCardInfoFreeArray.erase(m_HandCardInfoFreeArray.begin());


			//元素赋值
			memcpy( pHandCardInfoFree->cbHandCardData, pHandCardInfo->cbHandCardData, sizeof( pHandCardInfo->cbHandCardData ) );
			pHandCardInfoFree->cbHandCardCount = pHandCardInfo->cbHandCardCount;
			memcpy( pHandCardInfoFree->CardTypeResult, pHandCardInfo->CardTypeResult, sizeof( pHandCardInfo->CardTypeResult ) );

			//压入栈顶
			m_HandCardInfoArray.push_back( pHandCardInfoFree);

		}
		else
		{
			//申请空间
			tagHandCardInfo * pNewHandCardInfo = new tagHandCardInfo ;

			//元素赋值
			memcpy( pNewHandCardInfo->cbHandCardData, pHandCardInfo->cbHandCardData, sizeof( pHandCardInfo->cbHandCardData ) );
			pNewHandCardInfo->cbHandCardCount = pHandCardInfo->cbHandCardCount;
			memcpy( pNewHandCardInfo->CardTypeResult, pHandCardInfo->CardTypeResult, sizeof( pHandCardInfo->CardTypeResult ) );

			//压入栈顶
			m_HandCardInfoArray.push_back( pNewHandCardInfo );

		}

	}

	//弹出栈顶
	void Pop()
	{
		//非空判断
		if (empty()) return ;

		//获取元素
		tagHandCardInfo * pTopHandCardInfo = *(m_HandCardInfoArray.end()-1);

		//移除元素
		//m_HandCardInfoArray.erase(m_HandCardInfoArray.end()-1);
		m_HandCardInfoArray.pop_back();

		//保存空间
		m_HandCardInfoFreeArray.push_back(pTopHandCardInfo);

	}

	//初始栈
	void InitStack()
	{

		//保存空间
		while ( 0 < m_HandCardInfoArray.size() )
		{
			tagHandCardInfo *pHandCardInfo = m_HandCardInfoArray[ 0 ];
			m_HandCardInfoArray.erase( m_HandCardInfoArray.begin() );
			m_HandCardInfoFreeArray.push_back(pHandCardInfo);
		}
	}

	//清空栈
	void ClearAll()
	{

		//释放内存
		tagHandCardInfoArray::iterator iter1 = m_HandCardInfoArray.begin();
		for(;iter1 != m_HandCardInfoArray.end();++iter1)
		{
			if((*iter1)) SafeDelete((*iter1));
		}
		m_HandCardInfoArray.clear();

		//释放内存
		tagHandCardInfoArray::iterator iter2 = m_HandCardInfoFreeArray.begin();
		for(;iter2 != m_HandCardInfoFreeArray.end();++iter2)
		{
			if((*iter2)) SafeDelete((*iter2));
		}
		m_HandCardInfoFreeArray.clear();
	}

	//获取栈顶
	void GetTop( tagHandCardInfo * & pHandCardInfo )
	{
		//非空判断
		if (empty() )
		{
			return;
		}

		//获取元素
		tagHandCardInfoArray::iterator iter2 = m_HandCardInfoArray.end();
		pHandCardInfo=(*(iter2-1));
	}

	//空判断
	bool empty()
	{
		return m_HandCardInfoArray.empty();
	}

	//成员变量
private:
	tagHandCardInfoArray			m_HandCardInfoFreeArray;					//扑克信息
	tagHandCardInfoArray			m_HandCardInfoArray;						//扑克信息

};


//////////////////////////////////////////////////////////////////////////

//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const uint8				m_cbCardData[FULL_COUNT];			//扑克数据
	static const uint8				m_cbGoodcardData[GOOD_CARD_COUTN];	//好牌数据

	//AI变量
public:
	uint8							m_cbAllCardData[GAME_PLAYER][MAX_COUNT];//所有扑克
	uint8							m_cbLandScoreCardData[MAX_COUNT];	//叫牌扑克
	uint8							m_cbUserCardCount[GAME_PLAYER];		//扑克数目
	uint16							m_wBankerUser;						//地主玩家
	uint32							m_lBankerOutCardCount ;				//出牌次数

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取类型
	uint8 GetCardType(const uint8 cbCardData[], uint8 cbCardCount);
	//获取数值
	uint8 GetCardValue(uint8 cbCardData) { return cbCardData&MASK_VALUE; }
	//获取花色
	uint8 GetCardColor(uint8 cbCardData) { return cbCardData&MASK_COLOR; }
	//uint8类型转uint16类型
	uint16 ChangeCardDate( uint8 cbCardData);
	//uint16类型转uint8类型
	uint8 OnChangeCardDate(uint16 bCardDate);

	//控制函数
public:
	//混乱扑克
	void RandCardList(uint8 cbCardBuffer[], uint8 cbBufferCount);
	//得到好牌
	void GetGoodCardData(uint8 cbGoodCardData[NORMAL_COUNT]) ;
	//删除好牌
	bool RemoveGoodCardData(uint8 cbGoodcardData[NORMAL_COUNT], uint8 cbGoodCardCount, uint8 cbCardData[FULL_COUNT], uint8 cbCardCount) ;
	//排列扑克
	void SortCardList(uint8 cbCardData[], uint8 cbCardCount, uint8 cbSortType);
	//删除扑克
	bool RemoveCard(const uint8 cbRemoveCard[], uint8 cbRemoveCount, uint8 cbCardData[], uint8 cbCardCount);
	//随机扑克
	uint8 GetRandomCard(void) ;
	//获取好牌
	void GetGoodCard(uint8 cbALLCardData[FULL_COUNT]);

	//逻辑函数
public:
	//有效判断
	bool IsValidCard(uint8 cbCardData);
	//逻辑数值
	uint8 GetCardLogicValue(uint8 cbCardData);
	//对比扑克
	bool CompareCard(const uint8 cbFirstCard[], const uint8 cbNextCard[], uint8 cbFirstCount, uint8 cbNextCount);

	//内部函数
public:
	//分析扑克
	bool AnalysebCardData(const uint8 cbCardData[], uint8 cbCardCount, tagAnalyseResult & AnalyseResult);



	//////////////////////////////////////////////////////////////////////////
	//AI函数

	//设置函数
public:
	//设置扑克
	void SetUserCard(uint16 wChairID, uint8 cbCardData[], uint8 cbCardCount) ;
	//设置底牌
	void SetBackCard(uint16 wChairID, uint8 cbBackCardData[], uint8 cbCardCount) ;
	//设置庄家
	void SetBanker(uint16 wBanker) ;
	//叫牌扑克
	void SetLandScoreCardData(uint8 cbCardData[], uint8 cbCardCount) ;
	//删除扑克
	void RemoveUserCardData(uint16 wChairID, uint8 cbRemoveCardData[], uint8 cbRemoveCardCount) ;

	//辅助函数
public:
	//组合算法
	void Combination(uint8 cbCombineCardData[], uint8 cbResComLen,  uint8 cbResultCardData[254][5], uint8 &cbResCardLen,uint8 cbSrcCardData[] , uint8 cbCombineLen1, uint8 cbSrcLen, const uint8 cbCombineLen2);
	//排列算法
	void Permutation(uint8 *list, int m, int n, uint8 result[][4], uint8 &len) ;
	//分析炸弹
	void GetAllBomCard(uint8 const cbHandCardData[], uint8 const cbHandCardCount, uint8 cbBomCardData[], uint8 &cbBomCardCount);
	//分析顺子
	void GetAllLineCard(uint8 const cbHandCardData[], uint8 const cbHandCardCount, uint8 cbLineCardData[], uint8 &cbLineCardCount);
	//分析三条
	void GetAllThreeCard(uint8 const cbHandCardData[], uint8 const cbHandCardCount, uint8 cbThreeCardData[], uint8 &cbThreeCardCount);
	//分析对子
	void GetAllDoubleCard(uint8 const cbHandCardData[], uint8 const cbHandCardCount, uint8 cbDoubleCardData[], uint8 &cbDoubleCardCount);
	//分析单牌
	void GetAllSingleCard(uint8 const cbHandCardData[], uint8 const cbHandCardCount, uint8 cbSingleCardData[], uint8 &cbSingleCardCount);
	//出牌测试
	bool _TestOutAllCard(uint16 wTestUser, uint8 cbWantOutCardData[], uint8 cbWantOutCardCount, uint8	cbAllCardData[GAME_PLAYER][MAX_COUNT], uint8 cbUserCardCount[GAME_PLAYER], bool bFirstOutCard) ;
	//出牌测试
	bool TestOutAllCard(uint16 wTestUser, uint8 cbWantOutCardData[], uint8 cbWantOutCardCount, bool bFirstOutCard) ;
	//四带牌型
	bool AnalyseFourCardType( uint8 const cbHandCardData[MAX_COUNT], uint8 cbHandCardCount, uint8 cbEnemyCardData[MAX_COUNT], uint8 cbEnemyCardCount, tagOutCardResult &CardResult ) ;
	//最大判断
	bool IsLargestCard( uint16 wTestUser, uint8 const cbWantOutCardData[], uint8 const cbWantOutCardCount );
	//是否能出
	bool VerifyOutCard( uint16 wTestUser, uint8 const cbWantOutCardData[], uint8 const cbWantOutCardCount, uint8 const cbCurHandCardData[ MAX_COUNT ], uint8 const cbCurHandCardCount, bool bFirstOutCard ) ;

	//主要函数
protected:
	//分析牌型（后出牌调用）
	void AnalyseOutCardType(uint8 const cbHandCardData[], uint8 const cbHandCardCount, uint8 const cbTurnCardData[], uint8 const cbTurnCardCount, tagOutCardTypeResult CardTypeResult[12+1]);
	//分析牌牌（先出牌调用）
	void AnalyseOutCardType(uint8 const cbHandCardData[], uint8 const cbHandCardCount, tagOutCardTypeResult CardTypeResult[12+1]);
	//单牌个数
	uint8 AnalyseSinleCardCount(uint8 const cbHandCardData[], uint8 const cbHandCardCount, uint8 const cbWantOutCardData[], uint8 const cbWantOutCardCount, uint8 cbSingleCardData[]=NULL);

	//出牌函数
public:
	//地主出牌（先出牌）
	void BankerOutCard(const uint8 cbHandCardData[], uint8 cbHandCardCount, tagOutCardResult & OutCardResult) ;
	//地主出牌（后出牌）
	void BankerOutCard(const uint8 cbHandCardData[], uint8 cbHandCardCount, uint16 wOutCardUser, const uint8 cbTurnCardData[], uint8 cbTurnCardCount, tagOutCardResult & OutCardResult) ;
	//地主上家（先出牌）
	void UpsideOfBankerOutCard(const uint8 cbHandCardData[], uint8 cbHandCardCount, uint16 wMeChairID,tagOutCardResult & OutCardResult) ;
	//地主上家（后出牌）
	void UpsideOfBankerOutCard(const uint8 cbHandCardData[], uint8 cbHandCardCount, uint16 wOutCardUser,  const uint8 cbTurnCardData[], uint8 cbTurnCardCount, tagOutCardResult & OutCardResult) ;
	//地主下家（先出牌）
	void UndersideOfBankerOutCard(const uint8 cbHandCardData[], uint8 cbHandCardCount, uint16 wMeChairID,tagOutCardResult & OutCardResult) ;
	//地主下家（后出牌）
	void UndersideOfBankerOutCard(const uint8 cbHandCardData[], uint8 cbHandCardCount, uint16 wOutCardUser, const uint8 cbTurnCardData[], uint8 cbTurnCardCount, tagOutCardResult & OutCardResult) ;
	//出牌搜索
	bool SearchOutCard(const uint8 cbHandCardData[], uint8 cbHandCardCount, const uint8 cbTurnCardData[], uint8 cbTurnCardCount, uint16 wOutCardUser, uint16 wMeChairID, tagOutCardResult & OutCardResult);


	//叫分函数
public:
	//叫分判断
	uint16 LandScore(uint16 wMeChairID, uint8 cbCurrentLandScore, uint8 cbCallScorePhase) ;

	//////////////////////////////////////////////////////////////////////////
};

#endif
