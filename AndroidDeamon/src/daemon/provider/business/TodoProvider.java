package daemon.provider.business;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;


import daemon.model.TodoInfo;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;

public class TodoProvider implements IProvider {

	Context mContext = null;
	private Uri TODO_URI = null;

	public static final String ID = "_id";
	public static final String TITLE = "title";
	public static final String STATE = "state";
	public static final String ALARM = "alarm";
	public static final String BEGIN = "begin";
	public static final String OVER = "over";
	public static final String PRIORITY = "priority";
	public static final String NOTES = "notes";
	public static final String REMINDER = "reminder";
	public static final String CREATED = "created";
	public static final String MODIFIED = "modified";

	public TodoProvider(Context context) {
		mContext = context;
		TODO_URI = Uri.parse("content://todo/todo_tasks");
	}

	@Override
	public void execute(ProviderExecuteContext ctx) {
		ByteReader reader = ctx.getByteReader();
		ByteWriter writer = ctx.getByteWriter();
		int action = reader.ReadInt32();
		writer.Write(action);
		switch (action) {
		case 1:// ��ѯ�������
			Query(writer);
			break;
		case 2:// ����
			Add(reader, writer);
			break;
		case 3:// ɾ��
			Delete(reader, writer);
			break;
		case 4:// �޸�
			Update(reader, writer);
			break;
		case 5:
			DeleteAll(reader, writer);
			break;
		}
	}

	private void Query(ByteWriter writer) {
		List<TodoInfo> todoLst = queryTodoList();

		int count = todoLst.size();
		writer.Write(count);
		for (TodoInfo todo : todoLst) {
			todo.Write(writer);
		}
	}

	// ����
	private void Add(ByteReader reader, ByteWriter writer) {
		TodoInfo todo = new TodoInfo();
		todo.Read(reader);

		boolean result = AddTodoList(todo);
		writer.Write(result);
		if (result) {
			writer.Write(todo.getId());
		}
	}

	// ɾ��
	private void Delete(ByteReader reader, ByteWriter writer) {
		Integer id = reader.ReadInt32();
		boolean result = DeleteTodoInfo(id);
		writer.Write(result);
	}

	// ����
	private void Update(ByteReader reader, ByteWriter writer) {
		TodoInfo todo = new TodoInfo();
		todo.Read(reader);
		if (UpdateTodoInfo(todo)) {
			writer.Write(true);
		} else {
			writer.Write(false);
		}
	}

	// 删除所有代办事项
	private void DeleteAll(ByteReader reader, ByteWriter writer) {
		boolean result = DeleteAll();
		writer.Write(result);
	}

	public ArrayList<TodoInfo> queryTodoList() {

		ArrayList<TodoInfo> result = new ArrayList<TodoInfo>();
		Cursor cur = mContext.getContentResolver().query(TODO_URI, null, null,
				null, null);
		try {			
			if (null != cur && cur.moveToFirst()) {

				result = new ArrayList<TodoInfo>();
				TodoInfo todoinfo = null;

				while (!cur.isAfterLast()) {
					todoinfo = new TodoInfo();
					todoinfo.setId(cur.getInt(cur.getColumnIndexOrThrow(ID)));
					todoinfo.setTitle(cur.getString(cur
							.getColumnIndexOrThrow(TITLE)));
					todoinfo.setState(cur.getInt(cur
							.getColumnIndexOrThrow(STATE)));
					todoinfo.setAlarm(cur.getInt(cur
							.getColumnIndexOrThrow(ALARM)));
					todoinfo.setBegin(cur.getLong(cur
							.getColumnIndexOrThrow(BEGIN)));
					todoinfo.setOver(cur.getLong(cur
							.getColumnIndexOrThrow(OVER)));
					todoinfo.setPriority(cur.getInt(cur
							.getColumnIndexOrThrow(PRIORITY)));
					todoinfo.setNotes(cur.getString(cur
							.getColumnIndexOrThrow(NOTES)));
					todoinfo.setReminder(cur.getInt(cur
							.getColumnIndexOrThrow(REMINDER)));
					todoinfo.setCreated(cur.getLong(cur
							.getColumnIndexOrThrow(CREATED)));
					todoinfo.setModified(cur.getLong(cur
							.getColumnIndexOrThrow(MODIFIED)));					

					result.add(todoinfo);
					cur.moveToNext();
				}
			}
		} catch(Exception e){
			e.printStackTrace();
		}finally {
			if (null != cur) {
				cur.close();
			}
		}

		return result;
	}

	// ����������
	public boolean AddTodoList(TodoInfo todo) {
		boolean result = false;

		try {
			ContentValues values = new ContentValues();
			values.put(TITLE, todo.getTitle());
			values.put(STATE, todo.getState());
			values.put(ALARM, todo.getAlarm());
			values.put(BEGIN, todo.getBegin());
			values.put(OVER, todo.getOver());
			values.put(PRIORITY, todo.getPriority());
			values.put(NOTES, todo.getNotes());
			values.put(REMINDER, todo.getReminder());
			values.put(CREATED, todo.getCreated());
			values.put(MODIFIED, todo.getModified());

			Uri inserted = mContext.getContentResolver().insert(TODO_URI,
					values);
			Integer id = Integer.parseInt(inserted.getLastPathSegment());
			todo.setId(id);
			result = true;
		} catch(Exception e){
			e.printStackTrace();
		}

		return result;
	}

	// ɾ��������
	public boolean DeleteTodoInfo(Integer id) {
		boolean result = false;

		Uri uri = ContentUris.withAppendedId(TODO_URI, id);
		if (this.mContext.getContentResolver().delete(uri, null, null) != 0) {
			result = true;
		}

		return result;
	}

	// ���´������
	public boolean UpdateTodoInfo(TodoInfo todo) {
		boolean result = false;
		ContentValues values = new ContentValues();
		values.put(TITLE, todo.getTitle());
		values.put(STATE, todo.getState());
		values.put(ALARM, todo.getAlarm());
		values.put(BEGIN, todo.getBegin());
		values.put(OVER, todo.getOver());
		values.put(PRIORITY, todo.getPriority());
		values.put(NOTES, todo.getNotes());
		values.put(REMINDER, todo.getReminder());
		values.put(CREATED, todo.getCreated());
		values.put(MODIFIED, todo.getModified());

		Uri uri = ContentUris.withAppendedId(TODO_URI, todo.getId());
		if (this.mContext.getContentResolver().update(uri, values, null, null) == 0) {
			result = false;
		} else {
			result = true;
		}

		return result;
	}

	// 删除所有代办事项
	public boolean DeleteAll() {
		boolean result = false;

		this.mContext.getContentResolver().delete(TODO_URI, null, null);

		result = true;

		return result;
	}

	@Override
	public int getBusiness() {
		// TODO Auto-generated method stub
		return 4;
	}

}
