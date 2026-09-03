# Building HTMLButcher

HTMLButcher 1.1.0.28 is a wxWidgets 2.8-era C++ desktop application built with CMake. This
document covers prerequisites, building, regenerating the checked-in generated files, and
packaging.

## Prerequisites

| Dependency | Required | Notes |
|---|---|---|
| CMake | yes | 2.6+ per `CMakeLists.txt`. See the CMake 4.x note below. |
| A C++ compiler | yes | MSVC on Windows, GCC/Clang elsewhere. C++11 is required. |
| wxWidgets | yes | **Unicode** build, 2.8 series. Components: `core base aui html xrc adv xml` |
| FreeImage | yes | All image decoding, cropping, quantization, transparency and saving |
| GTK2 | Linux only | Not needed on Windows or macOS |
| wxStEdit | optional | Syntax-highlighted CSS/HTML editing. Windows only in practice |

### wxWidgets

The code targets the **2.8** series — the reference build was wxMSW 2.8.9. There is exactly one
version conditional in the whole tree, a `wxCHECK_VERSION(2, 9, 0)` compatibility patch in
`src/HTMLButcherApp.cpp`, so 2.9 mostly works but is not the primary target.

Discovery uses CMake's stock `FindwxWidgets`, so set `wxWidgets_ROOT_DIR` (and `wxWidgets_LIB_DIR`
on Windows) if wx is not in a default location.

### FreeImage

Located by `cmake/modules/FindFreeImage.cmake`, which searches:

- **Windows** — `%PROGRAMFILES%/FreeImage/include`, `$ENV{FREEIMAGE}/Dist`,
  `${PROJECT_SOURCE_DIR}/FreeImage/include`, and for the library `%PROGRAMFILES%/FreeImage/lib`,
  `$ENV{FREEIMAGE}/Dist`, `${PROJECT_SOURCE_DIR}/FreeImage/{bin,lib}`
- **Unix** — `/usr/include`, `/usr/local/include`, `/sw/include`, `/opt/local/include`, and
  `/usr/lib64`, `/usr/lib`, `/usr/local/lib64`, `/usr/local/lib`, `/sw/lib`, `/opt/local/lib`

The easiest route on Windows is to point the `FREEIMAGE` environment variable at the FreeImage
distribution so that `$ENV{FREEIMAGE}/Dist` contains both `FreeImage.h` and `FreeImage.lib`.

> **Careful:** `find_package(FreeImage REQUIRED)` does **not** fail when FreeImage is absent. The
> bundled Find module never calls `find_package_handle_standard_args`, so the `REQUIRED` keyword is
> inert — configuration continues with `FREEIMAGE_FOUND=FALSE` and an empty `FREEIMAGE_LIBRARIES`,
> and you get an unexplained *link* error much later. If linking fails on FreeImage symbols, check
> `FREEIMAGE_INCLUDE_PATH` and `FREEIMAGE_LIBRARY` in your `CMakeCache.txt` rather than trusting
> that configuration succeeded.

### wxStEdit (optional)

When found, `BUTCHER_USE_STEDIT` is defined and `ButcherControl_FmtTextCtrl` derives from
`wxSTEditor` instead of `wxTextCtrl`, giving syntax-highlighted CSS/HTML editing. It is a
compile-time base-class swap, so the feature cannot be toggled at runtime.

`cmake/modules/Findwxstedit.cmake` looks under `%PROGRAMFILES%/wxstedit`, `$ENV{wxstedit}`, or
`${PROJECT_SOURCE_DIR}/wxstedit` for `wx/stedit/stedit.h` and `wxcode_msw28u_stedit`.

> **Careful:** the module's Unix branch is broken — it searches for `GL/glew.h` instead of the
> wxStEdit header (copy-pasted from a `FindGLEW`). On a Unix machine without GLEW, wxStEdit is
> simply never found; on one *with* GLEW it is falsely reported found and the build then tries to
> link the Windows-only `wxcode_msw28u_stedit`. Treat wxStEdit as Windows-only until that module is
> fixed.

### CMake 4.x

`CMakeLists.txt` declares `cmake_minimum_required(VERSION 2.6)`. CMake 3.x only warns:

```
CMake Deprecation Warning at CMakeLists.txt:3 (cmake_minimum_required):
  Compatibility with CMake < 2.8.12 will be removed from a future version of CMake.
```

CMake **4.x removes that compatibility entirely** and will hard-error. Until the minimum is raised,
configure with `-DCMAKE_POLICY_VERSION_MINIMUM=3.5`.

## Building

```sh
cmake -S . -B build
cmake --build build --config Release
```

### Options

| Option | Default | Effect |
|---|---|---|
| `HB_WITH_DEMO` | `OFF` | Also build `HTMLButcherDemo`, the feature-limited demo binary |
| `HB_WITH_UTILS` | `OFF` | Also build `MetadataFileViewer`, a debug tool that dumps a project file's metadata tree |

### Targets

- `HTMLButcher` — the application
- `hbcppcomp` — small in-tree static helper library (`util/cppcomp`), always built
- `HTMLButcherDemo` — only with `HB_WITH_DEMO=ON`
- `MetadataFileViewer` — only with `HB_WITH_UTILS=ON`

### Output location

Binaries go to **`<source-dir>/bin`** and libraries to **`<source-dir>/lib`**, not into the build
directory — the root `CMakeLists.txt` pins `CMAKE_RUNTIME_OUTPUT_DIRECTORY`,
`CMAKE_LIBRARY_OUTPUT_DIRECTORY` and `CMAKE_ARCHIVE_OUTPUT_DIRECTORY` to `${CMAKE_SOURCE_DIR}`.
Both directories are gitignored, so an out-of-tree build still writes into the source tree.

With multi-config generators (Visual Studio) the executable lands in `bin/<Config>/`, e.g.
`bin/Release/HTMLButcher.exe` — which is why the Windows installer script expects
`bin\Release\htmlbutcher.exe`. Single-config generators (Makefiles, Ninja) write straight to `bin/`.

### Compiler definitions

Set automatically; you do not normally pass these yourself:

- Windows: `_CRT_SECURE_NO_WARNINGS`, `UNICODE`, `_UNICODE`; the app links as a `WIN32` subsystem
  executable
- Debug configurations: `HTMLBUTCHER_DEBUG`, `HTMLBUTCHER_KEEPOLDSAVE`, `__WXDEBUG__`
- With wxStEdit found: `BUTCHER_USE_STEDIT`
- Demo target: `HTMLBUTCHER_DEMO`
- macOS: builds a `MACOSX_BUNDLE` with `CMAKE_OSX_ARCHITECTURES x86_64`; this is the only platform
  where CPack is configured (`cpack` installs to `/Applications`)

> **No `-std=` flag is set anywhere.** Commit `26b6301` converted the codebase to C++11
> (`std::unique_ptr`/`shared_ptr` replacing an in-tree `linked_ptr`), but no standard is requested
> in any `CMakeLists.txt`. This is fine for MSVC and for GCC/Clang versions that default to C++11
> or later; on an older GCC you must add `-std=c++11` yourself.

### Adding source files

The source lists in `src/CMakeLists.txt` are **explicit — there is no globbing**. A new file must
be added by hand to the appropriate variable (`SOURCES_MAIN`, `SOURCES_DIALOG`, `SOURCES_PROJECT`,
`SOURCES_VIEW`, `SOURCES_UTILS`) along with its `SOURCE_GROUP` entry, or it will simply not be
compiled.

`src`, `src/dialogs`, `src/project` and `src/view` are all on the include path, so headers are
included by bare filename regardless of which subdirectory they live in.

## Regenerating checked-in files

Some generated files are committed and must be regenerated by hand after editing their sources.

### XRC resources

`resources/htmlbutcher_resources.cpp` is a `wxrc`-generated C++ blob containing the XRC layouts and
icons, compiled into the binary and loaded by `InitXmlResource()`. **Never hand-edit it.** Edit
`resources/htmlbutcher.xrc`, then regenerate:

```sh
wxrc htmlbutcher.xrc -v -c -o htmlbutcher_resources.cpp
```

`resources/compile.bat` does this but hardcodes a path into a wxMSW 2.8.9 source tree; adjust it to
your own `wxrc`.

### Translations

Sources are `locale/*.po` (currently `ja` and `pt_BR`). `locale/install.sh` compiles each with
`msgfmt` into `<lang>/htmlbutcher.mo`. `locale/install.bat` does the same on Windows and also
copies the matching wxWidgets catalog from `deps/wx/locale` as `wxstd.mo`.

### Manual

DocBook sources live in `doc/docbook/src`. From `doc/docbook`:

| Script | Output |
|---|---|
| `makehtml.sh` | `html/` (xsltproc) |
| `makehh.sh` | `htmlhelp/h.hhp`, then zips `htmlbutcher.htb` — the wxWidgets help book |
| `makepdf.sh` | `pdf/htmlbutcher.pdf` (dblatex) |

Requires `xsltproc`, `zip` and `dblatex`. The `.bat` equivalents hardcode a `libxslt` install path.

> `makehh.bat` only produces the HTML Help **project** (`htmlhelp/h.hhp`). Compiling that into the
> `htmlbutcher.chm` the Windows installer expects is a separate manual step with HTML Help Workshop
> (`hhc.exe`); no script in the repo does it.

## Packaging

All packaging scripts consume already-built artifacts, and they fail confusingly if run too early.
Before packaging:

1. Build the binary into `bin/` (and `HTMLButcherDemo` too, if you want the demo package).
2. **Generate the help file** — Linux packages copy `doc/docbook/htmlbutcher.htb`; the Windows
   installer copies `doc/docbook/htmlhelp/htmlbutcher.chm`.

### Debian / Ubuntu / Raspberry Pi / Pandora

Run `build_htmlbutcher.sh` from inside the relevant `setup/` directory (`setup/ubuntu`,
`setup/debian`, `setup/rpi`, `setup/pandora`). The script re-executes itself under `sudo`, then
stages a package tree under `build/`: copies and `strip`s the binary, installs the desktop/MIME
files from `data/`, adds the `.htb` help file, compiles each `.po` into
`usr/share/locale/<lang>/LC_MESSAGES/`, substitutes `%%SIZE%%` in `DEBIAN/control` with the staged
size from `du`, and finally runs `dpkg-deb --build`.

It builds the demo package too, but only if `bin/HTMLButcherDemo` exists — detection is a plain
`[ -f ../../bin/HTMLButcherDemo ]` test.

The variants differ mainly in their `DEBIAN/control`: Ubuntu is `i386` / `1.1.0.28-ubuntu0`,
Raspberry Pi is `armhf` / `1.1.0.28-raspbian0`. Declared runtime dependencies are `libc6 (>= 2.4)`,
`libstdc++6 (>= 4.2)`, `libgtk2.0-0 (>= 2.12.0)`, `libglib2.0-0 (>= 2.16.0)`, `libx11-6`.

Requires `dpkg-deb`, `msgfmt` and root.

### Windows

Inno Setup scripts: `setup/win32/htmlbutcher.iss` and `setup/win64/htmlbutcher.iss` (plus
`htmlbutcherdemo.iss` and `htmlbutcher-locale-files.iss`). They use `SourceDir=..\..` and expect
`bin\Release\htmlbutcher.exe` and `doc\docbook\htmlhelp\htmlbutcher.chm`, and they bundle the
redistributable from `setup/win*/redist`.

> The scripts **hardcode `m:\prog\src\FreeImage\Dist\FreeImage.dll`** as the source of the bundled
> FreeImage DLL. Edit that path before building an installer on any other machine.

### Red Hat / Fedora

RPM spec files in `setup/redhat/fedora9/` (`htmlbutcher.spec`, `htmlbutcher-demo.spec`), driven by
`setup/redhat/build_htmlbutcher.sh`.

### macOS

Either `cpack` (configured in the root `CMakeLists.txt`, installs to `/Applications`) or the
PackageMaker project at `setup/macosx/htmlbutcher.pmproj`. Note `HTMLButcher-Info.plist` still
carries a placeholder `CFBundleIdentifier` of `com.yourcompany.Debug`.

## Releasing

The version number is duplicated in four places and they must be changed together:

1. `src/HTMLButcherVersion.h` — `HTMLBUTCHERVERSION_1..4`, `..._NUMBER`, `..._STRING`
2. root `CMakeLists.txt` — the `MACOSX_BUNDLE_*_VERSION*` values and the CPack major/minor/patch
   fields
3. `setup/win32/*.iss` and `setup/win64/*.iss` — `AppVerName`, `VersionInfoVersion`,
   `OutputBaseFilename`
4. `setup/*/htmlbutcher*/DEBIAN/control` — `Version`

The project-file format has its own separate version (`BFILE_VERSION` in `src/ButcherFileDefs.h`,
currently 2), with a change log in `doc/bfileversion.txt`. Bump it only when the `.hbp` layout
changes.

## Tests

There is no test suite. `util/HBTest/HBTMain.cpp` looks like a Google Test suite but is dead code:
no `CMakeLists.txt` references it, the repo contains no `enable_testing()`, `add_test()` or
`find_package(GTest)`, and the file still includes a `BProject.h` that no longer exists — so it
would not compile as written. There is no CI configuration either.

Verification is manual: build, run the binary, and open a project such as
`doc/manual/tutorial/simple_apple/apple.hbp`.

## Known build-system defects

Collected here so they are not re-diagnosed each time:

- `find_package(FreeImage REQUIRED)` (`CMakeLists.txt:29`) cannot fail — the bundled Find module
  ignores `REQUIRED`, so a missing FreeImage surfaces as a link error instead. Its `DOC` strings
  also still mention GLEW.
- `Findwxstedit.cmake`'s Unix branch searches for `GL/glew.h`, so wxStEdit is never correctly found
  on Unix and is falsely found if GLEW is installed.
- The wxStEdit link line hardcodes wx 2.8 MSW library names (`wxmsw28ud_stc.lib` /
  `wxmsw28u_stc.lib`), so it cannot work against another wx version or platform.
- `include_directories(${HTMLButcher_SOURCE_DIR}/secure)` (`CMakeLists.txt:61`) points at a
  directory that does not exist — the licensing code was removed before the project was
  open-sourced. Harmless, but misleading.
- `util/MetadataFileViewer/CMakeLists.txt` links `${Cryptopp_LIBRARIES}`, a variable never defined
  anywhere; it expands to nothing.
- `doc/htmlbutcher.Doxyfile` has absolute `INPUT` and `OUTPUT_DIRECTORY` paths pointing at
  `C:/prog/personal/HTMLButcher`.
