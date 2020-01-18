# component-sdl2

[![Language](https://img.shields.io/badge/language-C++17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://mit-license.org/)

[![Dependencies](https://img.shields.io/badge/dependencies-SDL2-brightgreen.svg)](https://www.libsdl.org/)
[![Dependencies](https://img.shields.io/badge/dependencies-SDL2_ttf-brightgreen.svg)](https://www.libsdl.org/tmp/SDL_ttf/)
[![Dependencies](https://img.shields.io/badge/dependencies-SDL2_image-brightgreen.svg)](https://www.libsdl.org/tmp/SDL_image/)
[![Dependencies](https://img.shields.io/badge/dependencies-SDL2_gfx-brightgreen.svg)](http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)


[![Dependencies](https://img.shields.io/badge/dependency_if_not_MSVC-boost-brightgreen.svg)](https://www.boost.org/)

[![](https://img.shields.io/badge/Conventional%20Commits-1.0.0-yellow.svg)](https://www.conventionalcommits.org)


## Описание

`component-sdl2` это небольшая кроссплатформенная библиотека для создания интефейсов созданная на базе низкоуровневой графической библиотеки `SDL2`. Она поддерживает полную кастомизацию внешнего вида компонентов при помощи `css`, а так же простое создание кастомных компонентов любой сложности. 


## Пример интерфейса

![example](docs/1.png)

## Установка

### Установка boost (Для GCC)

Для поддержки работы с поиском шрифтов используется `std::filesystem`, которая в достаточной форме реализована в `Visual C++`, но не в `GCC 8.2`, поэтому для поддержки `filesystem` используется `boost::filesystem`!

```bash
# download boost (https://www.boost.org) and go to the folder 
# with boost and run the next command

./b2

# The compilation of boost is quite long, so be patient.
```

### Установка SDL2 & SDL2_image & SDL2_ttf & SDL_gfx

```bash
# On Linux
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-gfx-dev

# On Window
# Download from 
# 1. https://www.libsdl.org/
# 2. https://www.libsdl.org/tmp/SDL_ttf/
# 3. https://www.libsdl.org/tmp/SDL_image/
# 4. http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/

# note: SDL2_gfx must be manually assembled.

# and unzip folders to your convenience.

```

### Далее

```bash
git clone https://github.com/i582/component-sdl2.git
cd component-sdl2
git clone https://github.com/aminosbh/sdl2-cmake-modules
mv sdl2-cmake-modules cmake
cd test
git clone https://github.com/google/googletest

# for Window
# In CMakeLists.txt, write the paths to the folders.
```



## Ссылки 
[Документация](https://i58215.gitbook.io/component-sdl2/)

## Лицензия

MIT
