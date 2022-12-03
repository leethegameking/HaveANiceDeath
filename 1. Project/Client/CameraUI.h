#pragma once
#include "ComponentUI.h"

class ListUI;

class CameraUI :
    public ComponentUI
{
private:
    PROJ_TYPE   m_eProjType;
    float       m_fFar;
    float       m_fScale;

    int        m_iLayerVisible;
    int        m_iCamIdx;

    vector<string> m_vecLayer;

    ListUI*         m_ListUI;

public: 
    void SetLayerVisible();

public:
    void init() override;
    void update() override;
    void render_update() override;

public:
    CameraUI();
    ~CameraUI();
};

