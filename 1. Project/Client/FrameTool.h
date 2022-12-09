#pragma once
#include "UI.h"

class ComboBox;
class CAnimation2D;

enum FRAME_TYPE
{
    FRAME_BUTTON,
    FRAME_IMAGE,
};

class FrameTool :
    public UI
{
private:
    ComboBox* m_AnimComboBox;
    CAnimation2D* m_pCurAnim;

    vector<string> m_vecRes;

    vector<tAnim2DFrm>* m_OriginFrm;
    vector<tAnim2DFrm>  m_ChangeFrm;
    int m_iFrmIdx;
    bool m_bAnimChanged;

    int m_opt;

    Vec2 m_vImageScale;


public:
    void Init_Frame(CAnimation2D* _pAnim);
    void update() override;
    void render_update() override;
    void Close() override;

public:
    void SetAnimaton(DWORD_PTR _animKey);

public:
    void DeleteFrame(size_t& idx);

public:
    FrameTool();
    ~FrameTool();
};

