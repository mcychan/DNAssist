#pragma once

#include <string>
#include <vector>

using namespace std;

class CRichTextFormat
{
public:
	CRichTextFormat(LOGFONT logfont);
	~CRichTextFormat();
	void SetFont();
	void SetBold();
	void SetUnderline();
	void AddString(string text);
	void AddCharacter(char character);
	void AddLineBreak();
	string GetRichTextFormat(string& result);
	void Initialise();
	void Finalise();
	void GetColorTable(COLORREF fident, COLORREF fcons, COLORREF bident, COLORREF bcons);
	void SetForeGroundColor(int index);
	void SetBackGroundColor(int index);

protected:
	string GetFontTable(string& result);
	string data;
	wstring fontface;
	int fontsize;
	LOGFONT logfont;
	string GetColorComponent(string& result, COLORREF color, char component);

};
