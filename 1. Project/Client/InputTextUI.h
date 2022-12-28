#pragma once
#include "UI.h"
class InputTextUI :
    public UI
{
private:
    string strInputType;
    string strText;
    DWORD_PTR data;

    UI* m_CloseInst;
    FUNC_2 m_CloseFunc;

    UI* m_Close_Inst;
    FUNC_1 m_Close_Func;

public:
    void SetData(DWORD_PTR _data) { data = _data; }
    DWORD_PTR GetData() { return data; }

    void SetPopupType(const string& _name) { strInputType = _name; }
    const string& GetPopupType() { return strInputType; }

    void SetName(string _name) override;

public:
    AddDynFunc2(Close);
    AddDynFunc1(Close_);

public:
    void render_update() override;

public:
    InputTextUI();
    ~InputTextUI();
};

