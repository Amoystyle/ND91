#pragma once
App_CC_BEGIN
enum ApplicationMsgType
{
	ApplicationMsgType_Begin,
	ApplicationMsgType_LoadData,
	ApplicationMsgType_End,
};

struct ApplicationInnerMsg : public BaseMessage<ApplicationInnerMsg>
{
	ApplicationMsgType type;
};
App_CC_END