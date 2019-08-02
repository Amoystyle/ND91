package daemon.util;

public class FileInfo {
	
	private boolean isExist =false;
	private String path;
	private long size;
	private long time;
	private int mode;
	private boolean hasChild;
	private boolean isCanWrite;
	public boolean isExist() {
		return isExist;
	}
	public void setExist(boolean isExist) {
		this.isExist = isExist;
	}
	public String getPath() {
		return path;
	}
	public void setPath(String path) {
		this.path = path;
	}
	public long getTime() {
		return time;
	}
	public void setTime(long time) {
		this.time = time;
	}
	public long getSize() {
		return size;
	}
	public void setSize(long size) {
		this.size = size;
	}
	public int getMode() {
		return mode;
	}
	public void setMode(int mode) {
		this.mode = mode;
	}
	public boolean isHasChild() {
		return hasChild;
	}
	public void setHasChild(boolean hasChild) {
		this.hasChild = hasChild;
	}
	public boolean isCanWrite() {
		return isCanWrite;
	}
	public void setCanWrite(boolean isCanWrite) {
		this.isCanWrite = isCanWrite;
	}
	
     
	
}
