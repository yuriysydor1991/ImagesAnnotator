## Необов'язковий програми для генерації пакету flatpak

Для того щоб мати змогу генерувати пакет flatpak з ресурсів проекту необхідно встановити інструменти `flatpak` і `flatpak-builder`:

```
sudo apt install -y flatpak flatpak-builder
```

Також знадобляться середовище виконання та SDK, які називає маніфест flatpak обраного графічного інструментарію. Ціль `flatpak` сама встановлює їх з flathub у встановлення користувача, але їх можна встановити й заздалегідь, для типового маніфесту GTKmm3 - командою подібною на наступну:

```
flatpak remote-add --user --if-not-exists flathub https://dl.flathub.org/repo/flathub.flatpakrepo
flatpak install --user flathub org.gnome.Platform//48 org.gnome.Sdk//48
```

Маніфести GTKmm4 та wxWidgets натомість вимагають пари `org.gnome.Platform//49 org.gnome.Sdk//49`, маніфест Qt6 - пари `org.kde.Platform//6.10 org.kde.Sdk//6.10`, а маніфест збірки лише конвертера - пари `org.freedesktop.Platform//24.08 org.freedesktop.Sdk//24.08`. У документації до flatpak можна визначити як переглянути усі доступні варіанти.

Деталі у секції [Вмикання підтримки генерації flatpak пакету](/doc/sections/uk_UA/5-11-enabling-the-flatpak-package-generation-support.md).
