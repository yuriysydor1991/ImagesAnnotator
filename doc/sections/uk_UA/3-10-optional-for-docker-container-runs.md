## Необов'язкові пакети для запуску контейнера Docker

Для того щоб мати змогу будувати і запускати контейнери Docker разом з побудовою проекту шаблону його необхідно встановити за допомогою команди (ОС на базі GNU/Linux):

```
sudo apt install -y docker.io docker-buildx
```

Наступним кроком являється конфігурування сервісу Docker для прослуховування на локальному інтерфейсі для TCP підключень:

```
sudo systemctl edit docker 
```

Після відкриття конфігураційного файлу сервісу Docker необхідно вставити в його кінець наступні рядки:

```
[Service]
ExecStart=
ExecStart=/usr/bin/dockerd -H fd:// -H tcp://127.0.0.1:2375
ExecReload=/bin/kill -s HUP $MAINPID
```

Після чого зберегти файл і закрити. Тепер потрібно перезавантажити сервіси командами:

```
sudo systemctl daemon-reload
sudo systemctl restart docker.service
```

Для перевірки того чи сервіс Docker працює коректно можна спробувати локально запустити спеціальний контейнер `hello-world`:

```
DOCKER_HOST="tcp://127.0.0.1:2375" docker run hello-world
```

Можна додати рядок `export DOCKER_HOST="tcp://127.0.0.1:2375"` до файлу конфігурації Bash поточного користувача ОС `.bashrc` щоб уникнути вказування змінної середовища `DOCKER_HOST` у кожну команду запуску програми Docker:

```
echo 'export DOCKER_HOST="tcp://127.0.0.1:2375"' >> ~/.bashrc
```

Деталі у секції [Вмикання підтримки запуску Docker-контейнера](/doc/sections/uk_UA/5-12-enabling-the-docker-container-build-and-run.md).
