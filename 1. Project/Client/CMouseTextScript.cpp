#include "pch.h"
#include "CMouseTextScript.h"

#include <Engine/CDevice.h>

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
		
	}
}




