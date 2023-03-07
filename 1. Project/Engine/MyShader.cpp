#include "pch.h"
#include "CResMgr.h"

#include "CParticleUpdateShader.h"

void CResMgr::CreateMyGraphicsShader()
{
	CreateBlockOuterShader();
	CreateBlockInnerShader();
	CreateUIShader();
	CreatePlayerShader();
	CreateEnemyShader();
	CreateCarpetShader();
	CreateGrayBoxShader();
	CreateDistOrderShader();

	CreateStarParticleShader();
	CreateFrontParticleShader();
}

void CResMgr::CreateMyCSShader()
{
	CreateStunStarCS();
	CreateGhostDeathCS();
	CreateSpawnDirCS();
}



void CResMgr::CreateBlockInnerShader()
{
	// Outer Block Shader
	CGraphicsShader* pShader = nullptr;

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\block_inner.fx", "VS_Block_Inner");
	pShader->CreatePixelShader(L"shader\\block_inner.fx", "PS_Block_Inner");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_BLOCK);

	pShader->AddTexParam(TEX_0, "Fill   ");


	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"BlockInnerShader", pShader);

	// Outer Block Shader Alpha
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\block_inner.fx", "VS_Block_Inner");
	pShader->CreatePixelShader(L"shader\\block_inner.fx", "PS_Block_Inner_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_BLOCK);

	pShader->AddTexParam(TEX_0, "Fill   ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"BlockInnerShaderAlpha", pShader);
}

void CResMgr::CreateBlockOuterShader()
{
	// Outer Block Shader
	CGraphicsShader* pShader = nullptr;

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\block_outer.fx", "VS_Block_Outer");
	pShader->CreatePixelShader(L"shader\\block_outer.fx", "PS_Block_Outer");
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_BLOCK);

	pShader->AddScalarParam(INT_0, "Texture Selection");

	pShader->AddTexParam(TEX_0, " CornerInside   ");
	pShader->AddTexParam(TEX_1, " CornerOutside  ");
	pShader->AddTexParam(TEX_2, " CornerOrnements");
	pShader->AddTexParam(TEX_3, " BlockBorder    ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"BlockOuterShader", pShader);

	// Outer Block Shader Alpha
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\block_outer.fx", "VS_Block_Outer");
	pShader->CreatePixelShader(L"shader\\block_outer.fx", "PS_Block_Outer_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_BLOCK);

	pShader->AddScalarParam(INT_0, "Texture Selection");
	pShader->AddScalarParam(FLOAT_0, "Color Coeffi");

	pShader->AddTexParam(TEX_0, " CornerInside   ");
	pShader->AddTexParam(TEX_1, " CornerOutside  ");
	pShader->AddTexParam(TEX_2, " CornerOrnements");
	pShader->AddTexParam(TEX_3, " BlockBorder    ");
	pShader->AddTexParam(TEX_4, " PlatformShort  ");
	pShader->AddTexParam(TEX_5, " PlatformMedium ");
	pShader->AddTexParam(TEX_6, " PlatformLong   ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"BlockOuterShaderAlpha", pShader);
}

void CResMgr::CreateUIShader()
{
	CGraphicsShader* pShader = nullptr;
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\ui.fx", "VS_UI");
	pShader->CreatePixelShader(L"shader\\ui.fx", "PS_UI_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	pShader->AddTexParam(TEX_0, " Image 1   ");
	pShader->AddTexParam(TEX_1, " Image 2   ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"UIShaderAlpha", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\ui.fx", "VS_UI");
	pShader->CreatePixelShader(L"shader\\ui.fx", "PS_Boss_HP_Bar_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	pShader->AddTexParam(TEX_0, " Image 1   ");
	pShader->AddScalarParam(FLOAT_0, " Ratio ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"BossHPBarShaderAlpha", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\ui.fx", "VS_UI");
	pShader->CreatePixelShader(L"shader\\ui.fx", "PS_Player_HP_Bar_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	pShader->AddTexParam(TEX_0, " Image 1   ");
	pShader->AddScalarParam(FLOAT_0, " Ratio ");
	pShader->AddScalarParam(FLOAT_1, " Incli ");
	pShader->AddScalarParam(INT_0, " Dist ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"PlayerHPBarShaderAlpha", pShader);
}

void CResMgr::CreatePlayerShader()
{
	CGraphicsShader* pShader = nullptr;
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\player.fx", "VS_Player");
	pShader->CreatePixelShader(L"shader\\player.fx", "PS_Player_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0,	"Alpha Ratio          ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1,	"Elevator Close Speed ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2,	"Elevator Close Ratio ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_0,	"Elevator Close Bool  ");

	pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Att_Color  ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"PlayerShaderAlpha", pShader);
}

void CResMgr::CreateEnemyShader()
{
	CGraphicsShader* pShader = nullptr;
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\enemy.fx", "VS_Player");
	pShader->CreatePixelShader(L"shader\\enemy.fx", "PS_Player_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddScalarParam(SCALAR_PARAM::VEC4_0, "Att_Color  ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"EnemyShaderAlpha", pShader);
}

void CResMgr::CreateCarpetShader()
{
	CGraphicsShader* pShader = nullptr;
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\carpet.fx", "VS_Carpet");
	pShader->CreatePixelShader(L"shader\\carpet.fx", "PS_Carpet_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MIDDLE);

	pShader->AddTexParam(TEX_PARAM::TEX_0, "Carpet Image  ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Red          ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_1, "Green        ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_2, "Blue         ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"CarpetShaderAlpha", pShader);
}

void CResMgr::CreateGrayBoxShader()
{
	CGraphicsShader* pShader = nullptr;
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\graybox.fx", "VS_GrayBox");
	pShader->CreatePixelShader(L"shader\\graybox.fx", "PS_GrayBox_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	pShader->AddTexParam(TEX_PARAM::TEX_0, "GrayBox Image  ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "Original Ratio ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "Black Ratio    ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, "Alpha Ratio    ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_3, "Alpha          ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"GrayBoxShaderAlpha", pShader);
}

void CResMgr::CreateDistOrderShader()
{
	CGraphicsShader* pShader = nullptr;
	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\distorder.fx", "VS_Dist_Order");
	pShader->CreatePixelShader(L"shader\\distorder.fx", "PS_Dist_Order_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_FAR_BACK);

	pShader->AddTexParam(TEX_PARAM::TEX_0, "Image ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Sampler Type ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_1, "MUL = 0 / ADD = 1 ");
	
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "RED   ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "GREEN ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, "BLUE  ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_3, "ALPHA ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"FarBackShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\distorder.fx", "VS_Dist_Order");
	pShader->CreatePixelShader(L"shader\\distorder.fx", "PS_Dist_Order_Light_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_BACK);

	pShader->AddTexParam(TEX_PARAM::TEX_0, "Image ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Sampler Type ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_1, "MUL = 0 / ADD = 1 ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_2, "Exclude Light");

	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "RED   ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "GREEN ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, "BLUE  ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_3, "ALPHA ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"BackShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\distorder.fx", "VS_Dist_Order");
	pShader->CreatePixelShader(L"shader\\distorder.fx", "PS_Dist_Order_Light_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MIDDLE);

	pShader->AddTexParam(TEX_PARAM::TEX_0, "Image ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Sampler Type ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_1, "MUL = 0 / ADD = 1 ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_2, "Exclude Light");

	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "RED   ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "GREEN ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, "BLUE  ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_3, "ALPHA ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"MiddleShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\distorder.fx", "VS_Dist_Order");
	pShader->CreatePixelShader(L"shader\\distorder.fx", "PS_Dist_Order_Light_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_FRONT);

	pShader->AddTexParam(TEX_PARAM::TEX_0, "Image ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Sampler Type ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_1, "MUL = 0 / ADD = 1 ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_2, "Exclude Light");

	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "RED   ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "GREEN ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, "BLUE  ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_3, "ALPHA ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"FrontShader", pShader);

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\distorder.fx", "VS_Dist_Order");
	pShader->CreatePixelShader(L"shader\\distorder.fx", "PS_Dist_Order_Alpha");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_FAR_FRONT);

	pShader->AddTexParam(TEX_PARAM::TEX_0, "Image ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_0, "Sampler Type ");
	pShader->AddScalarParam(SCALAR_PARAM::INT_1, "MUL = 0 / ADD = 1 ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_0, "RED   ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_1, "GREEN ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_2, "BLUE  ");
	pShader->AddScalarParam(SCALAR_PARAM::FLOAT_3, "ALPHA ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"FarFrontShader", pShader);
}

void CResMgr::CreateStarParticleShader()
{
	// ParticleRenderShader
	CGraphicsShader* pShader = nullptr;

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\starparticlerender.fx", "VS_StarParticleRender");
	pShader->CreateGeometryShader(L"shader\\starparticlerender.fx", "GS_StarParticleRender");
	pShader->CreatePixelShader(L"shader\\starparticlerender.fx", "PS_StarParticleRender");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MIDDLE);

	pShader->AddScalarParam(SCALAR_PARAM::INT_2, "Is Rot");
	pShader->AddScalarParam(SCALAR_PARAM::INT_3, "Has Color");
	
	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"ParticleRender_Star", pShader);
}

void CResMgr::CreateFrontParticleShader()
{
	CGraphicsShader* pShader = nullptr;

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\starparticlerender.fx", "VS_StarParticleRender");
	pShader->CreateGeometryShader(L"shader\\starparticlerender.fx", "GS_StarParticleRender");
	pShader->CreatePixelShader(L"shader\\starparticlerender.fx", "PS_StarParticleRender");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_FRONT);

	pShader->AddScalarParam(SCALAR_PARAM::INT_2, "Is Rot");
	pShader->AddScalarParam(SCALAR_PARAM::INT_3, "Has Color");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"ParticleRender_Front", pShader);
}

void CResMgr::CreateStunStarCS()
{
	CComputeShader* pShader = nullptr;

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\stunstar_cs.fx", "CS_StunStarUpdate");

	pShader->SetEngineRes();
	AddRes<CComputeShader>(L"StunStarParticleUpdate", pShader);

}

void CResMgr::CreateGhostDeathCS()
{
	CComputeShader* pShader = nullptr;

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\ghostdeath_cs.fx", "CS_GhostDeathUpdate");

	pShader->SetEngineRes();
	AddRes<CComputeShader>(L"GhostDeathParticleUpdate", pShader);
}

void CResMgr::CreateSpawnDirCS()
{
	CComputeShader* pShader = nullptr;

	pShader = new CParticleUpdateShader;
	pShader->CreateComputeShader(L"shader\\spawndir_cs.fx", "CS_SpawnDirUpdate");

	pShader->SetEngineRes();
	AddRes<CComputeShader>(L"SpawnDirParticleUpdate", pShader);
}
