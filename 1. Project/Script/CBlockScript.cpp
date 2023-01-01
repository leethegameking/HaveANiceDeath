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
			pColObj->Transform()->SetRelativePos(pColObj->Transform()->GetRelativePos() + Vec3(vBlockPos.y + vBlockScale.y / 2.f + 0.1f, 0.f, 0.f));
		}
	}

}

void CBlockScript::Overlap(CCollider2D* _other)
{
}

void CBlockScript::EndOverlap(CCollider2D* _other)
{
}


