package daemon.model.pimOms;

public enum MethodKind {
	Email(1), PostalAddress(2), IM(3), NickName(4), Website(5), Note(6), Phone(
			7), Organization(8), Name(9), Photo(10), GroupMembership(11), People(
			12), Group(13), EndFlag(-1);

	private int m_value;

	private MethodKind(int value) {
		this.m_value = value;
	}

	public int getValue() {
		return this.m_value;
	}

	public static MethodKind FromInt(int value) {
		MethodKind result = MethodKind.Email;
		
		switch (value) {
		case 1:
			result = MethodKind.Email;	
			break;
		case 2:
			result = MethodKind.PostalAddress;	
			break;
		case 3:
			result = MethodKind.IM;	
			break;
		case 4:
			result = MethodKind.NickName;	
			break;
		case 5:
			result = MethodKind.Website;	
			break;
		case 6:
			result = MethodKind.Note;	
			break;
		case 7:
			result = MethodKind.Phone;
			break;
		case 8:
			result = MethodKind.Organization;	
			break;
		case 9:
			result = MethodKind.Name;	
			break;
		case 10:
			result = MethodKind.Photo;	
			break;
		case 11:
			result = MethodKind.GroupMembership;	
			break;
		case 12:
			result = MethodKind.People;	
			break;
		case 13:
			result = MethodKind.Group;	
			break;
		case -1:
			result = MethodKind.EndFlag;	
			break;
		}
		
		return result;
	}
}
