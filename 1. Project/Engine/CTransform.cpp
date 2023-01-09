﻿#include "pch.h"
#include "CTransform.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CTransform::CTransform()
	: CComponent(COMPONENT_TYPE::TRANSFORM)
	, m_vRelativeScale(Vec3(1.f, 1.f, 1.f))
	, m_bIgnParentScale(false)
{
	m_vRelativeDir[(UINT)DIR::RIGHT]	= Vec3(1.f, 0.f, 0.f);
	m_vRelativeDir[(UINT)DIR::UP]		= Vec3(0.f, 1.f, 0.f);
	m_vRelativeDir[(UINT)DIR::FRONT]	= Vec3(0.f, 0.f, 1.f);
}

CTransform::~CTransform()
{
}



void CTransform::tick()
{

}

void CTransform::finaltick()
{
	// m_vRelativePos, m_vRelativeScale, m_vRelativeRotation
	// ㄴ=> 월드행렬을 만들어 냄
	m_matWorld = XMMatrixIdentity();

	Matrix matScale = XMMatrixScaling(m_vRelativeScale.x, m_vRelativeScale.y, m_vRelativeScale.z);

	Matrix matTrans = XMMatrixTranslation(m_vRelativePos.x, m_vRelativePos.y, m_vRelativePos.z);

	Matrix matRot = XMMatrixRotationX(m_vRelativeRotation.x);
	matRot *= XMMatrixRotationY(m_vRelativeRotation.y);
	matRot *= XMMatrixRotationZ(m_vRelativeRotation.z);

	// 회전행렬을 이용해서 현재 물체의 우, 상, 전 방향을 구해놓는다.
	m_vRelativeDir[(UINT)DIR::RIGHT] = m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), matRot);
	m_vRelativeDir[(UINT)DIR::UP]	 = m_vWorldDir[(UINT)DIR::UP]	 = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), matRot);
	m_vRelativeDir[(UINT)DIR::FRONT] = m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), matRot);

	m_matWorld = matScale * matRot * matTrans;

	// 부모 오브젝트가 있다면
	if (GetOwner()->GetParent())
	{
		// 부모 오브젝트의 크기를 무시하기로 한 경우
		if (m_bIgnParentScale)
		{
			// 위쪽으로 모든 부모의 크기값을 누적해서 역행렬을 만들어 둔다.
			CGameObject* pParent = GetOwner()->GetParent();
			Vec3 vParentScale = Vec3(1.f, 1.f, 1.f);

			while (pParent)
			{
				vParentScale *= pParent->Transform()->GetRelativeScale();

				// 부모 오브젝트도 그 위로 크기를 무시하기로 한 경우 크기를 더이상 누적할 필요가 없다.
				if (pParent->Transform()->m_bIgnParentScale)
					pParent = nullptr;
				else			
					pParent = pParent->GetParent();
			}

			// 부모 행렬의 크기부분을 역행렬을 구해서 상쇄시킨다.
			// 역행렬 예외처리
			if (0.f == vParentScale.x)
				vParentScale.x = 1.f;
			if (0.f == vParentScale.y)
				vParentScale.y = 1.f;
			if (0.f == vParentScale.z)
				vParentScale.z = 1.f;

			Matrix matParentScaleInv = XMMatrixScaling(vParentScale.x, vParentScale.y, vParentScale.z);
			matParentScaleInv = XMMatrixInverse(nullptr, matParentScaleInv);

			// 구한 역행렬을 미리 곱해두고 부모의 월드행렬을 곱한다.
			m_matWorld = m_matWorld * matParentScaleInv * GetOwner()->GetParent()->Transform()->GetWorldMat();			
		}
		else
		{
			m_matWorld *= GetOwner()->GetParent()->Transform()->GetWorldMat();
		}	

		// WorldDir 구하기
		m_vWorldDir[(UINT)DIR::RIGHT] = XMVector3TransformNormal(Vec3(1.f, 0.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::UP]	  = XMVector3TransformNormal(Vec3(0.f, 1.f, 0.f), m_matWorld);
		m_vWorldDir[(UINT)DIR::FRONT] = XMVector3TransformNormal(Vec3(0.f, 0.f, 1.f), m_matWorld);

		// 회전, 크기 변환이 이루어졌기 때문에 변경된 크기를 초기화 하기 위해 Normalize 해준다.ㄴㄴ
		m_vWorldDir[(UINT)DIR::RIGHT].Normalize();
		m_vWorldDir[(UINT)DIR::UP].Normalize();
		m_vWorldDir[(UINT)DIR::FRONT].Normalize();
	}
}


void CTransform::UpdateData()
{
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::TRANSFORM);
		
	g_transform.matWorld = m_matWorld;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;
	g_transform.vScale = m_vRelativeScale;
	
	pCB->SetData(&g_transform);
	pCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
}

void CTransform::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE eType = GetType();
	fwrite(&eType, sizeof(COMPONENT_TYPE), 1, _File);

	fwrite(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fwrite(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fwrite(&m_bIgnParentScale, sizeof(bool), 1, _File);
}

void CTransform::LoadFromFile(FILE* _File)
{
	fread(&m_vRelativePos, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeScale, sizeof(Vec3), 1, _File);
	fread(&m_vRelativeRotation, sizeof(Vec3), 1, _File);
	fread(&m_bIgnParentScale, sizeof(bool), 1, _File);
}


void CTransform::SetRelativePosWorld(Vec3 _vPos)
{

}

Vec3 CTransform::GetWorldScale()
{
	// 위쪽으로 모든 부모의 크기값을 누적해서 역행렬을 만들어 둔다.	
	Vec3 vWorldScale = m_vRelativeScale;

	if (m_bIgnParentScale)
		return vWorldScale;

	CGameObject* pParent = GetOwner()->GetParent();
	while (pParent)
	{		
		vWorldScale *= pParent->Transform()->GetRelativeScale();

		// 부모 오브젝트도 그 위로 크기를 무시하기로 한 경우 크기를 더이상 누적할 필요가 없다.
		if (pParent->Transform()->m_bIgnParentScale)
			pParent = nullptr;
		else
			pParent = pParent->GetParent();
	}

	return vWorldScale;
}