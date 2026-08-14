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

### Скрипти запуску

Директорія [scripts/docker](/scripts/docker) містить скорочення, які виконують обидві наведені вище команди, отож увесь запуск виконується однією командою:

| Скрипт | Запускає CMake-ціль |
| --- | --- |
| [single-run.sh](/scripts/docker/single-run.sh) | `docker-single-run` |
| [single-build.sh](/scripts/docker/single-build.sh) | `docker-single-build` |

```
# в середині кореневої директорії проекту

scripts/docker/single-run.sh
```

Кожен з них конфігурує директорію `build/release` з параметром `ENABLE_DOCKER` у значенні `ON` і потім будує потрібну ціль, отож жодної власної команди `docker` не виконується. CMake-ціль `docker-single-run` залежить від цілі `docker-single-build`, а отже скрипт [single-run.sh](/scripts/docker/single-run.sh) будує і образ теж, а скрипт [single-build.sh](/scripts/docker/single-build.sh) потрібен лише для побудови образу без запуску контейнера.

Скрипти приймають ті ж самі параметри, що і [Швидкі скрипти побудови](/doc/sections/uk_UA/5-38-quick-build-scripts.md), оскільки саме їх вони і викликають:

```
# в середині кореневої директорії проекту

# повторне використання вже сконфігурованої директорії build/release
scripts/docker/single-run.sh --no-reconfigure

# додатковий параметр CMake для кроку конфігурації
scripts/docker/single-run.sh -DDOCKERFILE_SINGLE_RUN_BASE_IMAGE_VERSION=22.04
```

Слід пам'ятати, що параметр `ENABLE_DOCKER` робить наявність виконуваного файлу `docker` у системі обов'язковою для кроку конфігурації, отож на машині без встановленого Docker скрипти завершать крок конфігурації помилкою (див. секцію вимог [Необов'язкові пакети для запуску контейнера Docker](/doc/sections/uk_UA/3-10-optional-for-docker-container-runs.md)).
