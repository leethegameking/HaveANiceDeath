#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
private:
    Vec2            m_vOffsetPos;
    Vec2            m_vScale;
    Vec3            m_vRot;

    Vec2            m_vFinalPos;
    Vec2            m_vFinalScale;
    Matrix          m_matWorld;

    COLLIDER2D_TYPE m_eType;
    int             m_iOverlapCount;
    bool            m_bIgnorObjectScale;

    Vec4            m_vIdleColor;
    Vec4            m_vCollisionColor;


public:
    void SetOffsetPos(Vec2 _vOffset) { m_vOffsetPos = _vOffset; }
    void SetScale(Vec2 _vScale) { m_vScale = _vScale; }

    void SetRotationX(float _fRadian) { m_vRot.x = _fRadian; }
    void SetRotationY(float _fRadian) { m_vRot.y = _fRadian; }
    void SetRotationZ(float _fRadian) { m_vRot.z = _fRadian; }

    void SetCollider2DType(COLLIDER2D_TYPE _type) { m_eType = _type; }
    COLLIDER2D_TYPE GetCollider2DType() { return m_eType; }

    float GetRotationX() { return m_vRot.x; }
    float GetRotationY() { return m_vRot.y; }
    float GetRotationZ() { return m_vRot.z; }

    Vec2 GetOffsetPos() { return m_vOffsetPos; }
    Vec2 GetScale() { return m_vScale; }

    Vec2 GetFinalPos() { return m_vFinalPos; }
    Vec2 GetFinalScale() { return m_vFinalScale; }
    const Matrix& GetWorldMat() { return m_matWorld; }

    bool GetIgnoreObjectScale() {return m_bIgnorObjectScale; }
    void SetIgnoreObjectScale(bool _bSet) { m_bIgnorObjectScale = _bSet; }

    Vec4 GetIdleColor() { return m_vIdleColor; }
    void SetIdleColor(Vec4 _color) { m_vIdleColor = _color; }

    Vec4 GetCollisionColor() { return m_vCollisionColor; }
    void SetCollisionColor(Vec4 _color) { m_vCollisionColor = _color; }

    int GetOverlapCount() { return m_iOverlapCount; }
public:
    virtual void finaltick() override;

public:
    void BeginOverlap(CCollider2D* _pOther);
    void Overlap(CCollider2D* _pOther);
    void EndOverlap(CCollider2D* _pOther);

public:
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    CLONE(CCollider2D);
public:
    CCollider2D();
    ~CCollider2D();
};

