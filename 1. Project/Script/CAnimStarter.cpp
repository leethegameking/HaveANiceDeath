#include "pch.h"
#include "CAnimStarter.h"

CAnimStarter::CAnimStarter()
	: CScript((int)SCRIPT_TYPE::ANIMSTARTER)
	, m_strAnimKey()
	, m_bOneAnim(true)
	, m_bRepeat(false)
	, m_bEndDestory(false)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Has One Animation ", &m_bOneAnim);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Repeat            ", &m_bRepeat);
	AddScriptParam(SCRIPT_PARAM::BOOL, "End Destory       ", &m_bEndDestory);
}

CAnimStarter::CAnimStarter(const CAnimStarter& _origin)
	: CScript(_origin)
	, m_strAnimKey(_origin.m_strAnimKey)
	, m_bOneAnim(_origin.m_bOneAnim)
	, m_bRepeat(_origin.m_bRepeat)
	, m_bEndDestory(_origin.m_bEndDestory)
{
	AddScriptParam(SCRIPT_PARAM::BOOL, "Has One Animation ", &m_bOneAnim);
	AddScriptParam(SCRIPT_PARAM::BOOL, "Repeat            ", &m_bRepeat);
	AddScriptParam(SCRIPT_PARAM::BOOL, "End Destory       ", &m_bEndDestory);
}

CAnimStarter::~CAnimStarter()
{
}

void CAnimStarter::begin()
{
	if (m_bOneAnim)
	{
		Animator2D()->Play(Animator2D()->GetAnimMap().begin()->first, m_bRepeat);
	}
}

void CAnimStarter::tick()
{
	if (!m_bRepeat && m_bEndDestory)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
			Destroy();
	}
}

void CAnimStarter::BeginOverlap(CCollider2D* _pOther)
{
}

void CAnimStarter::Overlap(CCollider2D* _pOther)
{
}

void CAnimStarter::EndOverlap(CCollider2D* _pOther)
{
}

void CAnimStarter::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	SaveWStringToFile(m_strAnimKey, _pFile);
	fwrite(&m_bOneAnim, sizeof(bool), 1, _pFile);
	fwrite(&m_bRepeat, sizeof(bool), 1, _pFile);
	fwrite(&m_bEndDestory, sizeof(bool), 1, _pFile);
}

void CAnimStarter::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	LoadWStringFromFile(m_strAnimKey, _pFile);
	fread(&m_bOneAnim, sizeof(bool), 1, _pFile);
	fread(&m_bRepeat, sizeof(bool), 1, _pFile);
	fread(&m_bEndDestory, sizeof(bool), 1, _pFile);
}


