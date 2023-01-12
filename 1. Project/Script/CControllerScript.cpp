#include "pch.h"
#include "CControllerScript.h"

#include "CScriptMgr.h"
#include "CUnitScript.h"
#include "CAnimController.h"

CControllerScript::CControllerScript()
	: CScript((UINT)SCRIPT_TYPE::CONTROLLERSCRIPT)
	, m_fAccTime(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpeedX     ", &m_fSpeedX);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Jump Speed ", &m_fJumpSpeed);
}

CControllerScript::CControllerScript(const CControllerScript& _origin)
	: CScript(_origin)
	, m_fSpeedX(_origin.m_fSpeedX)
	, m_fJumpSpeed(_origin.m_fJumpSpeed)
	, m_fAccTime(0.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpeedX     ", &m_fSpeedX);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Jump Speed ", &m_fJumpSpeed);
}

CControllerScript::~CControllerScript()
{
}

void CControllerScript::begin()
{
	m_sAnimCtrl = GetOwner()->GetScript<CAnimController>();
}

void CControllerScript::tick()
{
	if (CalBit(m_sAnimCtrl->GetCurAnimNode()->iPreferences, NO_MOVE, BIT_INCLUDE))
		return;

	if (KEY_PRESSED(KEY::A))
	{
		Rigidbody2D()->SetForceSpeedX(-m_fSpeedX);
	}
	else if (KEY_PRESSED(KEY::D))
	{
		Rigidbody2D()->SetForceSpeedX(m_fSpeedX);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		if (!KEY_PRESSED(KEY::S))
		{
			if (Rigidbody2D()->IsGround())
			{
				Rigidbody2D()->SetForceSpeedY(m_fJumpSpeed);
			}
		}
		// down jump
		else
		{
			if (Rigidbody2D()->IsGround())
			{
				CUnitScript* pUnitScript = GetOwner()->GetScript<CUnitScript>();
				if (pUnitScript && CalBit(UNIT_GROUND_PLATFORM, pUnitScript->GetUnitState(), BIT_EQUAL_SELECTED_BIT, UNIT_GROUND | UNIT_GROUND_PLATFORM))
				{
					Rigidbody2D()->SetGround(false);

					pUnitScript->RemoveUnitState( UNIT_GROUND_PLATFORM );
				}
			}
		}
	}
}
 
void CControllerScript::BeginOverlap(CCollider2D* _other)
{
}

void CControllerScript::Overlap(CCollider2D* _other)
{
}

void CControllerScript::EndOverlap(CCollider2D* _other)
{
}

void CControllerScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&m_fSpeedX, sizeof(float), 1, _pFile);
	fwrite(&m_fJumpSpeed, sizeof(float), 1, _pFile);
}

void CControllerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&m_fSpeedX, sizeof(float), 1, _pFile);
	fread(&m_fJumpSpeed, sizeof(float), 1, _pFile);
}

