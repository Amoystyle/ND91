#pragma once

struct CommonDlgMsg : public BaseMessage<CommonDlgMsg>
{
	CommDlgType commonDlgType;
	WPARAM wParam;
	LPARAM lParam;//Ϊ��Ӧ�ĵ�DialogParam�ṹ�壬�ɽ������ͷ�
};