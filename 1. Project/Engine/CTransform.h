#pragma once
#include "CComponent.h"


class CTransform :
	public CComponent
{
private:
	Vec3        m_vRelativePos;
	Vec3        m_vRelativeScale;
	Vec3        m_vRelativeRotation;

	Vec3		m_vRelativeDir[(UINT)DIR::END];
	Vec3		m_vWorldDir[(UINT)DIR::END];
	Matrix		m_matWorld;

	bool		m_bIgnParentScale;

public:
	void SetRelativePosWorld(Vec3 _vPos);

	void SetRelativePos(Vec3 _vPos) { m_vRelativePos = _vPos; }
	void SetRelativeScale(Vec3 _vScale) { m_vRelativeScale = _vScale; }
	void SetRelativeRotation(Vec3 _vRotation) { m_vRelativeRotation = _vRotation; }

	void SetRelativePos(float _x, float _y, float _z) { m_vRelativePos = Vec3(_x, _y, _z); }
	void SetRelativeScale(float _x, float _y, float _z) { m_vRelativeScale = Vec3(_x, _y, _z); }
	void SetRelativeRotation(float _x, float _y, float _z) { m_vRelativeRotation = Vec3(_x, _y, _z); }

	Vec3 GetRelativePos() { return	m_vRelativePos; }
	Vec2 GetRelativePosXY() { return Vec2(m_vRelativePos.x, m_vRelativePos.y); }
	Vec3 GetWorldPos() { return m_matWorld.Translation(); }

	Vec3 GetRelativeScale() { return m_vRelativeScale; }
	Vec3 GetWorldScale();

	Vec3 GetRelativeRotation() { return m_vRelativeRotation; }
	Vec3 GetRelativeDir(DIR _eType) { return m_vRelativeDir[(UINT)_eType]; }
	Vec3 GetWorldDir(DIR _eType) { return m_vWorldDir[(UINT)_eType]; }

	bool IsIgnoreParentScale() { return m_bIgnParentScale; }

	void SetIgnoreParentScale(bool _bIgn) { m_bIgnParentScale = _bIgn ;}


	const Matrix& GetWorldMat() { return m_matWorld; }

public:
	void AddRelativePos(Vec3 _add) { m_vRelativePos += _add; }

public:
	virtual void tick() override;
	virtual void finaltick() override;

	void UpdateData();

public:
	virtual void SaveToFile(FILE* _File) override;
	virtual void LoadFromFile(FILE* _File) override;

	CLONE(CTransform);
public:
	CTransform();
	virtual ~CTransform();
};

 