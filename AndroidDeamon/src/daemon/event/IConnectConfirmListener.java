package daemon.event;

public interface IConnectConfirmListener {
     public boolean isAllowConnect(String ip, String pcName, int timeout);
}
