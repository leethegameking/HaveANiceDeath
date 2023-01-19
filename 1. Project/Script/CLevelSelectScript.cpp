#include "pch.h"
#include "CLevelSelectScript.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CObjectManager.h>

CLevelSelectScript::CLevelSelectScript()
	: CScript((int)SCRIPT_TYPE::LEVELSELECTSCRIPT)
	, m_strLevelPath()
	, m_bFirstTick(true)
	, m_bLevelSelected(false)
{
}

CLevelSelectScript::CLevelSelectScript(const CLevelSelectScript& _origin)
	: CScript(_origin)
	, m_strLevelPath()
	, m_bFirstTick(true)
	, m_bLevelSelected(false)
{
}

CLevelSelectScript::~CLevelSelectScript()
{
}


void CLevelSelectScript::begin()
{
	m_bFirstTick = true;
	m_bLevelSelected = false;
}

void CLevelSelectScript::tick()
{

	if (m_bFirstTick)
	{
		m_pBossIcon = CObjectManager::GetInst()->FindObjByName(L"Boss_Icon");
		m_bFirstTick = false;
	}
}

void CLevelSelectScript::BeginOverlap(CCollider2D* _pOther)
{
	int iTexNumber = 1;
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &iTexNumber);

	m_pBossIcon->Transform()->AddRelativePos(Vec3(5.f, 0.f, 0.f));
}

void CLevelSelectScript::Overlap(CCollider2D* _pOther)
{
	if (KEY_TAP(KEY::LBTN))
	{
		m_bLevelSelected = true;
		/*tEvent evn;
		evn.eType = EVENT_TYPE::CHANGE_LEVEL;
		evn.wParam = (DWORD_PTR)strLevelPath.data();
		CEventMgr::GetInst()->AddEvent(evn);

		evn.eType = EVENT_TYPE::CHANGE_LEVEL_STATE;
		evn.wParam = (DWORD_PTR)LEVEL_STATE::PLAY;
		CEventMgr::GetInst()->AddEvent(evn);*/
	}
}

void CLevelSelectScript::EndOverlap(CCollider2D* _pOther)
{
	int iTexNumber = 0;
	MeshRender()->GetCurMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &iTexNumber);

	m_pBossIcon->Transform()->AddRelativePos(Vec3(-5.f, 0.f, 0.f)); // 축소되는 이미지 맞추기용.
}

void CLevelSelectScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	SaveStringToFile(m_strLevelPath, _pFile);
	
}

void CLevelSelectScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	LoadStringFromFile(m_strLevelPath, _pFile);
}


