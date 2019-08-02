package daemon.model;

import java.util.ArrayList;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

/**
 * 
 * <p> java bean object for the table  email account table.
 * @version1.0
 *
 */
public class EmailAccount extends ModuleCategory{
	private Integer id;
	private String  userName;
	private String  password;
	private String  inserver;
	private Integer inport;
	private String  outserver;
	private Integer outport;
	private String  fromname;
	private String  fromaddress;
	private String  replytoname;
	private String  replytoaddress;
	private String  copyaddress;
	private Long    nextfetchtime;
	private String  sms;
	private String  apn;
	private String  companyid;
	private String  signature;
	private Integer flags;
	private ArrayList<String> tracks;
	private ArrayList<EmailInfo> emails;
	
	public EmailAccount(){
		tracks = new ArrayList<String>();
		emails = new ArrayList<EmailInfo>();
	}
	
	public Integer getId(){
		return id;		
	}
	public void setId(Integer value){
		id = value;
	}
	
	public String getUserName(){
		return userName;
	}
	public void setUserName(String value){
		userName = value;
	}
	
	public String getPassword(){
		return password;
	}
	public void setPassword(String value){
		password = value;
	}
	
	public String getInServer(){
		return inserver;
	}
	public void setInServer(String value){
		inserver = value;
	}
	
	public Integer getInPort(){
		return inport;
	}
	public void setInPort(Integer value){
		inport = value;
	}
	
	public String getOutServer(){
		return outserver;
	}
	public void setOutServer(String value){
		outserver = value;
	}
	
	public Integer getOutPort(){
		return outport;
	}
	public void setOutPort(Integer value){
		outport = value;
	}
	
	public String getFromName(){
		return fromname;
	}
	public void setFromName(String value){
		fromname = value;
	}
	
	public String getFromAddress(){
		return fromaddress;
	}
	public void setFromAddress(String value){
		fromaddress = value;
	}
	
	public String getReplyToName(){
		return replytoname;
	}
	public void setReplyToName(String value){
		replytoname = value;
	}
	
	public String getReplyToAddress(){
		return replytoaddress;
	}
	public void setReplyToAddress(String value){
		replytoaddress = value;
	}
	
	public String getCopyAddress(){
		return copyaddress;
	}
	public void setCopyAddress(String value){
		copyaddress = value;
	}
	
	public Long getNextFetchTime(){
		return nextfetchtime;
	}
	public void setNextFetchTime(Long value){
		nextfetchtime = value;
	}
	
	public String getSms(){
		return sms;
	}
	public void setSms(String value){
		sms = value;
	}
	
	public String getApn(){
		return apn;
	}
	public void setApn(String value){
		apn = value;
	}
	
	public String getCompanyId(){
		return companyid;
	}
	public void setCompanyId(String value){
		companyid = value;
	}
	
	public String getSignature(){
		return signature;
	}
	public void setSignature(String value){
		signature = value;
	}
	
	public Integer getFlags(){
		return flags;
	}
	public void setFlags(Integer value){
		flags = value;
	}
	
	public ArrayList<String> getTracks(){
		return tracks;
	}
	
	public ArrayList<EmailInfo> getEmails(){
		return emails;
	}
	
	/**
	 * �ӽ�������������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		id = reader.ReadInt32();
		userName = reader.ReadString();
		password = reader.ReadString();
		inserver = reader.ReadString();
		inport = reader.ReadInt32();
		outserver = reader.ReadString();
		outport = reader.ReadInt32();
		fromname = reader.ReadString();
		fromaddress = reader.ReadString();
		replytoname = reader.ReadString();
		replytoaddress = reader.ReadString();
		copyaddress = reader.ReadString();
		nextfetchtime = reader.ReadInt64();
		sms = reader.ReadString();
		apn = reader.ReadString();
		companyid = reader.ReadString();
		signature = reader.ReadString();
		flags = reader.ReadInt32();
	}

	/**
	 * ͨ����������л���Ա����
	 * 
	 * @param writer������
	 */
	public void Write(ByteWriter writer) {
		writer.Write(id);
		writer.Write(userName);
		writer.Write(password);
		writer.Write(inserver);
		writer.Write(inport);
		writer.Write(outserver);
		writer.Write(outport);
		writer.Write(fromname);
		writer.Write(fromaddress);
		writer.Write(replytoname);
		writer.Write(replytoaddress);
		writer.Write(copyaddress);
		writer.Write(nextfetchtime);
		writer.Write(sms);
		writer.Write(apn);
		writer.Write(companyid);
		writer.Write(signature);
		writer.Write(flags);
	}
}
