#include "pch.h"
#include "CRandomMgr.h"

CRandomMgr::CRandomMgr()
	:	m_RandomEngine(m_RandomDevice())
{
}

CRandomMgr::~CRandomMgr()
{
}

void CRandomMgr::init()
{
}

int CRandomMgr::GetRandom(int _min, int _max)
{
	uniform_int_distribution<int> distribution(_min, _max);

	return distribution(m_RandomEngine);
}
