#pragma once
#include <Engine/CScript.h>

class CTriggerScript;

// class의 함수 포인터
typedef void (CTriggerScript::* FUNC_0)(void);
typedef void (CTriggerScript::* FUNC_1)(DWORD_PTR);
typedef void (CTriggerScript::* FUNC_2)(DWORD_PTR, DWORD_PTR);

class CTriggerScript :
    public CScript
{
private:
	int m_eTriggerCond; // begin, overlap, end
	int m_eTriggerType;

	FUNC_0 m_pFunc_0;
	FUNC_1 m_pFunc_1;
	FUNC_2 m_pFunc_2;

	DWORD_PTR m_Param_1;
	DWORD_PTR m_Param_2;

	bool m_bFirst;

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _pOther) override;
	virtual void Overlap(CCollider2D* _pOther) override;
	virtual void EndOverlap(CCollider2D* _pOther) override;

private:
	void ProgressTrigger(CCollider2D* _pOther);

public:
	virtual void SaveToFile(FILE* _pFile) override;
	virtual void LoadFromFile(FILE* _pFile) override;

private:
	void PlayerGo();
	void PlayerStop();


	CLONE(CTriggerScript);
public:
	CTriggerScript();
	CTriggerScript(const CTriggerScript& _origin);
	~CTriggerScript();

};



