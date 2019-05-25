LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include $(CLEAR_VARS)
LOCAL_MODULE := aubio
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libaubio.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include/src
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := pitch
LOCAL_SRC_FILES := pitch.c
LOCAL_SHARED_LIBRARIES := aubio
LOCAL_CFLAGS += -fopenmp
LOCAL_LDFLAGS += -fopenmp
include $(BUILD_SHARED_LIBRARY)

