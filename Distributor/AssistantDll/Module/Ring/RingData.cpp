#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/RingData.h"

RingData::RingData()
{
	_Id				= -1;			
	_IsSystemRing	= false;	
	_ringType		= Null;		
	_size			= 0;			
}

void RingData::Assign(const RingData* pData)
{
	_name       = pData->_name      ;      
	_mode       = pData->_mode      ;      
	_owner      = pData->_owner     ;     
	_group      = pData->_group     ;     
	_size       = pData->_size      ;		
	_modifyTime = pData->_modifyTime;
	_createTime = pData->_createTime;
	_Parent     = pData->_Parent;

	_Id				= pData->_Id			;		//
	_duration		= pData->_duration		;		//铃声时常
	_artist			= pData->_artist		;		//艺术家/演唱者
	_album			= pData->_album			;		//专辑
	_Guid			= pData->_Guid			;       //IOS铃声标记
	_IsSystemRing	= pData->_IsSystemRing	;		//是否系统铃声
	_ringType		= pData->_ringType		;		//铃声类型, MediaFileKind中值的组合?
	_title			= pData->_title			;		//标题名称
	_remotePath		= pData->_remotePath	;		//全路径 
}

RingData::~RingData()
{

}


std::wstring ND91Assistant::RingData::GetFullPath() const
{
	return _remotePath;
}


#endif