#include "../../include/molnet/MolNetMessage.h"
#include "../../include/molnet/MolMessageIn.h"

/**
 * 带参数的构造函数
 *
 * @param count 我们设置的消息列表要接收的消息个数
 */
NetMessage::NetMessage(int count)
: m_MaxCount(count)
{

}

/**
 * 析构函数
 */
NetMessage::~NetMessage()
{
	Clear();
}

/**
 * 清空消息列表
 */
void NetMessage::Clear(void)
{
	if(m_MesList.empty()) return;

	std::vector<MessageStru>::iterator iter = m_MesList.begin();
	for(;iter != m_MesList.end();++iter)
	{
		if((*iter).mes)
			delete (*iter).mes;
	}

	m_MesList.clear();
}

/**
 * 添加一个消息到消息列表中
 *
 * @param mes 要添加的消息
 */
void NetMessage::AddMessage(MessageStru mes)
{
	if(m_MaxCount <= 0 ||
		(int)m_MesList.size() > m_MaxCount)
		return;

	m_MesList.push_back(mes);
}

/**
 * 得到指定id的消息
 *
 * @param id 要得到消息的ID
 *
 * @return 如果消息为空返回NULL，否则返回这个消息
 */
MessageStru* NetMessage::GetMesById(int id)
{
	if(id < 0 || id >= (int)m_MesList.size())
		return NULL;

	return &m_MesList[id];
}

/**
 * 得到消息列表
 *
 * @return 返回当前的消息列表
 */
std::vector<MessageStru>& NetMessage::GetMessage(void)
{
	return m_MesList;
}

