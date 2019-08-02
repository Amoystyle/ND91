package daemon.util.zip;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.io.RandomAccessFile;
import java.io.UnsupportedEncodingException;
import java.util.Calendar;
import java.util.Date;
import java.util.Enumeration;
import java.util.Hashtable;
import java.util.zip.Inflater;
import java.util.zip.InflaterInputStream;
import java.util.zip.ZipException;

public class ZipFile
{
  private Hashtable entries = new Hashtable();

  private Hashtable nameMap = new Hashtable();

  private Hashtable dataOffsets = new Hashtable();

  private String encoding = null;
  private RandomAccessFile archive;
  private static final int CFH_LEN = 42;
  private static final int MIN_EOCD_SIZE = 22;
  private static final int CFD_LOCATOR_OFFSET = 16;
  private static final long LFH_OFFSET_FOR_FILENAME_LENGTH = 26L;

  public ZipFile(File f)
    throws IOException
  {
    this(f, null);
  }

  public ZipFile(String name)
    throws IOException
  {
    this(new File(name), null);
  }

  public ZipFile(String name, String encoding)
    throws IOException
  {
    this(new File(name), encoding);
  }

  public ZipFile(File f, String encoding)
    throws IOException
  {
    this.encoding = encoding;
    this.archive = new RandomAccessFile(f, "r");
    try {
      populateFromCentralDirectory();
      resolveLocalFileHeaderData();
    } catch (IOException e) {
      try {
        this.archive.close();
      }
      catch (IOException e2) {
      }
      throw e;
    }
  }

  public String getEncoding()
  {
    return this.encoding;
  }

  public void close()
    throws IOException
  {
    this.archive.close();
  }

  public Enumeration getEntries()
  {
    return this.entries.keys();
  }

  public ZipEntry getEntry(String name)
  {
    return (ZipEntry)this.nameMap.get(name);
  }

  public InputStream getInputStream(ZipEntry ze)
    throws IOException, ZipException
  {
    Long start = (Long)this.dataOffsets.get(ze);
    if (start == null) {
      return null;
    }
    BoundedInputStream bis = new BoundedInputStream(start.longValue(), ze.getCompressedSize());

    switch (ze.getMethod())
    {
    case 0:
      return bis;
    case 8:
      bis.addDummy();
      return new InflaterInputStream(bis, new Inflater(true));
    }
    throw new ZipException("Found unsupported compression method " + ze.getMethod());
  }

  private void populateFromCentralDirectory()
    throws IOException
  {
    positionAtCentralDirectory();

    byte[] cfh = new byte[42];

    byte[] signatureBytes = new byte[4];
    this.archive.readFully(signatureBytes);
    ZipLong sig = new ZipLong(signatureBytes);
    while (sig.equals(ZipOutputStream.CFH_SIG)) {
      this.archive.readFully(cfh);
      int off = 0;
      ZipEntry ze = new ZipEntry();

      ZipShort versionMadeBy = new ZipShort(cfh, off);
      off += 2;
      ze.setPlatform(versionMadeBy.getValue() >> 8 & 0xF);

      off += 4;

      ze.setMethod(new ZipShort(cfh, off).getValue());
      off += 2;

      ze.setTime(fromDosTime(new ZipLong(cfh, off)).getTime());
      off += 4;

      ze.setCrc(new ZipLong(cfh, off).getValue());
      off += 4;

      ze.setCompressedSize(new ZipLong(cfh, off).getValue());
      off += 4;

      ze.setSize(new ZipLong(cfh, off).getValue());
      off += 4;

      int fileNameLen = new ZipShort(cfh, off).getValue();
      off += 2;

      int extraLen = new ZipShort(cfh, off).getValue();
      off += 2;

      int commentLen = new ZipShort(cfh, off).getValue();
      off += 2;

      off += 2;

      ze.setInternalAttributes(new ZipShort(cfh, off).getValue());
      off += 2;

      ze.setExternalAttributes(new ZipLong(cfh, off).getValue());
      off += 4;

      this.entries.put(ze, new Long(new ZipLong(cfh, off).getValue()));

      byte[] fileName = new byte[fileNameLen];
      this.archive.readFully(fileName);
      ze.setName(getString(fileName));

      this.nameMap.put(ze.getName(), ze);

      this.archive.skipBytes(extraLen);

      byte[] comment = new byte[commentLen];
      this.archive.readFully(comment);
      ze.setComment(getString(comment));

      this.archive.readFully(signatureBytes);
      sig = new ZipLong(signatureBytes);
    }
  }

  private void positionAtCentralDirectory()
    throws IOException
  {
    long off = this.archive.length() - 22L;
    this.archive.seek(off);
    byte[] sig = ZipOutputStream.EOCD_SIG.getBytes();
    int curr = this.archive.read();
    boolean found = false;
    while (curr != -1) {
      if (curr == sig[0]) {
        curr = this.archive.read();
        if (curr == sig[1]) {
          curr = this.archive.read();
          if (curr == sig[2]) {
            curr = this.archive.read();
            if (curr == sig[3]) {
              found = true;
              break;
            }
          }
        }
      }
      this.archive.seek(--off);
      curr = this.archive.read();
    }
    if (!found) {
      throw new ZipException("archive is not a ZIP archive");
    }
    this.archive.seek(off + 16L);
    byte[] cfdOffset = new byte[4];
    this.archive.readFully(cfdOffset);
    this.archive.seek(new ZipLong(cfdOffset).getValue());
  }

  private void resolveLocalFileHeaderData()
    throws IOException
  {
    Enumeration e = getEntries();
    while (e.hasMoreElements()) {
      ZipEntry ze = (ZipEntry)e.nextElement();
      long offset = ((Long)this.entries.get(ze)).longValue();
      this.archive.seek(offset + 26L);
      byte[] b = new byte[2];
      this.archive.readFully(b);
      int fileNameLen = new ZipShort(b).getValue();
      this.archive.readFully(b);
      int extraFieldLen = new ZipShort(b).getValue();
      this.archive.skipBytes(fileNameLen);
      byte[] localExtraData = new byte[extraFieldLen];
      this.archive.readFully(localExtraData);
      ze.setExtra(localExtraData);
      this.dataOffsets.put(ze, new Long(offset + 26L + 2L + 2L + fileNameLen + extraFieldLen));
    }
  }

  protected static Date fromDosTime(ZipLong l)
  {
    long dosTime = l.getValue();
    Calendar cal = Calendar.getInstance();
    cal.set(1, (int)(dosTime >> 25 & 0x7F) + 1980);
    cal.set(2, (int)(dosTime >> 21 & 0xF) - 1);
    cal.set(5, (int)(dosTime >> 16) & 0x1F);
    cal.set(11, (int)(dosTime >> 11) & 0x1F);
    cal.set(12, (int)(dosTime >> 5) & 0x3F);
    cal.set(13, (int)(dosTime << 1) & 0x3E);
    return cal.getTime();
  }

  protected String getString(byte[] bytes)
    throws ZipException
  {
    if (this.encoding == null)
      return new String(bytes);
    try
    {
      return new String(bytes, this.encoding);
    } catch (UnsupportedEncodingException uee) {
      throw new ZipException(uee.getMessage());
    }
  }

  private class BoundedInputStream extends InputStream
  {
    private long remaining;
    private long loc;
    private boolean addDummyByte = false;

    BoundedInputStream(long start, long remaining) {
      this.remaining = remaining;
      this.loc = start;
    }

    public int read() throws IOException {
      if (this.remaining-- <= 0L) {
        if (this.addDummyByte) {
          this.addDummyByte = false;
          return 0;
        }
        return -1;
      }
      synchronized (ZipFile.this.archive) {
        ZipFile.this.archive.seek(this.loc++);
        return ZipFile.this.archive.read();
      }
    }

    public int read(byte[] b, int off, int len) throws IOException {
      if (this.remaining <= 0L) {
        if (this.addDummyByte) {
          this.addDummyByte = false;
          b[off] = 0;
          return 1;
        }
        return -1;
      }

      if (len <= 0) {
        return 0;
      }

      if (len > this.remaining) {
        len = (int)this.remaining;
      }
      int ret = -1;
      synchronized (ZipFile.this.archive) {
        ZipFile.this.archive.seek(this.loc);
        ret = ZipFile.this.archive.read(b, off, len);
      }
      if (ret > 0) {
        this.loc += ret;
        this.remaining -= ret;
      }
      return ret;
    }

    void addDummy()
    {
      this.addDummyByte = true;
    }
  }
}