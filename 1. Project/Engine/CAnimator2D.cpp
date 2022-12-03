#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

#include "CDevice.h"
#include "CTexture.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_bRepeat(false)
{
}

CAnimator2D::~CAnimator2D()
{
	Safe_Del_Map(m_mapAnim);
}

void CAnimator2D::finaltick()
{
	if (!IsValid(m_pCurAnim))
		return;

	// Animation �� Finish �����̰�, �ݺ������ �ϱ�� �� ���
	if (m_pCurAnim->m_bFinish && m_bRepeat)
	{
		// �ٽ� 0 ���������� ���½��Ѽ� �����ϰ� �Ѵ�.
		m_pCurAnim->Reset();
	}

	// Animation ������Ʈ
	m_pCurAnim->finaltick();
}

void CAnimator2D::CreateAnimation(const wstring& _strKey, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, float _fStep, int _iMaxFrm, float _FPS)
{
	assert(_AtlasTex.Get());

	CAnimation2D* pAnim = FindAnimation(_strKey);
	assert(!pAnim);

	pAnim = new CAnimation2D;
	pAnim->Create(_strKey, _AtlasTex, _vLeftTop, _vSlice, _fStep, _iMaxFrm, _FPS);

	pAnim->m_pOwner = this;
	m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
}

CAnimation2D* CAnimator2D::FindAnimation(const wstring& _strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second;
}

void CAnimator2D::Play(const wstring& _strKey, bool _bRepeat)
{
	CAnimation2D* pAnim = FindAnimation(_strKey);

	if (IsValid(pAnim))
	{
		if(m_pCurAnim)
			m_pCurAnim->Reset();
		m_pCurAnim = pAnim;
	}

	// �ݺ����
	m_bRepeat = _bRepeat;

	// ������ Animation �� �ʱ�ȭ
	m_pCurAnim->Reset();	
}

void CAnimator2D::UpdateData()
{
	if (!IsValid(m_pCurAnim))
		return;
	
	m_pCurAnim->UpdateData();
}

void CAnimator2D::Clear()
{
	if (!IsValid(m_pCurAnim))
		return;

	m_pCurAnim->Clear();
}
