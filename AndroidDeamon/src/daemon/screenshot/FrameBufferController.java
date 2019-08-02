package daemon.screenshot;


import java.io.File;
import java.io.FileInputStream;
import java.nio.ByteOrder;
import java.nio.ByteBuffer;

import android.content.Context;



import daemon.ndk.NdkManager;
import daemon.shell.Su;

public class FrameBufferController {
	private final static String FB_PATH = "/dev/graphics/fb0";
	private final static String ENABLE_FB_ACCESS_CMD = "chmod 666 " + FB_PATH;
	private final static String DISABLE_FB_ACCESS_CMD = "chmod 660 " + FB_PATH;
	private final static String CMD_GET_SI = "ioctl -l 84 /dev/graphics/fb0 0x4600\n";
	private final static String CMD_GET_VI = "ioctl -rl 50 /dev/graphics/fb0 0x4602\n";

	private static FrameBufferController instance_;
	private Su su;
	private ScreenInfo si_;

	private Context mContext;

	private FrameBufferController(Context context) {
		this.mContext = context;
	}

	private void enable_fb_access() {
		su.getSuAuthorize(mContext);
		su.exec(ENABLE_FB_ACCESS_CMD);
	}

	private void disable_fb_access() {
		su.getSuAuthorize(mContext);
		su.exec(DISABLE_FB_ACCESS_CMD);
	}

	public class ScreenInfo {
		public int alpha_length;
		public int alpha_offset;
		public int blue_length;
		public int blue_offset;
		public int green_length;
		public int green_offset;
		public int red_length;
		public int red_offset;
		public int bpp;
		public int width;
		public int height;
	    public int line_length;
	}

	private ScreenInfo convert_buf_to_si(StringBuffer sb) {
		ByteBuffer bb = ByteBuffer.allocate(84);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		for (int i = 0, l = sb.length(); i < l; i += 3) {
			bb.put(Integer.valueOf(sb.substring(i, i + 2), 16).byteValue());
		}

		bb.flip();
		ScreenInfo si = new ScreenInfo();
		si.width = bb.getInt();
		si.height = bb.getInt();
		bb.position(bb.position() + 4 * 4);
		si.bpp = bb.getInt();
		bb.position(bb.position() + 4);
		si.red_offset = bb.getInt();
		si.red_length = bb.getInt();
		bb.position(bb.position() + 4);
		si.green_offset = bb.getInt();
		si.green_length = bb.getInt();
		bb.position(bb.position() + 4);
		si.blue_offset = bb.getInt();
		si.blue_length = bb.getInt();
		bb.position(bb.position() + 4);
		si.alpha_offset = bb.getInt();
		si.alpha_length = bb.getInt();

		return si;
	}
	
	private void convert_buf_to_vi(ScreenInfo si,StringBuffer sb) {

		ByteBuffer bb = ByteBuffer.allocate(6);
		bb.order(ByteOrder.LITTLE_ENDIAN);
		for (int i = 44*3, l = sb.length(); i < l; i += 3) {
			bb.put(Integer.valueOf(sb.substring(i, i + 2), 16).byteValue());
		}
		bb.flip();
		si_.line_length = bb.getInt();
	}

	private boolean init() {
		enable_fb_access();

		int i = 0;
		for (; i < 10; ++i) {
			File fb0 = new File(FB_PATH);
			if (fb0.canRead()) {
				fb0 = null;
				break;
			}

			fb0 = null;

			try {
				Thread.sleep(50);
			} catch (InterruptedException ie) {
			}
		}

		if (i >= 10) {
			return false;
		}

		String si_buf = su.exec_with_str_result(CMD_GET_SI, 10000);
		// exec_with_str_result(CMD_GET_SI);
		if (null == si_buf) {
			return false;
		}

		int offset = si_buf.lastIndexOf("return buf: ");
		if (-1 == offset) {
			return false;
		}

		si_ = convert_buf_to_si(new StringBuffer(si_buf.substring(offset
				+ "return buf: ".length())));

		if (null == si_) {
			return false;
		}
		
		String vi_buf = su.exec_with_str_result(CMD_GET_VI, 10000);
		if (null == vi_buf) {
			return false;
		}

		offset = vi_buf.lastIndexOf("return buf: ");
		if (-1 == offset) {
			return false;
		}

		convert_buf_to_vi(si_ , new StringBuffer(vi_buf.substring(offset
				+ "return buf: ".length())));


		return true;
	}

	protected void finalize() {
		disable_fb_access();
	}

	public byte[] screenshot(boolean toBitmap565) {
		byte[] result = NdkManager.srceenshot(mContext, toBitmap565);

		return result;
	}

	public ByteBuffer grab_frame_buffer() {
		if (si_ != null) {
			File fb0 = new File(FB_PATH);
			try {
				int deepth = (si_.bpp + 7) >> 3;
                if(si_.line_length/deepth>si_.width){
    				int length = si_.line_length* si_.height ;
    				ByteBuffer buffer = ByteBuffer.allocate(length);
    				FileInputStream fis = new FileInputStream(fb0);
    				fis.getChannel().read(buffer);
    				byte filebyte[] =buffer.array();
    				int line_lenght = si_.width * deepth;
    				byte[] colors = new byte[si_.height*line_lenght];
    				for (int i = 0; i < si_.height; i++) {
    					int pos= si_.line_length*i;
    					for (int j = 0; j < line_lenght; j++) {
    						colors[i*line_lenght+j]=filebyte[pos+j];
    					}
    				}
    				buffer = ByteBuffer.wrap(colors);
    				return buffer;
                }else{
    				int length = si_.width * si_.height * deepth;
    				ByteBuffer buffer = ByteBuffer.allocate(length);
    				FileInputStream fis = new FileInputStream(fb0);
    				fis.getChannel().read(buffer);
    				return buffer;
                }

			} catch (Exception e) {
				e.printStackTrace();
			}
		}
		
		return null;
	}
	public ScreenInfo get_si() {
		return si_;
	}

	public static FrameBufferController instance(Context context)
			throws IllegalAccessException {
		if (null == instance_) {
			instance_ = new FrameBufferController(context);
			instance_.su = Su.getInstance();
			try {
				Thread.sleep(500);
			} catch (InterruptedException e) {
			}

			if (!instance_.init()) {
				// instance_ = null;
				// throw new IllegalAccessException("file:fb0 access denied!");
			}
		}

		return instance_;
	}
}
