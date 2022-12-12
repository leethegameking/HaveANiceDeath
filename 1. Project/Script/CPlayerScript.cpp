#include "pch.h"
#include "CPlayerScript.h"


#include "CMissileScript.h"
#include <Engine/CMaterial.h>

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_fAccTime(0.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::begin()
{
	int a = 0;
}

void CPlayerScript::tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT) && KEY_PRESSED(KEY::LCTRL))
	{
		vPos.x -= 100.f * DT;
	}

	if (KEY_PRESSED(KEY::RIGHT) && KEY_PRESSED(KEY::LCTRL))
	{
		vPos.x += 100.f * DT;
	}

	if (KEY_PRESSED(KEY::UP) && KEY_PRESSED(KEY::LCTRL))
	{
		vPos.y += 100.f * DT;
	}

	if (KEY_PRESSED(KEY::DOWN) && KEY_PRESSED(KEY::LCTRL))
	{
		vPos.y -= 100.f * DT;
	}

	if (KEY_PRESSED(KEY::Y))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRotation(vRot);

		float fZRot = Collider2D()->GetRotationZ();
		fZRot += DT * XM_PI;
		Collider2D()->SetRotationZ(fZRot);
	}
	
	if(vPos != Transform()->GetRelativePos())
		Transform()->SetRelativePos(vPos);

	static bool bStart = false;
	if (KEY_TAP(KEY::SPACE))
	{
		// �÷��̾� ��ġ�� �̻��� ����
		Vec3 vMissilePos = Transform()->GetRelativePos();
		vMissilePos.y += Transform()->GetRelativeScale().y / 2.f;

		Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
		Instantiate(pMissilePrefab->Instantiate(), vMissilePos);

		DebugDrawCircle(Vec4(1.f, 0.2f, 0.2f, 1.f), vMissilePos, 50.f, 2.f);

		//Destroy();
		//GetOwner()->GetChildObject()[0]->SetDead();		
		bStart = true;
	}

	if (bStart)
	{
		m_fAccTime += DT;
		MeshRender()->GetDynamicMaterial()->SetScalarParam(FLOAT_0, &m_fAccTime);
	}	
}

void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{
	_pOther->Destroy();
}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}
