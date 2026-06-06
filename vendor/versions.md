# Vendored Sources

Third-party code is committed directly under 'vendor/' so 'dans-platform' can
be built from a fixed repository commit without submodules or configure-time
downloads.

## Dependencies

- GLFW: 'https://github.com/glfw/glfw'
  - Tag: '3.4'
  - Commit: '7b6aead9fb88b3623e3b3725ebb42670cbe4c579'
  - Version macro: '3.4.0'
  - Vendored form: shortened to the buildable library, kept functional for
    every GLFW target. Kept: 'src/' (all platform backends - Win32/WGL,
    Cocoa/NSGL, X11/GLX, Wayland/EGL, plus the EGL, OSMesa, and null
    contexts), the public 'include/GLFW' headers, the 'CMake/' modules,
    'deps/wayland' (protocol XML consumed by the Wayland backend),
    'deps/mingw' (DirectInput/XInput compat headers for MinGW builds), and the
    'LICENSE.md', 'CONTRIBUTORS.md', and 'README.md' files. Deleted: 'tests/',
    'examples/', 'docs/', '.github' and other CI metadata, editor config, and
    the example/test-only helpers under 'deps/' (glad, nuklear, getopt,
    tinycthread, linmath, stb_image_write).
  - Every kept file is byte-for-byte upstream; no GLFW source was edited, so
    the only differences from upstream are the deletions listed above.
