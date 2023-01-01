#pragma once
#include "UI.h"
class ComboBox :
    public UI
{
private:
    vector<string> m_vecItem;
    vector<string> m_vecItemName;
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
    template<typename T>
    void init(vector<string> _itemList, int _idx);

    template<typename T>
    void SetItem(vector<string>& _vecItem);

public:
    void SetCurIdx(int _idx) { m_iCurItemIdx = _idx; }

public:
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
    void init_not_res(vector<string>& _itemList, int _idx);
    void render_update() override;
    void update();



public:
    ComboBox();
    ~ComboBox();
};

template<typename T>
inline void ComboBox::SetItem(vector<string>& _vecItem)
{
	m_vecItem = _vecItem;

	for (size_t i = 0; i < m_vecItem.size(); ++i)
	{
        wstring strKey = StrToWstr(m_vecItem[i]);
        Ptr<T> ptrRes = CResMgr::GetInst()->FindRes<T>(strKey);
        m_vecItemName.push_back(WstrToStr(ptrRes.Get()->GetName()));
	}
}

template<typename T>
inline void ComboBox::init(vector<string> _itemList, int _idx)
{
    m_vecItem.clear();
    m_vecItemName.clear();
    SetItem<T>(_itemList);

    m_iCurItemIdx = _idx;
}
