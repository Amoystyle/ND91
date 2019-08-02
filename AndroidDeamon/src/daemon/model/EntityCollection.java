package daemon.model;

import java.util.ArrayList;


import daemon.logger.LogCenter;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.IBaseEntity;


public class EntityCollection<T> extends ArrayList<T> implements IBaseEntity {
	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;

	@SuppressWarnings("unchecked")
	private Class clazz;

	public EntityCollection(String className) {
		try {
			this.clazz = Class.forName(className);
		} catch (ClassNotFoundException e) {
			LogCenter.error(e.getMessage(), "");
		}
	}

	@Override
	public Integer GetAction() {
		return 0;
	}

	@Override
	public void Read(ByteReader reader) {
		int count = reader.ReadInt32();
		for (int i = 0; i < count; i++) {
			try {
				IBaseEntity entity = (IBaseEntity) clazz.newInstance();
				entity.Read(reader);
			} catch (IllegalAccessException e) {
				LogCenter.error(e.getMessage(), "");
			} catch (InstantiationException e) {
				LogCenter.error(e.getMessage(), "");
			}
		}
	}

	@Override
	public void Write(ByteWriter writer) {
		int count = this.size();
		writer.Write(count);
		for (int i = 0; i < count; i++) {
			T entity = this.get(i);
			if (entity instanceof IBaseEntity) {
				((IBaseEntity) entity).Write(writer);
			}
		}
	}
}

