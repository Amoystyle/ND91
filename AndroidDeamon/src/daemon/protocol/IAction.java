package daemon.protocol;

/**
 *  <p>Title: IAction</p>
 *  <p>Description: IAction</p>
 *  <p>Copyright: Copyright (c) 2009</p>
 *  <p>Company: nd </p>
 *
 * @author:  Jason.Lee
 * @version: 1.0
 */

public interface IAction {
    String ACTION = "ACTION";
    int ADD = 1;
    int EDIT = 2;
    int DELETE = 4;
    int QUERY = 8;
    int UNKNOWN = 0;
    //the status to response to client.
    int FAIL = 100;
    int OKAY = 200;
    String PROTOCOL_FLAG = "NDPC";
}
