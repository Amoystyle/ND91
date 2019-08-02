package daemon.util;

import android.annotation.TargetApi;
import android.graphics.Bitmap;
import android.media.MediaMetadataRetriever;
import android.os.Build;

@TargetApi(Build.VERSION_CODES.GINGERBREAD_MR1)
public class GetVideoThumbnail {    
 
    public static Bitmap CreateVideoThumbnail(String filePath){
        Bitmap  bitmap = null;
        MediaMetadataRetriever retriever = new MediaMetadataRetriever();
        try {
            retriever.setDataSource(filePath);
            bitmap = retriever.getFrameAtTime(-1);
        } catch (IllegalArgumentException ex) {
            // Assume this is a corrupt video file
        } catch (RuntimeException ex) {
            // Assume this is a corrupt video file.
        } finally {
            try {
                retriever.release();
            } catch (RuntimeException ex) {
               
            }
        }
        if (bitmap == null) return null;
        
         // Scale down the bitmap if it's too large.
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();
        int dbHeight = (int)(height * 1.5);
        int max = Math.max(width, dbHeight);
        if (max > 150) {
            bitmap = Bitmap.createScaledBitmap(bitmap, 150, 100, true);
        }
       
        return bitmap;
    }
    

}
