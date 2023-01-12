#include "pch.h"
#include "CResMgr.h"

void CResMgr::CreateMyGraphicsShader()
{
	CreateBlockOuterShader();
	CreateBlockInnerShader();
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
