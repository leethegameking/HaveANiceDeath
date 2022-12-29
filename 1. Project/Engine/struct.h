#pragma once


struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};


struct tEvent
{
	EVENT_TYPE	eType;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

struct tLightInfo
{
	Vec4		vDiff;		// 난반사 광
	Vec4		vSpec;		// 반사광 계수
	Vec4		vEmb;		// 환경광
	
	Vec4		vWorldPos;	// 광원 월드 위치
	Vec4		vWorldDir;	// 광원의 방향
	LIGHT_TYPE	iLightType;	// 광원 타입
	float		fRadius;	// 광원 반경 (POINT, SPOT)
	float		fAngle;		// 광원 각도 (SPOT)
	int			pad;
};


// ================
// Animation 구조체
// ================
struct tAnim2DFrm
{
	Vec2 vLeftTop; 
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
	
	// Collider
	int bColiiderOn;
	Vec2 iColliderPos;
	Vec2 iColliderScale;
	int iPadding[2];
};


// ===============
// Particle 구조체
// ===============
struct tParticle
{
	Vec4 vRelativePos;
	Vec4 vDir;
		
	float fMaxTime;
	float fCurTime;
	float fSpeed;
	UINT  iActive;
};

struct tParticleShare
{
	UINT	iAliveCount;
	UINT	iPadding[3];
};



// ================
// Debug Shape Info
// ================
struct tDebugShapeInfo
{
	DEBUG_SHAPE eShape;
	Vec4		vColor;
	Vec3		vPosition;
	Vec3		vScale;
	Vec3		vRot;
	float		fRadius;
	float		fDuration;
	float		fCurTime;
};


// ================
// 상수버퍼용 구조체
// ================
struct tTransform
{	
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

extern tTransform g_transform;

struct tMtrlConst
{
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int iTexCheck[(UINT)TEX_PARAM::TEX_END];

public:
	// mat과 TexCheck은 바꿔주지 않음.
	void operator = (const tMtrlConst& _other)
	{
		for (int i = 0; i < 4; ++i)
		{
			iArr[i] = _other.iArr[i];
			fArr[i] = _other.fArr[i];
			v2Arr[i] = _other.v2Arr[i];
			v4Arr[i] = _other.v4Arr[i];
		}
	}
};

struct tGlobalData
{
	Vec2	vRenderResolution;
	Vec2	vNoiseResolution;
	int		iLight2DCount;
	int		iLight3DCount;

	float	fAccTime;
	float	fDT;	
};

extern tGlobalData g_global;

struct tAnim2DInfo
{	
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;

	int iAnim2DUse;
	int iPadding[3];
};

struct tTile
{
	Vec2 vLeftTop;
	Vec2 vSlice;
};

struct tScalarParam
{
	SCALAR_PARAM	eParam;
	string			strName;
};

struct tTextureParam
{
	TEX_PARAM	eParam;
	string		strName;
};