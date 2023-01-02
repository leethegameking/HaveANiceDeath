#include "pch.h"
#include "CEngine.h"

#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CResMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CEventMgr.h"
#include "CDevice.h"
#include "CCollisionMgr.h"
#include "CSound.h"

#include "CLevel.h"
#include "CGameObject.h"
#include "CCollider2D.h"
#include "CTransform.h"


CEngine::CEngine()
	: m_hMainWnd(nullptr)	
{

}

CEngine::~CEngine()
{

}

int CEngine::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hMainWnd = _hWnd;
	m_vResolution.x = (float)_iWidth;
	m_vResolution.y = (float)_iHeight;

	// ��Ʈ�� �ػ󵵸� �����ϱ� ���� ���� ������ ũ�� ���
	RECT rt = { 0, 0, (LONG)_iWidth , (LONG)_iHeight };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);

	// ������ ũ�� ���� �� Show ����
	SetWindowPos(m_hMainWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
	ShowWindow(m_hMainWnd, true);

	// Manager �ʱ�ȭ
	if (FAILED(CDevice::GetInst()->init(m_hMainWnd, m_vResolution)))
	{
		return E_FAIL;
	}

	CPathMgr::GetInst()->init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
			
	CResMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CLevelMgr::GetInst()->init();

	

	return S_OK;
}

void CEngine::progress()
{
	tick();

	render();
}

void CEngine::tick()
{
	// Manager ������Ʈ
	// CResMgr::GetInst()->tick();
	CSound::g_pFMOD->update();
	CTimeMgr::GetInst()->tick();
	CKeyMgr::GetInst()->tick();
	CRenderMgr::GetInst()->tick();

	CLevelMgr::GetInst()->progress();

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecObj =CLevelMgr::GetInst()->GetCurLevel()->GetLayer(i)->GetObjects();
		for (size_t j = 0; j < vecObj.size(); ++j)
		{
			if (vecObj[j]->GetComponent(COMPONENT_TYPE::COLLIDER2D))
			{
				vecObj[j]->Transform()->finaltick();
				vecObj[j]->Collider2D()->finaltick();
			}
		}
	}
	CCollisionMgr::GetInst()->tick();
}

void CEngine::render()
{	
	CDevice::GetInst()->TargetClear();

	CTimeMgr::GetInst()->render();

	CRenderMgr::GetInst()->render();		

	//CEventMgr::GetInst()->tick();
}
