#pragma once
#include "UI.h"

class TreeUI;

class TreeNode
{
private:
    static int ID;

    int m_ID;

private:
    string          m_strName;
    DWORD_PTR       m_data;

    vector<TreeNode*> m_vecChildNode;
    TreeNode*       m_ParentNode;
    TreeUI*         m_TreeUI;

    bool            m_bFrame;
    bool            m_bSelected;

public:
    int GetID() { return m_ID; }
    DWORD_PTR GetData() { return m_data; }

public:
    void render_update();

private:
    void SetNodeName(const string& _name) { m_strName = _name; }

    void SetData(DWORD_PTR _data) { m_data = _data; }


    void SetFrame(bool _bFrame) { m_bFrame = _bFrame; }
    void AddChild(TreeNode* _ChildNode)
    {
        _ChildNode->m_ParentNode = this;
        m_vecChildNode.push_back(_ChildNode);
    }

    const vector<TreeNode*>& GetChild() { return m_vecChildNode; }

public:
    TreeNode();
    ~TreeNode();

    friend class TreeUI;
};

class TreeUI :
    public UI
{
private:
    TreeNode* m_RootNode;
    bool m_bDummyRootUse;
    
    TreeNode* m_SelectedNode;

    UI* m_SelectInst;
    FUNC_1 m_SelectFunc;

    UI* m_DragDropInst;
    FUNC_2 m_DragDropFunc;

    TreeNode* m_BeginDragNode;
    TreeNode* m_DropTargetNode;

public:
    TreeNode* AddItem(TreeNode* _parent, const string& _strName, DWORD_PTR _data, bool _IsFrame = false);
    void Clear();
    void SetDummyRoot(bool _bUse) { m_bDummyRootUse = _bUse; }
    void SetSelectedNode(TreeNode* _SelectedNode);

    void AddDynamic_Selected(UI* _Inst, FUNC_1 _Func)
    {
        m_SelectInst = _Inst;
        m_SelectFunc = _Func;
    }


    void AddDynamic_DragDop(UI* _Inst, FUNC_2 _Func)
    {
        m_DragDropInst = _Inst;
        m_DragDropFunc = _Func;
    }
    void SetBeginDragNode(TreeNode* _node) { m_BeginDragNode = _node; }
    void SetDropTargetNode(TreeNode* _node);

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    TreeUI(const string& _strName);
    ~TreeUI();
};

