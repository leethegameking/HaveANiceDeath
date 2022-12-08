#pragma once
#include "CEntity.h"

#include "Ptr.h"

class CRes :
    public CEntity
{
private:
    wstring         m_strKey;
    wstring         m_strRelativePath;
    const RES_TYPE  m_eResType;
    int             m_iRefCount;

private:
    void AddRef() 
    { 
        ++m_iRefCount; 
    }
    void Release() 
    {
        --m_iRefCount;
        assert(!(m_iRefCount < 0));

        if (0 == m_iRefCount)
        {
            delete this;
        }
    }

    // Save & Load
    virtual int Load(const wstring& _strFilePath) = 0;
    virtual void Save(const wstring& _strFilePath) = 0;

protected:
    void SaveKeyPath(FILE* _pFile);
    void LoadKeyPath(FILE* _pFile);


public:
    const wstring& GetKey() { return m_strKey; }
    const wstring& GetRelativePath() { return m_strRelativePath; }
    RES_TYPE GetResType() { return m_eResType; }

private:
    void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
    void SetRelativePath(const wstring& _strRelativePath) { m_strRelativePath = _strRelativePath; }



    virtual CRes* Clone() = 0;

public:
    CRes(RES_TYPE _eResType);
    CRes(const CRes& _other);
    ~CRes();

    friend class CResMgr;

    template<typename T>
    friend class Ptr;    
};

