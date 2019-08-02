#include "stdafx.h"
#include "IosCreateDBSQL.h"

IosCreateDBSQL::IosCreateDBSQL(void)
{
}

IosCreateDBSQL::~IosCreateDBSQL(void)
{
}

string IosCreateDBSQL::CreateCallDataDBSQL()
{
    return "Drop Table IF Exists [call];\
        Create  TABLE [call]([ROWID] INTEGER PRIMARY KEY AUTOINCREMENT,[address] TEXT,[date] INTEGER,[duration] INTEGER,[flags] INTEGER,[id] INTEGER);\
        CREATE INDEX date_index on call(date);\
        CREATE TRIGGER timer_last_trigger INSERT ON call WHEN (NEW.duration != 0) BEGIN UPDATE _SqliteDatabaseProperties SET value = NEW.duration WHERE key = 'timer_last'; END;\
        CREATE TRIGGER timer_outgoing_trigger INSERT ON call WHEN (NEW.flags & (1 << 0) == 1) BEGIN UPDATE _SqliteDatabaseProperties SET value = (SELECT NEW.duration + (SELECT value FROM _SqliteDatabaseProperties WHERE key = 'timer_outgoing')) WHERE key = 'timer_outgoing'; END;\
        CREATE TRIGGER timer_incoming_trigger INSERT ON call WHEN (NEW.flags & (1 << 0) == 0) BEGIN UPDATE _SqliteDatabaseProperties SET value = (SELECT NEW.duration + (SELECT value FROM _SqliteDatabaseProperties WHERE key = 'timer_incoming')) WHERE key = 'timer_incoming'; END;\
        CREATE TRIGGER timer_all_trigger INSERT ON call WHEN (NEW.duration != 0) BEGIN UPDATE _SqliteDatabaseProperties SET value = (SELECT NEW.duration + (SELECT value FROM _SqliteDatabaseProperties WHERE key = 'timer_all')) WHERE key = 'timer_all'; END;CREATE TRIGGER timer_lifetime_trigger INSERT ON call WHEN (NEW.duration != 0) BEGIN UPDATE _SqliteDatabaseProperties SET value = (SELECT NEW.duration + (SELECT value FROM _SqliteDatabaseProperties WHERE key = 'timer_lifetime')) WHERE key = 'timer_lifetime'; END;";
}

string IosCreateDBSQL::CreateBookMarkDBSQL()
{
	return "DROP TABLE IF EXISTS [group_member];\
		   CREATE TABLE 'bookmarks'([id] INTEGER PRIMARY KEY,\
		   [special_id] INTEGER DEFAULT 0,\
		   [parent] INTEGER,\
		   [type] INTEGER,\
		   [title] TEXT,\
		   [url] TEXT COLLATE NOCASE,\
		   [num_children] INTEGER DEFAULT 0,\
		   [editable] INTEGER DEFAULT 1,\
		   [deletable] INTEGER DEFAULT 1,\
		   [order_index] INTEGER NOT NULL,\
		   [external_uuid] TEXT)";
}


string IosCreateDBSQL::CreateMessageDBSQL(int iProductVersion)
{
    if (iProductVersion < 113)
    {
        return "DROP TABLE IF EXISTS [message];\
            CREATE TABLE [message] (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, address TEXT, date INTEGER, text TEXT, flags INTEGER, replace INTEGER, svc_center TEXT)";
    }
    else if((iProductVersion < 500))
    {
        return "DROP TABLE IF EXISTS [group_member];\
                 CREATE TABLE [group_member] (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, group_id INTEGER, address TEXT);\
                 DROP TABLE IF EXISTS [message];\
                 CREATE TABLE [message] (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, address TEXT, date INTEGER, text TEXT, flags INTEGER, replace INTEGER, svc_center TEXT, group_id INTEGER, association_id INTEGER, height INTEGER, UIFlags INTEGER, version INTEGER);\
                 DROP TABLE IF EXISTS [msg_group];\
                 CREATE TABLE [msg_group] (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, type INTEGER, newest_message INTEGER, unread_count INTEGER);\
                 Drop Trigger IF EXISTS [delete_message];\
                 Create  Trigger [delete_message] AFTER DELETE On [message] when not read(old.flags) begin  update msg_group set unread_count = (select unread_count from msg_group where rowid = old.group_id) - 1 where rowid = old.group_id; end;\
                 Drop Trigger IF EXISTS [delete_newest_message];\
                 Create  Trigger [delete_newest_message] AFTER DELETE On [message] WHEN old.ROWID = (SELECT newest_message FROM msg_group WHERE ROWID = old.group_id) BEGIN UPDATE msg_group SET newest_message = (SELECT ROWID FROM message WHERE group_id = old.group_id AND ROWID = (SELECT max(ROWID) FROM message WHERE group_id = old.group_id)) WHERE ROWID = old.group_id; END;\
                 Drop Trigger IF EXISTS [insert_newest_message];\
                 Create  Trigger [insert_newest_message] AFTER INSERT On [message] WHEN new.ROWID >= IFNULL((SELECT MAX(ROWID) FROM message WHERE message.group_id = new.group_id), 0) BEGIN UPDATE msg_group SET newest_message = new.ROWID WHERE ROWID = new.group_id; END;\
                 Drop Trigger IF EXISTS [insert_unread_message];\
                 Create Trigger [insert_unread_message] AFTER INSERT On [message] WHEN NOT read(new.flags) BEGIN UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) + 1 WHERE ROWID = new.group_id; END;\
                 Drop Trigger IF EXISTS [mark_message_read];\
                 Create  Trigger [mark_message_read] AFTER UPDATE On [message] WHEN NOT read(old.flags) AND read(new.flags) BEGIN  UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) - 1 WHERE ROWID = new.group_id; END;\
                 Drop Trigger IF EXISTS [mark_message_unread];\
                 Create  Trigger [mark_message_unread] AFTER UPDATE On [message] WHEN read(old.flags) AND NOT read(new.flags) BEGIN  UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) + 1 WHERE ROWID = new.group_id;  END;\
                 Drop Index If Exists [message_flags_index];\
                 CREATE  INDEX [message_flags_index] On [message] ([flags] );\
                 Drop Index If Exists [message_group_index];\
                 CREATE  INDEX [message_group_index] On [message] ([group_id] ,[ROWID] );";

    }
	else
	{

	return "DROP TABLE IF EXISTS [group_member];\
			CREATE TABLE group_member (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, group_id INTEGER, address TEXT, country TEXT);\
			DROP TABLE IF EXISTS [madrid_attachment];\
			CREATE TABLE madrid_attachment (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, attachment_guid TEXT, created_date INTEGER, start_date INTEGER, filename TEXT, uti_type TEXT, mime_type TEXT, transfer_state INTEGER, is_incoming INTEGER, message_id INTEGER);\
			Drop Index If Exists [madrid_attachment_filename_index];\
			CREATE INDEX madrid_attachment_filename_index ON madrid_attachment(filename);\
			Drop Index If Exists [madrid_attachment_guid_index];\
			CREATE INDEX madrid_attachment_guid_index ON madrid_attachment(attachment_guid);\
			Drop Index If Exists [madrid_attachment_message_index];\
			CREATE INDEX madrid_attachment_message_index ON madrid_attachment(message_id);\
			DROP TABLE IF EXISTS [madrid_chat];\
			CREATE TABLE madrid_chat (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, style INTEGER, state INTEGER, account_id TEXT, properties BLOB, chat_identifier TEXT, service_name TEXT, guid TEXT, room_name TEXT, account_login TEXT, participants BLOB);\
			Drop Index If Exists [madrid_chat_account_id_index];\
			CREATE INDEX madrid_chat_account_id_index ON madrid_chat(account_id);\
			Drop Index If Exists [madrid_chat_account_login_index];\
			CREATE INDEX madrid_chat_account_login_index ON madrid_chat(account_login);\
			Drop Index If Exists [madrid_chat_chat_identifier_index];\
			CREATE INDEX madrid_chat_chat_identifier_index ON madrid_chat(chat_identifier);\
			Drop Index If Exists [madrid_chat_guid_index];\
			CREATE INDEX madrid_chat_guid_index ON madrid_chat(guid);\
			Drop Index If Exists [madrid_chat_room_name_index];\
			CREATE INDEX madrid_chat_room_name_index ON madrid_chat(room_name);\
			Drop Index If Exists [madrid_chat_service_name_index];\
			CREATE INDEX madrid_chat_service_name_index ON madrid_chat(service_name);\
			Drop Index If Exists [madrid_chat_state_index];\
			CREATE INDEX madrid_chat_state_index ON madrid_chat(state);\
			Drop Index If Exists [madrid_chat_style_index];\
			CREATE INDEX madrid_chat_style_index ON madrid_chat(style);\
			DROP TABLE IF EXISTS [message];\
			CREATE TABLE message (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, address TEXT, date INTEGER, \
								  text TEXT, flags INTEGER, replace INTEGER, svc_center TEXT, group_id INTEGER,\
								  association_id INTEGER, height INTEGER, UIFlags INTEGER, version INTEGER, \
								  subject TEXT, country TEXT, headers BLOB, recipients BLOB, read INTEGER, \
								  madrid_attributedBody BLOB, madrid_handle TEXT, madrid_version INTEGER,\
								  madrid_guid TEXT, madrid_type INTEGER, madrid_roomname TEXT, madrid_service TEXT,\
								  madrid_account TEXT, madrid_account_guid TEXT, madrid_flags INTEGER, madrid_attachmentInfo BLOB,\
								  madrid_url TEXT, madrid_error INTEGER, is_madrid INTEGER, madrid_date_read INTEGER, madrid_date_delivered INTEGER);\
			Drop Trigger IF EXISTS [delete_message];\
			CREATE TRIGGER delete_message AFTER DELETE ON message WHEN NOT read(old.flags) BEGIN UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = old.group_id) - 1 WHERE ROWID = old.group_id; END;\
			Drop Trigger IF EXISTS [delete_newest_message];\
			CREATE TRIGGER delete_newest_message AFTER DELETE ON message WHEN old.ROWID = (SELECT newest_message FROM msg_group WHERE ROWID = old.group_id) BEGIN UPDATE msg_group SET newest_message = (SELECT ROWID FROM message WHERE group_id = old.group_id AND ROWID = (SELECT max(ROWID) FROM message WHERE group_id = old.group_id)) WHERE ROWID = old.group_id; END;\
			Drop Trigger IF EXISTS [delete_pieces];\
			CREATE TRIGGER delete_pieces AFTER DELETE ON message BEGIN DELETE from msg_pieces where old.ROWID == msg_pieces.message_id; END;\
			 Drop Trigger IF EXISTS [insert_newest_message];\
			CREATE TRIGGER insert_newest_message AFTER INSERT ON message WHEN new.ROWID >= IFNULL((SELECT MAX(ROWID) FROM message WHERE message.group_id = new.group_id), 0) BEGIN UPDATE msg_group SET newest_message = new.ROWID WHERE ROWID = new.group_id; END;\
			Drop Trigger IF EXISTS [insert_unread_message];\
			CREATE TRIGGER insert_unread_message AFTER INSERT ON message WHEN NOT read(new.flags) BEGIN UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) + 1 WHERE ROWID = new.group_id; END;\
			Drop Trigger IF EXISTS [mark_message_read];\
			CREATE TRIGGER mark_message_read AFTER UPDATE ON message WHEN NOT read(old.flags) AND read(new.flags) BEGIN UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) - 1 WHERE ROWID = new.group_id; END;\
			Drop Trigger IF EXISTS [mark_message_unread];\
			CREATE TRIGGER mark_message_unread AFTER UPDATE ON message WHEN read(old.flags) AND NOT read(new.flags) BEGIN UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) + 1 WHERE ROWID = new.group_id; END;\
			Drop Index If Exists [madrid_guid_index];\
			CREATE INDEX madrid_guid_index ON message(madrid_guid);\
			Drop Index If Exists [madrid_handle_service_index];\
			CREATE INDEX madrid_handle_service_index ON message(madrid_handle, madrid_service);\
			Drop Index If Exists [madrid_roomname_service_index];\
			CREATE INDEX madrid_roomname_service_index ON message(madrid_roomname, madrid_service);\
			Drop Index If Exists [message_flags_index];\
			CREATE INDEX message_flags_index ON message(flags);\
			Drop Index If Exists [message_group_index];\
			CREATE INDEX message_group_index ON message(group_id, ROWID);\
			DROP TABLE IF EXISTS [msg_group];\
			CREATE TABLE msg_group (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, type INTEGER, newest_message INTEGER, unread_count INTEGER, hash INTEGER);\
			DROP TABLE IF EXISTS [msg_pieces];\
			CREATE TABLE msg_pieces (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, message_id INTEGER, data BLOB, part_id INTEGER, preview_part INTEGER, content_type TEXT, height INTEGER, version INTEGER, flags INTEGER, content_id TEXT, content_loc TEXT, headers BLOB);\
			Drop Index If Exists [pieces_message_index];\
			CREATE INDEX pieces_message_index ON msg_pieces(message_id);";
	}

}

string IosCreateDBSQL::CreateContactDBSQL(int iProductVersion)
{
    if (iProductVersion >= 100 && iProductVersion < 200)
    {
        return CreateContact1XDBSQL();
    }
    else if (iProductVersion >= 200 && iProductVersion < 300)
    {
        return CreateContact2XDBSQL();
    }
    else if (iProductVersion >= 300 && iProductVersion < 400)
    {
        return CreateContact3XDBSQL();
    }
    else if (iProductVersion >= 400 && iProductVersion < 500)
    {
        if (iProductVersion < 430)
            return CreateContact4XDBSQL();
        else
            return CreateContact43XDBSQL();
    }
    else
        return "";
}

string IosCreateDBSQL::CreateContactImageDBSQL(int iProductVersion)
{
    if (iProductVersion >= 100 && iProductVersion < 200)
    {
        return CreateContactImage1XDBSQL();
    }
    else if (iProductVersion >= 200 && iProductVersion < 300)
    {
        return CreateContactImage2XDBSQL();
    }
    else if (iProductVersion >= 300 && iProductVersion < 400)
    {
        return CreateContactImage3XDBSQL();
    }
    else if (iProductVersion >= 400 && iProductVersion < 500)
    {
        return CreateContactImage4XDBSQL();
    }
    else
        return "";
}

string IosCreateDBSQL::CreateCalenderDBSQL(int iProductVersion)
{
    if (iProductVersion >= 100 && iProductVersion < 200)
    {
        return CreateCalender1XDBSQL();
    }
    else if (iProductVersion >= 200 && iProductVersion < 300)
    {
        return CreateCalender2XDBSQL();
    }
    else
    {
        return CreateCalender3XDBSQL();
    }
}

string IosCreateDBSQL::CreateCalender1XDBSQL()
{
    return "";
}

string IosCreateDBSQL::CreateCalender2XDBSQL()
{
    return "";
}

string IosCreateDBSQL::CreateCalender3XDBSQL()
{
    return "";
}

string IosCreateDBSQL::CreateContact1XDBSQL()
{
    return "Drop Table IF EXISTS [ABGroup];\
            Create  TABLE [ABGroup]([ROWID] INTEGER PRIMARY KEY AUTOINCREMENT,[Name] TEXT);\
            Drop Table IF EXISTS [ABGroupChanges];\
            Create  TABLE [ABGroupChanges]([record] INTEGER,[type] INTEGER);\
            Drop Table IF EXISTS [ABGroupMembers];\
            Create  TABLE [ABGroupMembers]([UID] INTEGER PRIMARY KEY,[group_id] INTEGER,[member_type] INTEGER,[member_id] INTEGER,UNIQUE(group_id,member_type,member_id));\
            Drop Table IF EXISTS [ABMultiValue];\
            Create  TABLE [ABMultiValue]([UID] INTEGER PRIMARY KEY,[record_id] INTEGER,[property] INTEGER,[identifier] INTEGER,[label] INTEGER,[value] TEXT);\
            Drop Table IF EXISTS [ABMultiValueEntry];\
            Create  TABLE [ABMultiValueEntry]([parent_id] INTEGER,[key] INTEGER,[value] TEXT,UNIQUE(parent_id,key));\
            Drop Table IF EXISTS [ABMultiValueEntryKey];\
            Create  TABLE [ABMultiValueEntryKey]([value] TEXT, UNIQUE(value));\
            Drop Table IF EXISTS [ABMultiValueLabel];\
            Create  TABLE [ABMultiValueLabel]([value] TEXT, UNIQUE(value));\
            Drop Table IF EXISTS [ABPerson];\
            Create  TABLE [ABPerson]([ROWID] INTEGER PRIMARY KEY AUTOINCREMENT,[First] TEXT,[Last] TEXT,[Middle] TEXT,[FirstPhonetic] TEXT,[MiddlePhonetic] TEXT,[LastPhonetic] TEXT,[Organization] TEXT,[Department] TEXT,[Note] TEXT,[Kind] INTEGER,[Birthday] TEXT,[JobTitle] TEXT,[Nickname] TEXT,[Prefix] TEXT,[Suffix] TEXT,[FirstSort] TEXT,[LastSort] TEXT,[CreationDate] INTEGER,[ModificationDate] INTEGER,[CompositeNameFallback] TEXT);\
            Drop Table IF EXISTS [ABPersonChanges];\
            Create  TABLE [ABPersonChanges]([record] INTEGER,[type] INTEGER);\
            Drop Table IF EXISTS [ABPersonMultiValueDeletes];\
            Create  TABLE [ABPersonMultiValueDeletes]([record_id] INTEGER,[property_id] INTEGER,[identifier] INTEGER);\
            Drop Table IF EXISTS [ABPhoneLastFour];\
            Create  TABLE [ABPhoneLastFour]([multivalue_id] INTEGER PRIMARY KEY,[value] TEXT);\
            Drop Table IF EXISTS [ABRecent];\
            Create  TABLE [ABRecent]([date] INTEGER,[name] ,[property] INTEGER,[value] );\
            Drop Table IF EXISTS [sorting_first_section_list];\
            Create  TABLE [sorting_first_section_list]([character] ,[number] , UNIQUE(character));\
            Drop Table IF EXISTS [sorting_last_section_list];\
            Create  TABLE [sorting_last_section_list]([character] ,[number] , UNIQUE(character));\
            Drop Table IF EXISTS [_SqliteDatabaseProperties];\
            Create  TABLE [_SqliteDatabaseProperties]([key] TEXT,[value] TEXT, UNIQUE(key));\
            Drop Trigger If Exists [delete_first_prefix_trigger];\
            Create  Trigger [delete_first_prefix_trigger] AFTER DELETE On [ABPerson] BEGIN INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(OLD.FirstSort, '~'), 1, 1), (SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(OLD.FirstSort, '~'), 1, 1)) - 1);END;\
            Drop Trigger If Exists [delete_last_prefix_trigger];\
            Create  Trigger [delete_last_prefix_trigger] AFTER DELETE On [ABPerson] BEGIN INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(Old.LastSort, '~'), 1, 1), (SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(Old.LastSort, '~'), 1, 1)) - 1);END;\
            Drop Trigger If Exists [delete_phone_last_four];\
            Create  Trigger [delete_phone_last_four] AFTER DELETE On [ABMultiValue] BEGIN DELETE FROM ABPhoneLastFour WHERE multivalue_id = OLD.UID;END;\
            Drop Trigger If Exists [sorting_first_prefix_trigger];\
            Create  Trigger [sorting_first_prefix_trigger] AFTER INSERT On [ABPerson] BEGIN INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(NEW.FirstSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(NEW.FirstSort, '~'), 1, 1)), 0));END;\
            Drop Trigger If Exists [sorting_last_prefix_trigger];\
            Create  Trigger [sorting_last_prefix_trigger] AFTER INSERT On [ABPerson] BEGIN INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(NEW.LastSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(NEW.LastSort, '~'), 1, 1)), 0));END;\
            Drop Trigger If Exists [update_first_prefix_trigger];\
            Create  Trigger [update_first_prefix_trigger] AFTER UPDATE On [ABPerson] BEGIN INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(OLD.FirstSort, '~'), 1, 1), (SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(OLD.FirstSort, '~'), 1, 1)) - 1); INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(NEW.FirstSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(NEW.FirstSort, '~'), 1, 1)), 0)); END;\
            Drop Trigger If Exists [update_last_prefix_trigger];\
            Create  Trigger [update_last_prefix_trigger] AFTER UPDATE On [ABPerson] BEGIN INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(OLD.LastSort, '~'), 1, 1), (SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(OLD.LastSort, '~'), 1, 1)) - 1); INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(NEW.LastSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(NEW.LastSort, '~'), 1, 1)), 0)); END;\
            Drop Index If Exists [ABFirstSortIndex];\
            CREATE  INDEX [ABFirstSortIndex] On [ABPerson] ([FirstSort] );\
            Drop Index If Exists [ABLastSortIndex];\
            CREATE  INDEX [ABLastSortIndex] On [ABPerson] ([LastSort] );\
            Drop Index If Exists [ABMultiValueEntryKeyIndex];\
            CREATE  INDEX [ABMultiValueEntryKeyIndex] On [ABMultiValueEntry] ([key] );\
            Drop Index If Exists [ABMultiValueLabelIndex];\
            CREATE  INDEX [ABMultiValueLabelIndex] On [ABMultiValue] ([label] );\
            Drop Index If Exists [ABMultiValueRecordIDIndex];\
            CREATE  INDEX [ABMultiValueRecordIDIndex] On [ABMultiValue] ([record_id] );\
            Drop Index If Exists [ABPhoneLastFourIndex];\
            CREATE  INDEX [ABPhoneLastFourIndex] On [ABPhoneLastFour] ([value] );\
            Drop Index If Exists [ABRecent_date_index];\
            CREATE  INDEX [ABRecent_date_index] On [ABRecent] ([property],[date] );\
            Drop Index If Exists [ABRecent_value_index];\
            CREATE  INDEX [ABRecent_value_index] On [ABRecent] ([property] ,[value] ) ;";
}

string IosCreateDBSQL::CreateContact2XDBSQL()
{
    return "Drop Table IF EXISTS [ABGroup];\
            CREATE TABLE ABGroup (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, StoreID INTEGER);\
            Drop Table IF EXISTS [ABGroupChanges];\
            CREATE TABLE ABGroupChanges (record INTEGER, type INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
            Drop Table IF EXISTS [ABGroupMembers];\
            CREATE TABLE ABGroupMembers (UID INTEGER PRIMARY KEY, group_id INTEGER, member_type INTEGER, member_id INTEGER, UNIQUE(group_id, member_type, member_id));\
            Drop Table IF EXISTS [ABMultiValue];\
            CREATE TABLE ABMultiValue (UID INTEGER PRIMARY KEY, record_id INTEGER, property INTEGER, identifier INTEGER, label INTEGER, value TEXT);\
            Drop Table IF EXISTS [ABMultiValueEntry];\
            CREATE TABLE ABMultiValueEntry (parent_id INTEGER, key INTEGER, value TEXT, UNIQUE(parent_id, key));\
            Drop Table IF EXISTS [ABMultiValueEntryKey];\
            CREATE TABLE ABMultiValueEntryKey (value TEXT, UNIQUE(value));\
            Drop Table IF EXISTS [ABMultiValueLabel];\
            CREATE TABLE ABMultiValueLabel (value TEXT, UNIQUE(value));\
            Drop Table IF EXISTS [ABPerson];\
            CREATE TABLE ABPerson (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, First TEXT, Last TEXT, Middle TEXT, FirstPhonetic TEXT, MiddlePhonetic TEXT, LastPhonetic TEXT, Organization TEXT, Department TEXT, Note TEXT, Kind INTEGER, Birthday TEXT, JobTitle TEXT, Nickname TEXT, Prefix TEXT, Suffix TEXT, FirstSort TEXT, LastSort TEXT, CreationDate INTEGER, ModificationDate INTEGER, CompositeNameFallback TEXT, ExternalIdentifier TEXT, StoreID INTEGER, DisplayName TEXT, FirstSortSection TEXT, LastSortSection TEXT, FirstSortLanguageIndex INTEGER DEFAULT 2147483647, LastSortLanguageIndex INTEGER DEFAULT 2147483647);\
            Drop Table IF EXISTS [ABPersonChanges];\
            CREATE TABLE ABPersonChanges (record INTEGER, type INTEGER, Image INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
            Drop Table IF EXISTS [ABPersonMultiValueDeletes];\
            CREATE TABLE ABPersonMultiValueDeletes (record_id INTEGER, property_id INTEGER, identifier INTEGER);\
            Drop Table IF EXISTS [ABPersonSearchKey];\
            CREATE TABLE ABPersonSearchKey (person_id INTEGER, value BLOB, UNIQUE(person_id));\
            Drop Table IF EXISTS [ABPhoneLastFour];\
            CREATE TABLE ABPhoneLastFour (multivalue_id INTEGER PRIMARY KEY, value TEXT);\
            Drop Table IF EXISTS [ABRecent];\
            CREATE TABLE ABRecent(date INTEGER, name, property INTEGER, value);\
            Drop Table IF EXISTS [ABStore];\
            CREATE TABLE ABStore (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, Type INTEGER, ConstraintsPath TEXT, ExternalModificationTag TEXT, StoreInternalIdentifier TEXT, AccountIdentifier TEXT, UNIQUE(StoreInternalIdentifier));\
            Drop Table IF EXISTS [ABStoreChanges];\
            CREATE TABLE ABStoreChanges (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, record INTEGER, type INTEGER);\
            Drop Table IF EXISTS [FirstSortSectionCount];\
            CREATE TABLE FirstSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section, StoreID));\
            Drop Table IF EXISTS [LastSortSectionCount];\
            CREATE TABLE LastSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section, StoreID));\
            Drop Table IF EXISTS [_SqliteDatabaseProperties];\
            Create TABLE [_SqliteDatabaseProperties]([key] TEXT,[value] TEXT, UNIQUE(key));\
            Drop Trigger If Exists [DeleteSearchKey];\
            CREATE TRIGGER DeleteSearchKey AFTER DELETE ON ABPerson BEGIN DELETE FROM ABPersonSearchKey WHERE person_id = OLD.ROWID; END;\
            Drop Trigger If Exists [delete_phone_last_four];\
            CREATE TRIGGER delete_phone_last_four AFTER DELETE ON ABMultiValue BEGIN DELETE FROM ABPhoneLastFour WHERE multivalue_id = OLD.UID;END;\
            Drop Trigger If Exists [FirstSortSectionDelete];\
            CREATE TRIGGER FirstSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
            Drop Trigger If Exists [FirstSortSectionInsert];\
            CREATE TRIGGER [FirstSortSectionInsert] AFTER INSERT On [ABPerson] BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
            Drop Trigger If Exists [FirstSortSectionUpdate];\
            CREATE TRIGGER [FirstSortSectionUpdate] AFTER UPDATE On [ABPerson] BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1);DELETE FROM FirstSortSectionCount WHERE (LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0) OR number is null;INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
            Drop Trigger If Exists [LastSortSectionDelete];\
            CREATE TRIGGER LastSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
            Drop Trigger If Exists [LastSortSectionInsert];\
            CREATE TRIGGER [LastSortSectionInsert] AFTER INSERT On [ABPerson] BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
            Drop Trigger If Exists [LastSortSectionUpdate];\
            CREATE TRIGGER [LastSortSectionUpdate] AFTER UPDATE On [ABPerson]BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1);DELETE FROM LastSortSectionCount WHERE (LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0) OR number is null;INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
            Drop Index If Exists [ABFirstSortIndex];\
            CREATE INDEX ABFirstSortIndex on ABPerson(FirstSortLanguageIndex, FirstSortSection, FirstSort) ;\
            Drop Index If Exists [ABFirstSortStore];\
            CREATE INDEX ABFirstSortStore on ABPerson(StoreID, FirstSortLanguageIndex, FirstSortSection, FirstSort);\
            Drop Index If Exists [ABLastSortIndex];\
            CREATE INDEX ABLastSortIndex on ABPerson(LastSortLanguageIndex, LastSortSection, LastSort);\
            Drop Index If Exists [ABMultiValueEntryKeyIndex];\
            CREATE INDEX ABMultiValueEntryKeyIndex ON ABMultiValueEntry(key);\
            Drop Index If Exists [ABMultiValueLabelIndex];\
            CREATE  INDEX [ABMultiValueLabelIndex] On [ABMultiValue] ([label] ) ;\
            Drop Index If Exists [ABMultiValueRecordIDIndex];\
            CREATE  INDEX [ABMultiValueRecordIDIndex] On [ABMultiValue] ([record_id] ) ;\
            Drop Index If Exists [ABPhoneLastFourIndex];\
            CREATE  INDEX [ABPhoneLastFourIndex] On [ABPhoneLastFour] ([value] ) ;\
            Drop Index If Exists [ABRecent_date_index];\
            CREATE  INDEX [ABRecent_date_index] On [ABRecent] (	[property] ,[date] ) ;\
            Drop Index If Exists [ABRecent_value_index];\
            CREATE  INDEX [ABRecent_value_index] On [ABRecent] ([property] ,[value] ) ;";
}

string IosCreateDBSQL::CreateContact3XDBSQL()
{
    return "Drop Table IF EXISTS [ABGroup];\
           CREATE TABLE ABGroup (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, StoreID INTEGER);\
           Drop Table IF EXISTS [ABGroupChanges];\
           CREATE TABLE ABGroupChanges (record INTEGER, type INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
           Drop Table IF EXISTS [ABGroupMembers];\
           CREATE TABLE ABGroupMembers (UID INTEGER PRIMARY KEY, group_id INTEGER, member_type INTEGER, member_id INTEGER, UNIQUE(group_id, member_type, member_id));\
           Drop Table IF EXISTS [ABMultiValue];\
           CREATE TABLE ABMultiValue (UID INTEGER PRIMARY KEY, record_id INTEGER, property INTEGER, identifier INTEGER, label INTEGER, value TEXT);\
           Drop Table IF EXISTS [ABMultiValueEntry];\
           CREATE TABLE ABMultiValueEntry (parent_id INTEGER, key INTEGER, value TEXT, UNIQUE(parent_id, key));\
           Drop Table IF EXISTS [ABMultiValueEntryKey];\
           CREATE TABLE ABMultiValueEntryKey (value TEXT, UNIQUE(value));\
           Drop Table IF EXISTS [ABMultiValueLabel];\
           CREATE TABLE ABMultiValueLabel (value TEXT, UNIQUE(value));\
           Drop Table IF EXISTS [ABPerson];\
           CREATE TABLE ABPerson (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, First TEXT, Last TEXT, Middle TEXT, FirstPhonetic TEXT, MiddlePhonetic TEXT, LastPhonetic TEXT, Organization TEXT, Department TEXT, Note TEXT, Kind INTEGER, Birthday TEXT, JobTitle TEXT, Nickname TEXT, Prefix TEXT, Suffix TEXT, FirstSort TEXT, LastSort TEXT, CreationDate INTEGER, ModificationDate INTEGER, CompositeNameFallback TEXT, ExternalIdentifier TEXT, StoreID INTEGER, DisplayName TEXT,ExternalRepresentation BLOB, FirstSortSection TEXT, LastSortSection TEXT, FirstSortLanguageIndex INTEGER DEFAULT 2147483647, LastSortLanguageIndex INTEGER DEFAULT 2147483647);\
           Drop Table IF EXISTS [ABPersonChanges];\
           CREATE TABLE ABPersonChanges (record INTEGER, type INTEGER, Image INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
           Drop Table IF EXISTS [ABPersonMultiValueDeletes];\
           CREATE TABLE ABPersonMultiValueDeletes (record_id INTEGER, property_id INTEGER, identifier INTEGER);\
           Drop Table IF EXISTS [ABPersonSearchKey];\
           CREATE TABLE ABPersonSearchKey (person_id INTEGER, value BLOB, UNIQUE(person_id));\
           Drop Table IF EXISTS [ABPhoneLastFour];\
           CREATE TABLE ABPhoneLastFour (multivalue_id INTEGER PRIMARY KEY, value TEXT);\
           Drop Table IF EXISTS [ABRecent];\
           CREATE TABLE ABRecent(date INTEGER, name, property INTEGER, value);\
           Drop Table IF EXISTS [ABStore];\
           CREATE TABLE ABStore (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, Type INTEGER, ConstraintsPath TEXT, ExternalModificationTag TEXT, StoreInternalIdentifier TEXT, AccountIdentifier TEXT,Enabled INTEGER DEFAULT 1, UNIQUE(StoreInternalIdentifier));\
           Drop Table IF EXISTS [FirstSortSectionCount];\
           CREATE TABLE FirstSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section, StoreID));\
           Drop Table IF EXISTS [LastSortSectionCount];\
           CREATE TABLE LastSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section, StoreID));\
           Drop Table IF EXISTS [_SqliteDatabaseProperties];\
           Create TABLE [_SqliteDatabaseProperties]([key] TEXT,[value] TEXT, UNIQUE(key));\
           Drop Trigger If Exists [DeleteSearchKey];\
           CREATE TRIGGER DeleteSearchKey AFTER DELETE ON ABPerson BEGIN DELETE FROM ABPersonSearchKey WHERE person_id = OLD.ROWID; END;\
           Drop Trigger If Exists [delete_phone_last_four];\
           CREATE TRIGGER delete_phone_last_four AFTER DELETE ON ABMultiValue BEGIN DELETE FROM ABPhoneLastFour WHERE multivalue_id = OLD.UID;END;\
           Drop Trigger If Exists [FirstSortSectionDelete];\
           CREATE TRIGGER FirstSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
           Drop Trigger If Exists [FirstSortSectionInsert];\
           CREATE TRIGGER [FirstSortSectionInsert] AFTER INSERT On [ABPerson] BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
           Drop Trigger If Exists [FirstSortSectionUpdate];\
           CREATE TRIGGER [FirstSortSectionUpdate] AFTER UPDATE On [ABPerson] BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1);DELETE FROM FirstSortSectionCount WHERE (LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0) OR number is null;INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
           Drop Trigger If Exists [LastSortSectionDelete];\
           CREATE TRIGGER LastSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
           Drop Trigger If Exists [LastSortSectionInsert];\
           CREATE TRIGGER [LastSortSectionInsert] AFTER INSERT On [ABPerson] BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
           Drop Trigger If Exists [LastSortSectionUpdate];\
           CREATE TRIGGER [LastSortSectionUpdate] AFTER UPDATE On [ABPerson]BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1);DELETE FROM LastSortSectionCount WHERE (LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0) OR number is null;INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0));END;\
           Drop Index If Exists [ABFirstSortIndex];\
           CREATE INDEX ABFirstSortIndex on ABPerson(FirstSortLanguageIndex, FirstSortSection, FirstSort) ;\
           Drop Index If Exists [ABFirstSortStoreIndex];\
           CREATE INDEX ABFirstSortStoreIndex on ABPerson(StoreID, FirstSortLanguageIndex, FirstSortSection, FirstSort);\
           Drop Index If Exists [ABLastSortIndex];\
           CREATE INDEX ABLastSortIndex on ABPerson(LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index If Exists [ABLastSortStoreIndex];\
           CREATE INDEX ABLastSortStoreIndex on ABPerson(StoreID, LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index If Exists [ABMultiValueEntryKeyIndex];\
           CREATE INDEX ABMultiValueEntryKeyIndex ON ABMultiValueEntry(key);\
           Drop Index If Exists [ABMultiValueLabelIndex];\
           CREATE  INDEX [ABMultiValueLabelIndex] On [ABMultiValue] ([label] );\
           Drop Index If Exists [ABMultiValuePropertyValueIndex];\
           CREATE  INDEX [ABMultiValuePropertyValueIndex] On [ABMultiValue] ([property] ,[value] Collate NOCASE ) ;\
           Drop Index If Exists [ABMultiValueRecordIDIndex];\
           CREATE  INDEX [ABMultiValueRecordIDIndex] On [ABMultiValue] ([record_id] );\
           Drop Index If Exists [ABPhoneLastFourIndex];\
           CREATE  INDEX [ABPhoneLastFourIndex] On [ABPhoneLastFour] ([value] );\
           Drop Index If Exists [ABRecent_date_index];\
           CREATE  INDEX [ABRecent_date_index] On [ABRecent] (	[property] ,[date] );\
           Drop Index If Exists [ABRecent_value_index];\
           CREATE  INDEX [ABRecent_value_index] On [ABRecent] ([property],[value] );";
}

string IosCreateDBSQL::CreateContact4XDBSQL()
{
    return "Drop Table If Exists [ABGroup];\
           CREATE TABLE ABGroup (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, StoreID INTEGER, ExternalModificationTag TEXT, ExternalRepresentation BLOB, ExternalUUID TEXT);\
           Drop Table If Exists [ABGroupChanges];\
           CREATE TABLE ABGroupChanges (record INTEGER, type INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
           Drop Table If Exists [ABGroupMembers];\
           CREATE TABLE ABGroupMembers (UID INTEGER PRIMARY KEY, group_id INTEGER, member_type INTEGER, member_id INTEGER, UNIQUE(group_id, member_type, member_id));\
           Drop Table If Exists [ABMultiValue];\
           CREATE TABLE ABMultiValue (UID INTEGER PRIMARY KEY, record_id INTEGER, property INTEGER, identifier INTEGER, label INTEGER, value TEXT);\
           Drop Table If Exists [ABMultiValueEntry];\
           CREATE TABLE ABMultiValueEntry (parent_id INTEGER, key INTEGER, value TEXT, UNIQUE(parent_id, key));\
           Drop Table If Exists [ABMultiValueEntryKey];\
           CREATE TABLE ABMultiValueEntryKey (value TEXT, UNIQUE(value));\
           Drop Table If Exists [ABMultiValueLabel];\
           CREATE TABLE ABMultiValueLabel (value TEXT, UNIQUE(value));\
           Drop Table If Exists [ABPerson];\
           CREATE TABLE ABPerson (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, First TEXT, Last TEXT, Middle TEXT, FirstPhonetic TEXT, MiddlePhonetic TEXT, LastPhonetic TEXT, Organization TEXT, Department TEXT, Note TEXT, Kind INTEGER, Birthday TEXT,JobTitle TEXT, Nickname TEXT, Prefix TEXT, Suffix TEXT, FirstSort TEXT, LastSort TEXT, CreationDate INTEGER,ModificationDate INTEGER, CompositeNameFallback TEXT, ExternalIdentifier TEXT, ExternalModificationTag TEXT, ExternalUUID TEXT, StoreID INTEGER, DisplayName TEXT, ExternalRepresentation BLOB, FirstSortSection TEXT, LastSortSection TEXT, FirstSortLanguageIndex INTEGER DEFAULT 2147483647, LastSortLanguageIndex INTEGER DEFAULT 2147483647, PersonLink INTEGER DEFAULT -1, ImageURI TEXT, IsPreferredName INTEGER DEFAULT 1);\
           Drop Table If Exists [ABPersonChanges];\
           CREATE TABLE ABPersonChanges (record INTEGER, type INTEGER, Image INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
           Drop Table If Exists [ABPersonLink];\
           CREATE TABLE ABPersonLink (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, PreferredImagePersonID INTEGER DEFAULT -1,PreferredNamePersonID INTEGER DEFAULT -1);\
           Drop Table If Exists [ABPersonMultiValueDeletes];\
           CREATE TABLE ABPersonMultiValueDeletes (record_id INTEGER, property_id INTEGER, identifier INTEGER);\
           Drop Table If Exists [ABPersonSearchKey];\
           CREATE TABLE ABPersonSearchKey (person_id INTEGER, value BLOB, UNIQUE(person_id));\
           Drop Table If Exists [ABPhoneLastFour];\
           CREATE TABLE ABPhoneLastFour (multivalue_id INTEGER PRIMARY KEY, value TEXT);\
           Drop Table If Exists [ABRecent];\
           CREATE TABLE ABRecent(date INTEGER, name, property INTEGER, value);\
           Drop Table If Exists [ABStore];\
           CREATE TABLE ABStore (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, Type INTEGER,ConstraintsPath TEXT, ExternalModificationTag TEXT, StoreInternalIdentifier TEXT, AccountIdentifier TEXT, Enabled INTEGER DEFAULT 1, UNIQUE(StoreInternalIdentifier));\
           Drop Table If Exists [FirstSortSectionCount];\
           CREATE TABLE FirstSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section,StoreID));\
           Drop Table If Exists [FirstSortSectionCountTotal];\
           CREATE TABLE FirstSortSectionCountTotal(LanguageIndex, Section TEXT, Type INT, number, UNIQUE(LanguageIndex, Section,Type));\
           Drop Table If Exists [LastSortSectionCount];\
           CREATE TABLE LastSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section,StoreID));\
           Drop Table If Exists [LastSortSectionCountTotal];\
           CREATE TABLE LastSortSectionCountTotal(LanguageIndex, Section TEXT, Type INT, number, UNIQUE(LanguageIndex, Section,Type));\
           Drop Table If Exists [_SqliteDatabaseProperties];\
           CREATE TABLE _SqliteDatabaseProperties (key TEXT, value TEXT, UNIQUE(key));\
           Drop Trigger If Exists [DeletePersonLink];\
           CREATE TRIGGER DeletePersonLink AFTER DELETE ON ABPerson BEGIN DELETE FROM ABPersonLink WHERE (ROWID = OLD.PersonLink AND (SELECT count() FROM ABPerson WHERE PersonLink = OLD.PersonLink) < 2); END;\
           Drop Trigger If Exists [DeleteSearchKey];\
           CREATE TRIGGER DeleteSearchKey AFTER DELETE ON ABPerson BEGIN DELETE FROM ABPersonSearchKey WHERE person_id = OLD.ROWID; END;\
           Drop Trigger If Exists [delete_phone_last_four];\
           CREATE TRIGGER delete_phone_last_four AFTER DELETE ON ABMultiValue BEGIN DELETE FROM ABPhoneLastFour WHERE multivalue_id = OLD.UID;END;\
           Drop Trigger If Exists [FirstSortSectionDelete];\
           CREATE TRIGGER FirstSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
           Drop Trigger If Exists [FirstSortSectionInsert];\
           CREATE TRIGGER FirstSortSectionInsert AFTER INSERT ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [FirstSortSectionTotalDelete];\
           CREATE TRIGGER FirstSortSectionTotalDelete AFTER DELETE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES( OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), 0, (SELECT number FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0) - 1 ); DELETE FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0 AND number <= 0; END;\
           Drop Trigger If Exists [FirstSortSectionTotalInsert];\
           CREATE TRIGGER FirstSortSectionTotalInsert AFTER INSERT ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), 0, 1 + IFNULL((SELECT number from FirstSortSectionCountTotal WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [FirstSortSectionTotalUpdateIfIsPreferred];\
           CREATE TRIGGER FirstSortSectionTotalUpdateIfIsPreferred AFTER UPDATE ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), 0, 1 + IFNULL((SELECT number from FirstSortSectionCountTotal WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [FirstSortSectionTotalUpdateIfWasPreferred];\
           CREATE TRIGGER FirstSortSectionTotalUpdateIfWasPreferred AFTER UPDATE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), 0, (SELECT number FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0) - 1 ); DELETE FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0 AND number = 0; END;\
           Drop Trigger If Exists [FirstSortSectionUpdate];\
           CREATE TRIGGER FirstSortSectionUpdate AFTER UPDATE ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0; INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [InitializePersonLinkField];\
           CREATE TRIGGER InitializePersonLinkField AFTER INSERT ON ABPerson BEGIN UPDATE ABPerson SET IsPreferredName = (NEW.PersonLink = -1 OR ROWID = (SELECT PreferredNamePersonID FROM ABPersonLink abl WHERE abl.ROWID = NEW.PersonLink)) WHERE ROWID = NEW.ROWID;END;\
           Drop Trigger If Exists [LastSortSectionDelete];\
           CREATE TRIGGER LastSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
           Drop Trigger If Exists [LastSortSectionInsert];\
           CREATE TRIGGER LastSortSectionInsert AFTER INSERT ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [LastSortSectionTotalDelete];\
           CREATE TRIGGER LastSortSectionTotalDelete AFTER DELETE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), 0, (SELECT number FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0) - 1 ); DELETE FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0 AND number <= 0; END;\
           Drop Trigger If Exists [LastSortSectionTotalInsert];\
           CREATE TRIGGER LastSortSectionTotalInsert AFTER INSERT ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), 0, 1 + IFNULL((SELECT number from LastSortSectionCountTotal WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [LastSortSectionTotalUpdateIfIsPreferred];\
           CREATE TRIGGER LastSortSectionTotalUpdateIfIsPreferred AFTER UPDATE ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), 0, 1 + IFNULL((SELECT number from LastSortSectionCountTotal WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [LastSortSectionTotalUpdateIfWasPreferred];\
           CREATE TRIGGER LastSortSectionTotalUpdateIfWasPreferred AFTER UPDATE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), 0, (SELECT number FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0) - 1 ); DELETE FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0 AND number = 0; END;\
           Drop Trigger If Exists [LastSortSectionUpdate];\
           CREATE TRIGGER LastSortSectionUpdate AFTER UPDATE ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES (OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0; INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [NamePreferredPersonDeleteTrigger];\
           CREATE TRIGGER NamePreferredPersonDeleteTrigger AFTER DELETE ON ABPerson BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = (SELECT ROWID FROM ABPerson abp WHERE abp.PersonLink = OLD.PersonLink ORDER BY abp.ROWID LIMIT 1) WHERE (ROWID = OLD.PersonLink AND PreferredNamePersonID = OLD.ROWID); END;\
           Drop Trigger If Exists [NamePreferredPersonEditTrigger];\
           CREATE TRIGGER NamePreferredPersonEditTrigger AFTER UPDATE OF Prefix, First, Middle, Last, Suffix, Organization, Kind ON ABPerson BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = OLD.ROWID WHERE (OLD.PersonLink = ROWID); END;\
           Drop Trigger If Exists [NamePreferredPersonInsertTrigger];\
           CREATE TRIGGER NamePreferredPersonInsertTrigger AFTER INSERT ON ABPerson BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = NEW.ROWID WHERE (ROWID = NEW.PersonLink AND PreferredNamePersonID = -1); END;\
           Drop Trigger If Exists [UpdatePersonLink];\
           CREATE TRIGGER UpdatePersonLink AFTER UPDATE OF PersonLink ON ABPerson BEGIN DELETE FROM ABPersonLink WHERE (ROWID = OLD.PersonLink AND (SELECT count() FROM ABPerson WHERE PersonLink = OLD.PersonLink) < 2); END;\
           Drop Trigger If Exists [UpdatePersonLinkField];\
           CREATE TRIGGER UpdatePersonLinkField AFTER UPDATE OF PersonLink ON ABPerson WHEN OLD.PersonLink != NEW.PersonLink BEGIN UPDATE ABPerson SET IsPreferredName = (NEW.PersonLink = -1 OR ROWID = (SELECT PreferredNamePersonID FROM ABPersonLink abl WHERE abl.ROWID = NEW.PersonLink)) WHERE ROWID = NEW.ROWID; END;\
           Drop Trigger If Exists [UpdatePersonLinkUponPersonUnlink];\
           CREATE TRIGGER UpdatePersonLinkUponPersonUnlink AFTER UPDATE OF PersonLink ON ABPerson WHEN OLD.PersonLink != -1 AND NEW.PersonLink = -1 BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = (SELECT ROWID FROM ABPerson abp WHERE abp.PersonLink = OLD.PersonLink ORDER BY abp.ROWID LIMIT 1) WHERE (ROWID = OLD.PersonLink AND PreferredNamePersonID = OLD.ROWID); END;\
           Drop Trigger If Exists [UpdatePersonUponLinkDeletion];\
           CREATE TRIGGER UpdatePersonUponLinkDeletion AFTER DELETE ON ABPersonLink BEGIN UPDATE ABPerson SET PersonLink = -1, IsPreferredName = 1 WHERE PersonLink = OLD.ROWID; END;\
           Drop Trigger If Exists [UpdatePersonUponLinkInsertion];\
           CREATE TRIGGER UpdatePersonUponLinkInsertion AFTER INSERT ON ABPersonLink BEGIN UPDATE ABPerson SET IsPreferredName = (ROWID = NEW.PreferredNamePersonID) WHERE PersonLink = NEW.ROWID; END;\
           Drop Trigger If Exists [UpdatePersonUponLinkUpdate];";

string str = "CREATE TRIGGER UpdatePersonUponLinkUpdate AFTER UPDATE ON ABPersonLink BEGIN UPDATE ABPerson SET IsPreferredName = (ROWID = NEW.PreferredNamePersonID) WHERE PersonLink = NEW.ROWID; END;\
           Drop Index  If Exists [ABFirstSortIndex];\
           CREATE INDEX ABFirstSortIndex on ABPerson(FirstSortLanguageIndex, FirstSortSection, FirstSort);\
           Drop Index  If Exists [ABFirstSortStoreIndex];\
           CREATE INDEX ABFirstSortStoreIndex on ABPerson(StoreID, FirstSortLanguageIndex, FirstSortSection, FirstSort);\
           Drop Index  If Exists [ABLastSortIndex];\
           CREATE INDEX ABLastSortIndex on ABPerson(LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index  If Exists [ABLastSortStoreIndex];\
           CREATE INDEX ABLastSortStoreIndex on ABPerson(StoreID, LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index  If Exists [ABMultiValueEntryKeyIndex];\
           CREATE INDEX ABMultiValueEntryKeyIndex ON ABMultiValueEntry(key);\
           Drop Index  If Exists [ABMultiValueLabelIndex];\
           CREATE INDEX ABMultiValueLabelIndex ON ABMultiValue(label);\
           Drop Index  If Exists [ABMultiValuePropertyValueIndex];\
           CREATE INDEX ABMultiValuePropertyValueIndex ON ABMultiValue(property, value COLLATE NOCASE);\
           Drop Index  If Exists [ABMultiValueRecordIDIndex];\
           CREATE INDEX ABMultiValueRecordIDIndex on ABMultiValue(record_id);\
           Drop Index  If Exists [ABPersonPersonLinkIndex];\
           Drop Index  If Exists [ABPhoneLastFourIndex];\
           CREATE INDEX ABPhoneLastFourIndex ON ABPhoneLastFour(value);\
           Drop Index  If Exists [ABPreferredFirstSortIndex];\
           CREATE INDEX ABPreferredFirstSortIndex on ABPerson(IsPreferredName, FirstSortLanguageIndex, FirstSortSection,FirstSort);\
           Drop Index  If Exists [ABPreferredLastSortIndex];\
           CREATE INDEX ABPreferredLastSortIndex on ABPerson(IsPreferredName, LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index  If Exists [ABRecent_date_index];\
           CREATE INDEX ABRecent_date_index ON ABRecent(property, date);\
           Drop Index  If Exists [ABRecent_value_index];\
           CREATE INDEX ABRecent_value_index ON ABRecent(property, value);";
}

string IosCreateDBSQL::CreateContact43XDBSQL()
{
    return "Drop Table If Exists [ABGroup];\
           CREATE TABLE ABGroup (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, StoreID INTEGER,ExternalModificationTag TEXT, ExternalRepresentation BLOB, ExternalUUID TEXT);\
           Drop Table If Exists [ABGroupChanges];\
           CREATE TABLE ABGroupChanges (record INTEGER, type INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
           Drop Table If Exists [ABGroupMembers];\
           CREATE TABLE ABGroupMembers (UID INTEGER PRIMARY KEY, group_id INTEGER, member_type INTEGER, member_id INTEGER,UNIQUE(group_id, member_type, member_id));\
           Drop Table If Exists [ABMultiValue];\
           CREATE TABLE ABMultiValue (UID INTEGER PRIMARY KEY, record_id INTEGER, property INTEGER, identifier INTEGER,label INTEGER, value TEXT);\
           Drop Table If Exists [ABMultiValueEntry];\
           CREATE TABLE ABMultiValueEntry (parent_id INTEGER, key INTEGER, value TEXT, UNIQUE(parent_id, key));\
           Drop Table If Exists [ABMultiValueEntryKey];\
           CREATE TABLE ABMultiValueEntryKey (value TEXT, UNIQUE(value));\
           Drop Table If Exists [ABMultiValueLabel];\
           CREATE TABLE ABMultiValueLabel (value TEXT, UNIQUE(value));\
           Drop Table If Exists [ABPerson];\
           CREATE TABLE ABPerson (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, First TEXT, Last TEXT, Middle TEXT, FirstPhonetic TEXT, MiddlePhonetic TEXT, LastPhonetic TEXT, Organization TEXT, Department TEXT, Note TEXT, Kind INTEGER, Birthday TEXT,JobTitle TEXT, Nickname TEXT, Prefix TEXT, Suffix TEXT, FirstSort TEXT, LastSort TEXT, CreationDate INTEGER,ModificationDate INTEGER, CompositeNameFallback TEXT, ExternalIdentifier TEXT, ExternalModificationTag TEXT,ExternalUUID TEXT, StoreID INTEGER, DisplayName TEXT, ExternalRepresentation BLOB, FirstSortSection TEXT,LastSortSection TEXT, FirstSortLanguageIndex INTEGER DEFAULT 2147483647, LastSortLanguageIndex INTEGER DEFAULT2147483647, PersonLink INTEGER DEFAULT -1, ImageURI TEXT, IsPreferredName INTEGER DEFAULT 1);\
           Drop Table If Exists [ABPersonChanges];\
           CREATE TABLE ABPersonChanges (record INTEGER, type INTEGER, Image INTEGER, ExternalIdentifier TEXT, StoreID INTEGER);\
           Drop Table If Exists [ABPersonLink];\
           CREATE TABLE ABPersonLink (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, PreferredImagePersonID INTEGER DEFAULT -1,PreferredNamePersonID INTEGER DEFAULT -1);\
           Drop Table If Exists [ABPersonMultiValueDeletes];\
           CREATE TABLE ABPersonMultiValueDeletes (record_id INTEGER, property_id INTEGER, identifier INTEGER);\
           Drop Table If Exists [ABPersonSearchKey];\
           CREATE TABLE ABPersonSearchKey (person_id INTEGER, value BLOB, UNIQUE(person_id));\
           Drop Table If Exists [ABPhoneLastFour];\
           CREATE TABLE ABPhoneLastFour (multivalue_id INTEGER PRIMARY KEY, value TEXT);\
           Drop Table If Exists [ABRecent];\
           CREATE TABLE ABRecent(date INTEGER, name, property INTEGER, value);\
           Drop Table If Exists [ABStore];\
           CREATE TABLE ABStore (ROWID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT, ExternalIdentifier TEXT, Type INTEGER,ConstraintsPath TEXT, ExternalModificationTag TEXT, ExternalSyncTag TEXT, StoreInternalIdentifier TEXT,AccountIdentifier TEXT, Enabled INTEGER DEFAULT 1, SyncData BLOB, UNIQUE(StoreInternalIdentifier));\
           Drop Table If Exists [FirstSortSectionCount];\
           CREATE TABLE FirstSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section, StoreID));\
           Drop Table If Exists [FirstSortSectionCountTotal];\
           CREATE TABLE FirstSortSectionCountTotal(LanguageIndex, Section TEXT, Type INT, number, UNIQUE(LanguageIndex, Section, Type));\
           Drop Table If Exists [LastSortSectionCount];\
           CREATE TABLE LastSortSectionCount(LanguageIndex, Section TEXT, StoreID, number, UNIQUE(LanguageIndex, Section, StoreID));\
           Drop Table If Exists [LastSortSectionCountTotal];\
           CREATE TABLE LastSortSectionCountTotal(LanguageIndex, Section TEXT, Type INT, number, UNIQUE(LanguageIndex, Section, Type));\
           Drop Table If Exists [_SqliteDatabaseProperties];\
           CREATE TABLE _SqliteDatabaseProperties (key TEXT, value TEXT, UNIQUE(key));\
           Drop Trigger If Exists [DeletePersonLink];\
           CREATE TRIGGER DeletePersonLink AFTER DELETE ON ABPerson BEGIN DELETE FROM ABPersonLink WHERE (ROWID = OLD.PersonLink AND (SELECT count() FROM ABPerson WHERE PersonLink = OLD.PersonLink) < 2); END;\
           Drop Trigger If Exists [DeleteSearchKey];\
           CREATE TRIGGER DeleteSearchKey AFTER DELETE ON ABPerson BEGIN DELETE FROM ABPersonSearchKey WHERE person_id = OLD.ROWID; END;\
           Drop Trigger If Exists [delete_phone_last_four];\
           CREATE TRIGGER delete_phone_last_four AFTER DELETE ON ABMultiValue BEGIN DELETE FROM ABPhoneLastFour WHERE multivalue_id = OLD.UID;END;\
           Drop Trigger If Exists [FirstSortSectionDelete];\
           CREATE TRIGGER FirstSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
           Drop Trigger If Exists [FirstSortSectionInsert];\
           CREATE TRIGGER FirstSortSectionInsert AFTER INSERT ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [FirstSortSectionTotalDelete];\
           CREATE TRIGGER FirstSortSectionTotalDelete AFTER DELETE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), 0, (SELECT number FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0) - 1 ); DELETE FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0 AND number <= 0; END;\
           Drop Trigger If Exists [FirstSortSectionTotalInsert];\
           CREATE TRIGGER FirstSortSectionTotalInsert AFTER INSERT ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), 0, 1 + IFNULL((SELECT number from FirstSortSectionCountTotal WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [FirstSortSectionTotalUpdateIfIsPreferred];\
           CREATE TRIGGER FirstSortSectionTotalUpdateIfIsPreferred AFTER UPDATE ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), 0, 1 + IFNULL((SELECT number from FirstSortSectionCountTotal WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [FirstSortSectionTotalUpdateIfWasPreferred];\
           CREATE TRIGGER FirstSortSectionTotalUpdateIfWasPreferred AFTER UPDATE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO FirstSortSectionCountTotal VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), 0, (SELECT number FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0) - 1 ); DELETE FROM FirstSortSectionCountTotal WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND Type = 0 AND number = 0; END;\
           Drop Trigger If Exists [FirstSortSectionUpdate];\
           CREATE TRIGGER FirstSortSectionUpdate AFTER UPDATE ON ABPerson BEGIN INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0; INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [InitializePersonLinkField];\
           CREATE TRIGGER InitializePersonLinkField AFTER INSERT ON ABPerson BEGIN UPDATE ABPerson SET IsPreferredName = (NEW.PersonLink = -1 OR ROWID = (SELECT PreferredNamePersonID FROM ABPersonLink abl WHERE abl.ROWID = NEW.PersonLink)) WHERE ROWID = NEW.ROWID; END;\
           Drop Trigger If Exists [LastSortSectionDelete];\
           CREATE TRIGGER LastSortSectionDelete AFTER DELETE ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES (OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0; END;\
           Drop Trigger If Exists [LastSortSectionInsert];\
           CREATE TRIGGER LastSortSectionInsert AFTER INSERT ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [LastSortSectionTotalDelete];\
           CREATE TRIGGER LastSortSectionTotalDelete AFTER DELETE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), 0, (SELECT number FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0) - 1 ); DELETE FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0 AND number <= 0; END;\
           Drop Trigger If Exists [LastSortSectionTotalInsert];\
           CREATE TRIGGER LastSortSectionTotalInsert AFTER INSERT ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), 0, 1 + IFNULL((SELECT number from LastSortSectionCountTotal WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [LastSortSectionTotalUpdateIfIsPreferred];\
           CREATE TRIGGER LastSortSectionTotalUpdateIfIsPreferred AFTER UPDATE ON ABPerson WHEN NEW.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), 0, 1 + IFNULL((SELECT number from LastSortSectionCountTotal WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND Type = 0), 0) ); END;\
           Drop Trigger If Exists [LastSortSectionTotalUpdateIfWasPreferred];\
           CREATE TRIGGER LastSortSectionTotalUpdateIfWasPreferred AFTER UPDATE ON ABPerson WHEN OLD.IsPreferredName=1 BEGIN INSERT OR REPLACE INTO LastSortSectionCountTotal VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), 0, (SELECT number FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0) - 1 ); DELETE FROM LastSortSectionCountTotal WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND Type = 0 AND number = 0; END;\
           Drop Trigger If Exists [LastSortSectionUpdate];\
           CREATE TRIGGER LastSortSectionUpdate AFTER UPDATE ON ABPerson BEGIN INSERT OR REPLACE INTO LastSortSectionCount VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1); DELETE FROM LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0; INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0)); END;\
           Drop Trigger If Exists [NamePreferredPersonDeleteTrigger];\
           CREATE TRIGGER NamePreferredPersonDeleteTrigger AFTER DELETE ON ABPerson BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = (SELECT ROWID FROM ABPerson abp WHERE abp.PersonLink = OLD.PersonLink ORDER BY abp.ROWID LIMIT 1) WHERE (ROWID = OLD.PersonLink AND PreferredNamePersonID = OLD.ROWID); END;\
           Drop Trigger If Exists [NamePreferredPersonEditTrigger];\
           CREATE TRIGGER NamePreferredPersonEditTrigger AFTER UPDATE OF Prefix, First, Middle, Last, Suffix, Organization, Kind ON ABPerson BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = OLD.ROWID WHERE (OLD.PersonLink = ROWID); END;\
           Drop Trigger If Exists [NamePreferredPersonInsertTrigger];\
           CREATE TRIGGER NamePreferredPersonInsertTrigger AFTER INSERT ON ABPerson BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = NEW.ROWID WHERE (ROWID = NEW.PersonLink AND PreferredNamePersonID = -1); END;\
           Drop Trigger If Exists [UpdatePersonLink];\
           CREATE TRIGGER UpdatePersonLink AFTER UPDATE OF PersonLink ON ABPerson BEGIN DELETE FROM ABPersonLink WHERE (ROWID = OLD.PersonLink AND (SELECT count() FROM ABPerson WHERE PersonLink = OLD.PersonLink) < 2); END;\
           Drop Trigger If Exists [UpdatePersonLinkField];\
           CREATE TRIGGER UpdatePersonLinkField AFTER UPDATE OF PersonLink ON ABPerson WHEN OLD.PersonLink != NEW.PersonLink BEGIN UPDATE ABPerson SET IsPreferredName = (NEW.PersonLink = -1 OR ROWID = (SELECT PreferredNamePersonID FROM ABPersonLink abl WHERE abl.ROWID = NEW.PersonLink)) WHERE ROWID = NEW.ROWID; END;\
           Drop Trigger If Exists [UpdatePersonLinkUponPersonUnlink];\
           CREATE TRIGGER UpdatePersonLinkUponPersonUnlink AFTER UPDATE OF PersonLink ON ABPerson WHEN OLD.PersonLink != -1 AND NEW.PersonLink = -1 BEGIN UPDATE ABPersonLink SET PreferredNamePersonID = (SELECT ROWID FROM ABPerson abp WHERE abp.PersonLink = OLD.PersonLink ORDER BY abp.ROWID LIMIT 1) WHERE (ROWID = OLD.PersonLink AND PreferredNamePersonID = OLD.ROWID); END;\
           Drop Trigger If Exists [UpdatePersonUponLinkDeletion];\
           CREATE TRIGGER UpdatePersonUponLinkDeletion AFTER DELETE ON ABPersonLink BEGIN UPDATE ABPerson SET PersonLink = -1, IsPreferredName = 1 WHERE PersonLink = OLD.ROWID; END;\
           Drop Trigger If Exists [UpdatePersonUponLinkInsertion];\
           CREATE TRIGGER UpdatePersonUponLinkInsertion AFTER INSERT ON ABPersonLink BEGIN UPDATE ABPerson SET IsPreferredName = (ROWID = NEW.PreferredNamePersonID) WHERE PersonLink = NEW.ROWID; END;\
           Drop Trigger If Exists [UpdatePersonUponLinkUpdate];";

string str = "CREATE TRIGGER UpdatePersonUponLinkUpdate AFTER UPDATE ON ABPersonLink BEGIN UPDATE ABPerson SET IsPreferredName = (ROWID = NEW.PreferredNamePersonID) WHERE PersonLink = NEW.ROWID; END;\
           Drop Index  If Exists [ABFirstSortIndex];\
           CREATE INDEX ABFirstSortIndex on ABPerson(FirstSortLanguageIndex, FirstSortSection, FirstSort);\
           Drop Index  If Exists [ABFirstSortStoreIndex];\
           CREATE INDEX ABFirstSortStoreIndex on ABPerson(StoreID, FirstSortLanguageIndex, FirstSortSection, FirstSort);\
           Drop Index  If Exists [ABLastSortIndex];\
           CREATE INDEX ABLastSortIndex on ABPerson(LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index  If Exists [ABLastSortStoreIndex];\
           CREATE INDEX ABLastSortStoreIndex on ABPerson(StoreID, LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index  If Exists [ABMultiValueEntryKeyIndex];\
           CREATE INDEX ABMultiValueEntryKeyIndex ON ABMultiValueEntry(key);\
           Drop Index  If Exists [ABMultiValueLabelIndex];\
           CREATE INDEX ABMultiValueLabelIndex ON ABMultiValue(label);\
           Drop Index  If Exists [ABMultiValuePropertyValueIndex];\
           CREATE INDEX ABMultiValuePropertyValueIndex ON ABMultiValue(property, value COLLATE NOCASE);\
           Drop Index  If Exists [ABMultiValueRecordIDIndex];\
           CREATE INDEX ABMultiValueRecordIDIndex on ABMultiValue(record_id);\
           Drop Index  If Exists [ABPersonPersonLinkIndex];\
           Drop Index  If Exists [ABPhoneLastFourIndex];\
           CREATE INDEX ABPhoneLastFourIndex ON ABPhoneLastFour(value);\
           Drop Index  If Exists [ABPreferredFirstSortIndex];\
           CREATE INDEX ABPreferredFirstSortIndex on ABPerson(IsPreferredName, FirstSortLanguageIndex, FirstSortSection, FirstSort);\
           Drop Index  If Exists [ABPreferredLastSortIndex];\
           CREATE INDEX ABPreferredLastSortIndex on ABPerson(IsPreferredName, LastSortLanguageIndex, LastSortSection, LastSort);\
           Drop Index  If Exists [ABRecent_date_index];\
           CREATE INDEX ABRecent_date_index ON ABRecent(property, date);\
           Drop Index  If Exists [ABRecent_value_index];\
           CREATE INDEX ABRecent_value_index ON ABRecent(property, value);";
}

string IosCreateDBSQL::CreateContactImage1XDBSQL()
{
    return "DROP TABLE IF EXISTS [ABImage];\
           Create TABLE [ABImage]([record_id] INTEGER,[format] INTEGER,[crop_x] INTEGER,[crop_y] INTEGER,[crop_width] INTEGER,[crop_height] INTEGER,[data] TEXT);";
}

string IosCreateDBSQL::CreateContactImage2XDBSQL()
{
    return "DROP TABLE IF EXISTS [ABImage];\
           CREATE TABLE [ABImage]([record_id] INTEGER,[format] INTEGER,[crop_x] INTEGER,[crop_y] INTEGER,[crop_width] INTEGER,[crop_height] INTEGER,[data] TEXT)";
}

string IosCreateDBSQL::CreateContactImage3XDBSQL()
{
    return "DROP TABLE IF EXISTS [ABImage];\
           CREATE TABLE [ABImage]([record_id] INTEGER,[format] INTEGER,[crop_x] INTEGER,[crop_y] INTEGER,[crop_width] INTEGER,[crop_height] INTEGER,[data] TEXT)";
}

string IosCreateDBSQL::CreateContactImage4XDBSQL()
{
    return "DROP TABLE IF EXISTS [ABFullSizeImage];\
           CREATE TABLE [ABFullSizeImage] ([ROWID] INTEGER PRIMARY KEY, [record_id] INTEGER, [crop_x] INTEGER, [crop_y] INTEGER, [crop_width] INTEGER, [data] BLOB);\
           DROP INDEX IF EXISTS [ABFullSizeImageRecordIDIndex];\
           CREATE UNIQUE INDEX ABFullSizeImageRecordIDIndex ON ABFullSizeImage([record_id]);\
           DROP TABLE IF EXISTS [ABThumbnailImage];\
           CREATE TABLE [ABThumbnailImage] ([ROWID] INTEGER PRIMARY KEY, [record_id] INTEGER, [format] INTEGER, [data] BLOB);\
           DROP INDEX IF EXISTS [ABImageRecordIDFormatIndex];\
           CREATE UNIQUE INDEX ABImageRecordIDFormatIndex ON ABThumbnailImage(record_id, format)";
}

string IosCreateDBSQL::DropSmsTriggersSQL()
{
   return "DROP TRIGGER IF EXISTS [delete_message];\
           DROP TRIGGER IF EXISTS [delete_newest_message];\
           DROP TRIGGER IF EXISTS [insert_unread_message];\
           DROP TRIGGER IF EXISTS [insert_newest_message];\
           DROP TRIGGER IF EXISTS [mark_message_unread];\
           DROP TRIGGER IF EXISTS [mark_message_read];";
}

string IosCreateDBSQL::CreateSmsTriggersSQL()
{
    return "Create  Trigger [delete_message] AFTER DELETE On [message] when not read(old.flags) begin  update msg_group set unread_count = (select unread_count from msg_group where rowid = old.group_id) - 1 where rowid = old.group_id; end;\
            Create  Trigger [delete_newest_message] AFTER DELETE On [message] WHEN old.ROWID = (SELECT newest_message FROM msg_group WHERE ROWID = old.group_id) BEGIN UPDATE msg_group SET newest_message = (SELECT ROWID FROM message WHERE group_id = old.group_id AND ROWID = (SELECT max(ROWID) FROM message WHERE group_id = old.group_id)) WHERE ROWID = old.group_id; END;\
            Create  Trigger [insert_newest_message] AFTER INSERT On [message] WHEN new.ROWID >= IFNULL((SELECT MAX(ROWID) FROM message WHERE message.group_id = new.group_id), 0) BEGIN UPDATE msg_group SET newest_message = new.ROWID WHERE ROWID = new.group_id; END;\
            Create  Trigger [insert_unread_message] AFTER INSERT On [message] WHEN NOT read(new.flags) BEGIN UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) + 1 WHERE ROWID = new.group_id; END;\
            Create  Trigger [mark_message_read] AFTER UPDATE On [message] WHEN NOT read(old.flags) AND read(new.flags) BEGIN  UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) - 1 WHERE ROWID = new.group_id; END;\
            Create  Trigger [mark_message_unread] AFTER UPDATE On [message] WHEN read(old.flags) AND NOT read(new.flags) BEGIN  UPDATE msg_group SET unread_count = (SELECT unread_count FROM msg_group WHERE ROWID = new.group_id) + 1 WHERE ROWID = new.group_id;  END;";
}

// ∞≤◊∞≈≈–Ú≤π∂°”Ôæ‰(1X∞Ê±æ)
string IosCreateDBSQL::InstallPatchSQL_1X()
{
    return "DROP TRIGGER IF EXISTS update_ABPersonNameSort_trigger;\
               CREATE TRIGGER update_ABPersonNameSort_trigger AFTER UPDATE ON ABPerson\
               BEGIN\
               UPDATE ABPerson SET FirstSort=(\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), '~') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,' ')), 2, 1)), ' ') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,' ')), 3, 1)), ' ')),\
               LastSort=(\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), '~') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,' ')), 2, 1)), ' ') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,' ')), 3, 1)), ' '))\
               WHERE ROWID=OLD.ROWID;\
               END;\
            DROP TRIGGER IF EXISTS insert_ABPersonNameSort_trigger;\
               CREATE TRIGGER insert_ABPersonNameSort_trigger AFTER INSERT ON ABPerson\
               BEGIN\
               UPDATE ABPerson SET FirstSort=(\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), '~') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,' ')), 2, 1)), ' ') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,' ')), 3, 1)), ' ')),\
               LastSort=(\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), '~') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,' ')), 2, 1)), ' ') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,' ')), 3, 1)), ' '))\
               WHERE ROWID=NEW.ROWID;\
               END;\
            DROP TRIGGER IF EXISTS update_first_prefix_trigger;\
               CREATE TRIGGER update_first_prefix_trigger AFTER UPDATE OF FirstSort,LastSort ON ABPerson\
               BEGIN\
               INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(OLD.FirstSort, '~'), 1, 1), IFNULL((SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(OLD.FirstSort, '~'), 1, 1)), 0) - 1);\
               INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(NEW.FirstSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(NEW.FirstSort, '~'), 1, 1)), 0));\
               END;\
            DROP TRIGGER IF EXISTS update_last_prefix_trigger;\
               CREATE TRIGGER update_last_prefix_trigger AFTER UPDATE OF FirstSort,LastSort ON ABPerson\
               BEGIN\
               INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(OLD.LastSort, '~'), 1, 1), IFNULL((SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(OLD.LastSort, '~'), 1, 1)), 0) - 1);\
               INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(NEW.LastSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(NEW.LastSort, '~'), 1, 1)), 0));\
               END;\
            DROP TABLE IF EXISTS Pinyin;\
               CREATE TABLE Pinyin(ROWID INTEGER PRIMARY KEY, character TEXT, phoneticize TEXT);\
               CREATE INDEX PinyinRowIDX on Pinyin(ROWID);\
               CREATE INDEX PinyinCharIDX on Pinyin(character);";
}

// ∞≤◊∞≈≈–Ú≤π∂°”Ôæ‰
string IosCreateDBSQL::InstallPatchSQL_2X_3X()
{
    // 1.UpdateABPerson
    // 2.LastSortSectionUpdate
    // 3.FirstSortSectionUpdate
    // 4.InsertABPerson
    // 5.FirstSortSectionInsert
    // 6.LastSortSectionInsert
    return "Drop Trigger IF EXISTS [UpdateABPerson];\
               CREATE TRIGGER [UpdateABPerson] AFTER UPDATE On [ABPerson] \
               BEGIN \
               UPDATE [abperson] SET FirstSort = (\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 2, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 3, 1)), '') ),\
               LastSort = (\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 2, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 3, 1)), '') ),\
               FirstSortSection = (IFNULL(substr((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)),1,1), '' )),\
               LastSortSection = (IFNULL(substr((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)),1,1), '')),\
               FirstSortLanguageIndex = (IFNULL((select LanguageIndex from Pinyin where character = substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), 2147483647)),\
               LastSortLanguageIndex = (IFNULL((select LanguageIndex from Pinyin where character = substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), 2147483647))\
               WHERE ROWID=NEW.ROWID;\
               INSERT OR REPLACE INTO [ABPersonSearchKey]\
               VALUES(new.rowid, \
               (IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 2, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 3, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 2, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 3, 1)), '')));\
               END;\
            Drop Trigger IF EXISTS [LastSortSectionUpdate];\
               CREATE TRIGGER [LastSortSectionUpdate] AFTER UPDATE On [ABPerson]\
               BEGIN \
               INSERT OR REPLACE INTO LastSortSectionCount VALUES(OLD.LastSortLanguageIndex, IFNULL(OLD.LastSortSection, ''), OLD.StoreID, (SELECT number from LastSortSectionCount WHERE LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID) - 1);\
               DELETE FROM LastSortSectionCount WHERE (LanguageIndex = OLD.LastSortLanguageIndex AND Section = IFNULL(OLD.LastSortSection, '') AND StoreID = OLD.StoreID AND number = 0) OR number is null;\
               INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0));\
               END;\
               Drop Trigger IF EXISTS [FirstSortSectionUpdate];\
               CREATE TRIGGER [FirstSortSectionUpdate] AFTER UPDATE On [ABPerson] \
               BEGIN \
               INSERT OR REPLACE INTO FirstSortSectionCount VALUES(OLD.FirstSortLanguageIndex, IFNULL(OLD.FirstSortSection, ''), OLD.StoreID, (SELECT number from FirstSortSectionCount WHERE LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID) - 1);\
               DELETE FROM FirstSortSectionCount WHERE (LanguageIndex = OLD.FirstSortLanguageIndex AND Section = IFNULL(OLD.FirstSortSection, '') AND StoreID = OLD.StoreID AND number = 0) OR number is null;\
               INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0));\
               END;\
            Drop Trigger IF EXISTS [InsertABPerson];\
               CREATE TRIGGER [InsertABPerson] AFTER INSERT On [ABPerson] \
               BEGIN \
               UPDATE [abperson] SET FirstSort = (\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 2, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 3, 1)), '') ),\
               LastSort = (\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 2, 1)), '') ||\
               IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 3, 1)), '') ),\
               FirstSortSection = (IFNULL(substr((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)),1,1), '' )),\
               LastSortSection = (IFNULL(substr((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)),1,1), '')),\
               FirstSortLanguageIndex = (IFNULL((select LanguageIndex from Pinyin where character = substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), 2147483647)),\
               LastSortLanguageIndex = (IFNULL((select LanguageIndex from Pinyin where character = substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), 2147483647))\
               WHERE ROWID=NEW.ROWID;\
               END;\
            Drop Trigger IF EXISTS [FirstSortSectionInsert];\
               CREATE TRIGGER [FirstSortSectionInsert] AFTER INSERT On [ABPerson] \
               BEGIN \
               INSERT OR REPLACE INTO FirstSortSectionCount VALUES(NEW.FirstSortLanguageIndex, IFNULL(NEW.FirstSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from FirstSortSectionCount WHERE LanguageIndex = NEW.FirstSortLanguageIndex AND Section = IFNULL(NEW.FirstSortSection, '') AND StoreID = NEW.StoreID), 0));\
               END;\
            Drop Trigger IF EXISTS [LastSortSectionInsert];\
               CREATE TRIGGER [LastSortSectionInsert] AFTER INSERT On [ABPerson] \
               BEGIN \
               INSERT OR REPLACE INTO LastSortSectionCount VALUES(NEW.LastSortLanguageIndex, IFNULL(NEW.LastSortSection, ''), NEW.StoreID, 1 + IFNULL((SELECT number from LastSortSectionCount WHERE LanguageIndex = NEW.LastSortLanguageIndex AND Section = IFNULL(NEW.LastSortSection, '') AND StoreID = NEW.StoreID), 0));\
               END;";
}

// ∞≤◊∞≈≈–Ú≤π∂°”Ôæ‰(4X∞Ê±æ)
string IosCreateDBSQL::InstallPatchSQL_4X()
{
    // 1.UpdateABPerson
    return "Drop Trigger IF EXISTS [UpdateABPerson];\
            CREATE TRIGGER [UpdateABPerson] AFTER UPDATE On [ABPerson] \
            BEGIN \
            UPDATE [abperson] SET FirstSort = (\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 2, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 3, 1)), '') ),\
            LastSort = (\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 2, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 3, 1)), '') ),\
            FirstSortSection = (IFNULL(substr((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)),1,1), '' )),\
            LastSortSection = (IFNULL(substr((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)),1,1), '')),\
            FirstSortLanguageIndex = (IFNULL((select LanguageIndex from Pinyin where character = substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), 2147483647)),\
            LastSortLanguageIndex = (IFNULL((select LanguageIndex from Pinyin where character = substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), 2147483647))\
            WHERE ROWID=NEW.ROWID;\
            INSERT OR REPLACE INTO [ABPersonSearchKey]\
            VALUES(new.rowid, \
            (IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 1, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 2, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.First, IFNULL(NEW.Last,'~')), 3, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 1, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 2, 1)), '') ||\
            IFNULL((select phoneticize from Pinyin where character=substr(IFNULL(NEW.Last, IFNULL(NEW.First,'~')), 3, 1)), '')));\
            END;";
}

// –∂‘ÿ≈≈–Ú≤π∂°”Ôæ‰(1X∞Ê±æ)
string IosCreateDBSQL::UnInstallPatchSQL_1X()
{
    // ================ –∂‘ÿ≤π∂° ================ 
    // ================ ªπ‘≠œµÕ≥ƒ¨»œ ================ 
    return "DROP TRIGGER IF EXISTS update_ABPersonNameSort_trigger;\
               DROP TRIGGER IF EXISTS insert_ABPersonNameSort_trigger;\
               DROP TRIGGER IF EXISTS [update_first_prefix_trigger];\
               DROP TRIGGER IF EXISTS [update_last_prefix_trigger]; \
               DROP TABLE IF EXISTS Pinyin;\
               DROP TABLE IF EXISTS PinyinVersion;\
            Drop Trigger IF EXISTS [update_first_prefix_trigger];\
               Create Trigger [update_first_prefix_trigger] AFTER UPDATE On [ABPerson] \
               BEGIN\
               INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(OLD.FirstSort, '~'), 1, 1), (SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(OLD.FirstSort, '~'), 1, 1)) - 1);\
               INSERT OR REPLACE INTO sorting_first_section_list VALUES(substr(IFNULL(NEW.FirstSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_first_section_list WHERE character = substr(IFNULL(NEW.FirstSort, '~'), 1, 1)), 0));\
               END;\
            Drop Trigger IF EXISTS [update_last_prefix_trigger];\
               Create Trigger [update_last_prefix_trigger] AFTER UPDATE On [ABPerson] \
               BEGIN\
               INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(OLD.LastSort, '~'), 1, 1), (SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(OLD.LastSort, '~'), 1, 1)) - 1);\
               INSERT OR REPLACE INTO sorting_last_section_list VALUES(substr(IFNULL(NEW.LastSort, '~'), 1, 1), 1 + IFNULL((SELECT number from sorting_last_section_list WHERE character = substr(IFNULL(NEW.LastSort, '~'), 1, 1)), 0));\
               END;";
}

// –∂‘ÿ≈≈–Ú≤π∂°”Ôæ‰(2X,3X,4X∞Ê±æ)
string IosCreateDBSQL::UnInstallPatchSQL_2X_3X_4X()
{
    return "Drop Trigger IF EXISTS [InsertABPerson];\
            Drop Trigger IF EXISTS [UpdateABPerson];\
            DROP TABLE IF EXISTS Pinyin;DROP TABLE IF EXISTS PinyinVersion;";
}

