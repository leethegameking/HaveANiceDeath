#pragma once

#include "framework.h"

#include <crtdbg.h>

#include <Engine\global.h>
#include <Engine\CEngine.h>
#include <Engine\CKeyMgr.h>

#include "imgui.h"

#include "define.h"
#include "CommonUI.h"

#ifdef _DEBUG
#pragma comment(lib, "Engine\\Engine_debug.lib")
#else
#pragma comment(lib, "Engine\\Engine.lib")
#endif