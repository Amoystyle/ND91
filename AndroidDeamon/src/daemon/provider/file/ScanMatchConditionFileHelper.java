package daemon.provider.file;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.ResourceBundle.Control;

import android.content.Context;
import android.content.ContextWrapper;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Environment;
import daemon.connection.ConnectionManager;
import daemon.logger.LogCenter;
import daemon.protocol.ByteWriter;
import daemon.receiver.SendMessageType;

public class ScanMatchConditionFileHelper {
	
	private static ArrayList<Long> stopScanMatchConditionFilesIDlist =new ArrayList<Long>();

	/*
	 * 1临时文件
	 * 2程序日志文件
	 * 3失效文件
	 * 4空白文件
	 * 5apk文件
	 * 6大文件
	 * */
	private final int TEMP_FILE =1;
	private final int LOG_FILE =2;
	private final int ERRORAPK_FILE =3;
	private final int EMPTY_FILE =4;
	private final int APK_FILE =5;
	private final int BIG_FILE =6;
    class MatchConditionFile{
    	int type;
    	String path;
    	long size;
    }
    
    
    private static ScanMatchConditionFileHelper mScanMatchConditionFileHelper;
    public static ScanMatchConditionFileHelper getInstance(){
    	if(mScanMatchConditionFileHelper==null){
    		mScanMatchConditionFileHelper = new ScanMatchConditionFileHelper();
    	}
    	return mScanMatchConditionFileHelper;
    }
    
    public void scan(final Context mContext,final long used,final List<File> listFile,final long scanId,
    		final boolean tempFile,final boolean logFile,final boolean errorFIle,final boolean emptyFile,
    		final boolean apkFile,final long bigfileLengh){
		
    	
		new Thread(){
			private long hasScanCount;
			private long time;
			private int lastProcess;
			ArrayList<MatchConditionFile>  sendList =new ArrayList<ScanMatchConditionFileHelper.MatchConditionFile>();
			public void run(){
				hasScanCount =0;
				lastProcess =0;
				time = System.currentTimeMillis();
				for (int i = 0; i < listFile.size(); i++) {
					getAllFiles(listFile.get(i),scanId,tempFile,logFile,errorFIle,emptyFile,apkFile,bigfileLengh);
				}
				
				
				if(sendList.size()>0){
                    sendList(sendList);
                    sendList.clear();
				}
				sendProgress(used,true);
			}
			
			private void sendFileInfo(MatchConditionFile file){
//				ByteWriter writer = new ByteWriter();
//				writer.Write(SendMessageType.FILE_MATCHCONDITION);
//				writer.Write(scanId);
//				writer.Write(file.type);
//				writer.Write(file.path);
//				writer.Write(file.size);
//				ConnectionManager.getInstance().sendMessage(SendMessageType.FILE_BUSINESS_ID, writer);
				sendList.add(file);
			}
			
		    protected void sendList(ArrayList<MatchConditionFile> sendList) {
		    	if(sendList.size()==0){
		    		return;
		    	}
				ByteWriter writer = new ByteWriter();
				writer.Write(SendMessageType.FILE_MATCHCONDITION);
				writer.Write(scanId);
				int count = sendList.size();
				writer.Write(count);
		    	for (int i = 0; i <count ; i++) {
		    		MatchConditionFile file = sendList.get(i);
					writer.Write(file.type);
					writer.Write(file.path);
					writer.Write(file.size);
				}
				ConnectionManager.getInstance().sendMessage(SendMessageType.FILE_BUSINESS_ID, writer);
			}
		    
			private void sendProgress(long progress,boolean isOver){
				ByteWriter writer = new ByteWriter();
				writer.Write(SendMessageType.FILE_SCANPROCESS);
				writer.Write(scanId);
				if(progress>used){
					progress =used;
					if(!isOver){
						progress --;
					}
				}
				if(used==0){
					writer.Write(100L);
					writer.Write(100L);
				}else{
					writer.Write(progress);
					writer.Write(used);
				}
				ConnectionManager.getInstance().sendMessage(SendMessageType.FILE_BUSINESS_ID, writer);
			}
			
			private void getAllFiles(File root, long scanId, boolean tempFile, boolean logFile, boolean errorFile, boolean emptyFile, boolean apkFile, long bigfileLengh){  
		        File files[] = root.listFiles();  
		        if(files != null){  
		            for (File f : files){  
		            	if(stopScanMatchConditionFilesIDlist.contains(scanId)){
		            		return;
		            	}
		            	if(hasScanCount!=0){
		            		int currProcess= (int) (100*hasScanCount*1.0f/used);
		            		if(currProcess>lastProcess){
		            			sendProgress(hasScanCount,false);
		            			lastProcess = currProcess;
		            		}
		            	}
		            	if(System.currentTimeMillis()-time>1*1000){
		            		time = System.currentTimeMillis();
                            //sendProgress(hasScanCount,false);
                            sendList(sendList);
                            sendList.clear();
		            	}
		                if(f.isDirectory()){
		                	if(f.getAbsolutePath().equals(Environment.getExternalStorageDirectory().getAbsolutePath()+"/nd")){
		                		continue;
		                	}
		                	getAllFiles(f,scanId,tempFile,logFile,errorFIle,emptyFile,apkFile,bigfileLengh);  
		                }else{  
		                	//判断是否符合条件
		                	String name = f.getName().toLowerCase();
		                	hasScanCount+=f.length();
			            	if(f.getAbsolutePath().endsWith("apk")&&f.getAbsolutePath().toLowerCase().contains("91 wireless/pandaspace")){
			            		continue;
			            	}
		                    if(bigfileLengh>=0&&f.length()>=bigfileLengh){
		                    	MatchConditionFile file =new MatchConditionFile();
		                    	file.type =BIG_FILE;
		                    	file.path = f.getAbsolutePath();
		                    	file.size =f.length();
		                    	sendFileInfo(file);
		                    }
		                    
		                    if(name.endsWith(".tmp")
			                        || name.endsWith(".temp")
			                        || (f.getParentFile() != null && (f.getParentFile().getName()
			                                .equalsIgnoreCase("temp") || f.getParentFile()
			                                .getName().equalsIgnoreCase("tmp"))))  {
		                    	if(tempFile){
			                    	MatchConditionFile file =new MatchConditionFile();
			                    	file.type =TEMP_FILE;
			                    	file.path = f.getAbsolutePath();
			                    	file.size =f.length();
			                    	sendFileInfo(file);  
		                    	}
		                    }else if (name.endsWith(".log")
			                        || (f.getParentFile() != null && f.getParentFile().getName()
			                                .equalsIgnoreCase("log"))){
		                    	if(name.endsWith("ndsh.log")){
		                    		continue;
		                    	}
		                    	if(logFile){
			                    	MatchConditionFile file =new MatchConditionFile();
			                    	file.type =LOG_FILE;
			                    	file.path = f.getAbsolutePath();
			                    	file.size =f.length();
			                    	sendFileInfo(file);  
		                    	}
		                    }else if (name.endsWith(".apk")) {
		                    	if(errorFile&&!isCompleteApkFile(mContext,f.getAbsolutePath())){
		                        	MatchConditionFile file =new MatchConditionFile();
		                        	file.type =ERRORAPK_FILE;
		                        	file.path = f.getAbsolutePath();
		                        	file.size =f.length();
		                        	sendFileInfo(file); 
		                    	}
		                    	if(apkFile){
		                        	MatchConditionFile file =new MatchConditionFile();
		                        	file.type =APK_FILE;
		                        	file.path = f.getAbsolutePath();
		                        	file.size =f.length();
		                        	sendFileInfo(file); 
		                    	}
			                }else if (f.length()==0){
			                	//if(emptyFile){
			                   // 	MatchConditionFile file =new MatchConditionFile();
			                    //	file.type =EMPTY_FILE;
			                    //	file.path = f.getAbsolutePath();
			                    //	file.size =f.length();
			                    //	sendFileInfo(file); 
			                	//}
			                }
		                }  
		            }  
		        }  
		    }  
		}.start();
    }
    


	public void stopScan(long scanID){
    	stopScanMatchConditionFilesIDlist.add(scanID);
    }
    
	private boolean isCompleteApkFile(Context context, String filePath) {

		boolean result = false;
		try {
			PackageManager pm = context.getPackageManager();
			PackageInfo info = pm.getPackageArchiveInfo(filePath,
					PackageManager.GET_ACTIVITIES);
			if (info != null) {
				result = true;
			}
		} catch (Exception e) {
			result = false;
		}
		return result;
	}
}
