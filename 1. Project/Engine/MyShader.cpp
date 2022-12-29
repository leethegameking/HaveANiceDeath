#include "pch.h"
#include "CResMgr.h"

void CResMgr::CreateMyGraphicsShader()
{
	// Outer Block Shader
	CGraphicsShader* pShader = nullptr;

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\block_outer.fx", "VS_Block_Outer");
	pShader->CreatePixelShader(L"shader\\block_outer.fx", "PS_Block_Outer");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDSType(DS_TYPE::NO_WRITE);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	pShader->AddScalarParam(INT_0, "Texture Selection");

	pShader->AddTexParam(TEX_0, " CornerInside   ");
	pShader->AddTexParam(TEX_1, " CornerOutside  ");
	pShader->AddTexParam(TEX_2, " CornerOrnements");
	pShader->AddTexParam(TEX_3, " BlockBorder    ");

	pShader->SetEngineRes();
	AddRes<CGraphicsShader>(L"BlockOuterShader", pShader);
}

void CResMgr::CreateMyCSShader()
{

}