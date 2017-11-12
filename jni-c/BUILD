java_library(
    name = "OpenOCF",
    srcs = glob(["src/main/**/*.java"]),
    # runtime_deps = [#"libopenocf_jni.dylib",
    deps = [#"libopenocf_jni.dylib",
                    "libopenocf_jni.so"
    ],
    visibility = ["//visibility:public"]
)

android_library(
    name = "OpenOCFAndroid",
    srcs = glob(["src/main/**/*.java"]),
    deps = ["libopenocf_jni.so",
        #"libopenocf_jni"
    ],
    visibility = ["//visibility:public"]
)

cc_binary(
    # java on mac:
    # name = "libopenocf_jni.dylib",
    # java on linux/android:
    # name = "libopenocf_jni.so",
    name = "libopenocf_jni",
    # linkstatic = 1,
    # alwayslink = 1,
    linkshared = 1,
    copts = ["-std=c11"],
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
            "@openocf//src/ocf:hdrs"],
    # FIXME: put all public headers in openocf/include
    includes = ["external/openocf/src", # for wrapped lib
                "external/openocf/src/comm/api",
                "external/openocf/src/logger",
                "external/openocf/src/ocf",
                "external/openocf/src/portability",
                "external/openocf/src/util",
                "external/openocf/third_party/coap/include",
                "external/local_jdk/include",
                "external/local_jdk/include/darwin"],
    visibility = ["//visibility:public"]
)

cc_library(
    # mac:
    # name = "libopenocf_jni.dylib",
    # linux/android:
    name = "libopenocf_jni.so",
    # name = "libopenocf_jni",
    # linkstatic = 1,
    # alwayslink = 1,
    # linkshared = 1,
    copts = ["-std=c11"],
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
            "@openocf//src/ocf:hdrs"],
    # FIXME: put all public headers in openocf/include
    includes = ["external/openocf/src", # for wrapped lib
                "external/openocf/src/comm/api",
                "external/openocf/src/logger",
                "external/openocf/src/ocf",
                "external/openocf/src/portability",
                "external/openocf/src/util",
                "external/openocf/third_party/coap/include",
                "external/local_jdk/include",
                "external/local_jdk/include/darwin"],
    visibility = ["//visibility:public"]
)
