#include "pch.h"
#include "TileMapUI.h"

#include "ListUI.h"
#include "ComboBox.h"

#include "CImGuiMgr.h"
#include <Engine/CTileMap.h>
#include <Engine/CResMgr.h>
#include <Engine/CRes.h>
#include <Engine/CKeyMgr.h>
#include <Engine/CTransform.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CCamera.h>
#include <Engine/CDevice.h>
#include "TileEditor.h"
#include "CommonUI.h"


TileMapUI::TileMapUI()
	: ComponentUI("TileMapUI", COMPONENT_TYPE::TILEMAP)
	, m_AtlasComboBox(nullptr)
	, m_vImageScale(Vec2(0.25f, 0.25f))
	, m_SelectedTexIdx(Vec2(0.f,0.f))
	, m_vSlice(Vec2(64.f, 64.f))
	, m_bInstanceMode(false)
	, m_bTileChanged(false)
{
	m_AtlasComboBox = new ComboBox;
	m_AtlasComboBox->AddSelectedFunc(this, (FUNC_1)&TileMapUI::SetAtlasTex);
	
	m_SelectedMark = CResMgr::GetInst()->FindRes<CTexture>(L"Bubbles50px");
}

TileMapUI::~TileMapUI()
{
	if (m_AtlasComboBox)
		delete m_AtlasComboBox;

	//if (m_TileEditor)
	//	delete m_TileEditor;
}

void TileMapUI::init()
{
	const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE::TEXTURE);

	map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();

	for (; iter != mapRes.end(); ++iter)
	{
		m_vecRes.push_back(string(iter->first.begin(), iter->first.end()));
	}

	m_AtlasTex = GetTarget()->TileMap()->GetTileAtlas();
	for (int i = 0; i < m_vecRes.size(); ++i)
	{
		if (m_AtlasTex.Get()->GetKey() == wstring(m_vecRes[i].begin(), m_vecRes[i].end()))
			m_AtlasComboBox->init(m_vecRes, i);
	}

	m_vecTile = GetTarget()->TileMap()->GetTileInfo();
}

void TileMapUI::update()
{
	if (GetTarget())
	{
		m_AtlasTex = GetTarget()->TileMap()->GetTileAtlas();
		m_vTileCount = GetTarget()->TileMap()->GetTileCount();
		m_vSlice = GetTarget()->TileMap()->GetSlice();
	}
}

void TileMapUI::render_update()
{
	ComponentUI::render_update();

	// Input InstanceMode 에서만 넣을 수 있음.
	int Inputflag = ImGuiInputTextFlags_ReadOnly;
	if (m_bInstanceMode)
		Inputflag = 0;

	// Atlas select combo-box
	ImGui::Text("Image     "); ImGui::SameLine();
	if (m_bInstanceMode)
		m_AtlasComboBox->render_update();
	else
	{
		wstring wstrKey = m_AtlasTex->GetKey();
		ImGui::InputText("Image", (char*)WstrToStr(wstrKey).c_str(), wstrKey.length(), Inputflag);
	}

	// TileCount
	ImGui::PushItemWidth(200.f);
	int tileCount[2] = { (int)m_vTileCount.x, (int)m_vTileCount.y };
	ImGui::Text("TileCount "); ImGui::SameLine(); ImGui::InputInt2("##TileCount", tileCount, Inputflag);
	ImGui::PopItemWidth();

	// Slice
	Vec2 prevSlice = m_vSlice;
	ImGui::PushItemWidth(100.f);
	ImGui::Text("Slice     "); ImGui::SameLine(); ImGui::InputFloat2("##TileSlice", m_vSlice, "%0.1f", Inputflag);
	ImGui::PopItemWidth();

	// UI ImageScale
	ImGui::Text("Scale     ");
	ImGui::SameLine();
	ImGui::PushItemWidth(100.f);
	ImGui::InputFloat("##ImageScaleX", &m_vImageScale.x, 0.1f);
	ImGui::SameLine();
	ImGui::InputFloat("##ImageScaleY", &m_vImageScale.y, 0.1f);
	ImGui::PopItemWidth();

	// InstanceMode 
	ImGui::SameLine(); ImGui::Checkbox("InstanceMode", &m_bInstanceMode);

	ImTextureID AtlasSRV = m_AtlasTex->GetSRV().Get();
	float my_tex_w = m_AtlasTex->GetWidth();
	float my_tex_h = m_AtlasTex->GetHeight();

	ImTextureID MarkSRV = m_SelectedMark->GetSRV().Get();

	ImVec2 vCursorPos = ImGui::GetCursorScreenPos();

	// Image
	ImGui::Image(AtlasSRV, ImVec2(my_tex_w * m_vImageScale.x, my_tex_h * m_vImageScale.y));
	if (ImGui::IsItemHovered() && ImGui::IsItemClicked())
	{
		SelectImageTile(vCursorPos, m_vImageScale);
	}

	// bubble 
	ImVec2 p = ImVec2(vCursorPos.x + m_vSlice.x * m_SelectedTexIdx.x * m_vImageScale.x, vCursorPos.y + m_vSlice.y * m_SelectedTexIdx.y * m_vImageScale.y);
	ImGui::GetWindowDrawList()->AddImage(
		MarkSRV,
		p,
		ImVec2(p.x + my_tex_w * m_vImageScale.x / (m_AtlasTex->GetWidth() / m_vSlice.x)
			, p.y + my_tex_h * m_vImageScale.y / (m_AtlasTex->GetHeight() / m_vSlice.y)));

	CGameObject* pMainCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
	float camScale = pMainCamera->Camera()->GetOrthographicScale();


	Vec3 vTargetProjScale = GetTarget()->Transform()->GetWorldScale();
	vTargetProjScale /= camScale;

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	Vec2 vMouseViewPos = CKeyMgr::GetInst()->GetMouseViewPos();

	const Matrix viewMat = pMainCamera->Camera()->GetViewMat();

	Vec3 vTargetProjPos = GetTarget()->Transform()->GetWorldPos();
	Vec4 vTargetViewPos = MulMatrix(Vec4(vTargetProjPos, 1.f), viewMat);
	vTargetProjPos = Vec3(vTargetViewPos.x, vTargetViewPos.y, vTargetViewPos.z);
	vTargetProjPos /= camScale;


	// ȭ���� Tile Pressed �϶� ���ϴ� Ÿ�Ϸ� ��� �ٲ���.
	if (m_bInstanceMode)
	{
		if (KEY_PRESSED(KEY::LBTN) &&
			vTargetProjPos.x + vTargetProjScale.x / 2.f > vMouseViewPos.x &&
			vTargetProjPos.x - vTargetProjScale.x / 2.f < vMouseViewPos.x &&
			vTargetProjPos.y + vTargetProjScale.y / 2.f > vMouseViewPos.y &&
			vTargetProjPos.y - vTargetProjScale.y / 2.f < vMouseViewPos.y)
		{
			ArrangeTile();
		}
	}

	// Ÿ�� ������ �ٲ�� Ÿ�ٵ� ���� �ٲ���.
	if ((int)m_vTileCount.x != tileCount[0] || (int)m_vTileCount.y != tileCount[1])
	{
		GetTarget()->TileMap()->SetTileCount((UINT)tileCount[0], (UINT)tileCount[1]);
		m_bTileChanged = true;
	}
	if (m_vSlice != prevSlice)
	{
		GetTarget()->TileMap()->DataChanged();
		GetTarget()->TileMap()->SetSlice(m_vSlice);
	}

	if (CommonUI::ButtonCenteredOnLine("Create TileMap"))
	{
		m_TileEditor = (TileEditor*)CImGuiMgr::GetInst()->FindUI("TileEditor");
		m_TileEditor->SetTileMapUI(this);
		m_TileEditor->init();
		m_TileEditor->Open();
	}
}

void TileMapUI::SetAtlasTex(DWORD_PTR _texKey)
{
	string strKey = (char*)_texKey;
	wstring wstrKey = wstring(strKey.begin(), strKey.end());
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(wstrKey);

	GetTarget()->TileMap()->SetTileAtlas(pTex);
}

void TileMapUI::SelectImageTile(Vec2 _vCursorPos, Vec2 _vImageScale)
{
	ImGuiIO& io = ImGui::GetIO();
	
	// KeyMgr�� �޾ƿ��� ��ǥ�� ���̰� ����.
	Vec2 vMousePos = io.MousePos;

	Vec2 TexCoord = ((vMousePos - _vCursorPos) / _vImageScale);

	Vec2 TexIdx = Vec2(TexCoord.x / GetTarget()->TileMap()->GetSlice().x, TexCoord.y / GetTarget()->TileMap()->GetSlice().y);


	m_SelectedTexIdx.x = floor(TexIdx.x);
	m_SelectedTexIdx.y = floor(TexIdx.y);

	m_SelectedTexUV.x = m_SelectedTexIdx.x * GetTarget()->TileMap()->GetSlice().x / m_AtlasTex->GetWidth();
	m_SelectedTexUV.y = m_SelectedTexIdx.y * GetTarget()->TileMap()->GetSlice().y / m_AtlasTex->GetHeight();
}

void TileMapUI::ArrangeTile()
{
	CGameObject* pMainCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
	float camScale = pMainCamera->Camera()->GetOrthographicScale();

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();
	Vec2 vMouseViewPos = CKeyMgr::GetInst()->GetMouseViewPos();
	// Vec4 v4MouseViewPos = Vec4( vMouseViewPos.x, vMouseViewPos.y, 1.f, 1.f);

	
	const Matrix viewMat = pMainCamera->Camera()->GetViewMat();

	Vec3 vTargetProjPos = GetTarget()->Transform()->GetWorldPos();
	Vec4 vTargetViewPos = MulMatrix(Vec4(vTargetProjPos, 1.f), viewMat);
	vTargetProjPos = Vec3(vTargetViewPos.x, vTargetViewPos.y, vTargetViewPos.z);
	vTargetProjPos /= camScale;

	Vec3 vTargetProjScale = GetTarget()->Transform()->GetWorldScale();
	vTargetProjScale /= camScale;

	// tex 0,0 ��ǥ�� ���� ��ǥ
	Vec2 vConvertTexPos = Vec2(vTargetProjPos.x - vTargetProjScale.x / 2.f, vTargetProjPos.y + vTargetProjScale.y / 2.f);
	// tex 0,0 ��ǥ ���� ������� Ŭ���� texture ��ǥ
	Vec2 vCurTexPos = Vec2(vMouseViewPos.x - vConvertTexPos.x, -(vMouseViewPos.y - vConvertTexPos.y));

	// ���� ��ǥ�� tilemap������ Idx
	Vec2 vCurTexIdx = vCurTexPos / vTargetProjScale;
	vCurTexIdx *= m_vTileCount;

	// Vec2 Idx�� 1���� Idx�� ����
	UINT vChangeIdx = floor(vCurTexIdx.y) * m_vTileCount.x + vCurTexIdx.x;

	// 1���� Idx�� vecTile�� ������ ���õ� ��Ʋ�� �� �̹��� LT�� Set
	(*m_vecTile)[vChangeIdx].vLeftTop = m_SelectedTexUV;

	// vecTile StructuredBuffer�� ���Ҵ� �ǵ��� bool �� ����
	GetTarget()->TileMap()->DataChanged();
}





