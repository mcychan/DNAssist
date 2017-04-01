
// RichTextFormat.cpp : implementation of the CRichTextFormat class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "DNAssist.h"
#endif

#include "RichTextFormat.h"

CRichTextFormat::CRichTextFormat(LOGFONT logf)
{
	logfont = logf;
	fontface = logfont.lfFaceName;
	fontsize = abs(logfont.lfHeight);
}

CRichTextFormat::~CRichTextFormat()
{
}

void CRichTextFormat::Initialise()
{
	string temp;
	GetFontTable(temp);
	data = "{\\rtf1\\ansi " + temp;
	SetFont();
}

void CRichTextFormat::Finalise()
{
	data += "}";
}

void CRichTextFormat::SetFont()
{
	data += "\\plain \\f0 \\fs";
	data += to_string(2 * fontsize) + " ";
}

void CRichTextFormat::SetBold()
{
	data += "\\b ";
}

void CRichTextFormat::SetUnderline()
{
	data += "\\ul ";
}

void CRichTextFormat::AddString(string text)
{
	data += text;
}

void CRichTextFormat::AddCharacter(char character)
{
	data += character;
}

void CRichTextFormat::AddLineBreak()
{
	data += "\\par ";
}

string CRichTextFormat::GetRichTextFormat(string& result)
{
	result = data;
	return data;
}

string CRichTextFormat::GetFontTable(string& result)
{
	result.clear();
	result += "{\\fonttbl{\\f0\\";
	int family = (logfont.lfPitchAndFamily & 60) >> 2;
	switch(family) {
		case FF_DECORATIVE:  result += "{fdecor "; break;
		case FF_MODERN:  result += "{fmodern "; break;
		case FF_ROMAN:  result += "{froman "; break;
		case FF_SCRIPT:  result += "{fscript "; break;
		case FF_SWISS:  result += "{fswiss "; break;
		default: result += "fnil ";
	}
	string fontFamily(fontface.begin(), fontface.end());
	result += fontFamily + ";}}";
	return result;
}

void CRichTextFormat::GetColorTable(COLORREF fident, COLORREF fcons, COLORREF bident, COLORREF bcons)
{
	string temp;
	data += "{\\colortbl;\\red0\\green0\\blue0;\\red255\\green255\\blue255;";
	data += GetColorComponent(temp, fident, 'R');
	data += GetColorComponent(temp, fident, 'G');
	data += GetColorComponent(temp, fident, 'B') + ";";
	data += GetColorComponent(temp, fcons, 'R');
	data += GetColorComponent(temp, fcons, 'G');
	data += GetColorComponent(temp, fcons, 'B') + ";";
	data += GetColorComponent(temp, bident, 'R');
	data += GetColorComponent(temp, bident, 'G');
	data += GetColorComponent(temp, bident, 'B') + ";";
	data += GetColorComponent(temp, bcons, 'R');
	data += GetColorComponent(temp, bcons, 'G');
	data += GetColorComponent(temp, bcons, 'B') + ";}";
}

void CRichTextFormat::SetForeGroundColor(int index)
{
	data += "\\cf";
	data += to_string(index) + " ";
}

void CRichTextFormat::SetBackGroundColor(int index)
{
	data += "\\highlight";
	data += to_string(index) + " ";
}

string CRichTextFormat::GetColorComponent(string& result, COLORREF color, char component)
{
	result.clear();
	switch(component) {
		case 'R' :
		{
			result += "\\red";
			BYTE val = GetRValue(color);
			result += to_string(val);
			break;
		}
		case 'G' :
		{
			result += "\\green";
			BYTE val = GetGValue(color);
			result += to_string(val);
			break;
		}
		case 'B' :
		{
			result += "\\blue";
			BYTE val = GetBValue(color);
			result += to_string(val);
			break;
		}
	}
	return result;
}
