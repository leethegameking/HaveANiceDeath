#pragma once
#include "UI.h"
class ListUI :
    public UI
{
private:
    vector<string> m_ItemList;
    int m_iSelectIdx;

    UI* m_DBCInst;
    FUNC_1 m_DBCFunc;

    UI* m_SelectInst;
    FUNC_0 m_SelectFunc;

    UINT    m_ID;
    static UINT ID;

    bool m_bMultiSelect;
    //bool m_bMultiSelectIdx[MAX_LAYER];
    vector<bool> m_bMultiSelectIdx;

    int m_iIntial;
    bool m_bIntialized;

public:
    void SetItemList(const vector<string>& _inList);
    void SetItemList(const vector<wstring>& _inList);

    void AddDynamicDBClicked(UI* _Inst, FUNC_1 _Func)
    {
        m_DBCInst = _Inst;
        m_DBCFunc = _Func;
    }

    void AddDynamicSelected(UI* _Inst, FUNC_0 _Func)
    {
        m_SelectInst = _Inst;
        m_SelectFunc = _Func;
    }

    int GetSelectIdx() { return m_iSelectIdx; }

    bool IsMultiSelect() { return m_bMultiSelect; }

    void SetMultiSelect(bool _b) { m_bMultiSelect = _b; }

    // bool* GetMultiSelectIdx() { return m_bMultiSelectIdx; }
    vector<bool>& GetMultiSelectIdx() { return m_bMultiSelectIdx; }

    void SetIntialVar(UINT _iIntial) { m_iIntial = _iIntial; }

public:
    void init();
    virtual void render_update() override;
    virtual void Close() override;

public:
    ListUI();
    ~ListUI();

};

