package daemon.connection;

import java.net.Socket;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

import android.content.Context;
import android.os.Bundle;

import daemon.access.ProviderAccess;
import daemon.access.ProviderAccessManager;
import daemon.event.ChangeConnectListener;
import daemon.event.ConnectionEvent;
import daemon.event.IConnectConfirmListener;
import daemon.event.PcMediaStatusChangeListener;
import daemon.logger.LogCenter;
import daemon.net.base.IPeerEventObserver;
import daemon.net.base.Peer;
import daemon.net.base.PeerEvent;
import daemon.net.nio.NioPeer;
import daemon.net.nio.SelectReactor;
import daemon.net.original.OriginalPeer;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.MemoryStream;
import daemon.util.ConfigManager;

//负责管理长连接
public class ConnectionManager implements IPeerEventObserver {
	private final String TAG = getClass().getName();
	private ArrayList<ConnectionInfo> mConnections;
	private Object mLock = new Object();
	private LinkedList<Peer> mPeers = new LinkedList<Peer>();
	private List<ChangeConnectListener> mChangeConnecttListener = new ArrayList<ChangeConnectListener>();
	private PcMediaStatusChangeListener mediaStatusChangeListener;
	private HeartbeatService mHeartBeatService = HeartbeatService.getInstance();
	private IConnectConfirmListener mConfirmListener = null;

	private ConnectionManager() {
		mConnections = new ArrayList<ConnectionInfo>();
	}

	private static ConnectionManager instance;

	// 单例
	public static synchronized ConnectionManager getInstance() {
		if (instance == null)
			instance = new ConnectionManager();
		return instance;
	}

	public void setConfirmListener(IConnectConfirmListener listener) {
		mConfirmListener = listener;
	}

	public NioPeer createNioPeer(Context context, SocketChannel channel, SelectReactor reactor) {
		NioPeer peer = new NioPeer(context, channel, reactor);
		handleNewPeer(peer);
		
		return peer;
	}

	public OriginalPeer createOriginalPeer(Context context, Socket socket) {
		OriginalPeer peer = new OriginalPeer(context, socket);
		handleNewPeer(peer);
		
		return peer;
	}

	public void addChangeConnectListener(ChangeConnectListener listener) {
		synchronized (mChangeConnecttListener) {
			mChangeConnecttListener.add(listener);
		}
	}

	private void handleNewPeer(Peer peer) {
		synchronized (mLock) {
			mPeers.add(peer);
		}
		peer.setEventObserver(this);
	}

	private void handlePeerClosed(Peer peer) {
		synchronized (mLock) {
			if (mPeers.contains(peer))
				mPeers.remove(peer);
		}
		ConnectionInfo conn = getConnection(peer);

		if (conn != null) {
			if (conn.isPlayMediaStatus()) {
				// stop media
				stopPlayMedia(peer.getContext(), conn);
			}
			closePeers(conn.getConnectionId());
			synchronized (mLock) {
				mConnections.remove(conn);
				LogCenter.debug(TAG, "close conn",null,true);
			}
			synchronized (mChangeConnecttListener) {
				for (ChangeConnectListener listener : mChangeConnecttListener) {
					ConnectionEvent event = new ConnectionEvent(conn);
					listener.disConnected(event);
				}
			}

		}
	}

	private void stopPlayMedia(Context context, ConnectionInfo conn) {
		try {
			ByteWriter writer = new ByteWriter();
			writer.Write(15);		
			MemoryStream ms = new MemoryStream(writer.getBaseStream().GetBuffer());			
			ms.SetLength(4);
			writer = new ByteWriter();
	        ByteReader reader = new ByteReader(ms);
	    	reader.getBaseStream().setOffset(0);
			ProviderAccess pa = ProviderAccessManager.getInstance(context). createAccess(212, conn.getConnectionId());
			pa.execute(reader, writer);
		} catch (Exception e) {

		}
	}

	private void closePeers(int connectionId) {

		List<Peer> peersToClose = new ArrayList<Peer>();
		synchronized (mLock) {
			// 先记录要关闭的peer，然后再关闭移除，直接在for循环中关闭会导致集合改变异常
			for (Peer peer : mPeers) {
				if (peer.getConnectionId() == connectionId) {
					peersToClose.add(peer);
				}
			}
			for (Peer peer : peersToClose) {
				peer.close();
				mPeers.remove(peer);
			}
		}
	}

	public boolean createConnection(Context context, Peer peer, String pcName, int timeout, boolean doConfirm) {
		if (isWifiConnect(peer.getIPAddress())) {
			if (ConfigManager.getIsWifiOn(context)) {
				if (doConfirm && ConfigManager.getIsNeedConnectConfirm(context) && mConfirmListener != null) {
					boolean isAllow = mConfirmListener.isAllowConnect(peer.getIPAddress(), pcName, timeout);
					if (!isAllow)
						return false;
				}
			}
			else {// 如果wifi没开则不允许连接
				return false;
			}
		}

		synchronized (mLock) {
			ConnectionInfo conn = new ConnectionInfo(peer, pcName);
			if (conn != null && conn.isWifi() && mConnections.size() > 0) {
				// Log.e(" wifi mConnections add", ""+mConnections.size());
				List<ConnectionInfo> closedConnections = new ArrayList<ConnectionInfo>();
				for (ConnectionInfo connect : mConnections) {
					if (connect.isWifi()) {
						closePeers(connect.getConnectionId());
						closedConnections.add(connect);
					}
				}
				mConnections.removeAll(closedConnections);
			}
			mConnections.add(conn);
			mHeartBeatService.doHeartbeat(peer);
			if (!mHeartBeatService.isStarted())
				mHeartBeatService.start();
			if (conn != null && conn.isWifi()) {
				synchronized (mChangeConnecttListener) {
					for (ChangeConnectListener listener : mChangeConnecttListener) {
						ConnectionEvent event = new ConnectionEvent(conn);
						listener.connected(event);
					}
				}
			}
			return true;
		}
	}

	public void sendMessage(final int businessId, final ByteWriter writer) {
		try {
			for (ConnectionInfo conn : mConnections) {
				HeartbeatService.getInstance().sendMessage(
						conn.getConnectionId(), businessId, writer);
				// conn.sendMessage(businessId, writer);
			}
		} catch (Exception e) {

		}
	}

	public void sendMessage(final int businessId, final int connectionId,
			final ByteWriter writer) {

		ConnectionInfo conn = getConnection(connectionId);

		if (conn != null) {
			HeartbeatService.getInstance().sendMessage(conn.getConnectionId(),
					businessId, writer);
		} else {
			LogCenter.error(TAG,
					"sendMessage, long connection not found, businessId:"
							+ businessId);
		}

	}

	public ArrayList<ConnectionInfo> getConnectionList() {
		return mConnections;
	}

	public boolean isConnecting() {
		return mConnections.size() > 0;
	}

	public synchronized boolean isWifiConnecting() {
		for (ConnectionInfo conn : mConnections) {
			if (conn.isWifi()) {
                 return true;
			}
		}
		return false;
	}
	
	public void closeAllConnections() {
		synchronized (mLock) {
			mConnections.clear();

			for (Peer peer : mPeers) {
				peer.close();
			}

			mPeers.clear();
		}
	}

	public void closeWifiConnections() {
		synchronized (mLock) {

			List<ConnectionInfo> closedConnections = new ArrayList<ConnectionInfo>();

			for (ConnectionInfo conn : mConnections) {
				if (conn.isWifi()) {
					closePeers(conn.getConnectionId());
					closedConnections.add(conn);
				}
			}

			mConnections.removeAll(closedConnections);
		}
	}

	private ConnectionInfo getConnection(Peer peer) {
		synchronized (mLock) {
			for (ConnectionInfo conn : mConnections) {
				if (conn.getPeer() == peer)
					return conn;
			}
		}

		return null;
	}

	private ConnectionInfo getConnection(int connectionId) {
		synchronized (mLock) {
			for (ConnectionInfo conn : mConnections) {
				if (conn.getConnectionId() == connectionId)
					return conn;
			}
		}

		return null;
	}

	public static boolean needConfirm(Context context, String ip) {
		if (isWifiConnect(ip)) {
			if (ConfigManager.getIsNeedConnectConfirm(context)) {
				return true;
			}
		}

		return false;
	}

	/**
	 * 判断是否wifi连接
	 * 
	 * @param strIP
	 * @return
	 */
	public static boolean isWifiConnect(String strIP) {
		if (strIP.startsWith("127.0.0.1")) {
			return false;
		}
		return true;
	}

	@Override
	public void onEvent(PeerEvent event, Peer peer) {
		synchronized (mLock) {
			switch (event) {
				case CONNECTION_CREATED:

					break;
				case CLOSED:
					handlePeerClosed(peer);
					break;
			}
		}
	}

	public void setPcMediaStatusChangeListener(PcMediaStatusChangeListener lister){
		mediaStatusChangeListener = lister;
	}
	
	public void pcMediaStatus(int connectionId, int status){
		ConnectionInfo conn = getConnection(connectionId);
		if(conn != null){
			conn.setPcMediaStatus(status);
		}
		
		if(mediaStatusChangeListener!=null){
			mediaStatusChangeListener.getStatus(status);
		}
	}
	
	public int getMediaStatus(){
		for(ConnectionInfo conn : mConnections){
			if(conn.getPcMediaStatus() != -1){
				return conn.getPcMediaStatus();
			}
		}
		
		return 10;
	}

	public void setConnectInfoPlayMediaStatus(int connectId, boolean statu) {
		synchronized (mLock) {

			for (ConnectionInfo conn : mConnections) {
				if(conn.getConnectionId() == connectId){
					conn.setsPlayMediaStatus(statu);
				}
			}
		}
	}
}
