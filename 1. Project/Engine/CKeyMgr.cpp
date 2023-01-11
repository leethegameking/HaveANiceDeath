﻿#include "pch.h"
#include "CKeyMgr.h"

#include "CEngine.h"


int vkarr[(UINT)KEY::END]
=
{
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',

	VK_RETURN,
	VK_ESCAPE,
	VK_LSHIFT,
	VK_LMENU,
	VK_LCONTROL,
	VK_SPACE,

	VK_LEFT,
	VK_RIGHT,
	VK_UP,
	VK_DOWN,

	VK_LBUTTON,
	VK_RBUTTON,

	VK_NUMPAD0,
	VK_NUMPAD1,
	VK_NUMPAD2,
	VK_NUMPAD3,
	VK_NUMPAD4,
	VK_NUMPAD5,
	VK_NUMPAD6,
	VK_NUMPAD7,
	VK_NUMPAD8,
	VK_NUMPAD9,

	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
};

#include "CDevice.h"
Vec2 CKeyMgr::GetMouseDirectXPos()
{
	Vec2 resol = CDevice::GetInst()->GetRenderResolution();

	Vec2 MouseWinPos = GetMousePos();
	
	MouseWinPos.x += -resol.x / 2.f;
	MouseWinPos.y = -MouseWinPos.y + resol.y / 2.f;

	return MouseWinPos;
}

#include "CRenderMgr.h"
#include "CTransform.h"
#include "CCamera.h"
Vec2 CKeyMgr::GetMouseWorldPos()
{
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	Vec3 vCamPos = pMainCam->Transform()->GetWorldPos();
	float vCamScale = pMainCam->GetOrthographicScale();
	Vec2 vDXMousePos = GetMouseDirectXPos();

	return vCamPos + vDXMousePos * vCamScale;
}

CKeyMgr::CKeyMgr()
{

}

CKeyMgr::~CKeyMgr()
{

}




void CKeyMgr::init()
{
	for (UINT i = 0; i < (UINT)KEY::END; ++i)
	{
		m_vecKey.push_back(tKeyInfo{ (KEY)i, KEY_STATE::NONE, false });
	}
	int a = 'Q';
}

void CKeyMgr::tick()
{
	if (GetFocus())
	{
		for (UINT i = 0; i < (UINT)KEY::END; ++i)
		{
			// 해당키가 현재 눌려있다.
			if (GetAsyncKeyState(vkarr[i]) & 0x8000)
			{
				// 이전 프레임에도 눌려 있었다.
				if (m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eState = KEY_STATE::PRESS;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::TAP;
				}

				m_vecKey[i].bPrevPress = true;
			}

			// 해당키가 현재 안눌려있다.
			else
			{
				// 이전 프레임에는 눌려 있었다.
				if (m_vecKey[i].bPrevPress)
				{
					m_vecKey[i].eState = KEY_STATE::RELEASE;
				}

				// 이전 프레임에도 안눌려 있었다.
				else
				{
					m_vecKey[i].eState = KEY_STATE::NONE;
				}

				m_vecKey[i].bPrevPress = false;
			}
		}

		m_vPrevMousePos = m_vMousePos;

		POINT ptMouse = {};
		GetCursorPos(&ptMouse);
		ScreenToClient(CEngine::GetInst()->GetMainHwnd(), &ptMouse);
		m_vMousePos.x = (float)ptMouse.x;
		m_vMousePos.y = (float)ptMouse.y;

		// 이전 마우스 위치와 현재 마우스 위치를 계산
		m_vMouseDir = Vec2(m_vMousePos.x - m_vPrevMousePos.x, m_vMousePos.y - m_vPrevMousePos.y);
		m_vMouseDir.y *= -1.f;

		/*if (!(m_vMouseDir.x == 0.f && m_vMouseDir.y == 0.f))
		{
			m_vMouseDir.Normalize();
		}		*/
	}

	else
	{
		for (UINT i = 0; i < (UINT)KEY::END; ++i)
		{
			if (KEY_STATE::TAP == m_vecKey[i].eState || KEY_STATE::PRESS == m_vecKey[i].eState)
				m_vecKey[i].eState = KEY_STATE::RELEASE;
			else if (KEY_STATE::RELEASE == m_vecKey[i].eState)
				m_vecKey[i].eState = KEY_STATE::NONE;

			m_vecKey[i].bPrevPress = false;
		}
	}
}
