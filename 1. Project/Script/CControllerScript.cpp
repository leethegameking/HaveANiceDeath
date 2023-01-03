#include "pch.h"
#include "CControllerScript.h"

#include "CScriptMgr.h"
#include "CUnitScript.h"

CControllerScript::CControllerScript()
	: CScript((UINT)SCRIPT_TYPE::CONTROLLERSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "SpeedX     ", &m_fSpeedX);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Jump Speed ", &m_fJumpSpeed);
}

CControllerScript::~CControllerScript()
{
}

void CControllerScript::tick()
{

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
		// 아랫 점프
		else
		{
			if (Rigidbody2D()->IsGround())
			{
				CUnitScript* pUnitScript = GetOwner()->GetScript<CUnitScript>();
				if (pUnitScript && HasBit(pUnitScript->GetStateBits(), UNIT_GROUND_PLATFORM, BIT_LEAST_ONE))
				{
					Rigidbody2D()->SetGround(false);

					pUnitScript->RemoveStateBits(UNIT_GROUND_PLATFORM | UNIT_GROUND);
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

