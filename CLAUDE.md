# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

HTMLButcher 1.1.0.28 — an HTML slicing tool. It takes a source image (a mockup, typically a PSD)
and produces an HTML page that reproduces the layout, cutting the image into pieces and emitting
either a nested `<table>` structure or absolutely-positioned `<div>`s, so text and images can be
edited independently afterwards.

wxWidgets C++ desktop GUI application, CMake build, BSD-3 licensed, open-sourced in 2013. It began
on wxWidgets 2.8 and now targets **wx 3.2**. The style still reads as 2.8-era (see Conventions), but
nothing requires 2.8 any more and there are no `wxCHECK_VERSION` forks left in the tree.

## Build

```sh
cmake -S . -B build
cmake --build build --config Release
```

On Windows and macOS that downloads and builds wxWidgets 3.2 and FreeImage; on Linux it uses the
distro packages (`libwxgtk3.2-dev`, `libfreeimage-dev`). The switch is `HB_FETCH_DEPS`, defaulted
per platform — `ON` on Windows/macOS, `OFF` on Linux. Output goes to `<source-dir>/bin` (pinned to
`CMAKE_SOURCE_DIR`, not the build dir). Other options: `HB_WITH_DEMO`, `HB_WITH_UTILS` (both `OFF`).

**See `BUILDING.md`** for prerequisites, packaging and release steps. Two invariants from it are
worth repeating, because breaking either yields a build that compiles and links but fails at
runtime:

- **`FREEIMAGE_LIB` must be defined only for a static FreeImage.** `src/wxFreeImage.cpp` guards its
  `FreeImage_Initialise()` on it, so a static build without it registers no format plugins and every
  load and save fails silently. The `freeimage` target carries the definition — never add it by hand.
- **FreeImage must be BGR-ordered.** `src/wxFreeImage.cpp` unconditionally swaps red and blue, so an
  RGB-ordered build inverts every image. The fetch path pins `FREEIMAGE_COLORORDER=0`.

Also, in the top-level `CMakeLists.txt` the dependency block must stay **above** the
`add_definitions(-DUNICODE)` call and the output-directory settings — both are directory-scoped and
would otherwise leak into the fetched dependencies.

Two things to know before editing:

- **Adding a source file requires editing `src/CMakeLists.txt` by hand.** The lists
  (`SOURCES_MAIN`, `SOURCES_DIALOG`, `SOURCES_PROJECT`, `SOURCES_VIEW`, `SOURCES_UTILS`) are
  explicit — there is no globbing — and each has a matching `SOURCE_GROUP`. A new file not added
  there is silently never compiled.
- `src`, `src/dialogs`, `src/project` and `src/view` are all on the include path, so headers are
  included **by bare filename** regardless of subdirectory.

## Tests

There are none wired up. `util/HBTest/HBTMain.cpp` looks like a Google Test suite but is dead code:
nothing references it from any `CMakeLists.txt`, there is no `enable_testing()`/`add_test()`/
`find_package(GTest)` in the repo, and it includes a `BProject.h` that no longer exists (the class
is now `src/project/ButcherProject.h`), so it would not compile. No CI config either.

Verification is manual: build, run the GUI, open a project such as
`doc/manual/tutorial/simple_apple/apple.hbp`.

## Architecture

Code is organised by layer:

| Path | Contents |
|---|---|
| `src/` (root) | Cross-cutting infrastructure, `B*`-prefixed: `BList.h`, `BImage.h`, `BHTML.h`, `BProc.h`, `BUtil.h`, `BControls.h`, `BExcept.h`, plus `ButcherMetadataFile.*`, `ButcherFileDefs.h`, `wxFreeImage.*` |
| `src/project/` | The domain model, ~60 `ButcherProject*` classes |
| `src/view/` | Canvas/editor widgets and their custom wx events |
| `src/dialogs/` | Dialogs (`Dialog*` files, `HTMLButcher*Dialog` classes) and one wizard |
| `util/cppcomp/` | `hbcppcomp` static lib — path/string/client-data helpers in namespace `cppcomp` |

### The domain model

`ButcherProject` (`src/project/ButcherProject.h`) is the aggregate root, and **is itself a
`wxEvtHandler`**. It owns ten collections by value, each exposed as a reference accessor: `Files()`,
`Masks()`, `Views()`, `ColorSchemes()`, `ImageFormats()`, `AssortedFiles()`, `AssortedFileGroups()`,
`FilePaths()`, `CSSFiles()`, `ProjectOptions()`.

Three axes make up the core:

- **`ButcherProjectFile`** — a source image, embedded in the project file as binary metadata.
- **`ButcherProjectMask`** — the geometry: cut lines and the areas they bound.
  `ButcherProjectMaskRoot` owns its containers by value; `ButcherProjectMaskInner` (a mask nested
  inside an area, giving nested tables) owns them by pointer, deliberately — creating them
  statically risks infinite recursion.
- **`ButcherProjectView`** — one output HTML page: a `fileid_` + `maskid_`, up to
  `BUTCHERCONST_VIEW_MAXALTERNATE` (8) alternate images, and the HTML/CSS output settings. Owns
  `GenerateHTML()`.

A mask holds up to three containers keyed by area class: `AC_DEFAULT` (the real table grid,
`ButcherProjectMaskContainerJoined`), `AC_GLOBAL` and `AC_MAP` (free-floating areas and `<map>`
hotspots, both `ButcherProjectMaskContainerSplit`).

**Geometry is defined by line identity, not coordinates.** A `ButcherProjectArea` holds four *line
pointers* (`left_`, `top_`, `right_`, `bottom_`); its rectangle is derived via `GetRect()`. In a
joined container interior lines are **shared** between adjacent areas, so moving one line resizes
both neighbours and deleting a line merges areas. `FindAreaByLines()` looks areas up by their four
line ids. Treat any edit to lines or areas as a cascading operation — the topology algebra lives in
`ButcherProjectMaskContainerJoined` (`AddLine`, `DeleteLine`, `Check`, `CheckConnect`, `LineAreas`).

**One mask can serve many views.** `ButcherProjectAreaConfigs` is a map keyed by *view id* with
`DEFAULT_CONFIG = 0`, so each view can override any area's rendering. This is why nearly every
model method takes a `BLID_t viewid` parameter.

**Cross-references are ids, not pointers.** `BLID_t` (`src/BList.h`) is the universal handle and
`ButcherList<T>` is a `map<BLID_t, shared_ptr<T>>` with a monotonic `maxid_`. Relations are stored
as ids (`fileid_`, `maskid_`, `cssfileid_`, `imageformat_`, …) and resolved lazily through
`GetProject()->Xxx().Get(id)`, **which throws if the id is gone**. Back-pointers (`project_`,
`mask_`, `area_`, `parent_`) are raw and non-owning.

`ButcherList<T>` is a callback base class, not just storage. Subclasses override `can_delete`,
`do_deleting`, `do_deleted`, `do_modified` and `do_clear` to veto deletions and fire the right
event; the public mutators are thin wrappers over protected `op_add` / `op_delete` / `op_edit` /
`op_move` / `op_clear`.

**The model draws itself.** `ButcherProjectMask::Draw`/`DrawArea`/`DrawLine`/`DrawPreview` take a
`ButcherViewBase*` and a `wxDC*` and paint, taking pens and brushes from
`ButcherProjectColorScheme`. A `Draw()` returning `false` means the cached underlying image is
stale and needs regenerating. `ButcherProjectAreaConfigPreview` caches a rendered image per
area-config per view and uses `ConfigChanged()`/`NeedImage()` to decide when to re-slice — that is
what makes the in-place preview with file sizes work.

### GUI

`HTMLButcherApp : wxApp` → `HTMLButcherFrame : wxFrame`, a single main window using a
`wxAuiManager` for docking. It hosts `ButcherViewEditor` (the editing canvas) and
`ButcherViewDisplay` (a read-only zoom pane), both deriving from
`ButcherView : wxControl, ButcherViewBase`, with `ButcherDocument : wxControl` as the actual paint
surface — it re-broadcasts paint, mouse and key input as Butcher events.

There are **three coordinate spaces**: image/global pixels, mask-local (nested masks are offset by
their parent area), and client/zoomed. Conversions are centralized in
`ButcherViewBase::PosToClient`/`ClientToPos` and the mask's `GlobalToMask`/`MaskToGlobal` — use
them rather than doing the arithmetic inline.

Dialogs derive from `ButcherControl_Dialog`. **`HTMLButcherListEditDialog`
(`src/dialogs/DialogListEdit.h`) is a reusable list-CRUD base** — subclasses only override
`do_load`/`do_add`/`do_edit`/`do_remove`/`do_duplicate`. Reuse it rather than writing a new list
dialog from scratch.

Right-click menus for areas and lines live in `src/BProc.h`
(`ButcherProjectProc_AreaMenu`, `ButcherProjectProc_LineMenu`) — **not** in the frame, whose
equivalent code is still present but commented out. The live menu-id ranges are the ones in
`BProc.h`.

### Two notification channels

Deliberately separate, and easy to confuse:

1. **Model → GUI.** `ButcherProjectEvent : wxEvent`, event type `wxEVT_BUTCHERPROJECT_ACTION`, with
   a `BPE_*` enum (`BPE_PROJECTOPEN`, `BPE_MODIFIED`, `BPE_<THING>MODIFIED`, `BPE_<THING>DELETED`,
   `BPE_PROGRESS`, …) and `eid_`/`eid2_` payload ids. Because the project *is* a `wxEvtHandler`,
   listeners `Connect()` straight to it. Dispatch is **synchronous `ProcessEvent`**, not posted —
   the `wxPostEvent` branch is commented out.
2. **Model → model.** `ButcherProjectEventNotify` wraps an event and adds `changecount_` /
   `removecount_` counters plus an `isexecute` flag — a **two-phase count-then-execute pass**.
   `ButcherProject::ExecuteEventNotify()` walks every collection so that, for example, deleting a
   file lets every file link and view report how many references it *would* change before the
   deletion is committed. This is the referential-integrity mechanism; preserve it when adding
   anything that holds an id.

Every setter funnels through `ProjectModified()` →
`DoProjectModified(GetProjectModifiedItem(), …)`, where `GetProjectModifiedItem()` is overridden per
class to return the right `BPE_*MODIFIED` constant, and nested objects override
`SendProjectModified` to re-route notifications up to the owning mask/view/area.

Batching uses RAII scope guards, the signature idiom of this codebase:
`ButcherProjectBaseAutoUpdate` (coalesce notifications), `ButcherProjectBaseAutoDisable` (suppress
them), `ButcherProjectBaseAutoProgress` (push/pop a progress entry driving `BPE_PROGRESS`).

**There is no undo/redo and no command pattern** — zero `Undo` hits in `src/`. Edits mutate the
model in place and rely on event-driven refresh.

## The `.hbp` project format

Custom **binary**, not XML. Two layers:

1. **`ButcherMetadataFile`** (`src/ButcherMetadataFile.h`) — a generic self-describing container,
   signature `"BMTD"`. A recursive tree of `ButcherMetadataStorage` → `ButcherMetadataDataList`
   (keyed by numeric id) → `ButcherMetadataData` (a record) → `ButcherMetadataDataItem` (a field),
   with field types `DT_METADATA`, `DT_STRING`, `DT_INTEGER`, `DT_BINARY`. Binary items are **read
   lazily**: the file handle stays open and `wxVirtualInputStream` provides a seekable window onto a
   byte range, so embedded images stream on demand instead of sitting in RAM.
2. **`ButcherProjectMetadataFile`** — overrides the header handling to prepend signature `"BPRJ"`
   and `BFILE_VERSION 2`.

**All tag numbers live in `src/ButcherFileDefs.h`** — `BFILE_MD_*` for record kinds, `BFILE_MDI_*`
for field ids. Persisting a new property means adding a constant there *and* a
`LoadMetadata`/`SaveMetadata` pair on the class. `ButcherProject::DoLoad`/`DoSave` dispatch to each
collection in dependency order, masks last, because areas reference view ids. Saving writes a temp
file then does a three-way rename with rollback; `HTMLBUTCHER_KEEPOLDSAVE` (debug) keeps a
timestamped backup instead of deleting the old file.

Format history is logged in `doc/bfileversion.txt`. `ButcherFileDefs.h` also still carries unused
`#pragma pack(1)` structs from the v1 fixed-record format — the loader no longer uses them.

## HTML generation

`ButcherProjectView::GenerateHTML(filename)` creates the output directories, writes the linked CSS
file, builds the `<body>` tag, then calls the recursive `do_genhtml(level, generator, dirname, mask,
progress)`. Recursion happens when an area's config is `AK_MASK`, descending into
`GetInnerMask()` — that is how nested tables are produced. Cell spans come from
`ButcherProjectMaskLayout`, which flattens the line grid into rows of
`{areaid, colspan, rowspan, forcewidth, forceheight, skipline}`.

Emission goes through `ButcherHTMLGenerator` (accumulates head/title/styles/body sections, then
writes with the right `wxMBConv`) and `ButcherHTMLAttributeGenerator`, an ordered attribute map that
can parse *and* regenerate either tag attributes or CSS declarations — this is how user-supplied
"tag add" snippets get **merged** with computed attributes rather than blindly concatenated. The
older concatenating code is left commented out throughout `ButcherProjectView.cpp`.

Two layout backends, chosen by `ButcherProjectConsts::layouttype_t`: `LTYPE_TABLE` and
`LTYPE_APTABLELESS` (absolutely-positioned divs). `LTYPE_DEFAULT` means "inherit" and resolves
area config → view → project options; if it is still unresolved at generation time it **throws**.

`ButcherOutputFile` is the small abstract interface (`GetOutputFileURL`, `GetOutputFilename`,
`SaveOutputFile`, `SaveOutputPath`) implemented by everything exportable — project files, CSS files,
assorted files, area images — and is the polymorphic hook the exporter uses.
`ButcherProjectViewFileCheck` pre-flights output to detect two views writing the same file.

Images go through a factory: `ButcherImageFactory::Load(...)` (`src/BImage.h`) is the only public
way to make a `ButcherImage`, and the concrete `ButcherImage_FreeImage` keeps its methods protected.
All FreeImage types are confined to `BImage_FreeImage.*` and `wxFreeImage.*` — keep them there.

## Conventions

Match the surrounding code; there is no `.clang-format` or `.editorconfig`.

- **Class prefixes**: `ButcherProject*` = domain model, `Butcher*` = generic infrastructure,
  `ButcherControl_*` = custom widgets (note the underscore), `HTMLButcher*` = application-level,
  `wxFreeImage*` = the FreeImage wrapper, `cppcomp::ccu_*`/`wxccu_*` = the helper lib.
- **File naming**: file name matches its principal class, except infrastructure files, which use the
  terse `B` prefix. A collection lives in the plural file (`ButcherProjectMask.h` /
  `ButcherProjectMasks.h`).
- **Header guards**: `__BPROJECT_BUTCHERPROJECTMASK_H__` under `project/`, `__BVIEW_*__` under
  `view/`, `__BLIST_H__` at root. A few older files are inconsistent.
- Every file opens with a Doxygen banner (`@file`, a CVS `$Id$` keyword, `@author`, `@date`); every
  class has `@class`/`@brief`.
- **Members** use a `trailing_underscore_`. Protected/internal helpers use `snake_case`
  (`do_load`, `op_add`, `do_genhtml`, `do_modified`, `can_delete`). Enums are `PREFIX_VALUE` inside
  a typedef'd `..._t`; bitmask enums pair a `xxxflag_t` enum with a `typedef unsigned short
  xxxflags_t`.
- Accessors are inline in the header; setters end with `ProjectModified();`. Collection accessors
  return references.
- **Memory**: `shared_ptr` inside the container templates, `unique_ptr` for locals and a few owned
  members, otherwise raw pointers with manual `new`/`delete` in destructors and raw non-owning
  back-pointers. wx windows follow wx's parent-owns-child rule.
- `std::map`/`deque`/`set`, and `using namespace std;` in headers — project-wide and deliberate.
- `friend class` is used pervasively so collections can construct and mutate their items; the
  protected default constructor plus friend collection is how metadata loading creates blank
  objects.
- wx style is 2.8-era and deliberately left that way: static
  `DECLARE_EVENT_TABLE()`/`BEGIN_EVENT_TABLE` macro tables rather than `Bind()`, `wxT()` around every literal, `_()` for translations. Custom events follow an identical
  boilerplate triple: an event class with `Clone()`, a `…EventFunction` typedef, a
  `…EventHandler(func)` cast macro and an `EVT_…(id, fn)` table macro.
- Indentation is tabs. Dead code is commented out in place rather than deleted.

## Compile-time switches

`HTMLBUTCHER_DEBUG` and `HTMLBUTCHER_KEEPOLDSAVE` (both automatic in Debug configs),
`BUTCHER_USE_HELP` (MSW-only, set in `src/BConsts.h`), `NEED_CHOOSELANG_UI`, and
`HTMLBUTCHER_DEMO`.

`HTMLBUTCHER_DEMO` is far more invasive than it looks, and demo-guarded code must be kept
compiling:

- In `src/BList.h` it swaps the `ButcherList_1` / `ButcherList_2` aliases from the unlimited
  `ButcherList` to fixed-capacity containers holding one or two items that throw
  `ButcherException(_("List is full"))` on overflow — so masks and views silently become 1-item
  collections.
- `BUTCHERCONST_VIEW_MAXALTERNATE` drops from 8 to 1 (`src/BConsts.h`).
- `Open`, `Save` and `SaveAs` are **removed from `ButcherProject` entirely**, along with the
  corresponding menu items and handlers in the frame.

## Gotchas

- Model code throws freely (`ButcherException`, `wxFreeImageException`, `std::runtime_error`) and
  relies on the app to catch. The same four-way catch block is duplicated in `OnRun`,
  `OnExceptionInMainLoop`, `OnUnhandledException` and (GTK/Mac) `HandleEvent`.
- `resources/htmlbutcher_resources.cpp` is `wxrc`-generated and checked in — edit
  `resources/htmlbutcher.xrc` and regenerate; never hand-edit the generated file.
- The version string is duplicated across four files; see the release checklist in `BUILDING.md`.
- `origin/qt` holds an incomplete Qt port, 11 commits ahead of `master`. `cpp11` is already merged.
- Command line: `htmlbutcher [-h|--help] [-l|--license-file <file>] [project.hbp]`. The switch
  character is forced to `-` only, so that `/path` arguments still work.
- The CSS/HTML editor (`ButcherControl_FmtTextCtrl` in `src/BControls.h`) derives from wx's
  `wxStyledTextCtrl`. It used to optionally derive from the third-party `wxSTEditor`, with a plain
  `wxTextCtrl` fallback; both are gone, so syntax highlighting is now unconditional. `SetEditFormat`
  maps to Scintilla lexers.

## Repo conventions

Commit messages are a single line prefixed with `* ` — e.g. `* Fix layout asserts`,
`* Remove title from radio button menus because of wx bug`.

Line endings are pinned by `.gitattributes`: all text is stored LF. Shell scripts and the Debian
`control`/`postinst`/`postrm` files stay LF in the working tree too (`/bin/sh` and `dpkg-deb` break
on CRLF); `.bat`, `.iss` and `.rc` are CRLF; `.hbp` is binary. Do not "fix" line endings by hand.
