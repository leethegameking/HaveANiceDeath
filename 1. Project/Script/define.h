﻿#define EXPEL Vec2(-1000000.f, -1000000.f)

#define FindNode(AnimPath) mapAnimNode.find(L#AnimPath)->second
#define PushTranNode(AnimPath) 	pTrNode = new tTransitionNode(L#AnimPath); \
								assert(pTrNode->pAnim.Get()); \
								pNode->vecNextAnim.push_back(pTrNode)
#define AERIALANIM false

enum class UNIT_NAME
{
    PLAYER,
    WORKMAN,
    END,
};

enum E_ANIM_CONDITION
{
    E_WAITING,
    E_APPEAR,
    E_IDLE,
    E_UTURN,
    E_DETECT,
    E_RUN,
    E_ATTACK,
};