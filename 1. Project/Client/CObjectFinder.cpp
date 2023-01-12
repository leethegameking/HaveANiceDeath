#include "pch.h"
#include "CObjectFinder.h"

#include "CImGuiMgr.h"
#include "InspectorUI.h"
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

void CObjectFinder::FindAndSetObject()
{
	InspectorUI* pInsp = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	Vec2 vWorldMousePos = CKeyMgr::GetInst()->GetMouseWorldPos();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	for (size_t i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecLayerObj = pCurLevel->GetLayer(i)->GetObjects();

		for (size_t j = 0; j < vecLayerObj.size(); ++j)
		{
			Vec3 vWorldPos =  vecLayerObj[j]->Transform()->GetWorldPos();
			Vec3 vWorldScale = vecLayerObj[j]->Transform()->GetWorldScale();
			Vec2 v2WorldScale = Vec2(vWorldScale.x, vWorldScale.y);
			Vec2 v2WorldPos = Vec2(vWorldPos.x, vWorldPos.y);
			if (InRect(vWorldMousePos, v2WorldPos, v2WorldScale))
			{
				pInsp->SetTargetObj(vecLayerObj[j]);
				return;
			}
		}
	}
}

bool CObjectFinder::InRect(Vec2 _dot, Vec2 _RectPos, Vec2 _RectScale)
{
	if (_dot.x < _RectPos.x + _RectScale.x / 2.f &&
		_dot.x > _RectPos.x - _RectScale.x / 2.f &&
		_dot.y < _RectPos.y + _RectScale.y / 2.f &&
		_dot.y > _RectPos.y - _RectScale.y / 2.f)
	{
		return true;
	}
	return false;
}
