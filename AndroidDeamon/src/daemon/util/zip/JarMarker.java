package daemon.util.zip;

import java.util.zip.ZipException;

public final class JarMarker
  implements ZipExtraField
{
  private static ZipShort ID = new ZipShort(51966);
  private static ZipShort NULL = new ZipShort(0);
  private static byte[] NO_BYTES = new byte[0];
  private static JarMarker DEFAULT = new JarMarker();

  public static JarMarker getInstance()
  {
    return DEFAULT;
  }

  public ZipShort getHeaderId()
  {
    return ID;
  }

  public ZipShort getLocalFileDataLength()
  {
    return NULL;
  }

  public ZipShort getCentralDirectoryLength()
  {
    return NULL;
  }

  public byte[] getLocalFileDataData()
  {
    return NO_BYTES;
  }

  public byte[] getCentralDirectoryData()
  {
    return NO_BYTES;
  }

  public void parseFromLocalFileData(byte[] data, int offset, int length)
    throws ZipException
  {
    if (length != 0)
      throw new ZipException("JarMarker doesn't expect any data");
  }
}