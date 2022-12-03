#include "pch.h"
#include "CLevelMgr.h"

#include "CResMgr.h"
#include "CCollisionMgr.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "GlobalComponent.h"


#include "CPlayerScript.h"
#include "CCameraScript.h"
#include "CGrid2DScript.h"
#include "CPaintShader.h"
#include "CParticleHandler.h"
#include "CTileMap.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _out)
{
	m_pCurLevel->FindObjectByName(_strName, _out);
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);
}

void CLevelMgr::init()
{
	// Level 하나 제작하기
	m_pCurLevel = new CLevel;

	// Layer 이름 설정
	m_pCurLevel->GetLayer(1)->SetName(L"Player");
	m_pCurLevel->GetLayer(2)->SetName(L"PlayerProjectile");
	m_pCurLevel->GetLayer(3)->SetName(L"Monster");
	m_pCurLevel->GetLayer(4)->SetName(L"MonsterProjectile");


	// ComputeShader 테스트
	Ptr<CPaintShader> pComputeShader = (CPaintShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PaintShader").Get();
	pComputeShader->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));	
	pComputeShader->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	pComputeShader->Execute();

	
	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->AddComponent(new CCameraScript);
	pCamObj->Transform()->SetRelativePos(Vec4(0.f, 0.f, 0.f, 0.f));

	pCamObj->Camera()->SetLayerAllVisible();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	m_pCurLevel->AddGameObject(pCamObj, 0);


	// Directional Light 추가
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	 pDirLight->AddComponent(new CTransform);
	 pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pDirLight->Light2D()->SetLightEmbient(Vec3(0.2f, 0.2f, 0.2f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	
	m_pCurLevel->AddGameObject(pDirLight, 0);


	// TileMap
	CGameObject* pTileMapObj = new CGameObject;
	pTileMapObj->SetName(L"TileMap");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);

	pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, -100.f, 900.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1.f));

	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"TileTex"));
	pTileMapObj->TileMap()->SetTileCount(10, 10);

	m_pCurLevel->AddGameObject(pTileMapObj, 0);


	// PointLight 추가
	//CGameObject* pPointLight = new CGameObject;
	//pPointLight->SetName(L"PointLight");

	//pPointLight->AddComponent(new CTransform);
	//pPointLight->AddComponent(new CLight2D);

	//pPointLight->Transform()->SetRelativePos(-500.f, 0.f, 0.f);

	//pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	//pPointLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	//pPointLight->Light2D()->SetRadius(500.f);


	//m_pCurLevel->AddGameObject(pPointLight, 0);
		

	
	CGameObject* pObject = nullptr;


	// Player
	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CPlayerScript);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));

	pObject->Animator2D()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 650.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->CreateAnimation(L"RightWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 910.f), Vec2(120.f, 130.f), 120.f, 10, 16);
	pObject->Animator2D()->Play(L"LeftWalk", true);

	int a = 0;
	Vec4 vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::VEC4_0, &vColor);
	pObject->MeshRender()->GetSharedMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &a);
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Character"));
	
	m_pCurLevel->AddGameObject(pObject, 0);

	// 충돌 Test Obj
	pObject = new CGameObject;
	pObject->SetName(L"CollisionTestObj1");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(Vec3(500.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	m_pCurLevel->AddGameObject(pObject, 1);

	pObject = new CGameObject;
	pObject->SetName(L"CollisionTestObj2");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMeshRender);

	pObject->Transform()->SetRelativePos(Vec3(-500.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	m_pCurLevel->AddGameObject(pObject, 2);
	



	//CGameObject* pChild = new CGameObject;
	//pChild->SetName(L"Child");

	//pChild->AddComponent(new CTransform);
	//pChild->AddComponent(new CMeshRender);
	//pChild->AddComponent(new CCollider2D);

	//pChild->Transform()->SetIgnoreParentScale(true);
	//pChild->Transform()->SetRelativePos(Vec3(512.f, 0.f, 100.f));
	//pChild->Transform()->SetRelativeScale(Vec3(256.f, 256.f, 1.f));

	//pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pChild->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pChild->MeshRender()->GetCurMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));

	
	//m_pCurLevel->AddGameObject(pChild, 1);

	// Particle
	Ptr<CPrefab> rainParticle = CResMgr::GetInst()->FindRes<CPrefab>(L"RainParticlePrefab");
	Instantiate(rainParticle->Instantiate(),Vec3(0.f, 0.f, 10.f));

	//Ptr<CPrefab> fireParticle = CResMgr::GetInst()->FindRes<CPrefab>(L"FireParticlePrefab");
	//Instantiate(fireParticle->Instantiate(), Vec3(0.f, 0.f, 10.f));

	//Ptr<CPrefab> fireworkParticle = CResMgr::GetInst()->FindRes<CPrefab>(L"FireworkParticlePrefab");
	//Instantiate(fireworkParticle->Instantiate(), Vec3(0.f, 0.f, 10.f));


	// LightWrap Object
	//CGameObject* pPostProcess = new CGameObject;
	//pPostProcess->AddComponent(new CTransform);
	//pPostProcess->AddComponent(new CMeshRender);

	//pPostProcess->Transform()->SetRelativeScale(Vec3(1600.f, 900.f, 1.f));
	//pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	//pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LightWrapMtrl"));
	//pPostProcess->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));

	//m_pCurLevel->AddGameObject(pPostProcess, 0);

	// PostProcess Object
	//CGameObject* pPostProcess = new CGameObject;
	//pPostProcess->AddComponent(new CTransform);
	//pPostProcess->AddComponent(new CMeshRender);

	//pPostProcess->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	//pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	//pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LightPostProcessMtrl"));

	//m_pCurLevel->AddGameObject(pPostProcess, 0);
	

	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 1);


	// 레벨 활성화
	m_pCurLevel->begin();
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
}

void CLevelMgr::finaltick()
{
	m_pCurLevel->finaltick();
}
