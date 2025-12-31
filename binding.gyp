{
  "targets": [
    {
      "target_name": "PseudoPTY",
      "sources": ["src/addon.cc"],
      "include_dirs": [
        "<(module_root_dir)/node_modules/node-addon-api"
      ],
      "libraries": [
        "<(module_root_dir)/bin/x64/Release/Engine.lib",
        "<(module_root_dir)/bin/x64/Release/WindowsPty.lib"
      ],
      "defines": ["NAPI_CPP_EXCEPTIONS"],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
        ["OS=='win'", { "msvs_settings": { "VCCLCompilerTool": { "ExceptionHandling": 1 } } }]
      ]
    }
  ]
}
