package daemon.util.zip;

public final class ZipLong
  implements Cloneable
{
  private long value;

  public ZipLong(long value)
  {
    this.value = value;
  }

  public ZipLong(byte[] bytes)
  {
    this(bytes, 0);
  }

  public ZipLong(byte[] bytes, int offset)
  {
    this.value = (bytes[(offset + 3)] << 24 & 0xFF000000);
    this.value += (bytes[(offset + 2)] << 16 & 0xFF0000);
    this.value += (bytes[(offset + 1)] << 8 & 0xFF00);
    this.value += (bytes[offset] & 0xFF);
  }

  public byte[] getBytes()
  {
    byte[] result = new byte[4];
    result[0] = (byte)(int)(this.value & 0xFF);
    result[1] = (byte)(int)((this.value & 0xFF00) >> 8);
    result[2] = (byte)(int)((this.value & 0xFF0000) >> 16);
    result[3] = (byte)(int)((this.value & 0xFF000000) >> 24);
    return result;
  }

  public long getValue()
  {
    return this.value;
  }

  public boolean equals(Object o)
  {
    if ((o == null) || (!(o instanceof ZipLong))) {
      return false;
    }
    return this.value == ((ZipLong)o).getValue();
  }

  public int hashCode()
  {
    return (int)this.value;
  }
}