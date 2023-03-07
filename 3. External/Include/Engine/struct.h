#pragma once


enum class CAMERA_EVENT_TYPE
{
	ATTACK,
	UP_DOWN_SHAKE,
	END,
};

struct tCameraEvent
{
	CAMERA_EVENT_TYPE eType;
	float fAccTime;
	float fMaxTime;

	tCameraEvent()
		: fAccTime(0.f)
		, eType(CAMERA_EVENT_TYPE::END)
	{
	}

	tCameraEvent(float _maxTime)
		: fMaxTime(_maxTime)
		, fAccTime(0.f)
		, eType(CAMERA_EVENT_TYPE::END)
	{
	}
};

struct Vtx
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;
};


struct tText
{
	wstring strText;
	Vec2 vWorldPos;
	Vec2 vWidth_Height;
	D2D1_COLOR_F vColor;
	TEXT_FORMAT eFormat;
	bool		bScreenPos;

	tText()
		: strText()
		, vWorldPos(Vec2::Zero)
		, eFormat(TEXT_FORMAT::TEXT_MOUSE_POS)
		, vWidth_Height(Vec2(500.f, 500.f))
		, bScreenPos(false)
	{
		vColor = { 1.f, 1.f, 1.f, 1.f };
	}
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

struct tParticleAnim
{
	Vec2 vLeftTop;
	Vec2 vSlice;
	Vec2 vOffset;
	Vec2 vFullSize;
	float fDuration;
	float fAcc;

	int iEnd;
	int iPadding;
};

struct tAnimIdx
{
	int iAnimIdx;
	float fAcc;

	int iPadding[2];
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

	Matrix matRot;

	Vec3 vScale;
	int padding;
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

	Vec2 vAtlasSize;
	int iPadding;
};

struct tParticleAnim2DInfo
{
	tAnim2DInfo arrAnim2DInfo[50];
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