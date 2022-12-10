#pragma once
#include "UI.h"

#include <Engine/Ptr.h>
#include <Engine/CTexture.h>

class CAnimation2D;
class ComboBox;

enum class ToolMode
{
    CREATE,
    EDIT
};

class AnimTool :
    public UI
{
private:
    int m_iMode;

    vector<tAnim2DFrm> m_ChangeFrm;

    // ======================
    // Create
    // ======================

    ComboBox* m_CreateCombo;
    Ptr<CTexture> m_AtlasTex;
    ImTextureID m_AtlasSRV;
    bool m_bImageChanged;
    int m_CreateFrmIdx;

    bool m_bUniformed;
    bool m_bHasSelected;
    bool m_bDragMode;
    bool m_bVertical;
    bool m_bAllChange;
    
     Vec2 m_vSliceCount;
     Vec2 m_vAtlasScale;

     Vec2 m_vClickedIdx;
     
     int m_iMaxFrm;



    // ======================
    // Edit
    // ======================

    ComboBox* m_EditCombo;
    Ptr<CAnimation2D> m_pCurAnim;
    int m_EditFrmIdx;

    bool m_bAnimChanged;

public:
    void init() override;
    void init_edit(CAnimation2D* _pAnim);
    void update() override;
    void render_update() override;

private:
    void CreateMode();
    void EditMode();

    void SetDefaultTexture();
    void SetCreateCombo();
    void ToolModeSet();


    
    void FrameWindow();
    void AtlasWindow();
    void FrameImageWindow();
    void SettingWindow();

    void DeleteFrame(int& idx);


    Vec2 GetClickedIdx(Vec2 _vCursorPos);




    bool IsCreateMode() { return m_iMode == (int)ToolMode::CREATE; }
    bool IsEditMode() { return m_iMode == (int)ToolMode::EDIT; }

    bool IsUniformed() { return m_bUniformed; }
    bool IsIndulgent() { return !m_bUniformed; }

private:
    void SetAtlasTex(DWORD_PTR _texKey);
    void ClearFrm() { m_ChangeFrm.clear(); }

    void SetAnimaton(DWORD_PTR _animKey);

public:
    AnimTool();
    ~AnimTool();
};

