#include "pch.h"
#include "ContentUI.h"

#include "TreeUI.h"

#include "InspectorUI.h"
#include "CImguiMgr.h"

#include <Engine/CEventMgr.h>
#include <Engine/CResMgr.h>

ContentUI::ContentUI()
	: UI("Content")
	, m_Tree(nullptr)
{
	m_Tree = new TreeUI("##ContentTree");
	AddChild(m_Tree);

	m_Tree->SetShowSeperator(false);
	m_Tree->SetDummyRoot(true);

	m_Tree->AddDynamic_Select(this, (FUNC_1)&ContentUI::SetResourceToInspector);


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
	m_Tree->Clear();

	// ���� �߰�
	TreeNode* pRootNode = m_Tree->AddItem(nullptr, "", "Content", 0);

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		// RES_TYPE�� �ش��ϴ� ���ڿ��� �θ�� �߰�
		TreeNode* pResNode = m_Tree->AddItem(pRootNode, "", ToString((RES_TYPE)i), 0, true);

		// �� RES_TYPE�� ���� Res���� �ڽ����� �߰� (data�� ����.)
		const map<wstring, Ptr<CRes>>& mapRes = CResMgr::GetInst()->GetResource(RES_TYPE(i));
		map<wstring, Ptr<CRes>>::const_iterator iter = mapRes.begin();
		for (; iter != mapRes.end(); ++iter)
		{
			wstring strKey = iter->first;
			wstring strName = (iter->second).Get()->GetName();
			m_Tree->AddItem(pResNode, WstrToStr(strKey), WstrToStr(strName), (DWORD_PTR)iter->second.Get());
		}
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
			continue;

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

