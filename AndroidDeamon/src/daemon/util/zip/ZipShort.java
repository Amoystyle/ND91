package daemon.util.zip;

public final class ZipShort
  implements Cloneable
{
  private int value;

  public ZipShort(int value)
  {
    this.value = value;
  }

  public ZipShort(byte[] bytes)
  {
    this(bytes, 0);
  }

  public ZipShort(byte[] bytes, int offset)
  {
    this.value = (bytes[(offset + 1)] << 8 & 0xFF00);
    this.value += (bytes[offset] & 0xFF);
  }

  public byte[] getBytes()
  {
    byte[] result = new byte[2];
    result[0] = (byte)(this.value & 0xFF);
    result[1] = (byte)((this.value & 0xFF00) >> 8);
    return result;
  }

  public int getValue()
  {
    return this.value;
  }

  public boolean equals(Object o)
  {
    if ((o == null) || (!(o instanceof ZipShort))) {
      return false;
    }
    return this.value == ((ZipShort)o).getValue();
  }

  public int hashCode()
  {
    return this.value;
  }
}