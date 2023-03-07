#include "pch.h"
#include "CScriptMgr.h"

#include "CAnimController.h"
#include "CAnimStarter.h"
#include "CAttScript.h"
#include "CBasicTextScript.h"
#include "CBlockScript.h"
#include "CBossCameraScript.h"
#include "CBossHPUIScript.h"
#include "CBrad.h"
#include "CCameraPoint.h"
#include "CCamPointWall.h"
#include "CControllerScript.h"
#include "CCutSceneCameraScript.h"
#include "CCutSceneMgr.h"
#include "CDamageText.h"
#include "CEffectScaler.h"
#include "CElevatorScript.h"
#include "CEnemyAttScript.h"
#include "CEnemyController.h"
#include "CEnemyScript.h"
#include "CFarBack.h"
#include "CFarFront.h"
#include "CGameCameraScript.h"
#include "CHitScript.h"
#include "CItemScript.h"
#include "CLevelSelectScript.h"
#include "CMainCameraScript.h"
#include "CMouseColliderScript.h"
#include "COrderTextScript.h"
#include "CParticleEnder.h"
#include "CPlayerAttScript.h"
#include "CPlayerHPText.h"
#include "CPlayerHPUIScript.h"
#include "CPlayerScript.h"
#include "CProjectileScript.h"
#include "CSoundPlay.h"
#include "CTestScript.h"
#include "CTextScript.h"
#include "CTitleMenuScript.h"
#include "CTriggerScript.h"
#include "CUICameraScript.h"
#include "CUnitScript.h"
#include "CWorkman.h"
#include "CWorkwoman.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAnimController");
	_vec.push_back(L"CAnimStarter");
	_vec.push_back(L"CAttScript");
	_vec.push_back(L"CBasicTextScript");
	_vec.push_back(L"CBlockScript");
	_vec.push_back(L"CBossCameraScript");
	_vec.push_back(L"CBossHPUIScript");
	_vec.push_back(L"CBrad");
	_vec.push_back(L"CCameraPoint");
	_vec.push_back(L"CCamPointWall");
	_vec.push_back(L"CControllerScript");
	_vec.push_back(L"CCutSceneCameraScript");
	_vec.push_back(L"CCutSceneMgr");
	_vec.push_back(L"CDamageText");
	_vec.push_back(L"CEffectScaler");
	_vec.push_back(L"CElevatorScript");
	_vec.push_back(L"CEnemyAttScript");
	_vec.push_back(L"CEnemyController");
	_vec.push_back(L"CEnemyScript");
	_vec.push_back(L"CFarBack");
	_vec.push_back(L"CFarFront");
	_vec.push_back(L"CGameCameraScript");
	_vec.push_back(L"CHitScript");
	_vec.push_back(L"CItemScript");
	_vec.push_back(L"CLevelSelectScript");
	_vec.push_back(L"CMainCameraScript");
	_vec.push_back(L"CMouseColliderScript");
	_vec.push_back(L"COrderTextScript");
	_vec.push_back(L"CParticleEnder");
	_vec.push_back(L"CPlayerAttScript");
	_vec.push_back(L"CPlayerHPText");
	_vec.push_back(L"CPlayerHPUIScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CProjectileScript");
	_vec.push_back(L"CSoundPlay");
	_vec.push_back(L"CTestScript");
	_vec.push_back(L"CTextScript");
	_vec.push_back(L"CTitleMenuScript");
	_vec.push_back(L"CTriggerScript");
	_vec.push_back(L"CUICameraScript");
	_vec.push_back(L"CUnitScript");
	_vec.push_back(L"CWorkman");
	_vec.push_back(L"CWorkwoman");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAnimController" == _strScriptName)
		return new CAnimController;
	if (L"CAnimStarter" == _strScriptName)
		return new CAnimStarter;
	if (L"CAttScript" == _strScriptName)
		return new CAttScript;
	if (L"CBasicTextScript" == _strScriptName)
		return new CBasicTextScript;
	if (L"CBlockScript" == _strScriptName)
		return new CBlockScript;
	if (L"CBossCameraScript" == _strScriptName)
		return new CBossCameraScript;
	if (L"CBossHPUIScript" == _strScriptName)
		return new CBossHPUIScript;
	if (L"CBrad" == _strScriptName)
		return new CBrad;
	if (L"CCameraPoint" == _strScriptName)
		return new CCameraPoint;
	if (L"CCamPointWall" == _strScriptName)
		return new CCamPointWall;
	if (L"CControllerScript" == _strScriptName)
		return new CControllerScript;
	if (L"CCutSceneCameraScript" == _strScriptName)
		return new CCutSceneCameraScript;
	if (L"CCutSceneMgr" == _strScriptName)
		return new CCutSceneMgr;
	if (L"CDamageText" == _strScriptName)
		return new CDamageText;
	if (L"CEffectScaler" == _strScriptName)
		return new CEffectScaler;
	if (L"CElevatorScript" == _strScriptName)
		return new CElevatorScript;
	if (L"CEnemyAttScript" == _strScriptName)
		return new CEnemyAttScript;
	if (L"CEnemyController" == _strScriptName)
		return new CEnemyController;
	if (L"CEnemyScript" == _strScriptName)
		return new CEnemyScript;
	if (L"CFarBack" == _strScriptName)
		return new CFarBack;
	if (L"CFarFront" == _strScriptName)
		return new CFarFront;
	if (L"CGameCameraScript" == _strScriptName)
		return new CGameCameraScript;
	if (L"CHitScript" == _strScriptName)
		return new CHitScript;
	if (L"CItemScript" == _strScriptName)
		return new CItemScript;
	if (L"CLevelSelectScript" == _strScriptName)
		return new CLevelSelectScript;
	if (L"CMainCameraScript" == _strScriptName)
		return new CMainCameraScript;
	if (L"CMouseColliderScript" == _strScriptName)
		return new CMouseColliderScript;
	if (L"COrderTextScript" == _strScriptName)
		return new COrderTextScript;
	if (L"CParticleEnder" == _strScriptName)
		return new CParticleEnder;
	if (L"CPlayerAttScript" == _strScriptName)
		return new CPlayerAttScript;
	if (L"CPlayerHPText" == _strScriptName)
		return new CPlayerHPText;
	if (L"CPlayerHPUIScript" == _strScriptName)
		return new CPlayerHPUIScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CProjectileScript" == _strScriptName)
		return new CProjectileScript;
	if (L"CSoundPlay" == _strScriptName)
		return new CSoundPlay;
	if (L"CTestScript" == _strScriptName)
		return new CTestScript;
	if (L"CTextScript" == _strScriptName)
		return new CTextScript;
	if (L"CTitleMenuScript" == _strScriptName)
		return new CTitleMenuScript;
	if (L"CTriggerScript" == _strScriptName)
		return new CTriggerScript;
	if (L"CUICameraScript" == _strScriptName)
		return new CUICameraScript;
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
	case (UINT)SCRIPT_TYPE::ANIMSTARTER:
		return new CAnimStarter;
		break;
	case (UINT)SCRIPT_TYPE::ATTSCRIPT:
		return new CAttScript;
		break;
	case (UINT)SCRIPT_TYPE::BASICTEXTSCRIPT:
		return new CBasicTextScript;
		break;
	case (UINT)SCRIPT_TYPE::BLOCKSCRIPT:
		return new CBlockScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSCAMERASCRIPT:
		return new CBossCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSHPUISCRIPT:
		return new CBossHPUIScript;
		break;
	case (UINT)SCRIPT_TYPE::BRAD:
		return new CBrad;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAPOINT:
		return new CCameraPoint;
		break;
	case (UINT)SCRIPT_TYPE::CAMPOINTWALL:
		return new CCamPointWall;
		break;
	case (UINT)SCRIPT_TYPE::CONTROLLERSCRIPT:
		return new CControllerScript;
		break;
	case (UINT)SCRIPT_TYPE::CUTSCENECAMERASCRIPT:
		return new CCutSceneCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::CUTSCENEMGR:
		return new CCutSceneMgr;
		break;
	case (UINT)SCRIPT_TYPE::DAMAGETEXT:
		return new CDamageText;
		break;
	case (UINT)SCRIPT_TYPE::EFFECTSCALER:
		return new CEffectScaler;
		break;
	case (UINT)SCRIPT_TYPE::ELEVATORSCRIPT:
		return new CElevatorScript;
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
	case (UINT)SCRIPT_TYPE::FARBACK:
		return new CFarBack;
		break;
	case (UINT)SCRIPT_TYPE::FARFRONT:
		return new CFarFront;
		break;
	case (UINT)SCRIPT_TYPE::GAMECAMERASCRIPT:
		return new CGameCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::HITSCRIPT:
		return new CHitScript;
		break;
	case (UINT)SCRIPT_TYPE::ITEMSCRIPT:
		return new CItemScript;
		break;
	case (UINT)SCRIPT_TYPE::LEVELSELECTSCRIPT:
		return new CLevelSelectScript;
		break;
	case (UINT)SCRIPT_TYPE::MAINCAMERASCRIPT:
		return new CMainCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSECOLLIDERSCRIPT:
		return new CMouseColliderScript;
		break;
	case (UINT)SCRIPT_TYPE::ORDERTEXTSCRIPT:
		return new COrderTextScript;
		break;
	case (UINT)SCRIPT_TYPE::PARTICLEENDER:
		return new CParticleEnder;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERATTSCRIPT:
		return new CPlayerAttScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHPTEXT:
		return new CPlayerHPText;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERHPUISCRIPT:
		return new CPlayerHPUIScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::PROJECTILESCRIPT:
		return new CProjectileScript;
		break;
	case (UINT)SCRIPT_TYPE::SOUNDPLAY:
		return new CSoundPlay;
		break;
	case (UINT)SCRIPT_TYPE::TESTSCRIPT:
		return new CTestScript;
		break;
	case (UINT)SCRIPT_TYPE::TEXTSCRIPT:
		return new CTextScript;
		break;
	case (UINT)SCRIPT_TYPE::TITLEMENUSCRIPT:
		return new CTitleMenuScript;
		break;
	case (UINT)SCRIPT_TYPE::TRIGGERSCRIPT:
		return new CTriggerScript;
		break;
	case (UINT)SCRIPT_TYPE::UICAMERASCRIPT:
		return new CUICameraScript;
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

	case SCRIPT_TYPE::ANIMSTARTER:
		return L"CAnimStarter";
		break;

	case SCRIPT_TYPE::ATTSCRIPT:
		return L"CAttScript";
		break;

	case SCRIPT_TYPE::BASICTEXTSCRIPT:
		return L"CBasicTextScript";
		break;

	case SCRIPT_TYPE::BLOCKSCRIPT:
		return L"CBlockScript";
		break;

	case SCRIPT_TYPE::BOSSCAMERASCRIPT:
		return L"CBossCameraScript";
		break;

	case SCRIPT_TYPE::BOSSHPUISCRIPT:
		return L"CBossHPUIScript";
		break;

	case SCRIPT_TYPE::BRAD:
		return L"CBrad";
		break;

	case SCRIPT_TYPE::CAMERAPOINT:
		return L"CCameraPoint";
		break;

	case SCRIPT_TYPE::CAMPOINTWALL:
		return L"CCamPointWall";
		break;

	case SCRIPT_TYPE::CONTROLLERSCRIPT:
		return L"CControllerScript";
		break;

	case SCRIPT_TYPE::CUTSCENECAMERASCRIPT:
		return L"CCutSceneCameraScript";
		break;

	case SCRIPT_TYPE::CUTSCENEMGR:
		return L"CCutSceneMgr";
		break;

	case SCRIPT_TYPE::DAMAGETEXT:
		return L"CDamageText";
		break;

	case SCRIPT_TYPE::EFFECTSCALER:
		return L"CEffectScaler";
		break;

	case SCRIPT_TYPE::ELEVATORSCRIPT:
		return L"CElevatorScript";
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

	case SCRIPT_TYPE::FARBACK:
		return L"CFarBack";
		break;

	case SCRIPT_TYPE::FARFRONT:
		return L"CFarFront";
		break;

	case SCRIPT_TYPE::GAMECAMERASCRIPT:
		return L"CGameCameraScript";
		break;

	case SCRIPT_TYPE::HITSCRIPT:
		return L"CHitScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"CItemScript";
		break;

	case SCRIPT_TYPE::LEVELSELECTSCRIPT:
		return L"CLevelSelectScript";
		break;

	case SCRIPT_TYPE::MAINCAMERASCRIPT:
		return L"CMainCameraScript";
		break;

	case SCRIPT_TYPE::MOUSECOLLIDERSCRIPT:
		return L"CMouseColliderScript";
		break;

	case SCRIPT_TYPE::ORDERTEXTSCRIPT:
		return L"COrderTextScript";
		break;

	case SCRIPT_TYPE::PARTICLEENDER:
		return L"CParticleEnder";
		break;

	case SCRIPT_TYPE::PLAYERATTSCRIPT:
		return L"CPlayerAttScript";
		break;

	case SCRIPT_TYPE::PLAYERHPTEXT:
		return L"CPlayerHPText";
		break;

	case SCRIPT_TYPE::PLAYERHPUISCRIPT:
		return L"CPlayerHPUIScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::PROJECTILESCRIPT:
		return L"CProjectileScript";
		break;

	case SCRIPT_TYPE::SOUNDPLAY:
		return L"CSoundPlay";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"CTestScript";
		break;

	case SCRIPT_TYPE::TEXTSCRIPT:
		return L"CTextScript";
		break;

	case SCRIPT_TYPE::TITLEMENUSCRIPT:
		return L"CTitleMenuScript";
		break;

	case SCRIPT_TYPE::TRIGGERSCRIPT:
		return L"CTriggerScript";
		break;

	case SCRIPT_TYPE::UICAMERASCRIPT:
		return L"CUICameraScript";
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