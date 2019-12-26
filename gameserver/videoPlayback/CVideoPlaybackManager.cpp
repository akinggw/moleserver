#include "CVideoPlaybackManager.h"
#include <stdio.h>

void string_replace( std::string &strBig, const std::string &strsrc, const std::string &strdst )
{
	std::string::size_type pos = 0;
	std::string::size_type srclen = strsrc.size();
	std::string::size_type dstlen = strdst.size();

	while( (pos=strBig.find(strsrc, pos)) != std::string::npos )
	{
		strBig.replace( pos, srclen, strdst );
		pos += dstlen;
	}
}

/// 构造函数
CVideoPlaybackManager::CVideoPlaybackManager():m_mySelfID(0),m_curFrameIndex(0),m_VideoState(VIDEO_NULL),m_mySelfChairIndex(-1)
{

}

/// 析构函数
CVideoPlaybackManager::~CVideoPlaybackManager()
{
	Clear();
}

/// 清除所有的帧
void CVideoPlaybackManager::ClearAllFrames(void)
{
	if(m_VideoGameSteps.empty()) return;

	std::vector<VideoGameStep*>::iterator iter = m_VideoGameSteps.begin();
	for(;iter != m_VideoGameSteps.end();++iter)
		if((*iter)) { SafeDelete((*iter)); }

	m_VideoGameSteps.clear();
	m_UserDatas.clear();
}

/// 清空所有数据
void CVideoPlaybackManager::Clear(void)
{
	m_mySelfID = 0;
	m_curFrameIndex = 0;
	m_VideoState = VIDEO_NULL;

	ClearAllFrames();
}

/// 得到指定索引的帧
VideoGameStep* CVideoPlaybackManager::GetFrame(int index)
{
	if(index < 0 || index >= (int)m_VideoGameSteps.size())
		return NULL;

	return m_VideoGameSteps[index];
}

/// 得到指定索引的玩家
VPPlayer* CVideoPlaybackManager::GetUser(int index)
{
	if(index < 0 || index >= (int)m_UserDatas.size())
		return NULL;

	return &m_UserDatas[index];
}

/// 载入记录文件
bool CVideoPlaybackManager::Load(std::string filename)
{
	if(filename.empty()) return false;

	ClearAllFrames();

	//string_replace(filename,"\\","\\\\");

	// 建立一个包文件
	FILE* m_File = fopen(filename.c_str(),"rb");
	if(!m_File)	return false;

	FileHeader m_Header;
	if(fread(&m_Header,1,sizeof(FileHeader),m_File) != sizeof(FileHeader))
	{
		fclose(m_File);
		return false;
	}

	m_mySelfID = m_Header.useMe;
	m_hDllGameName = m_Header.gamelogic;

	// 读取房间信息
	if(fread(&m_mySelfRoom,1,sizeof(VPRoom),m_File) != sizeof(VPRoom))
	{
		fclose(m_File);
		return false;
	}

	for(int i=0;i<m_Header.wUserCount;i++)
	{
		VPPlayer pUserDate;
		if(fread(&pUserDate,1,sizeof(VPPlayer),m_File) != sizeof(VPPlayer))
		{
			fclose(m_File);
			return false;
		}

		m_UserDatas.push_back(pUserDate);
	}

	for(int i=0;i<m_Header.wFrameCount;i++)
	{
		VideoGameStep pVideoGameStep;
		if(fread(&pVideoGameStep,1,sizeof(VideoGameStep),m_File) != sizeof(VideoGameStep))
		{
			fclose(m_File);
			return false;
		}

		m_VideoGameSteps.push_back(new VideoGameStep(pVideoGameStep.wTimeSpace,pVideoGameStep.wBuffer,pVideoGameStep.wDataSize));
	}

    fclose(m_File);

	return true;
}

/// 保存到文件
bool CVideoPlaybackManager::Sava(std::string filename)
{
	if(filename.empty()) return false;

	string_replace(filename,"\\","\\\\");

	// 建立一个包文件
	FILE* m_File = fopen(filename.c_str(),"wb");
	if(!m_File)	return false;

	FileHeader m_Header;
	m_Header.marker[0] = 'X';
	m_Header.marker[1] = 'P';
	m_Header.marker[2] = 'V';
	m_Header.marker[3] = '0';
	m_Header.marker[4] = 'D';
	m_Header.useMe = m_mySelfID;

	strncpy(m_Header.gamelogic,m_hDllGameName.c_str(),CountArray(m_Header.gamelogic));

	m_Header.wUserCount = (int)m_UserDatas.size();
	m_Header.wFrameCount = (int)m_VideoGameSteps.size();

	// 写入头文件
	if(fwrite(&m_Header,1,sizeof(FileHeader),m_File) != sizeof(FileHeader))
	{
		fclose(m_File);
		return false;
	}

	// 写房间
	if(fwrite(&m_mySelfRoom,1,sizeof(VPRoom),m_File) != sizeof(VPRoom))
	{
		fclose(m_File);
		return false;
	}

	// 写玩家数据
	for(int i=0;i<(int)m_UserDatas.size();i++)
	{
		if(fwrite(&m_UserDatas[i],1,sizeof(VPPlayer),m_File) != sizeof(VPPlayer))
		{
			fclose(m_File);
			return false;
		}
	}

	// 写帧数据
	for(int i=0;i<(int)m_VideoGameSteps.size();i++)
	{
		if(fwrite(m_VideoGameSteps[i],1,sizeof(VideoGameStep),m_File) != sizeof(VideoGameStep))
		{
			fclose(m_File);
			return false;
		}
	}

    fclose(m_File);

	return true;
}
