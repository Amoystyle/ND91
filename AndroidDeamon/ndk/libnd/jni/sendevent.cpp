#include "sendevent.h"

int touchfd = -1;
static void init_touch(char *device)
{
	if((touchfd = open(device, O_RDWR)) == -1)
	{
		printf("cannot open touch device %s\n", device);
	}
}
static void cleanup_touch()
{
	if(touchfd != -1)
	{
		close(touchfd);
	}
}
void TouchEvent(int down, int x, int y)
{
	struct input_event ev;

	memset(&ev, 0, sizeof(ev));
	gettimeofday(&ev.time,0);
	ev.type = EV_ABS;
	ev.code = 0x30;
	ev.value = down;
	if(write(touchfd, &ev, sizeof(ev)) < 0)
	{
		printf("write event failed, %s\n", strerror(errno));
	}

	gettimeofday(&ev.time,0);
	ev.type = EV_ABS;
	ev.code = 0x35;
	ev.value = x;
	if(write(touchfd, &ev, sizeof(ev)) < 0)
	{
		printf("write event failed, %s\n", strerror(errno));
	}

	gettimeofday(&ev.time,0);
	ev.type = EV_ABS;
	ev.code = 0x36;
	ev.value = y;
	if(write(touchfd, &ev, sizeof(ev)) < 0)
	{
		printf("write event failed, %s\n", strerror(errno));
	}

	gettimeofday(&ev.time,0);
	ev.type = EV_SYN;
	ev.code = 2;
	ev.value = 0;
	if(write(touchfd, &ev, sizeof(ev)) < 0)
	{
		printf("write event failed, %s\n", strerror(errno));
	}
	
	gettimeofday(&ev.time,0);
	ev.type = EV_SYN;
	ev.code = 0;
	ev.value = 0;
	if(write(touchfd, &ev, sizeof(ev)) < 0)
	{
		printf("write event failed, %s\n", strerror(errno));
	}
}

void WriteToTouchfd(struct input_event ev,int type,int code,int valve)
{
   	gettimeofday(&ev.time,0);
	ev.type = type;
	ev.code = code;
	ev.value = valve;
	if(write(touchfd, &ev, sizeof(ev)) < 0)
	{
		ErrorNdsh("write event failed, %s\n", strerror(errno));
	}
}

void TrackingBegin(struct input_event ev )
{
     WriteToTouchfd(ev,EV_ABS,0x39,0x2c);
}

void TrackingEnd(struct input_event ev )
{
     WriteToTouchfd(ev,EV_ABS,0x39,0xffffffff);
}

void StartDown(struct input_event ev )
{
     WriteToTouchfd(ev,EV_ABS,0x30,0x1c);
}

void StartUp(struct input_event ev )
{
     WriteToTouchfd(ev,EV_ABS,0x30,0x0);
}

void SpecialDown(struct input_event ev )
{
     WriteToTouchfd(ev,EV_ABS,0x3a,0x1c);
}

void SpecialUp(struct input_event ev )
{
     WriteToTouchfd(ev,EV_ABS,0x3a,0x0);
}

void KeyDown(struct input_event ev )
{
     WriteToTouchfd(ev,EV_KEY,BTN_TOUCH,0x1);
}

void KeyUp(struct input_event ev )
{
     WriteToTouchfd(ev,EV_KEY,BTN_TOUCH,0x0);
}

void NormalEnd(struct input_event ev )
{
     WriteToTouchfd(ev,EV_SYN,0x2,0x0);
}

void Sync(struct input_event ev )
{
     WriteToTouchfd(ev,EV_SYN,0x0,0x0);
}

void PositionX(struct input_event ev,int x )
{
     WriteToTouchfd(ev,EV_ABS,0x35,x);
}

void PositionY(struct input_event ev ,int y)
{
     WriteToTouchfd(ev,EV_ABS,0x36,y);
}






void DownEvent(int x, int y,BOOLEAN hasTracking,BOOLEAN has3A,BOOLEAN hasKeydownEvent,BOOLEAN isSynaptics)
{
	struct input_event ev;
	memset(&ev, 0, sizeof(ev));
	if(hasTracking){
	   TrackingBegin(ev);
	   if(has3A)
	   {
	      SpecialDown(ev);
	   }else
	   {
	      StartDown(ev);
	   }
	   PositionX(ev,x);
	   PositionY(ev,y);

	   if(isSynaptics)
	   {
	       NormalEnd(ev);
	   }
	   
	}else{
	   if(has3A)
	   {
	      SpecialDown(ev);
	   }else
	   {
	      StartDown(ev);
	   }
	   PositionX(ev,x);
	   PositionY(ev,y);
	   NormalEnd(ev);
	}
	if(hasKeydownEvent)
    {
	    KeyDown(ev);
	}
    Sync(ev);
}

void UpEvent(int x, int y,BOOLEAN hasTracking,BOOLEAN has3A,BOOLEAN hasKeydownEvent,BOOLEAN isSynaptics)
{
	struct input_event ev;
	memset(&ev, 0, sizeof(ev));
	
	
	
	if(hasTracking&&!isSynaptics){
	   if(has3A)
	   {
	      SpecialUp(ev);
	   }else
	   {
	      StartUp(ev);
	   }
	   PositionX(ev,x);
	   PositionY(ev,y);
       TrackingEnd(ev);

	   
	}else{
	   if(isSynaptics)
	   {
	       TrackingBegin(ev);
	   }
	   if(has3A)
	   {
	      SpecialUp(ev);
	   }else
	   {
	      StartUp(ev);
	   }
	   PositionX(ev,x);
	   PositionY(ev,y);
	   NormalEnd(ev);
	}
	if(hasKeydownEvent)
    {
	    KeyUp(ev);
	}
    Sync(ev);
}



void LongPressEvent(int x, int y,BOOLEAN hasTracking,BOOLEAN has3A,BOOLEAN hasKeydownEvent,BOOLEAN isSynaptics)
{
    if(hasTracking)
	{
	    if(isSynaptics)
		{
		    DownEvent( x,  y, hasTracking, has3A, hasKeydownEvent, isSynaptics);
		}else
		{
		    struct input_event ev;
	        memset(&ev, 0, sizeof(ev));
			TrackingBegin(ev);
			if(has3A)
	        {
	            SpecialDown(ev);
	        }else
	        {
	            StartDown(ev);
	        }	
	        if(hasKeydownEvent)
            {
	            KeyDown(ev);
	        }	
			int i=0;
            for(;i<7;i++)
            {
			     PositionX(ev,x);
	             PositionY(ev,y);
				 Sync(ev);
            }			
		}
	    
	}else
	{
	    DownEvent( x,  y, hasTracking, has3A, hasKeydownEvent, isSynaptics);
	}
	
     
}


void Keyboard(int key)
{
	struct input_event ev;
	memset(&ev, 0, sizeof(ev));
    WriteToTouchfd(ev,EV_SYN,0x0,0x0);
}

struct nd_data Sendevent(struct nd_data* data_receive)
{
	DebugNdsh("Sendevent()\n");
	struct nd_data data_result = {0, 0, NULL};
	char *device=ReadStringFromBuffer(data_receive);
	BOOLEAN hasTraccking= ReadBooleanFromBuffer(data_receive);
	BOOLEAN isSynaptics= ReadBooleanFromBuffer(data_receive);
	BOOLEAN has3A= ReadBooleanFromBuffer(data_receive);
	BOOLEAN hasKeydownEvent= ReadBooleanFromBuffer(data_receive);
	int action=ReadIntFromBuffer(data_receive);
	init_touch(device);
	DebugNdsh("init_touch() %d \n",touchfd);
	if (touchfd==-1)
	{
		WriteIntToBuffer(&data_result, UNKNOW_ERROR);
		return data_result;
	}
	DebugNdsh("action : %d \n",action);
	if(action==0){
	    int key=ReadIntFromBuffer(data_receive);
	    Keyboard(key);
	}else{
	    int x=ReadIntFromBuffer(data_receive);
	    DebugNdsh("x : %d \n",x);
	    int y=ReadIntFromBuffer(data_receive);
	    DebugNdsh("y : %d \n",y);
	    if(action==1){
	        DownEvent( x,  y, hasTraccking, has3A, hasKeydownEvent, isSynaptics);
	    }else if(action==2){
	        UpEvent( x,  y, hasTraccking, has3A, hasKeydownEvent, isSynaptics);
	    }
	}
	cleanup_touch();
	WriteIntToBuffer(&data_result, UNKNOW_ERROR);
	WriteIntToBufferInPos(&data_result, 0, SUCCESS);
	return data_result;
}


