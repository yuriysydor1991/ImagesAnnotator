### Запобігання використання GTest з ОС

Для вимкнення використання ресурсів наявного фреймворку GTest необхідно встановити відповідне значення для CMake-змінної `GTEST_TRY_SYSTEM_PROBE` за допомогою команди конфігурування проекту (для GNU/Linux):

```
# з кореневої директорії проекту-шаблону

mkdir -vp build && cd build && cmake ../ -DENABLE_UNIT_TESTS=ON -DGTEST_TRY_SYSTEM_PROBE=OFF && cmake --build . --target all
```

Під час виконання команди система побудови проекту спробує встановити GTest тільки у межах поточного проекту через мережу Інтернет з версією вказаною у файлі `cmake/enablers/template-project-GTest-enabler.cmake`.
