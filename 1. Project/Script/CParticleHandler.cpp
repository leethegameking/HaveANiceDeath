#include "pch.h"
#include "CParticleHandler.h"
#include <Engine/GlobalComponent.h>

#include <Engine/CKeyMgr.h>

void CParticleHandler::begin()
{
}

void CParticleHandler::tick()
{
	return;

	if (KEY_TAP(KEY::N_1))
	{
		ParticleSystem()->RaiseAliveCount();
	}
	if (KEY_TAP(KEY::N_2))
	{
		ParticleSystem()->ReduceAliveCount();
	}
	if (KEY_PRESSED(KEY::N_3))
	{
		ParticleSystem()->RaiseFrequency();
	}
	if (KEY_PRESSED(KEY::N_4))
	{
		ParticleSystem()->ReduceFrequency();
	}
	if (KEY_TAP(KEY::N_5))
	{
		ParticleSystem()->RaiseLifeTime();
	}
	if (KEY_TAP(KEY::N_6))
	{
		ParticleSystem()->ReduceLifeTime();
	}
	if (KEY_PRESSED(KEY::N_7))
	{
		ParticleSystem()->RaiseSpawnRange();
	}
	if (KEY_PRESSED(KEY::N_8))
	{
		ParticleSystem()->ReduceSpawnRange();
	}
	if (KEY_PRESSED(KEY::N_9))
	{
		ParticleSystem()->RaiseSpeed();
	}
	if (KEY_PRESSED(KEY::N_0))
	{
		ParticleSystem()->ReduceSpeed();
	}
}

void CParticleHandler::BeginOverlap(CCollider2D* _pOther)
{
}

void CParticleHandler::Overlap(CCollider2D* _pOther)
{
}

void CParticleHandler::EndOverlap(CCollider2D* _pOther)
{
}

CParticleHandler::CParticleHandler()
{
}

CParticleHandler::~CParticleHandler()
{
}
