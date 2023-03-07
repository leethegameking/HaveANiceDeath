#include "pch.h"

#include "define.h"

#include <Engine/GlobalComponent.h>
#include <Engine/CRenderMgr.h>
#include <Engine/CDevice.h>

#include "imgui.h"

Vec2 GetClickedIdx(Vec2 _vImgSize, Vec2 _vCursorPos, Vec2 _vImageScale)
{
	ImGuiIO& io = ImGui::GetIO();

	// KeyMgr가 받아오는 좌표와 차이가 있음.
	Vec2 vMousePos = io.MousePos;

	Vec2 TexCoord = ((vMousePos - _vCursorPos) / _vImageScale);

	float my_tex_w = _vImgSize.x;
	float my_tex_h = _vImgSize.y;

	Vec2 vSlice = Vec2(my_tex_w / _vImageScale.x, my_tex_h / _vImageScale.y);
	Vec2 TexIdx = Vec2((TexCoord.x / vSlice.x), TexCoord.y / vSlice.y);
	TexIdx.x = floorf(TexIdx.x);
	TexIdx.y = floorf(TexIdx.y);

	return TexIdx;
}

Vec2 GetMouseWorldPos()
{
	CCamera* pMainCamera = CRenderMgr::GetInst()->GetMainCam();
	Vec3 vCamPos = pMainCamera->Transform()->GetRelativePos();
	float vCamScale = pMainCamera->GetOrthographicScale();

	Vec2 vResol = CDevice::GetInst()->GetRenderResolution();

	Vec2 vMouseDirectXPos = CKeyMgr::GetInst()->GetMouseDirectXPos();

	const Matrix viewMat = pMainCamera->Camera()->GetViewMat();

	Vec2 vMouseWorldPos;
	vMouseWorldPos.x = vCamPos.x + vMouseDirectXPos.x / vCamScale;
	vMouseWorldPos.y = vCamPos.y + vMouseDirectXPos.y / vCamScale;

	return vMouseWorldPos;
}
