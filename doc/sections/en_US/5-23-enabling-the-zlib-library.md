## Enabling the zlib library

In order to enable the [zlib](https://www.zlib.net/) compression library (deflate, gzip, zip) for the project by using the installed system one or by fetching it through the available connection set an `ON` value to the `ENABLE_ZLIB` CMake variable.

```
# Inside the source root directory

cmake -S . -B build -DENABLE_ZLIB=ON
```

See the `cmake/enablers/compression/template-project-zlib-enabler.cmake` module for the targets (`ZLIB::ZLIB` for a system install, `zlibstatic`/`zlib` for the FetchContent build) to link to your targets of interest.

### Compressing data of interest (copy-paste examples)

After `ENABLE_ZLIB=ON` and linking a zlib target (e.g. `target_link_libraries(${PROJECT_BINARY_NAME} ZLIB::ZLIB)`), include `<zlib.h>` and copy-paste any of the snippets below.

The quickest way to compress a buffer of interest — one-shot, in-memory, using the zlib/deflate wrapper (RFC 1950):

```cpp
#include <zlib.h>

#include <cstddef>
#include <stdexcept>
#include <vector>

std::vector<unsigned char> zlib_compress(
    const std::vector<unsigned char>& input, int level = Z_BEST_COMPRESSION)
{
  uLongf bound = compressBound(static_cast<uLong>(input.size()));
  std::vector<unsigned char> out(bound);
  if (compress2(out.data(), &bound, input.data(),
                static_cast<uLong>(input.size()), level) != Z_OK) {
    throw std::runtime_error("zlib compress2 failed");
  }
  out.resize(bound);
  return out;
}

// Decompression needs the original (uncompressed) size, so keep it next to the
// compressed blob (e.g. as a small header).
std::vector<unsigned char> zlib_uncompress(
    const std::vector<unsigned char>& input, std::size_t original_size)
{
  std::vector<unsigned char> out(original_size);
  uLongf out_size = static_cast<uLongf>(original_size);
  if (uncompress(out.data(), &out_size, input.data(),
                 static_cast<uLong>(input.size())) != Z_OK) {
    throw std::runtime_error("zlib uncompress failed");
  }
  out.resize(out_size);
  return out;
}
```

### Creating different compressed archive types

The same module produces several container formats; the wrapper is selected by the `windowBits` argument of `deflateInit2()`:

| Format | `windowBits` | Notes |
| --- | --- | --- |
| zlib (RFC 1950) | `15` | the default `compress()` / `uncompress()` wrapper |
| raw deflate (RFC 1951) | `-15` | headerless stream, e.g. for individual `.zip` members |
| gzip / `.gz` (RFC 1952) | `15 + 16` (`31`) | the `.gz` file format |

Streaming compression that chooses the output format (raw deflate / zlib / gzip):

```cpp
#include <zlib.h>

#include <stdexcept>
#include <vector>

// windowBits: 15 = zlib, -15 = raw deflate, 31 = gzip
std::vector<unsigned char> deflate_stream(
    const std::vector<unsigned char>& input, int windowBits,
    int level = Z_BEST_COMPRESSION)
{
  z_stream zs{};
  if (deflateInit2(&zs, level, Z_DEFLATED, windowBits, 8 /* memLevel */,
                   Z_DEFAULT_STRATEGY) != Z_OK) {
    throw std::runtime_error("deflateInit2 failed");
  }
  zs.next_in = const_cast<Bytef*>(input.data());
  zs.avail_in = static_cast<uInt>(input.size());

  std::vector<unsigned char> out;
  unsigned char buf[16384];
  int rc = Z_OK;
  do {
    zs.next_out = buf;
    zs.avail_out = sizeof(buf);
    rc = deflate(&zs, Z_FINISH);
    out.insert(out.end(), buf, buf + (sizeof(buf) - zs.avail_out));
  } while (rc == Z_OK);
  deflateEnd(&zs);

  if (rc != Z_STREAM_END) {
    throw std::runtime_error("deflate failed");
  }
  return out;
}
```

A ready-to-write `.gz` file through the high-level gzip API (no `windowBits` juggling required):

```cpp
#include <zlib.h>

#include <stdexcept>
#include <string>
#include <vector>

void write_gz_file(const std::string& path,
                   const std::vector<unsigned char>& data)
{
  gzFile f = gzopen(path.c_str(), "wb9");  // write, binary, compression level 9
  if (f == nullptr) {
    throw std::runtime_error("gzopen (write) failed");
  }
  if (!data.empty() &&
      gzwrite(f, data.data(), static_cast<unsigned>(data.size())) == 0) {
    gzclose(f);
    throw std::runtime_error("gzwrite failed");
  }
  gzclose(f);
}

std::vector<unsigned char> read_gz_file(const std::string& path)
{
  gzFile f = gzopen(path.c_str(), "rb");
  if (f == nullptr) {
    throw std::runtime_error("gzopen (read) failed");
  }
  std::vector<unsigned char> out;
  unsigned char buf[16384];
  int n = 0;
  while ((n = gzread(f, buf, sizeof(buf))) > 0) {
    out.insert(out.end(), buf, buf + n);
  }
  gzclose(f);
  return out;
}
```

**`.zip` archives.** zlib's core only provides the *compression* (raw deflate) used inside a ZIP; the ZIP *container* itself is produced with **minizip**, which ships together with zlib under `contrib/minizip`. The typical write flow is `zipOpen()` → `zipOpenNewFileInZip()` → `zipWriteInFileInZip()` → `zipCloseFileInZip()` → `zipClose()` (and `unzOpen()` / `unzGoToFirstFile()` / `unzReadCurrentFile()` to read it back). Enable/locate minizip from your zlib provider and link it next to `ZLIB::ZLIB`.
