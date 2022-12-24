#include "pch.h"
#include "TextureUI.h"

#include <Engine/CResMgr.h>
#include <Engine/CRes.h>

TextureUI::TextureUI()
	: ResUI("##TextureUI", RES_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::update()
{
	ResUI::update();
}

void TextureUI::render_update()
{
	ResUI::render_update();

	Ptr<CTexture> ptrTex = (CTexture*)GetTarget().Get();
	ImGui::Text("Name        : "); ImGui::SameLine(); ImGui::Text(WstrToStr(ptrTex->GetName()).c_str());
	ImGui::Text("Key & Path  : "); ImGui::SameLine(); ImGui::Text(WstrToStr(ptrTex->GetKey()).c_str());
	ImGui::Image(ptrTex->GetSRV().Get(), Vec2(300.f, 300.f));

}


