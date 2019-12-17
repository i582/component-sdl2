#include "text.h"
#include "../../component/component.h"


using namespace Kit;

Text::Text(Component* parent, string text, Rect size, Font* font, size_t fontSize, Color color)
{
	this->parent = parent;

	if (parent != nullptr)
		this->renderer = parent->renderer();

	this->texture = nullptr;
	this->text = text;
	this->size = size;
	this->font = font;
	this->fontSize = fontSize;

	if (font != nullptr)
		this->ttf_font = font->at(fontSize);

	this->color = color;

	this->lineHeight = 1.3;


	this->textAlign = TextAlign::LEFT;
	this->blockVerticalAlign = TextBlockVerticalAlign::TOP;

	this->blockMarginTop = 0;
	this->blockMarginBottom = 0;
	this->blockMarginLeft = 0;
	this->blockMarginRight = 0;


	this->splitted = false;

	this->x = 0;
	this->y = 0;

	this->needReRender = true;

	init();
}

Text::~Text()
{
	SDL_DestroyTexture(texture);

	delete words;
}

void Text::init()
{
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.w(), size.h());
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	words = Utils::split(text, ' ');
}

void Text::splitByLines()
{
	string tempLine;
	lines.clear();
	textBlockHeight = 0;

	textBlockHeight += blockMarginTop;

	for (auto& word : *words)
	{

		if (words->size() > 1)
		{
			TTF_SizeUTF8(ttf_font, (word + ' ').c_str(), &tw, &th);
		}
		else
		{
			TTF_SizeUTF8(ttf_font, word.c_str(), &tw, &th);
		}

		

		if (x + tw > size.w() - (blockMarginLeft + blockMarginRight))
		{
			x = 0;

			lines.push_back(tempLine);
			tempLine.clear();

			textBlockHeight += th + (lineHeight * fontSize) - fontSize;
		}


		x += tw;
		

		if (words->size() > 1)
		{
			tempLine += word + ' ';
		}
		else
		{
			tempLine += word;
		}

		
	}

	if (!tempLine.empty())
	{
		lines.push_back(tempLine);
		tempLine.clear();

		textBlockHeight += th + (lineHeight * fontSize) - fontSize;
	}

	splitted = true;
}

void Text::renderLines()
{
	int lineShift = 0;

	if (this->blockVerticalAlign == TextBlockVerticalAlign::TOP)
	{
		lineShift = 0;

		lineShift += blockMarginTop;
	}
	else if (this->blockVerticalAlign == TextBlockVerticalAlign::CENTER)
	{
		lineShift = (parent->height() - textBlockHeight) / 2;

		lineShift += blockMarginTop;
	}
	else if (this->blockVerticalAlign == TextBlockVerticalAlign::BOTTOM)
	{
		lineShift = parent->height() - textBlockHeight;

		lineShift -= blockMarginBottom;
	}


	



	for (auto& line : lines)
	{
		SDL_Surface* textSurface = TTF_RenderUTF8_Blended(ttf_font, line.c_str(), color.colorSDL());

		if (textSurface == nullptr)
			continue;

		if (textAlign == TextAlign::LEFT)
		{
			textRect.x = 0;

			textRect.x += blockMarginLeft;
		}
		else if (textAlign == TextAlign::CENTER)
		{
			textRect.x = (size.w() - textSurface->w) / 2;

			textRect.x += blockMarginLeft;
		}
		else if (textAlign == TextAlign::RIGHT)
		{
			textRect.x = size.w() - textSurface->w;

			textRect.x -= blockMarginRight;
		}

		

		textRect.y = lineShift;
		textRect.w = textSurface->w;
		textRect.h = textSurface->h;

		lineShift += (textSurface->h + lineHeight * fontSize - fontSize);

		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		SDL_SetTextureBlendMode(textTexture, SDL_BLENDMODE_BLEND);
		SDL_FreeSurface(textSurface);

		SDL_SetRenderTarget(renderer, texture);
		SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
		SDL_DestroyTexture(textTexture);
	}

}

void Text::render()
{
	this->x = 0;
	this->y = 0;

	
	if (needReRender)
	{
		SDL_SetRenderTarget(renderer, texture);

		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
		SDL_RenderClear(renderer);

		if (!splitted)
			splitByLines();
	
		renderLines();
 
		this->needReRender = false;
	}


	SDL_SetRenderTarget(renderer, parent->innerTexture());
	SDL_RenderCopy(renderer, texture, NULL, &size.toSdlRect());
}

void Text::setText(const string& text)
{
	if (this->text == text)
		return;

	this->text = text;

	delete this->words;
	this->words = Utils::split(text, ' ');

	this->splitted = false;


	this->needReRender = true;
}

void Text::setSize(const Rect& size)
{
	if (this->size == size)
		return;

	this->size = size;

	SDL_DestroyTexture(texture);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.w(), size.h());
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

	this->splitted = false;
	this->needReRender = true;
}

void Text::setFont(const Font& font)
{
	if (this->font == &font)
		return;

	Font* f = new Font;
	*f = font;

	this->font = f;

	if (this->font != nullptr)
		this->ttf_font = font.at(fontSize);

	this->splitted = false;
	this->needReRender = true;
}

void Text::setFontSize(const size_t& fontSize)
{
	if (this->fontSize == fontSize)
		return;

	this->fontSize = fontSize;

	if (this->font != nullptr)
		this->ttf_font = font->at(fontSize);


	this->splitted = false;
	this->needReRender = true;
}

void Text::setColor(const Color& color)
{
	if (this->color == color)
		return;

	this->color = color;

	this->needReRender = true;
}

void Text::setLineHeight(const double& lineHeight)
{
	if (this->lineHeight == lineHeight)
		return;

	this->lineHeight = lineHeight;

	this->needReRender = true;
}

void Text::setTextAlign(const string& align)
{
	TextAlign temp;
	if (align == "left")
	{
		temp = TextAlign::LEFT;
	}
	else if (align == "center")
	{
		temp = TextAlign::CENTER;
	}
	else if (align == "right")
	{
		temp = TextAlign::RIGHT;
	}

	if (this->textAlign == temp)
		return;

	this->textAlign = temp;

	this->needReRender = true;
}

void Text::setTextBlockVerticalAlign(const string& align)
{
	TextBlockVerticalAlign temp;
	if (align == "top")
	{
		temp = TextBlockVerticalAlign::TOP;
	}
	else if (align == "center")
	{
		temp = TextBlockVerticalAlign::CENTER;
	}
	else if (align == "bottom")
	{
		temp = TextBlockVerticalAlign::BOTTOM;
	}
	else
	{
		return;
	}

	if (this->blockVerticalAlign == temp)
		return;

	this->blockVerticalAlign = temp;

	this->needReRender = true;
}

void Text::setTextBlockMargin(const string& side, int value)
{
	if (side == "top")
	{
		if (blockMarginTop == value)
			return;

		blockMarginTop = value;
	}
	else if (side == "bottom")
	{
		if (blockMarginBottom == value)
			return;

		blockMarginBottom = value;
	}
	else if (side == "left")
	{
		if (blockMarginLeft == value)
			return;

		blockMarginLeft = value;
	}
	else if (side == "right")
	{
		if (blockMarginRight == value)
			return;

		blockMarginRight = value;
	}
	else
	{
		return;
	}

	this->splitted = false;
	this->needReRender = true;
}

void Text::setRenderer(SDL_Renderer* renderer)
{
	this->renderer = renderer;
}

bool Text::onHover(Point& p)
{


	return false;
}

SDL_Renderer* Text::getRenderer()
{
	return renderer;
}

SDL_Texture* Text::getTexture()
{
	return texture;
}

int Text::getFontSize()
{
	return fontSize;
}

Font* Text::getFont()
{
	return font;
}

Color Text::getColor()
{
	return color;
}