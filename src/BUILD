java_library(
    name = "OpenOCF",
    srcs = glob(["src/main/**/*.java"]),
    resources = ["libopenocf_jni.dylib"],
    visibility = ["//visibility:public"]
)

android_library(
    name = "OpenOCFAndroid",
    srcs = glob(["src/main/**/*.java"]),
    deps = [#"libopenocf_jni.so",
        ":openocf_jni"
    ],
    visibility = ["//visibility:public"]
)

# build the jni share lib
cc_binary(
    name = "libopenocf_jni.dylib",
    linkshared = 1,
    copts = ["-std=c11",
             "-Iexternal/openocf/include", # for wrapped lib
             "-Iexternal/openocf/src/ocf",
             "-Iexternal/openocf/src/portability",
             "-Iexternal/openocf/third_party/coap",
             "-Iexternal/openocf/third_party/coap/include",
             "-Iexternal/openocf/third_party/tinycbor/src",
             "-Iexternal/local_jdk/include",
             "-Iexternal/local_jdk/include/darwin"
    ],
    srcs = glob(["src/main/jni/*.c"]) # jni layer
    + glob(["src/main/jni/*.h"])
    + ["@local_jdk//:jni_header",
       "@local_jdk//:jni_md_header-darwin",
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

# build the jni layer
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
             "-Iexternal/local_jdk/include/darwin",
             "-isysroot=/Users/gar/android/sdk/ndk-bundle/sysroot"
    ],
    srcs = glob(["src/main/jni/*.c"]) # jni layer
    + glob(["src/main/jni/*.h"])
    + ["@local_jdk//:jni_header",
       "@local_jdk//:jni_md_header-darwin",
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

