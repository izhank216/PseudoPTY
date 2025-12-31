{
  "targets": [
    {
      "target_name": "PseudoPTY",
      "sources": [ "src/addon.cc" ],
      "include_dirs": [
        "<!(node -p \"require('node-addon-api').include\")"
      ],
      "libraries": [
        "<(module_root_dir)/bin/x64/Release/Engine.lib",
        "<(module_root_dir)/bin/x64/Release/WindowsPty.lib"
      ],
      "defines": [ "NAPI_CPP_EXCEPTIONS" ],
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "conditions": [
        ["OS=='win'", { "msvs_settings": { "VCCLCompilerTool": { "ExceptionHandling": 1 } } }]
      ]
    }
  ]
}
