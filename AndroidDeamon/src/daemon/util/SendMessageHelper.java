package daemon.util;

import daemon.connection.ConnectionManager;
import daemon.protocol.ByteWriter;
import daemon.receiver.SendMessageType;

public class SendMessageHelper {

	public static void   sendMediaMessageToPc(int action){
		ByteWriter writer = new ByteWriter();
		writer.Write(action);
		ConnectionManager.getInstance().sendMessage(SendMessageType.MEDIA_BUSINESS_ID, writer);
	}
}
