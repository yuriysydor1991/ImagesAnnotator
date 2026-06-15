## Optional for the snap packager

Install the `snapd` (if no available) and `snapcraft` packager application:

```
sudo apt install -y snapd

sudo snap install snapcraft --classic
```

Next install the [LXD](https://documentation.ubuntu.com/lxd/latest/) container manager:

```
sudo snap install lxd

# replace $USER with target user
sudo usermod -aG lxd $USER

# may require reboot
reboot

# initialize the LXD (without --auto will ask some questions)
lxd init --auto

# reboot OS
reboot
```
