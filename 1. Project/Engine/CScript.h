﻿#pragma once
#include "CComponent.h"


#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CResMgr.h"

#include "GlobalComponent.h"

enum class SCRIPT_PARAM
{
    BOOL,
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    TEXTURE,
    MATRERIAL,
    PREFAB,
    SOUND,
    STRING,
};

struct tScriptParam
{
    SCRIPT_PARAM Type;
    string strParamName;
    void* pParam;
};


class CScript :
    public CComponent
{
private:
    int     m_iScriptType;
    vector<tScriptParam> m_vecParam;

public:
    int GetScriptType() { return m_iScriptType; }
    void AddScriptParam(SCRIPT_PARAM _eParam, const string& _name, void* _pParam) 
    {
        m_vecParam.push_back(tScriptParam{ _eParam, _name, _pParam });
    }
    const vector<tScriptParam>& GetScriptParam() { return m_vecParam; }

public:
    virtual void tick() = 0;
    virtual void finaltick() final {};


public:
    virtual void BeginOverlap(CCollider2D* _pOther) {}
    virtual void Overlap(CCollider2D* _pOther) {}
    virtual void EndOverlap(CCollider2D* _pOther) {}

public:
    virtual CScript* Clone() = 0;
public:
    CScript(int _iScriptType);
    CScript(const CScript& _origin);
    ~CScript();
};

