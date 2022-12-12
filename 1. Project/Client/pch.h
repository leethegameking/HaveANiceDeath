#pragma once

#include "framework.h"

#include <crtdbg.h>

#include <Engine\global.h>
#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>

#include "define.h"


#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_debug.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Script\\Script_debug.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif