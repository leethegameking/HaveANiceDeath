#pragma once

Vec2 GetClickedIdx(Vec2 _vImgSize, Vec2 _vCursorPos, Vec2 _vImageScale);














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