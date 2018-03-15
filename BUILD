java_library(
    name = "OpenOCFJ",
    srcs = glob(["src/common/java/**/*.java"])
    + glob(["src/std/**/*.java"]),
    resources = ["libopenocf_jni.so"],
    # resources = ["libopenocf_jni.dylib"],
    visibility = ["//visibility:public"]
)

android_library(
    name = "OpenOCFAndroid",
    # android: default --fat_apk_cpu = armeabi-v7a
    srcs = glob(["src/common/**/*.java"])
    + glob(["src/android/**/*.java"]),
    deps = [#"libopenocf_jni.so",
        ":openocf_jni"
    ],
    visibility = ["//visibility:public"]
)

# Common/Android/Std Java APIs only, no native lib:
# use this to develop the Android API, e.g.

# just one file:
# $ javah -jni -cp $ANDROID_HOME/platforms/android-24/android.jar:bazel-bin/_javac/openocf_japi/libopenocf_japi_classes -d src/common/jni openocf.android.OCFService

# all files e.g. in openocf/app:
# for filename in bazel-bin/_javac/openocf_japi/libopenocf_japi_classes/openocf/app/*.class; do javah -jni -cp $ANDROID_HOME/platforms/android-24/android.jar:bazel-bin/_javac/openocf_japi/libopenocf_japi_classes -d src/common/jni "openocf.app.`basename ${filename%.*}`"; done

# NB: java_library would not pick up Android jars needed by src/android, so use android_library
android_library(
    name = "openocf_japi",
    srcs = glob(["src/common/**/*.java"])
    + glob(["src/std/**/*.java"])
    + glob(["src/android/**/*.java"]),
    visibility = ["//visibility:public"]
)

# javah: list of Java API files with native deps:
# filegroup(
#     name = "javah_files",
#     srcs = glob([""])
#     ]
# )

# genrule(
#     name = "javah",
#     srcs = [":openocf_japi"],
#     outs = [# "src/common/jni/openocf_android_OCFService.h",
#             "src/common/jni/openocf_CoResourceSP.h",
#             # etc
#     ],
#     cmd = """
#     mkdir classes_tmp
#     unzip -d classes_tmp $(location :openocf_japi) > /dev/null
#     javah -classpath classes_tmp -o src/common/jni/openocf_CoResourceSP.h openocf.CoResourceSP
#     rm -rf classes_tmp
# """)
    # cmd = "java -jni -cp /home/gar/Android/Sdk/platforms/android-24/android.jar:bazel-bin/_javac/openocf_japi/libopenocf_japi_classes -d src/common/jni $(locations :javah_files)"
#)

# build the jni share lib
cc_binary(
    # name = "libopenocf_jni.dylib",
    name = "libopenocf_jni.so",
    linkshared = 1,
    copts = ["-std=c11",
             "-Iexternal/openocf/include", # for wrapped lib
             "-Iexternal/openocf/src/ocf",
             "-Iexternal/openocf/src/portability",
             "-Iexternal/openocf/third_party/coap",
             "-Iexternal/openocf/third_party/coap/include",
             "-Iexternal/openocf/third_party/tinycbor/src",
             "-Iexternal/local_jdk/include",
             "-Iexternal/local_jdk/include/linux"
             # "-Iexternal/local_jdk/include/darwin"
    ],
    srcs = glob(["src/common/jni/*.c"]) # jni layer
    + glob(["src/common/jni/*.h"])
    + ["@local_jdk//:jni_header",
    # FIXME: use select on platform
       # "@local_jdk//:jni_md_header-darwin",
       "@local_jdk//:jni_md_header-linux",
       # openocf lib and jni layer as separate dylibs
       # "@openocf//src/ocf:libopenocf.dylib"
       # android shared lib:
       # "@openocf//src/ocf:libopenocf.so"
    ],
    deps = ["@openocf//src/ocf", # static - everything stuffed into the jni shared lib
            "@openocf//src/portability",
            "@openocf//include",
    ],
    # FIXME: put all public headers in openocf/include
    # includes = ["external/openocf/include", # for wrapped lib
    #             "external/openocf/src", # for wrapped lib
    #             "external/openocf/src/comm/api",
    #             "external/openocf/src/logger",
    #             "external/openocf/src/ocf",
    #             "external/openocf/src/portability",
    #             "external/openocf/src/util",
    #             "external/local_jdk/include",
    #             "external/local_jdk/include/darwin"],
    visibility = ["//visibility:public"]
)

# build the static jni layer
cc_library(
    name = "openocf_jni",    # produces libopenocf_jni.a, so
    # name = "libopenocf_jni",
    linkstatic = 1,
    alwayslink = 1,
    # linkshared = 1,
    copts = ["-std=c11",
             "-Iexternal/openocf/include", # for wrapped lib
             "-Iexternal/openocf/src/ocf",
             "-Iexternal/openocf/src/portability",
             "-Iexternal/openocf/third_party/coap",
             "-Iexternal/openocf/third_party/coap/include",
             "-Iexternal/openocf/third_party/tinycbor/src",
             "-Iexternal/local_jdk/include",
             "-Iexternal/local_jdk/include/linux",
             # "-Iexternal/local_jdk/include/darwin",
             "-isysroot=/Users/gar/android/sdk/ndk-bundle/sysroot"
    ],
    srcs = glob(["src/common/jni/*.c"]) # jni layer
    + glob(["src/common/jni/*.h"])
    + ["@local_jdk//:jni_header",
       # "@local_jdk//:jni_md_header-darwin",
       "@local_jdk//:jni_md_header-linux",
       # openocf lib and jni layer as separate dylibs
       # "@openocf//src/ocf:libopenocf.dylib"
       # android shared lib:
       # "@openocf//src/ocf:libopenocf.so"
    ],
    deps = ["@openocf//src/ocf", # static - everything stuffed into the jni shared lib
            # "@openocf//src/portability",
            "@openocf//include"],
    visibility = ["//visibility:public"]
)

## WIP:
# filegroup(
#     name = "java_srcs",
#     srcs = glob(["src/main/java/*.java"]),
# )

# filegroup(
#     name = "jni_hdrs",
#     srcs = glob(["src/main/jni/*.h"]),
# )

# genrule(
#     name = "jniheaders",
#     srcs = ["bazel-bin/_javac/OpenOCF/libOpenOCF_classes/openocf/Service.class"],
#     outs = ["src/main/jni/openocf_Service.h"],
#     cmd = "javah -jni -cp bazel-bin/_javac/OpenOCF/libOpenOCF_classes -d src/main/jni openocf.Service"
#     # cmd = "javah -jni -cp $(locations :java_srcs) -d src/main/jni openocf.Service"
#     )
