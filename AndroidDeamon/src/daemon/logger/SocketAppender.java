package daemon.logger;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SocketChannel;
import java.nio.channels.Selector;
import java.nio.channels.spi.SelectorProvider;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SelectionKey;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.Vector;
import java.util.Set;
import java.util.Iterator;

public class SocketAppender implements Appender {

	private static int BIND_PORT = 65525;
	
	private boolean terminated_ = false;
	private ServerSocketChannel ss_channel;
	private Selector selector_;
	private Thread listener_thr_;
	private Formatter formatter_;
	private Vector<SocketChannel> clients_ = new Vector<SocketChannel>(10);
	
	class SocketHandler 
	{	
		
		public void onAccept( SelectionKey k ) throws IOException {}
		public void onConnect( SelectionKey k ) throws IOException {}		
		public void onRead( SelectionKey k ) throws IOException {}		
		public void onWrite( SelectionKey k ) throws IOException {}
	}
	
	private class ListenerThreadFunc implements Runnable
	{
		public void run()
		{
			try
			{
				ss_channel = ServerSocketChannel.open();
				ss_channel.socket().setReuseAddress(true);
				ss_channel.configureBlocking(false);
				ss_channel.socket().bind(new InetSocketAddress(BIND_PORT));
				ss_channel.register(selector_, SelectionKey.OP_ACCEPT, new SocketHandler() {
					public void onAccept(SelectionKey k)
					{
						try
						{
							ServerSocketChannel ssc = (ServerSocketChannel)k.channel();
							SocketChannel ch = ssc.accept();			
							ch.configureBlocking( false );
							ch.socket().setTcpNoDelay(true);
							clients_.add(ch);
						}
						catch (IOException e)
						{
						}
					}
				});
			
				for (;;)
				{
					selector_.select();
					
					if (terminated_)
					{
						break;
					}

					synchronized(this)
					{
						Set<SelectionKey> rdy = selector_.selectedKeys();
						Iterator<SelectionKey> i = rdy.iterator();
						while (i.hasNext()) 
						{
							SelectionKey k = i.next();
							i.remove();
							if(!k.isValid()) continue;		// Key could be cancelled
							SocketHandler h = (SocketHandler) k.attachment();
							int op = k.readyOps();
							try 
							{
								// Figure out what happens and dispatch it to the thread.  Don't change
								// the order unless you know what you're doing!
								if ( (op & SelectionKey.OP_WRITE) != 0   ) h.onWrite( k );
								if ( (op & SelectionKey.OP_READ) != 0    ) h.onRead( k );							
								if ( (op & SelectionKey.OP_ACCEPT) != 0  ) h.onAccept( k );
								if ( (op & SelectionKey.OP_CONNECT) != 0 ) h.onConnect( k );
							} 
							catch( IOException e ) 
							{
								try 
								{
									k.channel().close();
								} 
								catch( IOException e2 ) 
								{								
								}
							}
						}
					}
				}
			}
			catch (IOException e)
			{
			}
			
			Set<SelectionKey> allKeys = selector_.keys();
			Iterator<SelectionKey> i = allKeys.iterator();
			while (i.hasNext()) 
			{
				SelectionKey k = i.next();
				try 
				{
					k.channel().close();
				} 
				catch (IOException e ) 
				{
				}
			}
		}
	}
	
	protected void finalize()
	{
		close();
	}
	
	public synchronized void doLog(String name, long time, Level level, Object message, Throwable t)
	{
		if (clients_.isEmpty() || null == formatter_)
		{
			return;
		}
		
		String str = formatter_.format(name, time, level, message, t);
		
		Vector<SocketChannel> well_clients = new Vector<SocketChannel>(10);
		
		for (int i = clients_.size() - 1; i >= 0; --i)
		{
			SocketChannel sc = clients_.elementAt(i);
			
			try
			{
				ByteBuffer bb = ByteBuffer.allocate(str.getBytes().length);
				bb.order(ByteOrder.BIG_ENDIAN);
				bb.put(str.getBytes("gb2312"));
				bb.flip();
				
				int rst = sc.write(bb); 
				
				if (rst >= 0)
				{
					well_clients.add(clients_.elementAt(i));
				}
			}
			catch (IOException e)
			{
			}
		}
		
		clients_ = null;
		clients_ = well_clients;
	}

	public void clear()
	{
	}

	public synchronized boolean close()
	{
		if (null == ss_channel)
		{
			return false;
		}
		
		try
		{	
			for (int i = clients_.size() - 1; i >= 0; --i)
			{
				try
				{
					clients_.elementAt(i).close();
				}
				catch (IOException e)
				{
				}
			}
		
			clients_.clear();
			terminated_ = true;
			selector_.wakeup();
			listener_thr_.join(1000);
			ss_channel.close();
			ss_channel.socket().close();
			ss_channel = null;
			return true;
		}
		catch (InterruptedException ie)
		{
		}
		catch (Exception e)
		{
		}
		
		return false;
	}
	
	public synchronized boolean open()
	{
		if (null != ss_channel)
		{
			return true;
		}
		
		try
		{
			selector_ = SelectorProvider.provider().openSelector();
			listener_thr_ = new Thread(new ListenerThreadFunc());
			listener_thr_.start();
			return true;
		}
		catch (IOException e)
		{
		}

		return false;
	}

	public boolean isLogOpen()
	{
		return (!clients_.isEmpty());
	}

	public void setFormatter(Formatter formatter)
	{
		formatter_ = formatter;
	}

	public void setProperty(String name, String value)
		throws IllegalArgumentException
	{
	}
	
}
