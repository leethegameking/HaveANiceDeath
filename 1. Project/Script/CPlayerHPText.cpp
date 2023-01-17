#include "pch.h"
#include "CPlayerHPText.h"

#include "CPlayerMgr.h"
#include "CUnitScript.h"
#include <Engine/SimpleText.h>
#include <Engine/CRenderMgr.h>

CPlayerHPText::CPlayerHPText()
	: CTextScript((int)SCRIPT_TYPE::PLAYERHPTEXT)
	, m_bFirstTick(true)
{
}

CPlayerHPText::CPlayerHPText(const CPlayerHPText& _origin)
	: CTextScript(_origin)
	, m_bFirstTick(true)
{
}

CPlayerHPText::~CPlayerHPText()
{
}

void CPlayerHPText::begin()
{
}

void CPlayerHPText::tick()
{
	if (m_bFirstTick)
	{
		m_pUnitScr = CPlayerMgr::GetInst()->GetPlayerObj()->GetScript<CUnitScript>();
	}
	 
	tText tTx;
	tTx.strText = to_wstring((int)m_pUnitScr->GetUnitInfo().m_fHP) + L" / " +  to_wstring((int)m_pUnitScr->GetUnitInfo().m_fMaxHP);
	tTx.eFormat = TEXT_FORMAT::TEXT_PLAYER_HP;
	tTx.vWorldPos = Vec3(800.f + Transform()->GetWorldPos().x, 450.f - Transform()->GetWorldPos().y, 0.f);
	tTx.bScreenPos = true;

	SimpleText::GetInst()->AddDrawVec(tTx);
}

void CPlayerHPText::BeginOverlap(CCollider2D* _pOther)
{
}

void CPlayerHPText::Overlap(CCollider2D* _pOther)
{
}

void CPlayerHPText::EndOverlap(CCollider2D* _pOther)
{
}

void CPlayerHPText::SaveToFile(FILE* _pFile)
{
	CTextScript::SaveToFile(_pFile);
}

void CPlayerHPText::LoadFromFile(FILE* _pFile)
{
	CTextScript::LoadFromFile(_pFile);
}



