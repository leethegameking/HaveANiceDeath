#include "pch.h"
#include "CMouseColliderScript.h"

CMouseColliderScript::CMouseColliderScript()
	: CScript((int)SCRIPT_TYPE::MOUSECOLLIDERSCRIPT)
{
}

CMouseColliderScript::CMouseColliderScript(const CMouseColliderScript& _origin)
	: CScript(_origin)
{
}

CMouseColliderScript::~CMouseColliderScript()
{
}

void CMouseColliderScript::begin()
{
}

void CMouseColliderScript::tick()
{
	Vec2 vMouseWorldPos = CKeyMgr::GetInst()->GetMouseDirectXPos();
	Transform()->SetRelativePos(vMouseWorldPos.x , vMouseWorldPos.y, 1.f);
}

void CMouseColliderScript::BeginOverlap(CCollider2D* _pOther)
{
}

void CMouseColliderScript::Overlap(CCollider2D* _pOther)
{
}

void CMouseColliderScript::EndOverlap(CCollider2D* _pOther)
{
}

void CMouseColliderScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
}

void CMouseColliderScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
}


