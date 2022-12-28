#pragma once
#include "UI.h"

class PopupMenuUI;

class TreeUI;

class TreeNode
{
private:
    static int ID;

    int m_ID;

private:
    string          m_strNodeKey;
    string          m_strShowingName;
    DWORD_PTR       m_data;

    vector<TreeNode*> m_vecChildNode;
    TreeNode*       m_ParentNode;
    TreeUI*         m_TreeUI;

    bool            m_bNodeOpen;
    bool            m_bNodeOpenFlag;

    bool            m_bFrame;
    bool            m_bSelected;

public:
    int GetID() { return m_ID; }
    DWORD_PTR GetData() { return m_data; }

public:
    void render_update();

private:
    void SetNodeKey(const string& _name) { m_strNodeKey = _name; }
    string& GetNodeKey() { return m_strNodeKey; }

    void SetData(DWORD_PTR _data) { m_data = _data; }
    void SetNodeName(const string& _name) { m_strShowingName = _name; }

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
    friend class ContentUI;
    friend class OutlinerUI;
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

    UI* m_DBClickedInst;
    FUNC_1 m_DBClickedFunc;

    UI* m_DragDropInst;
    FUNC_2 m_DragDropFunc;

    TreeNode* m_BeginDragNode;
    TreeNode* m_DropTargetNode;

    UI* m_RightClickInst;
    FUNC_1 m_RightClickFunc;

    PopupMenuUI* m_PopupMenu;
    bool         m_bEmptySpace;

public:
    virtual void update() override;
    virtual void render_update() override;

public:
    TreeNode* AddItem(TreeNode* _parent, const string& _strName, const string& _strShowingName, DWORD_PTR _data, bool _IsFrame = false);
    void Clear();
    void SetDummyRoot(bool _bUse) { m_bDummyRootUse = _bUse; }
    void SetSelectedNode(TreeNode* _SelectedNode);
    TreeNode* GetSelectedNode() { return m_SelectedNode; }

    void SetDBClickedNode(TreeNode* _DBClickedNode);

    void SetBeginDragNode(TreeNode* _node) { m_BeginDragNode = _node; }

    void SetRightClickNode(TreeNode* _node);
    void SetDropTargetNode(TreeNode* _node);

    AddDynFunc1(Select)
    AddDynFunc1(DBClicked)
    AddDynFunc1(RightClick)
    AddDynFunc2(DragDrop)

public:
    TreeNode* GetRootNode() { return m_RootNode; }


public:
    PopupMenuUI* GetPopupMenu();

    // 빈 곳 우클릭 이벤트 처리 위함.
    bool IsEmptySpace() { return m_bEmptySpace; }
    void SetNotEmptySpace() { m_bEmptySpace = false; }

private:
    

public:
    TreeUI(const string& _strName);
    ~TreeUI();
};

