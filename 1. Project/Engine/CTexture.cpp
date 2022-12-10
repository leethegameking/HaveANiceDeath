#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"


CTexture::CTexture()
    : CRes(RES_TYPE::TEXTURE)
    , m_Desc{}
{
}

CTexture::~CTexture()
{
}

int CTexture::Load(const wstring& _strFilePath)
{
    HRESULT hr = E_FAIL;

    wchar_t szExt[50] = {};
    _wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

    wstring strExt = szExt;

    if (strExt == L".dds" || strExt == L".DDS")
    {
        hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
    }

    else if (strExt == L".tga" || strExt == L".TGA")
    {
        hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
    }

    else // WIC (png, jpg, jpeg, bmp )
    {
        hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
    }

    if (FAILED(hr))
        return hr;


    // sysmem -> GPU
    hr = CreateShaderResourceView(DEVICE
        , m_Image.GetImages()
        , m_Image.GetImageCount()
        , m_Image.GetMetadata()
        , m_SRV.GetAddressOf());

    m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
    m_Tex2D->GetDesc(&m_Desc);

    return hr;
}

void CTexture::Create(UINT _iWidth, UINT _iHeight, DXGI_FORMAT _Format, UINT _iBindFlag)
{
    // DepthStencilTexture
    m_Desc.BindFlags = _iBindFlag;

    m_Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
    m_Desc.CPUAccessFlags = 0;

    m_Desc.Format = _Format;
    m_Desc.Width = _iWidth;
    m_Desc.Height = _iHeight;
    m_Desc.ArraySize = 1;

    m_Desc.SampleDesc.Count = 1;
    m_Desc.SampleDesc.Quality = 0;

    m_Desc.MipLevels = 1;
    m_Desc.MiscFlags = 0;

    HRESULT hr = DEVICE->CreateTexture2D(&m_Desc, nullptr, m_Tex2D.GetAddressOf());
    assert(!FAILED(hr));

    // View 持失
    if (_iBindFlag & D3D11_BIND_DEPTH_STENCIL)
    {
        hr = DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    }
    else
    {
        if (_iBindFlag & D3D11_BIND_RENDER_TARGET)
        {
            hr = DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (_iBindFlag & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.Format = m_Desc.Format;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &tSRVDesc, m_SRV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (_iBindFlag & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.Format = m_Desc.Format;
            tUAVDesc.Texture2D.MipSlice = 0;            
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &tUAVDesc, m_UAV.GetAddressOf());
            assert(!FAILED(hr));
        }
    }    
}

void CTexture::Create(ComPtr<ID3D11Texture2D> _Tex2D)
{
    m_Tex2D = _Tex2D;
    m_Tex2D->GetDesc(&m_Desc);

    // View 持失
    HRESULT hr = S_OK;
    if (m_Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
    {
        hr = DEVICE->CreateDepthStencilView(m_Tex2D.Get(), nullptr, m_DSV.GetAddressOf());
    }
    else
    {
        if (m_Desc.BindFlags & D3D11_BIND_RENDER_TARGET)
        {
            hr = DEVICE->CreateRenderTargetView(m_Tex2D.Get(), nullptr, m_RTV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (m_Desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
            tSRVDesc.Format = m_Desc.Format;
            tSRVDesc.Texture2D.MipLevels = 1;
            tSRVDesc.Texture2D.MostDetailedMip = 0;
            tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateShaderResourceView(m_Tex2D.Get(), &tSRVDesc, m_SRV.GetAddressOf());
            assert(!FAILED(hr));
        }

        if (m_Desc.BindFlags & D3D11_BIND_UNORDERED_ACCESS)
        {
            D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
            tUAVDesc.Format = m_Desc.Format;
            tUAVDesc.Texture2D.MipSlice = 0;
            tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
            hr = DEVICE->CreateUnorderedAccessView(m_Tex2D.Get(), &tUAVDesc, m_UAV.GetAddressOf());
            assert(!FAILED(hr));
        }
    }
}

void CTexture::UpdateData(UINT _iRegisterNum, UINT _iPipelineStage)
{
    if ((UINT)PIPELINE_STAGE::VS & _iPipelineStage)
        CONTEXT->VSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::HS & _iPipelineStage)
        CONTEXT->HSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::DS & _iPipelineStage)
        CONTEXT->DSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::GS & _iPipelineStage)
        CONTEXT->GSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

    if ((UINT)PIPELINE_STAGE::PS & _iPipelineStage)
        CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
}

void CTexture::UpdateData_CS(UINT _iRegisterNum, bool _bShaderRes)
{
    if (_bShaderRes)
    {
        CONTEXT->CSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());
    }
    else
    {
        assert(m_UAV.Get());

        UINT i = -1;
        CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, m_UAV.GetAddressOf(), &i);
    }
}

void CTexture::Clear(UINT _iRegisterNum)
{
    ID3D11ShaderResourceView* pSRV = nullptr;
    
    CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
    CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);    
}

void CTexture::Clear_CS(UINT _iRegisterNum)
{
    ID3D11UnorderedAccessView* pUAV = nullptr;
    UINT i = -1;
    CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, &pUAV, &i);
}

void CTexture::GetPixelVector(vector<vector<tBGRA>>& _inVec)
{
    uint8_t* pixelMem = m_Image.GetPixels();
    const Image* image = m_Image.GetImages();

    for (int width = 0; width < image->width; ++width)
    {
        static vector<tBGRA> vecTmp;
        static tBGRA tmp = {};
        for (int height = 0; height < image->height; ++height)
        {
            tmp = {};

            tmp.b = (pixelMem[0 + (height * image->width + width) * 4]);
            tmp.g = (pixelMem[1 + (height * image->width + width) * 4]);
            tmp.r = (pixelMem[2 + (height * image->width + width) * 4]);
            tmp.a = (pixelMem[3 + (height * image->width + width) * 4]);
            tmp.check = false;

            vecTmp.push_back(tmp);
        }
        _inVec.push_back(vecTmp);
        vecTmp.clear();
    }
}

Vec4 CTexture::WIdthSearch(vector<vector<tBGRA>>& _inVec, Vec2 _inPos, bool _opt)
{
    Vec2 size = GetSize();
    Vec2 hasAlpha;

    Vec4 Out;
    UINT OutLeft, OutRight, OutUp, OutDown;

    static list<Vec2> queue;
    if (_inPos.x < 0.f)
        _inPos.x = 0.f;
    if (_inPos.x >= size.x)
        _inPos.x = size.x - 1.f;
    if (_inPos.y < 0.f)
        _inPos.y = 0.f;
    if (_inPos.y >= size.y)
        _inPos.y = size.y - 1.f;

    queue.push_back(_inPos);

    while (!queue.empty())
    {
        if (_inVec[(UINT)queue.front().x][(UINT)queue.front().y].a != 0)
        {
            hasAlpha = queue.front();
            break;
        }

        Vec2 right = Vec2(queue.front().x + 1.f, queue.front().y);
        Vec2 down = Vec2(queue.front().x, queue.front().y + 1.f);

        if(right.x < size.x)
            queue.push_back(right);
        if (_inPos.x == queue.front().x && down.y < size.y)
            queue.push_back(down);

        queue.pop_front();
    }

    OutLeft = (UINT)hasAlpha.x;
    OutRight = (UINT)hasAlpha.x;
    OutUp = (UINT)hasAlpha.y;
    OutDown = (UINT)hasAlpha.y;

    queue.clear();
    queue.push_back(hasAlpha);
    _inVec[(UINT)hasAlpha.x][(UINT)hasAlpha.y].a = true;

    while (!queue.empty())
    {
        if (queue.front().x < OutLeft)
            OutLeft = queue.front().x;
        if (queue.front().x > OutRight)
            OutRight = queue.front().x;
        if (queue.front().y < OutUp)
            OutUp = queue.front().y;
        if (queue.front().y > OutDown)
            OutDown = queue.front().y;

        Vec2 up = Vec2(queue.front().x, queue.front().y - 1);
        Vec2 down = Vec2(queue.front().x, queue.front().y + 1);
        Vec2 left = Vec2(queue.front().x - 1, queue.front().y);
        Vec2 right = Vec2(queue.front().x + 1, queue.front().y);

        if (up.y >= 0 && _inVec[up.x][up.y].a != 0 &&_inVec[up.x][up.y].check == false )
        {
            queue.push_back(up);
            _inVec[up.x][up.y].check = true;
        }
        if (down.y < size.y && _inVec[down.x][down.y].a != 0 && _inVec[down.x][down.y].check == false )
        {
            queue.push_back(down);
            _inVec[down.x][down.y].check = true;
        }
        if (left.x >= 0 && _inVec[left.x][left.y].a != 0 && _inVec[left.x][left.y].check == false )
        {
            queue.push_back(left);
            _inVec[left.x][left.y].check = true;
        }
        if (right.x < size.x && _inVec[right.x][right.y].a != 0 && _inVec[right.x][right.y].check == false )
        {
            queue.push_back(right);
            _inVec[right.x][right.y].check = true;
        }

        queue.pop_front();
    }
   
    return Vec4((float)OutLeft, (float)OutUp, (float)OutRight, (float)OutDown);
}

void CTexture::CheckClear(vector<vector<tBGRA>>& _inVec)
{
    Vec2 size = GetSize();

	for (size_t i = 0; i < size.x; ++i)
	{
		for (size_t j = 0; j < size.y; ++j)
		{
			_inVec[i][j].check = false;
		}
	}
}
