package daemon.net.base;

//Nio socket属性信息
public class EventHandlerAttribute {

	//设置当前Nio socket无效
	public void setAvailable(boolean v)
	{
		mAvailableFlag = v;
	}
	
	//当前Nio socket是否有效
	public boolean isAvailable()
	{
		return mAvailableFlag;
	}
	
	//将当前Nio socket设置成挂起
	public void setSuspend(boolean v)
	{
		mSuspendFlag = v;
	}
	
	//当前Nio socket是否挂起
	public boolean isSuspend()
	{
		return mSuspendFlag;
	}
	
	private volatile boolean mAvailableFlag  = true;
	private volatile boolean mSuspendFlag = false;
}
