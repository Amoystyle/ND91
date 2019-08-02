package daemon.util;

import java.security.Key;
import java.security.SecureRandom;
import javax.crypto.Cipher;
import javax.crypto.KeyGenerator; 

public class DesEncrypt 
{
	/**
	* ��ݲ������KEY
	*
	* @param strKey
	*/ 
	public void getKey(String strKey) 
	{
		try 
		{
			KeyGenerator _generator = KeyGenerator.getInstance("DES");
			_generator.init(new SecureRandom(strKey.getBytes()));
			this.key = _generator.generateKey();
			_generator = null;
		} catch (Exception e) 
		{
			e.printStackTrace();
		}
	}

	/**
	* ����String��������,String�������
	*
	* @param strMing
	* @return
	*/
	public String getEncString(String strMing) 
	{
		String strMi = "";
		try 
		{
			return byte2hex(getEncCode(strMing.getBytes()));
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		} 
		
		return strMi;
	}

	/**
	* ���� ��String��������,String�������
	*
	* @param strMi
	* @return
	*/
	public String getDesString(String strMi) 
	{
		String strMing = "";
		try 
		{
			return new String(getDesCode(hex2byte(strMi.getBytes())));	
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}
		
		return strMing;
	}

	/**
	* ������byte[]��������,byte[]�������
	*
	* @param byteS
	* @return
	*/
	private byte[] getEncCode(byte[] byteS) 
	{
		byte[] byteFina = null;
		Cipher cipher;
		
		try 
		{
			cipher = Cipher.getInstance("DES");
			cipher.init(Cipher.ENCRYPT_MODE, key);
			byteFina = cipher.doFinal(byteS);
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		} 
		finally 
		{
			cipher = null;
		}
		
		return byteFina;
	}

	/**
	* ������byte[]��������,��byte[]�������
	*
	* @param byteD
	* @return
	*/
	private byte[] getDesCode(byte[] byteD) 
	{
		Cipher cipher;
		byte[] byteFina = null;
		try 
		{
			cipher = Cipher.getInstance("DES");
			cipher.init(Cipher.DECRYPT_MODE, key);
			byteFina = cipher.doFinal(byteD);
		}
		catch (Exception e) 
		{
			e.printStackTrace();
		} 
		finally 
		{
			cipher = null;
		}
		
		return byteFina;
	}

	/**
	* ������ת�ַ�
	*
	* @param b
	* @return
	*/
	public static String byte2hex(byte[] b) 
	{
		String hs = "";
		String stmp = "";
		for (int n = 0; n < b.length; n++) 
		{
			// ����ת��ʮ����Ʊ�ʾ
			stmp = (java.lang.Integer.toHexString(b[n] & 0XFF));
			if (stmp.length() == 1)
				hs = hs + "0" + stmp;
			else
				hs = hs + stmp;
		}
		
		return hs.toUpperCase(); // ת�ɴ�д
	}

	public static byte[] hex2byte(byte[] b) 
	{
		if ((b.length % 2) != 0)
			throw new IllegalArgumentException("���Ȳ���ż��");
		byte[] b2 = new byte[b.length / 2];
		for (int n = 0; n < b.length; n += 2) 
		{
			String item = new String(b, n, 2);
			// }λһ�飬��ʾһ���ֽ�,�������ʾ��16�����ַ���ԭ��һ������ֽ�
			b2[n / 2] = (byte) Integer.parseInt(item, 16);
		}

		return b2;
	}
	
	private Key key;
}
