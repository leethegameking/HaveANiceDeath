#include "pch.h"
#include "CSoundMgr.h"

#include <Engine/CResMgr.h>
#include <Engine/CSound.h>

CSoundMgr::CSoundMgr()
{

}

CSoundMgr::~CSoundMgr()
{

}

int CSoundMgr::Play(const wstring& _relPath, int _iRoopCount, float _fVolume, bool _bOverlap)
{
	const map<wstring, Ptr<CRes>>& mapSound = CResMgr::GetInst()->GetResource(RES_TYPE::SOUND);
	 
	return ((CSound*)mapSound.find(_relPath)->second.Get())->Play(_iRoopCount, _fVolume, _bOverlap);
}

int CSoundMgr::PlayBGM(const wstring& _relPath, float _fVolume)
{
	if(m_BGM.Get())
		m_BGM->Stop();
	
	const map<wstring, Ptr<CRes>>& mapSound = CResMgr::GetInst()->GetResource(RES_TYPE::SOUND);
	m_BGM = ((CSound*)mapSound.find(_relPath)->second.Get());

	return m_BGM->Play(0, _fVolume);
}

int CSoundMgr::PlayPlayerATK(const wstring& _relPath, int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (m_PlayerATK.Get())
		m_PlayerATK->Stop();

	const map<wstring, Ptr<CRes>>& mapSound = CResMgr::GetInst()->GetResource(RES_TYPE::SOUND);
	m_PlayerATK = ((CSound*)mapSound.find(_relPath)->second.Get());

	return m_PlayerATK->Play(_iRoopCount, _fVolume);
}

int CSoundMgr::PlayPlayerMumble(const wstring& _relPath, int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (m_PlayerMumble.Get())
		m_PlayerMumble->Stop();

	const map<wstring, Ptr<CRes>>& mapSound = CResMgr::GetInst()->GetResource(RES_TYPE::SOUND);
	m_PlayerMumble = ((CSound*)mapSound.find(_relPath)->second.Get());

	return m_PlayerMumble->Play(_iRoopCount, _fVolume);
}

int CSoundMgr::PlayBossATK(const wstring& _relPath, int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (m_BossATK.Get())
		m_BossATK->Stop();

	const map<wstring, Ptr<CRes>>& mapSound = CResMgr::GetInst()->GetResource(RES_TYPE::SOUND);
	m_BossATK = ((CSound*)mapSound.find(_relPath)->second.Get());

	return m_BossATK->Play(_iRoopCount, _fVolume);
}

int CSoundMgr::PlayBossMumble(const wstring& _relPath, int _iRoopCount, float _fVolume, bool _bOverlap)
{
	if (m_BossMumble.Get())
		m_BossMumble->Stop();

	const map<wstring, Ptr<CRes>>& mapSound = CResMgr::GetInst()->GetResource(RES_TYPE::SOUND);
	m_BossMumble = ((CSound*)mapSound.find(_relPath)->second.Get());

	return m_BossMumble->Play(_iRoopCount, _fVolume);
}
