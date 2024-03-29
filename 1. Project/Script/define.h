﻿#define EXPEL Vec2(-1000000.f, -1000000.f)

#define FindNode(AnimPath) mapAnimNode.find(L#AnimPath)->second
#define PushTranNode(AnimPath) 	pTrNode = new tTransitionNode(L#AnimPath); \
								assert(pTrNode->pAnim.Get()); \
								pNode->vecNextAnim.push_back(pTrNode)
#define AERIALANIM false

enum PLAYER_COMBO
{
    COMBO_NONE,
    COMBO1,
    COMBO2, // FightToIdle
    COMBO3, // FightToIdle
    COMBO4,
};

enum ANIM_PREFERENCES
{
    NEED_DIR_CHANGE = 0x00000001, // 다음 애니메이션 Uturn이 있는 애니메이션
    DIR_CHANGE_ANIM = 0x00000002, // Uturn 애니메이션
    NO_MOVE = 0x00000004, // 애니메이션 진행동안 움직일 수 없음
    DIR_CHANGE_END = 0x00000008, // 애니메이션 진행 중에 받은 방향을 다음 애니메이션 시작에 바꿔줌.
    HAS_RESERVE = 0x00000010, // 애니메이션이 다음 콤보를 저장하고 있음
    REPEAT = 0x00000020, // 반복
    COMBO_ANIM = 0x00000040, // 애니메이션 진행 중 마우스 클릭시 다음 콤보를 바로 진행
    ANY_STATE = 0x00000080, // 언제든지 진행 가능.
    IGNORE_GRAVITY = 0x00000100, // RigidBody의 Gravity 무효화
    HAS_COLLIDER = 0x00000200, // Collider를 애니메이션 정보에 따라 키고 끔.
    INVINCIBLE = 0x00000400, // 애니메이션 동안 무적판정
    INVINCIBLE_START = 0x00000800,
    DISABLE = 0x00001000, // 아무런 행동 불가
    DEATH = 0x0002000, // 죽음
    AIR_ATTACK_DELAY = 0x0004000, // 마지막 공중 공격 딜레이
    LANDING_ATTACK = 0x0008000, // 하강 공격 -> 그라운드시 y속도 없앰
    FX_JUMP = 0x0010000, // FX 점프 효과
    CAMERA_EVENT_SHAKE = 0x00200000, // camera event
    FX_DASH_FEATHER = 0x00400000,
    FX_DASH_CIRCLE = 0x00800000,
};

enum class ANIM_DIR
{
    ANIM_LEFT = -1,
    ANIM_RIGHT = 1,
    END = 0,
};

enum OBJ_TYPE
{
    OBJ_PLAYER,
    OBJ_ENEMY,
};

enum class UNIT_NAME
{
    PLAYER,
    WORKMAN,
    END,
};

enum ANIM_CONDITION
{
    GROUND                  = 0x00000001,
    KEY_A_OR_D              = 0x00000002,
    ANIM_DIR_CHANGED        = 0x00000004,
    ANIM_FINISHED           = 0x00000008,
    KEY_SPACE               = 0x00000010,
    GROUND_TO_AERIAL        = 0x00000020,
    AERIAL_TO_GROUND        = 0x00000040,
    SPEED_Y_POSITIVE        = 0x00000080,
    SPEED_Y_TURN            = 0x00000100, // Bit 안들어오게 해놓음
    COMBO_PROGRESS          = 0x00000200,
    MOUSE_LEFT              = 0x00000400,
    SPEED_Y_NEGATIVE        = 0x00000800,
    KEY_SHIFT               = 0x00001000,
    CAN_DASH                = 0x00002000,
    HP_DOWN                 = 0x00004000,
    HP_ZERO                 = 0x00008000,
    CAN_JUMP_ATTACK         = 0x00010000,
    KEY_W                   = 0x00020000,
    CAN_AIR_ATTACK          = 0x00040000,
    KEY_S                   = 0x00080000,
};

enum UNIT_STATE
{
    UNIT_GROUND_PLATFORM = 0x00000001,
    UNIT_GROUND = 0x00000002,
    UNIT_HP_DOWN = 0x00000004,
    UNIT_HP_ZERO = 0x00000008
};

enum E_PATTERN
{
    PATTERN_WAITING         = 0x00000001,
    PATTERN_APPEAR          = 0x00000002,
    PATTERN_IDLE            = 0x00000004,
    PATTERN_UTURN           = 0x00000008,
    PATTERN_DETECT          = 0x00000010,
    PATTERN_RUN             = 0x00000020,
    PATTERN_ATTACK_READY    = 0x00000040,
    PATTERN_ATTACK          = 0x00000080,
    PATTERN_DELAY           = 0x00000100,
    PATTERN_HIT_START       = 0x00000200,
    PATTERN_HIT_LOOP        = 0x00000400,
    PATTERN_HIT_END         = 0x00000800,
    PATTERN_DEATH           = 0x00001000,
};

enum BRAD_PATTERN
{
	BRAD_COMBO1_START,
	BRAD_COMBO1,
	BRAD_COMBO2_START,
	BRAD_COMBO2,
	BRAD_COMBO2_END,
	BRAD_COMBO3_START,
	BRAD_COMBO3,
	BRAD_DEATH,
    BRAD_DIVE_MOVE,
	BRAD_DIVE_READY,
	BRAD_DIVE_LOOP,
	BRAD_DIVE_END,
	BRAD_HIT,
	BRAD_IDLE,
	BRAD_INTRO_IDLE,
	BRAD_INTRO_DESK,
	BRAD_INTRO_MOVE,
	BRAD_INTRO_PINGPONG,
	BRAD_INTRO_TO_FIGHT,
	BRAD_INTRO_WELCOME,
	BRAD_STOMP_POSE,
	BRAD_STOMP_READY,
	BRAD_STOMP_ROCK,
	BRAD_STOMP_SPIN_AIR,
	BRAD_STOMP_SPIN_GROUND,
	BRAD_STOMP_TO_IDLE,
	BRAD_STUN_LOOP,
	BRAD_STUN_START,
	BRAD_STUN_END,
	BRAD_UTURN,
    BRAD_DELAY,
    BRAD_CHASE,
    BRAD_TO_GROUND,
};

enum BRAD_DIVE_PATTERN
{
    DIVE_CROSS = 1,
    DIVE_U,
    DIVE_LINE,
};

enum BRAD_ATTACK_PATTERN
{
    POSE = 1,
    SPIN_ROCK,
    COMBO,
    DIVE,
};

struct tKnockBack
{
    bool                bOn;
    float               fDist;
    float               fDurationTime;
    ANIM_DIR            eKnockBackDir;

    tKnockBack()
        : bOn(false)
        , fDist(0.f)
        , fDurationTime(0.f)
        , eKnockBackDir(ANIM_DIR::END)
    {
    }
};

enum class CAMERA_TYPE
{
    MAIN,
    UI,
    CUT_SCENE,
    BOSS,
    END
};

enum class TITLE_MENU_TYPE
{
    PLAY,
    EXIT,
    END,
};

enum class UI_TYPE
{
    UI_PLAYER_STATE,
    UI_BOSS_HP,
    UI_SELECT_LV,
    UI_CUTSCENE,
    END,
};

enum class FX_TYPE
{
    TRANSITION_START,
    TRANSITION_END,
    END
};

enum class TRIGGER_TYPE
{
    PLAYER_STOP,
    PLAYER_GO,
    
    CHANGE_CAMERA_BOSS_TYPE,
    END
};

enum class TRIGGER_COND
{
    BEGIN_OVERLAP,
    OVERLAP,
    END_OVERLAP,
    END
};

enum class CAMERA_POINT_TYPE
{
    BOSS_ROOM,
    END,
};

enum class MAIN_CAM_STATE
{
    MOVE_TO_NEW_POS,
    GENERAL,
    BOSS_BRAD,
};

extern const char* CHAR_CUTSCENE_TEXT_NAME[3];
extern const char* CHAR_CUTSCENE_TEXT_CONTENT[3];

struct tUnitInfo
{
    float   m_fMaxHP;
    float   m_fHP;
    float   m_fMaxMP;
    float   m_fMP;
    float   m_fAtt;
    float   m_fDef;

    float   m_fSpeed;
    ANIM_DIR m_eDir;

    UINT    m_iStateBits;
    UNIT_NAME   m_eName;

    tUnitInfo()
        : m_fMaxHP(30.f)
        , m_fHP(30.f)
        , m_fMaxMP(50.f)
        , m_fMP(50.f)
        , m_fAtt(3.f)
        , m_fDef(1.f)
        , m_fSpeed(100.f)
        , m_eDir(ANIM_DIR::ANIM_RIGHT)
        , m_iStateBits(0)
        , m_eName(UNIT_NAME::END)
    {
    }
};


//===========================DUMMY==============================

enum E_ANIM_CONDITION
{
    START_WAITING = 0x00000001,
    START_APPEAR = 0x00000002,
    START_IDLE = 0x00000004,
    START_UTURN = 0x00000008,
    START_DETECT = 0x00000010,
    START_RUN = 0x00000020,
    START_ATTACK = 0x00000040,
    E_ANIM_FINISHED = 0x00000080,
    E_DIR_CHANGED = 0x00000100,
};

