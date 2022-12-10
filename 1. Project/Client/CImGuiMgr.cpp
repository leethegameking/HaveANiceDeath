#include "pch.h"
#include "CImGuiMgr.h"

#include <Engine/CDevice.h>

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "UI.h"
#include "ParamUI.h"

CImGuiMgr::CImGuiMgr()
{
}

CImGuiMgr::~CImGuiMgr()
{
    Safe_Del_Map(m_mapUI);

    clear();
}

void CImGuiMgr::init(HWND _hwnd)
{
    // ImGui 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_hwnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    CreateUI();
}

void CImGuiMgr::progress()
{
    ImGuiIO& io = ImGui::GetIO();

    // ImGui Update    
    {
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    // ID 초기화
    ParamUI::ParamCount = 0;

    // 추가된 부분
    {
        bool bTrue = true;
        ImGui::ShowDemoWindow(&bTrue);

        map<string, UI*>::iterator iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->update();
        }

        iter = m_mapUI.begin();
        for (; iter != m_mapUI.end(); ++iter)
        {
            iter->second->render();
        }
    }


    // ImGui Render              
    {
        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }
}

void CImGuiMgr::clear()
{
    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

#include "InspectorUI.h"
#include "ListUI.h"
#include "ContentUI.h"
#include "OutlinerUI.h"
#include "AnimCreateTool.h"
#include "FrameTool.h"
#include "TileEditor.h"
#include "AnimTool.h"

void CImGuiMgr::CreateUI()
{
    UI* pUI = nullptr;

    pUI = new InspectorUI;
    m_mapUI.insert({ pUI->GetName(), pUI });

    pUI = new ContentUI;
    m_mapUI.insert({ pUI->GetName(), pUI});

    pUI = new OutlinerUI;
    m_mapUI.insert({ pUI->GetName(), pUI });

    pUI = new ListUI("ListUIModal");
    pUI->SetSize(Vec2(200.f, 500.f));
    pUI->SetModal(true);
    m_mapUI.insert({ pUI->GetName(), pUI });

    pUI = new ListUI("ListUI");
    pUI->SetSize(Vec2(200.f, 500.f));
    pUI->SetPopUp(true);
    m_mapUI.insert({ pUI->GetName(), pUI });

    pUI = new AnimCreateTool;
    pUI->SetSize(Vec2(200.f, 500.f));
    pUI->SetPopUp(true);
    m_mapUI.insert({ pUI->GetName(), pUI });

    pUI = new FrameTool;
    pUI->SetSize(Vec2(200.f, 500.f));
    pUI->SetPopUp(true);
    m_mapUI.insert({ pUI->GetName(), pUI });

    pUI = new TileEditor();
    pUI->SetSize(Vec2(200.f, 500.f));
    pUI->SetPopUp(true);
    m_mapUI.insert({ pUI->GetName(), pUI });

    pUI = new AnimTool();
    pUI->SetSize(Vec2(200.f, 500.f));
    pUI->SetPopUp(true);
    m_mapUI.insert({ pUI->GetName(), pUI });
}

UI* CImGuiMgr::FindUI(const string& _name)
{
    map<string, UI*>::iterator iter = m_mapUI.find(_name);
    if (iter == m_mapUI.end())
        return nullptr;

    return iter->second;
}


