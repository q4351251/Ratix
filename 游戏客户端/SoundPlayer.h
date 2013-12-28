#pragma once

#include <map>

namespace Sexy
{
	class SexyAppBase;
}



/*
//声音播放类
新增了直接用文件名播放声音的方法
*/
class CSoundPlayer
{
public:
	CSoundPlayer(Sexy::SexyAppBase *pGameApp);
	~CSoundPlayer(void);

	//参数一：声音文件路径  参数二：声音大小0~1 参数三：指定声音ID，必须小于1000，用于区分背景音乐
	int PlaySound(std::string str,double dVolume,int iInSoundID = -1);

	void PlayMusic(int theId);										//播放音乐
	void PlayMusic(std::string str,int iInSoundID = -1);			//背景音
	void StopMusic();												//关闭背景音
	int  PlaySound(std::string str);								//音效
	void PlaySpeech(std::string str);								//人物音
	void PlaySound(std::string str,int iInSoundID);					//音效


	int GetBackMusicNum();											//获得背景音乐的数目
	void UpdateMusic();												//持续播放背景音

	void SetSongVolume(const AppInfoDef *pAppInfo, int iPos);					//设置声音
	void EnableSound(bool bEnable);
private:
	int m_iSoundID;

	int m_iCurrentMusicID;											//当前播放背景音乐的ID
	int m_iMaxMusicNum;												//背景音乐数量

	Sexy::SexyAppBase *m_pGameApp;

	//游戏配置
	AppInfoDef			m_AppInfo;
	
	
	std::map<std::string, int> m_mapSound;
	std::map<int, std::string> m_mapSoundID;						//多个ID同一个路径

};
