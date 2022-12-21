#pragma once
#include "CEntity.h"

class CComponent;
class CTransform;
class CRenderComponent;
class CCollider2D;
class CAnimator2D;
class CLight2D;
class CCamera;
class CRigidbody2D;

class CMeshRender;
class CParticleSystem;
class CTileMap;

class CScript;

#define GET_COMPONENT(classname, CLASSNAME ) C##classname* classname() { return ( C##classname*)GetComponent(COMPONENT_TYPE::CLASSNAME); }



class CGameObject :
    public CEntity
{
private:
    CGameObject*            m_pParent;

    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    vector<CScript*>        m_vecScripts;

    CRenderComponent*       m_pRenderComponent;

    int                     m_iLayerIdx;
    bool                    m_bDead;

    // Grave 상태
    // CollisionMgr에서 충돌한 물체의 상태가 이 상태일 때 EndOverlap을 요청한다.
    // m_bGrave 상태 이후엔 컴포넌트가 tick()과 finaltick() 을 하지 않으며 layer에서 삭제되지 않고, EventMgr의 가비지 컬렉터에도 가지 않는다.
    bool                    m_bGrave;

protected:
    vector<CGameObject*>    m_vecChild;

public:
    void begin();
    void tick();
    void finaltick();
    void render();


public:
    void DisconnectFromParent();

    void AddComponent(CComponent* _pComponent);
    CComponent* GetComponent(COMPONENT_TYPE _eComType) { return m_arrCom[(UINT)_eComType]; }
    void AddChild(CGameObject* _pChild);

    template<typename T>
    T* GetScript();

    GET_COMPONENT(Transform, TRANSFORM);
    GET_COMPONENT(MeshRender, MESHRENDER);
    GET_COMPONENT(Camera, CAMERA);
    GET_COMPONENT(Collider2D, COLLIDER2D);
    GET_COMPONENT(Animator2D, ANIMATOR2D);
    GET_COMPONENT(Light2D, LIGHT2D);
    GET_COMPONENT(ParticleSystem, PARTICLESYSTEM);
    GET_COMPONENT(TileMap, TILEMAP);
    GET_COMPONENT(Rigidbody2D, RIGIDBODY2D);

    CRenderComponent* GetRenderComponent() { return m_pRenderComponent; }

    CGameObject* GetParent() { return m_pParent; }

    const vector<CGameObject*>& GetChildObject() const { return m_vecChild; }
    const vector<CScript*>& GetScripts()const { return m_vecScripts; }

    int GetLayerIdx() { return m_iLayerIdx; }
    bool IsDead() { return m_bDead; }
    bool IsGrave() { return m_bGrave; }
    void Destroy();

    CLONE(CGameObject);
public:
    CGameObject();
    CGameObject(const CGameObject& _origin);
    virtual ~CGameObject();

    friend class CLayer;
    friend class CEventMgr;
};

template<typename T>
inline T* CGameObject::GetScript()
{
    for (size_t i = 0; i < m_vecScripts.size(); ++i)
    {
        T* pScript = dynamic_cast<T*>(m_vecScripts[i]);
        if (nullptr != pScript)
            return pScript;
    }

    return nullptr;
}
