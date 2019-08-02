package daemon.event;

import java.util.EventListener;

public interface ChangeConnectListener extends EventListener {
	
	public void disConnected(ConnectionEvent e);
	
	public void connected(ConnectionEvent e);
	
}
