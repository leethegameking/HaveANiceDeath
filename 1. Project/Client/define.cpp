#include "pch.h"

#include "define.h"

Vec2 GetClickedIdx(Vec2 _vImgSize, Vec2 _vCursorPos, Vec2 _vImageScale)
{
	ImGuiIO& io = ImGui::GetIO();

	// KeyMgr�� �޾ƿ��� ��ǥ�� ���̰� ����.
	Vec2 vMousePos = io.MousePos;

	Vec2 TexCoord = ((vMousePos - _vCursorPos) / _vImageScale);

	float my_tex_w = _vImgSize.x;
	float my_tex_h = _vImgSize.y;

	Vec2 vSlice = Vec2(my_tex_w / _vImageScale.x, my_tex_h / _vImageScale.y);
	Vec2 TexIdx = Vec2((TexCoord.x / vSlice.x), TexCoord.y / vSlice.y);
	TexIdx.x = floor(TexIdx.x);
	TexIdx.y = floor(TexIdx.y);

	return TexIdx;
}
