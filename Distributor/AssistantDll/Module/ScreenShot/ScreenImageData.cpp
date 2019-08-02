#include "pch_module.h"

#ifdef MODULE_SCREENSHOT

#include "ScreenImageData.h"
#include "ScreenImageConvert.h"

ScreenImageData::ScreenImageData(void)
{
	_ScreenImageBinaryData =  "";

	_rotateType	= RotateNoneFlip;

	_scale		=			 1.0;

#ifdef WIN32
	_pImage     =			NULL;
#endif	
}

ScreenImageData::~ScreenImageData(void)
{
#ifdef WIN32	
	SAFE_DELETE(_pImage);
#endif
}

bool ND91Assistant::ScreenImageData::ParseAndroidData( BinaryBufferReader& reader, bool IsUsb)
{
	reader.ReadInt32(_ScreenInfo._bpp);
	if(IsUsb)
	{
		int  size;
		reader.ReadInt32(size);
	}
	reader.ReadInt32(_ScreenInfo._width);
	reader.ReadInt32(_ScreenInfo._height);
	reader.ReadInt32(_ScreenInfo._red_offset);
	reader.ReadInt32(_ScreenInfo._red_length);
	reader.ReadInt32(_ScreenInfo._blue_offset);
	reader.ReadInt32(_ScreenInfo._blue_length);
	reader.ReadInt32(_ScreenInfo._green_offset);
	reader.ReadInt32(_ScreenInfo._green_length);
	reader.ReadInt32(_ScreenInfo._alpha_offset);
	reader.ReadInt32(_ScreenInfo._alpha_length);
	return true;
}

void ND91Assistant::ScreenImageData::FreshBitmap()
{
	if(_ScreenImageBinaryData.empty())
		return;
		
#ifdef WIN32
	Bitmap* temp = ScreenImageConvert::ConvertScreenImage(_ScreenImageBinaryData, _ScreenInfo);
	if(temp)
	{
		SAFE_DELETE(_pImage);
		_pImage = temp;
	}
	if(_scale != 1.0)
	{
		temp = (Bitmap*)(_pImage->GetThumbnailImage((UINT)(_pImage->GetWidth()*_scale), (UINT)(_pImage->GetHeight()*_scale)));
		//temp = ScreenImageConvert::ScaleImage(_scale, _pImage);
		if(temp)
		{
			SAFE_DELETE(_pImage);
			_pImage = temp;
		}
	}
	if(_rotateType != RotateNoneFlip && _pImage)
	{
		switch(_rotateType)
		{
			case Rotate90Flip:
				_pImage->RotateFlip(Gdiplus::Rotate90FlipNone);
				break;
			case Rotate180Flip:
				_pImage->RotateFlip(Gdiplus::Rotate180FlipNone);
				break;
			case Rotate270Flip:
				_pImage->RotateFlip(Gdiplus::Rotate270FlipNone);
				break;
		}
	}
#endif	
}

void ND91Assistant::ScreenImageData::SaveToFile( wstring path, const DeviceData* pDevice)
{
#ifdef WIN32	
	Bitmap* temp = ScreenImageConvert::ConvertScreenImage(_ScreenImageBinaryData, _ScreenInfo);
	if(temp)
	{
		if ((int)CStrOperation::toLowerW(pDevice->GetDevInfo()->_deviceHWModel).find(L"zte-")>-1)
			temp->RotateFlip(Gdiplus::Rotate180FlipNone);
		DUMP_PNG(temp, path.c_str());  
	}
	SAFE_DELETE(temp);
#endif	
}

bool ND91Assistant::ScreenImageData::SetScreenBinaryData( string data, bool fresh )
{
	if(!data.empty() && data != _ScreenImageBinaryData)
	{
		_ScreenImageBinaryData = data;
		if (fresh)
			FreshBitmap();
		return true;
	}
	return false;
}

bool ND91Assistant::ScreenImageData::SetRotateFlipType( NDRotateFlipType type )
{
	if(_rotateType != type)
	{
		_rotateType = type;
		FreshBitmap();
		return true;
	}
	return false;
}

bool ND91Assistant::ScreenImageData::SetScale( float scale )
{
	if(_scale != scale)
	{
		_scale = scale;
		FreshBitmap();
		return true;
	}
	return false;
}

void ND91Assistant::ScreenImageData::ResetShowType()
{
	_scale = 1.0;
	_rotateType = RotateNoneFlip;
	FreshBitmap();
}

#endif