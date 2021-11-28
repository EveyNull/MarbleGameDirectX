#pragma once

#include <dwrite.h>

class Text
{
public:
	Text(IDWriteFactory* dWriteFactory, const wchar_t* textString, const wchar_t* fontFamily, const float& fontSize);
	~Text() = default;

	IDWriteTextFormat* GetFormat();
	const wchar_t* GetText();
	UINT32 GetLength();

	void SetText(const wchar_t* newText);

private:
	IDWriteTextFormat* textFormat;
	wchar_t* text;
	UINT32 textLength;
};

