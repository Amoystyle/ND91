package daemon.provider.pim;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.media.RingtoneManager;
import android.net.Uri;
import android.provider.Contacts;
import android.provider.Contacts.ContactMethods;
import android.provider.Contacts.GroupMembership;
import android.provider.Contacts.Groups;
import android.provider.Contacts.Organizations;
import android.provider.Contacts.People;
import android.provider.Contacts.Phones;


import daemon.model.pim15.ActionState;
import daemon.model.pim15.Email;
import daemon.model.pim15.GroupInfo;
import daemon.model.pim15.GroupMemberShip;
import daemon.model.pim15.IM;
import daemon.model.pim15.MethodBase;
import daemon.model.pim15.MethodKind;
import daemon.model.pim15.OrganizationInfo;
import daemon.model.pim15.PeopleInfo;
import daemon.model.pim15.PhoneInfo;
import daemon.model.pim15.PhotoInfo;
import daemon.model.pim15.PostalAddress;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public class ContactProvider15_V2 implements IContactProvider_V2 {

	private Context mContext;
	//0为成功，非0失败，1：插入联系人信息失败，2：获取联系人id异常，3：插入联系人异常，4：插入email信息失败，5：插入异常
	private int mErrorCode = 0;
	
	public ContactProvider15_V2(Context context) {
		mContext = context;
	}

	public void LoadContacts(ByteWriter writer) {
		// people
		getAllPeoples(writer);

		// phone
		getAllPhones(writer);

		// org
		getAllOrgInfos(writer);

		// group membership
		getAllMemberShip(writer);

		// group
		getAllGroups(writer);

		// method
		getAllMethods(writer);

		// end
		writer.Write(MethodKind.EndFlag.getValue());
	}

	private void getAllPeoples(ByteWriter writer) {
		String[] columns = new String[] { People._ID, People.NAME,
				People.STARRED, People.CUSTOM_RINGTONE, People.NOTES };
		Cursor cur = null;

		try {
			cur = mContext.getContentResolver().query(People.CONTENT_URI,
					columns, null, null, null);
			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					writer.Write(MethodKind.People.getValue());
					writer.Write(cur.getInt(0)); // id
					writer.WriteBlob(cur.getBlob(1)); // name
					writer.WriteBlob(cur.getBlob(3)); // ringtone
					writer.Write(cur.getInt(2) == 1); // starred
					writer.WriteBlob(cur.getBlob(4)); // notes
					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}

	private void getAllPhones(ByteWriter writer) {
		String[] columns = new String[] { Phones._ID, Phones.PERSON_ID,
				Phones.TYPE, Phones.NUMBER, Phones.LABEL };
		Cursor cur = null;

		try {
			cur = mContext.getContentResolver().query(Phones.CONTENT_URI,
					columns, null, null, null);
			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					writer.Write(MethodKind.Phone.getValue());
					writer.Write(cur.getInt(0)); // id
					writer.Write(cur.getInt(1)); // personId
					writer.Write(cur.getInt(2)); // type
					writer.WriteBlob(cur.getBlob(3)); // number
					writer.WriteBlob(cur.getBlob(4)); // label
					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}

	private void getAllOrgInfos(ByteWriter writer) {
		String[] columns = new String[] { Organizations._ID,
				Organizations.PERSON_ID, Organizations.TYPE,
				Organizations.COMPANY, Organizations.TITLE, Organizations.LABEL };
		Cursor cur = null;

		try {
			cur = mContext.getContentResolver().query(
					Organizations.CONTENT_URI, columns, null, null, null);
			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					writer.Write(MethodKind.Organization.getValue());
					writer.Write(cur.getInt(0)); // id
					writer.Write(cur.getInt(1)); // personid
					writer.Write(cur.getInt(2)); // type
					writer.WriteBlob(cur.getBlob(3)); // company
					writer.WriteBlob(cur.getBlob(4)); // jobtitle
					writer.WriteBlob(cur.getBlob(5)); // label
					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}

	}

	private void getAllMethods(ByteWriter writer) {
		String[] columns = new String[] { ContactMethods._ID,
				ContactMethods.PERSON_ID, ContactMethods.KIND,
				ContactMethods.DATA, ContactMethods.AUX_DATA,
				ContactMethods.TYPE, ContactMethods.LABEL };
		Cursor cur = null;

		try {
			cur = mContext.getContentResolver().query(
					ContactMethods.CONTENT_URI, columns, null, null, null);

			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					int kind = cur.getInt(2);
					int id = cur.getInt(0);
					int personId = cur.getInt(1);
					int type = cur.getInt(5);

					switch (kind) {
					case 1:
						writer.Write(MethodKind.Email.getValue());
						writer.Write(id); // id
						writer.Write(personId); // personid
						writer.Write(type); // type
						writer.WriteBlob(cur.getBlob(6)); // label
						writer.WriteBlob(cur.getBlob(3)); // data
						break;

					case 2:
						writer.Write(MethodKind.PostalAddress.getValue());
						writer.Write(id); // id
						writer.Write(personId); // personid
						writer.Write(type); // type
						writer.WriteBlob(cur.getBlob(6)); // label
						writer.WriteBlob(cur.getBlob(3)); // data
						break;

					case 3:
						writer.Write(MethodKind.IM.getValue());
						writer.Write(id); // id
						writer.Write(personId); // personid
						writer.WriteBlob(cur.getBlob(4)); // type(aux_data)
						writer.WriteBlob(cur.getBlob(3)); // data
						writer.WriteBlob(cur.getBlob(6)); // label
						break;
					}

					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}

	private void getAllGroups(ByteWriter writer) {
		Cursor cur = null;
		String[] columns = new String[] { Groups._ID, Groups.NAME, Groups.NOTES };

		try {
			cur = mContext.getContentResolver().query(Groups.CONTENT_URI,
					columns, null, null, null);

			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					writer.Write(MethodKind.Group.getValue());
					writer.Write(cur.getInt(0)); // id
					writer.WriteBlob(cur.getBlob(1)); // name
					writer.WriteBlob(cur.getBlob(2)); // notes
					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}
	
	private void getAllGroupsWithSystem(ByteWriter writer) {
		Cursor cur = null;
		String[] columns = new String[] { Groups._ID, Groups.NAME, Groups.NOTES };

		try {
			cur = mContext.getContentResolver().query(Groups.CONTENT_URI,
					columns, null, null, null);

			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					writer.Write(MethodKind.Group.getValue());
					writer.Write(cur.getInt(0)); // id
					writer.WriteBlob(cur.getBlob(1)); // name
					writer.WriteBlob(cur.getBlob(2)); // notes
					writer.Write(false); // isSystem
					
					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}

	private void getAllMemberShip(ByteWriter writer) {
		String[] columns = new String[] { GroupMembership._ID,
				GroupMembership.PERSON_ID, GroupMembership.GROUP_ID };
		Cursor cur = null;

		try {
			cur = mContext.getContentResolver().query(
					GroupMembership.CONTENT_URI, columns, null, null, null);
			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					writer.Write(MethodKind.GroupMembership.getValue());
					writer.Write(cur.getInt(0)); // id
					writer.Write(cur.getInt(1)); // personId
					writer.Write(cur.getInt(2)); // groupId
					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}

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

		result = result && PeopleDataDeal(addMethods, insertedIds);

		writer.Write(result);

		if (result) {
			for (int id : insertedIds) {
				writer.Write(id);
			}

			writer.Write(MethodKind.EndFlag.getValue());
		}
		else if (bplugin) {
			writer.Write(mErrorCode);
		}
	}

	private ArrayList<MethodBase> getMethodsFromStream(ByteReader reader) {
		ArrayList<MethodBase> lstMethods = new ArrayList<MethodBase>();

		MethodKind kind = MethodKind.FromInt(reader.ReadInt32());
		while (kind != MethodKind.EndFlag) {
			if (kind == MethodKind.Email) {
				Email email = new Email();
				email.Read(reader);
				lstMethods.add(email);
			} else if (kind == MethodKind.IM) {
				IM im = new IM();
				im.Read(reader);
				lstMethods.add(im);
			} else if (kind == MethodKind.PostalAddress) {
				PostalAddress addr = new PostalAddress();
				addr.Read(reader);
				lstMethods.add(addr);
			} else if (kind == MethodKind.Phone) {
				PhoneInfo phone = new PhoneInfo();
				phone.Read(reader);
				lstMethods.add(phone);
			} else if (kind == MethodKind.Organization) {
				OrganizationInfo org = new OrganizationInfo();
				org.Read(reader);
				lstMethods.add(org);
			} else if (kind == MethodKind.Photo) {
				PhotoInfo photo = new PhotoInfo();
				photo.Read(reader);
				lstMethods.add(photo);
			} else if (kind == MethodKind.GroupMembership) {
				GroupMemberShip ms = new GroupMemberShip();
				ms.Read(reader);			
				lstMethods.add(ms);
			}
			kind = MethodKind.FromInt(reader.ReadInt32());
		}

		return lstMethods;
	}

	private boolean AddPeopleInfo(ArrayList<PeopleInfo> lstPeoples) {
		boolean result = false;

		int peopleCnt = lstPeoples.size();
		ContentValues[] lstValues = new ContentValues[peopleCnt];

		for (int i = 0; i < peopleCnt; i++) {
			PeopleInfo people = lstPeoples.get(i);

			ContentValues values = new ContentValues();
			values.put(People.NAME, people.getName());
			values.put(People.STARRED, people.getStarred());

			String notes = people.getNote();
			if (notes != null && !notes.equals(""))
				values.put(People.NOTES, notes);

			String customRingtone = people.getCustomRingtone();
			if (customRingtone.equals("")) {
				Uri defaultRingUri = RingtoneManager
						.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
				customRingtone = defaultRingUri.toString();
			}

			values.put(People.CUSTOM_RINGTONE, customRingtone);

			lstValues[i] = values;
		}

		try {

			int insertedCnt = mContext.getContentResolver().bulkInsert(
					People.CONTENT_URI, lstValues);
			if (insertedCnt == peopleCnt) {
				ArrayList<Integer> ids = getLatestPeopleIds(peopleCnt);
				if (ids.size() == peopleCnt) {
					for (int i = 0; i < peopleCnt; i++) {
						lstPeoples.get(i).setId(ids.get(i));
					}
					result = true;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			result = false;
		} finally {
			// ToDo
		}

		return result;
	}

	private ArrayList<Integer> getLatestPeopleIds(int cnt) {
		ArrayList<Integer> ids = new ArrayList<Integer>();

		Cursor cur = null;
		String[] columns = new String[] { People._ID };
		String order = People._ID + " DESC";

		cur = mContext.getContentResolver().query(People.CONTENT_URI, columns,
				null, null, order);
		if (cur != null) {
			if (cur.moveToFirst()) {
				while (!cur.isAfterLast() && cnt > 0) {
					int id = cur.getInt(0);
					ids.add(0, id);

					cnt--;
					cur.moveToNext();
				}
			}

			cur.close();
		}

		return ids;
	}

	private boolean PeopleDataDeal(ArrayList<MethodBase> lstData,
			ArrayList<Integer> insertedIds) {
		boolean result = true;
		ArrayList<MethodBase> lstMethods = new ArrayList<MethodBase>();
		ArrayList<PhoneInfo> lstPhones = new ArrayList<PhoneInfo>();
		ArrayList<OrganizationInfo> lstOrgs = new ArrayList<OrganizationInfo>();

		for (MethodBase method : lstData) {
			if (method.getKind() == MethodKind.Email
					|| method.getKind() == MethodKind.IM
					|| method.getKind() == MethodKind.PostalAddress) {
				lstMethods.add(method);
			} else if (method.getKind() == MethodKind.Phone) {
				PhoneInfo phone = (PhoneInfo) method;
				lstPhones.add(phone);
			} else if (method.getKind() == MethodKind.Organization) {
				OrganizationInfo org = (OrganizationInfo) method;
				lstOrgs.add(org);
			} else if (method.getKind() == MethodKind.Photo) {
				PhotoInfo photo = (PhotoInfo) method;
				result = result && SetPhoto(photo.getContactId(), photo);
			} else if (method.getKind() == MethodKind.GroupMembership) {
				GroupMemberShip ms = (GroupMemberShip) method;
				if(ms.getActionState() == ActionState.Added){
				result = result
						&& AddGroupMembership(ms.getContactId(), ms
								.getGroupId());
				}else if(ms.getActionState() == ActionState.Deleted){
					result = result && removeFromGroup(ms.getGroupId(), ms.getContactId());
				}
			}
			
			if(!result){
				break;
			}
		}

		if (result == true && lstPhones.size() > 0) {
			result = result && PhoneDeal(lstPhones);
		}

		if (result == true && lstMethods.size() > 0) {
			result = result && MethodDeal(lstMethods);
		}

		if (result == true && lstOrgs.size() > 0) {
			result = result && OrganizationDeal(lstOrgs);
		}

		for (MethodBase method : lstData) {
			if (method.getActionState() == ActionState.Added)
				insertedIds.add(method.getId());
		}

		return result;
	}

	private boolean SetPhoto(Integer personId, PhotoInfo photo) {
		boolean result = true;

		try {
			Uri personUri = ContentUris.withAppendedId(People.CONTENT_URI,
					personId);
			Contacts.People.setPhotoData(mContext.getContentResolver(),
					personUri, photo.getData());
		} catch (Exception e) {
			result = false;
		}
		return result;
	}

	private boolean AddGroupMembership(Integer personId, Integer groupId) {
		boolean result = false;

		try {
			People.addToGroup(mContext.getContentResolver(), personId, groupId);

			result = true;

		} catch (Exception e) {
			e.printStackTrace();
		}

		return result;
	}

	private boolean UpdatePhoneInfo(PhoneInfo phone) {
		boolean result = false;

		try {
			ContentValues values = new ContentValues();

			values.put(Phones.TYPE, phone.getType());
			values.put(Phones.NUMBER, phone.getNumber());
			values.put(Phones.NUMBER_KEY, getNumberkey(phone.getNumber()));
			values.put(Phones.LABEL, phone.getType() == 0 ? phone.getLabel()
					: null);

			Uri uri = ContentUris.withAppendedId(Phones.CONTENT_URI, phone
					.getId());

			if (this.mContext.getContentResolver().update(uri, values, null,
					null) != 0) {
				result = true;
			}
		} finally {
			// ToDo
		}

		return result;
	}

	// ����PhoneInfo
	private boolean AddPhoneInfo(ArrayList<PhoneInfo> lstPhones) {
		boolean result = false;
		int phoneCnt = lstPhones.size();
		ContentValues[] lstValues = new ContentValues[phoneCnt];

		for (int i = 0; i < phoneCnt; i++) {
			ContentValues values = new ContentValues();
			PhoneInfo phone = lstPhones.get(i);

			values.put(Phones.PERSON_ID, phone.getContactId());
			values.put(Phones.TYPE, phone.getType());
			values.put(Phones.NUMBER, phone.getNumber());
			values.put(Phones.NUMBER_KEY, getNumberkey(phone.getNumber()));
			values.put(Phones.LABEL, phone.getType() == 0 ? phone.getLabel()
					: null);

			lstValues[i] = values;
		}

		try {
			int insertedCnt = mContext.getContentResolver().bulkInsert(
					Phones.CONTENT_URI, lstValues);
			if (insertedCnt == phoneCnt) {
				ArrayList<Integer> ids = getLatestPhoneIds(phoneCnt);
				if (ids.size() == phoneCnt) {
					for (int i = 0; i < phoneCnt; i++) {
						lstPhones.get(i).setId(ids.get(i));
					}

					result = true;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			result = false;
		}

		return result;
	}

	private ArrayList<Integer> getLatestPhoneIds(int cnt) {
		ArrayList<Integer> ids = new ArrayList<Integer>();
		Cursor cur = null;
		String[] columns = new String[] { Phones._ID };
		String order = Phones._ID + " DESC";

		cur = mContext.getContentResolver().query(Phones.CONTENT_URI, columns,
				null, null, order);
		if (cur != null) {
			if (cur.moveToFirst()) {
				while (!cur.isAfterLast() && cnt > 0) {
					int id = cur.getInt(0);
					ids.add(0, id);
					cnt--;
					cur.moveToNext();
				}
			}

			cur.close();
		}

		return ids;
	}

	// ɾ��PhoneInfo
	private boolean DeletePhoneInfo(Integer id) {
		boolean result = false;

		try {
			Uri uri = ContentUris.withAppendedId(Phones.CONTENT_URI, id);

			if (this.mContext.getContentResolver().delete(uri, null, null) != 0) {
				result = true;
			}
		} finally {
			// ToDo
		}

		return result;
	}

	private boolean PhoneDeal(ArrayList<PhoneInfo> lstPhones) {
		boolean result = true;
		ArrayList<PhoneInfo> addPhones = new ArrayList<PhoneInfo>();

		for (PhoneInfo phone : lstPhones) {
			if (result == false)
				return result;

			if (phone.getActionState() == ActionState.Added) {
				addPhones.add(phone);
			} else if (phone.getActionState() == ActionState.Edited) {
				result = result && UpdatePhoneInfo(phone);
			} else if (phone.getActionState() == ActionState.Deleted) {
				result = result && DeletePhoneInfo(phone.getId());
			}
		}

		if (result == true && addPhones.size() > 0) {
			result = result && AddPhoneInfo(addPhones);
		}

		return result;
	}

	private String getNumberkey(String number) {
		StringBuffer sbf = new StringBuffer(number);
		return sbf.reverse().toString();
	}

	private boolean UpdateOrgInfo(OrganizationInfo org) {
		boolean result = false;

		try {
			ContentValues values = new ContentValues();

			values.put(Organizations.COMPANY, org.getCompany());
			values.put(Organizations.TITLE, org.getPosition());
			values.put(Organizations.TYPE, org.getType());
			values.put(Organizations.LABEL, org.getType() == 0 ? org.getLabel()
					: null);

			Uri uri = ContentUris.withAppendedId(Organizations.CONTENT_URI, org
					.getId());

			if (this.mContext.getContentResolver().update(uri, values, null,
					null) != 0) {
				result = true;
			}
		} catch (Exception e) {
			e.printStackTrace();
			result = false;
		}

		return result;
	}

	private boolean AddOrgInfo(ArrayList<OrganizationInfo> lstOrg) {
		boolean result = false;
		int orgCnt = lstOrg.size();
		ContentValues[] lstValues = new ContentValues[orgCnt];

		for (int i = 0; i < orgCnt; i++) {
			OrganizationInfo org = lstOrg.get(i);
			ContentValues values = new ContentValues();

			values.put(Organizations.COMPANY, org.getCompany());
			values.put(Organizations.TITLE, org.getPosition());
			values.put(Organizations.TYPE, org.getType());
			values.put(Organizations.LABEL, org.getType() == 0 ? org.getLabel()
					: null);
			values.put(Organizations.PERSON_ID, org.getContactId());

			lstValues[i] = values;
		}

		try {
			int insertedCnt = mContext.getContentResolver().bulkInsert(
					Organizations.CONTENT_URI, lstValues);
			if (insertedCnt == orgCnt) {
				ArrayList<Integer> ids = getLatestOrgIds(orgCnt);

				if (ids.size() == orgCnt) {
					for (int i = 0; i < orgCnt; i++) {
						lstOrg.get(i).setId(ids.get(i));
					}

					result = true;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
			result = false;
		}

		return result;
	}

	private ArrayList<Integer> getLatestOrgIds(int cnt) {
		ArrayList<Integer> ids = new ArrayList<Integer>();

		String[] columns = new String[] { Organizations._ID };
		String order = Organizations._ID + " DESC";
		Cursor cur = mContext.getContentResolver().query(
				Organizations.CONTENT_URI, columns, null, null, order);
		if (cur != null) {
			if (cur.moveToFirst()) {
				while (!cur.isAfterLast() && cnt > 0) {
					int id = cur.getInt(0);
					ids.add(0, id);
					cnt--;
					cur.moveToNext();
				}
			}
			cur.close();
		}

		return ids;
	}

	// ɾ����֯��Ϣ
	private boolean DeleteOrgInfo(Integer id) {
		boolean result = false;

		try {
			Uri uri = ContentUris.withAppendedId(Organizations.CONTENT_URI, id);

			if (this.mContext.getContentResolver().delete(uri, null, null) != 0) {
				result = true;
			}
		} finally {
			// ToDo
		}

		return result;
	}

	// 组织相关操作
	private boolean OrganizationDeal(ArrayList<OrganizationInfo> lstOrg) {
		boolean result = true;
		ArrayList<OrganizationInfo> addOrgs = new ArrayList<OrganizationInfo>();

		for (OrganizationInfo org : lstOrg) {
			if (org.getActionState() == ActionState.Added) {
				addOrgs.add(org);
			} else if (org.getActionState() == ActionState.Edited) {
				result = result && UpdateOrgInfo(org);
			} else if (org.getActionState() == ActionState.Deleted) {
				result = result && DeleteOrgInfo(org.getId());
			}
		}

		if (result == true && addOrgs.size() > 0) {
			result = result && AddOrgInfo(addOrgs);
		}

		return result;
	}

	private boolean UpdateMethodInfo(ContentValues values, Integer id) {
		boolean result = false;

		try {
			Uri uri = ContentUris
					.withAppendedId(ContactMethods.CONTENT_URI, id);

			if (this.mContext.getContentResolver().update(uri, values, null,
					null) != 0) {
				result = true;
			}
		} finally {
			// ToDo
		}

		return result;
	}

	// ����Method
	private int AddMethodInfo(ContentValues[] lstValues) {
		int insertedCnt = 0;

		try {
			insertedCnt = this.mContext.getContentResolver().bulkInsert(
					ContactMethods.CONTENT_URI, lstValues);
		} catch (Exception e) {
			e.printStackTrace();
		}

		return insertedCnt;
	}

	// ɾ��Method
	private boolean DeleteMethodInfo(Integer id) {
		boolean result = false;

		try {
			Uri uri = ContentUris
					.withAppendedId(ContactMethods.CONTENT_URI, id);

			if (this.mContext.getContentResolver().delete(uri, null, null) != 0) {
				result = true;
			}
		} finally {
			// ToDo
		}

		return result;
	}

	// email，im，address相关操作
	private boolean MethodDeal(ArrayList<MethodBase> lstMethods) {
		boolean result = true;
		ArrayList<MethodBase> addMethods = new ArrayList<MethodBase>();
		ArrayList<ContentValues> addValues = new ArrayList<ContentValues>();

		for (MethodBase method : lstMethods) {
			ContentValues values = new ContentValues();

			if (method.getKind() == MethodKind.Email) {
				Email email = (Email) method;

				values.put(ContactMethods.KIND, email.getKind().getValue());
				values.put(ContactMethods.DATA, email.getEmailAddress());
				values.put(ContactMethods.TYPE, email.getType());
				values.put(ContactMethods.LABEL, email.getType() == 0 ? email
						.getLabel() : null);
			} else if (method.getKind() == MethodKind.IM) {
				IM im = (IM) method;

				values.put(ContactMethods.KIND, im.getKind().getValue());
				values.put(ContactMethods.DATA, im.getImAddress());
				values.put(ContactMethods.AUX_DATA, im.getType());
				values.put(ContactMethods.TYPE, 3); // 数据库中im的类型都昄1�7"3"
				values.put(ContactMethods.LABEL, im.getType() == "pre:-1" ? im
						.getLabel() : null);
			} else if (method.getKind() == MethodKind.PostalAddress) {
				PostalAddress addr = (PostalAddress) method;

				values.put(ContactMethods.KIND, addr.getKind().getValue());
				values.put(ContactMethods.DATA, addr.getDisplayAddress());
				values.put(ContactMethods.TYPE, addr.getType());
				values.put(ContactMethods.LABEL, addr.getType() == 0 ? addr
						.getLabel() : null);
			} else {
				continue;
			}

			if (method.getActionState() == ActionState.Added) {
				values.put(ContactMethods.PERSON_ID, method.getContactId());
				addMethods.add(method);
				addValues.add(values);
			} else if (method.getActionState() == ActionState.Edited) {
				result = result && UpdateMethodInfo(values, method.getId());
			} else if (method.getActionState() == ActionState.Deleted) {
				result = result && DeleteMethodInfo(method.getId());
			}
		}

		if (result == true && addMethods.size() > 0) {
			result = false;
			int insertedCnt = AddMethodInfo(addValues
					.toArray(new ContentValues[addValues.size()]));
			if (insertedCnt == addMethods.size()) {
				ArrayList<Integer> ids = getLatestMethodIds(insertedCnt);
				if (ids.size() == insertedCnt) {
					for (int i = 0; i < insertedCnt; i++) {
						lstMethods.get(i).setId(ids.get(i));
					}
					result = true;
				}
			}
		}

		return result;
	}

	private ArrayList<Integer> getLatestMethodIds(int cnt) {
		ArrayList<Integer> ids = new ArrayList<Integer>();

		String[] columns = new String[] { ContactMethods._ID };
		String order = ContactMethods._ID + " DESC";

		Cursor cur = mContext.getContentResolver().query(
				ContactMethods.CONTENT_URI, columns, null, null, order);
		if (cur != null) {
			if (cur.moveToFirst()) {
				while (!cur.isAfterLast() && cnt > 0) {
					int id = cur.getInt(0);
					ids.add(0, id);
					cnt--;
					cur.moveToNext();
				}
			}
			cur.close();
		}

		return ids;
	}

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

		result = result && PeopleDataDeal(methods, insertedIds);

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
			values.put(People.NAME, people.getName());
			values.put(People.STARRED, people.getStarred());

			String notes = people.getNote();
			if (notes != null && !notes.equals(""))
				values.put(People.NOTES, notes);

			String customRingtone = people.getCustomRingtone();
			if (customRingtone.equals("")) {
				Uri defaultRingUri = RingtoneManager
						.getDefaultUri(RingtoneManager.TYPE_RINGTONE);
				customRingtone = defaultRingUri.toString();
			}

			values.put(People.CUSTOM_RINGTONE, customRingtone);

			Uri uri = ContentUris.withAppendedId(People.CONTENT_URI, people
					.getId());

			if (this.mContext.getContentResolver().update(uri, values, null,
					null) != 0) {
				result = true;
			}
		} catch (Exception e) {
			result = false;
			e.printStackTrace();
		} finally {
			// ToDo
		}

		return result;
	}

	public void GetPhoto(ByteReader reader, ByteWriter writer) {
		Integer personId = reader.ReadInt32();

		String[] columns = new String[] { Contacts.Photos._ID,
				Contacts.Photos.DATA };
		String where = "person=" + personId;
		Cursor cur = null;
		writer.Write(MethodKind.Photo.getValue());

		try {
			cur = mContext.getContentResolver().query(
					Contacts.Photos.CONTENT_URI, columns, where, null, null);
			if (null != cur && cur.moveToFirst()) {
				writer.Write(cur.getInt(0));
				writer.Write(personId);
				byte[] tmpBytes = cur.getBlob(1);
				if (tmpBytes != null) {
					writer.Write(tmpBytes.length);
					writer.Write(tmpBytes);
				} else {
					writer.Write(0);
				}
			} else {
				writer.Write(0);
				writer.Write(personId);
				writer.Write(0);
			}

		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}

	public void DeleteContacts(ByteReader reader, ByteWriter writer) {
		int cnt = reader.ReadInt32();
		boolean result = false;
		
		if (cnt > 0) {
			String strIds = "";
			//int[] ids = new int[cnt];

			for (int i = 0; i < cnt; i++) {
				strIds += reader.ReadInt32() + ",";
			}
			strIds = strIds.substring(0, strIds.length() - 1);			

			try {
				String where = "_id in (" + strIds + ")";

				if (this.mContext.getContentResolver().delete(
						People.CONTENT_URI, where, null) != 0) {
					result = true;
				}
				result = true;

			} catch(Exception e){
				e.printStackTrace();
				result = false;
			}
		}
		
		writer.Write(result);
	}
	
	public void AddGroup(ByteReader reader, ByteWriter writer) {
		GroupInfo group = new GroupInfo();
		group.Read(reader);
		
		boolean result = false;

		try {
			ContentValues values = new ContentValues();

			values.put(Groups.NAME, group.getName());
			values.put(Groups.NOTES, group.getNotes());

			Uri inserted = this.mContext.getContentResolver().insert(
					Groups.CONTENT_URI, values);
			Integer id = Integer.parseInt(inserted.getLastPathSegment());
			group.setId(id);

			result = true;

		} catch(Exception e){
			e.printStackTrace();
			result = false;
		}		
		
		writer.Write(result);

		if (result) {
			writer.Write(group.getId());
		}
	}
	
	public void UpdateGroup(ByteReader reader, ByteWriter writer) {
		GroupInfo group = new GroupInfo();
		group.Read(reader);
		boolean result = false;

		try {
			ContentValues values = new ContentValues();

			values.put(Groups.NAME, group.getName());
			values.put(Groups.NOTES, group.getNotes());

			Uri uri = ContentUris.withAppendedId(Groups.CONTENT_URI, group
					.getId());
			if (this.mContext.getContentResolver().update(uri, values, null,
					null) != 0) {
				result = true;
			}

		} catch(Exception e){
			e.printStackTrace();
			result = false;
		}
		
		writer.Write(result);
	}
	
	public void DeleteGroup(ByteReader reader, ByteWriter writer) {
		int id = reader.ReadInt32();
		boolean result = false;

		try {
			Uri uri = ContentUris.withAppendedId(Groups.CONTENT_URI, id);
			if (this.mContext.getContentResolver().delete(uri, null, null) != 0) {
				result = true;
			}
		} catch(Exception e){
			e.printStackTrace();
			result = false;
		}

		writer.Write(result);
	}
	
	public void SetFavorite(ByteReader reader, ByteWriter writer) {
		boolean starred = reader.ReadBoolean();
		int cnt = reader.ReadInt32();
		int[] peopleIds = new int[cnt];

		for (int i = 0; i < cnt; i++) {
			peopleIds[i] = reader.ReadInt32();
		}
		
		boolean result = false;
		try {
			ContentValues values = new ContentValues();
			values.put(People.STARRED, starred ? 1 : 0);
			for (int id : peopleIds) {
				Uri uri = ContentUris.withAppendedId(People.CONTENT_URI, id);
				this.mContext.getContentResolver().update(uri, values, null,
						null);
			}

			result = true;

		} catch(Exception e){
			e.printStackTrace();
			result = false;
		}
		
		writer.Write(result);
	}
	
	public void AddToGroup(ByteReader reader, ByteWriter writer) {
		int newGroupId = reader.ReadInt32();
		int peopleCnt = reader.ReadInt32();
		int[] peopleIds = new int[peopleCnt];

		for (int i = 0; i < peopleCnt; i++) {
			peopleIds[i] = reader.ReadInt32();
		}
	
		boolean result = false;
		try {
			for (int i = 0; i < peopleIds.length; i++) {
				Contacts.People.addToGroup(mContext.getContentResolver(),
						peopleIds[i], newGroupId);
			}

			result = true;

		}catch(Exception e){
			e.printStackTrace();
			result = false;
		}
		
		writer.Write(result);
	}
	
	public void RemoveFromGroup(ByteReader reader, ByteWriter writer) {
		int oldGroupId = reader.ReadInt32();
		int peopleCnt = reader.ReadInt32();
		int[] peopleIds = new int[peopleCnt];

		for (int i = 0; i < peopleCnt; i++) {
			peopleIds[i] = reader.ReadInt32();
		}

		boolean result = false;
		Cursor delCur = null;

		try {
			for (int i = 0; i < peopleIds.length; i++) {
				Uri uri = ContentUris.withAppendedId(
						Contacts.GroupMembership.CONTENT_URI, peopleIds[i]);
				mContext.getContentResolver().delete(uri, null, null);
			}

			result = true;

		} catch (Exception e) {
			e.printStackTrace();
			result = false;
		} finally {
			if (null != delCur) {
				delCur.close();
			}
		}
		
		writer.Write(result);
	}
	
	private boolean removeFromGroup(int groupId, int contactId){
		List<Integer> contactIds = new ArrayList<Integer>();
		contactIds.add(contactId);
		return removeFromGroup(groupId, contactIds);
	}
	
	private boolean removeFromGroup(int groupId, List<Integer> contactIds){	
		boolean result = false;
		Cursor delCur = null;

		try {
			for (int contactId : contactIds) {
				Uri uri = ContentUris.withAppendedId(
						Contacts.GroupMembership.CONTENT_URI, contactId);
				mContext.getContentResolver().delete(uri, null, null);
			}

			result = true;

		} catch (Exception e) {
			e.printStackTrace();
			result = false;
		} finally {
			if (null != delCur) {
				delCur.close();
			}
		}
		
		return result;
	}
	
	public void GetPeopleCount(ByteWriter writer) {
		Cursor peopleCur = null;

		try {
			String[] columns = new String[]{ People._ID};
			peopleCur = mContext.getContentResolver().query(People.CONTENT_URI,
					columns, null, null, null);

			if (null != peopleCur) {
				int cnt = peopleCur.getCount();
				writer.Write(cnt);
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (peopleCur != null) {
				peopleCur.close();
			}
		}		
	}

	@Override
	public void LoadContactsWithGroupSystem(ByteWriter writer) {
		// people
		getAllPeoples(writer);

		// phone
		getAllPhones(writer);

		// org
		getAllOrgInfos(writer);

		// group membership
		getAllMemberShip(writer);

		// group
		getAllGroupsWithSystem(writer);

		// method
		getAllMethods(writer);

		// end
		writer.Write(MethodKind.EndFlag.getValue());
	}

	@Override
	public void LoadContactsEx(ByteWriter writer) {
		// people
		getAllPeoples(writer);

		// phone
		getAllPhones(writer);

		// org
		getAllOrgInfos(writer);

		// group membership
		getAllMemberShip(writer);

		// group
		getAllGroupsEx(writer);

		// method
		getAllMethods(writer);

		// end
		writer.Write(MethodKind.EndFlag.getValue());
	}
	
	private void getAllGroupsEx(ByteWriter writer) {
		Cursor cur = null;
		String[] columns = new String[] { Groups._ID, Groups.NAME, Groups.NOTES ,Groups.SYSTEM_ID};

		try {
			cur = mContext.getContentResolver().query(Groups.CONTENT_URI,
					columns, null, null, null);

			if (null != cur && cur.moveToFirst()) {
				while (!cur.isAfterLast()) {
					writer.Write(MethodKind.Group.getValue());
					writer.Write(cur.getInt(0)); // id
					writer.WriteBlob(cur.getBlob(1)); // name
					writer.WriteBlob(cur.getBlob(2)); // notes
					String systemId = cur.getString(3);
					boolean isSystem = false;
					if(systemId!=null&&!systemId.equals("")){
						isSystem = true;
					}
					writer.Write(isSystem);
					cur.moveToNext();
				}
			}
		} finally {
			if (null != cur) {
				cur.close();
			}
		}
	}
}
