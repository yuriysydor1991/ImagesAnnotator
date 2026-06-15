## Вмикання конвеєра Jenkins всередині Docker контейнера

Для того щоб увімкнути підтримку конвеєра [Jenkins](https://www.jenkins.io/) всередині спеціально виділеного Docker контейнера необхідно побудувати ціль `jenkins-pipeline-docker-run` за допомогою команд:

```
# в середині кореневої директорії проекту

# Конфігурування проекту для підтримки цілей Docker разом з Jenkins
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_DOCKER=ON -DENABLE_JENKINS_DOCKER_PIPELINE=ON

# Побудова і запуск контейнера для конвеєрів Jenkins
# або його перезапуск
cmake --build build --target jenkins-pipeline-docker-run
```

Після чого необхідно знайти і запам'ятати пароль до [Jenkins](https://www.jenkins.io/) Web-інтерфейсу у логах першого запуску Jenkins системи, який може виглядати наступним чином:

```
# ...

*************************************************************
*************************************************************
*************************************************************

Jenkins initial setup is required. An admin user has been created and a password generated.
Please use the following password to proceed to installation:

aefdc6a4a2214c298c821236c739165c

This may also be found at: /var/jenkins_home/secrets/initialAdminPassword

*************************************************************
*************************************************************
*************************************************************

# ...
```

Необхідно відвідати локальну адресу `http://127.0.0.1:8080` (порт може змінюватись) за допомогою використовуваного браузера щоб завершити початкову інсталяцію і конфігурацію [Jenkins](https://www.jenkins.io/) системи на Web-сторінках його GUI.

Після успішної інсталяції і конфігурації [Jenkins](https://www.jenkins.io/) необхідно створити відповідний конвеєр перевірки проекту всередині Web-панелі [Jenkins](https://www.jenkins.io/).

Усі CMake скрипти побудови контейнера для конвеєрів перевірки [Jenkins](https://www.jenkins.io/) розміщуються у файлі з відносним шляхом cmake/enablers/dockerers/template-project-docker-Jenkins-pipeline-runner-target.cmake. Відповідний базовий примірник скрипта конвеєра перевірки шаблонного проекту розміщений за відносним шляхом misc/Jenkinsfile котрий повинен бути вказаним для конфігурації конвеєра побудови у Web-панелі [Jenkins](https://www.jenkins.io/). Порт для Web доступу контейнера Docker до конфігурації конвеєрів перевірки [Jenkins](https://www.jenkins.io/) може бути змінено за допомогою встановлення нового значення для CMake-змінної `JENKINS_PIPELINES_PANEL_HTTP_PORT`.

Файл за відносним шляхом cmake/enablers/dockerers/template-project-docker-Jenkins-pipeline-runner-target.cmake містить рецепти побудови Docker контейнера для конвеєрів перевірки [Jenkins](https://www.jenkins.io/) що за замовчуванням перезапустить попередньо побудований контейнер для конвеєрів перевірки.
