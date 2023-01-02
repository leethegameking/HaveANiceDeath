#include "pch.h"
#include "CControllerScript.h"

CControllerScript::CControllerScript()
	: CScript((UINT)SCRIPT_TYPE::CONTROLLERSCRIPT)
{
}

CControllerScript::~CControllerScript()
{
}

void CControllerScript::tick()
{
	if (KEY_PRESSED(KEY::A))
	{
		Rigidbody2D()->SetForceSpeedX(-100.f);
	}
	else if (KEY_PRESSED(KEY::D))
	{
		Rigidbody2D()->SetForceSpeedX(100.f);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		// Rigidbody2D()->SetGround(false);
		Rigidbody2D()->AddForce(Vec2(0.f, 100000.f));
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
}

void CControllerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}

