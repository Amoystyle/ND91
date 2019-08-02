package daemon.model.pimOms;

public enum ActionState {
	UnChanged(0), Added(1), Edited(2), Deleted(3);
	
	private int m_value;
	
	private ActionState(int value){
		this.m_value = value;
	}
	
	public static ActionState FromInt(int value){
		ActionState state = ActionState.UnChanged;
		
		switch(value){
		case 0:
			state = ActionState.UnChanged;
			break;
		case 1:
			state = ActionState.Added;
			break;
		case 2:
			state = ActionState.Edited;
			break;
		case 3:
			state = ActionState.Deleted;
			break;
		}
		
		return state;
	}
}
