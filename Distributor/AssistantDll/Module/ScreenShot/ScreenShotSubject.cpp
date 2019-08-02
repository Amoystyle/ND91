#include "pch_module.h"

#ifdef MODULE_SCREENSHOT

#include "Module/ScreenShot/ScreenShotSubject.h"
#include "Module/ScreenShot/ScreenImageData.h"
#include "Module/ScreenShot/GetScreenImageInfoCommand.h"
#include "Module/ScreenShot/ScreenObserver.h"

ScreenShotSubject::ScreenShotSubject(const DeviceData* pDevice)
{
	_pDevice = pDevice;

	_pScreenImage = new ScreenImageData();

	GetScreenImageInfoCommand cmd(_pDevice);
	cmd.SetParam(_pScreenImage);
	cmd.Execute();
}

ScreenShotSubject::~ScreenShotSubject(void)
{
	SAFE_DELETE(_pScreenImage);
}

void ND91Assistant::ScreenShotSubject::SendFreshScreen()
{
	for(size_t i = 0; i < _observers.size(); ++i)
	{
		ScreenObserver* pObserver = dynamic_cast<ScreenObserver*>(_observers[i]);

		if ( pObserver )
			pObserver->freshScreen(_pScreenImage);
	}
}

void ND91Assistant::ScreenShotSubject::NextRotate()
{
	NDRotateFlipType  type = RotateNoneFlip;
	switch(_pScreenImage->GetRotateFlipType())
	{
	case RotateNoneFlip:
		type = Rotate270Flip;
		break;
	case Rotate270Flip:
		type = Rotate180Flip;
		break;
	case Rotate180Flip:
		type = Rotate90Flip;
		break;
	case Rotate90Flip:
		type = RotateNoneFlip;
		break;
	}
	if(_pScreenImage->SetRotateFlipType(type))
		SendFreshScreen();
}

void ND91Assistant::ScreenShotSubject::SetScaleType(ScaleType type,int width, int height)
{
	float scale = 1.0;
	switch(type)
	{
		case Scale_None:
			scale = 1.0;
			break;
		case Scale_Half:
			scale = 0.5;
			break;
		case Scale_Twice:
			scale = 2.0;
			break;
		case Scale_Fix:
			{
				float scale1 = (float)(width- 70 - 150) / _pScreenImage->GetWidth();
				float scale2 = (float)(height - 70 - 50) / _pScreenImage->GetHeight();
				scale = scale1 > scale2 ? scale2 : scale1;
			}
			break;
		default:
			break;
	}
	if (_pScreenImage->SetScale(scale))
		SendFreshScreen();

}

void ND91Assistant::ScreenShotSubject::SetScreenImageBinaryData( string data )
{
	if (_pScreenImage->SetScreenBinaryData(data))
		SendFreshScreen();
}

void ND91Assistant::ScreenShotSubject::ResetScreenImageShow()
{
	_pScreenImage->ResetShowType();
}

#endif