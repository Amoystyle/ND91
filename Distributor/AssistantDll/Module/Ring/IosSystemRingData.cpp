#include "pch_module.h"

#ifdef MODULE_RING

#include "Module/Ring/IosSystemRingData.h"
#include "Module/Ring/RingOperationCommand.h"

bool ND91Assistant::IosSystemRingData::Replace( wstring filePath )
{
	RETURN_FALSE_IF( _Parent->_pDeviceData->GetDevInfo()->_deviceType != Ios);
	RETURN_FALSE_IF( ! _IsSystemRing );	// 只有系统铃声才能【替换】

	RingOperationCommand cmd(_Parent->_pDeviceData);
	return cmd.ReplaceIosRing(this, filePath);

}


NodeData* ND91Assistant::IosSystemRingData::Clone( DirData* pParent ) const
{
	IosSystemRingData* pNode = new IosSystemRingData();
	pNode->Assign(this);
	pNode->_Parent = pParent;

	return pNode;
}



#endif