package daemon.net.base;

import java.nio.channels.SelectableChannel;
import java.nio.channels.spi.AbstractSelectableChannel;

//Nio Socket抽象事件基类
public abstract class EventHandler extends EventHandlerAttribute {
	// 获取socket channel
	public AbstractSelectableChannel getHandle() {
		return null;
	}

	// 获取当前channel监听的事件标志
	public int getInterestOp() {
		return 0;
	}

	// 处理Accept socket事件
	public boolean handleAccept(SelectableChannel c) {
		return true;
	}

	// 处理socket Connect事件
	public boolean handleConnect(SelectableChannel c) {
		return true;
	}

	// 处理socket数据发送
	public boolean handleInput(SelectableChannel c) {
		return true;
	}

	// 处理socket数据接收
	public boolean handleOutput(SelectableChannel c) {
		return true;
	}
}
/*
 * //Nio Socket抽象事件基类 public interface IEventHandler { //获取socket channel public
 * AbstractSelectableChannel getHandle(); //获取当前channel监听的事件标志 public int
 * getInterestOp(); //处理Accept socket事件 public boolean
 * handleAccept(SelectableChannel c); //处理socket Connect事件 public boolean
 * handleConnect(SelectableChannel c); //处理socket数据发送 public boolean
 * handleInput(SelectableChannel c); //处理socket数据接收 public boolean
 * handleOutput(SelectableChannel c); }
 */