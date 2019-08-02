package daemon.event;

import java.util.EventListener;

public interface DisconnectListener extends EventListener {
	
	public void disconnected(ConnectionEvent e);
}
