#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"

#include "InspectorUI.h"
#include "CImguiMgr.h"

#include <Engine/CEventMgr.h>
#include <Engine/CResMgr.h>

ContentUI::ContentUI()
	: UI("Content")
	, m_ContentTree(nullptr)
{
	m_ContentTree = new TreeUI("##ContentTree");
	AddChild(m_ContentTree);

	m_ContentTree->SetShowSeperator(false);
	m_ContentTree->SetDummyRoot(true);

	m_ContentTree->AddDynamic_Select(this, (FUNC_1)&ContentUI::SetResourceToInspector);

	m_LevelLoadTree = new TreeUI("##LevelLaodTree");
	AddChild(m_LevelLoadTree);

	m_LevelLoadTree->SetShowSeperator(false);
	m_LevelLoadTree->SetDummyRoot(true);
	m_LevelLoadTree->AddDynamic_DBClicked(this, (FUNC_1)&ContentUI::LoadLevel);

	ResetContent();
}

ContentUI::~ContentUI()
{
}

void ContentUI::update()
{
	// �������� ���� ���� Ʈ�������� reset�Ѵ�
	if (CEventMgr::GetInst()->IsResChanged())
	{
		ResetContent();
	}

	UI::update();
}

void ContentUI::render_update()
{
}


void ContentUI::ResetContent()
{
	// Frame �κ� open�� ��� 
	TreeNode* xRootNode = m_ContentTree->GetRootNode();
	if (xRootNode)
	{
		const vector<TreeNode*>& vecTreeChild = xRootNode->GetChild();
		m_vecNodeOpenBool.clear();
		for (size_t i = 0; i < vecTreeChild.size(); ++i)
		{
			m_vecNodeOpenBool.push_back(vecTreeChild[i]->m_bNodeOpen);
		}
	}

	m_ContentTree->Clear();

	// Res Load
	// ���� �߰�
	TreeNode* pRootNode = m_ContentTree->AddItem(nullptr, "", "Content", 0);

	
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		// RES_TYPE�� �ش��ϴ� ���ڿ��� �θ�� �߰�
		TreeNode* pResNode = m_ContentTree->AddItem(pRootNode, "", ToString((RES_TYPE)i), 0, true);
		if (xRootNode)
		{
			pResNode->m_bNodeOpen = m_vecNodeOpenBool[i];
		}

		// �� RES_TYPE�� ���� Res���� �ڽ����� �߰� (data�� ����.)
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE(i));
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			wstring strKey = iter->first;
			wstring strName = (iter->second).Get()->GetName();
			m_ContentTree->AddItem(pResNode, WstrToStr(strKey), WstrToStr(strName), (DWORD_PTR)iter->second.Get());
		}
	}

	// Level Load
	m_LevelLoadTree->Clear();
	TreeNode* pLevelNameNode = m_LevelLoadTree->AddItem(pRootNode, "", "Levels", 0, true);
	for (size_t i = 0; i < m_vecLevelName.size(); ++i)
	{
		wstring strName = GetNameFromPath(m_vecLevelName[i]);

		TreeNode* pLevelNode = m_LevelLoadTree->AddItem(pLevelNameNode, WstrToStr(m_vecLevelName[i]), WstrToStr(strName), 0, false);
	}
}

void ContentUI::ReloadContent()
{
	wstring strFolderPath = CONTENTPATH;
	FindContentFileName(strFolderPath);

	for (size_t i = 0; i < m_vecContentName.size(); ++i)
	{
		RES_TYPE resType = GetResTypeByExt(m_vecContentName[i]);

		// ���ҽ� Ÿ�Կ� �ش��ϴ� ���ҽ��� ��ηκ��� �ε�
		if (RES_TYPE::END == resType)
		{
			// ���� �� ��� (tmpsave.lv ������ ����) 
			if (m_vecContentName[i].rfind(L".lv") != -1 && m_vecContentName[i].rfind(L"tmpsave.lv") == -1)
			{
				m_vecLevelName.push_back(m_vecContentName[i]);
			}
			continue;
		}
			
		Ptr<CRes> tmpRes;

		switch (resType)
		{
		case RES_TYPE::PREFAB:
			tmpRes = CResMgr::GetInst()->Load<CPrefab>(m_vecContentName[i], m_vecContentName[i]).Get();
			break;

		case RES_TYPE::TEXTURE:
			tmpRes = CResMgr::GetInst()->Load<CTexture>(m_vecContentName[i], m_vecContentName[i]).Get();
			break;

		case RES_TYPE::MATERIAL:
			tmpRes = CResMgr::GetInst()->Load<CMaterial>(m_vecContentName[i], m_vecContentName[i]).Get();
			break;

		case RES_TYPE::ANIMATION2D:
			tmpRes = CResMgr::GetInst()->Load<CAnimation2D>(m_vecContentName[i], m_vecContentName[i]).Get();
			break;

		case RES_TYPE::SOUND:
			break;

		case RES_TYPE::MESHDATA:
			break;
		case RES_TYPE::MESH:
			break;

		}

		// �̸��� ���� ���.
		if (tmpRes->GetName() == L"")
		{
			wstring strKey = tmpRes->GetKey();
			size_t endIdx = strKey.rfind(L".");
			size_t startIdx = strKey.rfind(L"\\");

			tmpRes->SetName(strKey.substr(startIdx + 1, endIdx - startIdx - 1));
			int  a = 0;
		}
	}
}

void ContentUI::SetResourceToInspector(DWORD_PTR _res)
{
	TreeNode* pNode = (TreeNode*)_res;
	Ptr<CRes> pRes = (CRes*)pNode->GetData();

	InspectorUI* pInspectorUI = (InspectorUI*)CImGuiMgr::GetInst()->FindUI("Inspector");
	pInspectorUI->SetTargetRes(pRes);
}

void ContentUI::LoadLevel(DWORD_PTR _levelPath)
{
	TreeNode* pNode = (TreeNode*)_levelPath;
	string& strLevelPath = pNode->GetNodeKey();

	tEvent evn = {};
	evn.eType = EVENT_TYPE::CHANGE_LEVEL;
	evn.wParam = DWORD_PTR(strLevelPath.data());

	CEventMgr::GetInst()->AddEvent(evn);
}

void ContentUI::FindContentFileName(const wstring& _strFolderPath)
{
	// find name of all files
	wstring strFolderPath = _strFolderPath + L"*.*";

	HANDLE hFindHandle = nullptr;

	WIN32_FIND_DATA data = {};

	// Ž���� �ڵ� �������
	hFindHandle = FindFirstFile(strFolderPath.c_str(), &data);

	if (INVALID_HANDLE_VALUE == hFindHandle)
		return;

	// ��� ������ Ȯ�� �� �� ���� ��������� �ݺ�
	while (FindNextFile(hFindHandle, &data))
	{
		if (FILE_ATTRIBUTE_DIRECTORY == data.dwFileAttributes && wcscmp(data.cFileName, L".."))
		{
			FindContentFileName(_strFolderPath + data.cFileName + L"\\");
		}
		else
		{
			wstring strRelative = GetRelativePath(CONTENTPATH, _strFolderPath + data.cFileName);
			m_vecContentName.push_back(strRelative);
		}
	}

	FindClose(hFindHandle);
}

RES_TYPE ContentUI::GetResTypeByExt(wstring _filename)
{
	wstring strExt = path(_filename.c_str()).extension();

	if (strExt == L".pref")
		return RES_TYPE::PREFAB;
	else if (strExt == L".mdat")
		return RES_TYPE::MESHDATA;
	else if (strExt == L".mtrl")
		return RES_TYPE::MATERIAL;
	else if (strExt == L".mesh")
		return RES_TYPE::MESH;
	else if (strExt == L".dds" || strExt == L".DDS" || strExt == L".tag" || strExt == L".TGA"
		|| strExt == L".png" || strExt == L".jpg" || strExt == L".JPG" || strExt == L".jegp" || strExt == L".JPEG"
		|| strExt == L".bmp" || strExt == L".BMP")
		return RES_TYPE::TEXTURE;
	else if (strExt == L".mp3" || strExt == L".MP3" || strExt == L".wav" || strExt == L".WAV" || strExt == L".ogg" || strExt == L".OGG")
		return RES_TYPE::SOUND;
	else if (strExt == L".anim")
		return RES_TYPE::ANIMATION2D;
	else
		return RES_TYPE::END;
}

