#include "Stdafx.h"
#include "SoundPlayer.h"
#include "MusicInterface.h"
#include "SexyAppBase.h"
#include <io.h>


CSoundPlayer::CSoundPlayer(Sexy::SexyAppBase *pGameApp)
{
	m_pGameApp = pGameApp;
	m_mapSound.clear();

	m_iSoundID = 1000;

	m_AppInfo.bMusic = GetPrivateProfileInt("Music", "Open", 1, "./SystemOption.ini");
	m_AppInfo.iPosMusic = GetPrivateProfileInt("Music", "Pos", 1000, "./SystemOption.ini");
	m_AppInfo.bSound = GetPrivateProfileInt("Sound", "Open", 1, "./SystemOption.ini");
	m_AppInfo.iPosSound = GetPrivateProfileInt("Sound", "Pos", 1000, "./SystemOption.ini");
	m_AppInfo.bSpeech = GetPrivateProfileInt("Speech", "Open", 1, "./SystemOption.ini");
	m_AppInfo.iPosSpeech = GetPrivateProfileInt("Speech", "Pos", 1000, "./SystemOption.ini");
	m_AppInfo.bMasterSwitch = GetPrivateProfileInt("PlaySound", "bPlay", 1, "./SystemOption.ini");

// 	ZeroMemory(&m_AppInfo, sizeof(m_AppInfo));
// 	m_AppInfo.bMusic = true;
// 	m_AppInfo.iPosMusic = 800;
// 	m_AppInfo.bSound = true;
// 	m_AppInfo.iPosSound = 1000;
// 	m_AppInfo.bSpeech = true;
// 	m_AppInfo.iPosSpeech = 1000;


	m_iMaxMusicNum = GetBackMusicNum();
	//播放音乐
	if (m_iMaxMusicNum > 0)
	{
		m_iCurrentMusicID = -1;
		int iMusicID = rand() % m_iMaxMusicNum;
		//PlayMusic(iMusicID);
	}

}

CSoundPlayer::~CSoundPlayer(void)
{

}

int CSoundPlayer::PlaySound(std::string str,double dVolume,int iInSoundID)
{
	std::map<std::string, int>::iterator pos;
	pos = m_mapSound.find(str);
	
	int iSoundID = 0;
	if (pos != m_mapSound.end())
	{
		iSoundID = pos->second;
	}
	else
	{
		if(iInSoundID != -1 && iInSoundID < 1000)
		{
			iSoundID = iInSoundID;
		}
		else
		{
			m_iSoundID++;
			iSoundID = m_iSoundID;
		}
		
		m_mapSound.insert(std::make_pair<std::string, int>(str, iSoundID)) ;

		m_pGameApp->mMusicInterface->LoadMusic(iSoundID, str);
	}
	m_pGameApp->mMusicInterface->PlayMusic(iSoundID, 0, true);
	m_pGameApp->mMusicInterface->SetSongVolume(iSoundID,dVolume);
	return iSoundID;
}

int CSoundPlayer::PlaySound( std::string str )
{
	if (m_AppInfo.bSound  && m_pGameApp->mMusicInterface && m_AppInfo.bMasterSwitch)
	{
		return (PlaySound(str,(double)m_AppInfo.iPosSound / 1000.0f));
	}
	else
	{
		return -1;
	}
}

void CSoundPlayer::PlaySound( std::string str,int iInSoundID )
{
	if (m_AppInfo.bSound  && m_pGameApp->mMusicInterface && m_AppInfo.bMasterSwitch)
	{
		std::map<int, std::string>::iterator pos;
		pos = m_mapSoundID.find(iInSoundID);
		if (pos == m_mapSoundID.end())
		{
			m_mapSoundID.insert(std::make_pair<int, std::string>(iInSoundID, str)) ;
			m_pGameApp->mMusicInterface->LoadMusic(iInSoundID, str);
		}

		m_pGameApp->mMusicInterface->PlayMusic(iInSoundID, 0, true);
		m_pGameApp->mMusicInterface->SetSongVolume(iInSoundID,(double)m_AppInfo.iPosSound / 1000.0f);
	}

}

void CSoundPlayer::PlayMusic( int theId )
{
	if (theId<0 && theId>=m_iMaxMusicNum)
		return;
	string str = Sexy::StrFormat("Res\\RatiOx\\Music\\%d.mp3",theId);
	PlayMusic(str,theId);
}

void CSoundPlayer::PlayMusic( std::string str,int iInSoundID /*= -1*/ )
{
	if(m_pGameApp->mMusicInterface == NULL || !m_AppInfo.bMusic || !m_AppInfo.bMasterSwitch)
		return;
	if(m_iCurrentMusicID != iInSoundID && m_iCurrentMusicID != -1)
	{
		m_pGameApp->mMusicInterface->StopMusic(m_iCurrentMusicID);
	}
	m_iCurrentMusicID = iInSoundID;

	PlaySound(str,(double)m_AppInfo.iPosMusic / 1000.0f,iInSoundID);
}


void CSoundPlayer::StopMusic()
{
	if(m_iCurrentMusicID != -1)
	{
		m_pGameApp->mMusicInterface->StopMusic(m_iCurrentMusicID);
	}
}

void CSoundPlayer::PlaySpeech( std::string str )
{
	if (m_AppInfo.bSpeech  && m_pGameApp->mMusicInterface && m_AppInfo.bMasterSwitch)
	{
		PlaySound(str,(double)m_AppInfo.iPosSpeech / 1000.0f);
	}
}

int CSoundPlayer::GetBackMusicNum()
{
	int iNum = 0;
	std::string strTemp;
	while(1)
	{
		strTemp = Sexy::StrFormat("Res\\car\\Music\\%d.ogg",iNum);
		if(_access(strTemp.c_str(),0) != 0)//找不到文件了.跳出.
		{
			break;
		}
		iNum++;
	}
	return iNum;
}

void CSoundPlayer::UpdateMusic()
{
	//背景音乐(自动播放下一首)
	if (m_AppInfo.bMusic && m_iMaxMusicNum > 0 && !m_pGameApp->mMusicInterface->IsPlaying(m_iCurrentMusicID) && m_AppInfo.bMasterSwitch)
	{
		int iMusicID = m_iCurrentMusicID;
		//iMusicID++;(不播放下一首)
		if (iMusicID == m_iMaxMusicNum)
		{
			iMusicID = 0;
		}
		PlayMusic(iMusicID);
	}
}

void CSoundPlayer::SetSongVolume( const AppInfoDef *pAppInfo, int iPos)
{
	if (pAppInfo->bMusic != m_AppInfo.bMusic || pAppInfo->iPosMusic != m_AppInfo.iPosMusic)
	{
		m_AppInfo.bMusic = pAppInfo->bMusic;
		m_AppInfo.iPosMusic = pAppInfo->iPosMusic;
		m_pGameApp->mMusicInterface->SetSongVolume(m_iCurrentMusicID, m_AppInfo.iPosMusic/1000.0);
	}
	else if (pAppInfo->bSound != m_AppInfo.bSound || pAppInfo->iPosSound != m_AppInfo.iPosSound)
	{
		m_AppInfo.bSound = pAppInfo->bSound;
		m_AppInfo.iPosSound = pAppInfo->iPosSound;
		
	}
	else if (pAppInfo->bSpeech != m_AppInfo.bSpeech || pAppInfo->iPosSpeech != m_AppInfo.iPosSpeech)
	{
		m_AppInfo.bSpeech = pAppInfo->bSpeech;
		m_AppInfo.iPosSpeech = pAppInfo->iPosSpeech;
	}
	if (iPos > 0)
	{
		PlaySound(".\\Res\\Frame\\Sound\\Test.ogg", iPos/1000.0f, 10000);
	}
}

void CSoundPlayer::EnableSound( bool bEnable )
{
	m_AppInfo.bMasterSwitch = bEnable;
	if (m_AppInfo.bMasterSwitch)
	{
		m_pGameApp->mMusicInterface->SetSongVolume(m_iCurrentMusicID, m_AppInfo.iPosMusic/1000.0);
	}
	else
	{
		m_pGameApp->mMusicInterface->SetSongVolume(m_iCurrentMusicID, 0.0);
	}	
}


