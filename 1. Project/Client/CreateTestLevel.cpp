#include "pch.h"
#include "CreateTestLevel.h"

#include "CSaveLoadMgr.h"

#include "CCameraScript.h"
#include "CGrid2DScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/GlobalComponent.h>
#include <Engine/CPaintShader.h>
#include <Engine/CParticleHandler.h>
#include <Engine/CTileMap.h>
#include <Engine/CLevelMgr.h>

#include <Script/CMonsterScript.h>
#include <Script/CTestScript.h>
#include <Script/CPlayerScript.h>


void CreateTestLevel()
{
	CreateDefaultAnimation();

	//CResMgr::GetInst()->Load<CAnimation2D>(L"animation\\PlayerRun");
	//CResMgr::GetInst()->Load<CMaterial>(L"material\\New Material1");
	//CLevelMgr::GetInst()->ChangeLevel(CSaveLoadMgr::GetInst()->LoadLevel(L"level\\testLV.lv"));
	//return;

	CLevel* pTestLevel = new CLevel;
	pTestLevel->SetName(L"TestLevel");

	// Camera Object 추가
	CGameObject* pCamObj = new CGameObject;
	pCamObj->SetName(L"MainCamera");

	pCamObj->AddComponent(new CTransform);
	pCamObj->AddComponent(new CCamera);
	pCamObj->Transform()->SetRelativePos(Vec4(0.f, 0.f, 0.f, 0.f));

	pCamObj->Camera()->SetLayerAllVisible();
	pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAHPICS);

	pTestLevel->AddGameObject(pCamObj, 0);

	// TileMap
	CGameObject* pTileMapObj = new CGameObject;
	pTileMapObj->SetName(L"TileMap");
	pTileMapObj->AddComponent(new CTransform);
	pTileMapObj->AddComponent(new CTileMap);

	pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, -100.f, 900.f));
	pTileMapObj->Transform()->SetRelativeScale(Vec3(1000.f, 1000.f, 1.f));

	pTileMapObj->TileMap()->SetTileAtlas(CResMgr::GetInst()->FindRes<CTexture>(L"TileTex"));
	pTileMapObj->TileMap()->SetTileCount(10, 10);

	pTestLevel->AddGameObject(pTileMapObj, 0);

	CAnimation2D* pAnimation = nullptr;

	// Directional Light 추가
	CGameObject* pDirLight = new CGameObject;
	pDirLight->SetName(L"DirectionalLight");

	pDirLight->AddComponent(new CTransform);
	pDirLight->AddComponent(new CLight2D);

	pDirLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pDirLight->Light2D()->SetLightEmbient(Vec3(0.2f, 0.2f, 0.2f));
	pDirLight->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);

	pTestLevel->AddGameObject(pDirLight, 0);

	// PointLight 추가
	CGameObject* pPointLight = new CGameObject;
	pPointLight->SetName(L"PointLight");

	pPointLight->AddComponent(new CTransform);
	pPointLight->AddComponent(new CLight2D);

	pPointLight->Transform()->SetRelativePos(-500.f, 0.f, 0.f);

	pPointLight->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pPointLight->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pPointLight->Light2D()->SetRadius(500.f);

	pTestLevel->AddGameObject(pPointLight, 0);



	CGameObject* pObject = nullptr;


	// Player
	pObject = new CGameObject;
	pObject->SetName(L"Player");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CAnimator2D);
	pObject->AddComponent(new CPlayerScript);
	pObject->AddComponent(new CMonsterScript);
	pObject->AddComponent(new CTestScript);

	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAlphaBlendMtrl"));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);
	pObject->Collider2D()->SetOffsetPos(Vec2(0.f, 0.f));
	
	pObject->MeshRender()->GetSharedMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Character"));

	pObject->Animator2D()->AddAnimation(L"PlayerIdle");
	pObject->Animator2D()->AddAnimation(L"PlayerIdleToRun");
	pObject->Animator2D()->AddAnimation(L"PlayerIdleUturn");
	pObject->Animator2D()->AddAnimation(L"PlayerRun");
	pObject->Animator2D()->AddAnimation(L"PlayerRunToIdle");
	pObject->Animator2D()->AddAnimation(L"PlayerRunUturn");

	pTestLevel->AddGameObject(pObject, 0);

	// 충돌 Test Obj
	pObject = new CGameObject;
	pObject->SetName(L"CollisionTestObj1");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CCollider2D);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CMonsterScript);

	pObject->Transform()->SetRelativePos(Vec3(500.f, 0.f, 800.f));
	pObject->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObject->Collider2D()->SetCollider2DType(COLLIDER2D_TYPE::COLLIDER2D_RECT);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pObject->MeshRender()->GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"Character"));


	pTestLevel->AddGameObject(pObject, 1);

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

	pTestLevel->AddGameObject(pObject, 2);

	// PostProcess Object
	CGameObject* pPostProcess = new CGameObject;
	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);

	pPostProcess->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	pPostProcess->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PostProcessMtrl"));

	pTestLevel->AddGameObject(pPostProcess, 0);


	// 충돌 레이어 설정
	CCollisionMgr::GetInst()->CollisionLayerCheck(0, 1);

	CLevelMgr::GetInst()->ChangeLevel(pTestLevel);

	// 레벨 활성화
	pTestLevel->begin();

	// Test
	//vector<vector<tBGRA>> invec;
	//Ptr<CTexture> tex = CResMgr::GetInst()->FindRes<CTexture>(L"Link");
	//tex->GetPixelVector(invec);
	//Vec4 out = tex->WIdthSearch(invec, Vec2(10.f, 10.f));
	
	// ComputeShader 테스트
	//Ptr<CPaintShader> pComputeShader = (CPaintShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"PaintShader").Get();
	//pComputeShader->SetTexture(CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex"));
	//pComputeShader->SetColor(Vec4(0.f, 0.f, 1.f, 1.f));
	//pComputeShader->Execute();
	
	//Ptr<CPrefab> fireParticle = CResMgr::GetInst()->FindRes<CPrefab>(L"FireParticlePrefab");
	//Instantiate(fireParticle->Instantiate(), Vec3(0.f, 0.f, 10.f));
	
	//Ptr<CPrefab> fireworkParticle = CResMgr::GetInst()->FindRes<CPrefab>(L"FireworkParticlePrefab");
	//Instantiate(fireworkParticle->Instantiate(), Vec3(0.f, 0.f, 10.f));

	int debug = 0;
}

void CreateDefaultAnimation()
{
	// Animation
	CResMgr::GetInst()->CreateAnimation(L"UpWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 520.f), Vec2::Zero, Vec2(120.f, 130.f), 120.f, 10, 16);
	CResMgr::GetInst()->CreateAnimation(L"DownWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 780.f), Vec2::Zero, Vec2(120.f, 130.f), 120.f, 10, 16);
	CResMgr::GetInst()->CreateAnimation(L"LeftWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 650.f), Vec2::Zero, Vec2(120.f, 130.f), 120.f, 10, 16);
	CResMgr::GetInst()->CreateAnimation(L"RightWalk", CResMgr::GetInst()->FindRes<CTexture>(L"Link"), Vec2(0.f, 910.f), Vec2::Zero, Vec2(120.f, 130.f), 120.f, 10, 16);

	CResMgr::GetInst()->Load<CAnimation2D>(L"animation\\PlayerIdle");
	CResMgr::GetInst()->Load<CAnimation2D>(L"animation\\PlayerIdleToRun");
	CResMgr::GetInst()->Load<CAnimation2D>(L"animation\\PlayerIdleUturn");
	CResMgr::GetInst()->Load<CAnimation2D>(L"animation\\PlayerRun");
	CResMgr::GetInst()->Load<CAnimation2D>(L"animation\\PlayerRunToIdle");
	CResMgr::GetInst()->Load<CAnimation2D>(L"animation\\PlayerRunUturn");
}