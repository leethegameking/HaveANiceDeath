#include "pch.h"
#include "CCamera.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CRenderMgr.h"

#include "CDevice.h"
#include "CRenderMgr.h"
#include "CTransform.h"
#include "CRenderComponent.h"
#include "CMaterial.h"

#include "CGameObject.h"
#include "CMeshRender.h"


CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_eProjType(PROJ_TYPE::ORTHOGRAHPICS)
	, m_fAspectRatio(1.f)
	, m_fFar(100000.f)
	, m_fScale(1.f)
	, m_iLayerMask(0)
	, m_iCamIdx(0)
	, m_bRegister(true)
{
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();
	m_fAspectRatio = vRenderResolution.x / vRenderResolution.y;
}

CCamera::~CCamera()
{
}

void CCamera::finaltick()
{
	CalcViewMat();

	CalcProjMat();

	// 카메라 등록
	if(m_bRegister)
		CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::render()
{
	// 행렬 세팅
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;

	// Shader Domain 에 따른 물체 분류
	SortObject();
	render_far_back();

	// Domain 분류에 따른 렌더링
	render_back();
	render_block();
	render_middle();
	render_opaque();
	render_mask();
	render_transparent();
	render_postprocess();


	render_front();
	render_far_front();

	render_ui();
}


void CCamera::SetLayerVisible(const wstring& _strLayerName)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CLayer* pLayer = pCurLevel->GetLayer(_strLayerName);
	assert(pLayer);

	SetLayerVisible(pLayer->GetLayerIdx());
}

void CCamera::SetLayerVisible(int _iLayerIdx)
{
	m_iLayerMask |= (1 << _iLayerIdx);
}

void CCamera::SetLayerInvisible(int _iLayerIdx)
{
	m_iLayerMask &= ~(1 << _iLayerIdx);
}



void CCamera::CalcViewMat()
{
	// =============
	// View 행렬 계산
	// =============
	// View 이동행렬 ( 카메라를 원점으로 이동하는 만큼 )
	Vec3 vPos = Transform()->GetRelativePos();
	Matrix matViewTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	// View 회전 행렬 ( 카메라가 보는 전방 방향을 z 축을 보도록 돌리는 만큼 )
	Vec3 vRight = Transform()->GetRelativeDir(DIR::RIGHT);
	Vec3 vUp = Transform()->GetRelativeDir(DIR::UP);
	Vec3 vFront = Transform()->GetRelativeDir(DIR::FRONT);

	Matrix matViewRot = XMMatrixIdentity();
	matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

	m_matView = matViewTrans * matViewRot;
}

void CCamera::CalcProjMat()
{
	// =============
	// 투영 행렬 계산
	// =============
	Vec2 vRenderResolution = CDevice::GetInst()->GetRenderResolution();

	if (PERSPECTIVE == m_eProjType)
	{
		// 원근투영
		m_matProj = XMMatrixPerspectiveFovLH(XM_2PI / 6.f, m_fAspectRatio, 1.f, m_fFar);
	}
	else
	{
		// 직교투영
		m_matProj = XMMatrixOrthographicLH(vRenderResolution.x * m_fScale, vRenderResolution.y * m_fScale, 1.f, m_fFar);
	}
}

void CCamera::SortObject()
{
	ClearVec();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer 확인
		if (m_iLayerMask & (1 << i))
		{
			// 해당 레이어에 속한 게임오브젝트를 가져온다.
			CLayer* pLayer = pCurLevel->GetLayer(i);


			// 오브젝트들을 쉐이더 도메인에 따라 분류한다.
			const vector<CGameObject*>& vecObj = pLayer->GetObjects();

			for (size_t j = 0; j < vecObj.size(); ++j)
			{
				// 카메라 안에 들어오는 것만 렌더링 
				if (!InCamera(vecObj[j], this, Vec2(1.2f, 1.2f)))
					continue;

				CRenderComponent* pRenderCom = vecObj[j]->GetRenderComponent();

				if (   nullptr == pRenderCom
					|| nullptr == pRenderCom->GetMesh()
					|| nullptr == pRenderCom->GetCurMaterial()
					|| nullptr == pRenderCom->GetCurMaterial()->GetShader())
				{
					continue;
				}
				
				Ptr<CGraphicsShader> pShader = pRenderCom->GetCurMaterial()->GetShader();

				SHADER_DOMAIN eDomain = pShader->GetDomain();

				switch (eDomain)
				{
				case SHADER_DOMAIN::DOMAIN_OPAQUE:
					m_vecQpaque.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MASK:
					m_vecMask.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_TRANSPARENT:
					m_vecTransparent.push_back(vecObj[j]);
					break;			
				case SHADER_DOMAIN::DOMAIN_POST_PROCESS:
					m_vecPostProcess.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_BLOCK:
					m_vecBlock.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_FAR_BACK:
					m_vecFarBack.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_BACK:
					m_vecBack.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_MIDDLE:
					m_vecMiddle.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_FRONT:
					m_vecFront.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_FAR_FRONT:
					m_vecFarFront.push_back(vecObj[j]);
					break;
				case SHADER_DOMAIN::DOMAIN_UI:
					m_vecUI.push_back(vecObj[j]);
					break;
				}
			}
		}
	}
}

void CCamera::ClearVec()
{
	m_vecQpaque.clear();
	m_vecMask.clear();
	m_vecTransparent.clear();
	m_vecPostProcess.clear();
	m_vecBlock.clear();
	m_vecFarBack.clear();
	m_vecBack.clear();
	m_vecMiddle.clear();
	m_vecFront.clear();
	m_vecFarFront.clear();
	m_vecUI.clear();
}

void CCamera::render_opaque()
{
	for (size_t i = 0; i < m_vecQpaque.size(); ++i)
	{
		m_vecQpaque[i]->render();
	}
}

void CCamera::render_mask()
{
	for (size_t i = 0; i < m_vecMask.size(); ++i)
	{
		m_vecMask[i]->render();
	}
}

void CCamera::render_transparent()
{
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->render();
	}
}

void CCamera::render_postprocess()
{
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->CopyRenderTarget();
		m_vecPostProcess[i]->render();
	}
}

void CCamera::render_block()
{
	static vector<CGameObject*> vecTransition;
	static vector<CGameObject*> vecBorder;
	static vector<CGameObject*> vecFill_Platform;
	vecTransition.clear();
	vecBorder.clear();
	vecFill_Platform.clear();

	for (size_t i = 0; i < m_vecBlock.size(); ++i)
	{
		CMeshRender* pMeshrender = m_vecBlock[i]->MeshRender();
		tMtrlConst tConst = pMeshrender->GetCurMaterial()->GetMtrlConst();
		bool bDynamic = pMeshrender->IsDynamicMtrl();
		if (tConst.iArr[0] == 3)
		{
			vecBorder.push_back(m_vecBlock[i]);
		}
		else if (bDynamic && tConst.iArr[0] == 0 || tConst.iArr[0] == 1 || tConst.iArr[0] == 2 )
		{
			vecTransition.push_back(m_vecBlock[i]);
		}
		else
		{
			vecFill_Platform.push_back(m_vecBlock[i]);
		}
	}

	for (size_t i = 0; i < vecFill_Platform.size(); ++i)
	{
		vecFill_Platform[i]->render();
	}


	for (size_t i = 0; i < vecBorder.size(); ++i)
	{
		vecBorder[i]->render();
	}



	for (size_t i = 0; i < vecTransition.size(); ++i)
	{
		vecTransition[i]->render();
	}
}

void CCamera::render_far_back()
{
	for (size_t i = 0; i < m_vecFarBack.size(); ++i)
	{
		m_vecFarBack[i]->render();
	}
}

void CCamera::render_back()
{
	for (size_t i = 0; i < m_vecBack.size(); ++i)
	{
		m_vecBack[i]->render();
	}
}

void CCamera::render_middle()
{
	for (size_t i = 0; i < m_vecMiddle.size(); ++i)
	{
		m_vecMiddle[i]->render();
	}
}

void CCamera::render_front()
{
	for (size_t i = 0; i < m_vecFront.size(); ++i)
	{
		m_vecFront[i]->render();
	}
}

void CCamera::render_far_front()
{
	for (size_t i = 0; i < m_vecFarFront.size(); ++i)
	{
		m_vecFarFront[i]->render();
	}
}

void CCamera::render_ui()
{
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->render();
	}
}

void CCamera::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE eType = GetType();
	fwrite(&eType, sizeof(COMPONENT_TYPE), 1, _File);

	 fwrite(&m_eProjType, sizeof(PROJ_TYPE), 1, _File);
	 fwrite(&m_fAspectRatio, sizeof(float), 1, _File);
	 fwrite(&m_fFar, sizeof(float), 1, _File);
	 fwrite(&m_fScale, sizeof(float), 1, _File);
	 fwrite(&m_iLayerMask, sizeof(UINT), 1, _File);
	 fwrite(&m_iCamIdx, sizeof(int), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_eProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_fAspectRatio, sizeof(float), 1, _File);
	fread(&m_fFar, sizeof(float), 1, _File);
	fread(&m_fScale, sizeof(float), 1, _File);
	fread(&m_iLayerMask, sizeof(UINT), 1, _File);
	fread(&m_iCamIdx, sizeof(int), 1, _File);
}
