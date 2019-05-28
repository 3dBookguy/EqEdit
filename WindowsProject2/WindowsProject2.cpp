// WindowsProject2.cpp : Defines the entry point for the application.
// Change

#include "stdafx.h"
#include "Windowsx.h" // for GET_X_LPARAM
#include "Log.h"

// wingdi.h (include Windows.h)  GetCharWidth32
#include "WindowsProject2.h"


#define MAX_LOADSTRING 100
#define DEBUG_GB
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
 
// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



// SafeRelease inline function .		
template <class T> inline void SafeRelease(T **ppT)
{
    if (*ppT) {  (*ppT)->Release();  *ppT = NULL; }
}

Math::Eq_Edit::Eq_Edit():
	dragging(false),
	dragBar(false),
    pageMargin(50.0f),
    dragStart(0),
	pD2DFactory_(NULL),
    pRT_(NULL),
    pMathBrush_(NULL),
    pPaperBrush_(NULL),
    pDWriteFactory_(NULL),
    pMathFormat_(NULL),
    pMathLayout_(NULL),
    pSubSupFormat_(NULL),
    pSubSupLayout_(NULL)
{}

Math::Eq_Edit::~Eq_Edit(){
    SafeRelease(&pD2DFactory_);
    SafeRelease(&pRT_);
    SafeRelease(&pMathBrush_);
    SafeRelease(&pPaperBrush_);
    SafeRelease(&pDWriteFactory_);
    SafeRelease(&pMathFormat_);
    SafeRelease(&pMathLayout_);
    SafeRelease(&pSubSupFormat_);
    SafeRelease(&pSubSupLayout_);


}

Math::Eq_Edit editor;


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{

	Win::logMode(Win::LOG_MODE_BOTH);
	Win::log(" in wWinMain");
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINDOWSPROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSPROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
 //   wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground 	= (HBRUSH)GetStockObject(BLACK_BRUSH);

    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINDOWSPROJECT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   editor.init_DirectWrite(hWnd);


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void Math::Eq_Edit::loadGlyphs(){

			
// u'\u0420\u043e\u0441\u0441\u0438\u044f'
	//glyph = L'\u222b';
//	glyph = 8747;
// greek letters   0391 03c9  913   969
//	glyph = (TCHAR) wParam;

//	glyphs[0][0] = L'\u222b';
// 		TCHAR glyphs[10][25]{};

// Top row = row 0: Greek Caps 
	for( int col = 0; col < 25; col++){
		  glyphs[0][col] = 913 + col;
	}

// Row 1: Greek Lower Case 
	for( int col = 0; col < 25; col++){
		  glyphs[1][col] = 945 + col;
	}

// fill out the top row == 0 25 - 49
	glyphs[2][0] =   L'\u2212'; //8722; // 2212 minus sign
	glyphs[2][1] =  0177; // 00b1 plus minus sign
	glyphs[2][2] =  8723; // 2213 minus plus sign
	glyphs[2][3] =  8721; // 2211 summation
	glyphs[2][4] =  8730; // 221a square root
	glyphs[2][5] =  8731; // 221b cube root

	glyphs[2][6] =  8733; // 221d prortional
	glyphs[2][7] =  8734; // 221e infinity
	glyphs[2][8] =  8747; // 222b integral
	glyphs[2][9] =  8748; // 222c double integral
	glyphs[2][10] =  8749; // 222d triple integral
	glyphs[2][11] =  8750; // 222e contour integral

	glyphs[2][12] =  8751; // 222f surface integral
	glyphs[2][13] =  8752; // 2230 volume integral
	glyphs[2][14] =  8771; // 2243 approx
	glyphs[2][15] =  8800; // 2260 not equal
	glyphs[2][16] =  8801; // 2261 indentity
	glyphs[2][17] =  8804; // 2264 L.E.

	glyphs[2][18] =  8805; // 2265 G.E.
	glyphs[2][19] =  8901; // 22c5 dot operator
	glyphs[2][20] =  8706; // 2202 partial diff
	glyphs[2][21] =  8711; // 2207 nabla
	glyphs[2][22] =  8719; // 220f n-Nary product
	glyphs[2][23] =  8721; // 2211 summation.
	glyphs[2][24] =  8804; // 2264 L.E.

	glyphs[3][0] =  L'\u0332'; // // 304 comb macron.
	glyphs[3][1] =  L'\u0305'; //27E8; // 305 comb overline
	glyphs[3][2] =  27E9; // 2202 partial diff
	glyphs[3][3] =  L'\u035f'; // 2207 nabla
	glyphs[3][4] =  0305; // 220f n-Nary product
	glyphs[3][5] =  L'\u0303'; // 2211 summation.
	glyphs[3][6] =  0302; // 2264 L.E.
//	L'\u222b';
	glyphs[3][7] =  L'\u222b';

	glyphs[3][8] =  L'\u0300'; // 2207 nabla
	glyphs[3][9] =  L'\u0301'; // 220f n-Nary product
	glyphs[3][10] =  L'\u0302'; // 2211 summation.
	glyphs[3][11] =  L'\u0304'; // 2264 L.E.
	glyphs[3][12] =  L'\u00a8'; // 2207 nabla
	glyphs[3][13] =  L'\u005f'; // 220f n-Nary product
	glyphs[3][14] =  L'\u00af'; // 2211 summation.
	glyphs[3][15] =  L'\u02c6'; // 2264 L.E.


	glyphs[3][16] =  L'\u02d6'; // 2207 nabla
	glyphs[3][17] =  L'\u02d7'; // 220f n-Nary product
	glyphs[3][18] =  L'\u02d9'; // 2211 summation.
	glyphs[3][19] =  L'\u2202'; // 2264 L.E.
	glyphs[3][20] =  L'\u22c5'; // 2207 nabla
	glyphs[3][21] =  L'\u2a2f'; // 220f n-Nary product
	glyphs[3][22] =  L'\u00b2'; // 2211 summation.
	glyphs[3][23] =  L'\u2500'; // box drawing horiz.
	glyphs[3][23] =  L'\u0308'; // comb. diaeresis
	glyphs[3][24] =  L'\u0307'; // comb. dot above

	glyphs[4][0] =  L'\u2015'; // horiz. bar
	glyphs[4][1] =  L'\u2014'; // Em dash
	glyphs[4][2] =  L'\u2010'; // hyphen
	glyphs[4][3] =  L'\u2011'; // non-breaking hyphen
	glyphs[4][4] =  L'\u2012'; // figure dash
	glyphs[4][5] =  L'\u23af'; // horiz. line extension.
	glyphs[4][6] =  0302; // 2264 L.E.
//	L'\u222b';
	glyphs[4][7] =  L'\u222b';

	glyphs[4][8] =  L'\u0300'; // 2207 nabla
	glyphs[4][9] =  L'\u0301'; // 220f n-Nary product
	glyphs[4][10] =  L'\u0302'; // 2211 summation.
	glyphs[4][11] =  L'\u0304'; // 2264 L.E.
	glyphs[4][12] =  L'\u00a8'; // 2207 nabla
	glyphs[4][13] =  L'\u005f'; // 220f n-Nary product
	glyphs[4][14] =  L'\u00af'; // 2211 summation.
	glyphs[4][15] =  L'\u02c6'; // 2264 L.E.


	glyphs[4][16] =  L'\u02d6'; // 2207 nabla
	glyphs[4][17] =  L'\u02d7'; // 220f n-Nary product
	glyphs[4][18] =  L'\u02d9'; // 2211 summation.
	glyphs[4][19] =  L'\u2202'; // 2264 L.E.
	glyphs[4][20] =  L'\u22c5'; // 2207 nabla
	glyphs[4][21] =  L'\u2a2f'; // 220f n-Nary product
	glyphs[4][22] =  L'\u00b2'; // 2211 summation.
	glyphs[4][23] =  L' '; // 2264 L.E.
	glyphs[4][23] =  L'\u0308'; // comb. diaeresis
	glyphs[4][24] =  L'\u0307'; // comb. dot above

	base.resize(25);
	base[0].s = L'A';
	base[1].s = L'h';
	base[2].s = L'a';
	base[3].s = L'+';
	base[4].s = L'\u23af';
	base[5].s = L'\u23af';
	base[6].s = L' ';
	base[7].s = L' ';
	base[8].s = L' ';
	base[9].s = L'\u2212';
	base[10].s = L' ';
	base[11].s = L'\u23af';
	base[12].s = L'\u23af';
	base[13].s = L'\u23af';
	base[14].s = L'\u23af';
	base[15].s = L'\u23af';
	base[16].s = L'\u23af';
	base[17].s = L'\u23af';
	base[18].s = L'\u23af';
	base[19].s = L' ';
	base[20].s = L' ';
	base[21].s = L' ';
	base[22].s = L' ';
	base[23].s = L' ';
	base[24].s = L'.';

	baseSub.s = L't';
	baseSup.s = L's';
	//base[25]{L'c', L' ', L' ', L'+',L' ',
	//	           L' ',L'\u23af', L'\u23af', L'\u23af', L'\u23af',
	//	           L'\u23af', L'\u23af', L' ', L' ', L'\u2212',
	//	           L' ', L' ', L'\u23af', L'\u23af', L'\u23af',
	//	           L'\u23af', L'\u23af', L'\u23af', L'\u23af', L'\u23af'};

	numDenSup.s = L'2';
	numDen.s = L'b';
	numDenSub.s = L'1';
	denNumSup.s = L'3';
	denNum.s = L'G';
	denNumSub.s = L'y';


}
void Math::Eq_Edit::init_DirectWrite(HWND hWnd){

	handle = hWnd;

	hdc = GetDC(hWnd); 
	GetTextMetrics(hdc, &tm); 
	ReleaseDC(hWnd, hdc);



	fontSize = 32.0f;
	level[7] = fontSize/2.0f;
	level[6] = 0.0f; 
	level[5] = -fontSize/2.0f;

	charCount = 0;
	insertionPoint = pageMargin;
	indexLeft = -1;

	loadGlyphs();

	HRESULT hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory_ );
	
//	UINT DpI(0);
	float dpiScaleX_;
	float dpiScaleY_;

	if (SUCCEEDED(hr)){
		pD2DFactory_->GetDesktopDpi(&dpiScaleX_, &dpiScaleY_);	
//		DpI = GetDpiForWindow(hWnd);
		dpiScaleX_ = 96/dpiScaleX_;
		dpiScaleY_ = 96/dpiScaleY_;
	}

	if (SUCCEEDED(hr)){
		hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&pDWriteFactory_) );
	}



	if (SUCCEEDED(hr)){ 
		hr = pDWriteFactory_->CreateTextFormat( 
			L"Cambria Math",
			NULL, 
			DWRITE_FONT_WEIGHT_SEMI_LIGHT,
		//	DWRITE_FONT_WEIGHT_NORMAL,
		//	DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STYLE_ITALIC,

			DWRITE_FONT_STRETCH_NORMAL,
			fontSize,// font size
			L"en-us",
			&pMathFormat_);

		pMathFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		pMathFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		pMathFormat_->SetIncrementalTabStop(50.0f);
	}


	if (SUCCEEDED(hr)){ 
		hr = pDWriteFactory_->CreateTextFormat( 
			// L"Arial",
			//L"Cambria Math",
			L"Cambria Math",
			NULL, 
			DWRITE_FONT_WEIGHT_SEMI_LIGHT,
		//	DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			0.7f*fontSize,// font size
			L"en-us",
			&pSubSupFormat_);

		pMathFormat_->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_JUSTIFIED);
		pMathFormat_->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		pMathFormat_->SetIncrementalTabStop(50.0f);
	}

}
//struct DWRITE_TEXT_METRICS

    /// Left-most point of formatted text relative to layout box
    /// (excluding any glyph overhang).
//    FLOAT left;
    /// Top-most point of formatted text relative to layout box
    /// (excluding any glyph overhang).
 //   FLOAT top;
    /// The width of the formatted text ignoring trailing whitespace
    /// at the end of each line.
//    FLOAT width;
    /// The width of the formatted text taking into account the
    /// trailing whitespace at the end of each line.
//    FLOAT widthIncludingTrailingWhitespace;
    /// The height of the formatted text. The height of an empty string
    /// is determined by the size of the default font's line height.
//    FLOAT height;
    /// Initial width given to the layout. Depending on whether the text
    /// was wrapped or not, it can be either larger or smaller than the
    /// text content width.
 //   FLOAT layoutWidth;
    /// Initial height given to the layout. Depending on the length of the
    /// text, it may be larger or smaller than the text content height.
 //   FLOAT layoutHeight;
    /// The maximum reordering count of any line of text, used
    /// to calculate the most number of hit-testing boxes needed.
    /// If the layout has no bidirectional text or no text at all,
    /// the minimum level is 1.
//    UINT32 maxBidiReorderingDepth;
    /// Total number of lines.
//    UINT32 lineCount;

// Partition the glyphRect which is
//
// 0.5f*clientWidth  - pageMargin - middleMargin 
// x 0.5f*clientHeight - page margin -middle margin
// Into 10 rows and 25 columns to display upto 250 glyphs
//

void Math::Eq_Edit::displayGlyphs(){

	D2D1_POINT_2F glyphOrigin{0.0f};

	for( int row = 0; row < 10; row++ ){
		for( int col = 0; col < 25; col++){
			glyphOrigin.x = pageMargin + cellWidth*static_cast<float>(col);
			glyphOrigin.y = pageMargin + cellHeight*static_cast<float>(row);

			pDWriteFactory_->CreateTextLayout(
				&glyphs[row][col],
				1,			
				pMathFormat_,
				static_cast<float>(glyphRect.right), // width of layout box
				static_cast<float>(glyphRect.bottom), // width of layout box
				&pMathLayout_ );
			pRT_->DrawTextLayout( glyphOrigin, pMathLayout_, pMathBrush_ );
		}
	}
}

void Math::Eq_Edit::displayIconicEquations(){

	D2D1_POINT_2F modelOrigin{0.0f};
	modelOrigin.x = modelRect.left + pageMargin;
	modelOrigin.y = static_cast<float>(modelRect.bottom)/2.0f;
//	const std::wstring S0 = L"C + \u23af\u23af\u23af  \u2212  \u23af\u23af\u23af\u23af\u23af\u23af\u23af\u23af\u23af";
	//for( int i = 0; i < dragLength; i++ ){
	//	if( i == 0 ) D[i].x = modelRect.left + pageMargin + D[i].w;
	//	else D[i].x = D[i-1].x + D[i].w;
	//	pDWriteFactory_->CreateTextLayout(
	//		&D[i].s,
	//		1,			
	//		pMathFormat_,
	//		450.0f, // width of layout box
	//		100.0f,  // height of layout box
	//		&pMathLayout_ );
	//	// Handle the space bar
	//	modelOrigin.x  = D[i].x - D[i].w;
	//	pRT_->DrawTextLayout(modelOrigin , pMathLayout_, pMathBrush_ );

	//}

////	WCHAR base[25]{L'c', L' ', L' ', L'+',L' ',
//		           L' ',L'\u23af', L'\u23af', L'\u23af', L'\u23af',
//		           L'\u23af', L'\u23af', L' ', L' ', L'\u2212',
//		           L' ', L' ', L'\u23af', L'\u23af', L'\u23af',
//		           L'\u23af', L'\u23af', L'\u23af', L'\u23af', L'\u23af'};

  // origin.x = S[i].x - S[i].w; so S[i].x  is the right most extent of the glyph

// Draw the base run

	D2D1_POINT_2F baseOrigin = modelOrigin;

	for( int i = 0; i < 25; i++ ){
		base[i].w = getGlyphMetrics(base[i].s);
		if( base[i].w == 0.0f ) base[i].w = 16.0f;		
//		Win::log("getGlyphMetrics(glyph) width = %f", width);
		base[i].x = baseOrigin.x + base[i].w;
		pRT_->DrawTextLayout(baseOrigin , pMathLayout_, pMathBrush_ );
		baseOrigin.x += base[i].w;
	}

	D2D1_RECT_F baseRect{}; 
	baseRect.left = modelRect.left;
	baseRect.top = modelOrigin.y;
	baseRect.right = base[0].x;
	baseRect.bottom = baseRect.top + fontSize;

	D2D1_POINT_2F baseLine0{ modelOrigin.x, modelOrigin.y + 0.48f*fontSize};
	D2D1_POINT_2F baseLine1{ base[0].x + 3.0f*fontSize, modelOrigin.y+ 0.48f*fontSize};

	pRT_->DrawRectangle( &baseRect, pMathBrush_, 0.5f, NULL );

	pRT_->DrawLine( baseLine0, baseLine1, pMathBrush_, 0.5f, NULL );


// Draw baseSup
	float supOf = -2.0f;
	D2D1_POINT_2F baseSupOrigin{};
	DWRITE_TEXT_RANGE supRange{};
	supRange.startPosition = 0; supRange.length = 1;
	baseSupOrigin.y = modelOrigin.y + supOf;
	baseSupOrigin.x =  base[0].x + 3.0f;
	baseSup.w = getGlyphMetrics(baseSup.s);
	pMathLayout_->SetFontSize(18.0f, supRange);

	pRT_->DrawTextLayout(baseSupOrigin , pMathLayout_, pMathBrush_ );

	D2D1_RECT_F baseSupRect{}; 
	baseSupRect.left = baseRect.right;
	baseSupRect.top = baseRect.top - fontSize;
	baseSupRect.right = baseSupRect.left + fontSize;;
	baseSupRect.bottom = baseRect.top + 0.67f*fontSize;

	pRT_->DrawRectangle( &baseSupRect, pMathBrush_, 0.5f, NULL );

// Draw baseSub
	float subOf = 23.0f;
	D2D1_POINT_2F baseSubOrigin{};
	DWRITE_TEXT_RANGE subRange{};
	subRange.startPosition = 0; subRange.length = 1;
	baseSubOrigin.y = modelOrigin.y + subOf;
	baseSubOrigin.x =  base[0].x + 3.0f;
	baseSub.w = getGlyphMetrics(baseSub.s);
	pMathLayout_->SetFontSize(18.0f, subRange);

	pRT_->DrawTextLayout(baseSubOrigin , pMathLayout_, pMathBrush_ );

	D2D1_RECT_F baseSubRect{}; 
	baseSubRect.left = baseRect.right;
	baseSubRect.top = baseSupRect.bottom;
	baseSubRect.right = baseSupRect.left + fontSize;;
	baseSubRect.bottom = baseRect.bottom + fontSize;

	pRT_->DrawRectangle( &baseSubRect, pMathBrush_, 0.5f, NULL );



// Draw numDen
	float numDenOf = -23.0f;
	D2D1_POINT_2F numDenOrigin{};
	DWRITE_TEXT_RANGE numDenRange{};
	numDenRange.startPosition = 0; numDenRange.length = 1;
	numDenOrigin.y = modelOrigin.y + numDenOf;
	numDenOrigin.x =  base[3].x;
	numDen.w = getGlyphMetrics(numDen.s);
	pRT_->DrawTextLayout(numDenOrigin , pMathLayout_, pMathBrush_ );

// Draw numDenSup
	float numDenSupOf = -28.0f;
	D2D1_POINT_2F numDenSupOrigin{};
	DWRITE_TEXT_RANGE numDenSupRange{};
	numDenSupRange.startPosition = 0; numDenSupRange.length = 1;
	numDenSupOrigin.y = numDenOrigin.y + supOf;
	numDenSupOrigin.x =  base[4].x;
	numDenSup.w = getGlyphMetrics(numDenSup.s);
	pMathLayout_->SetFontSize(18.0f, numDenSupRange);

	pRT_->DrawTextLayout(numDenSupOrigin , pMathLayout_, pMathBrush_ );

// Draw numDenSup
	float numDenSubOf = 21.0f;
	D2D1_POINT_2F numDenSubOrigin{};
	DWRITE_TEXT_RANGE numDenSubRange{};
	numDenSubRange.startPosition = 0; numDenSubRange.length = 1;
	numDenSubOrigin.y = numDenOrigin.y + subOf;
	numDenSubOrigin.x =  base[4].x;
	numDenSub.w = getGlyphMetrics(numDenSub.s);
	pMathLayout_->SetFontSize(18.0f, numDenSubRange);

	pRT_->DrawTextLayout(numDenSubOrigin , pMathLayout_, pMathBrush_ );


//////////////////
   


}
float Math::Eq_Edit::getGlyphMetrics(WCHAR glyph){
 
   SafeRelease(&pMathLayout_);
   pDWriteFactory_->CreateTextLayout(
      &glyph,
      1,			
      pMathFormat_,
      100.0f, // width of layout box
      100.0f,  // height of layout box
      &pMathLayout_ );

   DWRITE_TEXT_METRICS metrics;
   pMathLayout_->GetMetrics( &metrics);
//   glyphWidth = metrics.width;
   return  metrics.width;
}

void Math::Eq_Edit::draw_Eq(){

	D2D1_POINT_2F origin {0.0f};
	D2D1_RECT_F paperRect{};
	D2D1_SIZE_F rtSize{};

	if (SUCCEEDED(CreateDeviceResources()))
	{
		HRESULT hr = S_OK;
		pRT_->BeginDraw(); 

		rtSize = pRT_->GetSize();
		paperRect.right = rtSize.width; paperRect.bottom = rtSize.height;

		pRT_->FillRectangle(&paperRect, pPaperBrush_);
		SafeRelease(&pMathLayout_);
		displayGlyphs();
		SafeRelease(&pMathLayout_);
		displayIconicEquations();
		SafeRelease(&pMathLayout_);

		origin.x = pageMargin;
		for( int i = 0; i < charCount; i++ ){		
			origin.y = static_cast<float>( currentLevel.top);
			origin.x = S[i].x - S[i].w;
			pDWriteFactory_->CreateTextLayout(
				&S[i].s,
				1,			
				pMathFormat_,
				450.0f, // width of layout box
				100.0f,  // height of layout box
				&pMathLayout_ );
			// Handle the space bar
			pRT_->DrawTextLayout( origin , pMathLayout_, pMathBrush_ );

		}
		SetCaretPos(insertionPoint, currentLevel.top ); 


// Diagnostics paperRect.bottom
		DWRITE_TEXT_METRICS metrics;
		float glyphWidth;
		SafeRelease(&pMathLayout_);
		std::wstring diagnostic{};
		std::wstring f_number{};
		f_number.clear();
		f_number = L"indexLeft =  ";
		f_number.append( std::to_wstring( indexLeft ));
		f_number.append( L"   charCount = ");
		f_number.append( std::to_wstring( charCount ));
		f_number.append( L"   insertionPoint = ");
		f_number.append( std::to_wstring( insertionPoint ));
		if( indexLeft >= 0 ){
		f_number.append( L"   glyph =");
		f_number.append( std::to_wstring( glyph));

		//f_number.append( L"   S[indexLeft].x =");
		//f_number.append( std::to_wstring( S[indexLeft].x));
		f_number.append( L"   S[indexLeft].w =");
		f_number.append( std::to_wstring( S[indexLeft].w));}

	//	float f_debug{};
		origin.x = 0.0f;
		origin.y = static_cast<float>(equationRect.bottom) - 35.0f;
	//	diagnostic = L"Hello";
		pDWriteFactory_->CreateTextLayout(
			(PWSTR)f_number.c_str(),
			(UINT32)lstrlen((PWSTR)f_number.c_str()),			
			pMathFormat_,
			paperRect.right, // width of layout box
			100.0f,  // height of layout box
			&pMathLayout_ );

		pRT_->DrawTextLayout( origin , pMathLayout_, pMathBrush_ );

//  Draw the Drag Bar

		D2D1_POINT_2F point0;
		D2D1_POINT_2F point1;

		point0.x = insertionPoint; 
		point0.y = point1.y = currentLevel.top;

		if( dragging ){

			if( dragStart >= 0 && charCount > 0)
				point1.x = S[dragStart].x;
			else point1.x = pageMargin;
				
			pRT_->DrawLine( point0, point1,
				pMathBrush_,
				1.0f,
				NULL );
		}

	if (D2DERR_RECREATE_TARGET == pRT_->EndDraw()) DiscardDeviceResources();
	} //End if (SUCCEEDED( CreateDeviceResources())
	return;
}

HRESULT Math::Eq_Edit::CreateDeviceResources(){
    HRESULT hr = S_OK;
	if(!pRT_){

		RECT rc;
		GetClientRect(handle, &rc);
		D2D1_SIZE_U Usize = D2D1::SizeU((rc.right - rc.left ),
										(rc.bottom - rc.top ));
		hr = pD2DFactory_->CreateHwndRenderTarget( 
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties( handle, Usize ), &pRT_ );

		pRT_->CreateSolidColorBrush(D2D1::ColorF(0xF4B480),  &pMathBrush_ );
//		pRT_->CreateSolidColorBrush(D2D1::ColorF(0x2F4F4F),  &pPaperBrush_ );
		pRT_->CreateSolidColorBrush(D2D1::ColorF(0x000000),  &pPaperBrush_ );
	}

	if( FAILED( hr )) return hr;

    return S_OK;
}


void Math::Eq_Edit::DiscardDeviceResources(){
	
	SafeRelease(&pPaperBrush_);
    SafeRelease(&pMathBrush_);
    SafeRelease(&pRT_);

}
void Math::Eq_Edit::wmChar(WPARAM wParam){
#ifdef DEBUG_GB
	Win::log(L"in wmChar indexLeft = %i", indexLeft );
#endif

	  
    bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;
	if (heldControl) return;
	switch (wParam) 
    { 
		case 0x08:  // backspace
			if( charCount == 0 || indexLeft == -1 ) MessageBeep((UINT) -1);
			else{
				deleteGlyph();
			    InvalidateRect(handle, NULL, FALSE);
			}
			break;
		//case 'C':
		//	Win::log(L"wmChar case 'C' indexLeft = %i", indexLeft );
		//	if (heldControl) break;
  //         // copyToClipboard();
		//case 'X':
		//	Win::log(L"wmChar case 'X' indexLeft = %i", indexLeft );
		//	if (heldControl){
		//		Win::log(L"wmChar case 'X' if (heldControl) = TRUE" );
		//		break;
		//	}

		case 0x0A:  // linefeed 
			break;
		case 0x1B:  // escape 
			MessageBeep((UINT) -1); 
			return; 
		case 0x09:  // tab 
			// Convert tabs to four consecutive spaces. 
			//for (i = 0; i < 4; i++) 
			//	SendMessage(hwndMain, WM_CHAR, 0x20, 0); 
			//return 0; 
			break;
		case 0x0D:  // carriage return 
 
            // Record the carriage return and position the 
            // caret at the beginning of the new line.

            //pchInputBuf[cch++] = 0x0D; 
            //nCaretPosX = 0; 
            //nCaretPosY += 1; 
            break;

// displayable character 
        default:   
			Win::log(L"in wmChar default CALLING setEqStringMetrics" );
			setEqStringMetrics( (WCHAR)wParam );
			InvalidateRect(handle, NULL, FALSE);
			break; 
    } 
}

// Handles caret navigation and special presses that
// do not generate characters.
void Math::Eq_Edit::keyDown( WPARAM wParam){
#ifdef DEBUG_GB
	Win::log(L"in keyDown linkIndex = %i", indexLeft );
#endif

    bool heldShift   = (GetKeyState(VK_SHIFT)   & 0x80) != 0;
    bool heldControl = (GetKeyState(VK_CONTROL) & 0x80) != 0;

    switch (wParam) 
    { 
        case VK_LEFT:   // LEFT ARROW 

			//if( charCount == 0 || indexLeft == -1 ) MessageBeep((UINT) -1);
			//else{
			//	moveCaret(true);
			//    InvalidateRect(handle, NULL, FALSE);
			//}
			//break;

			if( indexLeft  <= 0 ){
				insertionPoint = pageMargin;
				indexLeft = -1;
				MessageBeep((UINT) -1);
				}

			else{
				--indexLeft;
				insertionPoint = S[indexLeft].x;
			    
			}
	Win::log(L"VK_LEFT linkIndex = %i", indexLeft );
			SetCaretPos( insertionPoint, currentLevel.top);
			InvalidateRect(handle, NULL, FALSE);
 
            break; 
 
        case VK_RIGHT:  // RIGHT ARROW 

			if( indexLeft == charCount -1 ){
				MessageBeep((UINT) -1 );
				break;}

			insertionPoint = S[++indexLeft].x;
			SetCaretPos( insertionPoint, currentLevel.top);
	Win::log(L"VK_RIGHT linkIndex = %i", indexLeft );
 
  
            break; 
 		case 'C':
			Win::log(L"keyDown case 'C' linkIndex = %i", indexLeft );
			if (heldControl)
            copyToClipboard();
			break;
		case 'X':
			Win::log(L"keyDown case 'X' linkIndex = %i", indexLeft );
			if (heldControl)
			{
				cutTheDrag();
			}
			break;
		case 'V':
			Win::log(L"keyDown case 'X' linkIndex = %i", indexLeft );
			if (heldControl)
			{
				pasteTheDrag();
			}
			break;
        case VK_UP:     // UP ARROW 
			break;
        case VK_DOWN:   // DOWN ARROW 
            //MessageBeep((UINT) -1); 
            //return 0; 
			break;
        case VK_HOME:   // HOME 

			indexLeft = -1;
			insertionPoint = pageMargin;
			SetCaretPos( insertionPoint, currentLevel.top);
 
            // Set the caret's position to the upper left 
            // corner of the client area. 
 
            //nCaretPosX = nCaretPosY = 0; 
            //nCurChar = 0; 
            break; 
 
        case VK_END:    // END 

			if( indexLeft ==  -1 && charCount == 0){
				MessageBeep((UINT) -1 );
				break;}

			indexLeft = charCount-1;
			insertionPoint = S[indexLeft].x;
			SetCaretPos( insertionPoint, currentLevel.top);
			break;
 
            // Move the caret to the end of the text. 
 
            //for (i=0; i < cch; i++) 
            //{ 
            //    // Count the carriage returns and save the 
            //    // index of the last one. 
 
            //    if (pchInputBuf[i] == 0x0D) 
            //    { 
            //        cCR++; 
            //        nCRIndex = i + 1; 
            //    } 
            //} 
            //nCaretPosY = cCR; 
 
            //// Copy all text between the last carriage 
            //// return and the end of the keyboard input 
            //// buffer to a temporary buffer. 
 
            //for (i = nCRIndex, j = 0; i < cch; i++, j++) 
            //    szBuf[j] = pchInputBuf[i]; 
            //szBuf[j] = TEXT('\0'); 
 
            //// Retrieve the text extent and use it 
            //// to set the horizontal position of the 
            //// caret. 
 
            //hdc = GetDC(hwndMain);
            //hResult = StringCchLength(szBuf, 128, pcch);
            //if (FAILED(hResult))
            //{
            //// TODO: write error handler
            //} 
            //GetTextExtentPoint32(hdc, szBuf, *pcch, 
            //    &sz); 
            //nCaretPosX = sz.cx; 
            //ReleaseDC(hwndMain, hdc); 
            //nCurChar = cch; 
             
 
		default: 
            break; 
    } 
            //SetCaretPos(nCaretPosX, nCaretPosY * dwCharY); 
}

void Math::Eq_Edit::copyToClipboard(){

Win::log(L"copyToClipboard() dragLength = %i", dragLength );
    if (dragLength == 0)
        return;

    if (OpenClipboard(handle))
    {
        if (EmptyClipboard())
        {
            // Allocate room for the text
         //   size_t byteSize         = sizeof(wchar_t) * (dragLength + 1);

		  size_t byteSize         = sizeof(wchar_t) * (4 + 1);

            HGLOBAL hClipboardData  = GlobalAlloc(GMEM_DDESHARE | GMEM_ZEROINIT, byteSize);

            if (hClipboardData != NULL)
            {
                void* memory = GlobalLock(hClipboardData);  // [byteSize] in bytes

                if (memory != NULL)
                {
                    // Copy text to memory block.
					std::wstring hello = L"Hello World";
					const wchar_t* text = hello.c_str();
 
                //    const wchar_t* text = text_.c_str();
                //   memcpy(memory, &hello_.c_str(), byteSize);
                  memcpy(memory, &text[0], byteSize);

            //        memcpy(memory, &text[selectionRange.startPosition], byteSize);
                    GlobalUnlock(hClipboardData);

                    if (SetClipboardData(CF_UNICODETEXT, hClipboardData) != NULL)
                    {
                        hClipboardData = NULL; // system now owns the clipboard, so don't touch it.
                    }
                }
                GlobalFree(hClipboardData); // free if failed
            }
        }
        CloseClipboard();
    }
}

//  Snaps the IP and indexLeft to the left or right side of a glyph.
int Math::Eq_Edit::glyphSelection(){

	if( X < S[0].x ){
		if( X > pageMargin + S[0].w/2.0f){
			insertionPoint = S[0].x;
			indexLeft = 0;			
		}
		else{ 
			insertionPoint = pageMargin;
			indexLeft = -1;
		}
	}

	else if( X > S[charCount - 1].x ){
		insertionPoint = S[charCount - 1].x;
		indexLeft = charCount - 1;

	}		
	else{
		for( int i = 1; i < charCount; i++ ){
			float space = (S[ i ].x - S[i-1].x)/2.0f;
			if( X >= S[i-1].x  &&  X <= S[i-1].x  + space ){
				insertionPoint = S[i-1].x;
				indexLeft = i-1;
				break;
			}
			else if( X >= S[i-1].x + space && X <= S[i].x ){
				insertionPoint = S[i].x;
				indexLeft = i;
				break;
			}
		}
	}
	Win::log(L"glyphSelection indexLeft = %i", indexLeft );
	SetCaretPos( insertionPoint, currentLevel.top);
	InvalidateRect(handle, NULL, FALSE);
	return indexLeft;
}



//  On lButtonUp reads the drag into EquationString D[]. 
void Math::Eq_Edit::readTheDrag(){
	Win::log(L"readTheDrag() indexLeft = %i", indexLeft );
	dragLength = indexLeft - dragStart;

	if( dragLength < 0 ) dragLength  = -dragLength;

	D.clear();
	D.resize(dragLength);

	std::wstring x;

	if( dragStart < indexLeft ){
		for( int i = 0; i < dragLength; i++ ){
			D[i].s =  S[i + dragStart + 1].s;
			D[i].w =  S[i + dragStart + 1].w;
			x.clear();
			x = D[i].s;
			Win::log( x);
		} 
	}

	else{
		for( int i = 0; i < dragLength; i++ ){
			D[i].s =  S[i + indexLeft + 1].s;
			D[i].w =  S[i + indexLeft + 1].w;
			x.clear();
			x = D[i].s;
			Win::log( x);
		} 
	}

}

//  User has entered ctrl-V;  the dragGlyphs are already
//  in D[ 0 , dragLength - 1 ] after button up.
//  Reset charCount and S.size(). += dragLength;
//  Move any glyphs to right of IP.
//  Paste the dragGlyphs in the hole to right of IP.
void Math::Eq_Edit::pasteTheDrag(){

	int number_of_glyphs_to_move = charCount - indexLeft - 1;

	charCount += dragLength;
	S.resize(charCount);

//  Make a hole for the dragGlyphs.
	for( int i = 0; i <  number_of_glyphs_to_move; i++ ){ 
		S[indexLeft + dragLength + 1 + i].s = S[indexLeft + 1 + i].s;
		S[indexLeft + dragLength + 1 + i].w = S[indexLeft + 1 + i].w;
	}

//  Put the dragGlyphs in the hole.
	for( int i = 0; i < dragLength; i++ ){  
		S[indexLeft + 1 + i].s = D[i].s;
		S[indexLeft + 1 + i].w = D[i].w;
	}

//  Now we can set the glyph.x's from indexLeft + 1 to charCount -1
	for( int i = indexLeft + 1; i < charCount; i++ ){  
		 if( i == 0 ){S[i].x = pageMargin + S[i].w;}
		 else S[i].x = S[i-1].x + S[i].w;
	}

//  Put the IP at the end of the paste
	indexLeft += dragLength;
	insertionPoint = S[indexLeft].x;
	InvalidateRect(handle, NULL, FALSE);
}

//  User has entered ctrl-X;  the dragGlyphs are already
//  in D[ 0 , dragLength - 1 ] after button up.
//  Remove the dragGlyphs and move the remaing glyphs.
//  Reset charCount and S.size().
void Math::Eq_Edit::cutTheDrag(){

//  Assuming a right drag here.
	int number_of_glyphs_to_move = charCount - indexLeft - 1;
	int start = dragStart + 1;

//  ? Is it really a right drag ?
	if( dragStart > indexLeft ){
		number_of_glyphs_to_move = charCount - dragStart - 1;
		start = indexLeft + 1;
	}

//  Move the glyphs.
	for( int i = 0; i < number_of_glyphs_to_move;  i++ ){
		S[start + i].s = S[start + i + dragLength].s;
		S[start + i].w = S[start + i + dragLength].w;
		if( start + i == 0 ) S[start + i].x = pageMargin + S[start + i].w;
		else  S[start + i].x = S[start + i -1 ].x + S[start + i].w;
	}

	charCount -= dragLength;
	S.resize(charCount);

//  ? Drag right ?
	if( dragStart < indexLeft ){
		indexLeft = dragStart;
		if( dragStart == -1) insertionPoint = pageMargin;
		else insertionPoint = S[dragStart].x;
	}

//  Must be a drag left.
	else{
		if( indexLeft == -1) insertionPoint = pageMargin;
		else insertionPoint = S[indexLeft].x;
	}

	InvalidateRect(handle, NULL, FALSE);
}


void Math::Eq_Edit::lButtonDown( WPARAM wParam, LPARAM lParam ){
	Win::log(L"in lButtonDown linkIndex = %i", indexLeft );
	RECT eqLine{};
	eqLine.left = pageMargin; eqLine.right = equationRect.right;
	eqLine.top = currentLevel.top; eqLine.bottom = currentLevel.top + fontSize;

	POINT sP;  // sP for start point.
	GetCursorPos(&sP);
    ScreenToClient(handle, &sP);

// ? Mouse in glyphBox
 	if( PtInRect( &glyphBox, sP )){
		int row{}; int col{};
		row = ( sP.y - pageMargin )/cellHeight;
		col = ( sP.x - pageMargin )/cellWidth;
		if(glyphs[row][col]){
			glyph = glyphs[row][col];
			setEqStringMetrics( glyph );
			InvalidateRect(handle, NULL, FALSE);
		}
	}

// ? Mouse in modelBox
	else if( PtInRect(&modelRect, sP )){
		int modelX = sP.x;
	}

// Is Mouse in equation level?   
	else if( PtInRect(&eqLine, sP )){
		if(charCount == 0 ) return;
		dragging = true;
		X = static_cast<float>(sP.x);

		// get indexLeft of drag start.
		dragStart = glyphSelection();
	}

	Win::log(L"leaving lButtonDown linkIndex = %i", indexLeft );


}
//  Just determines where the cursor is until lButtonUp.
void Math::Eq_Edit::mouseMove(WPARAM wParam, LPARAM lParam){

	if( dragging ){

		RECT eqLine{};
		eqLine.left = pageMargin; eqLine.right = equationRect.right;
		eqLine.top = currentLevel.top; eqLine.bottom = currentLevel.top + fontSize;

		POINT mP;  // mP for moving point.
		GetCursorPos(&mP);
        ScreenToClient(handle, &mP);

//  Is Mouse in equation level?
		if( PtInRect(&eqLine, mP )){
			X = static_cast<float>(mP.x);
			// Snap to an IP and set indexLeft;  
			glyphSelection();

		} // if( PtInRect(&eqLine, mP )){
	} // End if( dragging ){
}

//  Resets drag flag.
//  If there is a drag, calls readTheDrag().
void  Math::Eq_Edit::lButtonUp( WPARAM wParam, LPARAM lParam ){
	
	if( dragging ){
		dragging = false;
	//	dragLength = indexLeft - dragStart;
		Win::log(L"lButtonUp dragStart = %i", dragStart );
		Win::log(L"lButtonUp indexLeft = %i", indexLeft );
		if( indexLeft != dragStart) readTheDrag();
	}
}

void Math::Eq_Edit::wmSize( int w, int h){

	HideCaret(handle);
	if (pRT_){		
		if(FAILED( pRT_->Resize(D2D1::SizeU(w, h)))){
			DiscardDeviceResources();
			Win::log(L"FAILED( pRT_->Resize");
			return; 
		}
	}

    ::GetClientRect( handle, &clientRect );
	 
	// these are all ints or long ints
    clientWidth = clientRect.right - clientRect.left;
	clientHeight = clientRect.bottom - clientRect.top;

	glyphRect.left = 0;	glyphRect.right = 0.5f*clientWidth;
	glyphRect.top  = 0;	glyphRect.bottom = 0.5f*clientHeight;

	glyphBox.left = glyphBox.top = pageMargin; 
	glyphBox.right = 0.5f*clientWidth -  middleMargin;
	glyphBox.bottom  = glyphRect.bottom;

	cellWidth = (glyphBox.right- glyphBox.left )/25.0f;
	cellHeight = 1.5f*cellWidth;

	modelRect.left = glyphRect.right;	modelRect.right = clientWidth;
	modelRect.top  = 0;	modelRect.bottom = glyphRect.bottom;

	equationRect.left = 0;	equationRect.right = clientWidth;
	equationRect.top  = modelRect.bottom;	equationRect.bottom = clientHeight;

	groundLevel = 0.55*clientHeight;

	currentLevel.top = groundLevel;
	SetCaretPos( insertionPoint, currentLevel.top); 
	ShowCaret(handle); 
}

void Math::Eq_Edit::wmSetFocus(){
	CreateCaret(handle, NULL, 0, fontSize); 
 	SetCaretPos(insertionPoint, currentLevel.top); 
	ShowCaret(handle); 
}

/*
void Math::Eq_Edit::moveCaret( bool left ){
	Win::log(L"in moveCaret indexLeft = %i", indexLeft );

	glyph = S[indexLeft].s;

 	if( glyph == 32 ) glyphWidth = 0.5f*fontSize;
	    
	else getGlyphMerics();

	//   A|   Handle this case
	if( charCount == 1 ){ 		
		indexLeft = -1;
		insertionPoint = pageMargin;
		SetCaretPos( insertionPoint, currentLevel.top); 
		return;
	}

	--indexLeft; 

	if( indexLeft == -1 ) insertionPoint = pageMargin;
	else insertionPoint -= glyphWidth;
	SetCaretPos( insertionPoint, currentLevel.top);

}
*/


//  Deletes a single glyph, the indexLeft = -1 case is delt with
//  in wmChar.
void  Math::Eq_Edit::deleteGlyph(){

	Win::log(L"in deleteGlyph indexLeft = %i", indexLeft );


	glyph = S[indexLeft].s;

 	if( glyph == 32 ) glyphWidth = 0.5f*fontSize;
	    
	else glyphWidth = getGlyphMetrics(glyph);

	//   A|   Handle this case
	if( charCount == 1 ){
		charCount = 0;		indexLeft = -1;
		insertionPoint = pageMargin;
		S.resize(0);
		return;
	}

	--charCount;

	for( int i = indexLeft; i < charCount;  i++){
		S[i].s = S[i+1].s;
		S[i].w = S[i+1].w;
		S[i].x = S[i+1].x - glyphWidth;
    }

	--indexLeft; 
    S.resize(charCount);

	if( indexLeft == -1 ) insertionPoint = pageMargin;
	else insertionPoint -= glyphWidth;
}

//  Gets metrics for a single glyph and reads them into S[].
//  Sets indexLeft, IP, charCount, S.size().
void Math::Eq_Edit::setEqStringMetrics(WCHAR newGlyph){
	Win::log(L"in setEqStringMetrics indexLeft = %i", indexLeft );

	glyph = newGlyph;

 	if( glyph == 32 ){
		glyphWidth = 0.5f*fontSize;
	//	glyph = 8193;
	}
 
	else glyphWidth = getGlyphMetrics(glyph);
	
//	? IP at the end line 
	if( indexLeft == charCount - 1 ){ 
		++charCount;                        
		S.resize(charCount);       
		S[charCount-1].s = glyph;            
		S[charCount-1].w = glyphWidth;       
		insertionPoint += glyphWidth;                    
		S[charCount-1].x = insertionPoint;		    
		++indexLeft;                                
		return; 
	} 

//  Ip is "in" the line.
	S.resize(charCount + 1); 
		for( int i = charCount; i > indexLeft + 1 ; i--){
			S[i].s = S[i-1].s;
			S[i].w = S[i-1].w;
			S[i].x = S[i-1].x + glyphWidth;
		}

  ++indexLeft; ++charCount; 

//  Put that new glyph in the run
	S[indexLeft].s = glyph;          
	S[indexLeft].w = glyphWidth;
	if( indexLeft == 0 ){
		S[indexLeft].x = pageMargin + glyphWidth;
		insertionPoint = S[indexLeft].x;
	}
	else{
		insertionPoint += glyphWidth;	   
		S[indexLeft].x = S[indexLeft-1].x + glyphWidth;
	}

   	Win::log(L"leaving setEqStringMetrics indexLeft = %i", indexLeft );
}    


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case WM_SIZE:
		Win::log(L"case WM_SIZE: wParam = %i", wParam );
	//	Win::log("in winProc rcvd WM_SIZE");
	//  Windows will send this when we resize, max or min the window.
	//	ViewDW will send this when changing the graphics mode or the OGL routine.
	//  The graphics mode will be in wParam GL_MODE = 40007, DW_MODE = 40012
    //  returnValue = ctrl->size(LOWORD(lParam), HIWORD(lParam), GL_WIN);
								// x			y                mode
		editor.wmSize(LOWORD(lParam), HIWORD(lParam));   
        break;

    case WM_PAINT:
	{
		Win::log(L"case WM_PAINT: wParam = %i", wParam );

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Add any drawing code that uses hdc here...
		editor.draw_Eq();
        EndPaint(hWnd, &ps);
	}
	break;
	// ANY key press will sen this msg. not so for WM_CHAR
	case WM_KEYDOWN:
	{   

		Win::log(L"case WM_KEYDOWN: wParam = %i", wParam );
		editor.keyDown( wParam);
		break;
	}
 
	// Mostly glyphs, space, backspace, 
	case WM_KEYUP:
	{
	//	editor.keyDown( wParam);
		break;
	}

	case WM_CHAR:
	{
		Win::log(L"case WM_CHAR: wParam = %i", wParam );

		editor.wmChar( wParam);
		break;
	}
	case WM_SETFOCUS: 
	{
		// Create, position, and display the caret when the 
        // window receives the keyboard focus. 

		editor.wmSetFocus();
			//CreateCaret(hWnd, (HBITMAP) 1, 0, 20); 
			//SetCaretPos(lineEnd, 0); 
			//ShowCaret(hWnd); 
        break; 
	}
	case WM_KILLFOCUS: 
	{ 
		// Hide and destroy the caret when the window loses the 
        // keyboard focus. 
 
        HideCaret(hWnd); 
        DestroyCaret(); 
        break; 
	}
	case WM_LBUTTONDOWN:

		editor.lButtonDown( wParam, lParam);
    break;
	case WM_MOUSEMOVE:

		editor.mouseMove( wParam, lParam);
    break;
	case WM_LBUTTONUP:

		editor.lButtonUp( wParam, lParam);
    break;

    case WM_DESTROY:
        DestroyCaret(); 
        PostQuitMessage(0);
		editor.DiscardDeviceResources();
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
