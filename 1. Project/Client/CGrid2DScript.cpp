﻿#include "pch.h"
#include "CGrid2DScript.h"

#include <Engine\CDevice.h>
#include <Engine\CRenderMgr.h>
#include <Engine\CTransform.h>
#include <Engine\CCamera.h>
#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>


CGrid2DScript::CGrid2DScript()
	: CScript(-1)
	, m_pMainCam(nullptr)
	, m_bDraw(true)
{
}

CGrid2DScript::~CGrid2DScript()
{
}

void CGrid2DScript::begin()
{
}

void CGrid2DScript::tick()
{
	if (KEY_PRESSED(KEY::LCTRL) && KEY_TAP(KEY::N_0))
	{
		m_bDraw = !m_bDraw;
	}


	m_pMainCam = CRenderMgr::GetInst()->GetMainCam();

	if (m_pMainCam)
	{
		if (PROJ_TYPE::PERSPECTIVE == m_pMainCam->GetProjType())
		{
			MeshRender()->Deactivate();
		}
		else
		{
			if(m_bDraw)
				MeshRender()->Activate();
			else
				MeshRender()->Deactivate();


			Vec3 vCamWorldPos = m_pMainCam->Transform()->GetRelativePos();
			float fScale = m_pMainCam->GetOrthographicScale();
			Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

			MeshRender()->GetCurMaterial()->SetScalarParam(VEC2_0, &vRenderResolution);
			MeshRender()->GetCurMaterial()->SetScalarParam(VEC4_0, &vCamWorldPos);
			MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_0, &fScale);
			MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_1, &m_fThickness);
			MeshRender()->GetCurMaterial()->SetScalarParam(FLOAT_2, &m_fGridInterval);
			MeshRender()->GetCurMaterial()->SetScalarParam(VEC4_1, &m_vColor);
		}
	}

}
