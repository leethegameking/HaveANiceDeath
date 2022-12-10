#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(1000) // ���� �����ÿ��� 10000��
	, m_iAliveCount(0)
	, m_vStartScale(Vec4(50.f, 50.f, 1.f, 0.f))
	, m_vEndScale(Vec4(10.f, 10.f, 1.f, 0.f))
	, m_vStartColor(Vec4(0.24f, 0.28f, 0.8f, 1.f))
	, m_vEndColor(Vec4(0.78f, 0.74f, 0.9f, 1.f))
	, m_vMinMaxSpeed(Vec2(100.f, 300.f))
	, m_vMinMaxLifeTime(Vec2(0.2f, 0.3f))
	, m_fSpawnRange(Vec2(200.f, 0.f))
	, m_Frequency(5.f)
	, m_fAccTime(0.f)
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	, m_WorldSpawn(1)
	, m_RenderType(1)
	, CS_KEY(L"ParticleUpdateShader")
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	GetDynamicMaterial();

	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	// ����ȭ���� ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);
}

CParticleSystem::CParticleSystem(wstring _CSkey)
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(1000) // ���� �����ÿ��� 10000��
	, m_iAliveCount(0)
	, m_vStartScale(Vec4(50.f, 50.f, 1.f, 0.f))
	, m_vEndScale(Vec4(10.f, 10.f, 1.f, 0.f))
	, m_vStartColor(Vec4(0.24f, 0.28f, 0.8f, 1.f ))
	, m_vEndColor(Vec4(0.78f, 0.74f, 0.9f, 1.f))
	, m_vMinMaxSpeed(Vec2(100.f, 300.f))
	, m_vMinMaxLifeTime(Vec2(0.2f, 0.3f))
	, m_fSpawnRange(Vec2(200.f, 0.f))
	, m_Frequency(5.f)
	, m_fAccTime(0.f)
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	, m_WorldSpawn(1)
	, m_RenderType(1)
	, CS_KEY(_CSkey)
{
	// �� �޽���, ParticleRenderMtrl�� �������� ��� �ִ� ��ƼŬ.
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	GetDynamicMaterial();

	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(_CSkey).Get();

	// ����ȭ���� ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _clone)
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(_clone.m_iMaxCount) // ���� �����ÿ��� 10000��
	, m_iAliveCount(_clone.m_iAliveCount)
	, m_vStartScale(_clone.m_vStartScale)
	, m_vEndScale(_clone.m_vEndScale)
	, m_vStartColor(_clone.m_vStartColor)
	, m_vEndColor(_clone.m_vEndColor)
	, m_vMinMaxSpeed(_clone.m_vMinMaxSpeed)
	, m_vMinMaxLifeTime(_clone.m_vMinMaxLifeTime)
	, m_fSpawnRange(_clone.m_fSpawnRange)
	, m_Frequency(_clone.m_Frequency)
	, m_fAccTime(0.f)
	, m_ParticleBuffer(nullptr)
	, m_ParticleShare(nullptr)
	, m_WorldSpawn(_clone.m_WorldSpawn)
	, m_RenderType(1)
	, CS_KEY(_clone.CS_KEY)
	, TEX_KEY(_clone.TEX_KEY)
	, LIGHT_TEX_KEY(_clone.LIGHT_TEX_KEY)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));
	GetDynamicMaterial();

	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(CS_KEY).Get();
	SetLightTex(LIGHT_TEX_KEY);
	SetTexture(TEX_KEY);

	// ����ȭ���� ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleShare)
		delete m_ParticleShare;
}

void CParticleSystem::SetTexture(wstring _texKey)
{
	GetCurMaterial()->SetTexParam(TEX_PARAM::TEX_0, CResMgr::GetInst()->FindRes<CTexture>(_texKey));
	TEX_KEY = _texKey;
}

void CParticleSystem::SetLightTex(wstring _texKey)
{
	LIGHT_TEX_KEY = _texKey;
	GetUpdateCS()->SetLightTex(LIGHT_TEX_KEY);
}

void CParticleSystem::ChangeBuffer()
{
	m_ParticleBuffer->ChangeBuffer(m_iMaxCount, nullptr);
}

void CParticleSystem::finaltick()
{
	/*Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += 500.f * DT;
	Transform()->SetRelativePos(vPos);*/

	// �ƽ�ī��Ʈ�� ��ƼŬ ������ ��� ���� �ٸ��ٸ� -> ���۸� �ٽ� �������. 
	if (m_iMaxCount != m_ParticleBuffer->GetElementCount())
		ChangeBuffer();

	// ��ƼŬ ���� �ð�
	float fAliveTime = 1.f / m_Frequency;

	// �����ð�
	m_fAccTime += DT;

	// �����ð��� ��ƼŬ ���� �ð��� �Ѿ��
	if (fAliveTime < m_fAccTime)
	{
		float f = (m_fAccTime / fAliveTime);
		UINT iAliveCount = (UINT)f;
		m_fAccTime = fAliveTime * (f - floorf(f));

		//tParticleShare share = { iAliveCount, };
		tParticleShare share = { m_iAliveCount, };
		m_ParticleShare->SetData(&share, 1);
	}
	else
	{
		tParticleShare share = {};
		m_ParticleShare->SetData(&share, 1);
	}

	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetParticleShareData(m_ParticleShare);
	m_UpdateCS->SetWorldSpawn(m_WorldSpawn);
	m_UpdateCS->SetObjectWorldPos(Transform()->GetWorldPos());
	// Vec3 temp = Transform()->GetWorldPos();
	m_UpdateCS->SetSpawnRange(m_fSpawnRange);
	m_UpdateCS->SetMinMaxSpeed(m_vMinMaxSpeed);
	m_UpdateCS->SetMinMaxLifeTime(m_vMinMaxLifeTime);

	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	// ��ġ���� ������Ʈ
	Transform()->UpdateData();
	m_ParticleBuffer->UpdateData(16, PIPELINE_STAGE::VS | PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);


	// ���� ������Ʈ
	GetCurMaterial()->SetScalarParam(INT_1, &m_WorldSpawn);
	GetCurMaterial()->SetScalarParam(VEC4_0, &m_vStartScale);
	GetCurMaterial()->SetScalarParam(VEC4_1, &m_vEndScale);
	GetCurMaterial()->SetScalarParam(VEC4_2, &m_vStartColor);
	GetCurMaterial()->SetScalarParam(VEC4_3, &m_vEndColor);
	GetCurMaterial()->SetScalarParam(INT_2, &m_RenderType);

	// Mtrl�� �������� ��� �ְų� ���⼭ 
	GetCurMaterial()->UpdateData();

	// ������
	GetMesh()->render_particle(m_iMaxCount);

	// Ŭ����
	m_ParticleBuffer->Clear();
	CMaterial::Clear();
}

