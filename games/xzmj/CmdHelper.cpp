#include "CmdHelper.h"

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_DealMJ &info, Json::Value &msg)
{
	msg["iBankerUser"] = info.iBankerUser;
	msg["iIsRandomBanker"] = info.iIsRandomBanker;
	msg["iBaseScore"] = info.iBaseScore;
	msg["iDiceNum01"] = info.iDiceNum01;
	msg["iDiceNum02"] = info.iDiceNum02;
	msg["iDiceNum11"] = info.iDiceNum11;
	msg["iDiceNum12"] = info.iDiceNum12;
	msg["iUserChairID"] = info.iUserChairID;

	msg["isForceQuit"] = info.iHandMJCount;
	for (int i=0; i<info.iHandMJCount; i++)
	{
		msg["bHandMJ"][i] = info.bHandMJ[i];
	}
	msg["bOperation"] = info.bOperation;
	msg["iStatMJPos"] = info.iStatMJPos;

	msg["isForceQuit"] = info.iGangCount;
	for (int i=0; i<MAX_SELECT_GANG; i++)
	{
		msg["bMJGang"][i] = info.bMJGang[i];
	}

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_DealMJ &info, Json::Value &msg)
{
	info.iBankerUser = msg["iBankerUser"].asInt();
	info.iIsRandomBanker = msg["iIsRandomBanker"].asInt();
	info.iBaseScore = msg["iBaseScore"].asInt();
	info.iDiceNum01 = msg["iDiceNum01"].asInt();
	info.iDiceNum02 = msg["iDiceNum02"].asInt();
	info.iDiceNum11 = msg["iDiceNum11"].asInt();
	info.iDiceNum12 = msg["iDiceNum12"].asInt();
	info.iUserChairID = msg["iUserChairID"].asInt();

	info.iHandMJCount = msg["iHandMJCount"].asInt();
	for (int i=0; i<info.iHandMJCount; i++)
	{
		info.bHandMJ[i] = msg["bHandMJ"][i].asInt();
	}
	info.bOperation  = msg["bOperation"].asInt();
	info.iStatMJPos = msg["iStatMJPos"].asInt();

	info.iGangCount = msg["iGangCount"].asInt();
	for (int i=0; i<MAX_SELECT_GANG; i++)
	{
		info.bMJGang[i] = msg["bMJGang"][i].asInt();
	}

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_OutMJ &info, Json::Value &msg)
{
	msg["bOutMJ"] = info.bOutMJ;
	msg["wOutMJUser"] = info.wOutMJUser;
	msg["bOperation"] = info.bOperation;
	msg["isOtherPriority"] = info.isOtherPriority;
	msg["isTimeOut"] = info.isTimeOut;

	msg["info"] = info.size();

	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_OutMJ &info, Json::Value &msg)
{
	info.bOutMJ = msg["bOutMJ"].asInt();
	info.wOutMJUser = msg["wOutMJUser"].asInt();
	info.bOperation = msg["bOperation"].asInt();
	info.isOtherPriority = msg["isOtherPriority"].asInt();
	info.isTimeOut = msg["isTimeOut"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_TouchMJ &info, Json::Value &msg)
{
	msg["bTouchMJ"] = info.bTouchMJ;
	msg["bTouchType"] = info.bTouchType;
	msg["wCurrentUser"] = info.wCurrentUser;
	msg["bOperation"] = info.bOperation;
	msg["isOtherPriority"] = info.isOtherPriority;
	msg["bGangType"] = info.bGangType;

	msg["iGangCount"] = info.iGangCount;
	for (int i=0; i<MAX_SELECT_GANG; i++)
	{
		msg["bMJGang"][i] = info.bMJGang[i];
	}

	msg["info"] = info.size();

	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_TouchMJ &info, Json::Value &msg)
{
	info.bTouchMJ = msg["bTouchMJ"].asInt();
	info.bTouchType = msg["bTouchType"].asInt();
	info.wCurrentUser = msg["wCurrentUser"].asInt();
	info.bOperation = msg["bOperation"].asInt();
	info.isOtherPriority = msg["isOtherPriority"].asInt();
	info.bGangType = msg["bGangType"].asInt();

	info.iGangCount = msg["iGangCount"].asInt();
	for (int i=0; i<MAX_SELECT_GANG; i++)
	{
		info.bMJGang[i] = msg["bMJGang"][i].asInt();
	}

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_PengMJ &info, Json::Value &msg)
{
	msg["wLastOutUser"] = info.wLastOutUser;
	msg["wCurrentUser"] = info.wCurrentUser;
	msg["bMJPeng"] = info.bMJPeng;
	msg["bOperation"] = info.bOperation;
	msg["isOtherPriority"] = info.isOtherPriority;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_PengMJ &info, Json::Value &msg)
{
	info.wLastOutUser = msg["wLastOutUser"].asInt();
	info.wCurrentUser = msg["wCurrentUser"].asInt();
	info.bMJPeng = msg["bMJPeng"].asInt();

	info.bOperation = msg["bOperation"].asInt();
	info.isOtherPriority = msg["isOtherPriority"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_GangMJ &info, Json::Value &msg)
{
	msg["bGangType"] = info.bGangType;
	msg["wCurrentUser"] = info.wCurrentUser;
	msg["wOutMJUser"] = info.wOutMJUser;
	msg["bMJGang"] = info.bMJGang;
	msg["bOperation"] = info.bOperation;
	msg["isOtherPriority"] = info.isOtherPriority;
	msg["iGangScore"] = info.iGangScore;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_GangMJ &info, Json::Value &msg)
{
	info.bGangType = msg["bGangType"].asInt();
	info.wCurrentUser = msg["wCurrentUser"].asInt();
	info.wOutMJUser = msg["wOutMJUser"].asInt();
	info.bMJGang = msg["bMJGang"].asInt();
	info.bOperation = msg["bOperation"].asInt();
	info.isOtherPriority = msg["isOtherPriority"].asInt();
	info.iGangScore = msg["iGangScore"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_GiveUp &info, Json::Value &msg)
{
	msg["bOperation"] = info.bOperation;
	msg["bOutMJ"] = info.bOutMJ;
	msg["isOtherPriority"] = info.isOtherPriority;
	msg["isOtherHu"] = info.isOtherHu;
	msg["iOperUser"] = info.iOperUser;
	msg["iLastOutUser"] = info.iLastOutUser;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_GiveUp &info, Json::Value &msg)
{
	info.bOperation = msg["bOperation"].asInt();
	info.bOutMJ = msg["bOutMJ"].asInt();
	info.isOtherPriority = msg["isOtherPriority"].asInt();
	info.isOtherHu = msg["isOtherHu"].asInt();
	info.iOperUser = msg["iOperUser"].asInt();
	info.iLastOutUser = msg["iLastOutUser"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_Trustee &info, Json::Value &msg)
{
	msg["bUserTrustee"] = info.bUserTrustee;
	msg["iUserChairID"] = info.iUserChairID;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_Trustee &info, Json::Value &msg)
{
	info.bUserTrustee = msg["bUserTrustee"].asInt();
	info.iUserChairID = msg["iUserChairID"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_HU &info, Json::Value &msg)
{
	msg["bUserHuMJ"] = info.bUserHuMJ;
	msg["iHuUser"] = info.iHuUser;
	msg["iDianPaoUser"] = info.iDianPaoUser;

	msg["enhuType"] = info.enhuType;

	msg["iNomalFan"] = (Json::Int64)info.iNomalFan;
	msg["iExtraFan"] = (Json::Int64)info.iExtraFan;
	msg["iHuScore"] = (Json::Int64)info.iHuScore;
	msg["iGangScore"] = (Json::Int64)info.iGangScore;
	msg["iDianPaoScore"] = (Json::Int64)info.iDianPaoScore;
	msg["iAllScore"] = (Json::Int64)info.iAllScore;
	for (int i=0; i<MAX_FAN_STYLE; i++)
	{
		msg["iFanStyle"][i] = info.iFanStyle[i];
	}

	msg["iAlreadyHu"] = info.iAlreadyHu;
	msg["bOperation"] = info.bOperation;
	msg["isOtherPriority"] = info.isOtherPriority;
	msg["isNowOper"] = info.isNowOper;
	msg["isOtherHu"] = info.isOtherHu;

	msg["bGangType"] = info.bGangType;
	msg["iGangCount"] = info.iGangCount;
	for (int i=0; i<MAX_SELECT_GANG; i++)
	{
		msg["bMJGang"][i] = info.bMJGang[i];
	}

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_HU &info, Json::Value &msg)
{
	info.bUserHuMJ = msg["bUserHuMJ"].asInt();
	info.iHuUser = msg["iHuUser"].asInt();
	info.iDianPaoUser = msg["iDianPaoUser"].asInt();

	info.enhuType = (enHuType)msg["enhuType"].asInt();

	info.iNomalFan = msg["iNomalFan"].asInt64();
	info.iExtraFan = msg["iExtraFan"].asInt64();
	info.iHuScore = msg["iHuScore"].asInt64();
	info.iGangScore = msg["iGangScore"].asInt64();
	info.iDianPaoScore = msg["iDianPaoScore"].asInt64();
	info.iAllScore = msg["iAllScore"].asInt64();

	for (int i=0; i<MAX_FAN_STYLE; i++)
	{
		info.iFanStyle[i] = msg["iFanStyle"][i].asInt();
	}

	info.iAlreadyHu = msg["iAlreadyHu"].asInt();
	info.bOperation = msg["bOperation"].asInt();
	info.isOtherPriority = msg["isOtherPriority"].asInt();
	info.isNowOper = msg["isNowOper"].asInt();
	info.isOtherHu = msg["isOtherHu"].asInt();

	info.bGangType = msg["bGangType"].asInt();
	info.iGangCount = msg["iGangCount"].asInt();
	for (int i=0; i<MAX_SELECT_GANG; i++)
	{
		info.bMJGang[i] = msg["bMJGang"][i].asInt();
	}

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_GameOver &info, Json::Value &msg)
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_BLOCK_COUNT; j++)
		{
			msg["bHUMjfirst"][i][j] = info.bHUMj[i][j].first;
			msg["bHUMjstyle"][i][j] = info.bHUMj[i][j].style;
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["bHUMjCount"][i] = info.bHUMjCount[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_HAND_MJ; j++)
		{
			msg["bHandMj"][i][j] = info.bHandMj[i][j];
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["bHandMjCount"][i] = info.bHandMjCount[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_OUT_MJ; j++)
		{
			msg["bOutMj"][i][j] = info.bOutMj[i][j];
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["bOutMjCount"][i] = info.bOutMjCount[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iDianPaoUser"][i] = info.iDianPaoUser[i];
	}

	msg["enEndType"] = info.enEndType;
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["enhuType"][i] = info.enhuType[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iNomalFan"][i] = (Json::Int64)info.iNomalFan[i];
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iExtraFan"][i] = (Json::Int64)info.iExtraFan[i];
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iFanScore"][i] = (Json::Int64)info.iFanScore[i];
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iGangScore"][i] = (Json::Int64)info.iGangScore[i];
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iTotalScore"][i] = (Json::Int64)info.iTotalScore[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["enUSEndType"][i] = info.enUSEndType[i];
	}

	msg["isChangeTable"] = info.isChangeTable;

	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iFanStyle"][i] = info.iFanStyle[i];
	}

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_GameOver &info, Json::Value &msg)
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_BLOCK_COUNT; j++)
		{
			info.bHUMj[i][j].first = msg["bHUMjfirst"][i][j].asInt();
			info.bHUMj[i][j].style = (enBlockStyle)msg["bHUMjstyle"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.bHUMjCount[i] = msg["bHUMjCount"][i].asInt();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_HAND_MJ; j++)
		{
			info.bHandMj[i][j] = msg["bHandMj"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.bHandMjCount[i] = msg["bHandMjCount"][i].asInt();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_OUT_MJ; j++)
		{
			info.bOutMj[i][j] = msg["bOutMj"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.bOutMjCount[i] = msg["bOutMjCount"][i].asInt();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iDianPaoUser[i] = msg["iDianPaoUser"][i].asInt();
	}

	info.enEndType = (enEndGameType)msg["enEndType"].asInt();
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.enhuType[i] = (enHuType)msg["enhuType"][i].asInt();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iNomalFan[i] = msg["iNomalFan"][i].asInt64();
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iExtraFan[i] = msg["iExtraFan"][i].asInt64();
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iFanScore[i] = msg["iFanScore"][i].asInt64();
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iGangScore[i] = msg["iGangScore"][i].asInt64();
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iTotalScore[i] = msg["iTotalScore"][i].asInt64();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.enUSEndType[i] = (enUserEndType)msg["enUSEndType"][i].asInt();
	}

	info.isChangeTable = msg["isChangeTable"].asInt();

	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iFanStyle[i] = msg["iFanStyle"][i].asInt();
	}

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_ReEnterRoom &info, Json::Value &msg)
{
	msg["iBankerUser"] = info.iBankerUser;
	msg["iBaseScore"] = info.iBaseScore;
	msg["iIsRandomBanker"] = info.iIsRandomBanker;
	msg["iDiceNum01"] = info.iDiceNum01;
	msg["iDiceNum02"] = info.iDiceNum02;
	msg["iDiceNum11"] = info.iDiceNum11;
	msg["iDiceNum12"] = info.iDiceNum12;

	msg["iUserChairID"] = info.iUserChairID;

	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iHandMJCount"][i] = info.iHandMJCount[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_HAND_MJ; j++)
		{
			msg["bHandMJ"][i][j] = info.bHandMJ[i][j];
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iHuMJCount"][i] = info.iHuMJCount[i];
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_BLOCK_COUNT; j++)
		{
			msg["bHuMJfirst"][i][j] = info.bHuMJ[i][j].first;
			msg["bHuMJstyle"][i][j] = info.bHuMJ[i][j].style;
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["iOutMJCount"][i] = info.iOutMJCount[i];
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_OUT_MJ; j++)
		{
			msg["bOutMJ"][i][j] = info.bOutMJ[i][j];
		}
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["enUserStatu"][i] = info.enUserStatu[i];
	}

	msg["iOldOperUser"] = info.iOldOperUser;
	msg["iCurrentUser"] = info.iCurrentUser;
	msg["isOtherPriority"] = info.isOtherPriority;

	msg["iHuFan"] = (Json::Int64)info.iHuFan;
	msg["iGangFan"] = (Json::Int64)info.iGangFan;
	msg["iHuScore"] = (Json::Int64)info.iHuScore;
	msg["iGangScore"] = (Json::Int64)info.iGangScore;

	for (int i=0; i<MAX_FAN_STYLE; i++)
	{
		msg["iFanStyle"][i] = (Json::Int64)info.iFanStyle[i];
	}

	msg["iFanStyle"] = info.iFanStyle;
	msg["iDianPaoUser"] = info.iDianPaoUser;

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		msg["bDianPaoMJ"][i] = info.bDianPaoMJ[i];
	}

	msg["isForceQuit"] = info.isForceQuit;
	msg["isYiPaoDuoXiang"] = info.isYiPaoDuoXiang;
	msg["iReadyPlayer"] = info.iReadyPlayer;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_ReEnterRoom &info, Json::Value &msg)
{
	info.iBankerUser = msg["iBankerUser"].asInt();
	info.iBaseScore = msg["iBaseScore"].asInt();
	info.iIsRandomBanker = msg["iIsRandomBanker"].asInt();
	info.iDiceNum01 = msg["iDiceNum01"].asInt();
	info.iDiceNum02 = msg["iDiceNum02"].asInt();
	info.iDiceNum11 = msg["iDiceNum11"].asInt();
	info.iDiceNum12 = msg["iDiceNum12"].asInt();

	info.iUserChairID = msg["iUserChairID"].asInt();


	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iHandMJCount[i] = msg["iHandMJCount"][i].asInt();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_HAND_MJ; j++)
		{
			info.bHandMJ[i][j] = msg["bHandMJ"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iHuMJCount[i] = msg["iHuMJCount"][i].asInt();
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_BLOCK_COUNT; j++)
		{
			info.bHuMJ[i][j].first = msg["bHuMJfirst"][i][j].asInt();
			info.bHuMJ[i][j].style = (enBlockStyle)msg["bHuMJstyle"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.iOutMJCount[i] = msg["iOutMJCount"][i].asInt();
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_OUT_MJ; j++)
		{
			info.bOutMJ[i][j] = msg["bOutMJ"][i][j].asInt();
		}
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.enUserStatu[i] = (enPlayerStatu)msg["enUserStatu"][i].asInt();
	}


	info.iOldOperUser = msg["iOldOperUser"].asInt();
	info.iCurrentUser = msg["iCurrentUser"].asInt();
	info.isOtherPriority = msg["isOtherPriority"].asInt();

	info.iHuFan = msg["iHuFan"].asInt64();
	info.iGangFan = msg["iGangFan"].asInt64();
	info.iHuScore = msg["iHuScore"].asInt64();
	info.iGangScore = msg["iGangScore"].asInt64();

	for (int i=0; i<MAX_FAN_STYLE; i++)
	{
		info.iFanStyle[i] = msg["iFanStyle"][i].asInt64();
	}

	info.enhuType = (enHuType)msg["enhuType"].asInt();
	info.iDianPaoUser = msg["iDianPaoUser"].asInt();

	for (int i = 0; i < GAME_PLAYER; i++)
	{
		info.bDianPaoMJ[i] = msg["bDianPaoMJ"][i].asInt();
	}

	info.isForceQuit = msg["isForceQuit"].asInt();
	info.isYiPaoDuoXiang = msg["isYiPaoDuoXiang"].asInt();
	info.iReadyPlayer = msg["iReadyPlayer"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_C_OutMJ &info, Json::Value &msg)
{
	msg["bOutMJ"] = info.bOutMJ;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_C_OutMJ &info, Json::Value &msg)
{
	info.bOutMJ = msg["bOutMJ"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_C_GangMJ &info, Json::Value &msg)
{
	msg["bGangMJ"] = info.bGangMJ;
	msg["bGangType"] = info.bGangType;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_C_GangMJ &info, Json::Value &msg)
{
	info.bGangMJ = msg["bGangMJ"].asInt();
	info.bGangType = (enOperationGangType)msg["bGangType"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_C_Chat &info, Json::Value &msg)
{
	msg["iFaceID"] = info.iFaceID;
	msg["iTalkID"] = info.iTalkID;
	msg["bType"] = info.bType;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_C_Chat &info, Json::Value &msg)
{
	info.iFaceID = msg["iFaceID"].asInt();
	info.iTalkID = msg["iTalkID"].asInt();
	info.bType = (enChatType)msg["bType"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_Chat &info, Json::Value &msg)
{
	msg["iChatUser"] = info.iChatUser;
	msg["iFaceID"] = info.iFaceID;
	msg["iTalkID"] = info.iTalkID;
	msg["bType"] = info.bType;

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_Chat &info, Json::Value &msg)
{
	info.iChatUser = msg["iChatUser"].asInt();
	info.iFaceID = msg["iFaceID"].asInt();
	info.iTalkID = msg["iTalkID"].asInt();
	info.bType = (enChatType)msg["bType"].asInt();

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
bool CmdHelper::CodeMessage(CMD_S_MJ &info, Json::Value &msg)
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_BLOCK_COUNT; j++)
		{
			msg["bHUMjfirst"][i][j] = info.bHUMj[i][j].first;
			msg["bHUMjstyle"][i][j] = info.bHUMj[i][j].style;
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["bHUMjCount"][i] = info.bHUMjCount[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_HAND_MJ; j++)
		{
			msg["bHandMj"][i][j] = info.bHandMj[i][j];
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["bHandMjCount"][i] = info.bHandMjCount[i];
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_OUT_MJ; j++)
		{
			msg["bOutMj"][i][j] = info.bOutMj[i][j];
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		msg["bOutMjCount"][i] = info.bOutMjCount[i];
	}

	msg["info"] = info.size();
	return true;
}

bool CmdHelper::DeCodeMessage(CMD_S_MJ &info, Json::Value &msg)
{
	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_BLOCK_COUNT; j++)
		{
			info.bHUMj[i][j].first = msg["bHUMjfirst"][i][j].asInt();
			info.bHUMj[i][j].style = (enBlockStyle)msg["bHUMjstyle"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.bHUMjCount[i] = (MJBYTE)msg["bHUMjCount"][i].asInt();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_HAND_MJ; j++)
		{
			info.bHandMj[i][j] = (MJBYTE)msg["bHandMj"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.bHandMjCount[i] = (MJBYTE)msg["bHandMjCount"][i].asInt();
	}

	for (int i=0; i<GAME_PLAYER; i++)
	{
		for (int j=0; j<MAX_OUT_MJ; j++)
		{
			info.bOutMj[i][j] = (MJBYTE)msg["bOutMj"][i][j].asInt();
		}
	}
	for (int i=0; i<GAME_PLAYER; i++)
	{
		info.bOutMjCount[i] = msg["bOutMjCount"][i].asInt();
	}

	if (msg["info"].asInt() != info.size())
	{
		return false;
	}
	return true;
}

