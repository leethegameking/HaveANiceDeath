#include "pch.h"
#include "CEnemyScript.h"

#include "CPlayerMgr.h"


CEnemyScript::CEnemyScript()
{
}

CEnemyScript::CEnemyScript(int _iScriptType)
	: CUnitScript(_iScriptType)
	, m_pProjectile(nullptr)
	, m_eCurPattern(0)
	, m_ePrevPattern(-1)
	, m_fAppearRadius(200.f)
	, m_fDetectRadius(100.f)
	, m_fAttackRadius(50.f)
	, m_fFaintGauge(10.f)
	, m_fFaintTime(3.f)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Projectile", &m_pProjectile);
}

CEnemyScript::CEnemyScript(const CEnemyScript& _origin)
	: CUnitScript(_origin)
	, m_pProjectile(nullptr)
	, m_eCurPattern(0)
	, m_ePrevPattern(-1)
	, m_fAppearRadius(200.f)
	, m_fDetectRadius(100.f)
	, m_fAttackRadius(50.f)
	, m_fFaintGauge(10.f)
	, m_fFaintTime(3.f)
{
	AddScriptParam(SCRIPT_PARAM::PREFAB, "Projectile", &m_pProjectile);
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

void CEnemyScript::SaveToFile(FILE* _pFile)
{
	CUnitScript::SaveToFile(_pFile);

	bool bProjectile = false;
	if (m_pProjectile.Get())
		bProjectile = true;

	fwrite(&bProjectile, sizeof(bool), 1, _pFile);
	if (bProjectile)
		SaveResourceRef<CPrefab>(m_pProjectile, _pFile);
}

void CEnemyScript::LoadFromFile(FILE* _pFile)
{
	CUnitScript::LoadFromFile(_pFile);

	bool bProjectile = false;
	fread(&bProjectile, sizeof(bool), 1, _pFile);
	if (bProjectile)
		LoadResourceRef<CPrefab>(m_pProjectile, _pFile);
}



