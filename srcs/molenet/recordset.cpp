#include "../../include/molnet/recordset.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "../../include/molnet/dalexcept.h"

/**
 * 初始的构造函数
 */
RecordSet::RecordSet(void)
	throw()
{

}

/**
 * 析构函数
 */
RecordSet::~RecordSet(void)
	throw()
{

}

/**
 * 清除所有的记录
 */
void RecordSet::clear(void)
{
	mHeaders.clear();
	mRows.clear();
}

/**
 * 检测当前记录是否为空
 *
 * @return 如果当前记录为空的话返回真，否则返回假
 */
bool RecordSet::isEmpty(void) const
{
	return (mRows.size() == 0);
}

/**
 * 得到当前记录有多少行
 *
 * @return 返回当前记录有多少行数据
 */
unsigned int RecordSet::rows(void) const
{
	return (unsigned int)mRows.size();
}

/**
 * 得到当前记录有多少列
 *
 * @return 返回当前记录有多少列数据
 */
unsigned int RecordSet::cols(void) const
{
	return (unsigned int)mHeaders.size();
}

/**
 * 设置当前记录的列表头
 *
 * @param headers 要设置的列表头
 */
void RecordSet::setColumnHeaders(const Row& headers)
{
	if(mHeaders.size() > 0)
	{
		throw AlreadySetException();
	}

	mHeaders = headers;
}

/**
 * 添加一行新的数据
 *
 * @param row 要添加的一行数据
 */
void RecordSet::add(const Row& row)
{
	const unsigned int nCols = (unsigned int)mHeaders.size();

	if(nCols == 0)
	{
		throw RsColumnHeadersNotSet();
	}

	if(row.size() != nCols)
	{
		std::ostringstream msg;
		msg << "当前行有" << (unsigned int)row.size() << "列;"
			<< "实际有" << nCols << "列" << std::ends;

		throw std::invalid_argument(msg.str());
	}

	mRows.push_back(row);
}

/**
* 得到指定行和指定列的数据
*
* @param row,col 要取得的数据的行号和列号
*
* @return 如果这个数据存在返回这个数据的字符串表示，否则抛出异常
*/
const std::string& RecordSet::operator()(const unsigned int row,
										 const unsigned int col) const
{
	if((row >= mRows.size() || (col >= mHeaders.size())))
	{
		std::ostringstream os;
		os << "(" << row << "," << col << ") 是超出了范围;"
			<< "最大行:" << (unsigned int)mRows.size()
			<< ",最大列:" << (unsigned int)mHeaders.size() << std::ends;

		throw std::out_of_range(os.str());
	}

	return mRows[row][col];
}

/**
 * 得到指定行指定名称的数据
 *
 * @param row 要取得的数据的行号
 * @param name 要取得的数据的名称
 *
 * @return 如果这个数据存在返回这个数据的字符串表示，否则抛出异常
 */
const std::string& RecordSet::operator()(const unsigned int row,
							  const std::string& name) const
{
	if(row >= mRows.size())
	{
		std::ostringstream os;
		os << "行号" << row << "超出范围;"
			<< "当前最大行:" << (unsigned int)mRows.size() << std::ends;

		throw std::out_of_range(os.str());
	}

	Row::const_iterator it = std::find(mHeaders.begin(),
		                               mHeaders.end(),
									   name);

	if(it == mHeaders.end())
	{
		std::ostringstream os;
		os << "名称为:" << name << "的数据不存在." << std::ends;

		throw std::invalid_argument(os.str());
	}

	// 找到这个数据的索引号
	const unsigned int nCols = (unsigned int)mHeaders.size();
	unsigned int i;
	for(i=0;i<nCols;++i)
	{
		if(mHeaders[i] == name)
			break;
	}

	return mRows[row][i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 初始的构造函数
 */
RecordSetList::RecordSetList(void)
	throw()
{

}

/**
 * 析构函数
 */
RecordSetList::~RecordSetList(void)
	throw()
{

}

/**
 * 清除所有的记录
 */
void RecordSetList::clear(void)
{
	mRecordSets.clear();
}

/**
 * 检测当前记录是否为空
 *
 * @return 如果当前记录为空的话返回真，否则返回假
 */
bool RecordSetList::isEmpty(void) const
{
	return ((int)mRecordSets.size() == 0);
}

/**
 * 添加一行新的数据
 *
 * @param row 要添加的一行数据
 */
void RecordSetList::add(const RecordSet& row)
{
	mRecordSets.push_back(row);
}

/**
* 得到指定行和指定列的数据
*
* @param row,col 要取得的数据的行号和列号
*
* @return 如果这个数据存在返回这个数据的字符串表示，否则抛出异常
*/
const RecordSet& RecordSetList::operator()(const unsigned int row) const
{
	if(row >= mRecordSets.size() || row < 0)
	{
		std::ostringstream os;
		os << "(" << row << ") 是超出了范围;";

		throw std::out_of_range(os.str());
	}

	return mRecordSets[row];
}
