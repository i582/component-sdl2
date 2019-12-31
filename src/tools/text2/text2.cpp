#include "text.h"

Kit::Text2::Text2(SDL_Renderer* renderer, const string& text, SimpleRect size, const class font& new_font, size_t fontSize, const Color& colorFont)
	: Text2(renderer, nullptr, text, size, new_font, fontSize, colorFont)
{}

Kit::Text2::Text2(SDL_Renderer* renderer, SDL_Texture* parentTarget, const string& text, SimpleRect size, const class font& new_font, size_t fontSize, const Color& colorFont)
{
	this->renderer = renderer;
	this->size = size;

	this->parentTarget = parentTarget;

	this->text = text;
	this->font = new_font;
	this->fontSize = fontSize;
	this->colorFont = colorFont;
	this->fontTTF = font.at(fontSize);

	this->lineHeight = 1.3;

	this->isSelected = false;


	splitByLines();
	setup();
}

Kit::Text2::~Text2()
{
	SDL_DestroyTexture(texture);
}

void Kit::Text2::splitByLines()
{
	vector<string>* text_lines = Utils::split(text, '\n');
	
	for (auto& line : *text_lines)
	{
		auto new_line = new TextLine(this, line);
		lines.push_back(new_line);
	}
	
	delete text_lines;
}

void Kit::Text2::setup()
{
	size.h = (int)(lines.size() * fontSize * lineHeight);

	for (auto& line : lines)
	{
		if (line->size.w > size.w)
		{
			size.w = line->size.w;
		}
	}

	Texture::destroy(texture);
	Texture::create(renderer, size);

	splitLinesByToken();
	deleteSelect();
}

void Kit::Text2::splitLinesByToken()
{
	for (auto& line : lines)
	{
		line->splitByToken();
	}
}

void Kit::Text2::renderCursor()
{
	// calculate cursor position
	int y = (int)(cursorPos.y * fontSize * lineHeight);
	int x = 0;


	const string& textBeforeCursor = lines[cursorPos.y]->text.substr(0, cursorPos.x);
	TTF_SizeUTF8(fontTTF, textBeforeCursor.c_str(), &x, nullptr);


	const SDL_Rect cursorRect = { x, y, 1, (int)(fontSize * lineHeight) };
	SDL_SetRenderDrawColor(renderer, 0xdf, 0xdf, 0xbf, 0xff);

	
	SDL_RenderFillRect(renderer, &cursorRect);

}

void Kit::Text2::handleSelect()
{
	//
	for (auto& line : lines)
	{
		line->deleteSelected();
	}

	CursorPosition tempStartCursorSelect = startCursorSelect;
	CursorPosition tempEndCursorSelect = endCursorSelect;

	if (tempStartCursorSelect.y > tempEndCursorSelect.y)
	{
		std::swap(tempStartCursorSelect, tempEndCursorSelect);
	}


	if (tempStartCursorSelect.y == tempEndCursorSelect.y)
	{
		lines[tempStartCursorSelect.y]->startCursorSelect.x = tempStartCursorSelect.x;
		lines[tempStartCursorSelect.y]->endCursorSelect.x = tempEndCursorSelect.x;
	}

	else if (tempStartCursorSelect.y == tempEndCursorSelect.y - 1)
	{

		lines[tempStartCursorSelect.y]->startCursorSelect.x = tempStartCursorSelect.x;
		lines[tempStartCursorSelect.y]->endCursorSelect.x = lines[tempStartCursorSelect.y]->text.size();


		lines[tempEndCursorSelect.y]->startCursorSelect.x = 0;
		lines[tempEndCursorSelect.y]->endCursorSelect.x = tempEndCursorSelect.x;
	}
	else if (tempEndCursorSelect.y - tempStartCursorSelect.y > 1)
	{

		lines[tempStartCursorSelect.y]->startCursorSelect.x = tempStartCursorSelect.x;
		lines[tempStartCursorSelect.y]->endCursorSelect.x = lines[tempStartCursorSelect.y]->text.size();



		for (size_t i = tempStartCursorSelect.y + 1; i < tempEndCursorSelect.y; i++)
		{
			lines[i]->startCursorSelect.x = 0;
			lines[i]->endCursorSelect.x = lines[i]->text.size();
		}



		lines[tempEndCursorSelect.y]->startCursorSelect.x = 0;
		lines[tempEndCursorSelect.y]->endCursorSelect.x = tempEndCursorSelect.x;
	}
}

void Kit::Text2::deleteSelect()
{
	isSelected = false;

	for (auto& line : lines)
	{
		line->deleteSelected();
	}
}

Kit::CursorPosition Kit::Text2::whereIsCursor(SimplePoint mouse)
{
	int numberLine = mouse.y / (int)(fontSize * lineHeight);

	if (numberLine > lines.size() - 1)
		numberLine = lines.size() - 1;

	int calcWidth = 0;
	int countSymbol = 0;
	for (auto& symbol : lines[numberLine]->text)
	{
		int widthSymbol = 0;
		char str[2] = { symbol, '\0' };
		TTF_SizeUTF8(fontTTF, str, &widthSymbol, nullptr);

		calcWidth += widthSymbol;
		countSymbol++;

		if (calcWidth > mouse.x)
		{
			int delta = calcWidth - mouse.x;

			if (delta > widthSymbol / 2.)
			{
				countSymbol--;
			}

			break;
		}
	}

	return { countSymbol, numberLine };
}

Kit::string Kit::Text2::copySelect()
{
	if (!isSelected)
		return "";

	string result;

	CursorPosition tempStartCursorSelect = startCursorSelect;
	CursorPosition tempEndCursorSelect = endCursorSelect;

	if (tempStartCursorSelect.y > tempEndCursorSelect.y)
	{
		std::swap(tempStartCursorSelect, tempEndCursorSelect);
	}


	if (tempStartCursorSelect.y == tempEndCursorSelect.y)
	{
		int start = tempStartCursorSelect.x;
		int end = tempEndCursorSelect.x;

		if (start > end)
		{
			std::swap(start, end);
		}

		result = lines[tempStartCursorSelect.y]->text.substr(start, end - start);
	}

	else if (tempStartCursorSelect.y == tempEndCursorSelect.y - 1)
	{
		int start = tempStartCursorSelect.x;
		int end = lines[tempStartCursorSelect.y]->text.size();

		result += lines[tempStartCursorSelect.y]->text.substr(start, end - start) + '\n';



		start = 0;
		end = tempEndCursorSelect.x;

		result += lines[tempEndCursorSelect.y]->text.substr(start, end - start);
	}
	else if (tempEndCursorSelect.y - tempStartCursorSelect.y > 1)
	{
		int start = tempStartCursorSelect.x;
		int end = lines[tempStartCursorSelect.y]->text.size();

		result += lines[tempStartCursorSelect.y]->text.substr(start, end - start) + '\n';



		for (size_t i = tempStartCursorSelect.y + 1; i < tempEndCursorSelect.y; i++)
		{
			start = 0;
			end = lines[i]->text.size();

			result += lines[i]->text.substr(start, end) + '\n';
		}



		start = 0;
		end = tempEndCursorSelect.x;
		result += lines[tempEndCursorSelect.y]->text.substr(start, end - start);
	}

	return result;
}

void Kit::Text2::render()
{


	SDL_SetRenderTarget(renderer, texture);

	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
	SDL_RenderClear(renderer);

	for (size_t i = 0; i < lines.size(); i++)
	{
		auto& line = lines[i];

		int y = (int)(i * fontSize * lineHeight);

		SDL_Rect rect = { 0, y, line->size.w, line->size.h };

		line->render();

		//SDL_SetRenderDrawColor(renderer, 0x33, 0x33, 0x33, 0xff);
		//SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderTarget(renderer, texture);
		SDL_RenderCopy(renderer, line->texture, nullptr, &rect);
	}

	if (!lines.empty())
		renderCursor();

	SDL_SetRenderTarget(renderer, parentTarget);

	SDL_RenderCopy(renderer, texture, nullptr, &size);
}

void Kit::Text2::setText(const string& new_text)
{
	this->text = new_text;

	splitByLines();
	setup();
}

void Kit::Text2::mouseMotion(SDL_Event* e)
{
	if (!mousePush)
		return;

	SimplePoint mouse = { e->motion.x, e->motion.y };
	mouse.x -= size.x;
	mouse.y -= size.y;

	if (!isSelected)
	{
		isSelected = true;
		startCursorSelect = endCursorSelect = whereIsCursor(mouse);
	}
	else
	{
		endCursorSelect = cursorPos = whereIsCursor(mouse);
	}

	handleSelect();
}

void Kit::Text2::mouseButtonUp(SDL_Event* e)
{
	mousePush = false;
}

void Kit::Text2::mouseButtonDown(SDL_Event* e)
{
	mousePush = true;

	SimplePoint mouse = { e->motion.x, e->motion.y };
	mouse.x -= size.x;
	mouse.y -= size.y;

	

	cursorPos = whereIsCursor(mouse);


	deleteSelect();
}

void Kit::Text2::mouseButtonDoubleDown(SDL_Event* e)
{
	SimplePoint mouse = { e->motion.x, e->motion.y };
	mouse.x -= size.x;
	mouse.y -= size.y;


	CursorPosition nowCursorPosition = whereIsCursor(mouse);

	auto& now_line = lines[nowCursorPosition.y];


	for (int i = nowCursorPosition.x; i > -1; i--)
	{
		char symbol = now_line->text[i];

		if (TextLine::isSplitSymbol(symbol) || i == 0)
		{
			startCursorSelect.x = i == 0 ? 0 : i + 1;
			startCursorSelect.y = nowCursorPosition.y;
			break;
		}
	}


	for (size_t i = nowCursorPosition.x; i < now_line->text.size(); i++)
	{
		char symbol = now_line->text[i];

		if (TextLine::isSplitSymbol(symbol) || i == now_line->text.size() - 1)
		{
			endCursorSelect.x = i;
			endCursorSelect.y = nowCursorPosition.y;
			break;
		}
	}

	cursorPos = endCursorSelect;

	handleSelect();
}

void Kit::Text2::keyDown(SDL_Event* e)
{
	switch (e->key.keysym.sym)
	{

	case SDLK_TAB:
	{
		lines.at(cursorPos.y)->addText("   ", cursorPos.x);

		cursorPos.x += 3;

		setup();
		break;
	}

	case SDLK_v:
	{
		if (SDL_GetModState() & KMOD_CTRL)
		{
			const string clipboardText = SDL_GetClipboardText();
			lines.at(cursorPos.y)->addText(clipboardText, cursorPos.x);

			cursorPos.x += clipboardText.length();

			setup();
		}

		break;
	}

	case SDLK_c:
	{
		if (SDL_GetModState() & KMOD_CTRL)
		{
			const string select_text = copySelect();
			SDL_SetClipboardText(text.c_str());
		}
		
		break;
	}

	case SDLK_a:
	{
		if (SDL_GetModState() & KMOD_CTRL)
		{
			if (startCursorSelect == CursorPosition(0, 0) &&
				endCursorSelect == CursorPosition((int)(lines[lines.size() - 1]->text.size()), (int)lines.size() - 1))
			{
				startCursorSelect = endCursorSelect = { 0, 0 };

				isSelected = false;
			}
			else
			{
				
				startCursorSelect = { 0, 0 };

				endCursorSelect =
				cursorPos = { (int)(lines[lines.size() - 1]->text.size()), (int)lines.size() - 1 };

				isSelected = true;
			}

			handleSelect();
		}
		
		break;
	}

	case SDLK_RETURN:
	{
		string new_line_text;


		if (cursorPos.x != lines[cursorPos.y]->text.length())
		{

			new_line_text += lines[cursorPos.y]->text.substr(cursorPos.x, lines[cursorPos.y]->text.length() - cursorPos.x);


			lines[cursorPos.y]->setText(lines[cursorPos.y]->text.substr(0, cursorPos.x));
		}

		else
		{
			new_line_text = "";
		}

		auto new_line = new TextLine(this, new_line_text);

		lines.insert(lines.begin() + cursorPos.y + 1, new_line);

		cursorPos.x = 0;
		cursorPos.y += 1;

		setup();
		break;
	}

	case SDLK_BACKSPACE:
	{
		bool line_delete = false;


		if (lines.at(cursorPos.y)->removeSymbol(cursorPos.x) && lines.size() > 1)
		{

			lines.erase(lines.begin() + cursorPos.y);
			line_delete = true;
		}

		if (cursorPos.x > 0)
		{
			cursorPos.x -= 1;
		}
		else
		{
			if (cursorPos.y > 0)
			{
				cursorPos.y -= 1;


				cursorPos.x = lines[cursorPos.y]->text.size();

				

				if (!line_delete)
				{

					lines[cursorPos.y]->setText(lines[cursorPos.y]->text + lines[cursorPos.y + 1]->text);


					lines.erase(lines.begin() + cursorPos.y + 1);
				}
			
			}
		}

		setup();
		break;
	}

	case SDLK_DELETE:
	{

		if (lines.at(cursorPos.y)->removeSymbolAfter(cursorPos.x) && cursorPos.y < lines.size() - 1)
		{

			lines.at(cursorPos.y)->setText(lines.at(cursorPos.y)->text + lines.at(cursorPos.y + 1)->text);


			lines.erase(lines.begin() + cursorPos.y + 1);
		}

		setup();

		break;
	}


	case SDLK_LEFT:
	{
		if (cursorPos.x > 0)
		{
			cursorPos.x -= 1;


			if (SDL_GetModState() & KMOD_SHIFT)
			{
				if (!isSelected)
				{

					startCursorSelect = cursorPos;
					startCursorSelect.x += 1;


					endCursorSelect = cursorPos;

					isSelected = true;
				}
				else
				{

					endCursorSelect = cursorPos;
				}

				handleSelect();
			}
			else
			{
				isSelected = false;


				deleteSelect();
			}
		}
		else
		{
			if (cursorPos.y > 0)
			{
				cursorPos.y -= 1;
				cursorPos.x = lines[cursorPos.y]->text.size();

				if (SDL_GetModState() & KMOD_SHIFT)
				{

					endCursorSelect = cursorPos;
				}
			}
				
		}

		break;
	}

	case SDLK_RIGHT:
	{
		if (cursorPos.x < lines[cursorPos.y]->text.size())	
		{
			cursorPos.x += 1;

			if (SDL_GetModState() & KMOD_SHIFT)
			{
				if (isSelected == false)
				{

					startCursorSelect = cursorPos;
					startCursorSelect.x -= 1;


					endCursorSelect = cursorPos;

					isSelected = true;
				}
				else
				{
					endCursorSelect = cursorPos;
				}

				handleSelect();
			}
			else
			{
				isSelected = false;

				deleteSelect();
			}
		}
		else
		{
			if (cursorPos.y < lines.size() - 1)
			{
				cursorPos.y += 1;
				cursorPos.x = 0;

				if (SDL_GetModState() & KMOD_SHIFT)
				{

					endCursorSelect = cursorPos;
				}
				else
				{
					isSelected = false;

					deleteSelect();
				}
			}
		}
		break;
	}

	case SDLK_DOWN:
	{

		if (cursorPos.y < lines.size() - 1)
		{

			cursorPos.y += 1;


			if (cursorPos.x > lines[cursorPos.y]->text.size())
			{

				cursorPos.x = lines[cursorPos.y]->text.size();
			}
		}

		else if (cursorPos.y == lines.size() - 1)
		{

			cursorPos.x = lines[cursorPos.y]->text.size();
		}



		if (SDL_GetModState() & KMOD_SHIFT)
		{
			if (!isSelected)
			{

				startCursorSelect = cursorPos;
				startCursorSelect.y -= 1;

				endCursorSelect = cursorPos;

				isSelected = true;
			}
			else
			{

				endCursorSelect = cursorPos;
			}

			handleSelect();
		}
		else
		{
			isSelected = false;

			deleteSelect();
		}

		break;
	}

	case SDLK_UP:
	{

		if (cursorPos.y > 0)
		{

			cursorPos.y -= 1;


			if (cursorPos.x > lines[cursorPos.y]->text.size())
			{

				cursorPos.x = lines[cursorPos.y]->text.size();
			}

		}

		else if (cursorPos.y == 0)
		{

			cursorPos.x = 0;
		}




		if (SDL_GetModState() & KMOD_SHIFT)
		{
			if (!isSelected)
			{

				startCursorSelect = cursorPos;
				startCursorSelect.y += 1;

				endCursorSelect = cursorPos;

				isSelected = true;
			}
			else
			{

				endCursorSelect = cursorPos;
			}

			handleSelect();
		}
		else
		{
			isSelected = false;
			deleteSelect();
		}

		break;
	}

	default:break;
	}

	render();
}

void Kit::Text2::textInput(SDL_Event* e)
{
	lines.at(cursorPos.y)->addText(e->text.text, cursorPos.x);

	cursorPos.x += 1;

	setup();
	render();
}
