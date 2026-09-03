# Building HTMLButcher

HTMLButcher 1.1.0.28 is a wxWidgets C++ desktop application built with CMake.

On **Windows and macOS** the build downloads and compiles its own dependencies, so a clean checkout
plus CMake and a compiler is enough:

```sh
cmake -S . -B build
cmake --build build --config Release
```

On **Linux** it uses the distribution's packages instead, because wxGTK needs the GTK development
packages present regardless — a downloaded wxWidgets would not make the build self-contained there.

## Dependencies

| Dependency | Required | Fetched? | Notes |
|---|---|---|---|
| CMake ≥ 3.18 | yes | — | 3.18 is the floor, for `file(ARCHIVE_EXTRACT)` |
| A C++11 compiler | yes | — | MSVC, GCC or Clang |
| wxWidgets 3.2 | yes | on Windows/macOS | Components: `core base aui html xrc xml stc` |
| FreeImage 3.18 | yes | on Windows/macOS | All image decoding, cropping, quantization, transparency and saving |
| GTK | Linux only | never | Comes in with the wxGTK development package |

### `HB_FETCH_DEPS`

Controls where dependencies come from. The default is per-platform: **`ON`** on Windows and macOS,
**`OFF`** on Linux.

```sh
cmake -S . -B build -DHB_FETCH_DEPS=ON    # download and build wxWidgets + FreeImage
cmake -S . -B build -DHB_FETCH_DEPS=OFF   # use system libraries
```

With it `ON`, CMake fetches pinned source archives (verified by SHA256) into the build tree and
builds them as part of the project:

- **wxWidgets 3.2.6** — the release tarball, which carries wx's own bundled zlib, libpng, libjpeg,
  libtiff, expat and Scintilla. This is what makes a Windows build self-contained. It is configured
  static and Unicode (`src/ButcherFileDefs.h` hard-errors on a non-Unicode build).
- **FreeImage 3.18.0** — the source distribution. FreeImage ships no CMake build of its own, so
  `cmake/freeimage/CMakeLists.txt` supplies one; see the note below.

Nothing is written into the source tree, and `/build` is gitignored.

With it `OFF`, `find_package` is used for both. On Debian/Ubuntu:

```sh
sudo apt install build-essential cmake libwxgtk3.2-dev libfreeimage-dev
```

`libwxgtk3.2-dev` pulls in the GTK development packages. On Fedora the equivalents are
`wxGTK-devel` and `freeimage-devel`.

A system FreeImage is located by `cmake/modules/FindFreeImage.cmake`, which honours the `FREEIMAGE`
environment variable as well as the usual system prefixes.

## Two invariants to preserve

Both of these produce a build that compiles and links but misbehaves at runtime, so they are worth
knowing before touching the dependency wiring.

1. **`FREEIMAGE_LIB` must be defined when, and only when, FreeImage is static.**
   `src/wxFreeImage.cpp` guards its `FreeImage_Initialise()` / `FreeImage_DeInitialise()` calls on
   that macro. A static FreeImage has no library constructor, so without the definition **no image
   format plugin is ever registered and every load and save fails silently**. A shared FreeImage
   registers them itself, so there the definition must be absent.

   This is handled by the `freeimage` target exporting `FREEIMAGE_LIB` as a `PUBLIC` compile
   definition in the fetch path, and the imported target deliberately not doing so in the system
   path. Do not add it by hand.

2. **FreeImage's colour order must be BGR.** `src/wxFreeImage.cpp` unconditionally swaps red and
   blue when handing scanlines to `wxImage`. A FreeImage built with `FREEIMAGE_COLORORDER_RGB`
   inverts every image. The fetch path pins `FREEIMAGE_COLORORDER=0` explicitly rather than relying
   on the endianness-based default.

There is also an ordering constraint in the top-level `CMakeLists.txt`: the dependency block must
stay **above** the `add_definitions(-DUNICODE -D_UNICODE)` call and the `CMAKE_*_OUTPUT_DIRECTORY`
settings. Both are directory-scoped and would otherwise leak into the fetched dependencies —
`UNICODE` in particular would reach FreeImage's ~15 bundled third-party C libraries, none of which
are Unicode builds. There is a comment saying so in the file; please leave it there.

## Build options

| Option | Default | Effect |
|---|---|---|
| `HB_FETCH_DEPS` | platform | Download and build dependencies (see above) |
| `HB_WITH_DEMO` | `OFF` | Also build `HTMLButcherDemo`, the feature-limited demo binary |
| `HB_WITH_UTILS` | `OFF` | Also build `MetadataFileViewer`, which dumps a project file's metadata tree |

### Targets

- `HTMLButcher` — the application
- `hbcppcomp` — small in-tree static helper library (`util/cppcomp`), always built
- `HTMLButcherDemo`, `MetadataFileViewer` — only with the options above

### Output location

Binaries go to `<source-dir>/bin` and libraries to `<source-dir>/lib`, not into the build
directory. With multi-config generators (Visual Studio) the executable lands in `bin/<Config>/`,
e.g. `bin/Release/HTMLButcher.exe` — which is why the Windows installer script expects
`bin\Release\htmlbutcher.exe`. Single-config generators (Makefiles, Ninja) write straight to `bin/`.

Both directories are gitignored.

### Compiler definitions

Set automatically:

- Windows: `_CRT_SECURE_NO_WARNINGS`, `UNICODE`, `_UNICODE`; links as a `WIN32` subsystem executable
- Debug configurations: `HTMLBUTCHER_DEBUG`, `HTMLBUTCHER_KEEPOLDSAVE`, `__WXDEBUG__`
- Demo target: `HTMLBUTCHER_DEMO`
- macOS: a `MACOSX_BUNDLE` with `CMAKE_OSX_ARCHITECTURES x86_64`; the only platform where CPack is
  configured (`cpack` installs to `/Applications`)

C++11 is requested explicitly via `CMAKE_CXX_STANDARD`.

### Adding source files

The source lists in `src/CMakeLists.txt` are **explicit — there is no globbing**. A new file must be
added by hand to the appropriate variable (`SOURCES_MAIN`, `SOURCES_DIALOG`, `SOURCES_PROJECT`,
`SOURCES_VIEW`, `SOURCES_UTILS`) along with its `SOURCE_GROUP` entry, or it is simply not compiled.

`src`, `src/dialogs`, `src/project` and `src/view` are all on the include path, so headers are
included by bare filename regardless of subdirectory.

## Regenerating checked-in files

### XRC resources

`resources/htmlbutcher_resources.cpp` is a `wxrc`-generated C++ blob holding the XRC layouts and
icons, compiled into the binary and loaded by `InitXmlResource()`. **Never hand-edit it.** Edit
`resources/htmlbutcher.xrc`, then regenerate:

```sh
wxrc htmlbutcher.xrc -v -c -o htmlbutcher_resources.cpp
```

In the fetch path `wxrc` is built as part of wxWidgets, under the build tree.

### Translations

Sources are `locale/*.po` (`ja`, `pt_BR`). `locale/install.sh` compiles each with `msgfmt` into
`<lang>/htmlbutcher.mo`. `locale/install.bat` does the same on Windows and also copies the matching
wxWidgets catalog from `deps/wx/locale` as `wxstd.mo`.

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

Packaging scripts consume already-built artifacts and fail confusingly if run too early. Before
packaging:

1. Build the binary into `bin/` (and `HTMLButcherDemo` too, for the demo package).
2. **Generate the help file** — Linux packages copy `doc/docbook/htmlbutcher.htb`; the Windows
   installer copies `doc/docbook/htmlhelp/htmlbutcher.chm`.

### Debian / Ubuntu / Raspberry Pi / Pandora

Run `build_htmlbutcher.sh` from inside the relevant `setup/` directory. It re-executes itself under
`sudo`, stages a package tree under `build/`, strips the binary, installs the desktop/MIME files
from `data/`, adds the `.htb` help file, compiles each `.po`, substitutes `%%SIZE%%` in
`DEBIAN/control` from `du`, and runs `dpkg-deb --build`. The demo package is built only if
`bin/HTMLButcherDemo` exists.

Requires `dpkg-deb`, `msgfmt` and root.

> The `setup/ubuntu` and `setup/rpi` `DEBIAN/control` files declare neither `libfreeimage3` nor a
> `libwxgtk*` dependency, unlike `setup/debian`. Those two packages are incomplete as shipped.

### Windows

Inno Setup scripts under `setup/win32` and `setup/win64`. They use `SourceDir=..\..` and expect
`bin\Release\htmlbutcher.exe` plus `doc\docbook\htmlhelp\htmlbutcher.chm`, and bundle the
redistributable from `setup/win*/redist`.

> The scripts still copy `FreeImage.dll` from an absolute path on the machine they were written
> on, so that line needs editing anywhere else. A fetched FreeImage is linked **statically**, so
> it is not needed for a fetch-path build at all — the executable has no FreeImage DLL dependency.

### Red Hat / Fedora

RPM specs in `setup/redhat/fedora9/`, driven by `setup/redhat/build_htmlbutcher.sh`.

### macOS

Either `cpack` (installs to `/Applications`) or `setup/macosx/htmlbutcher.pmproj` (PackageMaker).
Note `HTMLButcher-Info.plist` still carries a placeholder `CFBundleIdentifier` of
`com.yourcompany.Debug`.

## Releasing

The version number is duplicated in four places that must change together:

1. `src/HTMLButcherVersion.h` — `HTMLBUTCHERVERSION_1..4`, `..._NUMBER`, `..._STRING`
2. root `CMakeLists.txt` — the `MACOSX_BUNDLE_*_VERSION*` values and the CPack major/minor/patch
3. `setup/win32/*.iss` and `setup/win64/*.iss` — `AppVerName`, `VersionInfoVersion`,
   `OutputBaseFilename`
4. `setup/*/htmlbutcher*/DEBIAN/control` — `Version`

The project-file format has its own version (`BFILE_VERSION` in `src/ButcherFileDefs.h`, currently
2), with a change log in `doc/bfileversion.txt`. Bump it only when the `.hbp` layout changes.

## Tests

There is no test suite. `util/HBTest/HBTMain.cpp` looks like a Google Test suite but is dead code:
no `CMakeLists.txt` references it, the repo has no `enable_testing()`, `add_test()` or
`find_package(GTest)`, and it includes a `BProject.h` that no longer exists — so it would not
compile. There is no CI configuration either.

Verification is manual: build, run, and open a project such as
`doc/manual/tutorial/simple_apple/apple.hbp`.

## Line endings

`.gitattributes` pins the policy: all text is stored LF in the repository. Shell scripts and the
Debian `control`/`postinst`/`postrm` files are forced to LF in the working tree too, since `/bin/sh`
and `dpkg-deb` break on stray carriage returns; `.bat`, `.iss` and `.rc` are CRLF. `.hbp` files are
marked binary — they are HTMLButcher's own binary format and EOL translation would corrupt them.

## Known build-system defects

- `util/MetadataFileViewer` compiles `src/ButcherMetadataFile.cpp` from the main tree rather than
  linking anything, so the two can drift.
- `doc/htmlbutcher.Doxyfile` has absolute `INPUT` and `OUTPUT_DIRECTORY` paths baked in, pointing
  at wherever the project happened to live when it was written.
- `resources/compile.bat` hardcodes a path into a wxMSW 2.8.9 source tree for `wxrc`.
- The `.iss` installers hardcode an absolute path to `FreeImage.dll` (see Packaging).
