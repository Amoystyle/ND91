LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)   
LOCAL_MODULE    := nd  
LOCAL_SRC_FILES := ../../libnd/obj/local/armeabi/libnd.a
include $(PREBUILT_STATIC_LIBRARY)  

include $(CLEAR_VARS)
LOCAL_MODULE	:= file_jni
LOCAL_SRC_FILES	:= \
	jni_convert.cpp \
	daemon_provider_file_FileJni.cpp
	

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../libnd/jni
LOCAL_STATIC_LIBRARIES := nd	
#LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
include $(BUILD_SHARED_LIBRARY)