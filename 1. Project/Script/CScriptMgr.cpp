#include "pch.h"
#include "CScriptMgr.h"

#include "CAnimController.h"
#include "CAttScript.h"
#include "CBlockScript.h"
#include "CControllerScript.h"
#include "CEnemyAttScript.h"
#include "CEnemyController.h"
#include "CEnemyScript.h"
#include "CGameCameraScript.h"
#include "CHitScript.h"
#include "CPlayerAttScript.h"
#include "CPlayerScript.h"
#include "CProjectileScript.h"
#include "CTestScript.h"
#include "CUnitScript.h"
#include "CWorkman.h"
#include "CWorkwoman.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAnimController");
	_vec.push_back(L"CAttScript");
	_vec.push_back(L"CBlockScript");
	_vec.push_back(L"CControllerScript");
	_vec.push_back(L"CEnemyAttScript");
	_vec.push_back(L"CEnemyController");
	_vec.push_back(L"CEnemyScript");
	_vec.push_back(L"CGameCameraScript");
	_vec.push_back(L"CHitScript");
	_vec.push_back(L"CPlayerAttScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CProjectileScript");
	_vec.push_back(L"CTestScript");
	_vec.push_back(L"CUnitScript");
	_vec.push_back(L"CWorkman");
	_vec.push_back(L"CWorkwoman");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAnimController" == _strScriptName)
		return new CAnimController;
	if (L"CAttScript" == _strScriptName)
		return new CAttScript;
	if (L"CBlockScript" == _strScriptName)
		return new CBlockScript;
	if (L"CControllerScript" == _strScriptName)
		return new CControllerScript;
	if (L"CEnemyAttScript" == _strScriptName)
		return new CEnemyAttScript;
	if (L"CEnemyController" == _strScriptName)
		return new CEnemyController;
	if (L"CEnemyScript" == _strScriptName)
		return new CEnemyScript;
	if (L"CGameCameraScript" == _strScriptName)
		return new CGameCameraScript;
	if (L"CHitScript" == _strScriptName)
		return new CHitScript;
	if (L"CPlayerAttScript" == _strScriptName)
		return new CPlayerAttScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CProjectileScript" == _strScriptName)
		return new CProjectileScript;
	if (L"CTestScript" == _strScriptName)
		return new CTestScript;
	if (L"CUnitScript" == _strScriptName)
		return new CUnitScript;
	if (L"CWorkman" == _strScriptName)
		return new CWorkman;
	if (L"CWorkwoman" == _strScriptName)
		return new CWorkwoman;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ANIMCONTROLLER:
		return new CAnimController;
		break;
	case (UINT)SCRIPT_TYPE::ATTSCRIPT:
		return new CAttScript;
		break;
	case (UINT)SCRIPT_TYPE::BLOCKSCRIPT:
		return new CBlockScript;
		break;
	case (UINT)SCRIPT_TYPE::CONTROLLERSCRIPT:
		return new CControllerScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYATTSCRIPT:
		return new CEnemyAttScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYCONTROLLER:
		return new CEnemyController;
		break;
	case (UINT)SCRIPT_TYPE::ENEMYSCRIPT:
		return new CEnemyScript;
		break;
	case (UINT)SCRIPT_TYPE::GAMECAMERASCRIPT:
		return new CGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::HITSCRIPT:
		return new CHitScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERATTSCRIPT:
		return new CPlayerAttScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PROJECTILESCRIPT:
		return new CProjectileScript;
		break;
	case (UINT)SCRIPT_TYPE::TESTSCRIPT:
		return new CTestScript;
		break;
	case (UINT)SCRIPT_TYPE::UNITSCRIPT:
		return new CUnitScript;
		break;
	case (UINT)SCRIPT_TYPE::WORKMAN:
		return new CWorkman;
		break;
	case (UINT)SCRIPT_TYPE::WORKWOMAN:
		return new CWorkwoman;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ANIMCONTROLLER:
		return L"CAnimController";
		break;

	case SCRIPT_TYPE::ATTSCRIPT:
		return L"CAttScript";
		break;

	case SCRIPT_TYPE::BLOCKSCRIPT:
		return L"CBlockScript";
		break;

	case SCRIPT_TYPE::CONTROLLERSCRIPT:
		return L"CControllerScript";
		break;

	case SCRIPT_TYPE::ENEMYATTSCRIPT:
		return L"CEnemyAttScript";
		break;

	case SCRIPT_TYPE::ENEMYCONTROLLER:
		return L"CEnemyController";
		break;

	case SCRIPT_TYPE::ENEMYSCRIPT:
		return L"CEnemyScript";
		break;

	case SCRIPT_TYPE::GAMECAMERASCRIPT:
		return L"CGameCameraScript";
		break;

	case SCRIPT_TYPE::HITSCRIPT:
		return L"CHitScript";
		break;

	case SCRIPT_TYPE::PLAYERATTSCRIPT:
		return L"CPlayerAttScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PROJECTILESCRIPT:
		return L"CProjectileScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	case SCRIPT_TYPE::UNITSCRIPT:
		return L"CUnitScript";
		break;

	case SCRIPT_TYPE::WORKMAN:
		return L"CWorkman";
		break;

	case SCRIPT_TYPE::WORKWOMAN:
		return L"CWorkwoman";
		break;

	}
	return nullptr;
}