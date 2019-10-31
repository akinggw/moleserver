#ifndef _DEFINES_H_INCLUDE_
#define _DEFINES_H_INCLUDE_

#include <string>

#define IDD_MESSAGE_HEART_BEAT                  100                   // 心跳消息

/*********************************************************************************************/

#define IDD_MESSAGE_REGISTER_GAME               200                   // 注册游戏
#define IDD_MESSAGE_REGISTER_SUCCESS            201                   // 注册游戏成功
#define IDD_MESSAGE_REGISTER_FAIL               202                   // 注册游戏失败
#define IDD_MESSAGE_RE_REGISTER                 203                   // 重复注册
#define IDD_MESSAGE_REGISTER_EXIST              204                   // 服务器存在

/*********************************************************************************************/

#define IDD_MESSAGE_CONNECT                     300                   // 用户连接消息
#define IDD_MESSAGE_CONNECT_SUCESS              301                   // 用户连接成功
#define IDD_MESSAGE_CONNECT_FAIL                302                   // 用户连接失败
#define IDD_MESSAGE_CONNECT_EXIST               303                   // 用户连接存在

/*********************************************************************************************/

#define IDD_MESSAGE_CENTER_LOGIN                400                   // 用户登录消息
#define IDD_MESSAGE_CENTER_LOGIN_SUCESS         401                   // 用户登录成功
#define IDD_MESSAGE_CENTER_LOGIN_FAIL           402                   // 用户登录失败
#define IDD_MESSAGE_CENTER_LOGIN_BUSY           403                   // 系统忙,用户登录过于频繁
#define IDD_MESSAGE_CENTER_BANK                 404                   // 银行操作
#define IDD_MESSAGE_CENTER_BANK_GET_MONEY       405                   // 得到用户的钱
#define IDD_MESSAGE_CENTER_BANK_TRANSFER        406                   // 存取操作
#define IDD_MESSAGE_CENTER_BANK_UPDATEBANKPWD   407                   // 修改银行密码
#define IDD_MESSAGE_CENTER_BANK_SUCESS          408                   // 银行操作成功
#define IDD_MESSAGE_CENTER_BANK_FAIL            409                   // 银行操作失败
#define IDD_MESSAGE_CENTER_UPDATEUSER           410                   // 玩家信息操作
#define IDD_MESSAGE_CENTER_UPDATEUSER_SUCESS    411                   // 玩家信息操作成功
#define IDD_MESSAGE_CENTER_UPDATEUSER_FAIL      412                   // 玩家信息操作失败
#define IDD_MESSAGE_CENTER_UPDATEUSER_INFO      413                   // 更新玩家信息
#define IDD_MESSAGE_CENTER_UPDATEUSER_PWD       414                   // 更新玩家登陆密码
#define IDD_MESSAGE_CENTER_LOGIN_BAN            415                   // 账号被封
#define IDD_MESSAGE_CENTER_BANK_TRANSFERACCOUNT 416                   // 银行转账
#define IDD_MESSAGE_CENTER_GETGAMEONLINECOUNT   417                   // 获取指定游戏的在线人数
#define IDD_MESSAGE_CENTER_LOCKMACHINE          418                   // 锁定机器
#define IDD_MESSAGE_CENTER_LOCKMACHINE_SUC      419                   // 锁定机器成功
#define IDD_MESSAGE_CENTER_LOCKMACHINE_FAI      420                   // 锁定机器失败
#define IDD_MESSAGE_CENTER_BANK_FAIL_LASTMONEY  421                   // 转账金币少于规定值
#define IDD_MESSAGE_CENTER_BANK_FAIL_LASTCOUNT  422                   // 一天只能转3次
#define IDD_MESSAGE_CENTER_SIGNIN               423                   // 每日签到
#define IDD_MESSAGE_CENTER_SINGIN_GETMES        424                   // 得到签到信息
#define IDD_MESSAGE_CENTER_SINGIN_START         425                   // 开始签到
#define IDD_MESSAGE_CENTER_SINGIN_SUCESS        426                   // 签到成功
#define IDD_MESSAGE_CENTER_SINGIN_FAIL          427                   // 签到失败
#define IDD_MESSAGE_CENTER_BUYGOODS             428                   // 买商品
#define IDD_MESSAGE_CENTER_WEEKSIGNIN           429                   // 每周签到
#define IDD_MESSAGE_CENTER_WEEKSINGIN_GETMES    430                   // 获取每周的签到信息
#define IDD_MESSAGE_CENTER_BANK_CARDCHONGZHI    431                   // 点卡充值
#define IDD_MESSAGE_CENTER_BANK_CARDCZ_SUC      432                   // 点卡充值成功
#define IDD_MESSAGE_CENTER_BANK_CARDCZ_FAIL     433                   // 点卡充值失败
#define IDD_MESSAGE_CHAT_SENDMSG                434                   // 发送聊天信息
#define IDD_MESSAGE_CHAT_SENDHONGBAO            435                   // 发送红包
#define IDD_MESSAGE_CHAT_SENDHONGBAO_SUC        436                  // 发送红包成功
#define IDD_MESSAGE_CHAT_SENDHONGBAO_FAIL       437                   // 发送红包失败
#define IDD_MESSAGE_CHAT_SENDHONGBAO_GET        438                   // 抢红包
#define IDD_MESSAGE_CHAT_SENDHONGBAO_GET_SUC    439                   // 抢红包成功
#define IDD_MESSAGE_CHAT_SENDHONGBAO_GET_FAIL   440                   // 抢红包失败
#define IDD_MESSAGE_CENTER_BANK_OTHERCHONGZHI   441                   // 其它充值
#define IDD_MESSAGE_CENTER_BANK_OTHERCZ_FAIL    442                   // 其它充值失败
#define IDD_MESSAGE_CENTER_BANK_OTHERCZ_SUC     443                   // 其它充值成功
#define IDD_MESSAGE_CENTER_BANK_GETDUIHUANRATE  444                   // 得到金币兑换比例
#define IDD_MESSAGE_CENTER_BANK_STARTDUIHUAN    445                   // 开始金币兑换
#define IDD_MESSAGE_CENTER_BANK_GETCHONGZHIRATE 446                   // 得到充值比例
#define IDD_MESSAGE_CHAT_SHISHIGUANGGAO         447                   // 游戏实时广告
#define IDD_MESSAGE_CHAT_SHISHIGUANGGAO_GET     448                   // 得到广告数据
#define IDD_MESSAGE_CHAT_SHISHIGUANGGAO_SEND    449                   // 发送广告
#define IDD_MESSAGE_CENTER_LOGIN_BANLOGIN       450                   // 服务器禁止登录

/*********************************************************************************************/

#define IDD_MESSAGE_GAME_LOGIN                  500                   // 用户登录消息
#define IDD_MESSAGE_GAME_LOGIN_SUCESS           501                   // 用户登录成功
#define IDD_MESSAGE_GAME_LOGIN_FAIL             502                   // 用户登录失败
#define IDD_MESSAGE_GAME_LOGIN_BUSY             503                   // 系统忙,用户登录过于频繁
#define IDD_MESSAGE_GAME_LOGIN_EXIST            504                   // 用户已经在系统中
#define IDD_MESSAGE_GAME_LOGIN_FULL             505                   // 服务器满
#define IDD_MESSAGE_GAME_LOGIN_CLOSE_SERVER     506                   // 关闭当前服务器
#define IDD_MESSAGE_GAME_LOGIN_MATCHING_NOSTART 507                   // 比赛未开始
#define IDD_MESSAGE_GAME_LOGIN_MATCHING_NOSCROE 508                   // 没有达到比赛场所要求的积分
#define IDD_MESSAGE_GAME_LOGIN_MATCHING_NOLEVEL 509                   // 没有达到比赛场所要求的等级

/*********************************************************************************************/

#define IDD_MESSAGE_UPDATE_GAME_SERVER          600                   // 更新游戏服务器信息
#define IDD_MESSAGE_UPDATE_GAME_SUCCESS         601                   // 更新游戏服务器信息成功
#define IDD_MESSAGE_UPDATE_GAME_FAIL            602                   // 更新游戏服务器信息失败

/*********************************************************************************************/

#define IDD_MESSAGE_USER_REGISTER               700                   // 用户注册
#define IDD_MESSAGE_USER_REGISTER_SUCCESS       701                   // 用户注册成功
#define IDD_MESSAGE_USER_REGISTER_FAIL          702                   // 用户注册失败
#define IDD_MESSAGE_SUPER_BIG_MSG               703                   // 广播消息

/*********************************************************************************************/

#define IDD_MESSAGE_GET_GAMESERVER              800                   // 得到游戏服务器列表
#define IDD_MESSAGE_GET_GAMEINFO                801                   // 得到游戏信息列表
#define IDD_MESSAGE_GET_GAMEINFO_SUCCESS        802                   // 得到游戏信息列表成功
#define IDD_MESSAGE_GET_GAMEINFO_FAIL           803                   // 得到游戏信息列表失败

/*********************************************************************************************/

#define IDD_MESSAGE_FRAME                       900                   // 框架消息
#define IDD_MESSAGE_ENTER_ROOM                  901                   // 进入游戏房间
#define IDD_MESSAGE_LEAVE_ROOM                  902                   // 离开游戏房间
#define IDD_MESSAGE_REENTER_ROOM                903                   // 用户掉线后重新回到游戏房间
#define IDD_MESSAGE_CHANGER_ROOM                904                   // 换游戏房间
#define IDD_MESSAGE_FALLLINE_ROOM               905                   // 用户从游戏房间中掉线
#define IDD_MESSAGE_GET_ROOM_LIST               906                   // 得到房间列表
#define IDD_MESSAGE_LOOKON_ENTER_ROOM           907                   // 旁观进入游戏房间
#define IDD_MESSAGE_GET_ONLINE_PLAYERS          908                   // 得到当前所有在线玩家
#define IDD_MESSAGE_LEAVE_SERVER                909                   // 离开游戏服务器
#define IDD_MESSAGE_GET_ONLINE_PLAYERS_END      910                   // 获取在线玩家结束
#define IDD_MESSAGE_GET_ROOM_LIST_END           911                   // 获取房间列表结束
#define IDD_MESSAGE_ENTER_ROOM_FAIL             912                   // 进入游戏房间失败
#define IDD_MESSAGE_ENTER_ROOM_SUCC             913                   // 进入游戏房间成功
#define IDD_MESSAGE_ENTER_ROOM_LASTMONEY        914                   // 金币不能满足房间要求
#define IDD_MESSAGE_READY_START                 915                   // 玩家准备开始游戏
#define IDD_MESSAGE_GAME_START                  916                   // 游戏开始
#define IDD_MESSAGE_GAME_END                    917                   // 游戏结束
#define IDD_MESSAGE_UPDATE_USER_DATA            918                   // 更新用户信息
#define IDD_MESSAGE_USER_CHAT                   919                   // 用户聊天消息
#define IDD_MESSAGE_ENTER_ROOM_EXIST            920                   // 已经在另一个游戏中了
#define IDD_MESSAGE_UPDATE_USER_MONEY           921                   // 更新用户金币
#define IDD_MESSAGE_FRAME_SUPER_KICK_PLAYER     922                   // 踢指定玩家下线并封号
#define IDD_MESSAGE_FRAME_SUPER_BIG_MSG         923                   // 大喇叭消息
#define IDD_MESSAGE_FRAME_ROOM_UNDERFULL        924                   // 未满房间提示消息
#define IDD_MESSAGE_FRAME_MATCH                 925                   // 比赛消息
#define IDD_MESSAGE_FRAME_MATCH_START           926                   // 比赛开始消息
#define IDD_MESSAGE_FRAME_MATCH_OVER            927                   // 比赛结束消息
#define IDD_MESSAGE_FRAME_MATCH_GETRAKING       928                   // 获取比赛排名
#define IDD_MESSAGE_FRAME_MATCH_CONTINUE        929                   // 比赛继续
#define IDD_MESSAGE_FRAME_MATCH_WAITINGCOUNT    930                   // 比赛等待人数
#define IDD_MESSAGE_ENTER_ROOM_FULL             931                   // 进入游戏房间失败,房间已满
#define IDD_MESSAGE_FRAME_MATCH_TIME            932                   // 更改当前比赛时间
#define IDD_MESSAGE_FRAME_ROOM_SITDOWN          933                   // 坐下
#define IDD_MESSAGE_FRAME_ROOM_STANDUP          934                   // 起来
#define IDD_MESSAGE_FRAME_JACKPOT               935                   // 奖池信息
#define IDD_MESSAGE_FRAME_JACKPOT_REFRESH       936                   // 奖池彩金刷新
#define IDD_MESSAGE_FRAME_JACKPOT_WINTIP        937                   // 奖池获奖提示
#define IDD_MESSAGE_FRAME_ROOM_JOINSET          938                   // 房间进入设置
#define IDD_MESSAGE_FRAME_MATCH_GETPRIZE        939                   // 得到比赛奖品
#define IDD_MESSAGE_FRAME_ENTER_MATCHINGTIP     940                   // 比赛中提示
#define IDD_MESSAGE_ENTER_ROOM_BAOMING_SUC      941                   // 报名成功
#define IDD_MESSAGE_ENTER_ROOM_TUISAI_SUC       942                   // 退赛成功
#define IDD_MESSAGE_PERSONROOM_ENTER_ROOM_FAIL  943                   // 私人游戏房间进入失败

/*******************************************游戏消息**************************************************/

#define IDD_MESSAGE_ROOM					    1000                  // 游戏房间消息

#endif
