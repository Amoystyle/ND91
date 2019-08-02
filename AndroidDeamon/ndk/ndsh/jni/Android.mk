LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)   
LOCAL_MODULE    := nd  
LOCAL_SRC_FILES := ../../libnd/obj/local/armeabi/libnd.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)
LOCAL_MODULE	:= ndsh
LOCAL_SRC_FILES	:= \
	ndsh.cpp \
	threadpool.cpp \
	
LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../libnd/jni
LOCAL_STATIC_LIBRARIES := nd	
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
include $(BUILD_EXECUTABLE)