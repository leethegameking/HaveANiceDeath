#include "pch.h"
#include "CRenderMgr.h"

#include "GlobalComponent.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"

CRenderMgr::CRenderMgr()
	: m_pLight2DBuffer(nullptr)
	, m_EditorCam(nullptr)
{
	m_pLight2DBuffer = new CStructuredBuffer();
	m_pLight2DBuffer->Create(sizeof(tLightInfo), 2, SB_TYPE::SRV_ONLY, nullptr, true);
}

CRenderMgr::~CRenderMgr()
{
	if (nullptr != m_pLight2DBuffer)
		delete m_pLight2DBuffer;
}

void CRenderMgr::init()
{
	Vec2 vRenderResol = CDevice::GetInst()->GetRenderResolution();

	m_LightTex = CResMgr::GetInst()->FindRes<CTexture>(L"UAVTex");

	// ��ó�� �뵵, ����Ÿ�� �����
	m_RTCopyTex = CResMgr::GetInst()->CreateTexture(L"RTCopyTex"
		, (UINT)vRenderResol.x, (UINT)vRenderResol.y
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
	m_RTCopyTex->SetName(L"RTCopyTex");

	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}

void CRenderMgr::tick()
{
	m_vecCam.clear();
	m_vecLight2D.clear();
}

void CRenderMgr::render()
{
	static Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
	static Ptr<CTexture> pDSTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// NoiseTexture Update
	UpdateNoiseTexture();

	// 2D ���� ������Ʈ
	UpdateLight2D();

	// 3D ���� ������Ʈ
	//UpdateLight3D();

	// ���� UAV �� �ؽ��� ������Ʈ
	// UpdateLightTex();

	// GlobalData ������Ʈ
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_global);
	pGlobalCB->UpdateData(PIPELINE_STAGE::ALL_STAGE);
	pGlobalCB->UpdateData_CS();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();


	if (LEVEL_STATE::PLAY == pCurLevel->GetState())
	{
		render_game();
	}
	else
	{
		render_editor();
	}


	// Post Process
	// Vec4 ClearColor = { 0.f, 0.f, 0.f, 1.f };

	// m_LightTex->Clear(17);
	// CONTEXT->ClearUnorderedAccessViewFloat(m_LightTex->GetUAV().Get(), ClearColor);
}

void CRenderMgr::render_game()
{
	// �������� ������ ī�޶�� ����
	for (size_t i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render();
	}
}

void CRenderMgr::render_editor()
{
	assert(m_EditorCam);

	m_EditorCam->render();
}

void CRenderMgr::UpdateNoiseTexture()
{
	Ptr<CTexture> NoiseTex = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\noise\\noise_02.png");
	NoiseTex->UpdateData(15, PIPELINE_STAGE::ALL_STAGE);
	NoiseTex->UpdateData_CS(15, true);

	g_global.vNoiseResolution = Vec2(NoiseTex->GetWidth(), NoiseTex->GetHeight());
}

void CRenderMgr::UpdateLightTex()
{
	m_LightTex->UpdateData(17, PIPELINE_STAGE::PS);
}

void CRenderMgr::UpdateLight2D()
{
	// ���� ������ ���ڶ� ��� ���Ҵ�
	if (m_pLight2DBuffer->GetElementCount() < (UINT)m_vecLight2D.size())
	{
		m_pLight2DBuffer->Create(m_pLight2DBuffer->GetElementSize(), (UINT)m_vecLight2D.size(), SB_TYPE::SRV_ONLY, nullptr);
	}

	// ���������� ����ȭ���۷� �ű�
	m_pLight2DBuffer->SetData(m_vecLight2D.data(), (UINT)m_vecLight2D.size()); 	

	// ����ȭ ���۸� t13 �������Ϳ� ���ε�
	m_pLight2DBuffer->UpdateData(13, PIPELINE_STAGE::VS | PIPELINE_STAGE::PS); 	

	// ���� ���� �۷ι� �����Ϳ� ������Ʈ
	g_global.iLight2DCount = (UINT)m_vecLight2D.size();
}


void CRenderMgr::CopyRenderTarget()
{
	static Ptr<CTexture> RTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");

	// ���ε� ����
	ID3D11ShaderResourceView* SRV = nullptr;
	CONTEXT->PSSetShaderResources(60, 1, &SRV);

	// ����Ÿ�� ����ޱ�
	CONTEXT->CopyResource(m_RTCopyTex->GetTex2D().Get(), RTTex->GetTex2D().Get());

	// 60 �������� ���ε�
	m_RTCopyTex->UpdateData(60, PIPELINE_STAGE::PS);
}

CCamera* CRenderMgr::GetMainCam()
{
	if (CLevelMgr::GetInst()->GetCurLevel()->GetState() == LEVEL_STATE::PLAY)
	{
		if (m_vecCam.empty())
			return nullptr;

		return m_vecCam[0];
	}
	else
	{
		return m_EditorCam;
	}
}
