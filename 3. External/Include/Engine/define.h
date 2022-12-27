#pragma once

typedef Vector2 Vec2;
typedef Vector3 Vec3;
typedef Vector4 Vec4;

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define DEVICE CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetContext()

#define KEY_PRESSED(key)	(CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::PRESS)
#define KEY_TAP(key)		(CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::TAP)
#define KEY_RELEASE(key)	(CKeyMgr::GetInst()->GetKeyState(key) == KEY_STATE::RELEASE)

#define CLONE(type) public: type* Clone() { return new type(*this); }
#define CLONE_DUMMY(type) public: type* Clone() { return nullptr; }
#define CLONE_ASSERT(type) public: type* Clone() { assert(nullptr); return nullptr; }

#define VERTICAL 1
#define HORIZONTAL 0

#define MAX_LAYER 32

#define SINGLETON(class_name) private:\
class_name();\
~class_name();\
friend class CSingleton<class_name>;

#define CONTENTPATH CPathMgr::GetInst()->GetContentPath()

enum class SAMPLER_TYPE
{
	ANISOTROPIC, // 이방성
	POINT,		 

	END,
};

enum class CB_TYPE
{
	TRANSFORM,	 // b0
	MATERIAL,	 // b1
	GLOBAL,	     // b2
	ANIMATION2D, // b3

	END,
};

// Rasterizer State
enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,				// Less, Depth Write
	GREATER,			// Greater, Depth Write
	NO_WRITE,			// Less, Depth No Write
	NO_TEST_NO_WRITE,	// NoTest, Depth No Write

	END,
};

// BlendState Type
enum class BS_TYPE
{
	DEFAULT,
	ALPHABLEND,
	ONE_ONE,

	END,
};

enum PROJ_TYPE
{
	PERSPECTIVE,
	ORTHOGRAHPICS,
};



enum PIPELINE_STAGE
{
	VS = 0x01,
	HS = 0x02,
	DS = 0x04,
	GS = 0x08,
	PS = 0x10,
	ALL_STAGE = VS | HS | DS | GS | PS,
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		// 불투명 단계
	DOMAIN_MASK,		// 출력, 비출력
	DOMAIN_TRANSPARENT, // 반투명
	DOMAIN_POST_PROCESS,// 후처리	    
	NONE,
};

enum class RES_TYPE
{
	PREFAB,
	MESHDATA,
	COMPUTE_SHADER,

	MATERIAL,
	MESH,
	TEXTURE,	
	SOUND,
	GRAPHICS_SHADER,

	ANIMATION2D,
	
	END,
};

extern const char* RES_TYPE_CHAR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WCHAR[(UINT)RES_TYPE::END];

// DUMMY 사용하려면 char 배열 부분도 수정.
enum class COMPONENT_TYPE
{
	TRANSFORM,
	CAMERA,
	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	RIGIDBODY2D,
	DUMMY_1,
	DUMMY_2			= 10,

	// render
	MESHRENDER,
	TILEMAP,
	PARTICLESYSTEM,
	SKYBOX,
	DECAL,
	LANDSCAPE,
	DUMMY_3,
	DUMMY_4,

	END				= 19,

	SCRIPT,
};

extern const char* COMPONENT_TYPE_CHAR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WCHAR[(UINT)COMPONENT_TYPE::END];

enum class EVENT_TYPE
{
	CREATE_OBJECT,  // wParam : Object Adress, lParam : Layer Index
	DELETE_OBJECT,	// wParam : Object Adress,
	BURRY_OBJECT,
	ADD_CHILD,
	CHANGE_LEVEL,
	CHANGE_LEVEL_STATE,

	END,
};

enum class DEBUG_SHAPE
{
	// 2D
	RECT,
	CIRCLE,

	// 3D
	CUBE,
	SPHERE,

	END,
};


enum class COLLIDER2D_TYPE
{
	COLLIDER2D_RECT,
	COLLIDER2D_CIRCLE,
};

enum RENDER_TYPE
{
	BUBBLE,
	FIRE,
	SPARK,
};

enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};


enum class DIR
{
	RIGHT,
	UP,
	FRONT,
	END,
};

enum class SB_TYPE
{
	SRV_ONLY,
	UAV_INC,
};

enum class LIGHT_TYPE
{
	DIRECTIONAL,	// 방향성
	POINT,			// 점광원
	SPOT,			// 스포트 라이트
};

enum class LEVEL_STATE
{
	PLAY,
	STOP,
	PAUSE,
};