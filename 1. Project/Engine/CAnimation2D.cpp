#include "pch.h"
#include "CAnimation2D.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CResMgr.h"

#include "CTimeMgr.h"
#include "CTexture.h"



CAnimation2D::CAnimation2D()
	: CRes(RES_TYPE::ANIMATION2D)
	, m_pMasterAnim(nullptr)
	, m_iCurIdx(-1)
	, m_pOwner(nullptr)
	, m_fAccTime(0.f)
	, m_vecAnim{}
{
}

CAnimation2D::CAnimation2D(CAnimation2D& _origin)
	: CRes(_origin)
	, m_pMasterAnim(&_origin)
	, m_iCurIdx(-1)
	, m_pOwner(nullptr)
	, m_AtlasTex(_origin.m_AtlasTex)
	, m_fAccTime(0.f)
	, m_bFinish(false)
	, m_vecFrm(_origin.m_vecFrm)
	, m_vecAnim{}
{
}


CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::finaltick()
{


	if (m_bFinish)
		return;

	// 시간 체크
	m_fAccTime += DT;


	// 누적 시간이 해당 프레임 유지시간을 넘어서면 다음프레임으로 넘어감
	if (m_vecFrm[m_iCurIdx].fDuration < m_fAccTime)
	{
		m_fAccTime = 0.f;
		++m_iCurIdx;

		// 최대 프레임에 도달하면 Finish 상태로 전환
		if (m_vecFrm.size() <= m_iCurIdx)
		{
			m_iCurIdx = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}	
}

void CAnimation2D::Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS, Vec2 _vFullsize, bool _bVTHZ)
{
	// Animation Name
	SetName(_strKey);

	// Atlas Texture
	m_AtlasTex = _AtlasTex;

	float fWidth = (float)m_AtlasTex->GetWidth();
	float fHeight = (float)m_AtlasTex->GetHeight();

	// Frame Info
	for (int i = 0; i < _iMaxFrm; ++i)
	{
		tAnim2DFrm frm = {};
		if(_bVTHZ == HORIZONTAL)
			frm.vLeftTop = Vec2((_vLeftTop.x + _fStep * (float)i) / fWidth, _vLeftTop.y / fHeight);
		if(_bVTHZ == VERTICAL)
			frm.vLeftTop = Vec2(_vLeftTop.x / fWidth, (_vLeftTop.y +_fStep * (float)i) / fHeight);
		frm.vSlice = Vec2(_vSlice.x / fWidth, _vSlice.y / fHeight);
		frm.fDuration = 1.f / _FPS;

		frm.vFullSize = Vec2(_vFullsize.x / fWidth, _vFullsize.y / fHeight);
		/*if (i == 5)
		{
			frm.vOffset = Vec2(20.f / fWidth, 30.f / fHeight);
		}*/

		m_vecFrm.push_back(frm);
	}

	CResMgr::GetInst()->AddRes<CAnimation2D>(this->GetName(), (CAnimation2D*)this);
}



void CAnimation2D::Reallocate()
{
	for (size_t i = 0; i < m_vecAnim.size(); ++i)
	{
		m_vecAnim[i]->m_vecFrm.assign(m_vecFrm.begin(), m_vecFrm.end());
		m_vecAnim[i]->m_iCurIdx = 0;
	}
}

void CAnimation2D::UpdateData()
{
	m_AtlasTex->UpdateData(12, PIPELINE_STAGE::PS);

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	tAnim2DInfo info = {};
	info.iAnim2DUse = 1;
	info.vLeftTop = m_vecFrm[m_iCurIdx].vLeftTop;
	info.vSlice = m_vecFrm[m_iCurIdx].vSlice;
	info.vFullSize = m_vecFrm[m_iCurIdx].vFullSize;
	info.vOffset = m_vecFrm[m_iCurIdx].vOffset;
	
	pCB->SetData(&info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}

void CAnimation2D::Clear()
{
	CTexture::Clear(12);	

	static CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::ANIMATION2D);

	tAnim2DInfo info = {};
	info.iAnim2DUse = 0;
	
	pCB->SetData(&info);
	pCB->UpdateData(PIPELINE_STAGE::PS);
}