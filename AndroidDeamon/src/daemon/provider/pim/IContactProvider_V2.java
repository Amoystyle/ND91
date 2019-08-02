package daemon.provider.pim;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;

public interface IContactProvider_V2 {
	
	void LoadContacts(ByteWriter writer);
	
	void AddContacts(ByteReader reader, ByteWriter writer, boolean bplugin);
	
	void UpdateContact(ByteReader reader, ByteWriter writer);
	
	void GetPhoto(ByteReader reader, ByteWriter writer);
	
	void DeleteContacts(ByteReader reader, ByteWriter writer);
	
	void AddGroup(ByteReader reader, ByteWriter writer);
	
	void UpdateGroup(ByteReader reader, ByteWriter writer);
	
	void DeleteGroup(ByteReader reader, ByteWriter writer);
	
	void SetFavorite(ByteReader reader, ByteWriter writer);
	
	void AddToGroup(ByteReader reader, ByteWriter writer);
	
	void RemoveFromGroup(ByteReader reader, ByteWriter writer);
	
	void GetPeopleCount(ByteWriter writer);
	
    //加载联系人信息，分组信息带IsSystem字段
	void LoadContactsWithGroupSystem(ByteWriter writer);

	void LoadContactsEx(ByteWriter writer);
}
