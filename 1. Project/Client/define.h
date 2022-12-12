#pragma once

Vec2 GetClickedIdx(Vec2 _vImgSize, Vec2 _vCursorPos, Vec2 _vImageScale);


enum class MENU_TYPE
{
    // Outliner
	MAKE_PREFAB,
    

    
    // Content
    INSTANTIATE,

	MENU_END
};

extern const char* Menu[(UINT)MENU_TYPE::MENU_END];

enum MENU_BIT
{
    // Contetn
    MAKE_PREFAB = 0x01,


    // Outliner
    INSTANTIATE = 0x02,

};












#define AddDynFunc0(name)\
void AddDynamic_##name(UI* _Inst, FUNC_0 _Func)\
{\
    m_##name##Inst = _Inst;\
    m_##name##Func = _Func;\
}


#define AddDynFunc1(name)\
void AddDynamic_##name(UI* _Inst, FUNC_1 _Func)\
{\
    m_##name##Inst = _Inst;\
    m_##name##Func = _Func;\
}

#define AddDynFunc2(name)\
void AddDynamic_##name(UI* _Inst, FUNC_2 _Func)\
{\
    m_##name##Inst = _Inst;\
    m_##name##Func = _Func;\
}