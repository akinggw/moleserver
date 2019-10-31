#include "GameFrameManager.h"
#include "../include/Common/defines.h"
#include "DBOperator.h"

#include <map>
#include <sstream>

bool cmpList(const GameDataInfo& x,const GameDataInfo& y) { return x.showindex > y.showindex; }

initialiseSingleton(GameFrameManager);

/**
 * 构造函数
 */
GameFrameManager::GameFrameManager()
{
}

/**
 * 析构函数
 */
GameFrameManager::~GameFrameManager()
{
}

/**
 * 用于处理接收到的网络消息
 *
 * @param connId 要处理网络消息的客户端ID
 * @param mes 要处理的网络消息
 */
void GameFrameManager::OnProcessNetMes(uint32 connId,CMolMessageIn *mes)
{
	if(mes == NULL) return;

    Json::Reader reader;
    Json::Value json_object;

    if (!reader.parse(mes->getData(), json_object))
    {
        LOG_ERROR("数据解析失败:%s",mes->getData());
        return;
    }

	switch( json_object["MsgId"].asInt())
	{
	case IDD_MESSAGE_CENTER_LOGIN:                     // 登录消息
		{
			OnProcessUserLoginMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_CENTER_UPDATEUSER:                // 更新用户信息
		{
			OnProcessUserInfoUpdateMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_CENTER_BANK:                      // 钱包操作
		{
			OnProcessUserBankMes(connId,json_object);
		}
		break;
	case IDD_MESSAGE_USER_REGISTER:                    // 用户注册消息
		{
			OnProcessUserRegisterMes(connId,json_object);
		}
		break;
    case IDD_MESSAGE_GET_GAMESERVER:                   // 得到在线房间信息
        {
            OnProcessUserGetOnlineRoomMes(connId);
        }
        break;
	case IDD_MESSAGE_GET_GAMEINFO:                     // 得到所有的游戏信息
		{
			OnProcessGetGamesMes(connId,json_object);
		}
		break;
	default:
		break;
	}
}

/**
 * 用于处理接收网络连接消息
 *
 * @param connId 要处理网络消息的客户端ID
 */
void GameFrameManager::OnProcessConnectedNetMes(uint32 connId)
{
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_CONNECT;
    root["MsgSubId"] = IDD_MESSAGE_CONNECT_SUCESS;

    Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
}

/**
 * 用于处理用于断开网络连接消息
 *
 * @param connId 要处理网络消息的客户端ID
 */
void GameFrameManager::OnProcessDisconnectNetMes(uint32 connId)
{

}

/// 处理钱包消息
void GameFrameManager::OnProcessUserBankMes(uint32 connId,Json::Value &mes)
{
	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_CENTER_BANK_TRANSFERACCOUNT:              // ¸玩家之间转账
		{
			uint32 pUserID = mes["UserID"].asInt();
			int64 pMoney = mes["Money"].asInt();
			std::string pReceiver = mes["Receiver"].asString();

			int32 pReceiverID = ServerDBOperator.TransferAccounts(pUserID,pReceiver,pMoney);
			if(pReceiverID <= 0)
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_CENTER_BANK;
                root["MsgSubId"] = IDD_MESSAGE_CENTER_BANK_FAIL;

				if(pReceiverID == -1)
					root["MsgSubId2"] = IDD_MESSAGE_CENTER_BANK_FAIL_LASTMONEY;

				Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			int64 pUserMoney,pUserBankMoney;
			pUserMoney = pUserBankMoney = 0;

			if(!ServerDBOperator.GetUserMoney(pUserID,&pUserMoney,&pUserBankMoney))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_CENTER_BANK;
                root["MsgSubId"] = IDD_MESSAGE_CENTER_BANK_FAIL;

                Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
                return;
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_CENTER_BANK;
            root["MsgSubId"] = IDD_MESSAGE_CENTER_BANK_TRANSFERACCOUNT;
            root["ReceiverID"] = pReceiverID;
            root["UserMoney"] = (uint32)pUserMoney;
            root["UserBankMoney"] = (uint32)pUserBankMoney;

            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
		break;
    default:
        break;
	}
}


/**
 * 处理用户注册消息
 *
 * @param connId 要处理网络消息的客户端ID
 * @param mes 要处理的网络消息
 */
void GameFrameManager::OnProcessUserRegisterMes(uint32 connId,Json::Value &mes)
{
	std::string pUserName = mes["UserName"].asString();
	std::string pUserPW = mes["UserPW"].asString();
	std::string pEmail = mes["Email"].asString();
	int pSex = mes["Sex"].asInt();
	std::string pRealName = mes["RealName"].asString();
	std::string pTelephone = mes["Telephone"].asString();
	std::string pAvatorIndex = mes["AvatorIndex"].asString();
	std::string pReferrer = mes["Referrer"].asString();
	std::string pcardnumber = mes["cardnumber"].asString();

	// 如果用户名和密码太长，是不能注册的
	if(pUserName.length() >= 20 || pUserPW.length() >= 50 || pReferrer.length() >= 20 ||
		pUserName.empty() || pUserPW.empty())
	{
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_USER_REGISTER;
		root["MsgSubId"] = IDD_MESSAGE_USER_REGISTER_FAIL;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	unsigned int pUserId = ServerDBOperator.IsExistUser(pUserName.c_str(),pUserPW.c_str());

	if(pUserId > 0)
	{
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_USER_REGISTER;
		root["MsgSubId"] = IDD_MESSAGE_USER_REGISTER_FAIL;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		return;
	}

	bool isSucc = ServerDBOperator.RegisterGameUser(pUserName.c_str(),
													pUserPW.c_str(),
													pEmail.c_str(),
													pSex,
													pRealName.c_str(),
													pTelephone.c_str(),
													pAvatorIndex,
													pReferrer.c_str(),
													GetIpAddress(connId),
													pcardnumber.c_str()) > 0 ? true : false;

	if(isSucc)
	{
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_USER_REGISTER;
		root["MsgSubId"] = IDD_MESSAGE_USER_REGISTER_SUCCESS;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
	}
	else
	{
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_USER_REGISTER;
		root["MsgSubId"] = IDD_MESSAGE_USER_REGISTER_FAIL;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
	}
}

/**
 * 处理用户信息更改消息
 *
 * @param connId 要处理网络消息的客户端ID
 * @param mes 要处理的网络消息
 */
void GameFrameManager::OnProcessUserInfoUpdateMes(uint32 connId,Json::Value &mes)
{
	switch(mes["MsgSubId"].asInt())
	{
	case IDD_MESSAGE_CENTER_UPDATEUSER_INFO:              // ¸更新用户信息
		{
			uint32 pUserID = mes["UserID"].asInt();
			std::string pNickName = mes["NickName"].asString();
			std::string pEmail = mes["Email"].asString();
			std::string pTelephone = mes["Telephone"].asString();
			std::string pQQ = mes["QQ"].asString();
			std::string pUserAvatar = mes["UserAvatar"].asString();
			int pSex = mes["Sex"].asInt();

			if(!ServerDBOperator.UpdateUserInfo(pUserID,pNickName,pEmail,pTelephone,pQQ,pUserAvatar,pSex))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_CENTER_UPDATEUSER;
                root["MsgSubId"] = IDD_MESSAGE_CENTER_UPDATEUSER_FAIL;

                Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_CENTER_UPDATEUSER;
            root["MsgSubId"] = IDD_MESSAGE_CENTER_UPDATEUSER_INFO;
            root["NickName"] = pNickName;
            root["Email"] = pEmail;
            root["Telephone"] = pTelephone;
            root["QQ"] = pQQ;
            root["UserAvatar"] = pUserAvatar;
            root["Sex"] = pSex;

            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
		break;
	case IDD_MESSAGE_CENTER_UPDATEUSER_PWD:               // 更新用户密码
		{
			uint32 pUserID = mes["UserID"].asInt();
			std::string pLoginPWD = mes["LoginPWD"].asString();

			if(!ServerDBOperator.UpdateUserLoginPassword(pUserID,pLoginPWD))
			{
                Json::Value root;
                root["MsgId"] = IDD_MESSAGE_CENTER_UPDATEUSER;
                root["MsgSubId"] = IDD_MESSAGE_CENTER_UPDATEUSER_FAIL;

                Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
				return;
			}

			CMolMessageOut out(IDD_MESSAGE_CENTER_UPDATEUSER);
			out.write16(IDD_MESSAGE_CENTER_UPDATEUSER_PWD);
			out.writeString(pLoginPWD);
			Send(connId,out);

            Json::Value root;
            root["MsgId"] = IDD_MESSAGE_CENTER_UPDATEUSER;
            root["MsgSubId"] = IDD_MESSAGE_CENTER_UPDATEUSER_PWD;
            root["LoginPWD"] = pLoginPWD;

            Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
		}
		break;
	default:
		break;
	}
}

/**
 * 处理用户登录系统消息
 *
 * @param connId 要处理网络消息的客户端ID
 * @param mes 要处理的网络消息
 */
void GameFrameManager::OnProcessUserLoginMes(uint32 connId,Json::Value &mes)
{
	std::string pUserName = mes["username"].asString();
	std::string pUserPW = mes["userpwd"].asString();
	std::string pMachineCode = mes["machinecode"].asString();

    //查看服务器是否禁止进入
    tagServerSet ptagServerSet;
    if(ServerDBOperator.GetAccountServerConfig(m_accountserverid,ptagServerSet) && ptagServerSet.serverstate == 1)
    {
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_CENTER_LOGIN;
		root["MsgSubId"] = IDD_MESSAGE_CENTER_LOGIN_BANLOGIN;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

        return;
    }

	uint32 pUserId = ServerDBOperator.IsExistUser(pUserName.c_str(),pUserPW.c_str());

	// 得到用户数据
	MemberDataStru pUserData;
	if(!ServerDBOperator.GetUserData(pUserId,pUserData))
	{
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_CENTER_LOGIN;
		root["MsgSubId"] = IDD_MESSAGE_CENTER_LOGIN_FAIL;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

		return;
	}

	// 检测用户是否已经被封号
	if(pUserData.state != 1)
	{
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_CENTER_LOGIN;
		root["MsgSubId"] = IDD_MESSAGE_CENTER_LOGIN_BAN;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

		return;
	}

	// 向玩家发送成功登录服务器消息
    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_CENTER_LOGIN;
    root["MsgSubId"] = IDD_MESSAGE_CENTER_LOGIN_SUCESS;
    root["UserId"] = pUserId;
    root["username"] = pUserData.username;
    root["password"] = pUserData.password;
    root["bankpassword"] = pUserData.bankpassword;
    root["email"] = pUserData.email;
    root["sex"] = pUserData.sex;
    root["realname"] = pUserData.realname;
    root["homeplace"] = pUserData.homeplace;
    root["telephone"] = pUserData.telephone;
    root["QQ"] = pUserData.QQ;
    root["ipaddress"] = pUserData.ipaddress;

    root["money"] = (uint32)pUserData.money;
    root["bankmoney"] = (uint32)pUserData.bankmoney;
    root["experience"] = pUserData.experience;
    root["level"] = pUserData.level;

	Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

	std::string ploginipaddress = GetIpAddress(connId);
	// 更新用户登陆日期
	ServerDBOperator.UpdatePlayerLastLogin(pUserId,ploginipaddress,pMachineCode.c_str());
}

/// 处理得到当前在线服务器列表
void GameFrameManager::OnProcessUserGetOnlineRoomMes(uint32 connId)
{
	// 得到房间列表
	std::vector<tagGameRoom> pgamerooms;
	if(!ServerDBOperator.GetOnlineGameRooms(pgamerooms) || pgamerooms.empty())
	{
		Json::Value root;
		root["MsgId"] = IDD_MESSAGE_GET_GAMESERVER;
		root["MsgSubId"] = IDD_MESSAGE_GET_GAMEINFO_FAIL;

		Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

		return;
	}

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_GET_GAMESERVER;
    root["MsgSubId"] = IDD_MESSAGE_GET_GAMEINFO_SUCCESS;
    root["RoomCount"] = (int)pgamerooms.size();

	for(int i=0;i<(int)pgamerooms.size();i++)
    {
        Json::Value root2;
        root2["gameid"] = pgamerooms[i].gameid;
        root2["serverport"] = pgamerooms[i].serverport;
        root2["serverip"] = pgamerooms[i].serverip;
        root2["servername"] = pgamerooms[i].servername;
        root2["tablecount"] = pgamerooms[i].tablecount;
        root2["tableplayercount"] = pgamerooms[i].tableplayercount;
        root2["gamingtype"] = pgamerooms[i].gamingtype;
        root2["lastmoney"] = (uint32)pgamerooms[i].lastmoney;
        root2["pielement"] = (uint32)pgamerooms[i].pielement;
        root2["roomrevenue"] = pgamerooms[i].roomrevenue;
        root2["state"] = pgamerooms[i].state;

        root["Room"].append(root2);
    }

	Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
}

/// 得到所有的游戏信息
void GameFrameManager::OnProcessGetGamesMes(uint32 connId,Json::Value &mes)
{
	std::vector<GameDataInfo> pGameDataList;
	ServerDBOperator.GetGameInfos(pGameDataList);
	if(pGameDataList.empty())
	{
        Json::Value root;
        root["MsgId"] = IDD_MESSAGE_GET_GAMEINFO;
        root["MsgSubId"] = IDD_MESSAGE_GET_GAMEINFO_FAIL;
        Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());

		return;
	}

	std::vector<GameDataInfo> pTempPlayerList;

	for(int i=0;i < (int)pGameDataList.size();i++)
	{
		pTempPlayerList.push_back(pGameDataList[i]);
	}

	std::sort(pTempPlayerList.begin(),pTempPlayerList.end(),cmpList);

    Json::Value root;
    root["MsgId"] = IDD_MESSAGE_GET_GAMEINFO;
    root["MsgSubId"] = IDD_MESSAGE_GET_GAMEINFO_SUCCESS;
    root["GameCount"] = (int)pGameDataList.size();

	std::vector<GameDataInfo>::iterator iterTwo = pTempPlayerList.begin();
	for(int i=0;iterTwo!=pTempPlayerList.end();++iterTwo,i++)
	{
        Json::Value root2;
        root2["GameID"] = (*iterTwo).GameID;
        root2["GameName"] = (*iterTwo).GameName;
        root2["GameType"] = (*iterTwo).GameType;
        root2["MaxVersion"] = (*iterTwo).MaxVersion;
        root2["ProcessName"] = (*iterTwo).ProcessName;
        root2["GameLogo"] = (*iterTwo).GameLogo;
        root2["GameState"] = (*iterTwo).GameState;

        root["Game"].append(root2);
	}

	Sendhtml5(connId,(const char*)root.toStyledString().c_str(),root.toStyledString().length());
}
