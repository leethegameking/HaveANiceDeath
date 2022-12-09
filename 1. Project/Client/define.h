#pragma once

inline Vec2::operator ImVec2() const { return ImVec2(x, y); }
inline ImVec2::operator Vec2() const { return Vec2(x, y); }
inline Vec2::operator f2() const { return (f2)x; }
// inline Vec2::operator i2() const { int _x = (int)x; return (i2)_x; }

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