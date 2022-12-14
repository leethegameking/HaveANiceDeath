#include "pch.h"
#include "CTestScript.h"

CTestScript::CTestScript()
	: CScript(TESTSCRIPT)
	, fSpeed(0.f)
	, vPos(Vec3(0.f,0.f,0.f))
	, vDir(Vec2(0.f, 0.f))
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "Speed", &fSpeed);
	AddScriptParam(SCRIPT_PARAM::VEC2, "DIr",  &vDir);
	AddScriptParam(SCRIPT_PARAM::VEC3, "Pos", &vPos);
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
}


