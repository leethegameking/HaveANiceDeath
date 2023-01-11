#pragma once

#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "D2D1.lib")
#pragma comment(lib, "Dwrite.lib")

class SimpleText
	: public CSingleton<SimpleText>
{
private:
	// 인터페이스
	ComPtr<IDWriteFactory>	m_pDWriteFactory;
	ComPtr<IDWriteTextFormat>	m_pTextFormat;

	// text
	const wchar_t*	m_pwText;
	UINT			m_iTextLength;

	// 렌더 관련
	ComPtr<ID2D1Factory> m_pD2DFactory;
	ComPtr<ID2D1RenderTarget> m_pRT;
	ComPtr<IDXGISurface> m_pBackBuffer;
	ComPtr<IDWriteTextLayout> m_pTextLayout;

public:
	void init();


public:
	// 나중에 Text크기도 조정할 수 있게 변경.
	void Draw(wstring _text, Vec2 _WorldPos, Vec2 _Width_Height = { 2000.f, 200.f }, D2D1_COLOR_F _color = { 1.f, 1.f, 1.f, 1.f });
	void TestDraw();

public:
	template <class T> 
	void SafeRelease(T** ppT);

public:
	SimpleText();
	~SimpleText();

};

template<class T>
inline void SimpleText::SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
