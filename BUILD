# this is just the Java part of the JNI wrapper.  PoJo and Android
# libraries require different native builds, cc_binary for PoJo and
# cc_library for Android, so we do not specify them here.
java_library(
    name = "OpenOCFJ",
    srcs = glob(["src/main/java/**/*.java"],
                exclude = ["src/main/java/openocf/ConfigAndroid.java",
                           "src/main/java/openocf/ConfigJava.java"]),
    visibility = ["//visibility:public"]
)

java_library(
    name = "OpenOCFJava",
    srcs = glob(["src/main/java/**/*.java"],
                exclude = ["src/main/java/openocf/ConfigAndroid.java"]),
    # + ["src/main/java/openocf/ConfigJava.java",
    #    "src/main/java/openocf/OpenOCF.java"],
    # deps = [":OpenOCFJ"],
    # exports = ["openocfjni"],
    resources = ["//src/main/jni:libopenocf_jni.so"],
    # runtime_deps = ["//src/main/jni:libopenocf_jni.so"],
    visibility = ["//visibility:public"]
)

android_library(
    name = "AndroidConfig",
    # srcs = glob(["src/main/java/**/*.java"],
    #             exclude = ["src/main/java/openocf/ConfigJava.java"]),
    srcs = ["src/main/java/openocf/OpenOCF.java",
            "src/main/java/openocf/ConfigAndroid.java"],
    visibility = ["//visibility:public"]
)

android_library(
    name = "OpenOCFAndroid",
    srcs = glob(["src/main/java/**/*.java"],
                exclude = ["src/main/java/openocf/ConfigJava.java"]),
    #+ [#"src/main/java/openocf/OpenOCF.java",
    #srcs = [":OpenOCFJ"],
    deps = [":openocfjni"],
    exports = [":openocfjni"],
    visibility = ["//visibility:public"]
)

# Main/Android/Std Java APIs only, no native lib:
# use this to develop the Android API, e.g.

# just one file:
# $ javah -jni -cp $ANDROID_HOME/platforms/android-24/android.jar:bazel-bin/_javac/openocf_japi/libopenocf_japi_classes -d src/main/jni openocf.android.OCFService

# all files e.g. in openocf/app:
# for filename in bazel-bin/_javac/openocf_japi/libopenocf_japi_classes/openocf/app/*.class; do javah -jni -cp $ANDROID_HOME/platforms/android-24/android.jar:bazel-bin/_javac/openocf_japi/libopenocf_japi_classes -d src/main/jni "openocf.app.`basename ${filename%.*}`"; done

# NB: java_library would not pick up Android jars needed by src/android, so use android_library
android_library(
    name = "openocf_android",
    srcs = glob(["src/main/**/*.java"],
                exclude = ["src/main/java/openocf/ConfigJava.java"]),
    # + glob(["src/std/**/*.java"])
    # + glob(["src/android/**/*.java"]),
    visibility = ["//visibility:public"]
)

java_library(
    name = "openocf_japi",
    srcs = glob(["src/main/java/**/*.java"],
                # exclude anything depending on Android
                exclude = ["src/main/java/openocf/ConfigAndroid.java"]),
    #+ glob(["src/std/**/*.java"]),
    # resources = ["libopenocf_jni.so"],
    # resources = ["libopenocf_jni.dylib"],
    visibility = ["//visibility:public"]
)

genrule(
    name = "javah",
    visibility = ["//visibility:public"],
    # srcs = [":openocf_japi"],
    srcs = ["OpenOCFJ"],
    outs = [# "src/main/jni/openocf_android_OCFService.h",
        "openocf_app_CoResourceSP.h",
        # "openocf_app_DefaultCoRSP.h",
        # "openocf_app_ICoResourceSP.h",
        "openocf_app_ResourceSP.h",
        # "openocf_app_IResourceSP.h",
        "openocf_behavior_BehaviorRecord.h",
        "openocf_behavior_InboundStimulus.h",
        "openocf_behavior_InboundResponse.h",
        "openocf_behavior_OutboundStimulus.h",
        "openocf_utils_Endpoint.h",
        "openocf_OpenOCF.h",
        "openocf_OpenOCFClient.h",
        "openocf_OpenOCFServer.h"
    ],
    cmd = """
    mkdir classes_tmp
    unzip -d classes_tmp $(location :OpenOCFJ) > /dev/null
    for f in $(OUTS)
    do
        filename=$$(basename -- "$$f")
        extension="$${filename##*.}"
        filename="$${filename%.*}"
        filename=$${filename//_/.}
        echo $$filename
        javah -classpath classes_tmp -o $$f $$filename
    done
   rm -rf classes_tmp
""")

#    javah -classpath classes_tmp -o $@ openocf.utils.Endpoint openocf.behavior.BehaviorRecord


    # cmd = "java -jni -cp /home/gar/Android/Sdk/platforms/android-24/android.jar:bazel-bin/_javac/openocf_japi/libopenocf_japi_classes -d src/main/jni $(locations :javah_files)"
#)

# cc_binary(
#     #name = "libopenocf_jni.dylib",
#     name = "libopenocf_jni.soX",
#     linkshared = 1,
#     copts = ["-std=c11",
#              # "-Igenfiles",
#              "-Iexternal/openocf/include", # for wrapped lib
#              "-Iexternal/openocf/src/ocf",
#              "-Iexternal/openocf/src/portability",
#              "-Iexternal/openocf/third_party/coap",
#              "-Iexternal/openocf/third_party/coap/include",
#              "-Iexternal/openocf/third_party/tinycbor/src",
#              "-Iexternal/local_jdk/include",
#     ] + select({"@openocf//config:darwin_with_jni": ["-Iexternal/local_jdk/include/darwin"],
#                 "@openocf//config:linux_with_jni": ["-Iexternal/local_jdk/include/linux"],
#                 "//conditions:default": ["BROKEN"]}),
#     srcs = glob(["src/main/jni/*.c"]) # jni layer
#     + glob(["src/main/jni/*.h"])
#     + [":javah",
#         "@local_jdk//:jni_header"
#     ] + select({"@openocf//config:darwin_with_jni": ["@local_jdk//:jni_md_header-darwin"],
#                 "@openocf//config:linux_with_jni": ["@local_jdk//:jni_md_header-linux"],
#                 "//conditions:default": ["BROKEN2"]}),

#     deps = ["@openocf//src/ocf", # static - everything stuffed into the jni shared lib
#             "@openocf//src/portability",
#             "@openocf//include",
#     ],
#     visibility = ["//visibility:public"]
# )

cc_import(
    name="openocflib",
    hdrs = [],
    static_library = "libopenocf.a",
    shared_library = "libopenocf.so",
)

cc_library(
    name = "openocfjni",
    alwayslink = 1,
    srcs = ["//src/main/jni:libopenocf_jni.so"]
)

# cc_binary(
#     name = "libopenocf_jni.soX",
#     linkshared = 1,
#     #alwayslink = 1,
#     copts = ["-std=c11",
#              "-Iexternal/openocf/include", # for wrapped lib
#              "-Iexternal/openocf/src/portability",
#              "-Iexternal/cjson",
#              # "-Iconfig/darwin",
#              # "-Iconfig/darwin/coap",
#              "-Iexternal/libcoap/include",
#              "-Iexternal/libcoap/include/coap",
#              # "-Iexternal/openocf/third_party/coap",
#              # "-Iexternal/openocf/third_party/coap/include",
#              "-Iexternal/tinycbor/src",
#              # "-Iexternal/openocf/third_party/tinycbor/src",
#              "-Iexternal/local_jdk/include",
#     ] + select({"@openocf//config:darwin_with_jni": ["-Iexternal/local_jdk/include/darwin"],
#                 "@openocf//config:linux_with_jni": ["-Iexternal/local_jdk/include/linux"],
#                 "//conditions:default": ["BROKEN"]}),

#     srcs = glob(["src/main/jni/*.c"]) # jni layer
#     + glob(["src/main/jni/*.h"])
#     + [":javah",
#        "@local_jdk//:jni_header",
#        #"@openocf//src/ocf",
#     ] + select({"@openocf//config:darwin_with_jni": ["@local_jdk//:jni_md_header-darwin"],
#                 "@openocf//config:linux_with_jni": ["@local_jdk//:jni_md_header-linux"],
#                 "//conditions:default": ["BROKEN"]}),
#     deps = ["@openocf//:openocf", # static - everything stuffed into the jni shared lib
#             #"@openocf//src/portability",
#             # FIXME: remove cjson dep, it should go in an extension lib, e.g. a logger
#             #"@cjson//:cjson",
#             # "@openocf//third_party/coap",
#             # "@openocf//third_party/tinycbor",
#             #"@openocf//include"
#     ],
#     linkopts = ["-llog"],
#     visibility = ["//visibility:public"]
# )
