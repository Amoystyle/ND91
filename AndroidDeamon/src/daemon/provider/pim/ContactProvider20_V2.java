package daemon.provider.pim;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import android.annotation.TargetApi;
import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.media.RingtoneManager;
import android.net.Uri;
import android.os.Build;
import android.provider.ContactsContract.CommonDataKinds;
import android.provider.ContactsContract.Contacts;
import android.provider.ContactsContract.Data;
import android.provider.ContactsContract.Groups;
import android.provider.ContactsContract.RawContacts;
import android.provider.ContactsContract.Settings;

import daemon.model.pim20.Account;
import daemon.model.pim20.ActionState;
import daemon.model.pim20.ContactName;
import daemon.model.pim20.Email;
import daemon.model.pim20.GroupInfo;
import daemon.model.pim20.GroupMemberShip;
import daemon.model.pim20.IM;
import daemon.model.pim20.MethodBase;
import daemon.model.pim20.MethodKind;
import daemon.model.pim20.Note;
import daemon.model.pim20.OrganizationInfo;
import daemon.model.pim20.PeopleInfo;
import daemon.model.pim20.PhoneInfo;
import daemon.model.pim20.PhotoInfo;
import daemon.model.pim20.PostalAddress;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

@TargetApi(Build.VERSION_CODES.ECLAIR)
public class ContactProvider20_V2 implements IContactProvider_V2 {

	private Context mContext;

	final String[] Data_Select_Columns = new String[] {Data._ID, Data.RAW_CONTACT_ID, Data.DATA1, Data.DATA2, Data.DATA3, Data.DATA4, Data.DATA5, Data.DATA6, Data.DATA7, Data.DATA8, Data.DATA9, Data.MIMETYPE};

	final int Index_Data_ID = 0;
	final int Index_Data_ContactID = 1;
	final int Index_Data_Data1 = 2;
	final int Index_Data_Data2 = 3;
	final int Index_Data_Data3 = 4;
	final int Index_Data_Data4 = 5;
	final int Index_Data_Data5 = 6;
	final int Index_Data_Data6 = 7;
	final int Index_Data_Data7 = 8;
	final int Index_Data_Data8 = 9;
	final int Index_Data_Data9 = 10;
	final int Index_Data_MIMETYPE = 11;
	
	//0为成功，非0失败，1：插入联系人信息失败，2：获取联系人id异常，3：插入联系人异常，4：插入email信息失败，5：插入异常
	private int mErrorCode = 0;
	
	String mfacturer;

	public ContactProvider20_V2(Context context) {
		mContext = context;
		mfacturer = android.os.Build.MANUFACTURER.toUpperCase();
	}

	@Override
	public void LoadContacts(ByteWriter writer) {
		// account
		getAccounts(writer);

		// people
		List<Integer> lstRawConatctIds = getAllPeoples(writer);

		// group
		getAllGroups(writer);

		// method
		getAllMethods(writer, lstRawConatctIds);

		// end
		writer.Write(MethodKind.EndFlag.getValue());
	}

	private void getAccounts(ByteWriter writer) {
		ArrayList<Account> lstAccounts = new ArrayList<Account>();
		String[] columns = new String[] {Settings.ACCOUNT_NAME, Settings.ACCOUNT_TYPE};
		Cursor accountCur = null;

		try {
			accountCur = mContext.getContentResolver().query(Settings.CONTENT_URI, columns, null, null, null);
			if (null != accountCur && accountCur.moveToFirst()) {
				while (!accountCur.isAfterLast()) {
					Account account = new Account();

					account.setName(accountCur.getString(0));
					account.setType(accountCur.getString(1));
					if (checkAccount(lstAccounts, account)) {
						lstAccounts.add(account);
					}
					accountCur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (null != accountCur) {
				accountCur.close();
			}
		}

		for (Account account : lstAccounts) {
			account.Write(writer);
		}
	}

	private boolean checkAccount(ArrayList<Account> lstAccounts, Account account) {
		for (Account item : lstAccounts) {
			if (item.getName().equals(account.getName()) && item.getType().equals(account.getType())) {
				return false;
			}
		}
		return true;
	}

	private List<Integer> getAllPeoples(ByteWriter writer) {
		Cursor rawCur = null;
		List<Integer> lstIds = new ArrayList<Integer>();

		try {
			String[] columns = new String[] {RawContacts._ID, RawContacts.ACCOUNT_NAME, RawContacts.ACCOUNT_TYPE, RawContacts.CUSTOM_RINGTONE, RawContacts.STARRED, RawContacts.CONTACT_ID};
			String where = RawContacts.DELETED + "=0";

			rawCur = mContext.getContentResolver().query(RawContacts.CONTENT_URI, columns, where, null, null);

			if (null != rawCur && rawCur.moveToFirst()) {
				int kind = MethodKind.People.getValue();

				while (!rawCur.isAfterLast()) {
					writer.Write(kind);
					int id = rawCur.getInt(0);
					lstIds.add(id);

					writer.Write(id);
					String accountName = rawCur.getString(1);
					writer.Write(accountName);
					String accountType = rawCur.getString(2);
					writer.Write(accountType);

					// writer.WriteBlob(rawCur.getBlob(1)); 账号数据库类型为STRING如果是纯数字的话getBlog将会出错
					// writer.WriteBlob(rawCur.getBlob(2));
					writer.WriteBlob(rawCur.getBlob(3));
					boolean starred = rawCur.getInt(4) == 1;
					writer.Write(starred);

					rawCur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			String msg = "";
			msg += e.getMessage() + "\r\n\r\n";
			msg += e.getCause() + "\r\n\r\n";
			msg += e.getStackTrace() + "\r\n\r\n";
			writer.Write(msg);
		}
		finally {
			if (null != rawCur) {
				rawCur.close();
			}
		}

		return lstIds;
	}

	private void getAllGroups(ByteWriter writer) {
		String[] columns = new String[] {Groups._ID, Groups.TITLE, Groups.NOTES, Groups.ACCOUNT_NAME, Groups.ACCOUNT_TYPE};
		Cursor groupCur = null;

		try {

			groupCur = mContext.getContentResolver().query(Groups.CONTENT_URI, columns, "deleted=0", null, null);
			if (null != groupCur && groupCur.moveToFirst()) {
				while (!groupCur.isAfterLast()) {
					writer.Write(MethodKind.Group.getValue());
					
					writer.Write(groupCur.getInt(0)); // id
					writer.WriteBlob(groupCur.getBlob(1)); // title
					writer.WriteBlob(groupCur.getBlob(2)); // notes
					// writer.WriteBlob(groupCur.getBlob(3)); // account-name 账号数据库类型为STRING如果是纯数字的话getBlog将会出错
					// writer.WriteBlob(groupCur.getBlob(4)); // account-type
					String accountName = groupCur.getString(3);
					writer.Write(accountName);
					String accountType = groupCur.getString(4);
					writer.Write(accountType);

					groupCur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (null != groupCur) {
				groupCur.close();
			}
		}
	}

	private void getAllGroupsWithSystem(ByteWriter writer) {
		String[] columns = new String[] {Groups._ID, Groups.TITLE, Groups.NOTES, Groups.ACCOUNT_NAME, Groups.ACCOUNT_TYPE, Groups.SYSTEM_ID};
		Cursor groupCur = null;

		try {

			groupCur = mContext.getContentResolver().query(Groups.CONTENT_URI, columns, "deleted=0", null, null);
			if (null != groupCur && groupCur.moveToFirst()) {
				while (!groupCur.isAfterLast()) {
					writer.Write(MethodKind.Group.getValue());
					writer.Write(groupCur.getInt(0)); // id
					writer.WriteBlob(groupCur.getBlob(1)); // title
					writer.WriteBlob(groupCur.getBlob(2)); // notes
					// writer.WriteBlob(groupCur.getBlob(3)); // account-name 账号数据库类型为STRING如果是纯数字的话getBlog将会出错
					// writer.WriteBlob(groupCur.getBlob(4)); // account-type
					String accountName = groupCur.getString(3);
					writer.Write(accountName);
					String accountType = groupCur.getString(4);
					writer.Write(accountType);
					String systemID = groupCur.getString(5);
					if (systemID == null || systemID.trim().equals(""))
						writer.Write(false);
					else
						writer.Write(true);
					groupCur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (null != groupCur) {
				groupCur.close();
			}
		}
	}

	private void getAllMethods(ByteWriter writer, List<Integer> lstRawContactIds) {
		// String where = Data.RAW_CONTACT_ID + "=" + rawConatctId;
		Cursor methodCur = null;
		try {
			methodCur = mContext.getContentResolver().query(Data.CONTENT_URI, Data_Select_Columns, null, null, null);
			if (methodCur != null) {
				int methodsPerContact = methodCur.getCount() / lstRawContactIds.size();
				if (methodsPerContact < 10) {
					getMethods(writer, methodCur);
					methodCur.close();
				}
				else {
					methodCur.close();

					for (int rawConatctId : lstRawContactIds) {
						String where = Data.RAW_CONTACT_ID + "=" + rawConatctId;
						methodCur = mContext.getContentResolver().query(Data.CONTENT_URI, Data_Select_Columns, where, null, null);
						getMethods(writer, methodCur);
						methodCur.close();
					}
				}

			}
		}
		catch (Exception e) {

		}
	}

	private void getMethods(ByteWriter writer, Cursor methodCur) {
		try {
			if (null != methodCur && methodCur.moveToFirst()) {
				while (!methodCur.isAfterLast()) {
					int id = methodCur.getInt(Index_Data_ID);
					int contactId = methodCur.getInt(Index_Data_ContactID);
					MethodKind kind = MethodKind.Phone;

					String mimetype = methodCur.getString(Index_Data_MIMETYPE);

					if (mimetype.equals(CommonDataKinds.Phone.CONTENT_ITEM_TYPE)) {
						kind = MethodKind.Phone;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data2)); // type
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data1)); // number
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data3)); // label
					}
					else if (mimetype.equals(CommonDataKinds.Email.CONTENT_ITEM_TYPE)) {
						kind = MethodKind.Email;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data2)); // type
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data3)); // label
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data1)); // email
						// address
					}
					else if (mimetype.equals(CommonDataKinds.Im.CONTENT_ITEM_TYPE)) {
						kind = MethodKind.IM;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data5)); // type
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data1)); // im
						// address
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data6)); // label
					}
					else if (mimetype.equals(CommonDataKinds.StructuredPostal.CONTENT_ITEM_TYPE)) {

						kind = MethodKind.PostalAddress;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data2)); // type
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data3)); // label
						writer.Write(""); // display
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data4)); // street
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data7)); // city
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data8)); // state
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data9)); // zip
						// code

					}
					else if (mimetype.equals(CommonDataKinds.Organization.CONTENT_ITEM_TYPE)) {
						kind = MethodKind.Organization;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data2)); // type
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data1)); // company
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data4)); // position
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data3)); // label
					}
					else if (mimetype.equals(CommonDataKinds.GroupMembership.CONTENT_ITEM_TYPE)) {

						kind = MethodKind.GroupMembership;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.Write(methodCur.getInt(Index_Data_Data1)); // groupId
					}
					else if (mimetype.equals(CommonDataKinds.Note.CONTENT_ITEM_TYPE)) {

						kind = MethodKind.Note;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.Write(methodCur.getString(Index_Data_Data1)); // note
						// data
					}
					else if (mimetype.equals(CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE)) {

						kind = MethodKind.Name;
						writer.Write(kind.getValue());
						writer.Write(id);
						writer.Write(contactId);
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data1)); // displayname
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data2)); // givenName
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data3)); // familyName
						writer.WriteBlob(methodCur.getBlob(Index_Data_Data5)); // middleName
					}
					else if (mimetype.equals(CommonDataKinds.Photo.CONTENT_ITEM_TYPE)) {

						// kind = MethodKind.Photo;
						// writer.Write(kind.getValue());
						// writer.Write(id);
						// writer.Write(contactId);
						// writer.Write(0);
					}

					methodCur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	@Override
	public void AddContacts(ByteReader reader, ByteWriter writer , boolean bplugin) {
		boolean result = true;
		ArrayList<Integer> insertedIds = new ArrayList<Integer>();
		ArrayList<PeopleInfo> addPeoples = new ArrayList<PeopleInfo>();
		ArrayList<MethodBase> addMethods = new ArrayList<MethodBase>();
		HashMap<PeopleInfo, ArrayList<MethodBase>> peopleMethodsMap = new HashMap<PeopleInfo, ArrayList<MethodBase>>();

		int cnt = reader.ReadInt32();

		for (int i = 0; i < cnt; i++) {
			// people
			PeopleInfo people = new PeopleInfo();
			people.Read(reader);
			addPeoples.add(people);

			// methods
			ArrayList<MethodBase> methods = getMethodsFromStream(reader);
			peopleMethodsMap.put(people, methods);
			addMethods.addAll(methods);
		}

		result = result && AddPeopleInfo(addPeoples);

		if (result == true) {
			for (PeopleInfo people : addPeoples) {
				insertedIds.add(people.getId());

				ArrayList<MethodBase> methods = peopleMethodsMap.get(people);
				for (MethodBase method : methods) {
					method.setContactId(people.getId());
				}
			}
		}

		result = result && MethodsDeal(addMethods, insertedIds);

		writer.Write(result);

		if (result) {
			for (int id : insertedIds) {
				writer.Write(id);
			}

			writer.Write(MethodKind.EndFlag.getValue());
		}
		else if(bplugin)
		{
			//加上错误原因
			writer.Write(mErrorCode);
		}
	}

	private boolean AddPeopleInfo(ArrayList<PeopleInfo> lstPeople) {
		boolean result = false;
		int peopleCnt = lstPeople.size();
		ContentValues[] lstValues = new ContentValues[peopleCnt];
		
		try {
			for (int i = 0; i < peopleCnt; i++) {
				PeopleInfo people = lstPeople.get(i);
				ContentValues values = new ContentValues();

				values.put(RawContacts.STARRED, people.getStarred() ? 1 : 0);

				String customRingtone = people.getCustomRingtone();

				if (customRingtone.equals("")) {
					Uri defaultRingUri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
					customRingtone = defaultRingUri.toString();
				}
				values.put(RawContacts.CUSTOM_RINGTONE, customRingtone);
				
				if (!mfacturer.equals("MEIZU")) { //对魅族做过滤，其实大部分手机也是不用带这个东西的 
					String accountName = people.getAccountName();
					if (accountName != null && !accountName.equals(""))
						values.put(RawContacts.ACCOUNT_NAME,people.getAccountName());
					String accountType = people.getAccountType();
					if (accountType != null && !accountType.equals(""))
						values.put(RawContacts.ACCOUNT_TYPE,people.getAccountType());
				}
              
				values.put(RawContacts.AGGREGATION_MODE, RawContacts.AGGREGATION_MODE_DISABLED);

				lstValues[i] = values;
			}

			int insertedCnt = this.mContext.getContentResolver().bulkInsert(RawContacts.CONTENT_URI, lstValues);

			if (peopleCnt == insertedCnt) {
				getAddedContactId(lstPeople);
			}
			else{
				mErrorCode = 1; //1代表插入联系人的时候失败
			}

			result = true;

		}
		catch (Exception e) {
			e.printStackTrace();
			mErrorCode = 3; //插入联系人信息异常
		}

		return result;
	}

	private void getAddedContactId(ArrayList<PeopleInfo> lstPeople) {
		int cnt = lstPeople.size();
		Cursor cur = null;
		String[] columns = new String[] {RawContacts._ID};
		String order = RawContacts._ID + " DESC";

		try {
			cur = mContext.getContentResolver().query(RawContacts.CONTENT_URI, columns, null, null, order);
			if (cur != null && cur.moveToFirst()) {
				while (!cur.isAfterLast() && cnt > 0) {
					int id = cur.getInt(0);

					cnt--;
					lstPeople.get(cnt).setId(id);

					cur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
			mErrorCode = 2; //2代表获取联系人id失败
		}
		finally {
			if (cur != null)
				cur.close();
		}
	}

	private boolean MethodsDeal(ArrayList<MethodBase> lstMethods, ArrayList<Integer> insertedIds) {

		boolean result = true;
		ArrayList<MethodBase> lstAddMethods = new ArrayList<MethodBase>();
		ArrayList<ContentValues> lstAddValues = new ArrayList<ContentValues>();

		for (MethodBase method : lstMethods) {
			if (!result)
				return false;

			MethodKind kind = method.getKind();
			ContentValues values = new ContentValues();

			if (kind == MethodKind.Email) {
				Email email = (Email) method;

				values.put(Data.MIMETYPE, CommonDataKinds.Email.CONTENT_ITEM_TYPE);
				values.put(Data.DATA1, email.getEmailAddress());
				values.put(Data.DATA2, email.getType());
				values.put(Data.DATA3, email.getType().equals("0") ? email.getLabel() : null);
			}
			else if (kind == MethodKind.IM) {
				IM im = (IM) method;

				values.put(Data.MIMETYPE, CommonDataKinds.Im.CONTENT_ITEM_TYPE);
				values.put(Data.DATA1, im.getImAddress());
				values.put(Data.DATA2, "3");// 鏁版嵁搴撲腑im鐨勭被鍨嬮兘鏄�1ￄ1�73"
				values.put(Data.DATA5, im.getType());
				values.put(Data.DATA6, im.getType().equals("-1") ? im.getLabel() : null);
			}
			else if (kind == MethodKind.PostalAddress) {
				PostalAddress addr = (PostalAddress) method;

				values.put(Data.MIMETYPE, CommonDataKinds.StructuredPostal.CONTENT_ITEM_TYPE);
				values.put(Data.DATA1, addr.getDisplayAddress());
				values.put(Data.DATA4, addr.getStreet());
				values.put(Data.DATA7, addr.getCity());
				values.put(Data.DATA8, addr.getState());
				values.put(Data.DATA9, addr.getZipCode());
				values.put(Data.DATA2, addr.getType());
				values.put(Data.DATA3, addr.getType().equals("0") ? addr.getLabel() : null);
			}
			else if (kind == MethodKind.Phone) {
				PhoneInfo phone = (PhoneInfo) method;

				values.put(Data.MIMETYPE, CommonDataKinds.Phone.CONTENT_ITEM_TYPE);
				values.put(Data.DATA2, phone.getType());
				values.put(Data.DATA1, phone.getNumber());
				values.put(Data.DATA3, phone.getType().equals("0") ? phone.getLabel() : null);
			}
			else if (kind == MethodKind.Name) {
				ContactName name = (ContactName) method;

				values.put(Data.MIMETYPE, CommonDataKinds.StructuredName.CONTENT_ITEM_TYPE);
				values.put(Data.DATA1, name.getDisplayName());
				values.put(Data.DATA2, name.getGivenName());
				values.put(Data.DATA3, name.getFamilyName());
				values.put(Data.DATA5, name.getMiddleName());
			}
			else if (kind == MethodKind.Organization) {
				OrganizationInfo org = (OrganizationInfo) method;

				values.put(Data.MIMETYPE, CommonDataKinds.Organization.CONTENT_ITEM_TYPE);
				values.put(Data.DATA1, org.getCompany());
				values.put(Data.DATA2, org.getType());
				values.put(Data.DATA4, org.getPosition());
				values.put(Data.DATA3, org.getType().equals("0") ? org.getLabel() : null);
			}
			else if (kind == MethodKind.Note) {
				Note note = (Note) method;

				values.put(Data.MIMETYPE, CommonDataKinds.Note.CONTENT_ITEM_TYPE);
				values.put(Data.DATA1, note.getData());

				method = note;
			}
			else if (kind == MethodKind.Photo) {
				PhotoInfo photo = (PhotoInfo) method;
				values.put(Data.MIMETYPE, CommonDataKinds.Photo.CONTENT_ITEM_TYPE);
				values.put(Data.DATA15, photo.getData());
				method = photo;
			}
			else if (kind == MethodKind.GroupMembership) {
				GroupMemberShip ms = (GroupMemberShip) method;
				if (method.getActionState() == ActionState.Added) {
					values.put(Data.MIMETYPE, CommonDataKinds.GroupMembership.CONTENT_ITEM_TYPE);
					values.put(Data.DATA1, ms.getGroupId());
				}else if(method.getActionState() == ActionState.Deleted){
					RemoveFromGroup(ms.getGroupId(), ms.getContactId());
				}
			}

			if (method != null) {
				if (method.getActionState() == ActionState.Added) {
					values.put(Data.RAW_CONTACT_ID, method.getContactId());
					lstAddMethods.add(method);
					lstAddValues.add(values);
				}
				else if (method.getActionState() == ActionState.Edited) {
					result &= UpdateMethodInfo(values, method.getId());
				}
				else if (method.getActionState() == ActionState.Deleted) {
					result &= DeleteMethodInfo(method.getId());
				}
			}

		}

		if (result == true && lstAddMethods.size() > 0) {
			result &= AddMethodInfos(lstAddMethods, lstAddValues);

			if (result == true) {
				for (MethodBase method : lstMethods) {
					if (method.getActionState() == ActionState.Added)
						insertedIds.add(method.getId());
				}
			}
			
		}

		return result;
	}

	private boolean UpdateMethodInfo(ContentValues values, Integer id) {
		boolean result = false;

		try {
			Uri uri = ContentUris.withAppendedId(Data.CONTENT_URI, id);

			this.mContext.getContentResolver().update(uri, values, null, null);

			result = true;
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	private boolean AddMethodInfos(List<MethodBase> lstMethods, ArrayList<ContentValues> lstValues) {
		boolean result = false;

		try {
			ContentValues[] valuesArray = new ContentValues[lstValues.size()];
			for (int i = 0; i < lstValues.size(); i++) {
				valuesArray[i] = lstValues.get(i);
			}

			int cnt = this.mContext.getContentResolver().bulkInsert(Data.CONTENT_URI, valuesArray);

			if (cnt == lstMethods.size()) {
				ArrayList<Integer> ids = getLatestDataIds(cnt);
				if (ids.size() == cnt) {
					for (int i = 0; i < cnt; i++) {
						lstMethods.get(i).setId(ids.get(i));
					}

					result = true;
				}
			}
			else{
				mErrorCode = 4; //插入诅如mail，IM信息失败
			}

		}
		catch (Exception e) {
			e.printStackTrace();
			mErrorCode = 5; //插入异常
		}

		return result;
	}

	private ArrayList<Integer> getLatestDataIds(int cnt) {
		ArrayList<Integer> ids = new ArrayList<Integer>();

		Cursor cur = null;
		try {
			cur = mContext.getContentResolver().query(Data.CONTENT_URI, new String[] {Data._ID}, null, null, Data._ID + " DESC");
			if (cur != null && cur.moveToFirst()) {
				while (!cur.isAfterLast() && cnt > 0) {
					int id = cur.getInt(0);
					ids.add(0, id);
					cnt--;
					cur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (cur != null) {
				cur.close();
			}
		}

		return ids;
	}

	private boolean DeleteMethodInfo(Integer id) {
		boolean result = false;

		try {
			Uri uri = ContentUris.withAppendedId(Data.CONTENT_URI, id);

			this.mContext.getContentResolver().delete(uri, null, null);

			result = true;
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	private ArrayList<MethodBase> getMethodsFromStream(ByteReader reader) {
		ArrayList<MethodBase> lstMethods = new ArrayList<MethodBase>();

		MethodKind kind = MethodKind.FromInt(reader.ReadInt32());
		while (kind != MethodKind.EndFlag) {
			if (kind == MethodKind.Email) {
				Email email = new Email();
				email.Read(reader);
				lstMethods.add(email);
			}
			else if (kind == MethodKind.IM) {
				IM im = new IM();
				im.Read(reader);
				lstMethods.add(im);
			}
			else if (kind == MethodKind.PostalAddress) {
				PostalAddress addr = new PostalAddress();
				addr.Read(reader);
				lstMethods.add(addr);
			}
			else if (kind == MethodKind.Phone) {
				PhoneInfo phone = new PhoneInfo();
				phone.Read(reader);
				lstMethods.add(phone);
			}
			else if (kind == MethodKind.Name) {
				ContactName name = new ContactName();
				name.Read(reader);
				lstMethods.add(name);
			}
			else if (kind == MethodKind.Organization) {
				OrganizationInfo org = new OrganizationInfo();
				org.Read(reader);
				lstMethods.add(org);
			}
			else if (kind == MethodKind.Note) {
				Note note = new Note();
				note.Read(reader);
				lstMethods.add(note);
			}
			else if (kind == MethodKind.Photo) {
				PhotoInfo photo = new PhotoInfo();
				photo.Read(reader);
				lstMethods.add(photo);
			}
			else if (kind == MethodKind.GroupMembership) {
				GroupMemberShip ms = new GroupMemberShip();
				ms.Read(reader);
				lstMethods.add(ms);
			}
			kind = MethodKind.FromInt(reader.ReadInt32());
		}

		return lstMethods;
	}

	@Override
	public void UpdateContact(ByteReader reader, ByteWriter writer) {
		boolean result = true;
		ArrayList<Integer> insertedIds = new ArrayList<Integer>();
		ArrayList<MethodBase> methods = new ArrayList<MethodBase>();

		// people
		PeopleInfo people = new PeopleInfo();
		people.Read(reader);
		result = result && UpdatePeopleInfo(people);

		// methods
		methods = getMethodsFromStream(reader);
		for (MethodBase method : methods) {
			method.setContactId(people.getId());
		}

		result = result && MethodsDeal(methods, insertedIds);

		writer.Write(result);

		if (result) {
			for (int id : insertedIds) {
				writer.Write(id);
			}

			writer.Write(MethodKind.EndFlag.getValue());
		}
	}

	private boolean UpdatePeopleInfo(PeopleInfo people) {
		boolean result = false;

		try {
			ContentValues values = new ContentValues();

			values.put(RawContacts.STARRED, people.getStarred() ? 1 : 0);

			String customRingtone = people.getCustomRingtone();
			if (customRingtone.equals("")) {
				Uri defaultRingUri = RingtoneManager.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
				customRingtone = defaultRingUri.toString();
			}

			values.put(RawContacts.CUSTOM_RINGTONE, customRingtone);

			Uri uri = ContentUris.withAppendedId(RawContacts.CONTENT_URI, people.getId());

			if (this.mContext.getContentResolver().update(uri, values, null, null) != 0) {
				result = true;
			}

			int contactId = getContactId(people.getId());
			if (contactId > 0) {
				uri = ContentUris.withAppendedId(Contacts.CONTENT_URI, contactId);
				mContext.getContentResolver().update(uri, values, null, null);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	private int getContactId(int rawContactId) {
		int contactId = -1;

		String where = RawContacts._ID + "=" + rawContactId;
		String[] columns = new String[] {RawContacts.CONTACT_ID};

		Cursor rawCur = null;

		try {
			rawCur = mContext.getContentResolver().query(RawContacts.CONTENT_URI, columns, where, null, null);

			if (rawCur != null && rawCur.moveToFirst()) {
				contactId = rawCur.getInt(0);
			}
		}
		catch (Exception ex) {
		}
		finally {
			if (rawCur != null) {
				rawCur.close();
			}
		}

		return contactId;
	}

	@Override
	public void GetPhoto(ByteReader reader, ByteWriter writer) {
		Integer personId = reader.ReadInt32();

		Cursor cur = null;
		String[] columns = new String[] {Data._ID, Data.DATA15};
		String where = Data.RAW_CONTACT_ID + "=" + personId + " and " + Data.MIMETYPE + " = '" + CommonDataKinds.Photo.CONTENT_ITEM_TYPE + "'";

		writer.Write(MethodKind.Photo.getValue());

		try {
			cur = mContext.getContentResolver().query(Data.CONTENT_URI, columns, where, null, null);
			if (cur != null && cur.moveToFirst()) {
				int id = cur.getInt(0);
				writer.Write(id);
				writer.Write(personId);

				byte[] data = cur.getBlob(1);
				if (data != null && data.length > 0) {
					writer.Write(data.length);
					writer.Write(data);
				}
				else {
					writer.Write(0);
				}
			}
			else {
				writer.Write(0);
				writer.Write(personId);
				writer.Write(0);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (cur != null)
				cur.close();
		}
	}

	@Override
	public void DeleteContacts(ByteReader reader, ByteWriter writer) {
		int cnt = reader.ReadInt32();
		boolean result = false;

		if (cnt > 0) {
			String where = RawContacts._ID + " in (";

			for (int i = 0; i < cnt; i++) {
				where += reader.ReadInt32() + ",";
			}
			where = where.substring(0, where.length() - 1) + ")";

			try {
				this.mContext.getContentResolver().delete(RawContacts.CONTENT_URI, where, null);
				result = true;
			}
			catch (Exception e) {
				e.printStackTrace();
				result = false;
			}
		}

		writer.Write(result);
	}

	@Override
	public void AddGroup(ByteReader reader, ByteWriter writer) {
		boolean result = false;
		GroupInfo group = new GroupInfo();

		try {
			group.Read(reader);

			ContentValues values = new ContentValues();

			if (!mfacturer.equals("MEIZU")) {
				values.put(Groups.ACCOUNT_NAME, group.getAccount());
				values.put(Groups.ACCOUNT_TYPE, group.getAccountType());
			}
			values.put(Groups.TITLE, group.getName());
			values.put(Groups.NOTES, group.getNotes());
			values.put(Groups.GROUP_VISIBLE, 1);

			Uri inserted = this.mContext.getContentResolver().insert(Groups.CONTENT_URI, values);
			Integer id = Integer.parseInt(inserted.getLastPathSegment());
			group.setId(id);

			result = true;

		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}

		writer.Write(result);

		if (result) {
			writer.Write(group.getId());
		}
	}

	@Override
	public void UpdateGroup(ByteReader reader, ByteWriter writer) {
		boolean result = false;

		try {
			GroupInfo group = new GroupInfo();
			group.Read(reader);

			ContentValues values = new ContentValues();

			values.put(Groups.TITLE, group.getName());
			values.put(Groups.NOTES, group.getNotes());

			Uri uri = ContentUris.withAppendedId(Groups.CONTENT_URI, group.getId());
			if (this.mContext.getContentResolver().update(uri, values, null, null) != 0) {
				result = true;
			}

		}
		catch (Exception e) {
			e.printStackTrace();
			result = false;
		}

		writer.Write(result);
	}

	@Override
	public void DeleteGroup(ByteReader reader, ByteWriter writer) {
		boolean result = false;
		Cursor membershipCur = null;

		try {
			int groupId = reader.ReadInt32();

			String where = Data.MIMETYPE + " = '" + CommonDataKinds.GroupMembership.CONTENT_ITEM_TYPE + "' and " + Data.DATA1 + " = '" + groupId + "'";
			membershipCur = this.mContext.getContentResolver().query(Data.CONTENT_URI, new String[] {Data._ID}, where, null, null);
			if (null != membershipCur && membershipCur.moveToFirst()) {
				while (!membershipCur.isAfterLast()) {
					int membershipId = membershipCur.getInt(membershipCur.getColumnIndexOrThrow(Data._ID));

					Uri membershipUri = ContentUris.withAppendedId(Data.CONTENT_URI, membershipId);
					this.mContext.getContentResolver().delete(membershipUri, null, null);

					membershipCur.moveToNext();
				}
			}

			Uri uri = ContentUris.withAppendedId(Groups.CONTENT_URI, groupId);
			mContext.getContentResolver().delete(uri, null, null);

			result = true;
		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}
		finally {
			if (null != membershipCur)
				membershipCur.close();
		}

		writer.Write(result);
	}

	@Override
	public void AddToGroup(ByteReader reader, ByteWriter writer) {
		boolean result = false;

		try {
			int newGroupId = reader.ReadInt32();
			int peopleCnt = reader.ReadInt32();
			int[] peopleIds = new int[peopleCnt];

			for (int i = 0; i < peopleCnt; i++) {
				peopleIds[i] = reader.ReadInt32();
			}

			ContentValues[] lstValues = new ContentValues[peopleIds.length];

			for (int i = 0; i < peopleIds.length; i++) {

				ContentValues values = new ContentValues();
				values.put(Data.MIMETYPE, CommonDataKinds.GroupMembership.CONTENT_ITEM_TYPE);
				values.put(Data.RAW_CONTACT_ID, peopleIds[i]);
				values.put(Data.DATA1, newGroupId);

				lstValues[i] = values;
			}

			this.mContext.getContentResolver().bulkInsert(Data.CONTENT_URI, lstValues);

			result = true;

		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}

		writer.Write(result);
	}

	@Override
	public void RemoveFromGroup(ByteReader reader, ByteWriter writer) {
		int oldGroupId = reader.ReadInt32();
		int peopleCnt = reader.ReadInt32();
		List<Integer> contactIds = new ArrayList<Integer>();
		for (int i = 0; i < peopleCnt; i++) {
			int contactId = reader.ReadInt32();
			contactIds.add(contactId);
		}

		boolean result = RemoveFromGroup(oldGroupId, contactIds);
		writer.Write(result);
	}

	private boolean RemoveFromGroup(int groupId, int contactId) {
		List<Integer> contactIds = new ArrayList<Integer>();
		contactIds.add(contactId);
		return RemoveFromGroup(groupId, contactIds);
	}

	private boolean RemoveFromGroup(int groupId, List<Integer> contactIds) {
		boolean result = false;
		Cursor delCur = null;

		try {
			String where = Data.MIMETYPE + " = '" + CommonDataKinds.GroupMembership.CONTENT_ITEM_TYPE + "'";

			where += " and " + Data.DATA1 + "=" + groupId + " and " + Data.RAW_CONTACT_ID + " in(";

			for (int contactId : contactIds) {
				where += contactId + ",";
			}

			where = where.substring(0, where.length() - 1) + ")";

			mContext.getContentResolver().delete(Data.CONTENT_URI, where, null);

			result = true;

		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}
		finally {
			if (null != delCur) {
				delCur.close();
			}
		}

		return result;
	}

	@Override
	public void SetFavorite(ByteReader reader, ByteWriter writer) {
		boolean starred = reader.ReadBoolean();
		int cnt = reader.ReadInt32();
		int[] peopleIds = new int[cnt];

		for (int i = 0; i < cnt; i++) {
			peopleIds[i] = reader.ReadInt32();
		}

		boolean result = false;

		try {
			for (int i = 0; i < peopleIds.length; i++) {
				ContentValues values = new ContentValues();
				values.put(Contacts.STARRED, starred ? 1 : 0);
				Uri uri = ContentUris.withAppendedId(RawContacts.CONTENT_URI, peopleIds[i]);
				this.mContext.getContentResolver().update(uri, values, null, null);
			}

			result = true;

		}
		catch (Exception e) {
			result = false;
			e.printStackTrace();
		}

		writer.Write(result);
	}

	public void GetPeopleCount(ByteWriter writer) {
		Cursor peopleCur = null;

		try {
			String[] columns = new String[] {RawContacts._ID};
			String where = RawContacts.DELETED + "=0";
			peopleCur = mContext.getContentResolver().query(RawContacts.CONTENT_URI, columns, where, null, null);

			if (null != peopleCur) {
				int cnt = peopleCur.getCount();
				writer.Write(cnt);
			}
			else {
				writer.Write(0);
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (peopleCur != null) {
				peopleCur.close();
			}
		}
	}

	@Override
	public void LoadContactsWithGroupSystem(ByteWriter writer) {
		// account
		getAccounts(writer);

		// people
		List<Integer> lstRawContactIds = getAllPeoples(writer);

		// group
		getAllGroupsWithSystem(writer);

		// method
		getAllMethods(writer, lstRawContactIds);

		// end
		writer.Write(MethodKind.EndFlag.getValue());
	}

	@Override
	public void LoadContactsEx(ByteWriter writer) {
		// account
		getAccounts(writer);

		// people
		List<Integer> lstRawConatctIds = getAllPeoples(writer);

		// group
		getAllGroupsEx(writer);

		// method
		getAllMethods(writer, lstRawConatctIds);

		// end
		writer.Write(MethodKind.EndFlag.getValue());
		
	}

	private void getAllGroupsEx(ByteWriter writer) {
		String[] columns = new String[] {Groups._ID, Groups.TITLE, Groups.NOTES, Groups.ACCOUNT_NAME, Groups.ACCOUNT_TYPE,Groups.SYSTEM_ID};
		Cursor groupCur = null;

		try {

			groupCur = mContext.getContentResolver().query(Groups.CONTENT_URI, columns, "deleted=0", null, null);
			if (null != groupCur && groupCur.moveToFirst()) {
				while (!groupCur.isAfterLast()) {
					writer.Write(MethodKind.Group.getValue());
					writer.Write(groupCur.getInt(0)); // id
					writer.WriteBlob(groupCur.getBlob(1)); // title
					writer.WriteBlob(groupCur.getBlob(2)); // notes
					// writer.WriteBlob(groupCur.getBlob(3)); // account-name 账号数据库类型为STRING如果是纯数字的话getBlog将会出错
					// writer.WriteBlob(groupCur.getBlob(4)); // account-type
					String accountName = groupCur.getString(3);
					writer.Write(accountName);
					String accountType = groupCur.getString(4);
					writer.Write(accountType);
					String systemId = groupCur.getString(5);
					boolean isSystemGroup = false;
					if(systemId!=null&&!systemId.equals("")){
						isSystemGroup = true;
					}
					writer.Write(isSystemGroup);
					groupCur.moveToNext();
				}
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		finally {
			if (null != groupCur) {
				groupCur.close();
			}
		}
		
	}
}
