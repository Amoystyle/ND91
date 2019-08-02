LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE	:= nd
LOCAL_SRC_FILES	:= \
	libjpeg/jcapimin.cpp \
	libjpeg/jcapistd.cpp \
	libjpeg/jccoefct.cpp \
	libjpeg/jccolor.cpp \
	libjpeg/jcdctmgr.cpp \
	libjpeg/jchuff.cpp \
	libjpeg/jcinit.cpp \
	libjpeg/jcmainct.cpp \
	libjpeg/jcmarker.cpp \
	libjpeg/jcmaster.cpp \
	libjpeg/jcomapi.cpp \
	libjpeg/jcparam.cpp \
	libjpeg/jcphuff.cpp \
	libjpeg/jcprepct.cpp \
	libjpeg/jcsample.cpp \
	libjpeg/jctrans.cpp \
	libjpeg/jdapimin.cpp \
	libjpeg/jdapistd.cpp \
	libjpeg/jdatadst.cpp \
	libjpeg/jdatasrc.cpp \
	libjpeg/jdcoefct.cpp \
	libjpeg/jdcolor.cpp \
	libjpeg/jddctmgr.cpp \
	libjpeg/jdhuff.cpp \
	libjpeg/jdinput.cpp \
	libjpeg/jdmainct.cpp \
	libjpeg/jdmarker.cpp \
	libjpeg/jdmaster.cpp \
	libjpeg/jdmerge.cpp \
	libjpeg/jdphuff.cpp \
	libjpeg/jdpostct.cpp \
	libjpeg/jdsample.cpp \
	libjpeg/jdtrans.cpp \
	libjpeg/jerror.cpp \
	libjpeg/jfdctflt.cpp \
	libjpeg/jfdctfst.cpp \
	libjpeg/jfdctint.cpp \
	libjpeg/jidctflt.cpp \
	libjpeg/jidctfst.cpp \
	libjpeg/jidctint.cpp \
	libjpeg/jidctred.cpp \
	libjpeg/jmemmgr.cpp \
	libjpeg/jmemnobs.cpp \
	libjpeg/jquant1.cpp \
	libjpeg/jquant2.cpp \
	libjpeg/jutils.cpp \
	common.cpp \
	log.cpp \
	buffer.cpp \
	ndstream.cpp \
	shell.cpp \
	process.cpp \
	file.cpp \
	app_manager.cpp \
	stdafx.cpp \
	sendevent.cpp \
	screenshot.cpp \

LOCAL_CFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE
#LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
include $(BUILD_STATIC_LIBRARY)