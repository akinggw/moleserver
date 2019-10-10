#ifndef _C_COMMON_RENDER_DEVICE_H_INCLUDE_
#define _C_COMMON_RENDER_DEVICE_H_INCLUDE_

class CCommonRenderFrame;
class CMolMessageOut;
class Player;

/** 
 * 处于游戏什么阶段
 */
enum GameStage
{
	GAMESTATE_MAINHALL = 0,               // 大厅
	GAMESTATE_GAMEROOM,                   // 房间
	GAMESTATE_NULL 
};

/** 
 * 录像状态
 */
enum VideoState
{
	VIDEO_RECORDING = 0,                       //录制
	VIDEO_PLAYBACK,                            //回放
	VIDEO_NULL
};

/** 
 * 通用的渲染设备
 */
class CCommonRenderDevice 
{
public:
	/// 得到渲染设备
	virtual irr::IrrlichtDevice* GetDevice(void) = 0;
	/// 显示图片
	virtual void draw2DImage(irr::video::ITexture* texture, irr::core::rect<irr::s32> sourceRect, const core::matrix4& mat, bool useAlphaChannel, irr::video::SColor color, bool useFiltering/* = true*/, bool usePremultipliedAlpha/* = false*/) = 0;
	/// 得到当前游戏场景类型
	virtual GameStage GetCurGameSceneStage(void) = 0;
	/// 得到当前游戏场景
	virtual CCommonRenderFrame* GetCurrentGameScene(void) = 0;
	/// 离开游戏房间
	virtual void LeaveGameRoom(bool isTip=true) = 0;
	/// 设置是否要退出游戏提示
	virtual void SetIsCloseTip(bool tip) = 0;
	/// 得到当前应用程序
	virtual CWinApp* GetCurWinApp(void) = 0;
	/// 设置游戏帧数
	virtual void SetFPS(int frame=60) = 0;
	/// 根据玩家ID得到玩家信息
	virtual Player* GetOnlinePlayer(int userid) = 0;
	/// 得到当前服务器玩家人数
	virtual int GetOnlinePlayerCount(void) = 0;
	/// 全屏窗口切换
	virtual void SwitchToFullscreen(void) = 0;
	/// 根据玩家得到玩家的头像ID
	virtual int GetAvatarIndexByPlayer(CString avatarIndex) = 0;
	/// 得到当前系统图标列表
	virtual gui::IGUISpriteBank* GetAvatarSpriteBank(void) = 0;
	/// 得到当前要使用的自定义头像的下载网址
	virtual CString GetCustomAvatarUrl(void) = 0;
	/// 根据类型得到当前选中的网址(0:主页;1:论坛;2:客服;3:充值;4:找回密码;5:用户头像;6:商城;7:游戏规则)
	virtual CString GetSelectedUrlByType(int type,bool isOpen=false) = 0;
	/// 得到指定文件完整的当前工作路径
	virtual CString GetCurrentWorkingPath(CString filename) = 0;
	/// 添加一个图标到系统中
	virtual void addTextureAsSprite(CString icon,CString path) = 0;
	/// 游戏状态提示
	virtual BOOL ShowBalloonTip(LPCTSTR szMessage,LPCTSTR szTitle,UINT uTimeout=5,DWORD dwInfoFlags=1,bool isBalloon=false) = 0;
	/// 显示头像
	virtual video::ITexture* DrawUserAvatar(Player *pPlayer,core::rect<s32> pRect) = 0;
	/// 得到游戏中常用提示语
	virtual CString GetGamingCommonUseTip(CString tip) = 0;
	/// 得到用户在游戏中称号
	virtual CString GetPlayerPraised(Player *pPlayer) = 0;
	/// 添加信息显示框
	virtual gui::IGUITextArea* AddInformationBox(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// 添加表情选择框
	virtual gui::IGUIChatShow* addChatShow(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// 添加聊天常用提示语框
	virtual gui::IGUIChatTip* addChatTip(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// 添加包裹选择框
	virtual gui::IGUIPackage* addPackage(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// 添加信息滚动框
	virtual gui::IGUIScrollBox* addScrollBox(const core::rect<s32>& rectangle,IGUIElement* parent=0) = 0;
	/// 显示信息
	virtual void ShowMessage(CString msg,video::SColor col=video::SColor(255,255,255,255),int msgType=IDD_MESSAGE_TYPE_NULL,CString sender=TEXT(""),CString reciver=TEXT("")) = 0;

	/// 开始一个定时器
	virtual bool StartTimer(int timerId,int space) = 0;
	/// 关闭一个定时器
	virtual void StopTimer(int id) = 0;
	/// 关闭所有的定时器
	virtual void StopAllTimer(void) = 0;

	/// 播放指定的音乐文件
	virtual void MusicPlay(CString sound,bool isLoop=false) = 0;
	/// 播放指定的音效文件
	virtual void EffectPlay(CString sound) = 0;
	/// 停止指定的声音文件
	virtual void SoundStop(CString sound) = 0;
	/// 停止播放所有的声音
	virtual void SoundStopAll(void) = 0;

	/// 发送游戏数据
	virtual void SendGameMsg(CMolMessageOut &msg) = 0;
	/// 发送准备消息
	virtual void SendReadyMsg(void) = 0;
	/// 发送换桌消息
	virtual void SendChangeTableMsg(void) = 0;
	/// 发送进入到下一次比赛消息
	virtual void SendEnterNextMatching(void) = 0;
	/// 关闭连接
	virtual void CloseSocket(void) = 0;
	/// 是否连接上
	virtual bool IsConnected(void) = 0;
	/// 连接指定的服务器
	virtual bool Connect(std::string ipaddress,int port) = 0;

	/// 字符串编码转换
	virtual std::wstring ConverToWideChar(const std::string& str) = 0;
	virtual std::string ConverToMultiChar(const std::wstring& str) = 0;
	virtual CString Utf8ConverToWideChar(const std::string& str) = 0;
	virtual std::string WideCharConverToUtf8(CString& str) = 0;
	/// 根据用户IP地址得到用户实际地理位置
	virtual CString GetIPPosition(unsigned __int32 wIPPosition) = 0;
	/// 设置过场动画
	virtual void SetCinematicsType(int type) = 0;

	/// 开始录制
	virtual void StartVideoTape(Player *pPlayer,Room *pRoom)=NULL;
	/// 结束录制
	virtual void EndVideoTape(void)=NULL;
	/// 得到当前状态
	virtual VideoState GetVideoState(void) = NULL;
	//记录游戏消息
	virtual void OnTapeGameMessage(const void * pBuffer, WORD wDataSize)=NULL;
};

#endif