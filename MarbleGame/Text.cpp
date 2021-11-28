#include "Text.h"

Text::Text(IDWriteFactory* dWriteFactory, const wchar_t* textString, const wchar_t* fontFamily, const float& fontSize)
{
	HRESULT result;

	SetText(textString);

	result = dWriteFactory->CreateTextFormat(fontFamily, NULL, 
		DWRITE_FONT_WEIGHT_REGULAR, 
		DWRITE_FONT_STYLE_NORMAL, 
		DWRITE_FONT_STRETCH_NORMAL, 
		fontSize, L"en-us", &textFormat);
	if (FAILED(result)) return;

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	if (FAILED(result)) return;

	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	if (FAILED(result)) return;
}

IDWriteTextFormat* Text::GetFormat()
{
	return textFormat;
}

const wchar_t* Text::GetText()
{
	return text;
}

UINT32 Text::GetLength()
{
	return textLength;
}

void Text::SetText(const wchar_t* newText)
{
	delete[] text;
	text = new wchar_t[wcslen(newText) + 1];
	wcscpy_s(text, wcslen(newText) + 1, newText);
	textLength = (UINT32)wcslen(text);
}