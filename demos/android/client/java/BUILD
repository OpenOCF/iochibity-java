android_binary(
    name = "client",
    srcs = glob(["org/openocf/test/client/*.java"])
    + glob(["org/openocf/test/*.java"]),
    manifest = "org/openocf/test/client/ClientManifest.xml",
    deps = ["//:OpenOCFAndroid",
            "//:AndroidConfig",
            "//demos/android/common/java:common"],
    resource_files = glob(["org/openocf/test/client/res/**"]),
    assets_dir = "config",
    assets = [":client_config"]
)

genrule(
    name = "client_json",
    srcs = ["resources/client_owner_config.edn"],
    outs = ["client_config.json"],
    cmd = "edn-to-json $(location resources/client_owner_config.edn)  > \"$@\"",
)

genrule(
    name = "client_config",
    srcs = [":client_json"],
    outs = ["config/svrs.cbor"],
    cmd = "json2cbor $(location :client_json) > \"$@\"",
)
