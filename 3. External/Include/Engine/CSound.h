#pragma once
#include "CRes.h"

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

class CSound :
    public CRes
{
public:
    // FMOD 싱글톤 객체
    static FMOD::System* g_pFMOD;

private:
    FMOD::Sound* m_pSound; // 메모리에 올려진 소리 정보
    list<FMOD::Channel*> m_listChannel; // 현재 소리가 중첩되어 어떤 채널에서 재생되고 있는지 알려줌.

public:
    // 0 (무한반복) 0 ~ 1(Volume)
    int Play(int _iRoopCount, float _fVolume = 1.f, bool _bOverlap = false);
    void Stop();

    // 0 ~ 1
    void SetVolume(float _f, int _iChannelIdx);

private:
    void RemoveChannel(FMOD::Channel* _pTargetChannel);

    // 소리가 끝날 때 자동으로 호출되는 callback 함수 -> FMOD도 계속 tick을 돌아야함.
    friend FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL* channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
        , FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
        , void* commanddata1, void* commanddata2);

public:
    virtual void UpdateData() {}
    virtual void Save(const wstring& _strRelativePath) override { return; }
    virtual int Load(const wstring& _strFilePath) override;

    CLONE_ASSERT(CSound);
public:
    CSound();
    virtual ~CSound();
};

