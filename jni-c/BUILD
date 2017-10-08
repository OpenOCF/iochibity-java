# this builds one lib containing both the wrapped lib (ocf) and the jni layer (?)
cc_binary(
    name = "libopenocf.jnilib",
    # copts = ["-dynamiclib"],    # for Darwin (MacOS)
    srcs = glob(["src/c/*.c"]) + glob(["src/c/*.h"]) # jni layer
    + ["@local_jdk//:jni_header",
       "@local_jdk//:jni_md_header-darwin"],
    deps = ["@openocf//src/ocf", # wrapped lib
            "@openocf//src/ocf:hdrs"],
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

# cc_binary(
#     name = "libopenocf.jnilibX",
#     copts = ["-dynamiclib"],
#     deps = [":openocf-jni.so",
#             "@openocf//src/ocf",
#             "@openocf//src/ocf:hdrs"],
#     # linkshared = 1,
#     visibility = ["//visibility:public"]
# )

java_library(
    name = "OpenOCF-JNI",
    srcs = glob(["src/main/**/*.java"]),
    runtime_deps = ["libopenocf.jnilib"],
    visibility = ["//visibility:public"]
)

