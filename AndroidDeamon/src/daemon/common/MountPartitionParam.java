package daemon.common;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.text.TextUtils;

import daemon.shell.Shell;

public class MountPartitionParam {
	public static final String SYSTEM = "/system";
	public static final String DATA = "/data";

	private String source;
	private String target;
	private String filesystemType;

	private MountPartitionParam(String source, String target, String filesystemType) {
		this.source = source;
		this.target = target;
		this.filesystemType = filesystemType;
	}

	private static MountPartitionParam createInstance(String target) {
		MountPartitionParam param = null;
		
		String[] lines = Shell.getInstance().execForString("mount").split("\n");
		String format = "(\\S+)\\s(\\S*\\s)?%1$s\\s(type\\s)?(\\S+)";
		String strPattern = String.format(format, target);
		Pattern pattern = Pattern.compile(strPattern);
		String source = "";
		String filesystemType = "";
		for (String line : lines) {
			Matcher matcher = pattern.matcher(line);
			if (matcher.find()) {
				source = matcher.group(1);
				filesystemType = matcher.group(4);
				break;
			}
		}
		if (!TextUtils.isEmpty(source) && !TextUtils.isEmpty(filesystemType)) {
			param = new MountPartitionParam(source, target, filesystemType);
		}

		return param;
	}

	public static MountPartitionParam createPartitionParam(String filePath) {
		MountPartitionParam result = null;
		int index = filePath.length();
		while (index > 0) {
			String folder = filePath.substring(0, index);
			MountPartitionParam param = createInstance(folder);
			if (param != null) {
				result = param;
				break;
			}
			filePath = folder;
			index = filePath.lastIndexOf("/");
		}

		return result;
	}

	public String getSource() {
		return source;
	}

	public void setSource(String source) {
		this.source = source;
	}

	public String getTarget() {
		return target;
	}

	public void setTarget(String target) {
		this.target = target;
	}

	public String getFilesystemType() {
		return filesystemType;
	}

	public void setFilesystemType(String filesystemType) {
		this.filesystemType = filesystemType;
	}

}
