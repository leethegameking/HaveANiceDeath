#include "pch.h"
#include "CTestScript.h"

#include <Engine/CResMgr.h>
#include <Engine/CPrefab.h>
#include <Engine/CEventMgr.h>

CTestScript::CTestScript()
	: CScript(TESTSCRIPT)
	, fSpeed(0.f)
	, vPos(Vec3(0.f,0.f,0.f))
	, vDir(Vec2(0.f, 0.f))
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &fSpeed);
	AddScriptParam(SCRIPT_PARAM::VEC2, "DIr",  &vDir);
	AddScriptParam(SCRIPT_PARAM::VEC3, "Pos", &vPos);

	testPrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"testPrefab");
}

CTestScript::~CTestScript()
{
}

void CTestScript::tick()
{
	Vec3 TargetPos = Transform()->GetRelativePos();
	TargetPos.x += vDir.x * fSpeed * DT;
	TargetPos.y += vDir.y * fSpeed * DT;

	Transform()->SetRelativePos(TargetPos);

	if (KEY_TAP(KEY::ENTER))
	{
		Transform()->SetRelativePos(vPos);
	}

	// save prefab test
	if (KEY_TAP(KEY::P))
	{
		testPrefab->Save(L"prefab\\testPrefab");
	}

	// 
	if (KEY_TAP(KEY::U))
	{
		tEvent evn = {};
		evn.eType = EVENT_TYPE::BURRY_OBJECT;
		evn.wParam = DWORD_PTR(GetOwner());
		CEventMgr::GetInst()->AddEvent(evn);
	}

	if (KEY_TAP(KEY::I))
	{
		Collider2D()->Activate();
	}
}


void CTestScript::SaveToFile(FILE* _pFile)
{
	CScript::SaveToFile(_pFile);
	fwrite(&fSpeed, sizeof(float), 1, _pFile);
	fwrite(&vPos, sizeof(Vec2), 1, _pFile);
	SaveResourceRef<CPrefab>(testPrefab, _pFile);
}

void CTestScript::LoadFromFile(FILE* _pFile)
{
	CScript::LoadFromFile(_pFile);
	fread(&fSpeed, sizeof(float), 1, _pFile);
	fread(&vPos, sizeof(Vec2), 1, _pFile);
	LoadResourceRef<CPrefab>(testPrefab, _pFile);
}



