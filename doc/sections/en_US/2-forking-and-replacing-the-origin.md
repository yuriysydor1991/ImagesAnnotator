# Forking and replacing the origin

In order to copy all the template project into your new clean repository (**clean repository with no** `README.md`, `.gitignore`, `LICENCE` or any other files) clone the current cpp-app-template project into your local directory and perform origin replacement command from inside the project root directory:

```
# REPLACE next URL with yours
git remote set-url origin https://github.com/yuriysydor1991/cpp-app-template.git
```

Replace the `https://github.com/yuriysydor1991/cpp-app-template.git` URL with one of your newly created project repository URL.

To verify successful repository remote URL (where all the changes will be pushed) replacement execute next command:

```
git remote -v
```

Which should show your new project repository.

Finally push your repository to a new location by executing next command:

```
git push
```

Forking may be performed in git Web interface panel.
