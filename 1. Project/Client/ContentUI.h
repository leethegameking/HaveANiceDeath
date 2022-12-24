#pragma once
#include "UI.h"

class TreeUI;

class ContentUI :
    public UI
{
private:
    TreeUI* m_Tree;

    vector<wstring> m_vecContentName;

public:
    void ResetContent();

    void ReloadContent();

private:
    virtual void update() override;
    virtual void render_update() override;

private:
    void SetResourceToInspector(DWORD_PTR _res);

    void FindContentFileName(const wstring& _strFolderPath);
    RES_TYPE GetResTypeByExt(wstring _filename);


public:
    ContentUI();
    ~ContentUI();
};

