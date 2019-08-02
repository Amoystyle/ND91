package daemon.protocol;

import java.util.List;

/**
 *  <p>Title: IBaseProtocol</p>
 *  <p>Description: IBaseProtocol</p>
 *  <p>Copyright: Copyright (c) 2009</p>
 *  <p>Company: nd </p>
 *
 * @author:  Jason.Lee
 * @version: 1.0
 */

public interface IBaseProtocol {

    List<IBaseEntity> decode();

    byte[] encode(Integer moduleName, Object params);
}
