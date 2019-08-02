package daemon.net.base;

import daemon.net.base.Peer;

//事件观察者
public interface IPeerEventObserver {
	
	public void onEvent(PeerEvent event, Peer peer);
	
}
