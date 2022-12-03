#pragma once
#include "CRes.h"
#include "CGraphicsShader.h"

#include "CTexture.h"

class CMaterial :
    public CRes
{
private:
    tMtrlConst              m_tConst;
    Ptr<CTexture>           m_arrTex[TEX_END];
    Ptr<CGraphicsShader>    m_pShader;

    Ptr<CMaterial>          m_pMasterMtrl;

public:
    void SetScalarParam(SCALAR_PARAM _eScalarType, void* _pData);
    void SetTexParam(TEX_PARAM _eTex, Ptr<CTexture> _pTex);

    void SetShader(Ptr<CGraphicsShader> _pShader) { m_pShader = _pShader;}
    Ptr<CGraphicsShader> GetShader() { return m_pShader; }

    Ptr<CTexture>* GetTexArr() { return m_arrTex; }

    void UpdateData();
    static void Clear();

    virtual int Load(const wstring& _strFilePath) { return S_OK; }


    CLONE(CMaterial);
public:
    CMaterial();
    CMaterial(const CMaterial& _other);
    ~CMaterial();
};

