## Вмикання інтеграції zlib

Для того щоб увімкнути бібліотеку стиснення [zlib](https://www.zlib.net/) (deflate, gzip, zip) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_ZLIB`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_ZLIB=ON
```

Дивись модуль `cmake/enablers/compression/template-project-zlib-enabler.cmake` щодо цілей (`ZLIB::ZLIB` для системної версії, `zlibstatic`/`zlib` для побудови через FetchContent), які слід прилінкувати до твоїх цільових об'єктів.

### Стиснення потрібних даних (приклади для копіювання)

Після `ENABLE_ZLIB=ON` та прилінкування цілі zlib (наприклад `target_link_libraries(${PROJECT_BINARY_NAME} ZLIB::ZLIB)`) підключи `<zlib.h>` і скопіюй будь-який зі сніпетів нижче.

Найшвидший спосіб стиснути потрібний буфер — за один виклик, у пам'яті, з використанням обгортки zlib/deflate (RFC 1950):

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

// Для розпакування потрібен початковий (нестиснений) розмір, тож зберігай його
// поряд зі стисненими даними (наприклад, у невеликому заголовку).
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

### Створення різних типів стиснених архівів

Той самий модуль створює кілька форматів-контейнерів; обгортка обирається аргументом `windowBits` функції `deflateInit2()`:

| Формат | `windowBits` | Примітки |
| --- | --- | --- |
| zlib (RFC 1950) | `15` | типова обгортка `compress()` / `uncompress()` |
| сирий deflate (RFC 1951) | `-15` | потік без заголовка, наприклад для окремих елементів `.zip` |
| gzip / `.gz` (RFC 1952) | `15 + 16` (`31`) | формат файлу `.gz` |

Потокове стиснення, що обирає вихідний формат (сирий deflate / zlib / gzip):

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

Готовий до запису файл `.gz` через високорівневий gzip API (без маніпуляцій з `windowBits`):

```cpp
#include <zlib.h>

#include <stdexcept>
#include <string>
#include <vector>

void write_gz_file(const std::string& path,
                   const std::vector<unsigned char>& data)
{
  gzFile f = gzopen(path.c_str(), "wb9");  // запис, бінарний, рівень стиснення 9
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

**Архіви `.zip`.** Ядро zlib надає лише *стиснення* (сирий deflate), що використовується всередині ZIP; сам *контейнер* ZIP створюється за допомогою **minizip**, який постачається разом із zlib у `contrib/minizip`. Типовий потік запису: `zipOpen()` → `zipOpenNewFileInZip()` → `zipWriteInFileInZip()` → `zipCloseFileInZip()` → `zipClose()` (а для читання — `unzOpen()` / `unzGoToFirstFile()` / `unzReadCurrentFile()`). Увімкни/знайди minizip у свого постачальника zlib і прилінкуй його поряд із `ZLIB::ZLIB`.
