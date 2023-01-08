#include "pch.h"
#include "CEnemyScript.h"

#include "CPlayerMgr.h"


CEnemyScript::CEnemyScript()
{
}

CEnemyScript::CEnemyScript(int _iScriptType)
	: CUnitScript(_iScriptType)
	, m_ePattern(0)
{

}

CEnemyScript::CEnemyScript(const CEnemyScript& _origin)
	: CUnitScript(_origin)
	, m_ePattern(0)
{
}

CEnemyScript::~CEnemyScript()
{
}

void CEnemyScript::begin()
{
	CUnitScript::begin();
	m_pPlayerObj = CPlayerMgr::GetInst()->GetPlayerObj();
}

void CEnemyScript::tick()
{
	CUnitScript::tick();
}

void CEnemyScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CEnemyScript::Overlap(CCollider2D* _pOther)
{
}

void CEnemyScript::EndOverlap(CCollider2D* _pOther)
{
}

void CEnemyScript::AppearCheck()
{
	//if (m_bAppear == false)
	//{
	//	if (m_pPlayerObj->Transform()->GetRelativePos() < m_AppearRadius)
	//	{
	//		m_bAppear = true;
	//		m_ePattern = E_APPEAR;
	//	}
	//}
}

void CEnemyScript::DetectCheck()
{
	//if (m_bAppear == true && m_bDetect == false)
	//{
	//	if (m_pPlayerObj->Transform()->GetRelativePos() < m_DetectRadius)
	//	{
	//		m_bDetect = true;
	//		m_ePattern = E_DETECT;
	//	}
	//}
}

void CEnemyScript::SaveToFile(FILE* _pFile)
{
}

void CEnemyScript::LoadFromFile(FILE* _pFile)
{
}



