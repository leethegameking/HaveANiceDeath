#include "pch.h"
#include "CPlayerScript.h"

wstring PlayerKey[PLAYER_ANIM_END] =
{
	L"PlayerIdle",
	L"PlayerIdleToRun",
	L"PlayerRun",
	L"PlayerRunToIdle",
	L"PlayerIdleUturn",
	L"PlayerRunUturn",
};

CPlayerScript::CPlayerScript()
	: CScript(PLAYERSCRIPT)
	, m_pCurAnim(nullptr)
	, m_pPrevAnim(nullptr)
	, m_eCurDir(ANIM_DIR::RIGHT)
	, m_ePrevDir(ANIM_DIR::RIGHT)
	, m_iAnimCondBit(0)
	, m_fSpeed(100.f)
	, m_eCurAnim(PLAYER_ANIM_END)
	, m_ePrevAnim(PLAYER_ANIM_END)
	, m_bEnsureFinish(false)
	, m_bGround(true) 
	, m_bDirChanged(false)
{
}



CPlayerScript::~CPlayerScript()
{
}


void CPlayerScript::begin()
{

}

void CPlayerScript::tick()
{
	m_pCurAnim = Animator2D()->GetCurAnim();

	GetConditionBit();
	GetAnimEnum();

	// Ensure Finish
	if (m_bEnsureFinish)
	{
		if ((m_iAnimCondBit & ANIM_FINISHED) == ANIM_FINISHED)
			m_bEnsureFinish = false;
		else
		{
			if (m_eCurAnim == PlayerIdleToRun)
			{
				MoveHorizontal();
			}
			return;
		}
	}
	  
	// play Idle
	if ((m_eCurAnim == PlayerRun) && (m_iAnimCondBit & (GROUND)) == (GROUND) && !KEY_PRESSED(KEY::A) && !KEY_PRESSED(KEY::D))
	{
		Animator2D()->Play(L"PlayerRunToIdle", false);
		m_bEnsureFinish = true;
	}

	if (m_eCurAnim != PlayerRun && (m_iAnimCondBit & (GROUND | ANIM_FINISHED)) == (GROUND | ANIM_FINISHED))
	{
		Animator2D()->Play(L"PlayerIdle", false);
	}

	if (KEY_PRESSED(KEY::A) || KEY_TAP(KEY::A))
	{
		SetAnimLeft();
		if (m_eCurAnim == PlayerIdle && (m_iAnimCondBit & (GROUND | ANIM_DIR_CHANGED)) == (GROUND | ANIM_DIR_CHANGED))
		{
			Animator2D()->Play(L"PlayerIdleUturn");
			m_bEnsureFinish = true;
		}
		else if (m_eCurAnim == PlayerRun && (m_iAnimCondBit & (GROUND | ANIM_DIR_CHANGED)) == (GROUND | ANIM_DIR_CHANGED))
		{
			Animator2D()->Play(L"PlayerRunUturn");
			m_bEnsureFinish = true;
		}
		else if (m_eCurAnim == PlayerIdle && (m_iAnimCondBit & (GROUND | ANIM_DIR_NOT_CHANGED)) == (GROUND | ANIM_DIR_NOT_CHANGED))
		{
			Animator2D()->Play(L"PlayerIdleToRun");
			m_bEnsureFinish = true;
		}
		else if (m_eCurAnim & (PlayerIdleToRun | PlayerRun | PlayerRunUturn) && (m_iAnimCondBit & (GROUND | ANIM_FINISHED)) == (GROUND | ANIM_FINISHED))
		{
			Animator2D()->Play(L"PlayerRun");
		}
		// Uturn을 안했다면.
		else
		{
			m_eCurDir = m_ePrevDir;
		}

		MoveHorizontal();
	}
	else if (KEY_PRESSED(KEY::D) || KEY_TAP(KEY::D))
	{
		SetAnimRight();
		if (m_eCurAnim == PlayerIdle && (m_iAnimCondBit & (GROUND | ANIM_DIR_CHANGED)) == (GROUND | ANIM_DIR_CHANGED))
		{
			Animator2D()->Play(L"PlayerIdleUturn");
			m_bEnsureFinish = true;
		}
		else if (m_eCurAnim == PlayerRun && (m_iAnimCondBit & (GROUND | ANIM_DIR_CHANGED)) == (GROUND | ANIM_DIR_CHANGED))
		{
			Animator2D()->Play(L"PlayerRunUturn");
			m_bEnsureFinish = true;
		}
		else if (m_eCurAnim == PlayerIdle && (m_iAnimCondBit & (GROUND | ANIM_DIR_NOT_CHANGED)) == (GROUND | ANIM_DIR_NOT_CHANGED))
		{
			Animator2D()->Play(L"PlayerIdleToRun");
			m_bEnsureFinish = true;
		}
		else if (m_eCurAnim & (PlayerIdleToRun | PlayerRun | PlayerRunUturn) && (m_iAnimCondBit & (GROUND | ANIM_FINISHED)) == (GROUND | ANIM_FINISHED))
		{
			Animator2D()->Play(L"PlayerRun");
		}
		// Uturn을 안했다면.
		else
		{
			m_eCurDir = m_ePrevDir;
		}

		MoveHorizontal();
	}


	SetPrevValues();
}

void CPlayerScript::BeginOverlap(CCollider2D* _pOther)
{

}

void CPlayerScript::Overlap(CCollider2D* _pOther)
{
}

void CPlayerScript::EndOverlap(CCollider2D* _pOther)
{
}

void CPlayerScript::GetConditionBit()
{
	m_iAnimCondBit = 0;

	GetAnimPlaying();
	GetAnimChanged();
	GetAnimRepeat();
	GetGround();
}

void CPlayerScript::GetAnimPlaying()
{
	if (m_pCurAnim.Get())
	{
		if (!m_pCurAnim->IsFinish())
			m_iAnimCondBit |= ANIM_PLAYING;
		else
			m_iAnimCondBit |= ANIM_FINISHED;
	}
	else
		m_iAnimCondBit |= ANIM_FINISHED;
}

void CPlayerScript::GetAnimChanged()
{
	if (m_pCurAnim.Get())
	{
		if (m_pCurAnim.Get() != m_pPrevAnim.Get())
			m_iAnimCondBit |= ANIM_CHANGED;
		else
			m_iAnimCondBit |= ANIM_NOT_CHANGED;
	}
}

void CPlayerScript::GetDirChanged()
{
	if (m_ePrevDir != m_eCurDir)
	{
		m_bDirChanged = true;
		m_iAnimCondBit |= ANIM_DIR_CHANGED;
	}
	else
	{
		m_bDirChanged = false;
		m_iAnimCondBit |= ANIM_DIR_NOT_CHANGED;
	}
}

void CPlayerScript::SetAnimLeft()
{
	m_eCurDir = ANIM_DIR::LEFT;
	GetDirChanged();
	Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
}

void CPlayerScript::SetAnimRight()
{
	m_eCurDir = ANIM_DIR::RIGHT;
	GetDirChanged();
	Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
}

void CPlayerScript::GetAnimRepeat()
{
	if (Animator2D()->IsRepeat())
		m_iAnimCondBit |= ANIM_REPEAT;
	else
		m_iAnimCondBit |= ANIM_NOT_REPEAT;
}

void CPlayerScript::GetGround()
{
	if (m_bGround)
		m_iAnimCondBit |= GROUND;
	else
		m_iAnimCondBit |= AERIAL;
}

void CPlayerScript::GetAnimEnum()
{
	if (m_pCurAnim.Get())
	{
		wstring strAnimKey = m_pCurAnim->GetKey();
		for (int i = 0; i < PLAYER_ANIM_END; ++i)
		{
			if (strAnimKey._Equal(PlayerKey[i]))
			{
				m_eCurAnim = (PLAYER_ANIM)(1 << i);
				break;
			}
		}
	}
}

void CPlayerScript::EnsureFinish()
{
	if (m_bEnsureFinish)
	{
		if ((m_iAnimCondBit & ANIM_FINISHED) == ANIM_FINISHED)
			m_bEnsureFinish = false;
		else
			return;
	}
}

void CPlayerScript::SetPrevValues()
{
	m_ePrevDir = m_eCurDir;
	m_pPrevAnim = m_pCurAnim;
	m_ePrevAnim = m_eCurAnim;
}

void CPlayerScript::MoveHorizontal()
{
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += (int)m_eCurDir * m_fSpeed * DT;
	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::MoveHorizontalReverse()
{
	Vec3 vPos = Transform()->GetRelativePos();
	vPos.x += -(int)m_eCurDir * m_fSpeed * DT;
	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CPlayerScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}
