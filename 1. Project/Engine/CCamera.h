#pragma once
#include "CComponent.h"



class CGameObject;

class CCamera :
    public CComponent
{
private:
    Matrix                  m_matView;      // �� ���
    Matrix                  m_matProj;      // ���� ���

    PROJ_TYPE               m_eProjType;    // ���� ���
    float                   m_fAspectRatio; // ��Ⱦ ��

    float                   m_fFar;         // ī�޶� �ִ� �þ� �Ÿ�
    float                   m_fScale;       // ���� ���� ����

    UINT                    m_iLayerMask;

    vector<CGameObject*>    m_vecQpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPostProcess;

    int                     m_iCamIdx;  // ī�޶� �켱����

    // ī�޶� �켱���� ���Ҷ�
    // static int CurExistIdx;          // ī�޶� �������� ++�ϰ� �̰��� Idx�� -> �켱���� ��ü�� ������ Idx��ü�� �̷����.

public:
    virtual void finaltick();
    void render();


public:
    float GetOrthographicScale() { return m_fScale; }
    void SetOrthographicScale(float _fScale) { m_fScale = _fScale; }

    void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; }
    PROJ_TYPE GetProjType() { return m_eProjType; }

    void SetAspectRatio(float _fRatio) { m_fAspectRatio = _fRatio; }
    float GetAspectRatio() { return m_fAspectRatio; }

    void SetFar(float _fFar) { m_fFar = _fFar; }
    float GetFar() { return m_fFar; }

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

    void SetLayerVisible(const wstring& _strLayerName);
    void SetLayerVisible(int _iLayerIdx);
    void SetLayerInvisible(int _iLayerIdx);
    void SetLayerAllVisible() { m_iLayerMask = 0xffffffff; }
    void SetLayerAllInVisible() { m_iLayerMask = 0; }

    UINT GetLayerVisible() { return m_iLayerMask; }

    void SetCamIdx(UINT _Idx) { m_iCamIdx = _Idx; }
    int GetCamIdx() { return m_iCamIdx; }

private:
    void SortObject();
    void render_opaque();
    void render_mask();
    void render_transparent();
    void render_postprocess();


    CLONE(CCamera);
public:
    CCamera();
    ~CCamera();
};

