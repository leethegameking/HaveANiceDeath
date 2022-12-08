#include "pch.h"
#include "CResMgr.h"



void CResMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultTexture();

	CreateDefaultGraphicsShader();

	CreateDefaultComputeShader();

	CreateDefaultMaterial();
}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	CMesh* pMesh = nullptr;

	// Point Mesh
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT idx = 0;

	pMesh = new CMesh;
	pMesh->Create(&v, 1, &idx, 1);
	AddRes<CMesh>(L"PointMesh", pMesh);	


	// 사각형 메쉬 만들기
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVtx.push_back(v);

	// Index
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);	
	vecIdx.clear();





	// Index
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);
	vecIdx.push_back(3);
	vecIdx.push_back(0);	

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"RectMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();


	// 원형메쉬 만들기
	// 중심점	
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);


	vecVtx.push_back(v);

	int iSlice = 20;
	float fRadius = 0.5f;
	float fTheta = XM_2PI / (float)iSlice;

	for (int i = 0; i < iSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		v.vUV = Vec2(0.5f + v.vPos.x, 1.f - (v.vPos.y + 0.5f));
		vecVtx.push_back(v);
	}

	for (UINT i = 0; i < (UINT)iSlice; ++i)
	{
		vecIdx.push_back(0);
		if (i == iSlice - 1)
		{
			vecIdx.push_back(1);
		}
		else
		{
			vecIdx.push_back(i + 2);
		}
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);	
	vecIdx.clear();


	// Cicle_debug
	for (int i = 0; i < vecVtx.size() - 2; ++i)
	{		
		vecIdx.push_back(i + 1);				
	}
	vecIdx.push_back(1);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());
	AddRes<CMesh>(L"CircleMesh_Debug", pMesh);
	vecVtx.clear();
	vecIdx.clear();
}

void CResMgr::CreateDefaultTexture()
{
	// 텍스쳐 로딩
	Load<CTexture>(L"Plane", L"texture\\Player.bmp");
	Load<CTexture>(L"Smoke", L"texture\\smokeparticle.png");
	Load<CTexture>(L"Character", L"texture\\Character.png");
	Load<CTexture>(L"Link", L"texture\\link.png");

	Load<CTexture>(L"DeadCellColor", L"texture\\beheaded.png");
	Load<CTexture>(L"DeadCellNormal", L"texture\\beheaded_n.png");

	Load<CTexture>(L"SmokeParticle", L"texture\\particle\\smokeparticle.png");
	Load<CTexture>(L"CartoonSmoke", L"texture\\particle\\CartoonSmoke.png");
	Load<CTexture>(L"Bubbles50px", L"texture\\particle\\Bubbles50px.png");
	Load<CTexture>(L"Sparks", L"texture\\particle\\Sparks.png");
	Load<CTexture>(L"HardRain", L"texture\\particle\\HardRain.png");
	Load<CTexture>(L"HardCircle", L"texture\\particle\\HardCircle.png");
	Load<CTexture>(L"AlphaCircle", L"texture\\particle\\AlphaCircle.png");
	Load<CTexture>(L"TileTex", L"texture\\TILE.bmp");
	Load<CTexture>(L"PlusButton", L"texture\\plus.png");

	
	// NoiseTexture
	Load<CTexture>(L"Noise_01", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"Noise_02", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"Noise_03", L"texture\\noise\\noise_03.jpg");
	
	Load<CTexture>(L"PlayerRunAtlas", L"texture\\player\\PlayerRunAtlas.png");
	Load<CTexture>(L"PlayerIdleAtlas", L"texture\\player\\PlayerIdleAtlas.png");

	// 텍스쳐 생성
	CreateTexture(L"UAVTex", 1600, 900, DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
}

void CResMgr::CreateDefaultGraphicsShader()
{
	AddInputLayout(DXGI_FORMAT_R32G32B32_FLOAT, "POSITION");
	AddInputLayout(DXGI_FORMAT_R32G32B32A32_FLOAT, "COLOR");
	AddInputLayout(DXGI_FORMAT_R32G32_FLOAT, "TEXCOORD");

	CGraphicsShader* pShader = nullptr;

	// Std2D Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	pShader->AddScalarParam(INT_0, "Test Int");
	pShader->AddScalarParam(FLOAT_2, "Test Float");
	pShader->AddScalarParam(VEC2_3, "Test Vec2");
	pShader->AddScalarParam(VEC4_2, "Test Vec4");
	pShader->AddTexParam(TEX_0, "Output Texture 1");
	pShader->AddTexParam(TEX_1, "Output Texture 2");
	pShader->AddTexParam(TEX_2, "Output Texture 3");

	AddRes<CGraphicsShader>(L"Std2DShader", pShader);

	// Std2dAlphaBlend Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphaBlend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"Std2DAlphaBlendShader", pShader);


	// Grid Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\editor.fx", "VS_Grid");
	pShader->CreatePixelShader(L"shader\\editor.fx", "PS_Grid");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"GridShader", pShader);

	// DebugDraw Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\debugdraw.fx", "VS_DebugDraw");
	pShader->CreatePixelShader(L"shader\\debugdraw.fx", "PS_DebugDraw");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"DebugDrawShader", pShader);


	// DeadCell Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_DeadCell");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_DeadCell");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::DEFAULT);
	pShader->SetDSType(DS_TYPE::LESS);	
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);	

	AddRes<CGraphicsShader>(L"DeadCellShader", pShader);


	// ParticleRenderShader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\particlerender.fx", "VS_ParticleRender");
	pShader->CreateGeometryShader(L"shader\\particlerender.fx", "GS_ParticleRender");
	pShader->CreatePixelShader(L"shader\\particlerender.fx", "PS_ParticleRender");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"ParticleRenderShader", pShader);


	// PostProcess Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_PostProcess");	
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_PostProcess");
	
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	AddRes<CGraphicsShader>(L"PostProcessShader", pShader);


	// LightPostProcess Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_LightPostProcess");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_LightPostProcess");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POST_PROCESS);

	AddRes<CGraphicsShader>(L"LightPostProcessShader", pShader);

	// LightPostProcess Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\lightwrap.fx", "VS_LightWrap");
	pShader->CreatePixelShader(L"shader\\lightwrap.fx", "PS_LightWrap");

	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"LightWrapShader", pShader);

	// TileMap Shader
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	AddRes<CGraphicsShader>(L"TileMapShader", pShader);
}


#include "CPaintShader.h"
#include "CParticleUpdateShader.h"
void CResMgr::CreateDefaultComputeShader()
{
	CComputeShader* pShader = nullptr;

	// Paint Shader
	pShader = new CPaintShader;
	pShader->CreateComputeShader(L"shader\\compute.fx", "CS_Paint");
	AddRes<CComputeShader>(L"PaintShader", pShader);

	// Particle Update Shader
	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\particleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ParticleUpdateShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\rainparticleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"RainParticleUpdateShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\fireparticleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"FireParticleUpdateShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\explosionparticleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"ExplosionParticleUpdateShader", pShader);

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\Fireworkparticleupdate.fx", "CS_ParticleUpdate");
	AddRes<CComputeShader>(L"FireworkParticleUpdateShader", pShader);


}

void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;

	//pMtrl = new CMaterial;
	//pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	//AddRes<CMaterial>(L"Std2DMtrl", pMtrl);

	Load<CMaterial>(L"Std2DMtrl", L"material\\test.mtrl");

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DAlphaBlendShader"));
	AddRes<CMaterial>(L"Std2DAlphaBlendMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"GridShader"));
	AddRes<CMaterial>(L"GridMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DebugDrawShader"));
	AddRes<CMaterial>(L"DebugDrawMtrl", pMtrl);	

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"DeadCellShader"));
	AddRes<CMaterial>(L"DeadCellMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"ParticleRenderShader"));
	AddRes<CMaterial>(L"ParticleRenderMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"PostProcessShader"));
	AddRes<CMaterial>(L"PostProcessMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"LightPostProcessShader"));
	AddRes<CMaterial>(L"LightPostProcessMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"LightWrapShader"));
	AddRes<CMaterial>(L"LightWrapMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TileMapShader"));
	AddRes<CMaterial>(L"TileMapMtrl", pMtrl);
}

void CResMgr::AddInputLayout(DXGI_FORMAT _eFormat, const char* _strSemanticName)
{
	D3D11_INPUT_ELEMENT_DESC LayoutDesc = {};
	LayoutDesc.AlignedByteOffset = m_iLayoutOffset;
	LayoutDesc.Format = _eFormat;
	LayoutDesc.InputSlot = 0;
	LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc.SemanticName = _strSemanticName;
	LayoutDesc.SemanticIndex = 0;
	m_vecLayoutInfo.push_back(LayoutDesc);

	m_iLayoutOffset += GetSizeofFormat(_eFormat);
}
