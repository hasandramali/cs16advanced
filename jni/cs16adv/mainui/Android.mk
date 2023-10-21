
#Xash3d mainui port for android
#Copyright (c) nicknekit

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

include $(XASH3D_CONFIG)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a-hard)
LOCAL_MODULE_FILENAME = libmenu_hardfp
endif

LOCAL_CPPFLAGS += -std=c++11

LOCAL_MODULE := menu

APP_PLATFORM := android-12

LOCAL_C_INCLUDES := $(SDL_PATH)/include \
		    $(LOCAL_PATH)/.			    \
		    $(LOCAL_PATH)/../common \
	            $(LOCAL_PATH)/../pm_shared \
	            $(LOCAL_PATH)/../engine \
		    $(LOCAL_PATH)/../engine/common \
		    $(LOCAL_PATH)/../utils/vgui/include \
		    $(HLSDK_PATH)/cl_dll/

LOCAL_SRC_FILES := basemenu.cpp \
           menu_advcontrols.cpp \
           menu_audio.cpp \
           menu_btns.cpp \
           menu_configuration.cpp \
           menu_controls.cpp \
           menu_creategame.cpp \
           menu_credits.cpp \
	   menu_filedialog.cpp \
           menu_gameoptions.cpp \
           menu_gamepad.cpp \
           menu_internetgames.cpp \
           menu_langame.cpp \
           menu_main.cpp \
           menu_multiplayer.cpp \
           menu_playersetup.cpp \
           menu_strings.cpp \
           menu_vidoptions.cpp \
	   menu_touchoptions.cpp \
	   menu_touch.cpp \
	   menu_touchedit.cpp \
	   menu_touchbuttons.cpp \
           udll_int.cpp \
           ui_title_anim.cpp \
	   utils.cpp \
	   legacy/menu_playrec.cpp \
	   legacy/menu_playdemo.cpp \
	   legacy/menu_recdemo.cpp 

#ifeq ($(XASH_SDL),1)
#LOCAL_SHARED_LIBRARIES += SDL2
#endif
include $(BUILD_SHARED_LIBRARY)
