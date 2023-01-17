#include "pch.h"
#include "CMouseTextScript.h"

#include <Engine/CDevice.h>
#include <Engine/SimpleText.h>
#include <Engine/CKeyMgr.h>

CMouseTextScript::CMouseTextScript()
	: CScript(-1)
	, m_bVisible(true)
{
}

CMouseTextScript::~CMouseTextScript()
{
}

void CMouseTextScript::begin()
{
}

void CMouseTextScript::tick()
{
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::N_8))
		m_bVisible = !m_bVisible;

	if (m_bVisible)
	{
	 	Vec2 vMouseWorldPos = CKeyMgr::GetInst()->GetMouseWorldPos();
		wstring strMousePos = StrToWstr(to_string((int)vMouseWorldPos.x)) + L"/" + StrToWstr(to_string((int)vMouseWorldPos.y));

		tText tTx;
		tTx.eFormat = TEXT_FORMAT::TEXT_MOUSE_POS;
		tTx.strText = strMousePos;
		tTx.vWorldPos = vMouseWorldPos;

		SimpleText::GetInst()->AddDrawVec(tTx);
	}
}




