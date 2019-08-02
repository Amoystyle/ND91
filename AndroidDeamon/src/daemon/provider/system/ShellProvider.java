package daemon.provider.system;

import java.nio.ByteBuffer;


import daemon.logger.LogCenter;
import daemon.ndk.NdkManager;
import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.provider.IProvider;
import daemon.provider.ProviderExecuteContext;
import daemon.screenshot.FrameBufferController;
import daemon.shell.Shell;

import android.content.Context;
import android.os.Environment;
import android.view.Display;
import android.view.WindowManager;

public class ShellProvider implements IProvider {

	private Context mContext;

	public ShellProvider(Context cxt) {
		this.mContext = cxt;
	}

	@Override
	public void execute(ProviderExecuteContext context) {
		ByteReader reader = context.getByteReader();
		ByteWriter writer = context.getByteWriter();
		int action = reader.ReadInt32();
		switch (action) {
			case 1:
				execForString(reader, writer);
				break;
			case 2:
				execForBytes(reader, writer);
				break;
			case 3:
				getRawImage(reader, writer);
				break;
			case 4:
				getScreenByteBuffer(reader, writer);
				break;
			case 5:
				open_socket_log_backend(reader, writer);
				break;
			case 6:
				close_socket_log_backend(reader, writer);
				break;
			case 7:
				getScreenSize(reader, writer);
				break;
			case 8:
				getSdcardPath(reader, writer);
				break;
			case 9:
				getScreen(reader, writer,false);
				break;
			case 10:
				getScreen(reader, writer,true);
				break;
			case 11:
				getOrientation(writer);
				break;
			case 20:
				getNdshStatus(writer);
				break;
			case 21:
				suAuthorize(writer);
				break;
			case 22:
				execCmdbyNdsh(reader);
				break;
			case 23:
				execCmdbyNdshForString(reader,writer);
				break;
			default:
				break;
		}
	}

	private void execCmdbyNdshForString(ByteReader reader, ByteWriter writer) {
		String cmd = reader.ReadString();
		String result = "";
		if(NdkManager.isNdshActualRunning(mContext)){
			result = NdkManager.execShellforStr(mContext, cmd);
		}else{
			result = Shell.getInstance().execForString(cmd);
		}
		writer.Write(result);
		
	}

	private void execCmdbyNdsh(ByteReader reader) {
		String cmd = reader.ReadString();
		if(NdkManager.isNdshActualRunning(mContext)){
			NdkManager.execShell(mContext, cmd);
		}else{
			Shell.getInstance().exec(cmd);
		}
	}
	
	private void getOrientation(ByteWriter writer)
	{
		android.view.WindowManager manager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
		int rotation = manager.getDefaultDisplay().getOrientation();//.getRotation();
		writer.Write(rotation);
	}

	private void getScreen(ByteReader reader, ByteWriter writer,boolean getJpg) {
		if (NdkManager.isNdshActualRunning(mContext)) {
			byte[] result = null;
			if (NdkManager.isRunInRoot(mContext)) {
				if(getJpg){
					int quaity = reader.ReadInt32();
					result = NdkManager.srceenshotJpeg(mContext,quaity);
				}else{
					boolean toBitmap565 = reader.ReadBoolean();
					result = NdkManager.srceenshot(mContext, toBitmap565);
				}
			}
			else {
				result = new byte[] {-1};
			}
			writer.Write(result);
		}
		else {
			try {
				FrameBufferController fbc = FrameBufferController.instance(mContext);
				if (fbc != null) {
					FrameBufferController.ScreenInfo si = fbc.get_si();
					ByteBuffer buffer = fbc.grab_frame_buffer();
					if (si != null && buffer != null) {
						writer.Write(1);

						writer.Write(si.bpp);
						writer.Write(si.width);
						writer.Write(si.height);

						writer.Write(si.red_offset);
						writer.Write(si.red_length);

						writer.Write(si.blue_offset);
						writer.Write(si.blue_length);

						writer.Write(si.green_offset);
						writer.Write(si.green_length);

						writer.Write(si.alpha_offset);
						writer.Write(si.alpha_length);

						int length = si.width * si.height * ((si.bpp + 7) >> 3);
						writer.Write(length);
						writer.Write(buffer.array());
						buffer.clear();
						buffer = null;

						return;
					}
				}
			}
			catch (Exception e) {
				LogCenter.error("ScreenShot", e.getMessage());
			}

			writer.Write(-1);
		}
	}

	private void open_socket_log_backend(ByteReader reader, ByteWriter writer) {
		if (!LogCenter.open_socket_backend()) {
			writer.WriteString("FAIL");
		}
		else {
			writer.WriteString("OKAY");
		}
	}

	private void close_socket_log_backend(ByteReader reader, ByteWriter writer) {
		if (!LogCenter.close_socket_backend()) {
			writer.WriteString("FAIL");
		}
		else {
			writer.WriteString("OKAY");
		}
	}

	private void getScreenByteBuffer(ByteReader reader, ByteWriter writer) {
		FrameBufferController fbc = null;
		try {
			fbc = FrameBufferController.instance(mContext);
			ByteBuffer buffer = fbc.grab_frame_buffer();
			
			if (buffer != null) {							
				writer.WriteString("OKAY");
				writer.Write(buffer.array().length);
				writer.Write(buffer.array());
				buffer.clear();
				buffer = null;
			}
			else {
				writer.WriteString("FAIL");
				writer.Write(10);
				writer.Write("fbc is null, you have superuser?");
			}
		}
		catch (Exception e) {
			LogCenter.error("ScreenShot", e.getMessage());
			writer.WriteString("FAIL");
			writer.Write(10);
			writer.Write(e.getMessage());
		}

	}

	private void getRawImage(ByteReader reader, ByteWriter writer) {
		FrameBufferController fbc = null;
		try {
			fbc = FrameBufferController.instance(mContext);
			if (fbc != null) {
				FrameBufferController.ScreenInfo si = fbc.get_si();
				if (si != null) {
					writer.WriteString("OKAY");
					writer.Write(si.bpp);
					writer.Write(si.width);
					writer.Write(si.height);
					writer.Write(si.red_offset);
					writer.Write(si.red_length);
					writer.Write(si.blue_offset);
					writer.Write(si.blue_length);
					writer.Write(si.green_offset);
					writer.Write(si.green_length);
					writer.Write(si.alpha_offset);
					writer.Write(si.alpha_length);
				}
				else {
					writer.WriteString("FAIL");
				}
			}
			else {
				writer.WriteString("FAIL");
			}
		}
		catch (Exception e) {
			LogCenter.error("ScreenShot", e.getMessage());
			writer.WriteString("FAIL");
		}
	}

	private void execForString(ByteReader reader, ByteWriter writer) {
		String cmd = reader.ReadString();
		String result = Shell.getInstance().execForString(cmd);
		writer.Write(1);
		writer.Write(true);
		writer.Write(result);
	}

	private void execForBytes(ByteReader reader, ByteWriter writer) {
		String cmd = reader.ReadString();
		// LogCenter.error("ExecShell", "Command: " + cmd);
		byte[] result = Shell.getInstance().execForBytes(cmd);
		// Shell.getInstance().exec(cmd);
		writer.Write(2);
		writer.Write(true);

		if (result != null) {
			writer.Write(result.length);
			writer.Write(result);
		}
		else {
			writer.Write(0);
		}
		// LogCenter.error("ExecShell", "lenth: " + String.valueOf(result.length));
	}

	private void getScreenSize(ByteReader reader, ByteWriter writer) {
		Display display = ((WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay();
		writer.Write(display.getWidth());
		writer.Write(display.getHeight());
	}

	private void getSdcardPath(ByteReader reader, ByteWriter writer) {
		writer.Write(Environment.getExternalStorageDirectory().getPath());
	}

	private void getNdshStatus(ByteWriter writer) {
		boolean blnRunning = NdkManager.isRunning(mContext);
		boolean blnRunInRoot = NdkManager.isRunInRoot(mContext);
		writer.Write(blnRunning);
		writer.Write(blnRunInRoot);
	}

	private void suAuthorize(ByteWriter writer) {
		boolean status = NdkManager.suAuthorize(mContext);
		writer.Write(status);
	}

	@Override
	public int getBusiness() {
		return 17;
	}
}
