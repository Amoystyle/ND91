#include "stdafx.h"
#include "virtualnamedirdata.h"



VirtualNameDirData::VirtualNameDirData(const DeviceData* pDevice)
: DirData(pDevice)
{
	_virtualname = _T("");
	_name = _T("");
}

VirtualNameDirData::VirtualNameDirData( DirData *dirdata )
:DirData(*dirdata)
{

}

VirtualNameDirData::~VirtualNameDirData()
{

}

std::wstring VirtualNameDirData::GetVirtualname()
{
	return _virtualname;
}

void VirtualNameDirData::SetVirtualname( wstring name )
{
	_virtualname = name;
}

std::wstring VirtualNameDirData::GetLogo()
{
	return _logo;
}

void VirtualNameDirData::SetLogo( wstring logo )
{
	_logo = logo;
}

