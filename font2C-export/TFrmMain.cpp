#include <vcl.h>
#include <tchar.h>
#pragma hdrstop

#include <System.IOUtils.hpp>
#include <System.Character.hpp>
#include <System.RegularExpressions.hpp>
#include "TFrmMain.h"
#include <utility>
#include <esgui/bmp2C-export/EsDibUtilities.h>

// use gif support
#include <gifimg.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "cxContainer"
#pragma link "cxControls"
#pragma link "cxDropDownEdit"
#pragma link "cxEdit"
#pragma link "cxFontNameComboBox"
#pragma link "cxGraphics"
#pragma link "cxLookAndFeelPainters"
#pragma link "cxLookAndFeels"
#pragma link "cxMaskEdit"
#pragma link "cxSpinEdit"
#pragma link "cxTextEdit"
#pragma link "dxSkinsCore"
#pragma link "dxSkinsDefaultPainters"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------

TFrmMain *FrmMain;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

__fastcall TFrmMain::TFrmMain(TComponent* Owner) :
TForm(Owner),
m_fontSmall(new TFont),
m_fontSaver(new TFont),
m_height(-1),
m_spaceWidth(4),
m_nullWidth(4),
m_symPageStart(0),
m_symPageEnd(0),
m_recentSymbol(static_cast<esU32>(-1))
{
	m_fontSmall->Name = "Arial";
	m_fontSmall->Height = 12;

  cbxBitDepth_->ItemIndex = 0;

  charMap_->DefaultColWidth = defaultCharmapColWidth;
  charMap_->DefaultRowHeight = defaultCharmapRowHeight;
	charMap_->Canvas->Font->Height = presentationFontHeight;
  charMap_->Canvas->Font->Charset = DEFAULT_CHARSET;
	edHeight_->Value = defaultFontHeight;

	cbxFonts_->FontName = charMap_->Canvas->Font->Name;
  cbxFonts_Change(nullptr);

  unicodePagesSelectorInit();
  cbxUnicodeScripts_->ItemIndex = 0;
  cbxUnicodeScripts_Change(nullptr);
}
//---------------------------------------------------------------------------

// Generate Unicode script IDs enum
#define ES_UNICODE_SCRIPTMAP_BEGIN enum {
#define ES_UNICODE_SCRIPTMAP_ENTRY(Id, Start, End, Name) esUnicodeScript_## Id,
#define ES_UNICODE_SCRIPTMAP_END esUnicodeScript_Cnt };
#include "EsUnicodeScriptMap.cxx"

// Generate Unicode script IDs map
#define ES_UNICODE_SCRIPTMAP_BEGIN static const struct { esU32 start; esU32 end; const char* name; } sc_unicodeScriptMap[esUnicodeScript_Cnt] = {
#define ES_UNICODE_SCRIPTMAP_ENTRY(Id, Start, End, Name) {Start, End, Name},
#define ES_UNICODE_SCRIPTMAP_END };
#include "EsUnicodeScriptMap.cxx"
//---------------------------------------------------------------------------

void TFrmMain::unicodePagesSelectorInit()
{
  try
  {
    cbxUnicodeScripts_->Items->BeginUpdate();

    for(int id = esUnicodeScript_Basic_Latin; id < esUnicodeScript_Cnt; ++id)
      cbxUnicodeScripts_->Items->Add( sc_unicodeScriptMap[id].name );
  }
  __finally
  {
    cbxUnicodeScripts_->Items->EndUpdate();
  }
}
//---------------------------------------------------------------------------

void TFrmMain::symbolPageSet(int scriptId)
{
  m_symPageStart = sc_unicodeScriptMap[ scriptId ].start;
  m_symPageEnd = sc_unicodeScriptMap[ scriptId ].end;
  m_recentSymbol = static_cast<esU32>(-1);

 	m_selection.clear();
  m_selection.resize(
    m_symPageEnd-m_symPageStart,
    true
  );

  if( esUnicodeScript_Basic_Latin == scriptId )
    m_selection[0] = false; // Deselect space by default

  charMapLayoutRecalc();
  charMap_->Invalidate();
}
//---------------------------------------------------------------------------

void TFrmMain::charMapLayoutRecalc()
{
  charMap_->ColCount = charMap_->ClientWidth / charMap_->DefaultColWidth;
  charMap_->RowCount = (m_selection.size() / charMap_->ColCount) +
    ((m_selection.size() % charMap_->ColCount) ? 1 : 0);
}
//---------------------------------------------------------------------------

bool TFrmMain::isSelected(esU32 symbol) const
{
  if( symbol >= m_symPageEnd )
    return false;

  return m_selection[symbol-m_symPageStart];
}
//---------------------------------------------------------------------------

void TFrmMain::spaceAndNullWidthUpdate()
{
  TFont* font = charMap_->Canvas->Font;
  int oldHeight = font->Height;
  font->Height = edHeight_->Value;

  csSpaceWidth_->Value = charMap_->Canvas->TextExtent(" ").cx;
  csNullWidth_->Value = charMap_->Canvas->TextExtent("X").cx;

  font->Height = oldHeight;
}
//---------------------------------------------------------------------------

void TFrmMain::symbolCellDraw(TCanvas* canvas, const TRect& rect, bool selected, bool focused, bool drawGrid, esU32 symbol)
{
	if( !IsRectEmpty( rect ) )
	{
  	TRect r = rect;
		TColor color;

    if( symbol >= m_symPageEnd ) // Not in the currently selected page
    {
      canvas->Brush->Color = clMedGray;
			canvas->Pen->Color = clRed;
      canvas->Pen->Style = TPenStyle::psDot;
  		canvas->FillRect( r );
      canvas->MoveTo(
        r.Left,
        r.Top
      );
      canvas->LineTo(
        r.Right,
        r.Bottom
      );
      canvas->MoveTo(
        r.Left,
        r.Bottom
      );
      canvas->LineTo(
        r.Right,
        r.Top
      );

      canvas->Pen->Style = TPenStyle::psSolid;
      return;
    }

		// draw selection
		if( selected )
			color = clInfoBk;
		else
			color = clWhite;
		canvas->Brush->Color = color;
		canvas->FillRect( r );

		// draw focus rect
		if( focused )
		{
			canvas->Pen->Color = clLime;
			canvas->Rectangle(
        r.left,
        r.top,
        r.right,
        r.bottom
      );
		}

		InflateRect(
      &r,
      -2,
      -2
    );

		// format symbol hex value
    UnicodeString str = UnicodeString::Format(
  		"0x%0.8X",
      ARRAYOFCONST((
        symbol
      ))
    );

		// save current font
		m_fontSaver->Assign( canvas->Font );
		// set small font
		canvas->Font->Assign( m_fontSmall.get() );
		canvas->Font->Color = clBlack;

		// draw char info
		//
		TSize ext = canvas->TextExtent(str);
		canvas->TextOut(
      r.left + (r.Width() - ext.cx) / 2,
      r.top,
      str
    );

		// restore current font
		canvas->Font->Assign( m_fontSaver.get() );
		canvas->Font->Color = clBlack;

    // draw generic canvas rectangle
		r.top += ext.cy + 2;
		canvas->Pen->Color = clDkGray;
		canvas->Rectangle(r);

		// draw single character
		str = TCharacter::ConvertFromUtf32( symbol );
		ext = canvas->TextExtent( str );

		r.left = r.left + (r.Width() - ext.cx) / 2;
		r.top = r.top + (r.Height() - ext.cy) / 2;
		r.right = r.left + ext.cx;
		r.bottom = r.top + ext.cy;

		// draw char cell frame
		canvas->Brush->Color = clLtGray;
		canvas->FrameRect(r);

		// draw char
		canvas->Brush->Style = bsClear;
		canvas->TextOut(
      r.Left,
      r.Top,
      str
    );

    // draw grid
    if( drawGrid )
    {
      int requestedHeight = edHeight_->Value;
      int presentationHeight = canvas->Font->Height;

      TColorRec rec;
      rec.Color = TColorRec::Lightgray;
      rec.A = 64;

      canvas->Pen->Color = rec.Color;
      canvas->Pen->Style = TPenStyle::psDot;
      canvas->Pen->Width = 1;

      int pos = 1;
      while(pos < requestedHeight)
      {
        int y = (presentationHeight * pos)/requestedHeight;

			  canvas->MoveTo(r.left, r.top+y);
			  canvas->LineTo(r.right, r.top+y);

        ++pos;
      }

      int oldHeight = canvas->Font->Height;
      canvas->Font->Height = requestedHeight;
      int width = canvas->TextExtent( str ).cx;
      canvas->Font->Height = oldHeight;

      if( 2 < width )
      {
        pos = 1;
        while(pos < width)
        {
          int x = (ext.cx * pos)/width;

          canvas->MoveTo(r.left+x, r.top);
          canvas->LineTo(r.left+x, r.bottom);

          ++pos;
        }
      }

      canvas->Pen->Style = TPenStyle::psSolid;
    }

		// draw top and bottom cutoff
		int cutoff = edCropTop_->Value;
		if( 0 != cutoff )
		{
			canvas->Pen->Color = clFuchsia;
			// scale coordinates to the requested font height
			int y = (canvas->Font->Height * cutoff)/ edHeight_->Value;
			canvas->MoveTo(r.left, r.top+y);
			canvas->LineTo(r.right, r.top+y);
		}

		cutoff = edCropBottom_->Value;
		if( 0 != cutoff )
		{
			canvas->Pen->Color = clBlue;
			// scale coordinates to the requested font height
			int y = (canvas->Font->Height * cutoff)/ edHeight_->Value;
			canvas->MoveTo(r.left, r.bottom-y);
			canvas->LineTo(r.right, r.bottom-y);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::charMap_DrawCell(TObject *Sender, int ACol, int ARow,
			TRect &Rect, TGridDrawState State)
{
  esU32 symbol = m_symPageStart + ACol + ARow*charMap_->ColCount;
  symbolCellDraw(
    charMap_->Canvas,
    Rect,
    isSelected(symbol),
    State.Contains( gdFocused ),
    false, //< Do not draw grid
    symbol
  );
}
//---------------------------------------------------------------------------

void TFrmMain::cFontNameUpdate()
{
	UnicodeString fontName = cbxFonts_->FontName;
	fontName += static_cast<UnicodeString>( m_height ) + "_";
  fontName += sc_unicodeScriptMap[cbxUnicodeScripts_->ItemIndex+esUnicodeScript_Basic_Latin].name;
  fontName += "_";

  int clrfmt = bitDepthToClrFmt();
  switch(clrfmt)
  {
  case esguiClrfmtGrayscale2:
    fontName += "2bpp";
    break;
  case esguiClrfmtGrayscale4:
    fontName += "4bpp";
    break;
  case esguiClrfmtGrayscale8:
    fontName += "8bpp";
    break;
  default:
    fontName += "1bpp";
    break;
  }

  fontName = fontName.LowerCase();

  fontName = TRegEx::Replace(fontName, "[~`\"<>\\{\\}\\|\\(\\)%$@!#\\+\\-\\.,\\&\\s\\*^\\?\\:]+", "_");
	edFontCName_->Text = fontName;
}
//---------------------------------------------------------------------------

void TFrmMain::fontViewUpdate()
{
  if( charMap_->Visible )
    charMap_->Repaint();
  else
    pbxCloseup_->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbxFonts_Change(TObject *Sender)
{
	UnicodeString fontName = cbxFonts_->FontName;
	TFont* font = charMap_->Canvas->Font;
	if( font->Name != fontName )
	{
		font->Name = fontName;
    pbxCloseup_->Canvas->Font->Name = fontName;

    fontViewUpdate();
		cFontNameUpdate();
    spaceAndNullWidthUpdate();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::chkBold_Click(TObject *Sender)
{
	if( chkBold_->Checked )
		charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style << fsBold;
	else
		charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style >> fsBold;

  fontViewUpdate();
  spaceAndNullWidthUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::chkItalic_Click(TObject *Sender)
{
	if( chkItalic_->Checked )
		charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style << fsItalic;
	else
		charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style >> fsItalic;

	fontViewUpdate();
  spaceAndNullWidthUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::chkUnderline_Click(TObject *Sender)
{
	if( chkUnderline_->Checked )
  	charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style << fsUnderline;
	else
  	charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style >> fsUnderline;

	fontViewUpdate();
  spaceAndNullWidthUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::chkStrikeout_Click(TObject *Sender)
{
	if( chkStrikeout_->Checked )
		charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style << fsStrikeOut;
	else
  	charMap_->Canvas->Font->Style = charMap_->Canvas->Font->Style >> fsStrikeOut;

	fontViewUpdate();
  spaceAndNullWidthUpdate();
}
//---------------------------------------------------------------------------

TRect TFrmMain::xyToCellRect(int x, int y, int& col, int& row) const
{
	charMap_->MouseToCell(x, y, col, row);
	return charMap_->CellRect(col, row);
}
//---------------------------------------------------------------------------

TRect TFrmMain::symbolToCellRect(esU32 symbol, int& col, int& row) const
{
  if( static_cast<esU32>(-1) == symbol || symbol >= m_symPageEnd )
    return TRect::Empty();

  esU32 idx = symbol - m_symPageStart;
  row = idx / charMap_->ColCount;
  col = idx % charMap_->ColCount;

  return charMap_->CellRect(col, row);
}
//---------------------------------------------------------------------------

esU32 TFrmMain::symbolFromXY(int x, int y, esU32& symIdx, TRect& symRect) const
{
	int row, col;
  symRect = xyToCellRect(
    x,
    y,
    col,
    row
  );

  symIdx = col + row*charMap_->ColCount;
	esU32 symbol = m_symPageStart + symIdx;

  if( symbol >= m_symPageEnd )
    return static_cast<esU32>(-1);

  return symbol;
}
//---------------------------------------------------------------------------

void TFrmMain::selectionToggle(int x, int y)
{
  esU32 symIdx;
  TRect symRect;
  esU32 symbol = symbolFromXY(
    x,
    y,
    symIdx,
    symRect
  );

	if( m_recentSymbol != symbol )
	{
		// save recent symbol selection
		m_recentSymbol = symbol;

		// toggle selection
		m_selection[symIdx] = !m_selection[symIdx];

		// redraw cell
		InvalidateRect(
      charMap_->Handle,
      &symRect,
      FALSE
    );
	}
}
//---------------------------------------------------------------------------

void TFrmMain::selectionSet(int x, int y)
{
  esU32 symIdx;
  TRect symRect;
  esU32 symbol = symbolFromXY(
    x,
    y,
    symIdx,
    symRect
  );

	if( m_recentSymbol != symbol )
	{
		// save recent symbol selection
		m_recentSymbol = symbol;

    if( !m_selection[symIdx] )
		{
      m_selection[symIdx] = true;

      // redraw cell
      InvalidateRect(
        charMap_->Handle,
        &symRect,
        FALSE
      );
    }
	}
}
//---------------------------------------------------------------------------

void TFrmMain::focusSet(int x, int y)
{
  esU32 symIdx;
  TRect symRect;
  esU32 symbol = symbolFromXY(
    x,
    y,
    symIdx,
    symRect
  );

	if( m_recentSymbol != symbol )
  {
		// save recent symbol focus
		m_recentSymbol = symbol;

    // redraw cell
    InvalidateRect(
      charMap_->Handle,
      &symRect,
      FALSE
    );
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::charMap_MouseDown(TObject *Sender,
			TMouseButton Button, TShiftState Shift, int X, int Y)
{
	m_recentSymbol = static_cast<esU32>(-1);

  if( Shift == (TShiftState() << ssRight) )
  {
    focusSet(X, Y);
    return;
  }

  if( Shift == (TShiftState() << ssLeft << ssShift) )
  {
    selectionSet(X, Y);
    return;
  }

	selectionToggle(X,Y);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::charMap_MouseMove(TObject *Sender, TShiftState Shift,
			int X, int Y)
{
	if( Shift == (TShiftState() << ssLeft) )
		selectionToggle(X, Y);
  else if( Shift == (TShiftState() << ssLeft << ssShift) )
  {
    if( static_cast<esU32>(-1) == m_recentSymbol )
      return;

    esU32 symIdx;
    TRect symRect;
    esU32 symbol = symbolFromXY(
        X,
        Y,
        symIdx,
        symRect
      );
    if( static_cast<esU32>(-1) == symbol )
      return;

    for(esU32 idx = (m_recentSymbol-m_symPageStart); idx <= symIdx; ++idx )
      m_selection[idx] = true;

    charMap_->Invalidate();
  }
}
//---------------------------------------------------------------------------

int TFrmMain::bitDepthToClrFmt() const
{
  switch( cbxBitDepth_->ItemIndex )
  {
  case 1:
    return esguiClrfmtGrayscale2;
  case 2:
    return esguiClrfmtGrayscale4;
  case 3:
    return esguiClrfmtGrayscale8;
  default:
    return esguiClrfmtBitmap1;
  }
}
//---------------------------------------------------------------------------

void TFrmMain::fontHeightUpdate()
{
  m_height = static_cast<int>(edHeight_->Value) -
		static_cast<int>(edCropBottom_->Value) -
		static_cast<int>(edCropTop_->Value);
}
//---------------------------------------------------------------------------

bool TFrmMain::selectedAny() const
{
  for(auto const& v : m_selection)
    if( v )
      return true;

  return false;
}
//---------------------------------------------------------------------------

bool TFrmMain::currentSymIsSelected() const
{
  if( static_cast<esU32>(-1) == m_recentSymbol )
    return false;

  return m_selection[m_recentSymbol-m_symPageStart];
}
//---------------------------------------------------------------------------

void TFrmMain::deselectAll()
{
  for(size_t idx = 0; idx < m_selection.size(); ++idx)
    m_selection[idx] = false;

  charMap_->Invalidate();
}
//---------------------------------------------------------------------------

void TFrmMain::deselectBlock()
{
  if( static_cast<esU32>(-1) == m_recentSymbol )
    return;

  // Find selection block to which m_recentSymbol belongs
	esU32 symIdx = m_recentSymbol - m_symPageStart;
  if( !m_selection[symIdx] )
    return;

  esU32 selStartIdx = symIdx;
  esU32 selEndIdx = symIdx;
  if( symIdx )
  {
    esU32 idx = symIdx;
    while( m_selection[idx] )
    {
      selStartIdx = idx;
      if( idx )
        --idx;
      else
        break;
    }
  }

  if( symIdx < m_symPageEnd-1 )
  {
    esU32 idx = symIdx;
    while( m_selection[idx] )
    {
      selEndIdx = idx;
      if( idx < m_symPageEnd-1 )
        ++idx;
      else
        break;
    }
  }

  for( symIdx = selStartIdx; symIdx <= selEndIdx; ++symIdx )
    m_selection[symIdx] = false;

  charMap_->Invalidate();
}
//---------------------------------------------------------------------------

void TFrmMain::closeupViewShow(bool show)
{
  if( pbxCloseup_->Visible != show )
  {
    pbxCloseup_->Visible = show;
    charMap_->Visible = !show;
  }
}
//---------------------------------------------------------------------------

UnicodeString TFrmMain::fontOffsetsTableExport(const UnicodeString& baseName, TStrings* code, const std::vector<esU32>& offsets, esU32 chfirst)
{
  // Font block offsets data
  UnicodeString otblName = baseName + "_otbl";
  code->Add(
    UnicodeString::Format(
      "// Font block '%s' glyphs offsets data\nstatic const esU16 sc_%s[] = {",
      ARRAYOFCONST((
        baseName.c_str(),
        otblName.c_str()
      ))
    )
  );

  size_t idx = 0;
  while(idx < offsets.size()-1)
  {
    const UnicodeString& chstr = TCharacter::ConvertFromUtf32(chfirst);

    code->Add(
      UnicodeString::Format(
        "%d, //< Offset for symbol 0x%0.8X '%s' ",
        ARRAYOFCONST((
          offsets[idx],
          chfirst,
          chstr.c_str()
        ))
      )
    );

    ++chfirst;
    ++idx;
  }

  code->Add(
    UnicodeString::Format(
      "%d //< Last offset item \n};\n",
      ARRAYOFCONST((
        offsets[idx]
      ))
    )
  );

  return otblName;
}
//---------------------------------------------------------------------------

void TFrmMain::fontBlockExport(TStrings* code, esU32 first, esU32 last)
{
  const UnicodeString& baseName = UnicodeString::Format(
      "%s_0x%X_0x%X",
      ARRAYOFCONST((
        edFontCName_->Text.c_str(),
        first,
        last
      ))
    );

  code->Add(
    UnicodeString::Format(
      "// Font block '%s' definition\n//\n",
      ARRAYOFCONST((
        baseName.c_str()
      ))
    )
  );

  // Export continuous selection block.
  // Bitmap is an Alpha channel, i.e. opacity mask, FF - opaque, 0 - transparent
  //
  m_fontCanvas.reset(new Graphics::TBitmap);
	m_fontCanvas->HandleType = bmDIB;

  TCanvas* canvas = m_fontCanvas->Canvas;
	canvas->Font->Assign( charMap_->Canvas->Font );
	canvas->Font->Height = edHeight_->Text.ToIntDef(defaultFontHeight);
  canvas->TextFlags = ETO_CLIPPED;

  m_fontCanvas->Transparent = false;
  if(cbxBitDepth_->ItemIndex != 0)
  {
    m_fontCanvas->Monochrome = false;
  	m_fontCanvas->PixelFormat = pf32bit;
    m_fontCanvas->AlphaFormat = afDefined;
  }
  else
  {
  	m_fontCanvas->PixelFormat = pf1bit;
    m_fontCanvas->Monochrome = true;
  }

  esU32 ch = first;
  esU32 width = 0;
  esU32 top = static_cast<int>(edCropTop_->Value);

  // Initialize glyphs block and character offsets inside glyphs block
  std::vector<esU32> offsets;
  offsets.reserve(
    last-first+1
  );
  offsets.push_back(width);

  while( ch <= last )
  {
    const UnicodeString& chstr = TCharacter::ConvertFromUtf32( ch );
    const TSize& chext = canvas->TextExtent( chstr );
    width += chext.cx;

    offsets.push_back(
      width
    );

    ++ch;
  }

  m_fontCanvas->SetSize(
    width,
    m_height
  );

  const TRect& r = {
    0,
    0,
    (int)width,
    (int)m_height
  };

  canvas->Brush->Color = clBlack;
  canvas->Pen->Color = clWhite;
  canvas->CopyMode = cmSrcCopy;
  canvas->Font->Color = clWhite;

  canvas->FillRect( r );

  size_t idx = 0;
  ch = first;
  while( ch <= last )
  {
    const UnicodeString& chstr = TCharacter::ConvertFromUtf32( ch );

    canvas->TextOut(
      offsets[idx],
      -top,
      chstr
    );

    ++ch;
    ++idx;
  }

  int clrFmt = bitDepthToClrFmt();
  if( esguiClrfmtGrayscale2 <= clrFmt && clrFmt <= esguiClrfmtGrayscale8 )
    EsUtilities::dibToGrayscale(
      m_fontCanvas.get(),
      clrFmt
    );
  else
  {
    std::unique_ptr<TGIFImage> tmp( new TGIFImage );
    tmp->Dithering = TGIFDithering::gdDisabled;
    tmp->ColorReduction = rmQuantize;
    tmp->ReductionBits = 4;
    tmp->Transparent = false;
    tmp->Add(m_fontCanvas.get());
		tmp->OptimizeColorMap();
    int palCnt = tmp->GlobalColorMap->Count;

    if( palCnt > 1 )
      m_fontCanvas->Palette = tmp->Palette;
  }

	// Font block glyps data
  EsUtilities::dibDataExport(
    baseName,
    m_fontCanvas.get(),
    code,
    clrFmt
  );

  // Font block glyphs pixel offsets
  const UnicodeString& otblName = fontOffsetsTableExport(
    baseName,
    code,
    offsets,
    first
  );

  // Font block bitmap
  code->Add(
    UnicodeString::Format(
      "// Font '%s' block glyphs bitmap\nstatic const ESGUI_BITMAP sc_%s_bmp = {\n"
      "// size\n"
      "{%d, %d},\n"
      "// hdr\n"
      "{\n%d, //< colorFmt\n0, //< haveTransparentColor\n0 //< clrTransparent\n},\n"
      "// data\n"
      "(esU8*)sc_%s_data\n};\n",
      ARRAYOFCONST((
        baseName.c_str(),
        baseName.c_str(),
        offsets[last-first+1],
        m_height,
        clrFmt,
        baseName.c_str()
      ))
    )
  );

	// Add font block record at the end of the data
  code->Add(
    UnicodeString::Format(
      "// Font '%s' block data\nconst ESGUI_FONT_DATA c_%s = {\n"
      "&sc_%s_bmp, //< data (glyphs bitmap)\n"
      "0x%X, //< chFirst\n0x%X, //< chLast\n"
      "%d, //< spaceWidth\n%d, //< nullWidth\n"
      "sc_%s //< glyphsMap\n};",
      ARRAYOFCONST((
        baseName.c_str(),
        baseName.c_str(),
        baseName.c_str(),
        first,
        last,
        m_spaceWidth,
        m_nullWidth,
        otblName.c_str()
      ))
    )
  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actExport_Execute(TObject *Sender)
{
	dlgExport_->FileName = edFontCName_->Text;
	if( dlgExport_->Execute() )
  {
  	std::unique_ptr<TStringList> code( new TStringList );

    // For each continuous selection range - generate block code
    //
    esU32 idx = 0;
    esU32 cnt = m_symPageEnd - m_symPageStart;
    esU32 beg = static_cast<esU32>(-1);
    esU32 end = beg;

    while(idx < cnt)
    {
      if( m_selection[idx] )
      {
        if( beg == static_cast<esU32>(-1) )
          beg = idx;

        end = idx;
      }
      else if( beg != static_cast<esU32>(-1) )
      {
        fontBlockExport(
          code.get(),
          m_symPageStart + beg,
          m_symPageStart + end
        );

        beg = static_cast<esU32>(-1);
        end = beg;
      }

      ++idx;
    }

    // Handle the last range
    if( beg != static_cast<esU32>(-1) )
      fontBlockExport(
        code.get(),
        m_symPageStart + beg,
        m_symPageStart + end
      );

  	code->SaveToFile(dlgExport_->FileName);
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edHeight_Change(TObject *Sender)
{
  fontHeightUpdate();
  spaceAndNullWidthUpdate();

	cFontNameUpdate();
  cropRestrictionsUpdate();

	fontViewUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edCropTop_ValueChanged(TObject *Sender)
{
	int diff = static_cast<int>(edHeight_->Value) -
  	static_cast<int>(edCropTop_->Value);
	if( static_cast<int>(edCropBottom_->Value) > diff )
		edCropBottom_->Value = diff;

  fontHeightUpdate();

	fontViewUpdate();
	cFontNameUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edCropBottom_ValueChanged(TObject *Sender)
{
	int diff = static_cast<int>(edHeight_->Value) -
		static_cast<int>(edCropBottom_->Value);
	if( static_cast<int>(edCropTop_->Value) > diff )
		edCropTop_->Value = diff;

  fontHeightUpdate();

	fontViewUpdate();
	cFontNameUpdate();
}
//---------------------------------------------------------------------------

void TFrmMain::cropRestrictionsUpdate()
{
  int bottom = static_cast<int>(edCropBottom_->Value);
  int top = static_cast<int>(edCropTop_->Value);
  int height = static_cast<int>(edHeight_->Value);
  int max = height/2;
	edCropBottom_->Properties->MaxValue = max;
  edCropTop_->Properties->MaxValue = max;

  if( bottom > max )
    bottom = max-1;

  if( top > max )
    top = max-1;

  if( bottom < 0 )
    bottom = 0;

  if( top < 0 )
    top = 0;

  edCropBottom_->Value = bottom;
  edCropTop_->Value = top;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbxUnicodeScripts_Change(TObject *Sender)
{
  int id = esUnicodeScript_Basic_Latin + cbxUnicodeScripts_->ItemIndex;
  symbolPageSet( id );
  cFontNameUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::csSpaceWidth_PropertiesEditValueChanged(TObject *Sender)
{
  m_spaceWidth = csSpaceWidth_->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::csNullWidth_PropertiesEditValueChanged(TObject *Sender)
{
  m_nullWidth = csNullWidth_->Value;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::FormResize(TObject *Sender)
{
  charMapLayoutRecalc();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbxBitDepth_Change(TObject *Sender)
{
  cFontNameUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actDeselectAll_Update(TObject *Sender)
{
  actDeselectAll_->Enabled = selectedAny();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actDeselectAll_Execute(TObject *Sender)
{
  deselectAll();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actBlockDeselect_Update(TObject *Sender)
{
  actBlockDeselect_->Enabled = currentSymIsSelected();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actBlockDeselect_Execute(TObject *Sender)
{
  deselectBlock();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actSymbolViewZoomed_Update(TObject *Sender)
{
  actSymbolViewZoomed_->Enabled = m_recentSymbol != static_cast<esU32>(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actSymbolViewZoomed_Execute(TObject *Sender)
{
  closeupViewShow(
    !pbxCloseup_->Visible
  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pbxCloseup_Paint(TObject *Sender)
{
  TFont* font = pbxCloseup_->Canvas->Font;
	font->Height = presentationFontHeight*8;
  font->Charset = DEFAULT_CHARSET;

	if( chkBold_->Checked )
    font->Style = font->Style << fsBold;
  else
    font->Style = font->Style >> fsBold;

	if( chkItalic_->Checked )
    font->Style = font->Style << fsItalic;
  else
    font->Style = font->Style >> fsItalic;

  if( chkUnderline_->Checked )
    font->Style = font->Style << fsUnderline;
  else
    font->Style = font->Style >> fsUnderline;

	if( chkStrikeout_->Checked )
    font->Style = font->Style << fsStrikeOut;
  else
    font->Style = font->Style >> fsStrikeOut;

  symbolCellDraw(
    pbxCloseup_->Canvas,
    pbxCloseup_->ClientRect,
    false,
    false,
    true,
    m_recentSymbol
  );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pbxCloseup_DblClick(TObject *Sender)
{
  closeupViewShow(false);
}
//---------------------------------------------------------------------------

