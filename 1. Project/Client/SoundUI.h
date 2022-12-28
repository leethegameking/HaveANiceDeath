#pragma once
#include "ResUI.h"
class SoundUI :
    public ResUI
{
private:
    int   m_iVolume;
    UINT m_iLength;
    float m_fAccTime;
    bool    m_bPlay;


private:
    virtual void update() override;
    virtual void render_update() override;

public:
    SoundUI();
    ~SoundUI();
};

