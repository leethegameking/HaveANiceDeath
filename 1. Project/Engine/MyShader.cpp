#include "pch.h"
#include "CResMgr.h"

void CResMgr::CreateMyGraphicsShader()
{
	CreateBlockOuterShader();
	CreateBlockInnerShader();
	CreateUIShader();
	CreatePlayerShader();
}

void CResMgr::CreateMyCSShader()
{

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
	pShader->SetRSType(RS_TYPE::CULL_NONE);
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
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

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
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

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
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

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

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"PlayerShaderAlpha", pShader);
}
