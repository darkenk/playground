
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_CLANG=true
LOCAL_MODULE := dk_test
LOCAL_MODULE_TAGS := optional
LOCAL_SRC_FILES := main.cpp
LOCAL_SHARED_LIBRARIES := libutils libcutils
LOCAL_C_INCLUDES += frameworks/base/include system/core/include
include $(BUILD_EXECUTABLE)
