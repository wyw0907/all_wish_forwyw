TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    cJSON.c

DISTFILES += \
    makefile \
    libcurl.a \
    libcurl.so \
    jSon.txt

HEADERS += \
    include/curl/curl.h \
    include/curl/curlbuild.h \
    include/curl/curlrules.h \
    include/curl/curlver.h \
    include/curl/easy.h \
    include/curl/mprintf.h \
    include/curl/multi.h \
    include/curl/stdcheaders.h \
    include/curl/typecheck-gcc.h \
    curl/curl.h \
    curl/curlbuild.h \
    curl/curlrules.h \
    curl/curlver.h \
    curl/easy.h \
    curl/mprintf.h \
    curl/multi.h \
    curl/stdcheaders.h \
    curl/typecheck-gcc.h \
    cJSON.h

