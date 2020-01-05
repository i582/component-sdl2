# Начало работы

### Первоначальная настройка

Для использования библиотеки необходимо подключить заголовочный файл `component-sdl2`:

```cpp
#include <component-sdl2>
```

Библиотека расположена в пространстве имен `Kit`, и чтобы каждый раз не писать `Kit::*` в следующих примерах будет подразумеваться следующая строка:

```cpp
using namespace Kit;
```

Очень важно, функция `main` должна принимать два параметра `int argc, char** argv` !

После подключения библиотеки доступ к объекту библиотеки осуществляется через переменную `$` . Для запуска библиотеки нужно вызвать метод `run` .

Так что простейший код будет выглядеть так:

```cpp
#include <component-sdl2>
using namespace Kit;

int main(int argc, char** argv)
{
    return $.run();
}
```

### Добавление окон

Основным методом у основного класса библиотеки является `addWindow` , который используется для добавления окон. Имеет следующий вид:

```cpp
Window* addWindow(Window* window);
```

 Класс `Window` является базовым классом для любых окон приложения. Имеет единственный конструктор:

```cpp
// SimpleRect is struct { x, y, w, h }
Window(std::string title, SimpleRect size, noBorder = false);
```

Здесь первый параметр — это заголовок окна, второй —размеры и положение окна \(указание `-1` в координатах положения выравнивает окно по центру\), а третий — флаг указывающий будет ли у окна стандартная рамка. 

Рассмотрим добавление окна:

```cpp
#include <component-sdl2>
using namespace Kit;

int main(int, char**)
{
    $.addWindow(new Window("Test Window", { -1, -1, 300, 300 }, false));
    return $.run();
}
```

В результате, будет выведено пустое окно по центру размером 300 на 300 пикселей.

![&#x41E;&#x43A;&#x43D;&#x43E; &#x43F;&#x440;&#x438;&#x43B;&#x43E;&#x436;&#x435;&#x43D;&#x438;&#x44F;](.gitbook/assets/window-1.png)

### Добавление компонентов

Существует несколько способ для добавления новых компонентов в окно. Первый способ — это добавление компонентов в функции `main`. Для добавления компонентов используется функция `add`.  Метод `add` имеет несколько прототипов:

```cpp
Component* add(const string& id, const string& classes, 
               const vector<Component*>& childrens = {});

Component* add(const string& classes = "", 
               const vector<Component*>& childrens = {});

Component* add(Component* component);
```

Рассмотрим первый прототип, как самый общий. 

Первым параметром метод принимает строку-идентификатор компонента **\(Идентификаторы должен начинаться с символа `#`!\)**. При этом в окне не может быть два компонента с одинаковым идентификатором! 

Второй параметр — это строка классовых-идентификаторов разделенных пробелом. **\(Классовые идентификаторы должны начинаться с символа `.`!\)**. Классовые идентификаторы, в отличии от обычных идентификаторов могут повторяться от компонента к компоненту.

Третий параметр — это вектор дочерних компонентов. Обычно для добавления компонентов можно использовать и просто вызов `new Component(...)`, но лучше использовать статический метод `create` класса `Component`. Данный метод полностью повторяет метод `add` рассмотренный выше.

Второй прототип отличается только тем, что не принимает идентификатор. Данный прототип будет генерировать рандомный идентификатор для компонента, что может быть удобно в некоторых случаях.

Третий прототип просто принимает указатель на `Component`. Данный прототип будет использоваться при добавлении стандартных компонентов \(`Button`, `Checkbox` и т.д.\). Обычно, как и с дочерними компонентами из второго прототипа, лучше использовать метод `create` у класса `Component` или у классов стандартных компонентов.

#### Пример

Рассмотрим пример добавления компонента первым способом:

```cpp
#include <component-sdl2>
using namespace Kit;

int main(int, char**)
{
    auto window = $.addWindow(new Window("Test Window", { -1, -1, 300, 300 }, false));
    
    // добавление простого компонента
    window->add("#test", ".test");
    
    // добавление компонента с дочерними компонентами 
    window->add("#test-childs", ".test", {
        Component::create("#inner", ".inner"),
        Component::create("#inner-1", ".inner", {
            Component::create("#inner-inner", ".inner-inner")
        })
    });
    
    
    return $.run();
}
```

Пока что, если запустить программу, ничего не изменится с последнего запуска без компонентов. Это происходит потому что компоненты в окне существуют, но не стилизованны. 

### Стилизация компонентов

Для стилизации компонентов используется `css`. Каждое окно может хранить в себе стили всех своих компонентов. Для добавления `css` файла используется метод `style`:

```cpp
void style(const string& path);
```

> Очень важное замечание, путь к `css` файлу должен быть относительным к исполняемому файлу!

#### Пример

Для примера добавим рядом с `main.cpp` папку `css`, а в ней добавим файл `style.css`. Пропишем в нем следующее:

```css
.test
{
    width: 70px;
    height: 24px;

    top: 100px;
    left: 100px;

    background-color: #263238;
    outline: #0F1518;
}
```

Что здесь происходит, первое, **все стили задаются для классовых идентификаторов**, что позволяет переиспользовать стили у разных компонентов. Второе, задаются размеры и положение при помощи стандартных `css-атрибутов`. Больше информации о том, какие значения могут принимать данные атрибуты в [разделе ](vse-podderzhivaemye-css-stili.md)`css`. Третье, задается фоновый цвет и цвет однопиксельной обводки, об этих и остальных возможных свойств подробнее в [разделе ](vse-podderzhivaemye-css-stili.md)`css`.

Вызовем метод `style` и пропишем путь к стилям:

```cpp
#include <component-sdl2>
using namespace Kit;

int main(int, char**)
{
    auto window = $.addWindow(new Window("Test Window", { -1, -1, 300, 300 }, false));
    
    // Добавляем стили в окно
    window->style("../test/css/style.css");
    
    window->add("#test", ".test");
    
    return $.run();
}
```

После этого, если запустить программу, то в окне появится прямоугольник с заданными размерами с обводкой. 

### Расширенная стилизация

Для более гибкой настройки стилей в разных ситуациях библиотека позволяет использовать 3 `css` псевдокласса. 

Псевдокласс `hover` задает стили для компонента на который в данный момент наведен курсор мыши.

Псевдокласс `active` задает стили для компонента на который нажата кнопка мыши.

Псевдокласс `focus` задает стили для компонента который в данный момент находится в фокусе.

Псевдоклассы задаются следующим образом:

```css
.name-class:pseudoclass-name
{ 
    /* тут стили */
}
```

### Добавление компонентов \(вариант 2\)

Во многих случаях добавлять компоненты в окна  первым способом бывает не удобно, а иногда и не целесообразно, например, если нужно создать два одинаковых окна. Специально для этого существует второй способ. Второй способ заключается в наследовании от класса `Window`.

Создадим рядом с `main.cpp` папку `MyWindow`и добавим в нее заголовочный файл `MyWindow.h`

Для наследования необходимо подключить заголовочный файл класса `Window`

```cpp
#include "window/window.h"
```

Дальше необходимо создать класс на базе `Window` добавить конструктор и метод `setup`, который вызвать в конструкторе:

```cpp
#pragma once

#include "window/window.h"
using namespace Kit;

class MyWindow : public Window
{
public:
    MyWindow(string title, SimpleRect size, bool noBorder = false)
        : Window(title, size, noBorder) 
    {
        setup();
    };

public:
    void setup()
    {

    }

};
```

Здесь метод `setup` нужен для настройки компонентов в окне. Все описанное в первом способе так же работает и здесь, перепишем последний пример:

```cpp
#pragma once

#include "window/window.h"
using namespace Kit;

class MyWindow : public Window
{
public:
    MyWindow(string title, SimpleRect size, bool noBorder = false)
        : Window(title, size, noBorder) 
    {
        setup();
    };

public:
    void setup()
    {
        style("../test/css/style.css");
    
        add("#test", ".test");
    }

};
```

Подключим данное окно в `main`, и заменим `WIndow` на `MyWindow`:

```cpp
#include <component-sdl2>

#include "MyWindow/MyWindow.h"

using namespace Kit;

int main(int, char**)
{
    // создаем объект нестандартного окна
    $.addWindow(new MyWindow("Test Window", { -1, -1, 300, 300 }, false));
    
    return $.run();
}
```

Теперь, если запустить программу, то мы получим тот же результат, что и в предыдущем примере. При этом мы можем создать, например, два окна с одинаковым интерфейсом:

```cpp
#include <component-sdl2>

#include "MyWindow/MyWindow.h"

using namespace Kit;

int main(int, char**)
{
    // создаем объект нестандартного окна
    $.addWindow(new MyWindow("Test Window", { -1, -1, 300, 300 }, false));
    // и еще один
    $.addWindow(new MyWindow("Test Window 2", { 200, 200, 300, 300 }, false));
    
    return $.run();
}
```

### Добавление прослушивателей для событий

Любые компоненты могут отслеживать следующие события:

| Event | Description |
| :--- | :--- |
| `click` | Event occurs when a component is clicked |
| `hover` | An event occurs when you hover over a component |
| `focus` | The event occurs when focusing on the component |
| `focusout` | Event occurs when component loses focus |
| `mousemotion` | The event occurs when the mouse moves on the component |
| `onmousedown` | An event occurs when a button is pressed on a component |
| `onmouseup` | An event occurs when a button is released |
| `onmouseover` | An event occurs when the mouse enters the component |
| `onmouseout` | An event occurs when the mouse leaves the component |

Для добавления прослушивателя используется метод `addEventListener`, где первым аргументом идет имя события, которое мы прослушиваем, а вторым функция, которая будет вызвана при возникновении данного события:

```cpp
void addEventListener(const string& name_event, 
                      function<void(Component* sender, Event* e) callback>);
```

#### Пример

Добавим прослушиватель события клика на наш компонент, используя лямбда-функцию для более краткой записи \(рассмотрим только метод`setup` окна `MyWindow`\):

```cpp
void setup()
{
    style("../test/css/style.css");

    add("#test", ".test");

    Window::getElementById("#test")->addEventListener("click", 
    [](Component* sender, Event* e)
    {
        std::cout << "Component clicked" << std::endl;
    });
}
```

Теперь при нажатии на компонент в консоль будет выведено сообщение.

### Удаление прослушивателя для события

Для удаления прослушивателя используется метод `removeEventListener`:

```cpp
void removeEventListener(const string& action);
```

### Немедленный вызов установленного прослушивателя

Для немедленного вызова прослушивателя используется метод `callEventListener`:

```cpp
void callEventListener(const string& action, Event* e);
```

Вторым параметром идет указатель на событие в котором хранится информация о текущем событии.

