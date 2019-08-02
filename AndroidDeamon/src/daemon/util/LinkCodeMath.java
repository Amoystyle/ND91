package daemon.util;

public class LinkCodeMath {
	private final static String BASESTR_NEW = "123456789ABCDEFGHJKLMNPQRSTUVWXYZ=";

	//新方案解析连接码
	public static String AnalyzeCode(String code)
	{
		int radix = BASESTR_NEW.length();
		long totalValue = 0;

		//将连接码换算成十进制
		for (int i = code.length() - 1; i >= 0; i--)
		{
			char c = code.charAt(i);
			int index = BASESTR_NEW.indexOf(c);
			if (index >= 0){
				totalValue += index * (long)Math.pow(radix, code.length() - i - 1);
			}else{
				return null;
			}
				
		}
		String base2 = Long.toString(totalValue, 2);
		if (base2.length() == 36)//对应二进制必须是刚好36位
		{
			String platBase2 = base2.substring(0, 4);
			if(!"1010".equals(platBase2)){
				return null;
			}
			//读取ip
			base2 = base2.substring(4);
			totalValue = Long.valueOf(base2, 2);
			int ip1 = (int)(totalValue >> 24) & 0xFF;
			int ip2 = (int)(totalValue >> 16) & 0xFF;
			int ip3 = (int)(totalValue >> 8) & 0xFF;
			int ip4 = (int)totalValue & 0xFF;
			String ip = ip1 + "." + ip2 + "." + ip3 + "." + ip4;
			return ip;
		}
		else
		{
			return null;
		}
	}
	
	public static String getLinkCode(String ip) {
		if(ip == null)
			return null;		
		
		String[] ipArray = ip.split("\\.");

		int ip1 = Integer.parseInt(ipArray[0]);
		int ip2 = Integer.parseInt(ipArray[1]);
		int ip3 = Integer.parseInt(ipArray[2]);
		int ip4 = Integer.parseInt(ipArray[3]);	

		long  totalValue = 0;
		totalValue |= (ip1 & 0x000000FFL) << 24;
		totalValue |= (ip2 & 0x000000FFL) << 16;
		totalValue |= (ip3 & 0x000000FFL) << 8;
		totalValue |= (ip4 & 0x000000FFL);
		
		String base2Str = Long.toString(totalValue, 2);		
		
		base2Str = "1001" + padLeft(base2Str, 32, "0"); //转二进制并加上平台参数“1001”	
		
		totalValue = Long.valueOf(base2Str, 2);
		
		//34进制换算
		String code =convetrtToStr(totalValue, BASESTR_NEW);
	
		return code;
	}
	
	//进制换算
	private static String convetrtToStr(long value, String baseStr){
		if (value > 0)
		{
			int radix = baseStr.length();
			return convetrtToStr(value / radix, baseStr) + baseStr.charAt((int)(value % radix));
		}
		else
		{
			return "";
		}
	}
	
	private static String padLeft(String content, int size, String padChar){
		while(content.length() < size){
			content = padChar + content;
		}
			
		return content;
	}
}
