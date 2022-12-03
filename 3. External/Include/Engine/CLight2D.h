#pragma once
#include "CComponent.h"

class CLight2D :
    public CComponent
{
private:
    tLightInfo      m_Info; // ±¤¿ø Á¤º¸

public:
    const tLightInfo& GetLightInfo() { return m_Info; }

    void SetLightColor(Vec3 _vColor) { m_Info.vDiff = _vColor; }
    void SetLightEmbient(Vec3 _vColor) { m_Info.vEmb = _vColor; }

    void SetLightType(LIGHT_TYPE _eType) { m_Info.iLightType = _eType; }

    void SetRadius(float _fRadius) { m_Info.fRadius = _fRadius; }
    void SetAngle(float _fAngle){ m_Info.fAngle = _fAngle; }

    Vec3 GetLightColor() { return m_Info.vDiff; }
    LIGHT_TYPE GetLightType(LIGHT_TYPE _eType) { return m_Info.iLightType; }
    float GetRadius(float _fRadius) { return m_Info.fRadius; }
    float GetAngle(float _fAngle) { return m_Info.fAngle; }


public:
    virtual void finaltick() override;

    CLONE(CLight2D)

public:
    CLight2D();
    ~CLight2D();
};

