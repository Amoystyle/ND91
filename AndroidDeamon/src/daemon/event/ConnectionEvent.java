package daemon.event;

import java.util.EventObject;

import daemon.connection.ConnectionInfo;

public class ConnectionEvent extends EventObject {

	private static final long serialVersionUID = 1L;

	public ConnectionEvent(Object source) {
		super(source);
	}

	public ConnectionInfo getConnection() {
		return (ConnectionInfo) super.source;
	}
}
