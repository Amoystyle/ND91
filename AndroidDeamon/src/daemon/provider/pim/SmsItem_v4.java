package daemon.provider.pim;

import daemon.model.pim20.SmsItem;

public class SmsItem_v4 {
    private SmsItem item;
    private long time;
    private int connectId;
    
    
	public SmsItem getItem() {
		return item;
	}
	public void setItem(SmsItem item) {
		this.item = item;
	}
	public long getTime() {
		return time;
	}
	public void setTime(long time) {
		this.time = time;
	}
	public int getConnectId() {
		return connectId;
	}
	public void setConnectId(int connectId) {
		this.connectId = connectId;
	}
    
    
    
}
