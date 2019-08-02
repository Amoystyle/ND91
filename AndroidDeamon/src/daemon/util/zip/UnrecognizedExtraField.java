package daemon.util.zip;

public class UnrecognizedExtraField
  implements ZipExtraField
{
  private ZipShort headerId;
  private byte[] localData;
  private byte[] centralData;

  public void setHeaderId(ZipShort headerId)
  {
    this.headerId = headerId;
  }

  public ZipShort getHeaderId() {
    return this.headerId;
  }

  public void setLocalFileDataData(byte[] data)
  {
    this.localData = data;
  }

  public ZipShort getLocalFileDataLength() {
    return new ZipShort(this.localData.length);
  }

  public byte[] getLocalFileDataData() {
    return this.localData;
  }

  public void setCentralDirectoryData(byte[] data)
  {
    this.centralData = data;
  }

  public ZipShort getCentralDirectoryLength() {
    if (this.centralData != null) {
      return new ZipShort(this.centralData.length);
    }
    return getLocalFileDataLength();
  }

  public byte[] getCentralDirectoryData() {
    if (this.centralData != null) {
      return this.centralData;
    }
    return getLocalFileDataData();
  }

  public void parseFromLocalFileData(byte[] data, int offset, int length) {
    byte[] tmp = new byte[length];
    System.arraycopy(data, offset, tmp, 0, length);
    setLocalFileDataData(tmp);
  }
}