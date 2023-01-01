#include "pch.h"
#include "CBlockScript.h"

#include <Engine/CGameObject.h>

CBlockScript::CBlockScript()
	:CScript((int)SCRIPT_TYPE::BLOCKSCRIPT)
{
}

CBlockScript::~CBlockScript()
{
}

void CBlockScript::tick()
{
}

void CBlockScript::SaveToFile(FILE* _pFile)
{
}

void CBlockScript::LoadFromFile(FILE* _pFile)
{
}

void CBlockScript::BeginOverlap(CCollider2D* _other)
{
	CGameObject* pColObj = _other->GetOwner();
	Vec3 vBlockScale = Transform()->GetWorldScale();
	Vec3 vBlockPos =  Transform()->GetWorldPos();

	if (pColObj->Rigidbody2D())
	{
		short sDir = pColObj->Rigidbody2D()->GetDir();

		if (sDir & RB_DOWN && vBlockPos.y + vBlockScale.y / 2.f < pColObj->Rigidbody2D()->GetPrevPos().y)
		{
			pColObj->Rigidbody2D()->SetGround(true);
			Vec3 test = Vec3(pColObj->Transform()->GetRelativePos().x, vBlockPos.y + vBlockScale.y / 2.f + 0.1f, 0.f);
			pColObj->Transform()->SetRelativePos( Vec3(pColObj->Transform()->GetRelativePos().x, pColObj->Transform()->GetRelativeScale().y /2.f + + vBlockPos.y + vBlockScale.y / 2.f + 0.1f, 0.f));
		}
	}

}

void CBlockScript::Overlap(CCollider2D* _other)
{
}

void CBlockScript::EndOverlap(CCollider2D* _other)
{
}


