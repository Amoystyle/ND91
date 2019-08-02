package com.nd.assistance.ui;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.nio.CharBuffer;
import java.nio.charset.Charset;
import java.nio.charset.CharsetDecoder;
import java.text.SimpleDateFormat;
import java.util.Date;


import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import com.nd.assistance.R;
import com.nd.assistance.util.ConnectHelper;
import com.nd.assistance.util.SuEx;
import com.nd.assistance.util.SuEx.SuResultLister;

import daemon.access.ProviderAccess;
import daemon.access.ProviderAccessManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.MemoryStream;
import daemon.shell.Shell;
import android.annotation.TargetApi;
import android.app.TabActivity;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.ScrollView;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TabHost;
import android.widget.TabHost.TabContentFactory;
import android.widget.TextView;
import android.widget.Toast;

@TargetApi(Build.VERSION_CODES.DONUT)
public class TestActivity extends TabActivity implements TabContentFactory {
	private final String LOG_FILE = Environment.getExternalStorageDirectory()
			.getPath() + "/nd/log.txt";
	private static final String LOG_DIR = Environment.getExternalStorageDirectory().getPath() + "/nd/log/";
	private final String NDSHLOG_FILE = Environment.getExternalStorageDirectory()
			.getPath() + "/nd/ndsh.log";
	private TextView mServerInfoTextView, mShowlog;
	private ScrollView sv_show;
	private EditText shelledt;
	private TextView shelltxt;
	private Spinner	logmodeSpinner;
	private Button startlog;
	
	private static final String[] mode={"当前为log.txt模式","当前为ndsh.log模式"};

	
	private boolean isLogtxtMode;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.test);

		createTabs();
		

		isLogtxtMode = true;

		
		mServerInfoTextView = (TextView) findViewById(R.id.serverInfo);
		Button serverBtn = (Button) findViewById(R.id.serverBtn);
		serverBtn.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				StringBuffer info = new StringBuffer();
				info.append(
						"connecting:"
								+ ConnectHelper.getInstance().isConnecting())
						.append("\n")
						.append("wifi connecting:"
								+ ConnectHelper.getInstance()
										.isWifiConnecting()).append("\n");
				mServerInfoTextView.setText(info.toString());

			}
		});
		Button createlog = (Button) findViewById(R.id.createlog);
		createlog.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				
				File file = new File(LOG_DIR);
				if(!isLogtxtMode){
					file = new File(NDSHLOG_FILE);
				}
				if (file.exists()) {
					Toast.makeText(TestActivity.this, "已存在", 2000).show();
					return;
				}
				if(isLogtxtMode){
					file.mkdirs();
				}else{
					try {
						file.createNewFile();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
				Toast.makeText(TestActivity.this, "创建成功", 2000).show();

			}
		});
		startlog = (Button) findViewById(R.id.startlog);
		startlog.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				ConnectHelper.getInstance().initLogCenter();
			}
		});
		
		Button deletelog = (Button) findViewById(R.id.deletelog);
		deletelog.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				File file = new File(LOG_DIR);
				if(!isLogtxtMode){
					file = new File(NDSHLOG_FILE);
				}
				file.delete();
				if (!file.exists()) {
					Toast.makeText(TestActivity.this, "删除成功", 2000).show();
				}
			}
		});
		Button startserver = (Button) findViewById(R.id.startserver);
		startserver.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				ConnectHelper.getInstance().setWifiStatus(true);
			}
		});

		Button stopserver = (Button) findViewById(R.id.stopserver);
		stopserver.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				ConnectHelper.getInstance().setWifiStatus(false);
			}
		});
		shelledt = (EditText) findViewById(R.id.shelledt);
		shelltxt = (TextView) findViewById(R.id.shelltxt);
		Button shellbtn = (Button) findViewById(R.id.shellbtn);
		shellbtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				ShellThread t = new ShellThread();
				t.start();
			}
		});

		final EditText ipedt = (EditText) findViewById(R.id.ipedt);
		final EditText portedt = (EditText) findViewById(R.id.portedt);

		

		Button ipbtn = (Button) findViewById(R.id.ipbtn);
		ipbtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				String ip = ipedt.getText().toString();
				String port = portedt.getText().toString();
				ConnectHelper.getInstance().startTcpConnect(ip, port,
						new Handler());
				// boolean result = pingIpAddr();
				// pingtxt.setText("ping 结果为:"+result);
			}
		});

		final TextView nettxt = (TextView) findViewById(R.id.netxt);

		Button netbtn = (Button) findViewById(R.id.netbtn);
		netbtn.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				boolean networkresult = isNetworkConnected(TestActivity.this);
				boolean wifinetworkresult = isWiFiActive(TestActivity.this);
				nettxt.setText("当前网络：" + networkresult + "\n当前wifi网络："
						+ wifinetworkresult);
			}
		});
		Button openlog = (Button) findViewById(R.id.openlog);
		openlog.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
				openLogTxt();
			}
		});
		final EditText showlogtime = (EditText) findViewById(R.id.showlogtime);
		final CheckBox check = (CheckBox) findViewById(R.id.check);
		check.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton arg0, boolean ck) {
				if (ck) {
					showlogtime.setEnabled(false);
					check.setText("停止显示log");
					showlogEnd = false;
					ShowlogThread t = new ShowlogThread(1000);
					t.start();
				} else {
					showlogEnd = true;
					showlogtime.setEnabled(true);
					check.setText("显示log");
				}

			}
		});

		mShowlog = (TextView) findViewById(R.id.showlog);
		Button clearlog = (Button) findViewById(R.id.clearlog);
		clearlog.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				mShowlog.setText("");
			}
		});
		
		sv_show = (ScrollView) findViewById(R.id.sv_show);
		Button getsubtn = (Button) findViewById(R.id.getsubtn);
		getsubtn.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View arg0) {
				SuEx.getInstance().getSuAuthorize(TestActivity.this);
			}
		});
		
		final TextView deviceTxt = (TextView) findViewById(R.id.devicetxt);
		Button devicebtn = (Button) findViewById(R.id.devicebtn);
		devicebtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				deviceTxt.setText(getDeviceInfo());				
			}
		});
		Button scanmediabtn = (Button) findViewById(R.id.scanmediabtn);
		scanmediabtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				sendBroadcast(new Intent(Intent.ACTION_MEDIA_MOUNTED, Uri.parse("file://" + Environment.getExternalStorageDirectory())));
			}
		});
		logmodeSpinner= (Spinner) findViewById(R.id.logmodeSpinner);
		ArrayAdapter<String>   adapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,mode);  
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);  
        logmodeSpinner.setAdapter(adapter);  
        logmodeSpinner.setOnItemSelectedListener(new SpinnerSelectedListener() );
		
		final CheckBox logmodecheck = (CheckBox) findViewById(R.id.logmode);
		logmodecheck.setOnCheckedChangeListener(new OnCheckedChangeListener() {

			@Override
			public void onCheckedChanged(CompoundButton arg0, boolean ck) {
				if (ck) {
					logmodecheck.setText("当前为ndsh.log模式");
					startlog.setVisibility(View.GONE);
					isLogtxtMode = false;
				} else {
					logmodecheck.setText("当前为log.txt模式");
					startlog.setVisibility(View.VISIBLE);
					isLogtxtMode = true;
				}
			}
		});
		
		Button checknetbtn = (Button)findViewById(R.id.checknetbtn);
		checknetbtn.setOnClickListener(mSendClickListener);
		final EditText businessedt = (EditText) findViewById(R.id.businessedt);
		final EditText actionedt = (EditText) findViewById(R.id.actionedt);
		Button commandbtn = (Button)findViewById(R.id.commandbtn);
		commandbtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				String result ="";
				try {
					result =simpleTest(Integer.valueOf(businessedt.getText().toString()),Integer.valueOf(actionedt.getText().toString()));
				} catch (Exception e) {
					// TODO: handle exception
				}
				shelltxt.setText(result);
			}
		});
		
		
		
		
	}
	
    class SpinnerSelectedListener implements OnItemSelectedListener{  
    	  
        public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
                long arg3) {  
			if (arg2==0) {
				isLogtxtMode = true;
				startlog.setVisibility(View.VISIBLE);
			} else if(arg2==1) {
				isLogtxtMode = false;
				startlog.setVisibility(View.GONE);
			}
        }  
  
        public void onNothingSelected(AdapterView<?> arg0) {  
        }  
    } 

	private boolean pingIpAddr(String ipAddress) {
		String mPingIpAddrResult;
		try {
			Process p = Runtime.getRuntime().exec(
					"ping -c 1 -w 100 " + ipAddress);
			int status = p.waitFor();
			if (status == 0) {
				return true;
			} else {
				mPingIpAddrResult = "Fail: IP addr not reachable";
			}
		} catch (IOException e) {
			mPingIpAddrResult = "Fail: IOException";
		} catch (InterruptedException e) {
			mPingIpAddrResult = "Fail: InterruptedException";
		}
		return false;
	}

	// 判断是否有网络连接
	public boolean isNetworkConnected(Context context) {
		if (context != null) {
			ConnectivityManager mConnectivityManager = (ConnectivityManager) context
					.getSystemService(Context.CONNECTIVITY_SERVICE);
			NetworkInfo mNetworkInfo = mConnectivityManager
					.getActiveNetworkInfo();
			if (mNetworkInfo != null) {
				return mNetworkInfo.isAvailable();
			}
		}
		return false;
	}

	private boolean isWiFiActive(Context inContext) {
		Context context = inContext.getApplicationContext();
		ConnectivityManager connectivity = (ConnectivityManager) context
				.getSystemService(Context.CONNECTIVITY_SERVICE);
		if (connectivity != null) {
			NetworkInfo[] info = connectivity.getAllNetworkInfo();
			if (info != null) {
				for (int i = 0; i < info.length; i++) {
					if (info[i].getTypeName().equals("WIFI")
							&& info[i].isConnected()) {
						return true;
					}
				}
			}
		}

		return false;
	}

	private void openLogTxt() {
		File file = new File(LOG_FILE);
		if(!isLogtxtMode){
			file = new File(NDSHLOG_FILE);
		}
		if (!file.exists()) {
			Toast.makeText(TestActivity.this, "文件不存在", 2000).show();
			return;
		}
		Intent intent = new Intent("android.intent.action.VIEW");
		intent.addCategory("android.intent.category.DEFAULT");
		intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		Uri uri2 = Uri.fromFile(file);
		intent.setDataAndType(uri2, "text/plain");
		TestActivity.this.startActivity(intent);
	}

	@Override
	protected void onDestroy() {
		SuEx.getInstance().execing = false;
		showlogEnd = true;
		super.onDestroy();
	}

	@Override
	public View createTabContent(String tag) {
		// TODO Auto-generated method stub
		return null;
	}

	@SuppressWarnings("deprecation")
	private void createTabs() {
		TabHost tabhost = getTabHost();
		tabhost.addTab(tabhost.newTabSpec("stats_tab").setIndicator("Log相关")
				.setContent(R.id.log));

		tabhost.addTab(tabhost.newTabSpec("meetings_tab").setIndicator("显示信息")
				.setContent(R.id.info));
		tabhost.addTab(tabhost.newTabSpec("meetings_tab").setIndicator("其他工具")
				.setContent(R.id.tool));
		getTabHost().setCurrentTab(0);
	}

	Handler mShellCmdHandler = new Handler() {
		public void handleMessage(Message message) {
			String info = message.getData().getString("result");
			shelltxt.setText(info);
		}
	};

	class ShellThread extends Thread {
		public void run() {
			Message message = new Message();
			Bundle b = new Bundle();
			String cmd = shelledt.getText().toString();
			String result = Shell.getInstance().execForString(cmd);
			b.putString("result", result);
			message.setData(b);
			mShellCmdHandler.sendMessage(message);
		}
	}

	
	
	Handler mHandler = new Handler() {
		public void handleMessage(Message message) {
			String info = message.getData().getString("info");
			mShowlog.setText( info);
			scroll2Bottom(sv_show, mShowlog);
		}
	};

	public void scroll2Bottom(final ScrollView scroll, final View inner) {
		Handler handler = new Handler();
		handler.post(new Runnable() {

			@Override
			public void run() {
				// TODO Auto-generated method stub
				if (scroll == null || inner == null) {
					return;
				}
				// 内层高度超过外层
				int offset = inner.getMeasuredHeight()
						- scroll.getMeasuredHeight();
				if (offset < 0) {
					offset = 0;
				}
				scroll.scrollTo(0, offset);
			}
		});
	}
	
	boolean showlogEnd = true;
	
	class ShowlogThread extends Thread {
		long mTime;
		String lastInfo="";
		
		ShowlogThread(long time){
			mTime = time;
		}
		public void run() {
			try {
				while (!showlogEnd) {
					StringBuffer sb = new StringBuffer();
					File file = new File(LOG_FILE);
					if(!isLogtxtMode){
						file = new File(NDSHLOG_FILE);
					}
					BufferedReader br = new BufferedReader(new FileReader(file));
					String line = "";
					while((line = br.readLine())!=null){
					sb.append(line);
					}
					br.close();
					String info = sb.toString();
					if(info.length()>1024*3){
						info = info.substring(info.length()-1024*3);
					}
					if(!lastInfo.equals(info)){
						lastInfo = info;
						Message message = new Message();
						Bundle b = new Bundle();
						b.putString("info", info);
						message.setData(b);
						mHandler.sendMessage(message);
					}
					Thread.sleep(mTime);
				}

			} catch (Exception e) {
				// TODO: handle exception
			}

		}
	}

	class LocatThread extends Thread {
		public void run() {

			SuEx.getInstance().exec_with_str_result_ex("logcat", 10 * 1000,
					new SuResultLister() {

						@Override
						public void OnSuResult(String result) {
							String time = getStrTime(System.currentTimeMillis()
									+ "")
									+ ":";
							result += "\n";
							result = time + result;
							Message message = new Message();
							Bundle b = new Bundle();
							b.putString("info", result);
							message.setData(b);
							mHandler.sendMessage(message);
						}
					});
		}
	}

	// 将时间戳转为字符串
	public static String getStrTime(String cc_time) {
		String re_StrTime = null;
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy年MM月dd日HH时mm分ss秒ms毫秒");
		long lcc_time = Long.valueOf(cc_time);
		re_StrTime = sdf.format(new Date(lcc_time));
		return re_StrTime;
	}
	
	//获取设备信息
	public String getDeviceInfo(){
		 StringBuffer info=new StringBuffer();
		 info.append("Build.BOARD:").append(Build.BOARD).append("\n")
		 .append("BRAND:").append(Build.BRAND).append("\n")
		 .append("CPU_ABI:").append(Build.CPU_ABI).append("\n")
		 .append("DEVICE:").append(Build.DEVICE).append("\n")
		 .append("DISPLAY:").append(Build.DISPLAY).append("\n")
		 .append("FINGERPRINT:").append(Build.FINGERPRINT).append("\n")
		 .append("HOST:").append(Build.HOST).append("\n")
		 .append("ID:").append(Build.ID).append("\n")
		 .append("MANUFACTURER:").append(Build.MANUFACTURER).append("\n")
		 .append("MODEL:").append(Build.MODEL).append("\n")
		 .append("PRODUCT:").append(Build.PRODUCT).append("\n")
		 .append("TAGS:").append(Build.TAGS).append("\n")
		 .append("TIME:").append(Build.TIME).append("\n")
		 .append("TYPE:").append(Build.TYPE).append("\n")
		 .append("USER:").append(Build.USER).append("\n")
		 .append("Build.VERSION.CODENAME:").append(Build.VERSION.CODENAME).append("\n")
		 .append("Build.VERSION.INCREMENTAL:").append(Build.VERSION.INCREMENTAL).append("\n")
		 .append("Build.VERSION.RELEASE:").append(Build.VERSION.RELEASE).append("\n")
		 .append("Build.VERSION.SDK:").append(Build.VERSION.SDK).append("\n")
		 .append("Build.VERSION.SDK_INT:").append(Build.VERSION.SDK_INT).append("\n")
		 .append("Build.VERSION_CODES.BASE:").append(Build.VERSION_CODES.BASE).append("\n")
		 .append("Build.VERSION_CODES.BASE_1_1:").append(Build.VERSION_CODES.BASE_1_1).append("\n")
		 .append("Build.VERSION_CODES.CUPCAKE:").append(Build.VERSION_CODES.CUPCAKE).append("\n")
		 .append("Build.VERSION_CODES.CUR_DEVELOPMENT:").append(Build.VERSION_CODES.CUR_DEVELOPMENT).append("\n")
		 .append("Build.VERSION_CODES.DONUT:").append(Build.VERSION_CODES.DONUT).append("\n")
		 .append("Build.VERSION_CODES.ECLAIR:").append(Build.VERSION_CODES.ECLAIR);
         return info.toString();
	}

	
    private OnClickListener mSendClickListener = new OnClickListener()
    {

        @Override
        public void onClick(View v)
        {
            HttpGet httpGet = new HttpGet("http://www.baidu.com/");
            HttpClient httpClient = new DefaultHttpClient();
            try
            {
            	HttpResponse  mHttpResponse = httpClient.execute(httpGet);
            	if(mHttpResponse.getStatusLine().getStatusCode()==200){
            		Toast.makeText(TestActivity.this, "连接网络正常", 3000).show();
            	}else{
            		Toast.makeText(TestActivity.this, "连接网络异常！！", 3000).show();
            	}

            }
            catch (Exception e)
            {
            	Toast.makeText(TestActivity.this, "连接网络异常！！", 3000).show();
            }
        }
    };
    
    
    
    private String simpleTest(int channel,int action ){
    	String result="";
		try {
			ByteWriter writer = new ByteWriter();
			writer.Write(action);
			MemoryStream ms = new MemoryStream(writer.getBaseStream().GetBuffer());			
			ms.SetLength(8);
			
			writer = new ByteWriter();
			
			
	        ByteReader reader = new ByteReader(ms);
	    	reader.getBaseStream().setOffset(0);
	    	
			ProviderAccess pa =new ProviderAccess(TestActivity.this, channel, 0);
			pa.execute(reader, writer);
			result = byteBufferToString(writer.getBaseStream().GetBuffer());
			
		} catch (Exception e) {

		}
    	return result;
    }
    
    private String byteBufferToString(ByteBuffer buffer) {
    	CharBuffer charBuffer = null;
    	try {
    	Charset charset = Charset.forName("UTF-8");
    	CharsetDecoder decoder = charset.newDecoder();
    	charBuffer = decoder.decode(buffer);
    	buffer.flip();
    	return charBuffer.toString();
    	} catch (Exception ex) {
    	ex.printStackTrace();
    	return null;
    	}
    	}

}
