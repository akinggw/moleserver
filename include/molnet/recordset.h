#ifndef _RECORD_SET_H_INCLUDE
#define _RECORD_SET_H_INCLUDE

/**
* MolNet网络引擎
*
* 描述:用于存储通过SQL查询的结果
* 作者:akinggw
* 日期:2010.2.28
*/

#include "common.h"

#include <iostream>
#include <vector>

/**
 * 用于存储RecordSet中的数据类型
 */
typedef std::vector<std::string> Row;

/**
 * RecordSet 用于存储通过SQL语句查询的结果
 */
class RecordSet
{
public:
	/// 初始的构造函数
	RecordSet(void)
		throw();
	/// 析构函数
	~RecordSet(void)
		throw();

	/// 清除所有的记录
	void clear(void);
	/// 检测当前记录是否为空
	bool isEmpty(void) const;
	/// 得到当前记录有多少行
	unsigned int rows(void) const;
	/// 得到当前记录有多少列
	unsigned int cols(void) const;
	/// 设置当前记录的列表头
	void setColumnHeaders(const Row& headers);
	/// 添加一行新的数据
	void add(const Row& row);
	/// 得到指定行和指定列的数据
	const std::string& operator()(const unsigned int row,
		                          const unsigned int col) const;
	/// 得到指定行指定名称的数据
	const std::string& operator()(const unsigned int row,
		                          const std::string& name) const;
private:
	Row mHeaders;              /**< 用于存储当前记录的数据名称 */
	typedef std::vector<Row> Rows;
	Rows mRows;                /**< 用于存储读取的数据 */
};

/**
 * 用于保存一组结果集
 */
class RecordSetList
{
public:
	/// 初始的构造函数
	RecordSetList(void)
		throw();
	/// 析构函数
	~RecordSetList(void)
		throw();

	/// 清除所有的记录
	void clear(void);
	/// 检测当前记录是否为空
	bool isEmpty(void) const;
	/// 得到记录个数
	inline int Count(void) { return (int)mRecordSets.size(); }

	/// 添加一行新的数据
	void add(const RecordSet& rowset);
	/// 得到指定行和指定列的数据
	const RecordSet& operator()(const unsigned int row) const;

private:
	typedef std::vector<RecordSet> RecordSets;
	RecordSets mRecordSets;
};

#endif
