package daemon.provider.business;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;


import daemon.logger.LogCenter;
import daemon.model.EmailAccount;
import daemon.model.EmailAttachment;
import daemon.model.EmailInfo;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;


public class EmailProvider implements IProvider{
	private Context mContext = null;
	private Uri ACCOUNT_URI = Uri.parse("content://borqs-mail/accounts/");
	private Uri MESSAGE_URI = Uri.parse("content://borqs-mail/messages-all");
	
	public final String ACCOUNT_ID = "_id";
	public final String ACCOUNT_USER_NAME = "_username";
	public final String ACCOUNT_PASSWORD = "_password";
	public final String ACCOUNT_IN_SERVER = "_inserver";
	public final String ACCOUNT_OUT_SERVER = "_outserver";
	public final String ACCOUNT_IN_PORT = "_inport";
	public final String ACCOUNT_OUT_PORT = "_outport";
	public final String ACCOUNT_FROM_NAME =  "_fromname";
	public final String ACCOUNT_FROM_ADDRESS = "_fromaddress";
	public final String ACCOUNT_REPLY_TO_NAME = "_replytoname";
	public final String ACCOUNT_REPLY_TO_ADDRESS = "_replytoaddress";
	public final String ACCOUNT_COPY_ADDRESS = "_copyaddress";
	public final String ACCOUNT_NEXT_FETCH_TIME = "_nextfetchtime";
	public final String ACCOUNT_SMS =  "_sms";
	public final String ACCOUNT_APN = "_apn";
	public final String ACCOUNT_COMPANY_ID = "_companyid";
	public final String ACCOUNT_SIGNATURE = "_signature";
	public final String ACCOUNT_FLAGS = "_flags";
	
	public final String TRACK_ACCOUNT = "_account";
	public final String TRACK_UIDL = "_uidl";
	public final String TRACK_DELETED_FLAG = "_delflag";
	
	public final String MAIL_ID = "_id";
	public final String MAIL_UIDL = "_uidl";
	public final String MAIL_FROM = "_from";
	public final String MAIL_FROM_DISPLAY = "_from_display";
    public final String MAIL_TO = "_to";
    public final String MAIL_TO_DISPLAY = "_to_display";
    public final String MAIL_REPLY_TO = "_replyto";
    public final String MAIL_DATE = "_date";
    public final String MAIL_SUBJECT =  "_subject";
    public final String MAIL_FLAGS =  "_flags";
    public final String MAIL_READ_SIZE = "_readsize";
    public final String MAIL_TOTAL_SIZE = "_totalsize";
    public final String MAIL_ACCOUNT = "_account";
    public final String MAIL_MESSAGE_BOX = "_msgbox";
    public final String MAIL_CC = "_cc";
    public final String MAIL_BCC = "_bcc";
    public final String MAIL_HEADERS = "_headers";
    
    public final String PART_ID = "_id";
    public final String PART_ACCOUNT = "_account";
    public final String PART_MESSAGE = "_message";
    public final String PART_C_ID = "_cid";
    public final String PART_SEQ = "_seq";
    public final String PART_MIME_TYPE = "_mimetype";
    public final String PART_CHAR_SET = "_charset";
    public final String PART_ENCODING =  "_encoding";
    public final String PART_FILE_NAME = "_filename";
    public final String PART_SIZE = "_size";
    public final String PART_FLAGS =  "_flags";
    public final String PART_DATA = "_data";
    
    public final String FOLDER_TYPE =  "_type";
    public final String FOLDER_ID = "_id";
	
	public EmailProvider(Context context){
		mContext = context;
	}

	@Override
	public void execute(ProviderExecuteContext ctx) {
		ByteReader reader = ctx.getByteReader();
		ByteWriter writer = ctx.getByteWriter();
		int action = reader.ReadInt32();
		writer.Write(action);
		switch (action) {
		case 1:// ��ѯ
			Query(writer);
			break;
		case 2:// �����ʼ�
			AddEmail(reader, writer);
			break;	
		case 3:// ɾ���ʼ�
			DeleteEmail(reader, writer);
			break;
		case 4:// �ƶ��ʼ�
			MoveEmail(reader, writer);
			break;
		case 5:// ��ȡ����
			GetAttachments(reader, writer);
			break;
//		case 6:// ��ȡ�ʼ�����
//			GetText(reader, writer);
//			break;
		}
	}

	@Override
	public int getBusiness() {
		// TODO Auto-generated method stub
		return 9;
	}


	private Uri getFolderUri(int accountId){
		String str = "content://borqs-mail/folders/" + accountId;
		return Uri.parse(str);
	}
	
	private Uri getTrackUri(int accountId){
		String str = "content://borqs-mail/tracks/" + accountId;
		return Uri.parse(str);
	}
	
	private Uri getPartUri(int accountId, int messageId){
		String str = "content://borqs-mail/messages/" + accountId + "/" + messageId + "/parts/";
		return Uri.parse(str);
	}
	
	private Uri getUriWithFolder(int accountId, int folderId){
		String str = "content://borqs-mail/messages/" + accountId + "/folders/" + folderId;
		return Uri.parse(str);
	}
	
	private void Query(ByteWriter writer) {
		List<EmailAccount> accountLst = queryAllEmails();

		int cnt = accountLst.size();
		writer.Write(cnt); // д���˻���Ŀ

		for (EmailAccount account : accountLst) {
			account.Write(writer);
			
			writer.Write(account.getTracks().size());
			for(String uidl : account.getTracks()){
				writer.Write(uidl);
			}
			
            writer.Write(account.getEmails().size());            
			for (EmailInfo email : account.getEmails()) {
				email.Write(writer);
			}
		}
	}

	// ����
	private void AddEmail(ByteReader reader, ByteWriter writer) {
		EmailInfo email = new EmailInfo();
		email.Read(reader);

		boolean addSuccess = addEmail(email);
		writer.Write(addSuccess);
		if (addSuccess) {
			writer.Write(email.getId());
		}
	}	

	// ɾ��
	private void DeleteEmail(ByteReader reader, ByteWriter writer) {
		int msgID = reader.ReadInt32();
		int accountID = reader.ReadInt32();
		
		boolean delSuccess = deleteEmail(msgID, accountID);
		writer.Write(delSuccess);
	}
	
	// �ƶ�
	private void MoveEmail(ByteReader reader, ByteWriter writer){
		int msgID = reader.ReadInt32();
		int accountID = reader.ReadInt32();
		int desFolder = reader.ReadInt32();
		
		boolean moveSuccess = moveEmail(msgID, accountID, desFolder);
		writer.Write(moveSuccess);
	}
	
	// ��ȡ����
	private void GetAttachments(ByteReader reader, ByteWriter writer){
		int msgID = reader.ReadInt32();
		int accountID = reader.ReadInt32();
		
		ArrayList<EmailAttachment> attachLst = getAttachments(accountID, msgID);
		
		writer.Write(attachLst.size());
		
		for(EmailAttachment attach : attachLst){
			attach.Write(writer);
		}
	}
	
	//��ȡ�ʼ�����
	private void GetText(ByteReader reader, ByteWriter writer){
		int msgID = reader.ReadInt32();
		int accountID = reader.ReadInt32();
		
		EmailAttachment attach = getEmailText(accountID, msgID);		
		
		attach.Write(writer);		
	}
	
	public ArrayList<EmailAccount> queryAllEmails() {
		ArrayList<EmailAccount> result = new ArrayList<EmailAccount>();
		Cursor messageCur = null;
		Cursor accountCur = mContext.getContentResolver().query(ACCOUNT_URI,
				null, null, null, null);
		Cursor folderCur = null;
		Cursor trackCur = null;
		try {
			if (null != accountCur && accountCur.moveToFirst()) {
				
				while (!accountCur.isAfterLast()) {
					//�˻���Ϣ
					EmailAccount account = new EmailAccount();
					account.setId(accountCur.getInt(accountCur
						   .getColumnIndexOrThrow(ACCOUNT_ID)));
					account.setUserName(accountCur.getString(accountCur
						   .getColumnIndexOrThrow(ACCOUNT_USER_NAME)));
					account.setPassword(accountCur.getString(accountCur
						   .getColumnIndexOrThrow(ACCOUNT_PASSWORD)));
					account	.setInServer(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_IN_SERVER))));
					account.setInPort(accountCur.getInt(accountCur
						   .getColumnIndexOrThrow(ACCOUNT_IN_PORT)));
					account.setOutServer(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_OUT_SERVER))));
					account.setOutPort(accountCur.getInt(accountCur
						   .getColumnIndexOrThrow(ACCOUNT_OUT_PORT)));
					account.setFromName(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_FROM_NAME))));
					account.setFromAddress(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_FROM_ADDRESS))));
					account.setReplyToName(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_REPLY_TO_NAME))));
					account.setReplyToAddress(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_REPLY_TO_ADDRESS))));
					account.setCopyAddress(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_COPY_ADDRESS))));
					account.setNextFetchTime(accountCur.getLong(accountCur
						   .getColumnIndexOrThrow(ACCOUNT_NEXT_FETCH_TIME)));
					account.setSms(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_SMS))));
					account.setApn(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_APN))));
					account.setCompanyId(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_COMPANY_ID))));
					account.setSignature(CheckStringNull(accountCur
						   .getString(accountCur.getColumnIndexOrThrow(ACCOUNT_SIGNATURE))));
					account.setFlags(accountCur.getInt(accountCur
						   .getColumnIndexOrThrow(ACCOUNT_FLAGS)));

					//tracks
					Uri trackURI = getTrackUri(account.getId());
					trackCur = mContext.getContentResolver().query(trackURI,
							null, null, null, null);
					if(null != trackCur && trackCur.moveToFirst()){
						while(!trackCur.isAfterLast()){
							account.getTracks().add(trackCur.getString(trackCur
								 .getColumnIndexOrThrow(TRACK_UIDL)));
							trackCur.moveToNext();
						}
					}
					
					//�ʼ���Ϣ
					String condition = "_account=" + account.getId();
					messageCur = mContext.getContentResolver().query(
							MESSAGE_URI, null, condition, null, null);
					if (null != messageCur && messageCur.moveToFirst()) {
						while (!messageCur.isAfterLast()) {
							EmailInfo email = new EmailInfo();
							email.setId(messageCur.getInt(messageCur
								 .getColumnIndexOrThrow(MAIL_ID)));
							email.setUidl(messageCur.getString(messageCur
								 .getColumnIndexOrThrow(MAIL_UIDL)));
							email.setFrom(CheckStringNull(messageCur
								 .getString(messageCur.getColumnIndexOrThrow(MAIL_FROM))));
							email.setFromDisplay(CheckStringNull(messageCur
								 .getString(messageCur.getColumnIndexOrThrow(MAIL_FROM_DISPLAY))));
							email.setTo(CheckStringNull(messageCur
								 .getString(messageCur.getColumnIndexOrThrow(MAIL_TO))));
							email.setToDisplay(CheckStringNull(messageCur
								 .getString(messageCur.getColumnIndexOrThrow(MAIL_TO_DISPLAY))));
							email.setReplyTo(CheckStringNull(messageCur
								 .getString(messageCur.getColumnIndexOrThrow(MAIL_REPLY_TO))));
							email.setDate(messageCur.getLong(messageCur
								 .getColumnIndexOrThrow(MAIL_DATE)));
							email.setSubject(CheckStringNull(messageCur
								 .getString(messageCur.getColumnIndexOrThrow(MAIL_SUBJECT))));
							email.setFlags(messageCur.getInt(messageCur
								 .getColumnIndexOrThrow(MAIL_FLAGS)));
							email.setReadSize(messageCur.getInt(messageCur
								 .getColumnIndexOrThrow(MAIL_READ_SIZE)));
							email.setTotalSize(messageCur.getInt(messageCur
								 .getColumnIndexOrThrow(MAIL_TOTAL_SIZE)));
							email.setAccountID(messageCur.getInt(messageCur
								 .getColumnIndexOrThrow(MAIL_ACCOUNT)));

							// ��ȡ�ʼ�����
							Uri folderUri = getFolderUri(email.getAccountID());
							Integer msgBox = messageCur.getInt(messageCur
									.getColumnIndexOrThrow(MAIL_MESSAGE_BOX));
							folderCur = mContext.getContentResolver().query(
									ContentUris.withAppendedId(folderUri,
											msgBox), null, null, null, null);
							if (null != folderCur && folderCur.moveToFirst()) {
								email.setType(folderCur.getInt(folderCur
										.getColumnIndexOrThrow(FOLDER_TYPE)));
							}
							account.getEmails().add(email);

							messageCur.moveToNext();
						}
					} else {

					}

					result.add(account);
					accountCur.moveToNext();
				}			
		     }
		} finally {
			if (null != accountCur) {
				accountCur.close();
			}

			if(null != trackCur){
				trackCur.close();
			}
			
			if (null != messageCur) {
				messageCur.close();
			}

			if (null != folderCur) {
				folderCur.close();
			}
		}
		return result;
	}

	// ����ʼ�
	public boolean addEmail(EmailInfo email) {
		boolean result = false;
		Cursor folderCur = null;
		try {
			int folderID = 0;
			Uri folderUri = getFolderUri(email.getAccountID());
			String where = "_account=" + email.getAccountID() + " and _type=" +email.getType();
			folderCur = mContext.getContentResolver().query(
					folderUri, null, where, null, null);
		    
			if (null != folderCur && folderCur.moveToFirst()) {
				folderID = folderCur.getInt(folderCur.getColumnIndexOrThrow(FOLDER_ID));
			}else{
				return false;
			}
			
			Uri ADD_MESSAGE_URI = getUriWithFolder(email.getAccountID(),
					folderID);

			ContentValues values = new ContentValues();
			values.put(MAIL_UIDL, CheckStringNull(email.getUidl()));
			values.put(MAIL_MESSAGE_BOX, folderID);
			values.put(MAIL_FROM, email.getFrom());
			values.put(MAIL_FROM_DISPLAY, email.getFromDisplay());
			values.put(MAIL_TO, email.getTo());
			values.put(MAIL_TO_DISPLAY, email.getToDisplay());		
			values.put(MAIL_REPLY_TO, CheckStringNull(email.getReplyTo()));
			values.put(MAIL_DATE, email.getDate());
			values.put(MAIL_SUBJECT, email.getSubject());		
			values.put(MAIL_FLAGS, email.getFlags());
			values.put(MAIL_READ_SIZE, email.getReadSize());
			values.put(MAIL_TOTAL_SIZE, email.getTotalSize());
			values.put(MAIL_ACCOUNT, email.getAccountID());

			Uri inserted = mContext.getContentResolver().insert(
					ADD_MESSAGE_URI, values);

			Integer id = Integer.parseInt(inserted.getLastPathSegment());
			email.setId(id);

			//����uidl��track��
			Uri trackURI = getTrackUri(email.getAccountID());
			values = new ContentValues();
			values.put(TRACK_ACCOUNT, email.getAccountID());
			values.put(TRACK_UIDL, email.getUidl());
			values.put(TRACK_DELETED_FLAG, 0);
			mContext.getContentResolver().insert(trackURI, values);
			
			result = true;
			
		} catch (Exception e) {
			e.printStackTrace();
			result = false;
		} finally{
			if(null != folderCur){
				folderCur.close();
			}
		}
		
		return result;
	}

	// ɾ���ʼ�
	public boolean deleteEmail(Integer msgID, Integer accountID) {
		boolean result = false;

		 try {
			 // ɾ���ʼ�
			 Uri msgURI = ContentUris.withAppendedId(MESSAGE_URI, msgID);
		     int cnt = mContext.getContentResolver().delete(msgURI, null, null);
		     if(cnt != 0) {
		    	result = true;
		     }else{
		    	 return false;
		     }
		     
		     // ɾ���������
		     Uri partURI = getPartUri(accountID, msgID);
		     mContext.getContentResolver().delete(partURI, null, null);		
				
		 } catch (Exception e) {
			 e.printStackTrace();
			 result = false;
		 }

		return result;
	}	
	
	// �ƶ��ʼ�
	public boolean moveEmail(Integer msgID, Integer accountID, Integer desFolder){
		boolean result = false;
		Cursor folderCur = null;
		
		try{	
			int folderID = 0;
			Uri folderUri = getFolderUri(accountID);
			String where = "_account=" + accountID + " and _type=" + desFolder;
			folderCur = mContext.getContentResolver().query(
					folderUri, null, where, null, null);
		    
			if (null != folderCur && folderCur.moveToFirst()) {
				folderID = folderCur.getInt(folderCur.getColumnIndexOrThrow(FOLDER_ID));
			}else{
				return false;
			}
			
			ContentValues values = new ContentValues();
			values.put(MAIL_MESSAGE_BOX, folderID);
			
			String condition = "_id=" + msgID;

			// �����ʼ�����ֵΪ0����ʵ���и��³ɹ�
			mContext.getContentResolver().update(MESSAGE_URI, values, condition, null); 			
		    result = true;
			
		}catch(Exception e){
		    e.printStackTrace();
			result = false;
		}finally{
			if(null != folderCur){
				folderCur.close();
			}
		}
		
		return result;
	}
	
	// ��ȡ����������
	public ArrayList<EmailAttachment> getAttachments(Integer accountID, Integer msgID){
		ArrayList<EmailAttachment> attachLst = new ArrayList<EmailAttachment>();
		
		Uri partURI = getPartUri(accountID, msgID);
		Cursor partCur = mContext.getContentResolver().query(partURI,
				null, null, null, null);
		
		try {
			if (null != partCur && partCur.moveToFirst()) {
				
				while (!partCur.isAfterLast()) {
					EmailAttachment attach = new EmailAttachment();
					attach.setID(partCur.getInt(partCur.getColumnIndexOrThrow(PART_ID)));
					attach.setFileName(partCur.getString(partCur.getColumnIndexOrThrow(PART_FILE_NAME)));
					attach.setFlags(partCur.getInt(partCur.getColumnIndexOrThrow(PART_FLAGS)));
					attach.setData(getAttachments(ContentUris.withAppendedId(partURI,attach.getID())));
					
					attachLst.add(attach);
					
					partCur.moveToNext();
				}
			}		
		}catch (Exception e){
			LogCenter.error("Get Part ID is exception",e.toString());
		}finally{
			if(null != partCur){
				partCur.close();
			}
		}
		
		return attachLst;
	}
	
	//��ȡ���ݣ����ڲݸ��䣬��pc�ϲ�����eml�ļ�ʱ�����ֻ������زݸ����ݣ�
	public EmailAttachment getEmailText(Integer accountID, Integer msgID){	
		EmailAttachment result = new EmailAttachment();
		Uri partURI = getPartUri(accountID, msgID);
		
		String condition = "_mimetype='text/plain' or _mimetype='text/html'";
		Cursor partCur = mContext.getContentResolver().query(partURI,
				null, condition, null, null);
		
		try{
			if (null != partCur && partCur.moveToFirst()) {
				try {
					EmailAttachment attach = new EmailAttachment();
					attach.setID(partCur.getInt(partCur.getColumnIndexOrThrow(PART_ID)));
					attach.setFileName(partCur.getString(partCur.getColumnIndexOrThrow(PART_FILE_NAME)));
					attach.setFlags(partCur.getInt(partCur.getColumnIndexOrThrow(PART_FLAGS)));
					attach.setIsHtml(partCur.getString(partCur.getColumnIndexOrThrow(PART_MIME_TYPE))== "text/html"? true: false);
					attach.setData(getAttachments(ContentUris.withAppendedId(partURI,attach.getID())));												
				}catch (Exception e){
					LogCenter.error("Get Part ID is exception",e.toString());
				}
			}
		}finally{
			if(null != partCur){
				partCur.close();
			}
		}
		return result;			
	}
	
	
	// ��ȡ����������
	public byte[] getAttachments(Uri uri) throws IOException{
	
		InputStream stream = mContext.getContentResolver().openInputStream(uri);
		ByteArrayOutputStream baos = new ByteArrayOutputStream();
			
		if (stream != null) {
			byte[] data = new byte[stream.available()];
			for (int len = stream.read(data); len >= 0; len = stream.read(data)) {
				baos.write(data, 0, len);
			}
			
			if(null != stream ){
				try{
					stream.close();
				}catch(Exception e){
					LogCenter.error("close is exception",e.toString());
				}
			}
			return baos.toByteArray();
		} else {
			return null;
		}		
		
	}
	
    // ��Ӹ���
	public boolean addPart(Integer accountId, Integer messageId) {
		boolean result = false;
		try {
			Uri uri = getPartUri(accountId, messageId);
			
			ContentValues values = new ContentValues();
			values.put(PART_ACCOUNT, accountId);
			values.put(PART_MESSAGE, messageId);
			values.put(PART_C_ID, "");
			values.put(PART_SEQ, 0);
			values.put(PART_MIME_TYPE, "text/plain");
			values.put(PART_CHAR_SET, "UTF-8");
			values.put(PART_ENCODING, "");
			values.put(PART_FILE_NAME, "");
			values.put(PART_SIZE, 0);
			values.put(PART_FLAGS, 0);
			values.put(PART_DATA,
					"/data/data/oms.mail.provider/app_mail/PART_1253779441045");

			Uri u = mContext.getContentResolver().insert(uri, values);
			
			result = true;
		} finally {
			// TODo
		}
		
		return result;
	}
		
	// ��Ӹ���
	private boolean setPartFileData(Uri uri, byte[] data) throws IOException {
		boolean result = false;
		try{
			OutputStream out = mContext.getContentResolver().openOutputStream(uri);			
			out.write(data);
			if(null != out) {
				result = true;
				out.close();
			}
		} finally {
			// TODO
		}
		
		return result;
	}
	
	/**
	 * Update Email
	 * 
	 * @param email
	 * @return
	 */
	public boolean updateEmail(EmailInfo email) {
		boolean result = false;

		try {
			ContentValues values = new ContentValues();
			values.put(MAIL_UIDL, CheckStringNull(email.getUidl()));
			values.put(MAIL_MESSAGE_BOX, email.getType());
			values.put(MAIL_FROM, email.getFrom());
			values.put(MAIL_FROM_DISPLAY, email.getFromDisplay());
			values.put(MAIL_TO, email.getTo());
			values.put(MAIL_TO_DISPLAY, email.getToDisplay());
			values.put(MAIL_CC, "");
			values.put(MAIL_BCC, "");
			values.put(MAIL_REPLY_TO, CheckStringNull(email.getReplyTo()));
			values.put(MAIL_DATE, email.getDate());
			values.put(MAIL_SUBJECT, email.getSubject());
			values.put(MAIL_HEADERS, "");
			values.put(MAIL_FLAGS, email.getFlags());
			values.put(MAIL_READ_SIZE, email.getReadSize());
			values.put(MAIL_TOTAL_SIZE, email.getTotalSize());
			values.put(MAIL_ACCOUNT, email.getAccountID());
			
			String where = "_id=" + email.getId();

			int res = mContext.getContentResolver().update(MESSAGE_URI, values, where, null);
			if(res != 0) {
				result = true;
			}

			// Update Part
			updatePart(email.getAccountID(), email.getId());
		
		} catch (Exception e) {
		  e.printStackTrace();
		} finally {
			// ToDo
		}
		return result;
	}
	
	/**
	 * Update Part
	 * 
	 * @param accountId
	 * @param messageId
	 * @return
	 */
	public boolean updatePart(Integer accountId, Integer messageId) {
		boolean result = false;
		try {
			Uri uri = getPartUri(accountId, messageId);
			
			ContentValues values = new ContentValues();
			values.put(PART_ACCOUNT, accountId);
			values.put(PART_MESSAGE, messageId);
			values.put(PART_C_ID, "");
			values.put(PART_SEQ, 0);
			values.put(PART_MIME_TYPE, "text/plain");
			values.put(PART_CHAR_SET, "UTF-8");
			values.put(PART_ENCODING, "");
			values.put(PART_FILE_NAME, "");
			values.put(PART_SIZE, 0);
			values.put(PART_FLAGS, 0);
			values.put(PART_DATA,
					"/data/data/oms.mail.provider/app_mail/PART_1253779441045");

			int res = mContext.getContentResolver().update(uri, values, null, null);
			if(res != 0) {
				result = true;
			}
		} finally {
			// TODo
		}
		
		return result;
	}
	
	// ����ַ��Ƿ�Ϊnull�������򷵻ؿ��ַ����򷵻�ԭ4��ֵ
	private String CheckStringNull(String value) {
		if (null == value) {
			return "";
		} else {
			return value;
		}
	}
}
