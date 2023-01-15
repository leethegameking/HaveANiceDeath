#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ANIMCONTROLLER,
	ATTSCRIPT,
	BLOCKSCRIPT,
	BRAD,
	CONTROLLERSCRIPT,
	CUTSCENECAMERASCRIPT,
	ENEMYATTSCRIPT,
	ENEMYCONTROLLER,
	ENEMYSCRIPT,
	GAMECAMERASCRIPT,
	HITSCRIPT,
	MAINCAMERASCRIPT,
	PLAYERATTSCRIPT,
	PLAYERSCRIPT,
	PROJECTILESCRIPT,
	TESTSCRIPT,
	UICAMERASCRIPT,
	UNITSCRIPT,
	WORKMAN,
	WORKWOMAN,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
