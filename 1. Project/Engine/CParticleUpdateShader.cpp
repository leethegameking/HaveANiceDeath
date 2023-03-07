#include "pch.h"
#include "CParticleUpdateShader.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"
#include "CParticleSystem.h"
#include "CAnimator2D.h"

CParticleUpdateShader::CParticleUpdateShader()
	: CComputeShader(128, 1, 1)
	, m_ParticleBuffer(nullptr)
	, m_LightTex(nullptr)
{
}

CParticleUpdateShader::~CParticleUpdateShader()
{
}


void CParticleUpdateShader::SetParticleBuffer(CStructuredBuffer* _Buffer)
{
	assert(_Buffer);

	m_ParticleBuffer = _Buffer;
	m_Param.iArr[0] = m_ParticleBuffer->GetElementCount();

	//tParticle test[8] = {};
	//m_ParticleBuffer->GetData(test, sizeof(tParticle) * 8);
	//int debug = 0;
}

void CParticleUpdateShader::SetParticleAnim(CStructuredBuffer* _Buffer)
{
	assert(_Buffer);

	m_ParticleAnim = _Buffer;

	//tParticleAnim test[50] = {};
	//m_ParticleAnim->GetData(test, sizeof(tParticleAnim)*50);
	//int debug = 0; 
}

void CParticleUpdateShader::SetParticleAnimFrm(CStructuredBuffer* _Buffer)
{
	assert(_Buffer);

	m_ParticleAnimFrm = _Buffer;

	//tAnimIdx test[8] = {};
	//m_ParticleAnimFrm->GetData(test, sizeof(tAnimIdx) * 8);
	//int debug = 0; 
}

void CParticleUpdateShader::SetLightTex(wstring _texKey)
{
	m_LightTex = CResMgr::GetInst()->FindRes<CTexture>(_texKey);
}

void CParticleUpdateShader::UpdateData()
{
	assert(m_ParticleBuffer);
	assert(m_ParticleShare);
	if (m_Owner->Animator2D())
	{
		assert(m_ParticleAnim);
		assert(m_ParticleAnimFrm);
	}

	m_ParticleBuffer->UpdateData_CS(0, false);
	m_ParticleShare->UpdateData_CS(1, false);

	if (m_Owner->Animator2D())
	{
		m_Owner->Animator2D()->UpdateData_CS();
		m_ParticleAnim->UpdateData_CS(2, false);
		m_ParticleAnimFrm->UpdateData_CS(3, false);
	}

	m_iGroupX = m_Param.iArr[0] / m_iGroupPerThreadX + 1;
	m_iGroupY = 1;
	m_iGroupZ = 1;
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear();
	m_ParticleShare->Clear();

	if (m_Owner->Animator2D())
	{
		m_ParticleAnim->Clear();
		m_ParticleAnimFrm->Clear();
	}
}
