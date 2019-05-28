#pragma once

#include "resource.h"
#include <windows.h>
#include <commdlg.h>
#include <d2d1.h>
#include <d2d1_3.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <string>
#include <vector>

namespace Math{

	struct Equation{
		std::wstring s;
		D2D1_POINT_2F xyPoint;
	};

	struct EquationString{
		WCHAR s;
		int type;
		int subType;
		float w;
		float x;
	};

	class Eq_Edit{
	public: 
		Eq_Edit();
		~Eq_Edit();
		void init_DirectWrite(HWND handle);
		void loadGlyphs();
		void displayIconicEquations();
		void displayGlyphs();
		float getGlyphMetrics(WCHAR glyph);

		HRESULT CreateDeviceResources();
		void DiscardDeviceResources();
		void draw_Eq();
		void keyDown(WPARAM wParam);
		void wmChar(WPARAM wParam);
		void wmSetFocus();
		
		void wmSize( int w, int h);
		void lButtonDown(WPARAM wParam, LPARAM lParam);
		void mouseMove(WPARAM wParam, LPARAM lParam);
		void lButtonUp(WPARAM wParam, LPARAM lParam);
		void readTheDrag();
		void cutTheDrag();
		void pasteTheDrag();
		int glyphSelection();
		void setEqStringMetrics(WCHAR glyph);
		void deleteGlyph();

		void copyToClipboard();
		int  countEqStrings( std::wstring eQ );
		void parseEqStrings( std::wstring s, int type, int subType);

	private:

		HWND handle;
		HDC hdc;


		ID2D1Factory* pD2DFactory_;
		ID2D1HwndRenderTarget* pRT_;
		IDWriteFactory* pDWriteFactory_;
		IDWriteTextFormat* pMathFormat_;
		IDWriteTextLayout* pMathLayout_;

		IDWriteTextFormat* pSubSupFormat_;
		IDWriteTextLayout* pSubSupLayout_;

		ID2D1SolidColorBrush* pMathBrush_;
		ID2D1SolidColorBrush* pPaperBrush_;

//		std::wstring glyph;
		std::vector<EquationString> S{};
		std::vector<EquationString> D{};

		EquationString baseSup{};
		std::vector<EquationString> base{};
		EquationString baseSub{};

		EquationString numDenSup{};
		EquationString numDen{};
		EquationString numDenSub{};

		EquationString denNumSup{};
		EquationString denNum{};
		EquationString denNumSub{};

		int charCount{};

		float fontSize;

		WCHAR glyphs[10][25]{};
		WCHAR glyph{};
		float glyphWidth{};
		//TCHAR glyphs[10][25]{};
		//TCHAR glyph{};

		RECT clientRect{};
		int clientWidth{}; 
		int clientHeight{};

		RECT glyphRect{};
		RECT glyphBox{};
		RECT modelRect{};
		RECT equationRect{};
		RECT currentLevel{};

//  Caret 
		bool dragging;
		bool dragBar;
		int dragLength;
		int dragStart;
		float X;
		float insertionPoint{};
		int indexLeft{};


// glyph box
		float cellWidth{};
		float cellHeight{}; 

		const float pageMargin{50.0f};
		const float middleMargin{25.0f};

		int level[13]{};
		//int currentLevel{};
		int groundLevel{};
//		float lineEnd{};
//		int nCaretPosX = 0; // horizontal position of caret 
//		int nCaretPosY = 0; // vertical position of caret 
//		int nCharWidth = 0; // width of a character 
//		ABC charWidth;
//		int cch = 0;        // characters in buffer 
//		int nCurChar = 0;   // index of current character 

		TEXTMETRIC tm;             // structure for text metrics 


	}; // End class Eq_Text










} // End namespace Math



