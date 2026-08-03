# ImagesAnnotator-DataExporters integration: the symbol clash analysis

This document records the one defect that blocked the switch of this project
over to the [ImagesAnnotator-DataExporters](https://github.com/yuriysydor1991/ImagesAnnotator-DataExporters.git)
library, how it was tracked down and where it had to be fixed. It is kept
because the defect is invisible in the sources of either project: both compile
and link cleanly, and the two libraries only destroy each other at the run time.

## Symptom

With the exporters library linked next to the
[ImagesAnnotator-DataDrivers](https://github.com/yuriysydor1991/ImagesAnnotator-DataDrivers.git)
one, every component test of the annotator controller crashed on construction,
before reaching a single assertion:

```
[ RUN      ] CTEST_ImagesDirLoader.no_context_init_failure
... ERR ... LibFactory.cpp:77 : Unknown export format requested
Segmentation fault (core dumped)
```

Two things are wrong in those two lines. The test constructs an
`AnnotatorController`, which opens an annotations database - it asks nothing of
the exporters. And `LibFactory.cpp:77` is the *exporters* library reporting an
unknown export format, in the middle of a *data drivers* call.

The 17 crashing tests were `CTEST_AnnotationsDirDB` and `CTEST_ImagesDirLoader`
in full. Every unit test passed, because no unit test links both libraries.

## What the crash actually was

```
#0  std::_Sp_counted_base<...>::_M_release ()                        (executable)
#4  ImagesAnnotatorDataExporters011::LibraryContext::~LibraryContext () (exporters .so)
#5  std::_Destroy<...DataExporters011::LibraryContext> ()               (exporters .so)
#6  std::_Sp_counted_ptr_inplace<...DataExporters011::LibraryContext, ...>::_M_dispose ()
#9  std::__shared_ptr<...DataDrivers011::LibraryContext, ...>::~__shared_ptr ()
#11 iannotator::AnnotatorController::create_project_data_instance ()
```

A `shared_ptr` holding a **data drivers** `LibraryContext` (frame 9) releases a
control block that disposes of an **exporters** `LibraryContext` (frames 6 to 4).
The pointer and its control block describe two different classes of two
different libraries.

## Root cause

Both libraries are generated from the same
[cpp-app-template](https://github.com/yuriysydor1991/cpp-app-template) `lib`
branch, so each carried its own `lib0impl::LibFactory` and `lib0impl::LibMain` -
same fully qualified names, unrelated class layouts.

The exporters library was already built with `CXX_VISIBILITY_PRESET hidden`
precisely to keep those names private. That is not enough. `nm -D` on the two
installed shared objects showed exactly two names defined by both:

```
std::__shared_count<(__gnu_cxx::_Lock_policy)2>::__shared_count<lib0impl::LibFactory, std::allocator<void>>(...)
std::__shared_count<(__gnu_cxx::_Lock_policy)2>::__shared_count<lib0impl::LibMain,    std::allocator<void>>(...)
```

These are the `std::make_shared` entry points. They are weak, globally visible
template instantiations that name the class in their mangled name, allocate the
control block, construct the class in place and stamp the control block with the
vtable that knows how to destroy it. The hidden visibility default does not lower
them: GCC lowers a *class template* instantiation to the visibility of its
arguments, but not a *member function template* of a `std::` class template,
which is what this constructor is. A minimal reproducer confirms it - the symbol
below stays `W` (weak, exported) even with `-fvisibility=hidden` and an explicit
hidden attribute on the class:

```cpp
namespace lib0impl {
class __attribute__((visibility("hidden"))) LibFactory { ... };
}
std::shared_ptr<lib0impl::LibFactory> make() { return std::make_shared<lib0impl::LibFactory>(); }
```

`LD_DEBUG=bindings` on the crashing test showed the one and only cross-library
binding, and it is that symbol:

```
binding file .../libImagesAnnotatorDataDrivers-0.11.so.0 [0]
     to .../libImagesAnnotatorDataExporters-0.11.so.0 [0]:
  normal symbol `std::__shared_count<...>::__shared_count<lib0impl::LibFactory, ...>'
```

The exporters library precedes the data drivers one in the link line (the
exporters target carries the drivers as a `PUBLIC` dependency, so CMake orders
the dependency after its dependent), so its definition won for both. Every
`std::make_shared<lib0impl::LibFactory>()` inside the data drivers library
therefore built an *exporters* `LibFactory`; the drivers then called their own
virtual method through the exporters vtable - which landed in
`create_exporter()`, hence "Unknown export format requested" - and the control
block later disposed of the object as an exporters `LibraryContext`.

## Why it could not be fixed in this project

The two shared objects are simply incompatible when loaded together. Reversing
the link order only swaps which library gets corrupted: the exporters would then
build a drivers `LibFactory`. Nothing in the consumer's build can make one
definition serve both, so the fix belongs to a library.

## The fix

In the exporters library, and pushed there rather than here: its implementation
namespace is `iade0impl` instead of the template `lib0impl`, which makes the
mangled names distinct and the collision impossible rather than merely
improbable. The hidden visibility and the `IADE_API` marked public interface
stay as they are - they remove every non-template collision, and now nothing is
left for them to miss.

A linker version script was the alternative, and was rejected as the less
portable one: the project also targets MS Windows, where `__declspec` and not a
GNU version script is what exports.

## Verification

- the exported symbol intersection of the two installed shared objects no longer
  contains a single project owned name;
- the exporters library keeps passing its own 61 unit and component tests;
- this project passes 197 of 197 tests with the GTKmm3 and the GTKmm4 toolkits
  and 169 of 169 with wxWidgets, including the new `CTEST_Exporters` component
  test that drives an export end to end through the library facade.

## Unrelated failures seen during the verification

The Qt6 build fails 8 tests - the seven `CTEST_Qt6WindowLoader` cases, which all
pass and then segfault during the process tear-down, and
`CTEST_Qt6CentralWorkingCanvas.rejects_a_null_pixmap`, which aborts on a
`CentralWorkingCanvas.cpp` assertion. Both reproduce unchanged on a worktree of
the commit before this integration, so they predate it and are not caused by
either library.
