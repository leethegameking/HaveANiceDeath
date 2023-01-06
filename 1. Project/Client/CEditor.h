#pragma once

class CGameObjectEx;

class CEditor
	: public CSingleton<CEditor>
{
private:
	vector<CGameObjectEx*>	m_vecEditorObj;

	list<tDebugShapeInfo>	m_DebugDrawList;
	CGameObjectEx*			m_DebugDrawObject[(UINT)DEBUG_SHAPE::END];

	bool					m_bDebugDrawRender;

public:
	vector<CGameObjectEx*>& GetEditorObject() { return m_vecEditorObj; }

public:
	void init();
	void progress();

public:
	bool GetDebugDrawRender() { return m_bDebugDrawRender; }
	void SetDebugDrawRender(bool _b) { m_bDebugDrawRender = _b; }

private:
	void tick();
	void render();
	void render_debug();

private:
	void CreateDebugDrawObject();
	
	void DebugDraw(tDebugShapeInfo& _info);

public:
	CEditor();
	~CEditor();
};

