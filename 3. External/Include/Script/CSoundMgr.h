#pragma once

class CSound;

class CSoundMgr
	: public CSingleton<CSoundMgr>
{
	SINGLETON(CSoundMgr)

private:
	Ptr<CSound> m_BGM;

	Ptr<CSound> m_PlayerATK;
	Ptr<CSound> m_PlayerMumble;

	Ptr<CSound> m_BossATK;
	Ptr<CSound> m_BossMumble;


public:
	int Play(const wstring& _relPath, int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);

	int PlayBGM(const wstring& _relPath, float _fVolume = 1.f);

	int PlayPlayerATK(const wstring& _relPath, int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
	int PlayPlayerMumble(const wstring& _relPath, int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);

	int PlayBossATK(const wstring& _relPath, int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
	int PlayBossMumble(const wstring& _relPath, int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
};

