#pragma once

struct CommonDlgMsg : public BaseMessage<CommonDlgMsg>
{
	CommDlgType commonDlgType;
	WPARAM wParam;
	LPARAM lParam;//为相应的的DialogParam结构体，由接收者释放
};