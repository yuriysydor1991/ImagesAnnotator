## Вмикання інтеграції бібліотеки nlohmann JSON

Для того щоб увімкнути підтримку бібліотеки [nlohmann JSON](https://github.com/nlohmann/json.git) для проекту, використовуючи встановлену версію або через завантаження з мережі, необхідно встановити значення `ON` для CMake змінної `ENABLE_NLOHMANN_JSON`:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_NLOHMANN_JSON=ON
```
