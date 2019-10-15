#ifndef CROBOTMANAGER_H_INCLUDED
#define CROBOTMANAGER_H_INCLUDED

#include "../../include/molnet/Network.h"
#include "CPlayer.h"

#include <map>

class CRobotManager  : public Singleton<CRobotManager>
{
public:
    CRobotManager();
    ~CRobotManager();

    // 导入机器人
    void LoadGameRobot(void);
    // 机器人进入房间
    void RobotsEnterGameRoom(void);
    // 机器人离开
    void AddLeaveRobot(uint32 connId);
    // 更新机器人
    void UpdateRobot(void);

private:
	std::vector<uint32>             m_RobotUserList;            /**< 用于管理所有的机器人 */
	int                             m_CurRobotUserCount;        /**< 管理的机器人数量 */

	std::list<uint32>               m_LeaveRobotList;           /**< 离开的机器人 */
	Mutex                           m_LeaveRobotLock;
};

#define RobotManager CRobotManager::getSingleton()

#endif // CROBOTMANAGER_H_INCLUDED
