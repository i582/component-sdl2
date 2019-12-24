#include "MyWindow.h"
#include "../kit/kit-main.h"

#include "../DialogWindow/DialogWindow.h"

MyWindow::MyWindow(string title, SimpleRect size)
	: Window(title, size)
{
	this->count_clicks = 0;
	setup();
}

void MyWindow::setup()
{
	include("./css/style.css");
	setDraggableArea({ 0, 0, _size.w - 135, 25 });


	$$->append(new Component("#window-header", { 0, 0, _size.w - 135, 30 }, ".window-header"))
		->setText(this->title);

	$$->append(new SystemButton("#system-exit", { "100% - 45px", "0px", "45px", "30px" }, ".system-button .system-exit"));
	$$->append(new SystemButton("#system-expand", { "100% - 90px", "0px", "45px", "30px" }, ".system-button .system-expand"));
	$$->append(new SystemButton("#system-collapse", { "100% - 135px", "0px", "45px", "30px" }, ".system-button .system-collapse"));



	add("#test-block", ".test-class",
	{
		create("#test-block-inner", ".test-class-inner-1", 
		{
			create("#test-block-inner-inner", ".test-class-inner-inner", 
			{
				Button::create("#button-1", "Кнопка", ".button")
			})
		}),
		create("#test-block-inner-2", ".test-class-inner-2")
	});

	/*add("#buttons-block", ".buttons",
	{
		Button::create("#button-1", "Кнопка", ".button-1")
	});*/




	
	/*
	//$$->append(new Label("#label", { "20px", "40px", "200px", "20px" }, "", "Скролл по вертикале"));
	//$$->append(
	//	new Component("#test-scroll", { "20px", "90px", "150px", "200px" }, ".outer-block",
	//	{
	//		new Component("#test-scroll-inner", { "0px", "0px", "100%", "200%" }, ".inner-block")
	//	})
	//);


	//$$->append(new Label("#label-1", { "210px", "40px", "200px", "40px" }, "", "Скролл по вертикале и горизонтали"));
	//$$->append(
	//	new Component("#test-scroll-1", { "210px", "90px", "150px", "200px" }, ".outer-block",
	//		{
	//			new Component("#test-scroll-inner-1", { "0px", "0px", "200%", "200%" }, ".inner-block")
	//		})
	//);


	//$$->append(new Label("#label-2", { "430px", "40px", "200px", "40px" }, "", "Наведите или кликните"));
	//$$->append(new Component("#click-or-hover", { "430px", "90px", "150px", "200px" }, ".click-and-hover"));
	//$$->append(new Label("#label-count-clicks", { "430px", "290px", "200px", "20px" }, "", "Количество кликов: 0"))->hide();


	//$$->append(new Label("#label-align-text", { "650px", "40px", "200px", "40px" }, "", "Выравнивание текста"));

	//$$->append(new Component("#label-align-text-centered", { "650px", "80px", "200px", "30px" }, ".label-align-centered"))
	//	->setText("По центру");

	//$$->append(new Component("#label-align-text-left", { "650px", "120px", "200px", "30px" }, ".label-align-left"))
	//	->setText("Слева");

	//$$->append(new Component("#label-align-text-right", { "650px", "160px", "200px", "30px" }, ".label-align-right"))
	//	->setText("Справа");

	//$$->append(new Component("#label-align-text-top", { "650px", "200px", "200px", "30px" }, ".label-align-top"))
	//	->setText("Сверху");

	//$$->append(new Component("#label-align-text-bottom", { "650px", "240px", "200px", "30px" }, ".label-align-bottom"))
	//	->setText("Снизу");



	//$$->append(new Label("#label-21", { "880px", "40px", "200px", "40px" }, "", "Тестирование на производительность"));
	//$$->append(new Component("#test", { "880px", "90px", "150px", "200px" }, ".test-block"));


	//auto test = Window::getElementById("#test");

	//for (size_t i = 0; i < 10; i++)
	//{
	//	int place_y = i * 21;

	//	test->append(new Component("#test-item-" + to_string(i), { "0px", to_string(place_y) + "px", "100%", "20px" }, ".test-block-item"))
	//		->setText(to_string(i) + "-блок");
	//}


	//$$->append(new Label("#label-events", { "20px", "330px", "200px", "40px" }, "", "События"));
	//$$->append(new Component("#events", { "20px", "380px", "150px", "200px" }, ".events"));
	//$$->append(new Label("#label-events-info", { "20px", "580px", "150px", "40px" }, "", "Событие: "));




	//$$->append(new Label("#label-big-block", { "210px", "330px", "200px", "40px" }, "", "Блок с большим уровнем вложенности"));



	//

	//

	////add(new Button("#button-test"));

	//$$->append(
	//	new Component("#big-block", ".outer",
	//		{
	//			new Component("#block-inner-1", { "0px", "0px", "45%", "200%" }, ".inner",
	//			{
	//				new Button("#button-inner-1", { 20, 20, 70, 25 }, ".button .button-blue", "Ок 1"),
	//				new Button("#button-inner-2", { 20, 60, 70, 25 }, ".button", "Ок 2")
	//			}),
	//			new Component("#block-inner-2", { "55%", "0px", "45%", "100%" }, ".inner-2",
	//			{
	//				new Component("#block-in-inner", { "20px", "20px", "100% - 40px", "50px" }, ".inner-in",
	//				{
	//					new Button("#button-inner-in-1", { "50% - 35px", "20px", "70px", "25px" }, ".button", "Ок In 1"),
	//					new Button("#button-inner-in-2", { "50% - 35px", "60px", "70px", "25px" }, ".button", "Ок In 2")
	//				}),

	//				new Checkbox("#checkbox", { "20px", "110px", "100% - 40px", "16px" }, ".checkbox", "Текст"),
	//				new Checkbox("#checkbox1", { "20px", "140px", "100% - 40px", "16px" }, ".checkbox", "Текст 2")
	//			})
	//		})
	//);


	//$$->append(new Label("#label-image", { "650px", "330px", "200px", "40px" }, "", "Добавление изображения"));
	//$$->append(new Component("#image", { "650px", "380px", "200px", "200px" }, ".image"));



	//$$->append(new Label("#label-text", { "880px", "330px", "200px", "40px" }, "", "Абзац текста"));
	//$$->append(new Component("#text-block1", { "880px", "380px", "200px", "200px" }, ".text-block"))
	//	->setText("Чтобы автоматически закрывать консоль при остановке отладки, установите параметр \"Сервис\" -> \"Параметры\" -> \"Отладка\" -> \"Автоматически закрыть консоль при остановке отладки\".");


	//Window::getElementById("#button-inner-1")->addEventListener("click", [&](Component* sender, Event* e)
	//{
	//	Window::getElementById("#text-block1")->startAnimation();
	//});

	//Window::getElementById("#button-inner-2")->addEventListener("click", [&](Component* sender, Event* e)
	//{
	//	Window::getElementById("#text-block1")->endAnimation();
	//});

	//


	//Window::getElementById("#events")->addEventListener("click", [&](Component* sender, Event* e)
	//{
	//	Label* label = (Label*)(Window::getElementById("#label-events-info"));
	//	label->setText("Клик по компоненту");
	//	sender->window()->render();
	//});

	//Window::getElementById("#events")->addEventListener("mousemotion", [&](Component* sender, Event* e)
	//{
	//	Label* label = (Label*)(Window::getElementById("#label-events-info"));
	//	label->setText("Движение мыши по компоненту");
	//	sender->window()->render();
	//});

	//Window::getElementById("#events")->addEventListener("onmousedown", [&](Component* sender, Event* e)
	//{
	//	Label* label = (Label*)(Window::getElementById("#label-events-info"));
	//	label->setText("Кнопка мыши нажата");
	//	sender->window()->render();
	//});

	//Window::getElementById("#events")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	//{
	//	Label* label = (Label*)(Window::getElementById("#label-events-info"));
	//	label->setText("Кнопка мыши отпущена");
	//	sender->window()->render();
	//});

	//Window::getElementById("#events")->addEventListener("onmouseout", [&](Component* sender, Event* e)
	//{
	//	Label* label = (Label*)(Window::getElementById("#label-events-info"));
	//	label->setText("Компонент потерял фокус курсора мыши");
	//	sender->window()->render();
	//});


	//Window::getElementById("#click-or-hover")->addEventListener("click", [&](Component* sender, Event* e)
	//{
	//	count_clicks++;
	//	Label* label = (Label*)(Window::getElementById("#label-count-clicks"));
	//	label->setText("Количество кликов: " + to_string(count_clicks));
	//	label->show();
	//	sender->window()->render();
	//});

	//Window::getElementById("#system-exit")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	//{
	//	//close();
	//		parent->addWindow(new DialogAnswer("Предупреждение", { -1, -1, 200, 150 }, "Вы действительно хотите выйти из системы?",
	//			[&](Window* owner)
	//			{
	//				owner->close();
	//				close();
	//			},
	//			[&](Window* owner)
	//			{
	//				owner->close();
	//			})
	//		);
	//});

	//Window::getElementById("#system-collapse")->addEventListener("onmouseup", [&](Component* sender, Event* e)
	//{
	//	collapse();
	//});
	*/

}
