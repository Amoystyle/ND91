package daemon.provider.file;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.Environment;
import android.os.StatFs;
import android.text.TextUtils;
import daemon.common.Common;
import daemon.common.FileSpaceInfo;
import daemon.common.MD5Util;
import daemon.connection.ConnectionManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.receiver.SendMessageType;
import daemon.util.ZipHelp;

public class FileManagerProvider implements IProvider {
	private Context mContext;
	private FileReader mReader;

	public FileManagerProvider(Context context) {
		this.mContext = context;
	}

	@Override
	public void execute(ProviderExecuteContext cxt) {
		byte action = cxt.getByteReader().ReadByte();
		switch (action) {
		case 0: 
			String fileName = cxt.getByteReader().ReadString();
			if (locateFile(fileName))
				cxt.getByteWriter().WriteString("OKAY");
			else
				cxt.getByteWriter().WriteString("FAIL");		
			break;
		case 1:
			if (mReader != null)
				cxt.getByteWriter().Write(mReader.getLength());

			break;
		case 2:
			readFile(cxt);
			break;
		case 3:
			closeReader();
			cxt.getByteWriter().WriteString("OKAY");
			break;
		case 4:
			String srcFile = cxt.getByteReader().ReadString();
			String dstFile = cxt.getByteReader().ReadString();
			boolean overwriter = cxt.getByteReader().ReadBoolean();
			boolean cpyResult = copyFile(srcFile, dstFile, overwriter);
			cxt.getByteWriter().Write(cpyResult);
			break;
		case 5:
			File external = Environment.getExternalStorageDirectory();
			if (external != null) {
				cxt.getByteWriter().WriteString("OKAY");
				cxt.getByteWriter().Write(external.getPath());
			} else {
				cxt.getByteWriter().WriteString("FAIL");
			}
			break;
		case 6:
			String path = cxt.getByteReader().ReadString();
			boolean success = makeDir(path);
			cxt.getByteWriter().Write(success);
			break;
		case 7:
			// 根据传入路径，显示文件（夹）名称，大小，类型(文件夹/快捷方式/文件),子文件数量
			readFileInfo(cxt);
			break;
		case 8:
			// 删除文件
			String delFilePath = cxt.getByteReader().ReadString();
			byte state = deleteFile(new File(delFilePath));
			cxt.getByteWriter().Write(state);
			break;
		case 9:
			// 获取权限
			String currPath = cxt.getByteReader().ReadString();
			String permissionResult = getPermission(currPath);
			cxt.getByteWriter().Write(permissionResult);
			break;
		case 10:
			refreshMedia();
			break;
		case 11:
			getDirInfo(cxt);
			break;
		case 12:
			makeDir(cxt);
			break;
		case 13:
			removeFile(cxt);
			break;
		case 14:
			moveFile(cxt);
			break;
		case 15:
			copyFile(cxt);
			break;
		case 16:
			getFileAttr(cxt);
			break;
		case 17:
			chmod(cxt);
			break;
		case 18:
			doZip(cxt);
			break;
		
		case 19:
			getDirInfoSubWritable(cxt);
			break;
		case 20:
			getFileAttrWritable(cxt);
			break;
		case 21:
		    getDirSize(cxt);
		    break;
		case 22:
		    getFileMd5(cxt);
		    break;
		case 23:
		    removeFileList(cxt);
		    break;
		case 24:
		    scanMatchConditionFiles(cxt);
		    break;
		case 25:
			stopScanMatchConditionFiles(cxt);
		    break;
		    
		}
	}
	
	


	
    private void stopScanMatchConditionFiles(ProviderExecuteContext cxt) {
    	long id =cxt.getByteReader().ReadInt64();
    	ScanMatchConditionFileHelper.getInstance().stopScan(id);
    	cxt.getByteWriter().Write(1);
	}

	private void scanMatchConditionFiles(ProviderExecuteContext cxt) {
		ByteReader reader=cxt.getByteReader();
		ByteWriter writer=cxt.getByteWriter();
		boolean readTempFile = reader.ReadBoolean();
		boolean readLogFile = reader.ReadBoolean();
		boolean readErrorFile =reader.ReadBoolean();
		boolean readEmptyFile = reader.ReadBoolean();
		boolean readApkFile = reader.ReadBoolean();
		long bigfileLengh = reader.ReadInt64();
		
		String path = Environment.getExternalStorageDirectory().getAbsolutePath();
		FileSpaceInfo info =new FileSpaceInfo(Environment.getExternalStorageDirectory());
		long used =info.getUsed();
		File sdcardFile = new File(path);
		if(!sdcardFile.exists()){
			writer.Write(0);
			return;
		}
		List<File> list = new ArrayList<File>();
		list.add(sdcardFile);
		String exPath = Common.getExternalSDCardPath(mContext);
		if(exPath!=null&&!exPath.equals("")){
			list.add(new File(exPath));
			FileSpaceInfo infoEx =new FileSpaceInfo(exPath);
			used+=infoEx.getUsed();
		}
		writer.Write(1);
		final long scanId = System.currentTimeMillis();
		writer.Write(scanId);
		
		ScanMatchConditionFileHelper.getInstance().scan(mContext, used, list, scanId, readTempFile, readLogFile, readErrorFile, readEmptyFile, readApkFile, bigfileLengh);
	}


	private void removeFileList(ProviderExecuteContext cxt) {
		ByteReader reader=cxt.getByteReader();
		ByteWriter writer=cxt.getByteWriter();
		int count =reader.ReadInt32();
		for (int i = 0; i < count; i++) {
			try {
				String path = reader.ReadString();
				byte[] data = FileOperater.remove(this.mContext, path);
				writer.Write(1);
				writer.Write(data);
			} catch (Exception e) {
				e.printStackTrace();
				writer.Write(0);
			}
		}
		
	}

	private void getFileMd5(ProviderExecuteContext cxt) {
		ByteReader reader=cxt.getByteReader();
		ByteWriter writer=cxt.getByteWriter();
		String filename = reader.ReadString();
		File f = new File(filename);
		if(!f.exists()){
			writer.Write(0);
			return;
		}
		String md5 = MD5Util.getFileMD5String(f);
		writer.Write(1);
		writer.Write(md5);
	}

	private void doZip(ProviderExecuteContext cxt) {
		ByteReader reader=cxt.getByteReader();
		ByteWriter writer=cxt.getByteWriter();
		String srcPath=reader.ReadString();
		String dstPath=reader.ReadString();
		boolean zipChild=reader.ReadBoolean();
		File srcFile=new File(srcPath);
		if(srcFile==null||!srcFile.exists()){
			writer.Write(0);
			return;
		}
		try {
			if(srcFile.isDirectory()&&zipChild){
	            ZipHelp.doZipDirectory(srcPath, dstPath);
			}else{
				ZipHelp.doZip(srcPath, dstPath);
			}
			writer.Write(1);
		} catch (Exception e) {
			writer.Write(0);
		}

		
		
	}

	private void chmod(ProviderExecuteContext ctx) {
		String path = ctx.getByteReader().ReadString();
		int mode = ctx.getByteReader().ReadInt32();
		byte[] data = FileOperater.chmod(mContext, path, mode);
		ctx.getByteWriter().Write(data);
	}

	private void getFileAttr(ProviderExecuteContext ctx) {
		try {
			String path = ctx.getByteReader().ReadString();
			byte[] data = FileOperater.getFileAttr(mContext, path);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void getFileAttrWritable(ProviderExecuteContext ctx) {
		try {
			String path = ctx.getByteReader().ReadString();
			byte[] data = FileOperater.getFileAttrWritable(mContext, path);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void copyFile(ProviderExecuteContext ctx) {
		try {
			String oldPath = ctx.getByteReader().ReadString();
			String newPath = ctx.getByteReader().ReadString();
			// int result = FileJni.copyFile(oldPath, newPath);
			byte[] data = FileOperater.copy(mContext, oldPath, newPath);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void moveFile(ProviderExecuteContext ctx) {
		try {
			String oldPath = ctx.getByteReader().ReadString();
			String newPath = ctx.getByteReader().ReadString();
			byte[] data = FileOperater.move(this.mContext, oldPath, newPath);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void removeFile(ProviderExecuteContext ctx) {
		try {
			String path = ctx.getByteReader().ReadString();
			byte[] data = FileOperater.remove(this.mContext, path);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void makeDir(ProviderExecuteContext ctx) {
		try {
			String path = ctx.getByteReader().ReadString();
			byte[] data = FileOperater.makeDir(this.mContext, path);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void getDirSize(ProviderExecuteContext ctx){
	    long size = 0;
	    try {
	        String path = ctx.getByteReader().ReadString();
	        File fileInfo = new File(path);
	        String absolutePath = fileInfo.getAbsolutePath();
            String canonicalPath = fileInfo.getCanonicalPath();
            
	        if(fileInfo.isFile() )
            {
               size = fileInfo.length();
            }
	        else if(fileInfo.isDirectory() && absolutePath.equals(canonicalPath))
            {
	           size = getFolderSize(fileInfo);
	        } 
	        //size = getFolderSize(fileInfo);
	        ctx.getByteWriter().Write(size);
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
        }
	}
	
    public static long getFolderSize(java.io.File file) throws Exception {
        long size = 0;
        java.io.File[] fileList = file.listFiles();
        for (int i = 0; i < fileList.length; i++) {
            String absolutePath = fileList[i].getAbsolutePath();
            String canonicalPath = fileList[i].getCanonicalPath();
            if (fileList[i].isDirectory() && absolutePath.equals(canonicalPath)) {
                size = size + getFolderSize(fileList[i]);
            } else {
                size = size + fileList[i].length();
            }
        }
        return size;
    }
	  
	private void getDirInfo(ProviderExecuteContext ctx) {
		try {
			String path = ctx.getByteReader().ReadString();
			byte[] data = FileOperater.getDirInfo(this.mContext, path);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
	
	private void getDirInfoSubWritable(ProviderExecuteContext ctx) {
		try {
			String path = ctx.getByteReader().ReadString();
			byte[] data = FileOperater.getDirInfoSubWritable(this.mContext, path);
			ctx.getByteWriter().Write(data);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * 获取权限
	 * 
	 * @param currPath
	 * @return
	 */
	public String getPermission(String currPath) {

		String result = getPermissionWithParamsD(currPath);
		if (TextUtils.isEmpty(result)) {
			result = getPermissionWithoutParamsD(currPath);
		}
		return result;
	}

	private String getPermissionWithParamsD(String currPath) {
		String result = "";
		try {
			String format = "ls -l -d %1$s";
			String cmd = String.format(format, currPath);
			result = execCommand(cmd).toLowerCase();
			if (result == null || result.contains("no such file or directory")
					|| result.contains("permission denied")
					|| result.contains("not a directory")) {
				return "";
			}
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
		return result.substring(0, 10);
	}

	private String getPermissionWithoutParamsD(String currPath) {
		String result = "";
		try {
			String format = "ls -l %1$s";
			String cmd = String.format(format, currPath);
			result = execCommand(cmd).toLowerCase();
			if (result == null || result.contains("no such file or directory")
					|| result.contains("permission denied")
					|| result.contains("not a directory")) {
				return "";
			}
		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}
		return result.substring(0, 10);
	}

	private String execCommand(String command) throws IOException {
		Runtime runtime = Runtime.getRuntime();
		Process proc = runtime.exec(command);
		InputStream inputstream = proc.getInputStream();
		InputStreamReader inputstreamreader = new InputStreamReader(inputstream);
		BufferedReader bufferedreader = new BufferedReader(inputstreamreader);
		String line = "";
		line = bufferedreader.readLine();
		try {
			if (proc.waitFor() != 0) {
				System.err.println("exit value = " + proc.exitValue());
			}
		} catch (InterruptedException e) {
			System.err.println(e);
		}
		return line;
	}

	/**
	 * 删除文件
	 * 
	 * @param file
	 * @return
	 */
	public byte deleteFile(File file) {

		try {
			if ((!file.canRead()) && (!file.canWrite())) {
				return (byte) 11;
			}
			if (!file.exists()) {
				return (byte) 10;
			}
			if (file.isFile()) {
				if (file.delete()) {
					return (byte) 0;
				} else {
					return (byte) 13;
				}
			}

			File[] list = file.listFiles();
			if (list.length > 0) {
				for (File subFile : list) {
					byte result = deleteFile(subFile);
					if (result != (byte) 0)
						return result;
				}
			}

			if (file.listFiles().length == 0)
				return file.delete() ? (byte) 0 : (byte) 13;

		} catch (Exception e) {
			return (byte) 12;
		}

		return 13;
	}

	private void readFileInfo(ProviderExecuteContext cxt) {
		String dirpath = cxt.getByteReader().ReadString();
		File f = new File(dirpath);
		File[] subFiles = null;
		if (f.isDirectory())
			subFiles = f.listFiles();
		else
			subFiles = new File[] { f };

		if (subFiles != null) {
			cxt.getByteWriter().Write(subFiles.length);
			try {
				for (File file : subFiles) {
					// 路径
					cxt.getByteWriter().Write(file.getPath());
					// 子文件数量
					if (file.isDirectory() && file.list() != null) {
						cxt.getByteWriter().Write(file.list().length);
					} else {
						cxt.getByteWriter().Write(0);
					}
					// 修改时间
					cxt.getByteWriter().Write(file.lastModified());
					// 名称
					cxt.getByteWriter().Write(file.getName());
					// 大小
					cxt.getByteWriter().Write(file.length());
					String canonicalPath = file.getCanonicalPath();
					if (canonicalPath == null) {
						// 完整路径
						cxt.getByteWriter().Write(file.getPath());
						// 类型0 文件，1文件夹，2链接
						if (!file.isDirectory()) {
							cxt.getByteWriter().Write((byte) 0);
						} else {
							cxt.getByteWriter().Write((byte) 1);
						}
					} else {
						cxt.getByteWriter().Write(canonicalPath);
						if (!file.isDirectory()) {
							cxt.getByteWriter().Write((byte) 0);
						} else if (canonicalPath.equals(file.getPath())) {
							cxt.getByteWriter().Write((byte) 1);
						} else {
							cxt.getByteWriter().Write((byte) 2);
						}
					}

				}

			} catch (IOException e) {
				e.printStackTrace();
			}
		} else {
			cxt.getByteWriter().Write(0);
		}

	}

	private boolean makeDir(String path) {
		try {
			File file = new File(path);
			if (!file.exists()) {
				return file.mkdir();
			}
		} catch (Exception e) {

		}

		return false;
	}

	private boolean locateFile(String fileName) {
		if (mReader != null) {
			if (!mReader.getFileName().equals(fileName)) {
				mReader.close();
				mReader = FileReader.CreateReader(fileName);
			}
		} else {
			mReader = FileReader.CreateReader(fileName);
		}
		return mReader != null;
	}

	private void readFile(ProviderExecuteContext cxt) {
		long pos = cxt.getByteReader().ReadInt64();
		int length = cxt.getByteReader().ReadInt32();
		byte[] buffer = new byte[length];
		if (mReader != null) {
			int read = mReader.read(pos, buffer);
			cxt.getByteWriter().Write(read);
			cxt.getByteWriter().Write(buffer);
		}
	}

	private void closeReader() {
		if (mReader != null) {
			mReader.close();
		}
	}

	private boolean copyFile(String srcFile, String dstFile, boolean overwrite) {
		try {
			java.io.File file_out = new java.io.File(dstFile);
			if (file_out.exists()) {
				if (!overwrite)
					throw new IOException("Dest file exists!");
				else
					file_out.delete();
			}
			FileInputStream in1 = new FileInputStream(srcFile);
			FileOutputStream out1 = new FileOutputStream(dstFile);
			byte[] bytes = new byte[1024];
			int c;
			while ((c = in1.read(bytes)) != -1)
				out1.write(bytes, 0, c);
			in1.close();
			out1.close();
			return true;
		} catch (Exception e) {
			System.out.println("Error!");
			return false;
		}
	}

	@Override
	public int getBusiness() {
		return 20;
	}

	/**
	 * 刷新媒体数据库
	 * 
	 */
	private void refreshMedia() {
		mContext.sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri
				.parse("file://" + Environment.getExternalStorageDirectory())));

	}

}
