LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := kanjigles2
LOCAL_SRC_FILES := ../../lib/libkanjigles2-$(TARGET_ARCH_ABI).a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE := kanjigame
LOCAL_SRC_FILES := ../../lib/libkanjigame-$(TARGET_ARCH_ABI).a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := kanjigamelib
LOCAL_CFLAGS    := -Werror -D__KANJI_ANDROID__ -I../../headers
LOCAL_STATIC_LIBRARIES := kanjigles2 kanjigame libatomic

LOCAL_CFLAGS    += -O2 -fno-stack-protector -fgcse-after-reload -ftree-vectorize -I../src_game
LOCAL_ARM_MODE  := arm
LOCAL_SRC_FILES += LibWrapper.cpp ../src_game/main.cpp ../src_game/MyGame.cpp

LOCAL_LDLIBS    := -llog -landroid -lGLESv2

include $(BUILD_SHARED_LIBRARY)
