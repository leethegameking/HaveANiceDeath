#include "pch.h"
#include "CSoundPlay.h"

#include "CSoundMgr.h"

CSoundPlay::CSoundPlay()
	: CScript((int)SCRIPT_TYPE::SOUNDPLAY)
{
	m_strSoundPath.reserve(255);

	AddScriptParam(SCRIPT_PARAM::STRING, "SoundRelPath", (char*)m_strSoundPath.data());
	AddScriptParam(SCRIPT_PARAM::BOOL, "BGM", &m_bBGM);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Volume", &m_fVolume);
	AddScriptParam(SCRIPT_PARAM::INT, "Repeat", &m_iRepeat);
	AddScriptParam(SCRIPT_PARAM::BOOL, "End-Destroy", &m_bEndDestroy);
}

CSoundPlay::CSoundPlay(const CSoundPlay& _origin)
	: CScript(_origin)
	, m_strSoundPath(_origin.m_strSoundPath)
	, m_fVolume(_origin.m_fVolume)
	, m_iRepeat(_origin.m_iRepeat)
	, m_bBGM(_origin.m_bBGM)
	, m_bEndDestroy(_origin.m_bEndDestroy)
{
	m_strSoundPath.reserve(255);

	AddScriptParam(SCRIPT_PARAM::STRING, "SoundRelPath", (char*)m_strSoundPath.data());
	AddScriptParam(SCRIPT_PARAM::BOOL, "BGM", &m_bBGM);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Volume", &m_fVolume);
	AddScriptParam(SCRIPT_PARAM::INT, "Repeat", &m_iRepeat);
	AddScriptParam(SCRIPT_PARAM::BOOL, "End-Destroy", &m_bEndDestroy);
}

CSoundPlay::~CSoundPlay()
{
}

void CSoundPlay::begin()
{
	string tmp = m_strSoundPath.data();
	wstring strRelPath = StrToWstr(tmp);
	strRelPath.substr(0, strRelPath.length() - 2);

	if(m_bBGM)
	{
		CSoundMgr::GetInst()->PlayBGM(strRelPath, m_fVolume);
	}
	else
	{
		CSoundMgr::GetInst()->Play(strRelPath, m_iRepeat, m_fVolume, true);
	}
	
	if (m_bEndDestroy)
	{
		Destroy();
	}
}

void CSoundPlay::tick()
{
}

void CSoundPlay::BeginOverlap(CCollider2D* _pOther)
{
}

void CSoundPlay::Overlap(CCollider2D* _pOther)
{
}

void CSoundPlay::EndOverlap(CCollider2D* _pOther)
{
}

void CSoundPlay::SaveToFile(FILE* _pFile)
{
	string tmp;
	tmp = m_strSoundPath.data();

	CScript::SaveToFile(_pFile);
	SaveStringToFile(tmp, _pFile);
	fwrite(&m_fVolume, sizeof(float), 1, _pFile);
	fwrite(&m_iRepeat, sizeof(int), 1, _pFile);
	fwrite(&m_bBGM, sizeof(bool), 1, _pFile);
	fwrite(&m_bEndDestroy, sizeof(bool), 1, _pFile);
}

void CSoundPlay::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	LoadStringFromFile(m_strSoundPath, _pFile);
	fread(&m_fVolume, sizeof(float), 1, _pFile);
	fread(&m_iRepeat, sizeof(int), 1, _pFile);
	fread(&m_bBGM, sizeof(bool), 1, _pFile);
	fread(&m_bEndDestroy, sizeof(bool), 1, _pFile);
}



