#include "pch.h"
#include "CMainCameraScript.h"
#include "CPlayerMgr.h"
#include "CCameraMgr.h"
#include <Engine/CGameObject.h>
#include <Engine/CRandomMgr.h>

CMainCameraScript::CMainCameraScript()
	: CGameCameraScript((int)SCRIPT_TYPE::MAINCAMERASCRIPT)
	, m_eMainCamState((int)MAIN_CAM_STATE::GENERAL)
	, m_pCamPoint(nullptr)
{
	m_eType = CAMERA_TYPE::MAIN;

	AddScriptParam(SCRIPT_PARAM::FLOAT, "event range", &m_fRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "event rot range", &m_fRotRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "event rot back", &m_fRotBack);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "event rot Max Time", &m_fRotMaxTime);
}

CMainCameraScript::CMainCameraScript(const CMainCameraScript& _origin)
	: CGameCameraScript(_origin)
	, m_eMainCamState((int)MAIN_CAM_STATE::GENERAL)
	, m_pCamPoint(nullptr)
{
	m_eType = CAMERA_TYPE::MAIN;

	AddScriptParam(SCRIPT_PARAM::FLOAT, "event range", &m_fRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "event rot range", &m_fRotRange);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "event rot back", &m_fRotBack);
	AddScriptParam(SCRIPT_PARAM::FLOAT, "event rot Max Time", &m_fRotMaxTime);
}

CMainCameraScript::~CMainCameraScript()
{
}

void CMainCameraScript::SetEvent(const tCameraEvent& _evn)
{
	if (m_tCurEvent.eType == CAMERA_EVENT_TYPE::END)
	{
		m_tCurEvent = _evn;
	}
	else
	{
		m_tNextEvent = _evn;
	}
}

void CMainCameraScript::EventProgress()
{
	if (m_tCurEvent.eType != CAMERA_EVENT_TYPE::END)
	{
		if (m_tCurEvent.eType == CAMERA_EVENT_TYPE::ATTACK)
		{
			Vec3 vPos = Transform()->GetRelativePos();
			Vec3 vRot = Transform()->GetRelativeRotation();

			int iRandom1 = GetRandom_D(-m_fRange, m_fRange);
			int iRandom2 = GetRandom_D(-m_fRange, m_fRange);

			int iRandomRot = GetRandom_D(-m_fRotRange, m_fRotRange);
			iRandomRot = iRandomRot / XM_PI * 180;

			Transform()->AddRelativePos(Vec3(iRandom1, iRandom2, 0.f) * DT);

			if (m_fRotMaxTime < m_fRotAccTime)
			{
				Transform()->SetRelativeRotation(Vec3(0.f, 0.f, vRot.z + iRandomRot / 180.f * XM_PI) * DT);
				m_fRotAccTime = 0.f;
			}
			else
			{
				m_fRotAccTime += DT;
			}

			m_tCurEvent.fAccTime += DT;
		}

		if (m_tCurEvent.eType == CAMERA_EVENT_TYPE::UP_DOWN_SHAKE)
		{
			static float fAdd = 200.f;

			int iRandom1 = GetRandom_D(-m_fRange + fAdd, m_fRange + fAdd);
			Transform()->AddRelativePos(Vec3(0.f, iRandom1, 0.f) * DT);

			m_tCurEvent.fAccTime += DT;
		}

		if (m_tCurEvent.fAccTime > m_tCurEvent.fMaxTime)
		{
			m_tCurEvent.eType = CAMERA_EVENT_TYPE::END;

			if (m_tNextEvent.eType != CAMERA_EVENT_TYPE::END)
			{
				m_tCurEvent = m_tNextEvent;
				m_tNextEvent.eType = CAMERA_EVENT_TYPE::END;
			}
		}
	}
}

void CMainCameraScript::StateGeneral()
{
	if (!m_pPlayer)
		m_pPlayer = CPlayerMgr::GetInst()->GetPlayerObj();
	else
	{
		EventProgress();

		Vec3 vPlayerPos = m_pPlayer->Transform()->GetRelativePos();
		Vec3 vCamPos = Transform()->GetRelativePos();
		vPlayerPos.y += 50.f;
		Vec3 vPosDiff = vPlayerPos - vCamPos;
		Transform()->AddRelativePos(vPosDiff / 10.f * (DT / 0.025f));

		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z = vRot.z - vRot.z / m_fRotBack * DT;
		Transform()->SetRelativeRotation(vRot);
	}
}

void CMainCameraScript::StateBossBrad()
{
	EventProgress();

	m_pCamPoint = CCameraMgr::GetInst()->GetCamPoint(CAMERA_POINT_TYPE::BOSS_ROOM);
	Vec3 vPointPos = m_pCamPoint->Transform()->GetWorldPos();
	Vec3 vCamPos = Transform()->GetWorldPos();
	CameraMove(vCamPos, vPointPos, Vec3::Zero);
}

void CMainCameraScript::CameraMove(Vec3 _vStart, Vec3 _vTarget, Vec3 _vOffset, float _fMoveDistRatio, float _fPerSecond)
{
	_vTarget += _vOffset;
	Vec3 vPosDiff = _vTarget - _vStart;
	Transform()->AddRelativePos(vPosDiff * _fMoveDistRatio * (DT * _fPerSecond));
}



void CMainCameraScript::begin()
{
	CGameCameraScript::begin();
	m_fRange = 400.f;
	m_fRotBack = 0.8f;
	m_fRotRange = 1.75f;  
	m_fRotMaxTime = 0.05f;
}

void CMainCameraScript::tick()
{


	switch ((MAIN_CAM_STATE)m_eMainCamState)
	{
	case MAIN_CAM_STATE::GENERAL:
	{
		StateGeneral();
	}
		break;

	case MAIN_CAM_STATE::BOSS_BRAD:
	{
		StateBossBrad();
	}
		break;
	}
}

void CMainCameraScript::SaveToFile(FILE* _pFile)
{
	CGameCameraScript::SaveToFile(_pFile);
}

void CMainCameraScript::LoadFromFile(FILE* _pFile)
{
	CGameCameraScript::LoadFromFile(_pFile);
}



