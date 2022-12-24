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

public:
    void SetData(DWORD_PTR _data) { data = _data; }
    DWORD_PTR GetData() { return data; }

    void SetPopupType(const string& _name) { strInputType = _name; }
    const string& GetPopupType() { return strInputType; }

    void SetName(string _name) override;

public:
    AddDynFunc2(Close);

public:
    void render_update() override;

public:
    InputTextUI();
    ~InputTextUI();
};

