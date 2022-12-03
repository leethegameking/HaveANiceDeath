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

    

public:
    void init(int _idx);
    void render_update() override;
    void update();

public:
    ComboBox();
    ~ComboBox();

};

