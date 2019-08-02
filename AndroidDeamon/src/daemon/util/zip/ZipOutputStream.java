package daemon.util.zip;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FilterOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.RandomAccessFile;
import java.io.UnsupportedEncodingException;
import java.util.Date;
import java.util.Hashtable;
import java.util.Vector;
import java.util.zip.CRC32;
import java.util.zip.Deflater;
import java.util.zip.ZipException;

public class ZipOutputStream extends FilterOutputStream
{
  private ZipEntry entry;
  private String comment = "";

  private int level = -1;

  private boolean hasCompressionLevelChanged = false;

  private int method = 8;

  private Vector entries = new Vector();

  private CRC32 crc = new CRC32();

  private long written = 0L;

  private long dataStart = 0L;

  private long localDataStart = 0L;

  private ZipLong cdOffset = new ZipLong(0L);

  private ZipLong cdLength = new ZipLong(0L);

  private static final byte[] ZERO = { 0, 0 };

  private static final byte[] LZERO = { 0, 0, 0, 0 };

  private Hashtable offsets = new Hashtable();

  private String encoding = null;

  protected Deflater def = new Deflater(-1, true);

  protected byte[] buf = new byte[512];

  private RandomAccessFile raf = null;
  public static final int DEFLATED = 8;
  public static final int STORED = 0;
  protected static final ZipLong LFH_SIG = new ZipLong(67324752L);

  protected static final ZipLong DD_SIG = new ZipLong(134695760L);

  protected static final ZipLong CFH_SIG = new ZipLong(33639248L);

  protected static final ZipLong EOCD_SIG = new ZipLong(101010256L);

  private static final ZipLong DOS_TIME_MIN = new ZipLong(8448L);

  public ZipOutputStream(OutputStream out)
  {
    super(out);
  }

  public ZipOutputStream(File file)
    throws IOException
  {
    super(null);
    try
    {
      this.raf = new RandomAccessFile(file, "rw");
      this.raf.setLength(0L);
    } catch (IOException e) {
      if (this.raf != null) {
        try {
          this.raf.close();
        }
        catch (IOException inner) {
        }
        this.raf = null;
      }
      this.out = new FileOutputStream(file);
    }
  }

  public boolean isSeekable()
  {
    return this.raf != null;
  }

  public void setEncoding(String encoding)
  {
    this.encoding = encoding;
  }

  public String getEncoding()
  {
    return this.encoding;
  }

  public void finish()
    throws IOException
  {
    closeEntry();
    this.cdOffset = new ZipLong(this.written);
    for (int i = 0; i < this.entries.size(); ++i) {
      writeCentralFileHeader((ZipEntry)this.entries.elementAt(i));
    }
    this.cdLength = new ZipLong(this.written - this.cdOffset.getValue());
    writeCentralDirectoryEnd();
    this.offsets.clear();
    this.entries.removeAllElements();
  }

  public void closeEntry()
    throws IOException
  {
    if (this.entry == null) {
      return;
    }

    long realCrc = this.crc.getValue();
    this.crc.reset();

    if (this.entry.getMethod() == 8) {
      this.def.finish();
      while (!this.def.finished()) {
        deflate();
      }

      this.entry.setSize(adjustToLong(this.def.getTotalIn()));
      this.entry.setComprSize(adjustToLong(this.def.getTotalOut()));
      this.entry.setCrc(realCrc);

      this.def.reset();

      this.written += this.entry.getCompressedSize();
    } else if (this.raf == null) {
      if (this.entry.getCrc() != realCrc) {
        throw new ZipException("bad CRC checksum for entry " + this.entry.getName() + ": " + Long.toHexString(this.entry.getCrc()) + " instead of " + Long.toHexString(realCrc));
      }

      if (this.entry.getSize() != this.written - this.dataStart) {
        throw new ZipException("bad size for entry " + this.entry.getName() + ": " + this.entry.getSize() + " instead of " + (this.written - this.dataStart));
      }

    }
    else
    {
      long size = this.written - this.dataStart;

      this.entry.setSize(size);
      this.entry.setComprSize(size);
      this.entry.setCrc(realCrc);
    }

    if (this.raf != null) {
      long save = this.raf.getFilePointer();

      this.raf.seek(this.localDataStart);
      writeOut(new ZipLong(this.entry.getCrc()).getBytes());
      writeOut(new ZipLong(this.entry.getCompressedSize()).getBytes());
      writeOut(new ZipLong(this.entry.getSize()).getBytes());
      this.raf.seek(save);
    }

    writeDataDescriptor(this.entry);
    this.entry = null;
  }

  public void putNextEntry(ZipEntry ze)
    throws IOException
  {
    closeEntry();

    this.entry = ze;
    this.entries.addElement(this.entry);

    if (this.entry.getMethod() == -1) {
      this.entry.setMethod(this.method);
    }

    if (this.entry.getTime() == -1L) {
      this.entry.setTime(System.currentTimeMillis());
    }

    if ((this.entry.getMethod() == 0) && (this.raf == null)) {
      if (this.entry.getSize() == -1L) {
        throw new ZipException("uncompressed size is required for STORED method when not writing to a file");
      }

      if (this.entry.getCrc() == -1L) {
        throw new ZipException("crc checksum is required for STORED method when not writing to a file");
      }

      this.entry.setComprSize(this.entry.getSize());
    }

    if ((this.entry.getMethod() == 8) && (this.hasCompressionLevelChanged)) {
      this.def.setLevel(this.level);
      this.hasCompressionLevelChanged = false;
    }
    writeLocalFileHeader(this.entry);
  }

  public void setComment(String comment)
  {
    this.comment = comment;
  }

  public void setLevel(int level)
  {
    this.hasCompressionLevelChanged = (this.level != level);
    this.level = level;
  }

  public void setMethod(int method)
  {
    this.method = method;
  }

  public void write(byte[] b, int offset, int length)
    throws IOException
  {
    if (this.entry.getMethod() == 8) {
      if ((length > 0) && 
        (!this.def.finished())) {
        this.def.setInput(b, offset, length);
        while (!this.def.needsInput())
          deflate();
      }
    }
    else
    {
      writeOut(b, offset, length);
      this.written += length;
    }
    this.crc.update(b, offset, length);
  }

  public void write(int b)
    throws IOException
  {
    byte[] buf = new byte[1];
    buf[0] = (byte)(b & 0xFF);
    write(buf, 0, 1);
  }

  public void close()
    throws IOException
  {
    finish();

    if (this.raf != null) {
      this.raf.close();
    }
    if (this.out != null)
      this.out.close();
  }

  public void flush()
    throws IOException
  {
    if (this.out != null)
      this.out.flush();
  }

  protected final void deflate()
    throws IOException
  {
    int len = this.def.deflate(this.buf, 0, this.buf.length);
    if (len > 0)
      writeOut(this.buf, 0, len);
  }

  protected void writeLocalFileHeader(ZipEntry ze)
    throws IOException
  {
    this.offsets.put(ze, new ZipLong(this.written));

    writeOut(LFH_SIG.getBytes());
    this.written += 4L;

    if ((ze.getMethod() == 8) && (this.raf == null))
    {
      writeOut(new ZipShort(20).getBytes());

      writeOut(new ZipShort(8).getBytes());
    } else {
      writeOut(new ZipShort(10).getBytes());
      writeOut(ZERO);
    }
    this.written += 4L;

    writeOut(new ZipShort(ze.getMethod()).getBytes());
    this.written += 2L;

    writeOut(toDosTime(new Date(ze.getTime())).getBytes());
    this.written += 4L;

    this.localDataStart = this.written;
    if ((ze.getMethod() == 8) || (this.raf != null)) {
      writeOut(LZERO);
      writeOut(LZERO);
      writeOut(LZERO);
    } else {
      writeOut(new ZipLong(ze.getCrc()).getBytes());
      writeOut(new ZipLong(ze.getSize()).getBytes());
      writeOut(new ZipLong(ze.getSize()).getBytes());
    }
    this.written += 12L;

    byte[] name = getBytes(ze.getName());
    writeOut(new ZipShort(name.length).getBytes());
    this.written += 2L;

    byte[] extra = ze.getLocalFileDataExtra();
    writeOut(new ZipShort(extra.length).getBytes());
    this.written += 2L;

    writeOut(name);
    this.written += name.length;

    writeOut(extra);
    this.written += extra.length;

    this.dataStart = this.written;
  }

  protected void writeDataDescriptor(ZipEntry ze)
    throws IOException
  {
    if ((ze.getMethod() != 8) || (this.raf != null)) {
      return;
    }
    writeOut(DD_SIG.getBytes());
    writeOut(new ZipLong(this.entry.getCrc()).getBytes());
    writeOut(new ZipLong(this.entry.getCompressedSize()).getBytes());
    writeOut(new ZipLong(this.entry.getSize()).getBytes());
    this.written += 16L;
  }

  protected void writeCentralFileHeader(ZipEntry ze)
    throws IOException
  {
    writeOut(CFH_SIG.getBytes());
    this.written += 4L;

    writeOut(new ZipShort(ze.getPlatform() << 8 | 0x14).getBytes());
    this.written += 2L;

    if ((ze.getMethod() == 8) && (this.raf == null))
    {
      writeOut(new ZipShort(20).getBytes());

      writeOut(new ZipShort(8).getBytes());
    } else {
      writeOut(new ZipShort(10).getBytes());
      writeOut(ZERO);
    }
    this.written += 4L;

    writeOut(new ZipShort(ze.getMethod()).getBytes());
    this.written += 2L;

    writeOut(toDosTime(new Date(ze.getTime())).getBytes());
    this.written += 4L;

    writeOut(new ZipLong(ze.getCrc()).getBytes());
    writeOut(new ZipLong(ze.getCompressedSize()).getBytes());
    writeOut(new ZipLong(ze.getSize()).getBytes());
    this.written += 12L;

    byte[] name = getBytes(ze.getName());
    writeOut(new ZipShort(name.length).getBytes());
    this.written += 2L;

    byte[] extra = ze.getCentralDirectoryExtra();
    writeOut(new ZipShort(extra.length).getBytes());
    this.written += 2L;

    String comm = ze.getComment();
    if (comm == null) {
      comm = "";
    }
    byte[] comment = getBytes(comm);
    writeOut(new ZipShort(comment.length).getBytes());
    this.written += 2L;

    writeOut(ZERO);
    this.written += 2L;

    writeOut(new ZipShort(ze.getInternalAttributes()).getBytes());
    this.written += 2L;

    writeOut(new ZipLong(ze.getExternalAttributes()).getBytes());
    this.written += 4L;

    writeOut(((ZipLong)this.offsets.get(ze)).getBytes());
    this.written += 4L;

    writeOut(name);
    this.written += name.length;

    writeOut(extra);
    this.written += extra.length;

    writeOut(comment);
    this.written += comment.length;
  }

  protected void writeCentralDirectoryEnd()
    throws IOException
  {
    writeOut(EOCD_SIG.getBytes());

    writeOut(ZERO);
    writeOut(ZERO);

    byte[] num = new ZipShort(this.entries.size()).getBytes();
    writeOut(num);
    writeOut(num);

    writeOut(this.cdLength.getBytes());
    writeOut(this.cdOffset.getBytes());

    byte[] data = getBytes(this.comment);
    writeOut(new ZipShort(data.length).getBytes());
    writeOut(data);
  }

  protected static ZipLong toDosTime(Date time)
  {
    int year = time.getYear() + 1900;
    int month = time.getMonth() + 1;
    if (year < 1980) {
      return DOS_TIME_MIN;
    }
    long value = year - 1980 << 25 | month << 21 | time.getDate() << 16 | time.getHours() << 11 | time.getMinutes() << 5 | time.getSeconds() >> 1;

    byte[] result = new byte[4];
    result[0] = (byte)(int)(value & 0xFF);
    result[1] = (byte)(int)((value & 0xFF00) >> 8);
    result[2] = (byte)(int)((value & 0xFF0000) >> 16);
    result[3] = (byte)(int)((value & 0xFF000000) >> 24);
    return new ZipLong(result);
  }

  protected byte[] getBytes(String name)
    throws ZipException
  {
    if (this.encoding == null)
      return name.getBytes();
    try
    {
      return name.getBytes(this.encoding);
    } catch (UnsupportedEncodingException uee) {
      throw new ZipException(uee.getMessage());
    }
  }

  protected final void writeOut(byte[] data)
    throws IOException
  {
    writeOut(data, 0, data.length);
  }

  protected final void writeOut(byte[] data, int offset, int length)
    throws IOException
  {
    if (this.raf != null)
      this.raf.write(data, offset, length);
    else
      this.out.write(data, offset, length);
  }

  protected static long adjustToLong(int i)
  {
    if (i < 0) {
      return 4294967296L + i;
    }
    return i;
  }
}