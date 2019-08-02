package daemon.provider;

import java.util.ArrayList;

import android.content.Context;

import daemon.common.Device;
import daemon.logger.LogCenter;
import daemon.provider.business.AlarmProvider;
import daemon.provider.business.BookmarkProvider;
import daemon.provider.business.CalendarProvider;
import daemon.provider.business.EmailProvider;
import daemon.provider.business.NotepadProvider;
import daemon.provider.business.PackManager;
import daemon.provider.business.PackManager_Old;
import daemon.provider.business.SmartHomeProvider;
import daemon.provider.business.ThemeManager;
import daemon.provider.business.TodoProvider;
import daemon.provider.file.FileManagerProvider;
import daemon.provider.media.MediaProvider;
import daemon.provider.media.MediaProvider_Old;
import daemon.provider.media.PhotoProvider;
import daemon.provider.media.PhotoProvider_Old;
import daemon.provider.pim.PimProvider;
import daemon.provider.system.DeamonProvider;
import daemon.provider.system.SettingProvider;
import daemon.provider.system.ShellProvider;
import daemon.provider.system.SysInfoProvider;

public class ProviderManager {

	private Context context;
	private ArrayList<IProvider> providers;
	private ProviderBroadcastReceiver receiver;

	private ProviderManager(Context context) {
		providers = new ArrayList<IProvider>();
		this.context = context;
		receiver = new ProviderBroadcastReceiver(this.context);
		receiver.register();
	}

	private static ProviderManager instance;

	public static ProviderManager getInstance(Context context) {
		if (instance == null)
			instance = new ProviderManager(context);
		return instance;
	}

	public IProvider createProvider(int business) {
		IProvider provider = getprovider(business);
		if (provider == null) {
			provider = createNewProvider(business);
			if (provider != null)
				providers.add(provider);
		}
		return provider;
	}

	private IProvider getprovider(int business) {
		IProvider provider;
		for (int i = 0; i < providers.size(); i++) {
			provider = (IProvider) providers.get(i);
			if (provider.getBusiness() == business)
				return provider;
		}
		return null;
	}

	private IProvider createNewProvider(int business) {
		// 预留:22(上传文件)， 23(下载文件)， 200（连接确认）， 201(上传文件)， 202(下载文件)，

		switch (business) {
			case 3:
				return new PhotoProvider_Old(this.context);
			case 4:
				return new TodoProvider(this.context);
			case 5:
				return new BookmarkProvider(this.context);
			case 6:
				return new CalendarProvider(this.context);
			case 7:
				return new MediaProvider_Old(this.context);
			case 9:
				return new EmailProvider(this.context);
			case 10:
				return new NotepadProvider(this.context);
			case 11:
				return new SettingProvider(this.context, Device.getIntSdkVersion());
			case 12:
				return new PackManager_Old(this.context);
			case 13:
				return new AlarmProvider(this.context);
			case 14:
				return new SysInfoProvider(this.context);
			case 15:
				return new ThemeManager(this.context);
			case 16:
				return new DeamonProvider(this.context);
			case 17:
				return new ShellProvider(this.context);
			case 20:
				return new FileManagerProvider(this.context);
			case 21:
				return new SmartHomeProvider(this.context);
			case 199:
				return new PimProvider(this.context);
				
			case 211:
				return new PhotoProvider(this.context);
			case 212:
				return new MediaProvider(this.context);
			case 213:
				return new PackManager(this.context);
		}

		return null;
	}
	

	@SuppressWarnings("unused")
	private boolean isOMS() {
		boolean result = false;

		try {
			Class<?> classType = Class.forName("oms.content.Action");
			result = true;
		}
		catch (Exception e) {
			result = false;
		}

		LogCenter.error("isoms", result + "");
		return result;
	}

	@Override
	protected void finalize() throws Throwable {
		receiver.unregister();
		super.finalize();
	}
}
