package daemon.shell;

import java.io.InputStream;

import daemon.logger.LogCenter;

public class Shell {

	private final String TAG = "Shell";
	private final String SHELL = "sh";
	private static Shell mInstance;
	
	private String mDfInfo = null;

	public static Shell getInstance() {
		if (mInstance == null)
			mInstance = new Shell();
		return mInstance;
	}
	
	private String getStreamLines(InputStream is) {		
	
		try {
			if (is.available() > 0)
			{		
                byte[] data = new byte[is.available()];
                is.read(data);
                String result = new String(data, 0, data.length, "UTF-8");
                if(result.startsWith("\n"))
                	result = result.substring(1);
                
                if(result.endsWith("\n"))
                	result = result.substring(0, result.length() - 1);
                
                return result;
			}
	
		} catch (Exception e) {
			e.printStackTrace();
		}

		return null;
	}

	private Process run(String cmd) {
		Process p = null;
		try {
			p = Runtime.getRuntime().exec(this.SHELL);
			cmd = "exec " + cmd;
			if(!cmd.endsWith("\n"))
				cmd += "\n";
			
//			DataOutputStream dos = new DataOutputStream(p.getOutputStream());
//			dos.writeBytes("exec " + cmd + "\n");
//			dos.flush();
			
			byte[] data = cmd.getBytes("UTF-8");
			p.getOutputStream().write(data);
			p.getOutputStream().flush();
		} catch (Exception localException) {
			localException.printStackTrace();
		}

		return p;
	}

	public String execForString(String cmd) {		
		Process p = run(cmd);
		if (p != null) {
			try {
				p.waitFor();								
				
				String stdIn = getStreamLines(p.getInputStream());
				if(stdIn != null)				
				    return stdIn;
				
				String err = getStreamLines(p.getErrorStream());
				return err;
			} catch (Exception e) {
				e.printStackTrace();
			} 
		}else{
			LogCenter.error("shell", "run process failed");
		}

		return null;
	}
	
	public byte[] execForBytes(String cmd) {

		Process p = run(cmd);
		if (p != null) {
			try {
				p.waitFor();
				InputStream is = p.getInputStream();
				if(is.available() > 0){
					byte[] data = new byte[is.available()];
					is.read(data);
					return data;
				}
			}  catch (Exception e) {
				e.printStackTrace();
			} 
		}

		return null;
	}
	
	public boolean exec(String cmd){
		Process p = run(cmd);
		if (p != null) {
			try {
				p.waitFor();
			
				return true;
			}  catch (Exception e) {
				e.printStackTrace();
			} 
		}

		return false;
	}
	
	public String getDfInfo(){
		if(mDfInfo == null)
			mDfInfo = execForString("df");
		
		return mDfInfo;
	}
}