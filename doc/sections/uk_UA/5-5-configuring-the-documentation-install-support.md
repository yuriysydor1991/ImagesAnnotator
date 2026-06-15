## Вмикання підтримки встановлення документації

Наявна можливість підтримки встановлення документації за допомогою встановлення змінних `ENABLE_DOC_DOXYGEN` і `DOXYGEN_DO_INSTALL` у значення `ON` під час конфігурування прокету-шаблону за допомогою команди:

```
# inside the project build directory

cmake ../ -DENABLE_DOC_DOXYGEN=ON -DDOXYGEN_DO_INSTALL=ON
```

Змінна `DOXYGEN_OUT_HTML_NAME` сконфігурує назву для директорії HTML документації (передається до файлу `Doxyfile`).
