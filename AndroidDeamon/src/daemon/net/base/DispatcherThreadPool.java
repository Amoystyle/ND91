package daemon.net.base;

import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.SynchronousQueue;

//Nio线程池
public class DispatcherThreadPool {

	private static final int CORE_POOL_SIZE = 2;
	private static final int MAX_POOL_SIZE = 20;
	private static final int KEEP_ALIVE_TIME = 120;
	
	private static DispatcherThreadPool mInstance = null;
	
	public synchronized static DispatcherThreadPool getInstance(){
		if(mInstance == null)
			mInstance = new DispatcherThreadPool();
		
		return mInstance;
	}
	
	private  DispatcherThreadPool()
	{
		mTaskQueue = new SynchronousQueue<Runnable>();
		mExecutor = new ThreadPoolExecutor(CORE_POOL_SIZE, MAX_POOL_SIZE,
				KEEP_ALIVE_TIME, TimeUnit.SECONDS, mTaskQueue,
				new ThreadPoolExecutor.CallerRunsPolicy());
		mExecutor.setThreadFactory(mFactory);
	}
	
	//通过线程池执行调度器Dispatcher
	public void accept(Runnable command)
	{
		mExecutor.execute(command);
	}
	
	//关闭线程池
	public void shutdown()
	{
		mExecutor.shutdown();
		try 
		{
			mExecutor.awaitTermination(60, TimeUnit.SECONDS);
		} 
		catch (InterruptedException e) 
		{
			e.printStackTrace();
		}
	}
	
	//线程创建工厂
	private ThreadFactory mFactory = new ThreadFactory()
	{
		int pool_thr_counter = 1;
		public Thread newThread(Runnable r)
		{
			Thread thr = new Thread(r);
			thr.setName("Pool Thread(" + Integer.toString(pool_thr_counter++) + ")");
			return thr;
		}
	};
	
	private ThreadPoolExecutor mExecutor;
	private SynchronousQueue<Runnable> mTaskQueue;
}
