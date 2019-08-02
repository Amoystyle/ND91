package daemon.common;

import java.io.File;

import android.os.StatFs;

public class FileSpaceInfo {
	private String mPath;
	private File mFile;
	private long total;
	private long free;
	private long used;
	
	public FileSpaceInfo(String path) {
		this(new File(path));
	}
	
	public FileSpaceInfo(File file) {
		this.mFile = file;
		this.mPath = mFile.getPath();
		String path = file.getPath();
		StatFs sf = new StatFs(path);
		long blockSize = sf.getBlockSize();
		long blockTotalCount = sf.getBlockCount();
		long blockFreeCount = sf.getAvailableBlocks();
		total = blockTotalCount * blockSize;
		free = blockFreeCount * blockSize;
		used = total - free;
	}

	public String getPath() {
		return mPath;
	}

	public void setPath(String path) {
		this.mPath = path;
	}

	public File getFile() {
		return mFile;
	}

	public void setFile(File file) {
		this.mFile = file;
	}

	public long getTotal() {
		return total;
	}

	public void setTotal(long total) {
		this.total = total;
	}

	public long getFree() {
		return free;
	}

	public void setFree(long free) {
		this.free = free;
	}

	public long getUsed() {
		return used;
	}

	public void setUsed(long used) {
		this.used = used;
	}
	
	
}
