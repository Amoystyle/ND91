package daemon.ndk;

public class DataPackage {
	private BusinessType mBusiness = BusinessType.INIT_C2JAVA;
	private byte[] mData = null;
	private boolean mIsTimeOut = false;

	public DataPackage(BusinessType business, byte[] data, boolean isTimeOut) {
		mBusiness = business;
		mData = data;
		mIsTimeOut = isTimeOut;
	}

	public byte getBusinessId() {
		return mBusiness.getValue();
	}
	
	public BusinessType getBusiness(){
		return mBusiness;
	}

	public byte[] getData() {
		return mData;
	}

	public boolean isTimeOut() {
		return mIsTimeOut;
	}

	// 生成协议包
	public byte[] getProtocalPackage() {
		int dataLen = 0;
		if(mData != null)
			dataLen = mData.length;
		
		byte[] pack = new byte[9 + dataLen];
		pack[0] = 'N';
		pack[1] = 'D';
		pack[2] = 'P';
		pack[3] = 'C';

		pack[4] = mBusiness.getValue();

		if (dataLen > 0) {
			pack[5] = (byte) (dataLen & 0x000000FF);
			pack[6] = (byte) ((dataLen & 0x0000FF00) >> 8);
			pack[7] = (byte) ((dataLen & 0x00FF0000) >> 16);
			pack[8] = (byte) ((dataLen & 0xFF000000) >> 24);

			System.arraycopy(mData, 0, pack, 9, mData.length);
		}
	
		return pack;
	}
}
