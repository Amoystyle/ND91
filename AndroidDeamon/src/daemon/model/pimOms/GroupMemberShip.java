package daemon.model.pimOms;

import daemon.protocol.ByteReader;

public class GroupMemberShip extends MethodBase{
	
	private Integer groupId;	
	
	public GroupMemberShip(){
		setKind(MethodKind.GroupMembership);
		groupId = 0;
	}
	
	public Integer getGroupId(){
		return groupId;
	}
	public void setGroupId(Integer value){
		groupId = value;
	}
	
	
	
	/**
	 * �ӽ�����������򻯳�Ա
	 * 
	 * @param reader
	 *            ������
	 */
	public void Read(ByteReader reader) {
		BaseFromStream(reader);
		
		groupId = reader.ReadInt32();		
	}	
}
