package daemon.provider.media;

import daemon.protocol.ByteReader;
import daemon.protocol.ByteWriter;
import daemon.protocol.IBaseEntity;


public class PhotoBucket implements IBaseEntity {
	private String bucket_id;
	private String display_name;
	private String path;
	private int visible;

	public String getBucketId() {
		return bucket_id;
	}

	public void setBucketId(String bucketId) {
		this.bucket_id = bucketId;
	}

	public String getDisplayName() {
		return display_name;
	}

	public void setDisplayName(String displayName) {
		this.display_name = displayName;
	}

	public String getPath() {
		return path;
	}

	public void setPath(String path) {
		this.path = path;
	}

	public int getVisible() {
		return visible;
	}

	public void setVisible(int visible) {
		this.visible = visible;
	}

	@Override
	public Integer GetAction() {
		return 0;
	}

	@Override
	public void Read(ByteReader reader) {
		bucket_id = reader.ReadString();
		display_name = reader.ReadString();
		path = reader.ReadString();
		visible = reader.ReadInt32();
	}

	@Override
	public void Write(ByteWriter writer) {
		writer.Write(bucket_id);
		writer.Write(display_name);
		writer.Write(path);
		writer.Write(visible);
	}
}