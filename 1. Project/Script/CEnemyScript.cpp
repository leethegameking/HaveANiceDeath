#include "pch.h"
#include "CEnemyScript.h"

#include "CPlayerMgr.h"
#include <Engine/CEventMgr.h>

CEnemyScript::CEnemyScript()
{
}

CEnemyScript::CEnemyScript(int _iScriptType)
	: CUnitScript(_iScriptType)
	, m_pProjectile(nullptr)
	, m_eCurPattern(0)
	, m_ePrevPattern(-1)
	, m_fAppearRadius(700.f)
	, m_fDetectRadius(500.f)
	, m_fAttackRadius(500.f)
	, m_fFaintGauge(10.f)
	, m_fFaintTime(3.f)
	, m_bUturn(false)
	, m_bDeath(false)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Projectile", &m_pProjectile);

	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Detect", &m_FX_Detect);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Death_1", &m_FX_Death_1);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Death_2", &m_FX_Death_2);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-StunStar", &m_FX_StunStar);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Alert", &m_FX_Alert);

	AddScriptParam(SCRIPT_PARAM::INT, "pattern", &m_eCurPattern);
}

CEnemyScript::CEnemyScript(const CEnemyScript& _origin)
	: CUnitScript(_origin)
	, m_pProjectile(_origin.m_pProjectile)
	, m_eCurPattern(0)
	, m_ePrevPattern(-1)
	, m_fAppearRadius(700.f)
	, m_fDetectRadius(500.f)
	, m_fAttackRadius(500.f)
	, m_fFaintGauge(10.f)
	, m_fFaintTime(3.f)
	, m_bUturn(false)
	, m_bDeath(false)
	, m_FX_Detect(_origin.m_FX_Detect)
	, m_FX_Death_1(_origin.m_FX_Death_1)
	, m_FX_Death_2(_origin.m_FX_Death_2)
	, m_FX_StunStar(_origin.m_FX_StunStar)
	, m_FX_Alert(_origin.m_FX_Alert)

{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Projectile", &m_pProjectile);

	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Detect", &m_FX_Detect);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Death_1", &m_FX_Death_1);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Death_2", &m_FX_Death_2);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-StunStar", &m_FX_StunStar);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "FX-Alert", &m_FX_Alert);

	AddScriptParam(SCRIPT_PARAM::INT, "pattern", &m_eCurPattern);
}

CEnemyScript::~CEnemyScript()
{
}

void CEnemyScript::begin()
{
	CUnitScript::begin();
	// 
	m_pPlayerObj = CPlayerMgr::GetInst()->GetPlayerObj();

	// Collider 가져옴
	const vector<CGameObject*>& vecChildObj = GetOwner()->GetChildObject();
	for (size_t i = 0; i < vecChildObj.size(); ++i)
	{
		if (vecChildObj[i]->GetLayerIdx() == (int)LAYER_NAME::ENEMY_HIT)
		{
			m_pHitObj = vecChildObj[i];
		}
		else if (vecChildObj[i]->GetLayerIdx() == (int)LAYER_NAME::ENEMY_ATTACK)
		{
			m_pAttObj = vecChildObj[i];
			m_pAttObj->Collider2D()->SetOffsetPos(EXPEL * (int)LAYER_NAME::ENEMY_ATTACK);
		}
	}
}

void CEnemyScript::tick()
{
	CUnitScript::tick();

	if (m_eCurPattern != m_ePrevPattern)
	{
		m_bStateEnter = true;
		m_ePrevPattern = m_eCurPattern;
	}

	// Faint가 0이고 HIT 관련 State가 아닐 경우.
	if (m_fFaintGauge < 0.f && !CalBit(m_eCurPattern, PATTERN_HIT_START | PATTERN_HIT_LOOP | PATTERN_HIT_END | PATTERN_DEATH, BIT_LEAST_ONE))
	{
		m_bStateEnter = true;
		m_eCurPattern = PATTERN_HIT_START;
		m_ePrevPattern = m_eCurPattern;
	}

	// Death
	if (m_CurUnitInfo.m_fHP <= 0.f && m_bDeath == false)
	{
		m_bDeath = true;
		m_bStateEnter = true;
		m_eCurPattern = PATTERN_DEATH;
		m_ePrevPattern = m_eCurPattern;
	}
}

void CEnemyScript::BeginOverlap(CCollider2D* _pOther)
{

}

void CEnemyScript::Overlap(CCollider2D* _pOther)
{
	// Player일때만
	if (!_pOther->GetOwner()->GetScript<CUnitScript>())
		return;

	static float fSpeed = 350.f;

	// Player
	Vec3 vPlayerPos = _pOther->Transform()->GetRelativePos();
	if (vPlayerPos.x < Transform()->GetRelativePos().x)
		vPlayerPos.x -= fSpeed * DT;
	else
		vPlayerPos.x += fSpeed * DT;
	_pOther->Transform()->SetRelativePos(vPlayerPos);
	_pOther->Transform()->finaltick();
	_pOther->Collider2D()->finaltick();
}

void CEnemyScript::EndOverlap(CCollider2D* _pOther)
{
}

bool CEnemyScript::CurAnimFinish()
{
	if (Animator2D()->GetCurAnim()->IsFinish())
		return true;
	else
		return false;
}

void CEnemyScript::FX_Detect()
{
	if (m_FX_Detect.Get())
	{
		CGameObject* pFXObj = m_FX_Detect->Instantiate();
		Vec3 v3InstPos = Vec3( 50.f , Collider2D()->GetFinalScale().y / 2.f + 30.f, 0.f);
		pFXObj->Transform()->SetRelativePos(v3InstPos);

		tEvent evn;
		evn.eType = EVENT_TYPE::ADD_CHILD;
		evn.wParam = (DWORD_PTR)pFXObj;
		evn.lParam = (DWORD_PTR)GetOwner();

		CEventMgr::GetInst()->AddEvent(evn);
	}
}

void CEnemyScript::FX_Alert()
{
	if (m_FX_Alert.Get())
	{
		CGameObject* pFXObj = m_FX_Alert->Instantiate();
		tEvent evn;
		evn.eType = EVENT_TYPE::ADD_CHILD;
		evn.wParam = (DWORD_PTR)pFXObj;
		evn.lParam = (DWORD_PTR)GetOwner();

		CEventMgr::GetInst()->AddEvent(evn);
	}
}

void CEnemyScript::FX_Death_1()
{
	if (m_FX_Death_1.Get())
	{
		CGameObject* pFXObj =  m_FX_Death_1->Instantiate();
		Instantiate(pFXObj, Transform()->GetWorldPos());
	}
}

void CEnemyScript::FX_Death_2()
{
	if (m_FX_Death_2.Get())
	{
		CGameObject* pFXObj = m_FX_Death_2->Instantiate();
		Instantiate(pFXObj, Transform()->GetWorldPos());
	}
}

void CEnemyScript::FX_StunStar()
{
	
	if (m_FX_StunStar.Get())
	{
		static float fCorrection = 30.f;

		CGameObject* pFXObj = m_FX_StunStar->Instantiate();
		Vec3 vPos = Vec3(0.f, m_pHitObj->Collider2D()->GetFinalScale().y / 2.f + fCorrection, 0.f);
		pFXObj->Transform()->SetRelativePos(vPos);
		AddChild(pFXObj, GetOwner());
	}
}



void CEnemyScript::SaveToFile(FILE* _pFile)
{
	CUnitScript::SaveToFile(_pFile);

	bool bProjectile = false;
	if (m_pProjectile.Get())
		bProjectile = true;

	fwrite(&bProjectile, sizeof(bool), 1, _pFile);
	if (bProjectile)
		SaveResourceRef<CPrefab>(m_pProjectile, _pFile);

	SaveResourceRef(m_FX_Detect, _pFile);
	SaveResourceRef(m_FX_Death_1, _pFile);
	SaveResourceRef(m_FX_Death_2, _pFile);
	SaveResourceRef(m_FX_StunStar, _pFile);
	SaveResourceRef(m_FX_Alert, _pFile);
}

void CEnemyScript::LoadFromFile(FILE* _pFile)
{
	CUnitScript::LoadFromFile(_pFile);

	bool bProjectile = false;
	fread(&bProjectile, sizeof(bool), 1, _pFile);
	if (bProjectile)
		LoadResourceRef<CPrefab>(m_pProjectile, _pFile);

	LoadResourceRef(m_FX_Detect, _pFile);
	LoadResourceRef(m_FX_Death_1, _pFile);
	LoadResourceRef(m_FX_Death_2, _pFile);
	LoadResourceRef(m_FX_StunStar, _pFile);
	LoadResourceRef(m_FX_Alert, _pFile);
}

ANIM_DIR CEnemyScript::GetAnimDirToPlayer()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();
	float fDir = vPlayerPos.x - vPos.x;
	ANIM_DIR eDir;
	if (fDir <= 0.f)
		eDir = ANIM_DIR::ANIM_LEFT;
	else
		eDir = ANIM_DIR::ANIM_RIGHT;

	return eDir;
}

Vec2 CEnemyScript::GetDirToPlayer()
{
	Vec3 vPos = Transform()->GetRelativePos();
	Vec3 vPlayerPos = m_pPlayerObj->Transform()->GetRelativePos();
	Vec2 vDir = Vec2(vPlayerPos.x - vPos.x, vPlayerPos.y - vPos.y);
	vDir.Normalize();

	return vDir;
}

void CEnemyScript::SetDir(ANIM_DIR _eDir)
{
	m_CurUnitInfo.m_eDir = _eDir;
	if (_eDir == ANIM_DIR::ANIM_LEFT)
		Transform()->SetRelativeRotation(0.f, XM_PI, 0.f);
	else
		Transform()->SetRelativeRotation(0.f, 0.f, 0.f);
}
