#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"
#include "CAnimator2D.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(50) // ���� �����ÿ��� 10000��
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
	, m_FirstEntry(true)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateShader").Get();

	// ����ȭ���� ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleAnim = new CStructuredBuffer;
	m_ParticleAnim->Create(sizeof(tParticleAnim), 50, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleAnimFrm = new CStructuredBuffer;
	m_ParticleAnimFrm->Create(sizeof(tAnimIdx), m_iMaxCount, SB_TYPE::UAV_INC, nullptr, true);
}

CParticleSystem::CParticleSystem(wstring _CSkey)
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxCount(50) // ���� �����ÿ��� 10000��
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
	, m_FirstEntry(true)
{
	// �� �޽���, ParticleRenderMtrl�� �������� ��� �ִ� ��ƼŬ.
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(_CSkey).Get();

	// ����ȭ���� ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);


	m_ParticleAnim = new CStructuredBuffer;
	m_ParticleAnim->Create(sizeof(tParticleAnim), 50, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleAnimFrm = new CStructuredBuffer;
	m_ParticleAnimFrm->Create(sizeof(tAnimIdx), m_iMaxCount, SB_TYPE::UAV_INC, nullptr, true);

}

CParticleSystem::CParticleSystem(const CParticleSystem& _clone)
	: CRenderComponent(_clone)
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
	, m_FirstEntry(true)
{
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(CS_KEY).Get();

	// ����ȭ���� ����
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleShare = new CStructuredBuffer;
	m_ParticleShare->Create(sizeof(tParticleShare), 1, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleAnim = new CStructuredBuffer;
	m_ParticleAnim->Create(sizeof(tParticleAnim), 50, SB_TYPE::UAV_INC, nullptr, true);

	m_ParticleAnimFrm = new CStructuredBuffer;
	m_ParticleAnimFrm->Create(sizeof(tAnimIdx), m_iMaxCount, SB_TYPE::UAV_INC, nullptr, true);
}


CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;

	if (nullptr != m_ParticleShare)
		delete m_ParticleShare;

	if (nullptr != m_ParticleAnim)
		delete m_ParticleAnim;

	if (nullptr != m_ParticleAnimFrm)
		delete m_ParticleAnimFrm;
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
	m_ParticleBuffer->ChangeBuffer(m_iMaxCount, nullptr, true);
}

void CParticleSystem::ChangeBuffer_Anim()
{
	m_ParticleAnimFrm->ChangeBuffer(m_iMaxCount, nullptr, true);
}

void CParticleSystem::finaltick()
{
	// �ƽ�ī��Ʈ�� ��ƼŬ ������ ��� ���� �ٸ��ٸ� -> ���۸� �ٽ� �������. 
	if (m_iMaxCount != m_ParticleBuffer->GetElementCount())
	{
		ChangeBuffer();
		ChangeBuffer_Anim();
	}

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

	if (m_FirstEntry)
	{
		m_UpdateCS->SetFirstEntry(1);
		m_FirstEntry = false;
	}
	else
	{
		m_UpdateCS->SetFirstEntry(0);
	}

	m_UpdateCS->SetOwner(this);
	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->SetParticleShareData(m_ParticleShare);

	if (Animator2D())
	{
		m_UpdateCS->SetParticleAnim(m_ParticleAnim);
		m_UpdateCS->SetParticleAnimFrm(m_ParticleAnimFrm);
	}

	m_UpdateCS->SetWorldSpawn(m_WorldSpawn);
	m_UpdateCS->SetObjectWorldPos(Transform()->GetWorldPos());
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
	m_ParticleAnim->UpdateData(17, PIPELINE_STAGE::VS | PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);
	m_ParticleAnimFrm->UpdateData(18, PIPELINE_STAGE::VS | PIPELINE_STAGE::GS | PIPELINE_STAGE::PS);

	// ���� ������Ʈ
	GetCurMaterial()->SetScalarParam(INT_1, &m_WorldSpawn);
	GetCurMaterial()->SetScalarParam(VEC4_0, &m_vStartScale);
	GetCurMaterial()->SetScalarParam(VEC4_1, &m_vEndScale);
	GetCurMaterial()->SetScalarParam(VEC4_2, &m_vStartColor);
	GetCurMaterial()->SetScalarParam(VEC4_3, &m_vEndColor);

	// Mtrl�� �������� ��� �ְų� ���⼭ 
	GetCurMaterial()->UpdateData();

	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	// ������
	GetMesh()->render_particle(m_iMaxCount);

	// Ŭ����
	m_ParticleBuffer->Clear();
	if (Animator2D())
	{
		m_ParticleAnim->Clear();
		m_ParticleAnimFrm->Clear();
	}

	CMaterial::Clear();

	if (Animator2D())
		Animator2D()->Clear();
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	CRenderComponent::SaveToFile(_File);

	// TexKey
	bool bTexKey = false;
	if (TEX_KEY != L"")
		bTexKey = true;

	fwrite(&bTexKey, sizeof(bool), 1, _File);
	if(bTexKey)
		SaveWStringToFile(TEX_KEY, _File);

	// CS
	bool bCS = !!m_UpdateCS.Get();;
	fwrite(&bCS, sizeof(bool), 1, _File);
	if (bCS)
		SaveWStringToFile(CS_KEY, _File);

	// option
	fwrite(&m_iMaxCount, sizeof(UINT), 1, _File);
	fwrite(&m_iAliveCount, sizeof(UINT), 1, _File);
	fwrite(&m_vStartScale, sizeof(Vec4), 1, _File);
	fwrite(&m_vEndScale, sizeof(Vec4), 1, _File);
	fwrite(&m_vStartColor, sizeof(Vec4), 1, _File);
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _File);
	fwrite(&m_vMinMaxSpeed, sizeof(Vec2), 1, _File);
	fwrite(&m_vMinMaxLifeTime, sizeof(Vec2), 1, _File);
	fwrite(&m_fSpawnRange, sizeof(Vec2), 1, _File);
	fwrite(&m_Frequency, sizeof(float), 1, _File);
	fwrite(&m_fAccTime, sizeof(float), 1, _File);
	fwrite(&m_WorldSpawn, sizeof(int), 1, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	CRenderComponent::LoadFromFile(_File);

	// TexKey
	bool bTexKey;
	fread(&bTexKey, sizeof(bool), 1, _File);
	if (bTexKey)
	{
		LoadWStringFromFile(TEX_KEY, _File);
		SetTexture(TEX_KEY);
	}

	// CS
	bool bCS;
	fread(&bCS, sizeof(bool), 1, _File);
	if (bCS)
	{
		LoadWStringFromFile(CS_KEY, _File);
		SetUpdateCS(CS_KEY);
	}

	// option
	fread(&m_iMaxCount, sizeof(UINT), 1, _File);
	fread(&m_iAliveCount, sizeof(UINT), 1, _File);
	fread(&m_vStartScale, sizeof(Vec4), 1, _File);
	fread(&m_vEndScale, sizeof(Vec4), 1, _File);
	fread(&m_vStartColor, sizeof(Vec4), 1, _File);
	fread(&m_vEndColor, sizeof(Vec4), 1, _File);
	fread(&m_vMinMaxSpeed, sizeof(Vec2), 1, _File);
	fread(&m_vMinMaxLifeTime, sizeof(Vec2), 1, _File);
	fread(&m_fSpawnRange, sizeof(Vec2), 1, _File);
	fread(&m_Frequency, sizeof(float), 1, _File);
	fread(&m_fAccTime, sizeof(float), 1, _File);
	fread(&m_WorldSpawn, sizeof(int), 1, _File);
}

