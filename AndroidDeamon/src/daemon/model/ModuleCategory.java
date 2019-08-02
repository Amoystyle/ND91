package daemon.model;

import daemon.protocol.IBaseEntity;

/**
 * <p>
 * Title: ModuleCategory
 * </p>
 * <p>
 * Description: ModuleCategory
 * </p>
 * <p>
 * Copyright: Copyright (c) 2009
 * </p>
 * <p>
 * Company: nd
 * </p>
 * 
 * @author: Jason.Lee
 * @version: 1.0
 */

public abstract class ModuleCategory implements IBaseEntity {
	protected Integer mAction;

	public ModuleCategory() {

	}

	public ModuleCategory(Integer action) {
		mAction = action;
	}

	public Integer GetAction() {
		return mAction;
	}

	public void SetAction(Integer action) {
		mAction = action;
	}
}
