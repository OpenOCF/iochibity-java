# NOTE that *.so works on OS X.
cc_binary(
    name = "libopenocf_jni.so",
    #alwayslink = 1,
    linkshared=1,
    copts = ["-std=c11",
             "-Iexternal/openocf/include", # for wrapped lib
             "-Iexternal/openocf/src/portability",
             #"-Iexternal/openocf/src/util",
             "-Iexternal/openocf/third_party/cjson",
             # "-Iconfig/darwin",
             # "-Iconfig/darwin/coap",
             "-Iexternal/libcoap/include",
             "-Iexternal/libcoap/include/coap",
             "-Iexternal/tinycbor/src",
             "-Iexternal/local_jdk/include",
    ] + select({"@openocf//config:darwin_with_jni": ["-Iexternal/local_jdk/include/darwin"],
                "@openocf//config:linux_with_jni": ["-Iexternal/local_jdk/include/linux"],
                "//conditions:default": ["BROKEN"]}),

    srcs = glob(["*.c"]) # jni layer
    + glob(["*.h"])
    + ["//:javah",
       "@local_jdk//:jni_header",
       #"@openocf//src/ocf",
    ] + select({"@openocf//config:darwin_with_jni": ["@local_jdk//:jni_md_header-darwin"],
                "@openocf//config:linux_with_jni": ["@local_jdk//:jni_md_header-linux"],
                "//conditions:default": ["BROKEN"]}),
    deps = [#"@openocf//src/ocf", # static - everything stuffed into the jni shared lib
            #"@openocf//src/portability",
            "@openocf//:openocf",
            # "@openocf//third_party/cjson",
            # "@openocf//third_party/coap",
            # "@tin//third_party/tinycbor",
        #"@openocf//include"
    ],
    visibility = ["//visibility:public"]
)

