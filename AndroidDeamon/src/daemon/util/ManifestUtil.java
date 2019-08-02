package daemon.util;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.json.JSONArray;
import org.json.JSONObject;



public class ManifestUtil {
	private static final String MANIFEST = "META-INF/MANIFEST.MF";
	public static final String FILELIST = "filelist.ini";
	
	public static Map<String,String> getManifestListFromApk(String apkFile){
		String maniFestString = ZipUtil.readZipTextFile(apkFile, MANIFEST);
		if(maniFestString == null)
			return null;
		Map<String,String> manifestMap = new LinkedHashMap<String, String>();
		Pattern pattern=Pattern.compile("Name: ([\\s\\S]*?)SHA1-Digest: (.*)");
		Matcher m = pattern.matcher(maniFestString); 
        while (m.find()) { 
        	String name = m.group(1); 
        	name = name.replaceAll("[\\r\\n]+ ","");
        	name = name.replaceAll("[\\r\\n]+","");
            String sha1 = m.group(2);
            manifestMap.put(name, sha1);
        }
		
		return manifestMap;
	}
	
	public static List<ManifestBean> getManifestListFromFile(String listFile){
		try{
			String maniFestString = ZipUtil.readZipTextFile(listFile, FILELIST, true);
			JSONArray jsonList = new JSONObject(maniFestString).getJSONArray("fileinfos");
			List<ManifestBean> manifestList = new ArrayList<ManifestBean>();
			for(int i=0;i<jsonList.length();i++){
				JSONObject obj = jsonList.getJSONObject(i);
				ManifestBean bean = new ManifestBean();
				
				bean.name = obj.getString("name");
				bean.checksum = obj.getString("checksum");
				bean.ignore = obj.getBoolean("ignore");
				
				manifestList.add(bean);
			}
			return manifestList;
		}catch(Exception e){
			return null;
		}
	}
	
	/**
	 * 比较
	 * @param localList
	 * @param remoteList
	 * @return
	 */
	public static List<String> compareManifest(Map<String,String> localList, List<ManifestBean> remoteList){
		List<String> list = new ArrayList<String>();
		
		Iterator<ManifestBean> it = remoteList.iterator();
		while(it.hasNext()){
			ManifestBean bean = it.next();
			if(bean.ignore){
				continue;
			}
			list.add(bean.name);
			String checkSum = localList.get(bean.name);
			if(checkSum == null || !checkSum.equals(bean.checksum)){
				return null;
			}
		}
		
		List<String> deleteList = new ArrayList<String>();
		
		Iterator<Entry<String, String>> itm = localList.entrySet().iterator();
		if(itm.hasNext()){
			Entry<String, String> entry = itm.next();
			String name = entry.getKey();
			if(!list.contains(name)){
				deleteList.add(name);
			}
		}
		
		return deleteList;
	}
	
	public static List<String> getDeleteManifest(String updatePackage){
		List<String> deleteList = new ArrayList<String>();
		try{
			String comments = ZipUtil.readZipComment(updatePackage);
			JSONObject json = new JSONObject(comments);
			JSONArray fileInfos = json.getJSONArray("fileinfos");

			for(int i = 0; i < fileInfos.length(); i++){
				JSONObject file = fileInfos.getJSONObject(i);
				String name = file.getString("name");
				deleteList.add(name);
			}
		}catch(Exception e){
			e.printStackTrace();
		}
		return deleteList;
		
	}
}
