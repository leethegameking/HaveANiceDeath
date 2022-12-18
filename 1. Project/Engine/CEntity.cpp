#include "pch.h"
#include "CEntity.h"

UINT CEntity::g_ID = 0;


void CEntity::SaveToFile(FILE* _pFile)
{
	SaveWStringToFile(m_strName, _pFile);
}

void CEntity::LoadFromFile(FILE* _pFile)
{
	LoadWStringFromFile(m_strName, _pFile);
}

CEntity::CEntity()
	: m_ID(g_ID++)
{
}

CEntity::CEntity(const CEntity& _other)
	: m_ID(g_ID++)
	, m_strName(_other.m_strName + L"_clone")
{
}

CEntity::~CEntity()
{
}
