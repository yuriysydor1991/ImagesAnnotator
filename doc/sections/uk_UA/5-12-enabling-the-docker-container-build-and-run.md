## Вмикання підтримки запуску Docker-контейнера

Щоб увімкнути і запустити Docker контейнер разом з цільовим бінарним виконуваним файлом поточного проекту-шаблону необхідно встановити у значення `ON` змінну `ENABLE_DOCKER` за допомогою команди:

```
# в середині кореневої директорії проекту

cmake -S . -B build -DENABLE_DOCKER=ON
```

Після чого можна запустити побудову контейнера Docker і слідуючий запуск цільового виконуваного файлу проекту за допомогою комнади (CMake-ціль `docker-single-run`):

```
# в середині кореневої директорії проекту

cmake --build build --target docker-single-run 
```

Для деталей CMake-цілі побудови `docker-single-run` слід звернутись до файлу `cmake/enablers/dockerers/template-project-docker-single-runner-target.cmake`.
