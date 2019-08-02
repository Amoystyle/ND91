package daemon.util;

public class BRMath {	
		private final static String BASESTR32 = "123456789ABCDEFGHIJKLMNoPQRSTUVWXYZ=";

		public static String createLinkCode(String cip, PlatformType plat) {
			if (cip == null) 
				return "";
			String lCode = "";
			String[] sCip = cip.split("\\.");
			// 获取可用的iP地址
			int ip1 = 0;
			int ip2 = 0;

			if (sCip.length > 1) {
				ip1 = Integer.parseInt(sCip[sCip.length - 2]);
				ip2 = Integer.parseInt(sCip[sCip.length - 1]);

			}
			int intCode1 = ip1 + plat.getValue() * 255;
			int intCode2 = ip2 + plat.getValue() * 255;

			String mCode = convertToStr(intCode1, (byte)36);
			mCode = padLeft(mCode, 2, '0');
			String mCode2 = convertToStr(intCode2, (byte)36);
			mCode2 = padLeft(mCode2, 2, '0');

			lCode = mCode + mCode2;

			return lCode;
		}
		
		public static String GetLinkIP(String cCode)
		{
			String newIp = "";		

			for (int i = 0; i < cCode.length(); i += 2)
			{
				char code1 = cCode.charAt(i);
				char code2 = cCode.charAt(i+1);

				int intCheckCode = 0;
				int intCode = BASESTR32.indexOf(code1);
				int intCode2 = BASESTR32.indexOf(code2);
				if (intCode > -1 && intCode2 > -1)
				{
					intCheckCode = intCode * 36 + intCode2;
				}

				//
				long ip = intCheckCode % 255;
				newIp += ip + ".";
			}

			if(newIp.endsWith(".")){
				newIp = newIp.substring(0, newIp.length() -1);
			}
			
			return newIp;
		}
		
		private static String padLeft(String s, int len, char paddingChar){
			while(s.length() < len){
				s = paddingChar + s;
			}
			
			return s;
		}

		private static String convertToStr(int ival, Byte encode) {
			int start = 0;

			if ((ival >= 0) && (ival < encode)) {
				start = ival;
				return BASESTR32.substring(start, start + 1);
			}

			start = ival % encode;
			return convertToStr(ival / encode, encode)
					+ BASESTR32.substring(start, start + 1);
		}

		public enum PlatformType {
			iOS(0), Android(1), Symbian(2), WM(3), WP7(4);

			private int mValue;

			PlatformType(int value) {
				mValue = value;
			}

			public int getValue() {
				return mValue;
			}
		}
}
