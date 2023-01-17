#pragma once



class SimpleText
	: public CSingleton<SimpleText>
{
private:
	// 인터페이스
	ComPtr<IDWriteFactory>	m_pDWriteFactory;
	ComPtr<IDWriteTextFormat>	m_pTextFormat[(int)TEXT_FORMAT::END];

	// text
	const wchar_t*	m_pwText;
	UINT			m_iTextLength;

	// 렌더 관련
	ComPtr<ID2D1Factory> m_pD2DFactory;
	ComPtr<ID2D1RenderTarget> m_pRT;
	ComPtr<IDXGISurface> m_pBackBuffer;
	ComPtr<IDWriteTextLayout> m_pTextLayout;

	vector<tText>	m_vecText;

public:
	void init();

public:
	// 나중에 Text크기도 조정할 수 있게 변경.
	void AddDrawVec(tText _tText);
	void Draw();
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
