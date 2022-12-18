#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
	m_Info.iLightType = LIGHT_TYPE::POINT;
}

CLight2D::~CLight2D()
{
}

#include "CKeyMgr.h"
#include "CTimeMgr.h"
void CLight2D::finaltick()
{
	if (KEY_PRESSED(KEY::L))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * (XM_PI / 2.f);
		Transform()->SetRelativeRotation(vRot);
	}


	m_Info.vWorldPos = Transform()->GetWorldPos();
	m_Info.vWorldDir = Transform()->GetWorldDir(DIR::RIGHT);

	CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE eType = GetType();
	fwrite(&eType, sizeof(COMPONENT_TYPE), 1, _File);

	fwrite(&m_Info, sizeof(tLightInfo), 1, _File);
}

void CLight2D::LoadFromFile(FILE* _File)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _File);
}
