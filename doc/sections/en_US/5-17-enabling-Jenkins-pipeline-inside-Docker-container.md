
## Enabling Jenkins pipeline inside Docker container

In order to enable the [Jenkins](https://www.jenkins.io/) pipeline execution inside the Docker container build and start the appropriate container with image by building the `jenkins-pipeline-docker-run` target:

```
# Inside the source root directory

# Configuring the project to support Docker with Jenkins targets
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_DOCKER=ON -DENABLE_JENKINS_DOCKER_PIPELINE=ON

# Building and starting Jenkins' pipeline Docker container
# or restarting it
cmake --build build --target jenkins-pipeline-docker-run
```

Locate and save the new generated password for the [Jenkins](https://www.jenkins.io/) Web GUI in the log output that may look like next:

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

Go to `http://127.0.0.1:8080` (port may change) address with the help of prefered Web browser. Process to finilize the installation of the [Jenkins](https://www.jenkins.io/) system with the help of it's GUI.

After successfull installation and configuration of the [Jenkins](https://www.jenkins.io/) create appropriate pipeline for the project inside the [Jenkins](https://www.jenkins.io/) Web GUI panel.

All the [Jenkins](https://www.jenkins.io/) pipeline CMake build scripts are located at [cmake/enablers/dockerers/template-project-docker-Jenkins-pipeline-runner-target.cmake](/cmake/enablers/dockerers/template-project-docker-Jenkins-pipeline-runner-target.cmake) file. The pipeline script located at [misc/Jenkinsfile](misc/Jenkinsfile) which should be specified in the pipeline source script at the [Jenkins](https://www.jenkins.io/) Web GUI pipelines configuration page. The [Jenkins](https://www.jenkins.io/) Web GUI Docker container port may be changed by specifying a new value for the `JENKINS_PIPELINES_PANEL_HTTP_PORT` CMake variable.

The [cmake/enablers/dockerers/template-project-docker-Jenkins-pipeline-runner-target.cmake](/cmake/enablers/dockerers/template-project-docker-Jenkins-pipeline-runner-target.cmake) CMake script for the template project [Jenkins](https://www.jenkins.io/) pipeline Docker container builder by default will restart previously built contianer if target build launched repeatedly.
