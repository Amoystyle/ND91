package daemon.provider.business;

import java.util.ArrayList;
import java.util.List;

import android.content.ContentUris;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;


import daemon.model.notepad.NotepadHeadInfo;
import daemon.model.notepad.NotepadInfo;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;

public class NotepadProvider implements IProvider {

	private String NOTE_STR = "content://com.android.provider.NotePad/notes";
	private Uri NOTE_URI = Uri.parse(NOTE_STR);
	private String ID = "_id";
	private String TITLE = "title";
	private String NOTE = "note";
	private String CREATED = "created";
	private String MODIFIED = "modified";

	private Context mContext;

	public NotepadProvider(Context ctx) {
		mContext = ctx;
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();
		int action = reader.ReadInt32();
		writer.Write(action);

		switch (action) {
		case 1: // Query All Notes
			queryNotepads(writer);
			break;
		case 2: // Query All Head
			queryHeadNotes(writer);
			break;
		case 3: // Query One Note By Id
			queryNoteById(reader, writer);
			break;
		case 4: // Add
			addNotepad(reader, writer);
			break;
		case 5: // Delete
			delNotepad(reader, writer);
			break;
		case 6: // Update
			updateNotepad(reader, writer);
			break;
		default:
			break;
		}
	}

	@Override
	public int getBusiness() {
		return 10;
	}

	public void queryNotepads(ByteWriter writer) {
		List<NotepadInfo> noteList = queryAllNotepads();

		int noteSize = noteList.size();
		writer.Write(noteSize);

		for (NotepadInfo notepad : noteList) {
			writer.Write(notepad.getHead().getId());
			writer.Write(notepad.getHead().getTitle());
			writer.Write(notepad.getNote());
			writer.Write(notepad.getCreated());
			writer.Write(notepad.getModified());
		}
	}

	/**
	 * Query All Head(id and title)
	 * 
	 * @param writer
	 */
	public void queryHeadNotes(ByteWriter writer) {
		List<NotepadHeadInfo> headList = queryAllNoteHaeds();

		int headSize = headList.size();
		writer.Write(headSize);

		for (NotepadHeadInfo headInfo : headList) {
			writer.Write(headInfo.getId());
			writer.Write(headInfo.getTitle());
		}
	}

	/**
	 * Query One Note By ID
	 * 
	 * @param reader
	 * @param writer
	 */
	public void queryNoteById(ByteReader reader, ByteWriter writer) {
		Integer id = reader.ReadInt32();
		NotepadInfo notepad = queryNotepadById(id);

		writer.Write(id);
		writer.Write(notepad.getHead().getTitle());
		writer.Write(notepad.getNote());
		writer.Write(notepad.getCreated());
		writer.Write(notepad.getModified());
	}

	/**
	 * Add one note
	 * 
	 * @param reader
	 * @param writer
	 */
	public void addNotepad(ByteReader reader, ByteWriter writer) {
		NotepadInfo notepad = new NotepadInfo();
		notepad.Read(reader);

		boolean addSuccess = addNotepad(notepad);
		writer.Write(addSuccess);
		if (addSuccess) {
			writer.Write(notepad.getHead().getId());
		}
	}

	/**
	 * Delete one Note by Id
	 * 
	 * @param reader
	 * @param writer
	 */
	public void delNotepad(ByteReader reader, ByteWriter writer) {
		int count = reader.ReadInt32();
		boolean delSuccess = true;
		if (count > 0) {
			for (int i = 0; i < count; i++) {
				Integer id = reader.ReadInt32();
				delSuccess = deleteNotepad(id);
				if (delSuccess == false)
					break;
			}
		}
		writer.Write(delSuccess);
	}

	/**
	 * Update one Note
	 * 
	 * @param reader
	 * @param writer
	 */
	public void updateNotepad(ByteReader reader, ByteWriter writer) {
		NotepadInfo notepad = new NotepadInfo();
		notepad.Read(reader);

		boolean updateSuccess = updateNotepad(notepad);
		writer.Write(updateSuccess);
	}

	public ArrayList<NotepadInfo> queryAllNotepads() {
		ArrayList<NotepadInfo> result = new ArrayList<NotepadInfo>();
		Cursor note_cursor = null;
		note_cursor = mContext.getContentResolver().query(NOTE_URI, null, null,
				null, null);

		try {
			if ((null != note_cursor) && (note_cursor.moveToFirst())) {
				while (!note_cursor.isAfterLast()) {
					NotepadInfo notepad = new NotepadInfo();
					NotepadHeadInfo head = new NotepadHeadInfo();

					head.setId(note_cursor.getInt(note_cursor
							.getColumnIndexOrThrow(ID)));
					head.setTitle(note_cursor.getString(note_cursor
							.getColumnIndexOrThrow(TITLE)));

					notepad.setHead(head);

					notepad.setNote(note_cursor.getString(note_cursor
							.getColumnIndexOrThrow(NOTE)));
					notepad.setCreated(note_cursor.getLong(note_cursor
							.getColumnIndexOrThrow(CREATED)));
					notepad.setModified(note_cursor.getLong(note_cursor
							.getColumnIndexOrThrow(MODIFIED)));

					result.add(notepad);
					note_cursor.moveToNext();
				}
			}
		} finally {
			if (null != note_cursor) {
				note_cursor.close();
			}
		}

		return result;
	}

	/**
	 * Query All Head Notepad(id and title)
	 * 
	 * @return ArrayList<NotepadHeadInfo>: id and title
	 */
	public ArrayList<NotepadHeadInfo> queryAllNoteHaeds() {
		ArrayList<NotepadHeadInfo> result = new ArrayList<NotepadHeadInfo>();
		Cursor head_cursor = mContext.getContentResolver().query(NOTE_URI,
				null, null, null, null);

		try {
			if ((null != head_cursor) && (head_cursor.moveToFirst())) {

				while (!head_cursor.isAfterLast()) {
					NotepadHeadInfo head = new NotepadHeadInfo();
					head.setId(head_cursor.getInt(head_cursor
							.getColumnIndexOrThrow(ID)));
					head.setTitle(head_cursor.getString(head_cursor
							.getColumnIndexOrThrow(TITLE)));

					result.add(head);
					head_cursor.moveToNext();
				}
			}
		} finally {
			if (null != head_cursor) {
				head_cursor.close();
			}
		}
		return result;
	}

	/**
	 * Query One Noetpad By Id
	 * 
	 * @param id
	 * @return NotepadInfo
	 */
	public NotepadInfo queryNotepadById(Integer id) {
		NotepadInfo note = new NotepadInfo();
		NotepadHeadInfo head = new NotepadHeadInfo();
		Uri uri = ContentUris.withAppendedId(NOTE_URI, id);
		Cursor note_cursor = mContext.getContentResolver().query(uri, null,
				null, null, null);
		try {
			if ((null != note_cursor) && (note_cursor.moveToFirst())) {

				while (!note_cursor.isAfterLast()) {
					head.setId(id);
					head.setTitle(note_cursor.getString(note_cursor
							.getColumnIndexOrThrow(TITLE)));
					note.setHead(head);

					note.setNote(note_cursor.getString(note_cursor
							.getColumnIndexOrThrow(NOTE)));
					note.setCreated(note_cursor.getLong(note_cursor
							.getColumnIndexOrThrow(CREATED)));
					note.setModified(note_cursor.getLong(note_cursor
							.getColumnIndexOrThrow(MODIFIED)));

					note_cursor.moveToNext();
				}
			}
		} finally {
			if (null != note_cursor) {
				note_cursor.close();
			}
		}

		return note;
	}

	/**
	 * Add one notepad
	 * 
	 * @param NotepadInfo
	 *            : notepad
	 * @return boolean
	 */
	public boolean addNotepad(NotepadInfo notepad) {
		boolean result = false;

		try {
			ContentValues values = new ContentValues();
			values.put(TITLE, notepad.getHead().getTitle());
			values.put(NOTE, notepad.getNote());
			values.put(CREATED, notepad.getCreated());
			values.put(MODIFIED, notepad.getModified());

			Uri inserted = mContext.getContentResolver().insert(NOTE_URI,
					values);
			Integer id = Integer.parseInt(inserted.getLastPathSegment());
			notepad.getHead().setId(id);
			result = true;
		} finally {
			// TODO
		}

		return result;
	}

	/**
	 * Delete one notepad with id
	 * 
	 * @param Integer
	 *            : id
	 * @return boolean
	 */
	public boolean deleteNotepad(Integer id) {
		boolean result = false;
		try {
			Uri uri = ContentUris.withAppendedId(NOTE_URI, id);
			if (this.mContext.getContentResolver().delete(uri, null, null) != 0) {
				result = true;
			}
		} finally {
			// TODO
		}

		return result;
	}

	/**
	 * Update one notepad with id
	 * 
	 * @param NotepadInfo
	 *            : notepad
	 * @return boolean
	 */
	public boolean updateNotepad(NotepadInfo notepad) {
		boolean result = false;
		try {
			ContentValues value = new ContentValues();
			value.put(TITLE, notepad.getHead().getTitle());
			value.put(NOTE, notepad.getNote());
			value.put(CREATED, notepad.getCreated());
			value.put(MODIFIED, notepad.getModified());

			Uri uri = ContentUris.withAppendedId(NOTE_URI, notepad.getHead()
					.getId());
			if (this.mContext.getContentResolver().update(uri, value, null,
					null) != 0) {
				result = true;
			}
		} finally {
			// TODO
		}
		return result;
	}
}
