﻿#include "pch.h"
#include "CCameraScript.h"

#include <Engine/CDevice.h>


CCameraScript::CCameraScript()
	: CScript(-1)
	, m_fSpeed(300.f)
{
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::begin()
{
}

void CCameraScript::tick()
{	
	Move();
}

void CCameraScript::Move()
{
	if (!CDevice::GetInst()->IsWindowFocused())
		return;

	Vec3 vPos = Transform()->GetRelativePos();

	float fSpeed = m_fSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		fSpeed *= 5.f;
	}

	if (Camera()->GetProjType() == PROJ_TYPE::PERSPECTIVE)
	{
		Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);
		Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);

		if (KEY_PRESSED(KEY::W))
			vPos += Edit_DT * vFront * fSpeed;
		if (KEY_PRESSED(KEY::S))
			vPos += Edit_DT * -vFront * fSpeed;
		if (KEY_PRESSED(KEY::A))
			vPos += Edit_DT * -vRight * fSpeed;
		if (KEY_PRESSED(KEY::D))
			vPos += Edit_DT * vRight * fSpeed;

		Vec2 vMouseDir = CKeyMgr::GetInst()->GetMouseDir();


		if (KEY_PRESSED(KEY::RBTN))
		{
			Vec3 vRot = Transform()->GetRelativeRotation();

			vRot.y += vMouseDir.x * Edit_DT * XM_PI;
			vRot.x -= vMouseDir.y * Edit_DT * XM_PI;
			Transform()->SetRelativeRotation(vRot);
		}

		Transform()->SetRelativePos(vPos);
	}

	else
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		if (vRot.x != 0.f)
			vRot.x = 0.f;
		if (vRot.y != 0.f)
			vRot.y = 0.f;
		
		if (KEY_PRESSED(KEY::W))
			vPos.y += Edit_DT * fSpeed;
		if (KEY_PRESSED(KEY::S))
			vPos.y -= Edit_DT * fSpeed;
		if (KEY_PRESSED(KEY::A))
			vPos.x -= Edit_DT * fSpeed;
		if (KEY_PRESSED(KEY::D))
			vPos.x += Edit_DT * fSpeed;

		if (vPos != Transform()->GetRelativePos())
			Transform()->SetRelativePos(vPos);		

		if (KEY_PRESSED(KEY::N))
		{
			float fScale = Camera()->GetOrthographicScale();
			fScale += Edit_DT;
			Camera()->SetOrthographicScale(fScale);
		}
		else if (KEY_PRESSED(KEY::M))
		{
			float fScale = Camera()->GetOrthographicScale();
			fScale -= Edit_DT;
			Camera()->SetOrthographicScale(fScale);
		}
	}	
}
