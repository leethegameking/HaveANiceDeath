#include "pch.h"
#include "CAnimator2D.h"
#include "CAnimation2D.h"

#include "CResMgr.h"

#include "CDevice.h"
#include "CTexture.h"

CAnimator2D::CAnimator2D()
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_bRepeat(false)
{
}

CAnimator2D::CAnimator2D(const CAnimator2D& _origin)
	: CComponent(COMPONENT_TYPE::ANIMATOR2D)
	, m_bRepeat(_origin.m_bRepeat)
{
	map<wstring, Ptr<CAnimation2D>>::const_iterator iter = _origin.m_mapAnim.begin();
	wstring curAnimKey = L"";
	for (; iter != _origin.m_mapAnim.end(); ++iter)
	{
		Ptr<CAnimation2D> animClone = iter->second.Get()->Clone();
		m_mapAnim.insert(make_pair(iter->first, animClone));

		if (iter->second == _origin.m_pCurAnim)
			curAnimKey = iter->second.Get()->GetKey();
	}

	if(nullptr != _origin.m_pCurAnim)
		m_pCurAnim = m_mapAnim.find(curAnimKey)->second;
}

CAnimator2D::~CAnimator2D()
{
	map < wstring, Ptr<CAnimation2D>>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		if (nullptr != iter->second->GetMasterAnim())
		{
			vector<Ptr<CAnimation2D>>& vecChild = iter->second->GetMasterAnim().Get()->m_vecChildAnim;
			vector<Ptr<CAnimation2D>>::iterator iterChild = vecChild.begin();

			for (; iterChild != vecChild.end(); ++iterChild)
			{
				if (iterChild->Get() == iter->second.Get())
				{
					vecChild.erase(iterChild);
					break;
				}
			}
		}

		iter->second = nullptr;
	}
	// 클론 받은 애니메이션 객체를 가진 것이 사라졌을떄. MasterAnim 자식 vector에서 빼줌
}

void CAnimator2D::finaltick()
{
	if (!IsValid(m_pCurAnim.Get()))
		return;

	// Animation 이 Finish 상태이고, 반복재생을 하기로 한 경우
	if (m_pCurAnim->m_bFinish && m_bRepeat)
	{
		// 다시 0 프레임으로 리셋시켜서 동작하게 한다.
		m_pCurAnim->Reset();
	}

	// Animation 업데이트
	m_pCurAnim->finaltick();
}

Ptr<CAnimation2D> CAnimator2D::FindAnimation(const wstring& _strKey)
{
	map<wstring, Ptr<CAnimation2D>>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
		return nullptr;

	return iter->second.Get();
}

void CAnimator2D::AddAnimation(wstring _key)
{
	// can't resgister same animation
	if (m_mapAnim.find(_key) != m_mapAnim.end())
		return;

	Ptr<CAnimation2D> pAnim = CResMgr::GetInst()->FindRes<CAnimation2D>(_key);

	if (m_mapAnim.find(pAnim->GetKey()) == m_mapAnim.end())
	{
		Ptr<CAnimation2D> pAnimClone = pAnim->Clone();
		m_mapAnim.insert(make_pair(pAnimClone->GetKey(), pAnimClone));
		pAnimClone->m_pOwner = this;
		pAnimClone->m_pMasterAnim = pAnim;
		pAnimClone->SetOwner(this);
		pAnim->m_vecChildAnim.push_back(pAnimClone);
	}
}

void CAnimator2D::Play(const wstring& _strKey, bool _bRepeat)
{
	Ptr<CAnimation2D> pAnim = FindAnimation(_strKey);

	if (IsValid(pAnim.Get()))
	{
		pAnim->SetOwner(this);
		//if(m_pCurAnim.Get())
		//	m_pCurAnim->Reset();
		m_pCurAnim = pAnim;
	}

	// 반복재생
	m_bRepeat = _bRepeat;

	// 지정된 Animation 을 초기화
	if(m_pCurAnim.Get())
		m_pCurAnim->Reset();	
}

void CAnimator2D::UpdateData()
{
	if (!IsValid(m_pCurAnim.Get()))
		return;
	
	m_pCurAnim->UpdateData();
}

void CAnimator2D::UpdateData_CS()
{
	if (!IsValid(m_pCurAnim.Get()))
		return;


	m_pCurAnim->UpdateData_CS();
}



void CAnimator2D::Clear()
{
	if (!IsValid(m_pCurAnim.Get()))
		return;

	m_pCurAnim->Clear();
}

void CAnimator2D::SaveToFile(FILE* _File)
{
	COMPONENT_TYPE eType = GetType();
	fwrite(&eType, sizeof(COMPONENT_TYPE), 1, _File);
	
	// Animation Count
	size_t iAnimCount = m_mapAnim.size();
	fwrite(&iAnimCount, sizeof(size_t), 1, _File);

	// Animation
	for (const auto& pair : m_mapAnim)
	{
		SaveResourceRef(pair.second, _File);
	}

	// repeat
	fwrite(&m_bRepeat, sizeof(bool), 1, _File);

}

void CAnimator2D::LoadFromFile(FILE* _File)
{
	// Animation Count
	size_t iAnimCount = 0;
	fread(&iAnimCount, sizeof(size_t), 1, _File);

	// Animation
	for (size_t i = 0; i < iAnimCount; ++i)
	{
		Ptr<CAnimation2D> ptrAnim;
		LoadResourceRef(ptrAnim, _File);

		AddAnimation(ptrAnim->GetKey());
	}

	// repeat
	fread(&m_bRepeat, sizeof(bool), 1, _File);
}