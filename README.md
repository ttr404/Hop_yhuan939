# HOP.cheap


## Live Demo

Main stream:
[https://hop.cheap](https://hop.cheap)

Dev servers:
[Task automation file]: [.vscode/launch.json](.vscode/launch.json)
* dev.hop.cheap
* dev-f.hop.cheap
* dev-o.hop.cheap
* dev-t.hop.cheap
* dev-x.hop.cheap

## Dependencies

* gcc
* boost
* libcurl
* cmake/v3.11 or higher
* emscripten/v2.0 or higher
* nlohmann_json/v3.0 or higher
* libmysqlcppconn-dev/v1.1 or higher

## How to Install
### MacOS
**Install Homebrew (if not already installed)**
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

**Install dependencies**
brew install gcc boost libcurl cmake emscripten nlohmann_json mysql-connector-c++

### Linux
#### apt based
**Update package lists**
sudo apt-get update

**Install dependencies**
sudo apt-get install -y gcc libboost-all-dev libcurl4-openssl-dev cmake emscripten nlohmann-json-dev libmysqlcppconn-dev


### yum based
**Update package lists**
sudo apt-get update

**Install dependencies**
sudo yum install -y gcc boost-devel libcurl-devel cmake emscripten nlohmann-json-devel mysql-connector-c++

### arch based
**Update package lists**
sudo pacman -Syu

**Install dependencies**
sudo pacman -Syu gcc boost curl cmake emscripten jsoncpp mariadb-connector-c++

## How to RUN

> Press F5 to run the project in VSCode

The server will be available at [http://localhost:8081](http://localhost:8081)

> Or go to our live demo

[https://hop.cheap](https://hop.cheap)

### Manually run

```
cd build
cmake ..
make
./Crow
```

## UI

![UI](https://res.cloudinary.com/alt/image/upload/f_auto,q_auto/v1696910668/assets/bfbba6foxurlge1aska0.png)

## Web Stack

* Caddy/v2.7.5 (web server)

## Database

* MariaDB/10.5.22
* phpMyAdmin/5.0.4

Dashboard (phpMyAdmin):
[https://db.hop.cheap](https://db.hop.cheap)