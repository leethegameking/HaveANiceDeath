#include "pch.h"
#include "CScriptMgr.h"

#include "CBlockScript.h"
#include "CControllerScript.h"
#include "CGameCameraScript.h"
#include "CPlayerScript.h"
#include "CTestScript.h"
#include "CUnitScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CBlockScript");
	_vec.push_back(L"CControllerScript");
	_vec.push_back(L"CGameCameraScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CTestScript");
	_vec.push_back(L"CUnitScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CBlockScript" == _strScriptName)
		return new CBlockScript;
	if (L"CControllerScript" == _strScriptName)
		return new CControllerScript;
	if (L"CGameCameraScript" == _strScriptName)
		return new CGameCameraScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CTestScript" == _strScriptName)
		return new CTestScript;
	if (L"CUnitScript" == _strScriptName)
		return new CUnitScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::BLOCKSCRIPT:
		return new CBlockScript;
		break;
	case (UINT)SCRIPT_TYPE::CONTROLLERSCRIPT:
		return new CControllerScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMECAMERASCRIPT:
		return new CGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTSCRIPT:
		return new CTestScript;
		break;
	case (UINT)SCRIPT_TYPE::UNITSCRIPT:
		return new CUnitScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::BLOCKSCRIPT:
		return L"CBlockScript";
		break;

	case SCRIPT_TYPE::CONTROLLERSCRIPT:
		return L"CControllerScript";
		break;

	case SCRIPT_TYPE::GAMECAMERASCRIPT:
		return L"CGameCameraScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	case SCRIPT_TYPE::UNITSCRIPT:
		return L"CUnitScript";
		break;

	}
	return nullptr;
}