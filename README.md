
# Component-sdl2

[![Language](https://img.shields.io/badge/language-C++17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://mit-license.org/)

[![Dependencies](https://img.shields.io/badge/dependencies-SDL2-brightgreen.svg)](https://www.Kitsdl.org/)
[![Dependencies](https://img.shields.io/badge/dependencies-SDL2_ttf-brightgreen.svg)](https://www.Kitsdl.org/tmp/SDL_ttf/)
[![Dependencies](https://img.shields.io/badge/dependencies-SDL2_image-brightgreen.svg)](https://www.Kitsdl.org/tmp/SDL_image/)
[![Dependencies](https://img.shields.io/badge/dependencies-SDL2_gfx-brightgreen.svg)](http://www.ferzkopp.net/wordpress/2016/01/02/sdl_gfx-sdl2_gfx/)

## Описание

`component-sdl2` это небольшая кроссплатформенная библиотека для создания интефейсов созданная на базе низкоуровневой графической библиотеки `SDL2`. Она поддерживает полную кастомизацию внешнего вида компонентов при помощи `css`, а так же простое создание кастомных компонентов любой сложности. 


## Пример интерфейса

![example](docs/1.png)

# Использование библиотеки

## Начало работы

Для использования библиотеки необходимо подключить заголовочный файл `kit.h`.

```cpp
#include "kit.h"
```

Вся библиотека расположена в пространстве имен `Kit`, чтобы каждый раз не писать `Kit::*` в дальнейшем будет подразумеваться следующая строка

```cpp
using namespace Kit;
```

После подключения библиотеки, доступ к объекту библиотеки осуществляется через короткое имя `$`.

Для запуска библиотеки необходимо вызвать у `$` метод `run`, передав в оператор `return`.

```cpp
#include "kit.h"
using namespace Kit;

int main(int argc, char** argv)
{
    return $.run();
}
```

## Базовый функционал

Метод (здесь и далее будет употреблен термин "метод", как более короткий и понятный):

```cpp
Window* addWindow(Window* window);
```

добавляет новое окно в текущее приложение.

Класс `Window` является базовым классом для любых окон приложения. Имеет единственный конструктор:

```cpp
// SimpleRect is struct { x, y, w, h }
Window(string title, SimpleRect size);
```

где первый параметр это заголовок окна, а второй его размеры и положение.

Рассмотрим пример добавление окна:

```cpp 
#include "kit.h"
using namespace Kit;

int main(int argc, char** argv)
{
    $.addWindow(new Window("new window", { 100, 100, 200, 200 }));
    return $.run();
}
```

Данная программа выведет пустое окно размерами 200 на 200 пикселей с координатами 100, 100. Базовый класс окна не имеет никаких компонентов и нужен исключительно для наследования. 

## Создание собственных классов окна на базе класса `Window`

Создадим папку `MyWindow` рядом с папкой `kit`. И создадим заголовочный файл `MyWindow.h`.

Для наследования необходимо подключить заголовочный файл базового класса окна:

```cpp
#include "../kit/window/window.h"
```

Далее, создаем класс `MyWindow` и наследуем его от `Window`, перегружаем конструктор и добавляем один метод `setup`, который вызываем в конструкторе. Пока класс небольшой, напишем реализацию в заголовочном файле.

```cpp
#pragma once

#include "../window/window.h"
using namespace Kit;

class MyWindow : public Window
{
public:
    MyWindow(string title, SimpleRect size)
        : Window(title, size) 
    {
        setup();
    };

public:
    void setup()
    {

    }

};
```

Подключим заголовочный файл класса в `main.cpp` и создадим экземпляр.

```cpp
#include "kit.h"

#include "MyWindow/MyWindow.h"
using namespace Kit;

int main(int argc, char** argv)
{
    $.addWindow(new MyWindow("new window", { 100, 100, 200, 200 }));
    return $.run();
}
```

Пока что, если запустить программу, ничего не поменялось, окно все еще пусто. Перейдем к добавлению компонентов в интерфейс.

## Добавление компонентов в интерфейс

Любые компоненты в окне строятся на базе класса `Component`. Возможностей одного объекта класса вполне хватит для создания простейших элементов интерфейса, таких как, *кнопка* или *надпись*, но для создания более комплексных компонентов таких как, например, *флажок с надписью* уже нужно использовать несколько экземпляров класса `Component`, специально для таких случаев создаются собственные компоненты на базе `Component`, но об это чуть позже.

Для того, чтобы добавить новый компонент в окно используется метод `add` у класса окна.

Метод `add` имеет несколько прототипов:

```cpp
Component& add(const string& id, const string& classes, const vector<Component*>& childrens = {});
Component& add(const string& classes = "", const vector<Component*>& childrens = {});
Component& add(Component* component);
```

Добавим один компонент в только что созданное окно `MyWindow`. Ранее в классе `MyWindow` был создан метод `setup`, в нем и будет происходить добавление новых компонентов в окно.
 
Для краткости будем рассматривать только сам метод:

```cpp
void setup()
{
    add();
}
```

Это самый простой путь добавления компонента, но этот компонент нельзя не получить не стилизовать.

```cpp
void setup()
{
    add("#test", ".test");
}
```

А вот этот уже можно. Рассмотрим данную функцию.

Первым параметром функция принимает строку-идентификатор компонента, по которому его в дальнейшем можно будет найти в окне. 

**Важно!** В окне не может быть двух компонентов с одинаковым идентификатором. 

Вторым параметром идет строка с набором классовых идентификаторов через пробел. Данные классовые идентификаторы могут повторяться у разных элементов. Они используются для стилизации компонентов. 

Еще одна возможность функции `add`, это создание сложной иерархии компонентов.

```cpp
void setup()
{
    add("#test", ".test", { // здесь можно создавать потомков
        create("#inner", ".inner"),
        create("#inner-1", ".inner"),
    });
}
```

Здесь мы создали компонент `#test` с двумя потомками. Функция `create` полностью совпадает с функцией `add` только используется для того, чтобы только создавать (но не добавлять) компоненты и используется она в большинстве случаев именно для добавления потомков.

Вложенность неограничена
```cpp
void setup()
{
    add("#test", ".test", {
        create("#inner", ".inner", {
            create("#inner-inner", ".inner-inner"),
            create("#inner-inner-1", ".inner-inner", {
                create("#inner-inner-inner", ".inner-inner-inner")
            })
        }),
        create("#inner-1", ".inner"),
    });
}
```



### Стилизация компонентов

Для стилизации компонентов используется `css`. 

Чтобы подключить стили к окну используется метод `include`:

```cpp
void include(string path);
```

Очень важно, путь к стилям должен быть относительным к исполняемому файлу!

Для теста добавим компонент:

```cpp
add("#button", ".button");
```

Создадим папку `css` в папке `MyWindow` и создадим в ней файл `style.css`.

Подключим данный файл в наше окно.

```cpp
void setup()
{
    include("./MyWindow/css/style.css");

    add("#button", ".button");
}
```

Теперь мы можем писать стили для нашего окна. 

В первую очередь рассмотрим как задавать размеры и положение компонента.

Для этого используются следующие свойства:

|  Attribute  | Value type  |  Description  |
|-------------|-------------|---------------|
|`width` | The width of the component | `[number + px/number + %/(expr -> calc(X% +(-) Ypx))]`|
|`height` | The height of the component | `same` |
|`top` | Shift the component from the top edge (Y) | `same` |
|`left` | Shift the component from the left edge (X) | `same` |

Как можно заметить по последней строке, задавать размеры можно свободно, как в пикселах, так и в процентах. Также поддерживаются выражения в стили X +(-) Y, где X, Y могут быть, как пикселами, так и процентами.

Зададим размеры и положение нашему компоненту и заодно цвет и обводку:

```css
.button
{
    width: 70px;
    height: 24px;

    top: 100px;
    left: 100px;

    background-color: #263238;
    border-color: #0F1518;
}
```

В итоге мы получим стилизованный прямоугольник заданных размеров.

Список всех поддерживаемых стилей приведен в конце.


### Доступ к компонентам по их id

Для доступа к добавленным компонентам в окне используется функция `getElementById`, где единственным параметром передается идентификатор компонента:

```cpp
Component* getElementById(string id);
```


### Добавление текста в компонент

Класс `Component` имеет метод `setText` для установки текста. Добавим текст недавно созданному компоненту:

```cpp
void setup()
{
    include("css/style.css");

    add("#button", ".button");

    Window::getElementById("#button")->setText("ok");
}
```

Для стилизации текста можно использовать следующие свойства:

|  Attribute  | Value type  |  Description  |
|-------------|-------------|---------------|
|`color` | Color of the text in the component. |`[hex/rgb/rgba]`|
|`font-size` | Font size in the component | `[number + px]`|
|`font-family` | Font family in the component |`[string]`|
|`font-style` | Font style in the component |`[normal/italic]`|
|`font-weight` | Weight of the font in the component |`[100-900]`|
|`line-height` | Line height of the font in the component |`[double]`|
|`text-align`  | Alignment of the text horizontally in the component |`[left/center/right]`|
|`vertical-align`  | Alignment of the text vertically in the component. |`[top/center/bottom]`|
|`margin-[top/bottom/left/right]` | Shift the text in the directions in the component. |`[number + px]`|

Например, добавим нашему компоненту следующие стили, чтобы текст не сливался с фоном и был центрирован:

```css
.button
{
    width: 70px;
    height: 24px;

    top: 100px;
    left: 100px;

    background-color: #263238;
    border-color: #0F1518;
    color: #ffffff;

    font-size: 12px;
    line-height: 1.2;

    text-align: center;
    vertical-align: center;
}
```

В итоге данный комонент уже больше походит на кнопку, чем на стилизованный прямоугольник.

### Расширенная настройка стилей компонента

Библиотека позволяет использовать 3 псевдокласса `css` такие как `hover`, `active` и `focus`. Псевдоклассы в `css` задаются следующим образом:
```css
.name-class:pseudoclass-name
{

}
```

Псевдокласс `hover` задает стили компонента, если на него наведен курсор мыши. `active`, когда на объект только была нажата кнопка мыши, но еще не была отпущена. `focus` – когда компонент в фокусе.

Добавим компоненту стили при наведении, для этого добавим в файл стилей следующее:

```css
.button:hover
{
    background-color: #0D1012;
}
```

Теперь при наведении на компонент, он меняет свой фоновый цвет.
 

### Добавление прослушивателей для событий

Любые компоненты могут отслеживать следующие события:

|    Event    |  Description  |
|-------------|---------------|
|  `click` | Event occurs when a component is clicked |
|  `hover` | An event occurs when you hover over a component |
|  `focus` | The event occurs when focusing on the component |
|  `focusout` | Event occurs when component loses focus |
|  `mousemotion` | The event occurs when the mouse moves on the component |
|  `onmousedown` | An event occurs when a button is pressed on a component |
|  `onmouseup` | An event occurs when a button is released |
|  `onmouseover` | An event occurs when the mouse enters the component |
|  `onmouseout` | An event occurs when the mouse leaves the component |


Для добавления прослушивателя используется метод `addEventListener`, где первым аргументом идет имя события, которое мы прослушиваем, а вторым функция, которая будет вызвана при возникновении данного события:

```cpp
void addEventListener(string name_event, function<void(Component* sender, Event* e) callback>);
```

Добавим прослушиватель события клика на наш элемент, используя лямбда-функцию для более краткой записи:

```cpp
void setup()
{
    include("css/style.css");

    add("#button", ".button").setText("ok");

    Window::getElementById("#button")->addEventListener("click", 
    [](Component* sender, Event* e)
    {
        std::cout << "Component clicked" << std::endl;
    });
}
```

Теперь при нажатии левой кнопкой мыши на компонент, в консоли будут выводиться сообщения.

Данная реализация кнопки довольно неудобна, из-за использования дополнительных методов, далее будет рассмотрено создание своих компонентов на базе класса `Component` в которых вся логика будет инкапсулирована в компоненте.

### Удаление прослушивателя для события

Так как можно установить прослушиватель, его можно и удалить с помощью функции `removeEventListener`:

```cpp
void removeEventListener(string action);
```

### Немедленный вызов установленного прослушивателя

Может случиться такая ситуация, что необходимо вызвать метод привязанный к какому-либо событию, для этого используется метод `callEventListener`, где первым парметром передается имя события, а вторым параметром передается событие:

```cpp
void callEventListener(string action, Event* e);
```


## Создание собственных компонентов на базе класса `Component`

Перейдем к созданию собственных компонентов.

Создадим папку `Button` рядом с папкой `MyWindow` и добавим `Button.h`.

Для создания собственных компонентов подключим заголовочный файл класса `Component`. Создадим класс и унаследуем его от `Component`, перегрузим конструктор и добавим метод `setup`, который вызовем в конструкторе.

В конструктор сразу добавим поле для текста кнопки и запишем его в поле `text`.

```cpp
#pragma once
#include "../kit/component/component.h"

using namespace Kit;

class Button : public Component
{
private:
    string text;

public:
    Button(string id, string classes, string text)
    	: Component(id, classes)
    {
        this->text = text;
        setup();
    }
	
public:
    void setup()
    {

    }
};
```

Метод `setup` используется для настройки компонента, в нем можно вызывать все методы класса `Component`. В предыдущем примере мы использовали метод `setText` для установки текста компоненту. Вызовем этот метод в `setup`.

```cpp
void setup()
{
    setText(this->text);
}
```

Таким образом мы инкапсулировали логику базового класса `Component` в обертке `Button`.

Подключим данный компонент в окно и добавим его:

```cpp
#pragma once

#include "../kit/window/window.h"
using namespace Kit;

class MyWindow : public Window
{
public:
    MyWindow(string title, SimpleRect size)
        : Window(title, size) 
    {
        setup();
    };

public:
    void setup()
    {
        include("css/style.css");

        add(new Button("#button", ".button", "Ok");
    }

};
```

Это все та же кнопка, но теперь не нужно вызывать метод `setText` для установки текста, так как компонент сам знает, что его надо установить при создании.


## Подключение стилей в компоненте

До этого все стили подключались глобально для всего окна, но каждый компонент может также содержать в себе необходимые стили, тем самым, этот компонент будет полностью обособлен от окна. 

Для добавления стилей для компонента используется функция `include`:

```cpp
void include(string path);
```

Например вынесем стили кнопок в стили отдельного компонента. Создадим в папке `Button` папку `css` и добавим в нее файл `button.css` и пропишем в нем стили из `style.css`:

```css
.button
{
    background-color: #263238;
    border-color: #0F1518;
    color: #ffffff;

    font-size: 12px;
    line-height: 1.2;

    text-align: center;
    vertical-align: center;
}

.button:hover
{
    background-color: #0D1012;
}

```

а затем подключим его:

```cpp
void setup()
{
    include("../Button/css/button.css");

    setText(this->text);
}
```

И удалим стили кнопки из `style.css`. 

Теперь компонент `Button` полностью обособлен. При его добавлении подключать стили в стилях окна нет необходимости, так как компонент сам подключит нужные стили.

## Дополнительная информация в компоненте

Любой компонент может хранить в себе дополнительную информацию. Для добавления используется метод `addUserData`:

```cpp
void addUserData(string key, std::any data);
```

А для получения информации по ключу --- метод `userData`:

```cpp
std::any userData(string key);
```


## Расширенная работа с классовыми идентификаторами

Классовые идентификаторы используются для стилизации компонентов, для работ с ними есть несколько методов. Названия методов говорят сами за себя.

```cpp
bool hasClass(const string& className) const;
Component* removeClass(const string& className);
Component* addClass(const string& className);
Component* toggleClass(const string& className);
```

## Получение компонентов по классовому идентификатору

Как уже было описано для доступа к компоненту используется его идентификатор. Но доступ к элементу можно получить и по классовому идентификатору, но с некоторыми оговорками. 

Так как один и тот же классовый идентификатор может быть у нескольких компонентов, то по классовому идентификатору будет получена коллекция компонентов.

Для получения коллекции используется метод `getElementsByClassName`:

```cpp
Components getElementsByClassName(const string& className);
```

Класс `Components` является оберткой над коллекцией компонентов. Над коллекцией можно производить следующие действия:

1. Установить каждому элементу коллекции одинаковый прослушиватель с помощью метода `addEventListener`, который полностью повторяет метод класса `Component`;

2. Вызвать для каждого элемента коллекции некоторую функцию обратного вызова с передачей первым параметром указателя на текущий элемент коллекции с помощью метода `each`
    ```cpp
    void each(function< void(Component* sender)> callback);
    ```

3. Получить компонент по номеру с помощью метода `at` или `оператора []`:
    ```cpp
    Component* at(size_t index);
    Component* operator[](size_t index);
    ```



## Tracked events

|    Event    |  Description  |
|-------------|---------------|
|  `click` | Event occurs when a component is clicked |
|  `hover` | An event occurs when you hover over a component |
|  `focus` | The event occurs when focusing on the component |
|  `focusout` | Event occurs when component loses focus |
|  `mousemotion` | The event occurs when the mouse moves on the component |
|  `onmousedown` | An event occurs when a button is pressed on a component |
|  `onmouseup` | An event occurs when a button is released |
|  `onmouseover` | An event occurs when the mouse enters the component |
|  `onmouseout` | An event occurs when the mouse leaves the component |

## Supported CSS attributes

|  Attribute  | Value type  |  Description  |
|-------------|-------------|---------------|
| **Size** |||
|`width` | The width of the component | `[number + px/number + %/calc((expr -> X% +(-) Ypx))]`|
|`height` | The height of the component | `same` |
|`top` | Shift the component from the top edge (Y) | `same` |
|`left` | Shift the component from the left edge (X) | `same` |
| **Background** |||
|`background-color`| The background color of the component. |`[hex/rgb/rgba]`|
|`background-image` |The background image of the component. |`[url(path_to_image_without_quotes)]`|
|`background-position-x` | Shift the background image by X |`[number + px]`|
|`background-position-y` | Shift the background image by Y |`[number + px]`|
|`background-size` | Background image width (height proportional) |`[number + px]`|
| **Text** |||
|`color` | Color of the text in the component. |`[hex/rgb/rgba]`|
|`font-size` | Font size in the component | `[number + px]`|
|`font-family` | Font family in the component |`[string]`|
|`font-style` | Font style in the component |`[normal/italic]`|
|`font-weight` | Weight of the font in the component |`[100-900]`|
|`line-height` | Line height of the font in the component |`[double]`|
|`text-align`  | Alignment of the text horizontally in the component |`[left/center/right]`|
|`vertical-align`  | Alignment of the text vertically in the component. |`[top/center/bottom]`|
|`margin-[top/bottom/left/right]` | Shift the text in the directions in the component. |`[number + px]`|
| **Border** |||
|`border-color` | Color of the border of the component. |`[hex/rgb/rgba]`|
|`border-radius` | The radius of the curve of the stroke in the component  |`[number + px]`|
|`border-[top/bottom/left/right]` | Border |`[number + px hex solid]`|
|`border-[top/bottom/left/right]-size` | Width of the border in the component |`[number + px]`|
|`border-[top/bottom/left/right]-color` |  Color of the border in the component |`[hex]`|
|`border-[top/bottom/left/right]-type` |  Type of the border in the component |`[solid]`|
|`outline` | Single pixel border. |`[hex/rgb/rgba]`|
| **Display** |||
|`display` | Display of child elements. `block` - alignment of elements in a column. `inline-block` - alignment of elements in a row. |`[block/inline-block/unset]`|
| **Scroll** |||
|`overflow` | Determines whether the scroll is added if the child components are outside the parent. |`[hidden/unset]`|

## Supported CSS Pseudo-Classes

|  Pseudo-Class |  Description  |
|---------------|---------------|
|  Hover | Describes the styles for the state when the mouse cursor is over the component |
|  Active | Describes styles for the state when the mouse is clicked on a component |
|  Focus  |Describes styles for the state when the component is in focus.|