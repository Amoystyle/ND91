package daemon.net.base;

//当前连接状态
public enum PeerState
{
	None,              //未知
	Idle,              //空闲
	Receiving,         //接收数据中
	Received,          //接收数据完毕
	DeliverPackage,    //处理业务包
	Close,             //关闭
	Error,             //错误
}