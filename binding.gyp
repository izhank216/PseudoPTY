{
  "targets": [
    {
      "target_name": "PseudoPTY",
      "sources": [ "src/addon.cc" ],
      "libraries": [
        "<(module_root_dir)/bin/x64/Release/Engine.lib",
        "<(module_root_dir)/bin/x64/Release/WindowsPty.lib"
      ]
    }
  ]
}
