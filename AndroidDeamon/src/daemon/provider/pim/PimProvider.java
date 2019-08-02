package daemon.provider.pim;

import java.util.Locale;

import android.content.Context;
import daemon.common.Device;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.util.ContentObserverListerHelper;
import daemon.util.KeyguardHelper;

public class PimProvider implements IProvider {
	IContactProvider_V2 contactProvider;
	SmsProvider_V2 mSmsProvider;
	CallProvider_V2 mCallProvider;
	private Context mContext;
	private SimContactsProvider mSimContactProvider;

	public final String RO_BUILD_VERSION_RELEASE = "ro.build.version.release"; // 固件版本 1.5 / 1.6
	public final String APPS_SETTING_PLATFORMVERSION = "apps.setting.platformversion"; // 平台判断 android 还是 oms

	public PimProvider(Context context) {
		this.mContext = context;

		if (Device.getIntSdkVersion() >= 5) {
			contactProvider = new ContactProvider20_V2(context);
		}
		else {
			if (isOMS()) {
				contactProvider = new ContactProviderOms_V2(context);
			}
			else {
				contactProvider = new ContactProvider15_V2(context);
			}
		}

		mSmsProvider = new SmsProvider_V2(context);
		mCallProvider = new CallProvider_V2(context);
	}

	private boolean isOMS() {
		String value = Device.getSystemProperties(APPS_SETTING_PLATFORMVERSION);
		value = value.toLowerCase(Locale.ENGLISH);
		if ((value.startsWith("oms") || value.startsWith("ophone"))) {
			return true;
		}

		return false;
	}

	@Override
	public void execute(ProviderExecuteContext ctx) {
		ByteReader reader = ctx.getByteReader();
		ByteWriter writer = ctx.getByteWriter();
		int action = reader.ReadInt32();
		writer.Write(action);

		int connectionId = ctx.getAccess().getConnectionId();
       
		switch (action) {
			case 1:
				contactProvider.LoadContacts(writer);
				break;
			case 2:
				contactProvider.AddContacts(reader, writer, false);
				break;
			case 3:
				contactProvider.UpdateContact(reader, writer);
				break;
			case 4:
				contactProvider.GetPhoto(reader, writer);
				break;
			case 5:
				contactProvider.DeleteContacts(reader, writer);
				break;
			case 6:
				contactProvider.AddGroup(reader, writer);
				break;
			case 7:
				contactProvider.UpdateGroup(reader, writer);
				break;
			case 8:
				contactProvider.DeleteGroup(reader, writer);
				break;
			case 9:
				contactProvider.SetFavorite(reader, writer);
				break;
			case 10:
				contactProvider.AddToGroup(reader, writer);
				break;
			case 11:
				contactProvider.RemoveFromGroup(reader, writer);
				break;
			case 12:
				contactProvider.GetPeopleCount(writer);
				break;
			case 13:
				getSimContactsProvider().insertContacts(reader, writer);
				break;
			case 14:
				getSimContactsProvider().deleteContacts(reader, writer);
				break;
			case 15:
				getSimContactsProvider().updateContact(reader, writer);
				break;
			case 16:
				getSimContactsProvider().queryContacts(writer);
				break;
			case 17:
				contactProvider.LoadContactsWithGroupSystem(writer);
				break;
			case 18:
				contactProvider.LoadContactsEx(writer);
				break;
			case 21:
				mSmsProvider.LoadSms(writer, false);
				break;
			case 22:
				mSmsProvider.AddSms(reader, writer, false);
				break;
			case 23:
				mSmsProvider.UpdateSms(reader, writer);
				break;
			case 24:
				mSmsProvider.DeleteSms(reader, writer);
				break;
			case 25:
				mSmsProvider.DeleteSmsByThread(reader, writer);
				break;
			case 26:
				mSmsProvider.SendSms(reader, writer, connectionId);
				break;
			case 27:
				mSmsProvider.SetSmsRead(reader, writer);
				break;
			case 28:
				mSmsProvider.ReSendSms(reader, writer, connectionId);
				break;
			case 29:
				mSmsProvider.GetUnreadSms(reader, writer);
				break;
			case 30:
				mSmsProvider.GetSMSCount(writer);
				break;
			case 31:
				mSmsProvider.LoadSms(writer, true);
			case 32:
				mSmsProvider.getDefaultPackName(writer);
			case 33:
				mSmsProvider.setDefaultPack(reader,writer);
			case 41:
				mCallProvider.GetCalls(writer);
				break;
			case 42:
				mCallProvider.AddCalls(reader, writer, false);
				break;
			case 43:
				mCallProvider.DeleteCalls(reader, writer);
				break;
			case 44:
				mCallProvider.call(reader, writer);
				break;
			case 45:
				mCallProvider.answerCall(reader, writer);
				break;
			case 46:
				mCallProvider.hangupCall(reader, writer);
				break;
			case 47:
				mCallProvider.openSpeaker(reader, writer);
				break;
			case 48:
				mCallProvider.closeSpeaker(reader, writer);
				break;
			case 49:
				getAllLastTime(reader, writer);
				break;
			case 50:
				contactProvider.AddContacts(reader, writer, true);
				break;
			case 51:
				mSmsProvider.AddSms(reader, writer, true);
				break;
			case 52:
				mCallProvider.AddCalls(reader, writer, true);
		}
	}

	private void getAllLastTime(ByteReader reader, ByteWriter writer) {
		writer.Write(1);
		writer.Write(ContentObserverListerHelper.getConactLastTime());
		writer.Write(ContentObserverListerHelper.getSmsLastTime());
		writer.Write(ContentObserverListerHelper.getCallLastTime());
	}

	@Override
	public int getBusiness() {
		return 199;
	}

	private SimContactsProvider getSimContactsProvider() {
		if (mSimContactProvider == null) {
			mSimContactProvider = new SimContactsProvider(mContext);
		}
		return mSimContactProvider;
	}
}
