#pragma once

class CRandomMgr
	: public CSingleton<CRandomMgr>
{
	SINGLETON(CRandomMgr)

private:
	random_device					m_RandomDevice;
	mt19937							m_RandomEngine;

public:
	void init();

	int GetRandom(int _min, int _max);
};

