#pragma once
#include "UI.h"
class ComboBox :
    public UI
{
private:
    vector<string> m_vecItem;
    int m_iCurItemIdx;
    
    FUNC_1  m_SelectedFunc;
    UI*     m_SelectedInst;

    FUNC_1 m_SelectedFunc_ReturnInt;
    UI* m_SelectedInst_ReturnInt;

    FUNC_0 m_SelectedNotifyFunc;
    UI* m_SelectedNotifyInst;

    UINT m_ID;
    static UINT ID;

public:
    void SetItem(vector<string>& _vecItem) { m_vecItem = _vecItem; }
    void SetPreviewIdx(int _PreviewIdx) { m_iCurItemIdx = _PreviewIdx; }

    void AddSelectedFunc(UI* _inst, FUNC_1 _func)
    {
        m_SelectedInst = _inst;
        m_SelectedFunc = _func;
    }

    void AddSelectedFunc_ReturnInt(UI* _inst, FUNC_1 _func)
    {
        m_SelectedInst_ReturnInt = _inst;
        m_SelectedFunc_ReturnInt = _func;
    }

    void AddSelectedNotify(UI* _inst, FUNC_0 _func)
    {
        m_SelectedNotifyFunc = _func;
        m_SelectedNotifyInst = _inst;
    }

public:
    void init( vector<string> _itemList, int _idx);
    void render_update() override;
    void update();



public:
    ComboBox();
    ~ComboBox();
};

