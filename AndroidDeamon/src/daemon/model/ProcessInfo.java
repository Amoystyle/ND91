package daemon.model;

import daemon.protocol.ByteReader;

public class ProcessInfo {
	private int pid;
	private int ppid;
	private int ruid;
	private int euid;

	public ProcessInfo() {
		
	}
	
	public ProcessInfo(ByteReader reader) {
		
	}
	
	public int getPid() {
		return pid;
	}

	public void setPid(int pid) {
		this.pid = pid;
	}

	public int getPpid() {
		return ppid;
	}

	public void setPpid(int ppid) {
		this.ppid = ppid;
	}

	public int getRuid() {
		return ruid;
	}

	public void setRuid(int ruid) {
		this.ruid = ruid;
	}

	public int getEuid() {
		return euid;
	}

	public void setEuid(int euid) {
		this.euid = euid;
	}
}
