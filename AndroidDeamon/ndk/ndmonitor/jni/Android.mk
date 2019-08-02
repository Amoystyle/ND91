LOCAL_PATH := $(call my-dir)
 
include $(CLEAR_VARS) 
LOCAL_MODULE	:= ndmonitor
LOCAL_SRC_FILES	:= \
	ndmonitor.cpp \  

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
include $(BUILD_EXECUTABLE)