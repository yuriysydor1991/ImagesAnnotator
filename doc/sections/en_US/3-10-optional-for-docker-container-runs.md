## Optional for the Docker container runs

In order to be able to start the Docker containers with the built template application install it with the next command:

```
sudo apt install -y docker.io docker-buildx
```

Next, configure the docker service to start to listen the local machine loopback TCP interface with it's configuration file edit with command:

```
sudo systemctl edit docker
```

Insert the next lines into the end of the configure file for opened docker service:

```
[Service]
ExecStart=
ExecStart=/usr/bin/dockerd -H fd:// -H tcp://127.0.0.1:2375
ExecReload=/bin/kill -s HUP $MAINPID
```

Save and close the configuration file. Now reload the systemctl and the Docker by itself:

```
sudo systemctl daemon-reload
sudo systemctl restart docker.service
```

To check if Docker works execute next command to run the simple `hello-world` container locally: 

```
DOCKER_HOST="tcp://127.0.0.1:2375" docker run hello-world
```

You may add the `export DOCKER_HOST="tcp://127.0.0.1:2375"` to your `.bashrc` file to avoid the `DOCKER_HOST` environment variable inclusion into every docker command run by executing the command:

```
echo 'export DOCKER_HOST="tcp://127.0.0.1:2375"' >> ~/.bashrc
```

Details at the section [Enabling the Docker container build and run](/doc/sections/en_US/5-12-enabling-the-docker-container-build-and-run.md).
