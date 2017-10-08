# cc_library(
#     name = "libopenocf",
#     # srcs = ["libopenocf-jni.dyld"],
#     deps = ["//:libopenocf-jni.dylib"],
# )

java_binary(
    name = "OCFTestServer",
    srcs = glob(["src/main/java/**/*.java"]),
    deps = ["//:OpenOCF-JNI"],
            # "//:libopenocf.jnilib"],
    # runtime_deps = ["//:libopenocf.jnilib"],
    # jvm_flags = ["-Djava.library.path=."]
    # data = ["//:openocf-jni.dylib"]
)

java_binary(
    name = "OCFTestClient",
    srcs = glob(["src/main/java/**/*.java"]),
    deps = ["//:OpenOCF-JNI"],
)
