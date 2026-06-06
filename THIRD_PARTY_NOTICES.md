# Third-Party Notices

'dans-platform' vendors third-party code directly under 'vendor/' so the
project can be built from a single fixed Git commit without submodules or
configure-time downloads. Exact upstream URLs, tags, and commits are recorded
in 'vendor/versions.md'.

The vendored trees are intentionally shortened copies. The removed material is
limited to non-runtime material such as tests, examples, generated docs, CI
metadata, and editor/IDE files. Core source, public headers, every platform
backend, the build integration, and license notices are kept, so each library
still builds for all of its supported targets.

## Vendored Dependencies

| Dependency | Location | Upstream pin | License notice |
| --- | --- | --- | --- |
| GLFW | 'vendor/glfw' | 'glfw/glfw' tag '3.4', commit '7b6aead9fb88b3623e3b3725ebb42670cbe4c579' | zlib/libpng, see 'vendor/glfw/LICENSE.md' |

## Shortened Copies

- GLFW was shortened to the buildable library and kept functional for every
  GLFW target (Win32, Cocoa, X11, Wayland). Kept: 'src/', the public
  'include/GLFW' headers, the 'CMake/' build modules, the 'deps/wayland'
  protocol data and 'deps/mingw' compatibility headers that the library build
  references, and the license, contributor, and readme files. Removed:
  'tests/', 'examples/', 'docs/', CI metadata, editor config, and the
  example/test-only helpers under 'deps/' (glad, nuklear, getopt, tinycthread,
  linmath, stb_image_write). No GLFW source was edited; the only differences
  from upstream are these deletions.
