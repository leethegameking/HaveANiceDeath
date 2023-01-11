#include "pch.h"
#include "SimpleText.h"
#include "CDevice.h"
#include "CEngine.h"


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
        L"궁서체", 0, DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_NORMAL,
        DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL,
        45, L"ko", m_pTextFormat.GetAddressOf());
}





void SimpleText::TestDraw()
{
    if (m_pRT)
    {
        D2D1_SIZE_F targetSize = m_pRT->GetSize();

        m_pRT->BeginDraw();

        wstring wstr = L"안녕 다이렉트11?";
        m_pDWriteFactory->CreateTextLayout(wstr.c_str(), wcslen(wstr.c_str()), m_pTextFormat.Get(), 800, 500, &m_pTextLayout);


        ComPtr<ID2D1SolidColorBrush> brush;
        D2D1_COLOR_F vColor = { 1.f, 0.f, 1.f, 1.f };
        m_pRT->CreateSolidColorBrush(vColor, brush.GetAddressOf());

        m_pRT->DrawTextLayout(D2D1::Point2F(100, 200), m_pTextLayout.Get(), brush.Get());

        m_pRT->EndDraw();
    }
}


