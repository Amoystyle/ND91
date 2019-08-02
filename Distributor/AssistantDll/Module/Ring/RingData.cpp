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
	_duration		= pData->_duration		;		//����ʱ��
	_artist			= pData->_artist		;		//������/�ݳ���
	_album			= pData->_album			;		//ר��
	_Guid			= pData->_Guid			;       //IOS�������
	_IsSystemRing	= pData->_IsSystemRing	;		//�Ƿ�ϵͳ����
	_ringType		= pData->_ringType		;		//��������, MediaFileKind��ֵ�����?
	_title			= pData->_title			;		//��������
	_remotePath		= pData->_remotePath	;		//ȫ·�� 
}

RingData::~RingData()
{

}


std::wstring ND91Assistant::RingData::GetFullPath() const
{
	return _remotePath;
}


#endif