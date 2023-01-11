#include "pch.h"
#include "SimpleText.h"
#include "CDevice.h"
#include "CEngine.h"
#include "CRenderMgr.h"
#include "CCamera.h"
#include "CTransform.h"

SimpleText::SimpleText()
{
}

SimpleText::~SimpleText()
{
}




void SimpleText::init()
{
    ComPtr<IDXGISwapChain> pSwapChain = CDevice::GetInst()->GetSwapChain();
    pSwapChain->GetBuffer(0, IID_PPV_ARGS(m_pBackBuffer.GetAddressOf()));

    float dpiX;
    float dpiY;

    D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pD2DFactory.GetAddressOf());
    dpiX = (float)GetDpiForWindow(CEngine::GetInst()->GetMainHwnd());
    dpiY = dpiX;



    D2D1_RENDER_TARGET_PROPERTIES props =
        D2D1::RenderTargetProperties(
            D2D1_RENDER_TARGET_TYPE_DEFAULT,
            D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
            dpiX,
            dpiY
        );

    m_pD2DFactory->CreateDxgiSurfaceRenderTarget(m_pBackBuffer.Get(), &props, m_pRT.GetAddressOf());

    int debug = 0;

    DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(IDWriteFactory),
        (IUnknown**)(m_pDWriteFactory.GetAddressOf())
    );

    HRESULT hr = m_pDWriteFactory->CreateTextFormat(
        L"Juice ITC", 0, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
        25, L"en-us", m_pTextFormat.GetAddressOf());
}


void SimpleText::Draw(wstring _text, Vec2 _WorldPos, Vec2 _Width_Height, D2D1_COLOR_F _color)
{
    if (m_pRT)
    {
        Vec2 resol = CDevice::GetInst()->GetRenderResolution();
        CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
        Vec3 vCamWorldPos = pMainCam->Transform()->GetWorldPos();
        float fCamScale = pMainCam->GetOrthographicScale();

        _WorldPos.x = (_WorldPos.x - vCamWorldPos.x) / fCamScale + resol.x / 2.f;
        _WorldPos.y = resol.y / 2.f - (_WorldPos.y - vCamWorldPos.y) / fCamScale;
        

        D2D1_SIZE_F targetSize = m_pRT->GetSize();

        m_pRT->BeginDraw();

        // 최대 높이 
        m_pDWriteFactory->CreateTextLayout(_text.c_str(), wcslen(_text.c_str()), m_pTextFormat.Get(), _Width_Height.x, _Width_Height.y, &m_pTextLayout);

        ComPtr<ID2D1SolidColorBrush> brush;
        D2D1_COLOR_F vColor = _color;
        m_pRT->CreateSolidColorBrush(vColor, brush.GetAddressOf());

        // Window 기준 Right Down
        m_pRT->DrawTextLayout(D2D1::Point2F(_WorldPos.x, _WorldPos.y), m_pTextLayout.Get(), brush.Get());

        m_pRT->EndDraw();
    }
}

void SimpleText::TestDraw()
{
    if (m_pRT)
    {
        D2D1_SIZE_F targetSize = m_pRT->GetSize();

        m_pRT->BeginDraw();

        wstring wstr = L"Hi Direct11?";

        // 최대 높이 
        m_pDWriteFactory->CreateTextLayout(wstr.c_str(), wcslen(wstr.c_str()), m_pTextFormat.Get(), 2000, 200, &m_pTextLayout);

        ComPtr<ID2D1SolidColorBrush> brush;
        D2D1_COLOR_F vColor = { 1.f, 0.f, 1.f, 1.f };
        m_pRT->CreateSolidColorBrush(vColor, brush.GetAddressOf());

        // Window 기준 Right Down
        m_pRT->DrawTextLayout(D2D1::Point2F(400, 500), m_pTextLayout.Get(), brush.Get());

        m_pRT->EndDraw();
    }
}


