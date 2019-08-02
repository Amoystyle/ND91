#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/IosUserRingData.h"
#include "Module/Ring/RingOperationCommand.h"

bool ND91Assistant::IosUserRingData::RenameRing( wstring title )
{
	RingOperationCommand cmd(_Parent->_pDeviceData);
	cmd.RenameRing(this, title);
	if(cmd.DoneSuccess())
		this->_title = title;
	return cmd.DoneSuccess();
}

bool ND91Assistant::IosUserRingData::SetIosRing()
{
	RETURN_FALSE_IF( _Parent->_pDeviceData->GetDevInfo()->_deviceType != Ios );
	RETURN_FALSE_IF( !_Parent->_pDeviceData->GetDevInfo()->_ios_bJailBreaked );
	RETURN_FALSE_IF( _IsSystemRing );	// 只有用户自定义铃声才能【设置为来电铃声】

	RingOperationCommand cmd(_Parent->_pDeviceData);
	return cmd.SettingRing(this, Ring);

}

NodeData* ND91Assistant::IosUserRingData::Clone( DirData* pParent ) const
{
	IosUserRingData* pNode = new IosUserRingData();
	pNode->Assign(this);
	pNode->_Parent = pParent;

	return pNode;
}



#endif