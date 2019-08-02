package daemon.protocol;

/**
 *  <p>Title: IDataType</p>
 *  <p>Description: IDataType</p>
 *  <p>Copyright: Copyright (c) 2009</p>
 *  <p>Company: nd </p>
 *
 * @author:  Jason.Lee
 * @version: 1.0
 */

public interface IDataType {
    int STRING = 1;
    int INT = 2;
    int BOOLEAN = 3;
    int DATETIME = 4;
    int BINARY = 5;
    int FLOAT = 6;
    int LONG = 7;
    int CHAR = 8;
    int DOUBLE = 9;
    int BYTE = 10;
    int SHORT = 11;

    String FIELD_SEGMENT = "FILD";
    String CONDITION = "CNDT";
    String DATA_SEGMENT = "DATA";
    String MODULE_NAME = "MODULE_NAME";
    String TABLE_NAME = "TABLE_NAME";
    String DATA_TYPE = "DATA_TYPE";
    String STAR = "*";
    String STATUS_CODE = "STATUS_CODE";

    String CONTACTS = "CONTACTS";
    String SMS = "SMS";
    String TODO = "TODO";
}
