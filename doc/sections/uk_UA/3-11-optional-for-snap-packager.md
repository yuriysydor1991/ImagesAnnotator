# Необов'язкові пакети для snap пакувальника

Для встановлення `snapd` (якщо ще не присутній в ОС) і інструмент для створення [snap](https://snapcraft.io/docs/) пакунків `snapcraft`:

```
sudo apt install -y snapd

sudo snap install snapcraft --classic
```

Наступним чином необхідно встановити менеджер контейнерів [LXD](https://documentation.ubuntu.com/lxd/latest/):

```
sudo snap install lxd

# опціонально можна замінити $USER на ідентифікатор цільового користувача
sudo usermod -aG lxd $USER

# може вимагати презевантаження ОС
reboot

# ініціалізація LXD (без --auto можна ініціювати з кастомними параметрами)
lxd init --auto

# презевантаження ОС
reboot
```
