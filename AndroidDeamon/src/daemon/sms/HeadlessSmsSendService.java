package daemon.sms;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class HeadlessSmsSendService extends Service
{
  public IBinder onBind(Intent paramIntent)
  {
    return null;
  }
}

