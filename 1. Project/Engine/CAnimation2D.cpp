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
	, m_vecChildAnim{}
	, m_vPosChange(Vec2::Zero)
{
}

CAnimation2D::CAnimation2D(CAnimation2D& _origin)
	: CRes(_origin)
	, m_pMasterAnim(&_origin)
	, m_iCurIdx(0)
	, m_pOwner(nullptr)
	, m_AtlasTex(_origin.m_AtlasTex)
	, m_fAccTime(0.f)
	, m_bFinish(false)
	, m_vecFrm(_origin.m_vecFrm)
	, m_vecChildAnim{}
{
	// 마스터
	if (_origin.m_pMasterAnim.Get() == nullptr)
		m_pMasterAnim = &_origin;
}


CAnimation2D::~CAnimation2D()
{
	int a = 0;

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

bool CAnimation2D::OFAFinish()
{
	static bool bOneFrameAfter = false;
	if (m_bFinish)
		bOneFrameAfter = true;

	if (bOneFrameAfter)
	{
		bOneFrameAfter = false;
		return m_bFinish;
	}
	return false;
}

void CAnimation2D::Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex
	, Vec2 _vLeftTop, Vec2 _vOffset, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS, Vec2 _vFullsize, bool _bVTHZ)
{
	// Animation Name
	SetName(_strKey);

	// Atlas Texture
	m_AtlasTex = _AtlasTex;

	float fWidth = (float)m_AtlasTex->GetWidth();
	float fHeight = (float)m_AtlasTex->GetHeight();
	Vec2 vSize = Vec2(fWidth, fHeight);

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
		frm.vOffset = _vOffset / vSize;

		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::Create(const wstring& _strKey, Ptr<CTexture> _AtlasTex, const vector<tAnim2DFrm>& _vecFrm)
{
	// Animation Name
	SetName(_strKey);

	// Atlas Texture
	m_AtlasTex = _AtlasTex;

	m_vecFrm.assign(_vecFrm.begin(), _vecFrm.end());
}





void CAnimation2D::Reallocate()
{
	for (size_t i = 0; i < m_vecChildAnim.size(); ++i)
	{
		m_vecChildAnim[i]->SetName(GetName());
		m_vecChildAnim[i]->m_vecFrm.assign(m_vecFrm.begin(), m_vecFrm.end());
		m_vecChildAnim[i]->m_iCurIdx = 0;
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


void CAnimation2D::Save(const wstring& _strRelativeFilePath)
{
	if (!CheckRelativePath(_strRelativeFilePath))
	{
		assert(nullptr);
		return;
	}

	
	wstring strPath = CONTENTPATH;
	strPath += _strRelativeFilePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strPath.c_str(), L"wb");

	// Key RelativePath 
	SaveKeyPath(pFile);

	// frame size
	size_t vecSize =  m_vecFrm.size();
	fwrite(&vecSize, sizeof(size_t), 1, pFile);

	// vector frame
	fwrite(m_vecFrm.data(), sizeof(tAnim2DFrm), vecSize, pFile);

	// texture
	SaveResourceRef(m_AtlasTex, pFile);

	// MasterAnim
	SaveResourceRef(m_pMasterAnim, pFile);

	// PosChange
	fwrite(&m_vPosChange, sizeof(Vec2), 1, pFile);

	fclose(pFile);
}

int CAnimation2D::Load(const wstring& _strFilePath)
{
	FILE* pFile = nullptr;
	if (FAILED(_wfopen_s(&pFile, _strFilePath.c_str(), L"rb")))
		return E_FAIL;
	
	// Key RelativePath 
	LoadKeyPath(pFile);

	// frame size
	size_t vecSize = 0;
	fread(&vecSize, sizeof(size_t), 1, pFile);
	m_vecFrm.resize(vecSize);

	// vector frame
	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fread(&m_vecFrm[i], sizeof(tAnim2DFrm), 1, pFile);
	}

	// texture
	LoadResourceRef(m_AtlasTex, pFile);

	// MasterAnim
	LoadResourceRef(m_pMasterAnim, pFile);

	if (nullptr != m_pMasterAnim)
	{
		m_pMasterAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(GetKey());
		m_pMasterAnim->m_vecChildAnim.push_back(this);
	}
	
	// PosChange
	fread(&m_vPosChange, sizeof(Vec2), 1, pFile);

	fclose(pFile);


	return S_OK;
}
