package daemon.model.pim15;

import daemon.protocol.ByteReader;

public abstract class MethodBase {
	private ActionState actionState = ActionState.UnChanged;
	private Integer id = 0;	
	private MethodKind kind = MethodKind.Phone;
	private Integer contactId = 0;		
	
	public ActionState getActionState(){
		return actionState;
	}
	public void setActionState(ActionState value){
		actionState = value;
	}
	
	public Integer getId(){
		return id;
	}
	public void setId(Integer value){
		id = value;
	}
	
	public MethodKind getKind(){
		return kind;
	}
	public void setKind(MethodKind value){
		kind = value;
	}
	
	public Integer getContactId(){
		return contactId;
	}
	public void setContactId(Integer value){
		contactId = value;
	}
	
	public void BaseFromStream(ByteReader reader){
		actionState = ActionState.FromInt(reader.ReadInt32());
		id = reader.ReadInt32();			
		contactId = reader.ReadInt32();		
	}
	
	public abstract void Read(ByteReader reader);
}
