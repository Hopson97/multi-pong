# multi-pong

WIP Multiplayer pong using SFML and UDP.

## Log

### 02/10/19

Got 4 players able to run around a map, with the "ball" also there. All positions are calculated on the server and then broadcasted to the clients.

![Players](https://i.imgur.com/i1iTugw.gif)

## Building and Running

### Libraries

SFML is required.

These can be installed from your project manager. For example, on Debian/ Ubuntu:

```sh
sudo apt install libsfml-dev
```

If this is not possible (eg windows), you can install these manually from their respective websites:

https://www.sfml-dev.org/download.php

### Linux

To build, at the root of the project:

```sh
sh scripts/build.sh
```

To run, at the root of the project:

```sh
sh scripts/run.sh server
sh scripts/run.sh client
```

To build and run in release mode, simply add the `release` suffix:

```sh
sh scripts/build.sh release
sh scripts/run.sh release [client or server here]
```

You can also create a deployable build (that can be sent) by doing:

```sh
sh scripts/deploy.sh
```
