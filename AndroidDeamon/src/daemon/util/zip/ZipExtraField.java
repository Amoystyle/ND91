package daemon.util.zip;

import java.util.zip.ZipException;

public abstract interface ZipExtraField
{
  public abstract ZipShort getHeaderId();

  public abstract ZipShort getLocalFileDataLength();

  public abstract ZipShort getCentralDirectoryLength();

  public abstract byte[] getLocalFileDataData();

  public abstract byte[] getCentralDirectoryData();

  public abstract void parseFromLocalFileData(byte[] paramArrayOfByte, int paramInt1, int paramInt2)
    throws ZipException;
}