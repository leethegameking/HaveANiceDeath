#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ANIMCONTROLLER,
	ATTSCRIPT,
	BLOCKSCRIPT,
	CONTROLLERSCRIPT,
	ENEMYATTSCRIPT,
	ENEMYCONTROLLER,
	ENEMYSCRIPT,
	GAMECAMERASCRIPT,
	HITSCRIPT,
	PLAYERATTSCRIPT,
	PLAYERSCRIPT,
	PROJECTILESCRIPT,
	TESTSCRIPT,
	UNITSCRIPT,
	WORKMAN,
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
