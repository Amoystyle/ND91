package daemon.util.zip;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.NoSuchElementException;
import java.util.Vector;
import java.util.zip.ZipException;

public class ZipEntry extends java.util.zip.ZipEntry
  implements Cloneable
{
  private static final int PLATFORM_UNIX = 3;
  private static final int PLATFORM_FAT = 0;
  private int internalAttributes = 0;
  private int platform = 0;
  private long externalAttributes = 0L;
  private Vector extraFields = new Vector();
  private String name = null;

  private Long compressedSize = null;

  private static Method setCompressedSizeMethod = null;

  private static Object lockReflection = new Object();

  private static boolean triedToGetMethod = false;

  public ZipEntry(String name)
  {
    super(name);
  }

  public ZipEntry(java.util.zip.ZipEntry entry)
    throws ZipException
  {
    super(entry.getName());
    init(entry);

  }
  
  private void init(java.util.zip.ZipEntry entry) throws ZipException{
	    setComment(entry.getComment());
	    setMethod(entry.getMethod());
	    setTime(entry.getTime());

	    long size = entry.getSize();
	    if (size > 0L) {
	      setSize(size);
	    }
	    long cSize = entry.getCompressedSize();
	    if (cSize > 0L) {
	      setComprSize(cSize);
	    }
	    long crc = entry.getCrc();
	    if (crc > 0L) {
	      setCrc(crc);
	    }

	    byte[] extra = entry.getExtra();
	    if (extra != null) {
	      setExtraFields(ExtraFieldUtils.parse(extra));
	    }
	    else
	      setExtra();
  }

  public ZipEntry(ZipEntry entry)
    throws ZipException
  {
	 super(entry.getName());
	init(entry);
    setInternalAttributes(entry.getInternalAttributes());
    setExternalAttributes(entry.getExternalAttributes());
    setExtraFields(entry.getExtraFields());
  }

  protected ZipEntry()
  {
    super("");
  }

  public Object clone()
  {
    try
    {
      ZipEntry e = (ZipEntry)super.clone();

      e.setName(getName());
      e.setComment(getComment());
      e.setMethod(getMethod());
      e.setTime(getTime());
      long size = getSize();
      if (size > 0L) {
        e.setSize(size);
      }
      long cSize = getCompressedSize();
      if (cSize > 0L) {
        e.setComprSize(cSize);
      }
      long crc = getCrc();
      if (crc > 0L) {
        e.setCrc(crc);
      }

      e.extraFields = ((Vector)this.extraFields.clone());
      e.setInternalAttributes(getInternalAttributes());
      e.setExternalAttributes(getExternalAttributes());
      e.setExtraFields(getExtraFields());
      return e;
    }
    catch (Throwable t)
    {
    }
    return null;
  }

  public int getInternalAttributes()
  {
    return this.internalAttributes;
  }

  public void setInternalAttributes(int value)
  {
    this.internalAttributes = value;
  }

  public long getExternalAttributes()
  {
    return this.externalAttributes;
  }

  public void setExternalAttributes(long value)
  {
    this.externalAttributes = value;
  }

  public void setUnixMode(int mode)
  {
    setExternalAttributes(mode << 16 | (((mode & 0x80) == 0) ? 1 : 0) | ((isDirectory()) ? 16 : 0));

    this.platform = 3;
  }

  public int getUnixMode()
  {
    return (int)(getExternalAttributes() >> 16 & 0xFFFF);
  }

  public int getPlatform()
  {
    return this.platform;
  }

  protected void setPlatform(int platform)
  {
    this.platform = platform;
  }

  public void setExtraFields(ZipExtraField[] fields)
  {
    this.extraFields.removeAllElements();
    for (int i = 0; i < fields.length; ++i) {
      this.extraFields.addElement(fields[i]);
    }
    setExtra();
  }

  public ZipExtraField[] getExtraFields()
  {
    ZipExtraField[] result = new ZipExtraField[this.extraFields.size()];
    this.extraFields.copyInto(result);
    return result;
  }

  public void addExtraField(ZipExtraField ze)
  {
    ZipShort type = ze.getHeaderId();
    boolean done = false;
    for (int i = 0; (!done) && (i < this.extraFields.size()); ++i) {
      if (((ZipExtraField)this.extraFields.elementAt(i)).getHeaderId().equals(type)) {
        this.extraFields.setElementAt(ze, i);
        done = true;
      }
    }
    if (!done) {
      this.extraFields.addElement(ze);
    }
    setExtra();
  }

  public void removeExtraField(ZipShort type)
  {
    boolean done = false;
    for (int i = 0; (!done) && (i < this.extraFields.size()); ++i) {
      if (((ZipExtraField)this.extraFields.elementAt(i)).getHeaderId().equals(type)) {
        this.extraFields.removeElementAt(i);
        done = true;
      }
    }
    if (!done) {
      throw new NoSuchElementException();
    }
    setExtra();
  }

  public void setExtra(byte[] extra)
    throws RuntimeException
  {
    try
    {
      setExtraFields(ExtraFieldUtils.parse(extra));
    } catch (Exception e) {
      throw new RuntimeException(e.getMessage());
    }
  }

  protected void setExtra()
  {
    super.setExtra(ExtraFieldUtils.mergeLocalFileDataData(getExtraFields()));
  }

  public byte[] getLocalFileDataExtra()
  {
    byte[] extra = getExtra();
    return (extra != null) ? extra : new byte[0];
  }

  public byte[] getCentralDirectoryExtra()
  {
    return ExtraFieldUtils.mergeCentralDirectoryData(getExtraFields());
  }

  public void setComprSize(long size)
  {
    if (haveSetCompressedSize())
      performSetCompressedSize(this, size);
    else
      this.compressedSize = new Long(size);
  }

  public long getCompressedSize()
  {
    if (this.compressedSize != null)
    {
      return this.compressedSize.longValue();
    }
    return super.getCompressedSize();
  }

  public String getName()
  {
    return (this.name == null) ? super.getName() : this.name;
  }

  public boolean isDirectory()
  {
    return getName().endsWith("/");
  }

  protected void setName(String name) {
    this.name = name;
  }

  private static boolean haveSetCompressedSize()
  {
    checkSCS();
    return setCompressedSizeMethod != null;
  }

  private static void performSetCompressedSize(ZipEntry ze, long size)
  {
    Long[] s = { new Long(size) };
    try {
      setCompressedSizeMethod.invoke(ze, s);
    } catch (InvocationTargetException ite) {
      Throwable nested = ite.getTargetException();
      throw new RuntimeException("Exception setting the compressed size of " + ze + ": " + nested.getMessage());
    }
    catch (Throwable other)
    {
      throw new RuntimeException("Exception setting the compressed size of " + ze + ": " + other.getMessage());
    }
  }

  private static void checkSCS()
  {
    if (!triedToGetMethod)
      synchronized (lockReflection) {
        triedToGetMethod = true;
        try {
        	try {
				setCompressedSizeMethod =Class.forName("java.util.zip.ZipEntry").getMethod("setCompressedSize", new Class[] { Long.TYPE });
			} catch (SecurityException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (ClassNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
          //setCompressedSizeMethod = class$java$util$zip$ZipEntry.getMethod("setCompressedSize", new Class[] { Long.TYPE });
        }
        catch (NoSuchMethodException nse)
        {
        }
      }
  }
}