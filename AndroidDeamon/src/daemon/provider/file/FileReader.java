package daemon.provider.file;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

import daemon.logger.LogCenter;

public class FileReader {

	String fileName;
	long length;
	RandomAccessFile fileAccess;

	public static FileReader CreateReader(String fileName) {
		File file = new File(fileName);
		if (file.exists()) {
			FileReader reader = new FileReader(fileName);
			reader.length = file.length();
			return reader;
		}
		return null;
	}

	private FileReader(String fileName) {
		this.fileName = fileName;
	}

	public String getFileName() {
		return fileName;
	}

	public long getLength() {
		return length;
	}

	public int read(long position, byte[] buffer) {
		if (fileAccess == null) {
			try {
				fileAccess = new RandomAccessFile(fileName, "r");
			} catch (FileNotFoundException e) {
				LogCenter.error("daemon.provider.file.FileReader", e
						.getMessage());
			}
		}
		if (fileAccess != null) {
			try {
				fileAccess.seek(position);
				return fileAccess.read(buffer, 0, buffer.length);
			} catch (IOException e) {
				LogCenter.error("daemon.provider.file.FileReader", e
						.getMessage());
			}
		}
		return -1;
	}

	public void close() {
		if (fileAccess != null) {
			try {
				fileAccess.close();
				fileAccess = null;
			} catch (IOException e) {
				LogCenter.error("daemon.provider.file.FileReader", e
						.getMessage());
			}
		}
	}
}
