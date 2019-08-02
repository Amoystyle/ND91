#pragma once
#include "SystemSetDlgInterface.h"


class SystemSetDlgModel
{
public:
	SystemSetDlgModel(SystemSetDlgInterface *pInterface);
	~SystemSetDlgModel();

private:
	SystemSetDlgInterface *_pInterface;

public:
	void Save();

};
