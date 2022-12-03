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

    UI* m_ConfirmInst;
    FUNC_1 m_ConfirmFunc;

    UINT    m_ID;
    static UINT ID;

    bool m_bMultiSelect;
    //bool m_bMultiSelectIdx[MAX_LAYER];
    vector<bool> m_bMultiSelectIdx;

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

    void AddDynamicConfirm(UI* _Inst, FUNC_1 _Func)
    {
        m_ConfirmInst = _Inst;
        m_ConfirmFunc = _Func;
    }

    int GetSelectIdx() { return m_iSelectIdx; }

    bool IsMultiSelect() { return m_bMultiSelect; }

    void SetMultiSelect(bool _b) { m_bMultiSelect = _b; }

    // bool* GetMultiSelectIdx() { return m_bMultiSelectIdx; }
    vector<bool>& GetMultiSelectIdx() { return m_bMultiSelectIdx; }

public:
    void init(vector<string> _itemList, int _bitInitial = 0);
    virtual void render_update() override;
    virtual void Close() override;

public:
    ListUI();
    ListUI(const string& _name);
    ~ListUI();

};

