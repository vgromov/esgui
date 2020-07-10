//---------------------------------------------------------------------------

#include <vcl.h>
#include <tchar.h>
#pragma hdrstop

#include <System.IOUtils.hpp>
#include <System.RegularExpressions.hpp>
#include "TFrmMain.h"
#include "EsDibUtilities.h"

// use jpeg, png and gif support
#include <jpeg.hpp>
#include <pngimage.hpp>
#include <gifimg.hpp>

// STD
#include <algorithm>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "cspin"
#pragma resource "*.dfm"
TFrmMain *FrmMain;
//---------------------------------------------------------------------------

// default color is black
const ESGUI_COLOR c_defClr = esguiColorSet(0,0,0);

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

__fastcall TFrmMain::TFrmMain(TComponent* Owner)
	: TForm(Owner),
  m_orig( new Graphics::TBitmap ),
  m_result( new Graphics::TBitmap ),
  m_palCnt(0),
  m_selClr(0),
  m_bpp(0),
  m_canUseTranspColor(false),
  m_useTranspColor(false),
  m_transpColorSelected(false),
  m_exportPalette(false),
  m_eyeDropperActive(false),
  m_transpColorIsPalIdx(false)
{
	ShowHint = true;
	cbxOutFmt_->ItemIndex = 0;
	cbxOutFmt_Change(nullptr);
	cbxPaletteOutFmt_->ItemIndex = 0;
	cbxPaletteOutFmt_Change(nullptr);
  lblSelectedClr_->Action = actSelectedClr_;

  m_curClr = TColorRec::Null;

	updateSelectionControls();
	updateImageInfo();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pbxImage_MouseMove(TObject *Sender, TShiftState Shift,
			int X, int Y)
{
	UnicodeString pos;
  TPaintBox* pbx = dynamic_cast<TPaintBox*>( Sender );

  if( pbx )
  {
    TScrollBox* scroller = dynamic_cast<TScrollBox*>( pbx->Parent );

    int x = X + scroller->HorzScrollBar->ScrollPos;
    int y = Y + scroller->VertScrollBar->ScrollPos;

    if(
      m_eyeDropperActive
    )
    {
      m_curClr = pbx->Canvas->Pixels[x][y];

      pos.printf(
        _T("x: %d; y: %d; RGB: 0x%0.8X"),
        x,
        y,
        m_curClr
      );
    }
    else
      pos.printf(
        _T("x: %d; y: %d"),
        x,
        y
      );

    TPoint pnt(X, Y);
    pnt = pbx->ClientToScreen(pnt);

    pbx->Hint = pos;
    Application->ActivateHint( pnt );
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actFileExit_Execute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actFileOpen_Execute(TObject *Sender)
{
	if( dlgOpen_->Execute() )
	{
  	std::unique_ptr<TPicture> img(new TPicture);
    img->LoadFromFile( dlgOpen_->FileName );
		m_orig->Width = img->Width;
    m_orig->Height = img->Height;
    m_orig->Canvas->Brush->Color = clWhite;
    m_orig->Canvas->FillRect( TRect(0, 0, img->Width, img->Height) );
    m_orig->Canvas->Draw(0, 0, img->Graphic);

		if( !m_orig->Empty )
		{
			// resize paint box for image to fit inside it
			pbxOrigView_->Width = m_orig->Width;
			pbxOrigView_->Height = m_orig->Height;

			BITMAP bm;
			if( GetObject( m_orig->Handle, sizeof(BITMAP), &bm ) )
				m_bpp = bm.bmBitsPixel;
		}
		else
			m_bpp = 0;

		updateSelectionControls();
		updateImageInfo();
		pbxOrigView_->Repaint();
    updateResult();
    updatePalette();
		updateMemoryEstimate();

    m_srcFileName = dlgOpen_->FileName;
    outputBaseNameUpdate();
	}
}
//---------------------------------------------------------------------------

void TFrmMain::updateSelectionControls()
{
	if( !m_orig->Empty )
	{
		edLeft_->Enabled = true;
		edTop_->Enabled = true;
		edRight_->Enabled = true;
		edBottom_->Enabled = true;

		// apply selectors restrictions & initialize selector values
		edLeft_->MaxValue = m_orig->Width-1;
		edLeft_->Value = 0;
		edRight_->MaxValue = edLeft_->MaxValue;
		edRight_->Value = edRight_->MaxValue;
		edTop_->MaxValue = m_orig->Height-1;
		edTop_->Value = 0;
		edBottom_->MaxValue = edTop_->MaxValue;
		edBottom_->Value = edBottom_->MaxValue;
	}
	else
	{
		edLeft_->Value = 0;
		edTop_->Value = 0;
		edRight_->Value = 0;
		edBottom_->Value = 0;

		edLeft_->Enabled = false;
		edTop_->Enabled = false;
		edRight_->Enabled = false;
		edBottom_->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void TFrmMain::updateResult()
{
	if( m_orig->Empty )
  	return;

	m_result->HandleType = bmDIB;

  int clrFmt = outputClrFmtGetFromCtl();
	int bpp = esguiBppFromColorFormat(clrFmt);

  bool usesPalette = esguiColorFormatUsesPalette(clrFmt);

  if( usesPalette )
	{
    switch(bpp)
    {
    case 1:
      m_result->PixelFormat = pf1bit;
      break;
    case 4:
      m_result->PixelFormat = pf4bit;
      break;
    case 8:
      m_result->PixelFormat = pf8bit;
      break;
    }
  }
  else
    m_result->PixelFormat = pf32bit;

	m_result->Width = edRight_->Value - edLeft_->Value + 1;
	m_result->Height = edBottom_->Value - edTop_->Value + 1;

  if( 1 == bpp )
  	m_palCnt = 2;
  if( 8 < bpp )
	  m_palCnt = 0;

  if( usesPalette && 1 <= bpp && bpp <= 8 ) // Palette - aware processing
  {
    std::unique_ptr<TGIFImage> tmp( new TGIFImage );
		tmp->ColorReduction = rmQuantize;
    tmp->ReductionBits = ((1 == bpp) ? 4 : bpp);
    tmp->Assign(m_orig.get());
    tmp->Transparent = false;
		tmp->OptimizeColorMap();
    m_palCnt = std::min(tmp->GlobalColorMap->Count, 256);
    m_result->Palette = tmp->Palette;
    m_result->Canvas->Draw(-edLeft_->Value, -edTop_->Value, tmp->Bitmap);
 	}
  else // Grayscale or RGB processing
  {
		m_result->Canvas->Draw(-edLeft_->Value, -edTop_->Value, m_orig.get());

    if( esguiClrfmtGrayscale2 <= clrFmt && clrFmt <= esguiClrfmtGrayscale8 )
      EsUtilities::dibToGrayscale(
        m_result.get(),
        clrFmt
      );
    else
      EsUtilities::dibToRGB(
        m_result.get(),
        clrFmt
      );
  }

	// resize paint box for image to fit inside it
	pbxResult_->Width = m_result->Width;
	pbxResult_->Height = m_result->Height;
  pbxResult_->Repaint();
  updatePalette();
}
//---------------------------------------------------------------------------

void TFrmMain::updateImageInfo()
{
	if( !m_orig->Empty )
		lblImageInfo_->Caption = UnicodeString::Format(
			"%d bpp image; WxH: (%dx%d)\n"
			"Selected area (%d,%d)-(%d,%d); WxH: (%dx%d)",
      ARRAYOFCONST((
        m_bpp,
        m_orig->Width,
        m_orig->Height,
        edLeft_->Value,
        edTop_->Value,
        edRight_->Value,
        edBottom_->Value,
        edRight_->Value - edLeft_->Value + 1,
        edBottom_->Value-edTop_->Value + 1
      ))
    );
	else
		lblImageInfo_->Caption = "No convertable image detected";
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edLeft_Change(TObject *Sender)
{
	// restrict right selector
	if( edLeft_->Value > edRight_->Value )
		edRight_->Value = edLeft_->Value;

	updateImageInfo();
	pbxOrigView_->Repaint();
  updateResult();
	updateMemoryEstimate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edTop_Change(TObject *Sender)
{
	// restrict bottom selector
	if( edTop_->Value > edBottom_->Value )
		edBottom_->Value = edTop_->Value;

	updateImageInfo();
	pbxOrigView_->Repaint();
  updateResult();
	updateMemoryEstimate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edRight_Change(TObject *Sender)
{
	// restrict left selector
	if( edLeft_->Value > edRight_->Value )
		edLeft_->Value = edRight_->Value;

	updateImageInfo();
	pbxOrigView_->Repaint();
  updateResult();
	updateMemoryEstimate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::edBottom_Change(TObject *Sender)
{
	// restrict top selector
	if( edTop_->Value > edBottom_->Value )
		edTop_->Value = edBottom_->Value;

	updateImageInfo();
	pbxOrigView_->Repaint();
  updateResult();
	updateMemoryEstimate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pbxOrigView_Paint(TObject *Sender)
{
	if( !m_orig->Empty )
	{
		pbxOrigView_->Canvas->Draw(0, 0, m_orig.get());
		TRect sel(edLeft_->Value, edTop_->Value, edRight_->Value+1, edBottom_->Value+1);
		pbxOrigView_->Canvas->DrawFocusRect(sel);
	}
}
//---------------------------------------------------------------------------

void TFrmMain::updateMemoryEstimate()
{
	if( !m_orig->Empty )
	{
		String estimate;

		int bpp = esguiBppFromColorFormat(
      outputClrFmtGetFromCtl()
    );

		int bytesPerLine = ((edRight_->Value - edLeft_->Value + 1) * bpp) / 8 +
			(((edRight_->Value - edLeft_->Value + 1) * bpp) % 8 ? 1 : 0);
		int dataSize = bytesPerLine	*	(edBottom_->Value-edTop_->Value + 1);

		int palSize = 0;
		if( m_canExportPalette && m_exportPalette )
    {
      bpp = esguiBppFromColorFormat( cbxPaletteOutFmt_->ItemIndex+esguiClrfmtRGB332 );
			palSize = bpp * m_palCnt;
    }

		estimate.printf(
			_T("Image data: %d bytes\n")
			_T("Palette data: %d bytes in %d entries\n")
			_T("Total: %d bytes"), dataSize, palSize, m_palCnt, dataSize + palSize);

		lblEstimate_->Caption = estimate;
	}
	else
		lblEstimate_->Caption = _T("Nothing to estimate");
}
//---------------------------------------------------------------------------

UnicodeString TFrmMain::esguiClrfmtStringGet(int clrfmt)
{
  static const char* sc_bppstr[esguiClrfmtCnt] = {
    "Invalid",
    "bpp1",
    "pal4",
    "pal8",
    "gs2",
    "gs4",
    "gs8",
    "rgb332",
    "bgr233",
    "rgb565",
    "bgr565",
    "rgb888",
    "bgr888",
    "rgba8888",
    "argb8888",
    "bgra8888",
    "abgr8888"
  };

  return sc_bppstr[clrfmt];
}
//---------------------------------------------------------------------------

void TFrmMain::outputBaseNameUpdate()
{
  if( m_srcFileName.IsEmpty() )
    m_srcFileName = "bitmap";

  UnicodeString fn = Ioutils::TPath::GetFileNameWithoutExtension(m_srcFileName);

  fn += "_";
  fn += esguiClrfmtStringGet(
    outputClrFmtGetFromCtl()
  );
  fn = TRegEx::Replace(fn, "[~`\"<>\\{\\}\\|\\(\\)%$@!#\\+\\-\\.,\\&\\s\\*^\\?\\:]+", "_");
  fn = fn.LowerCase();

  edBaseName_->Text = fn;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbxOutFmt_Change(TObject *Sender)
{
  updateResult();
	updatePaletteControls();
	updateMemoryEstimate();
  updateTransparencyControls();
  outputBaseNameUpdate();
}
//---------------------------------------------------------------------------

void TFrmMain::updatePaletteControls()
{
  int fmt = outputClrFmtGetFromCtl();
	int bpp = esguiBppFromColorFormat( fmt );
  m_canExportPalette = (bpp > 1 && bpp <= 8) && fmt <= esguiClrfmtPalette8;
}
//---------------------------------------------------------------------------

void TFrmMain::updatePalette()
{
  // populate palette with image colors
  if( m_palCnt )
    GetDIBColorTable(
      m_result->Canvas->Handle,  	// handle to DC
      0,  											 	// color table index of first entry
      m_palCnt,										// number of entries to retrieve
      m_pal   										// array of color table entries
    );
  updatePaletteView();
}
//---------------------------------------------------------------------------

void TFrmMain::updateTransparencyControls()
{
  int fmt = outputClrFmtGetFromCtl();

  m_transpColorSelected = false;
  m_canUseTranspColor = !(fmt >= esguiClrfmtGrayscale2 && fmt <= esguiClrfmtGrayscale8);
  m_transpColorIsPalIdx = esguiColorFormatUsesPalette(fmt);
}
//---------------------------------------------------------------------------

void TFrmMain::updatePaletteView()
{
	if( m_result->Empty || !m_palCnt )
  	gridPal_->Visible = false;
  else
  {
    int maxCols = gridPal_->ClientWidth / (gridPal_->DefaultColWidth + 1);
    int cols = m_palCnt > maxCols ? maxCols : m_palCnt;
    int rows = m_palCnt / cols + (m_palCnt % cols ? 1 : 0);

    gridPal_->ColCount = cols;
    gridPal_->RowCount = rows;
    gridPal_->Visible = true;
    gridPal_->Repaint();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::gridPal_DrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
	size_t idx = gridPal_->ColCount * ARow + ACol;
  if( idx < m_palCnt )
  {
  	const RGBQUAD& pe = m_pal[ idx ];
		gridPal_->Canvas->Brush->Color = static_cast<TColor>(RGB(pe.rgbRed, pe.rgbGreen, pe.rgbBlue));
		gridPal_->Canvas->FillRect(Rect);
  }
  else
  {
		gridPal_->Canvas->Brush->Color = clWhite;
 		gridPal_->Canvas->FillRect(Rect);
    gridPal_->Canvas->Pen->Color = clBlack;
    TPenStyle old = gridPal_->Canvas->Pen->Style;
    gridPal_->Canvas->Pen->Style = psDot;
    gridPal_->Canvas->MoveTo(Rect.left, Rect.top);
    gridPal_->Canvas->LineTo(Rect.right, Rect.bottom);
    gridPal_->Canvas->MoveTo(Rect.left, Rect.bottom);
    gridPal_->Canvas->LineTo(Rect.right, Rect.top);
    gridPal_->Canvas->Pen->Style = old;
  }
}
//---------------------------------------------------------------------------

int TFrmMain::outputClrFmtGetFromCtl() const
{
  return (cbxOutFmt_->ItemIndex + esguiClrfmtBitmap1);
}
//---------------------------------------------------------------------------

int TFrmMain::palOutputClrFmtGetFromCtl() const
{
  return (cbxPaletteOutFmt_->ItemIndex + esguiClrfmtRGB332);
}
//---------------------------------------------------------------------------

void TFrmMain::bitmapExport(const String& file)
{
  int clrFmt = outputClrFmtGetFromCtl();
	std::unique_ptr<TStringList> code(new TStringList);

  const UnicodeString& baseName = edBaseName_->Text;

  // Write bitmap data first
  EsUtilities::dibDataExport(
    baseName,
    m_result.get(),
    code.get(),
    clrFmt
  );

	// Write the bitmap structure itself
	code->Add(
    UnicodeString::Format(
      "// bitmap %s \nconst ESGUI_BITMAP c_%s_bitmap = {",
      ARRAYOFCONST((
        baseName.c_str(),
        baseName.c_str()
      ))
    )
  );

	code->Add(
    UnicodeString::Format(
      "// size\n{%d, %d},",
      ARRAYOFCONST((
        m_result->Width,
        m_result->Height
      ))
    )
  );

	code->Add(
    UnicodeString::Format(
      "// header\n{\n%d, // Color Format\n%d, // Transparent color specified",
      ARRAYOFCONST((
        outputClrFmtGetFromCtl(),
        (int)(m_useTranspColor && m_canUseTranspColor)
      ))
    )
  );

	int bpp = esguiBppFromColorFormat( clrFmt );
  if( !(m_useTranspColor && m_canUseTranspColor) )
  {
    code->Add(
      "0 // Transparent color\n},"
    );
  }
  else
  {
    UnicodeString fmt = "0x%0.8X";
    switch(bpp)
    {
    case 8:
      fmt = "0x%0.2X";
      break;
    case 16:
      fmt = "0x%0.4X";
      break;
    case 24:
      fmt = "0x%0.6X";
      break;
    }

    code->Add(
      UnicodeString::Format(
        fmt + " // Transparent color\n},",
        ARRAYOFCONST((
          m_selClr
        ))
      )
    );
  }

  code->Add(
    UnicodeString::Format(
      "// data\nsc_%s_data\n};\n",
      ARRAYOFCONST((
        baseName.c_str()
      ))
    )
  );

  // Optionally, write a palette
  if( m_canExportPalette && m_exportPalette )
    EsUtilities::dibPaletteExport(
      baseName,
      m_pal,
      m_palCnt,
      code.get(),
      cbxPaletteOutFmt_->ItemIndex+esguiClrfmtRGB332
    );

	code->SaveToFile(file);
}
//---------------------------------------------------------------------------

int TFrmMain::palIdxGetFromMousePos(int x, int y)
{
  int col;
  int row;

  gridPal_->MouseToCell(
    x,
    y,
    col,
    row
  );

  if( -1 < col && -1 < row )
  {
    int idx = col + row * gridPal_->ColCount;
    if( idx < m_palCnt )
      return idx;
  }

  return -1;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actFileExport_Execute(TObject *Sender)
{
	dlgExport_->FileName = edBaseName_->Text;
	if( dlgExport_->Execute() )
	{
		bitmapExport( dlgExport_->FileName );
	}
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actFileExport_Update(TObject *Sender)
{
	actFileExport_->Enabled = !m_orig->Empty;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::pbxResult_Paint(TObject *Sender)
{
	if( !m_result->Empty )
		pbxResult_->Canvas->Draw(
      0,
      0,
      m_result.get()
    );
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actHaveTranspColor_Update(TObject *Sender)
{
  actHaveTranspColor_->Enabled = m_canUseTranspColor;
  actHaveTranspColor_->Checked = m_useTranspColor;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actHaveTranspColor_Execute(TObject *Sender)
{
  m_useTranspColor = !m_useTranspColor;
  updateTransparencyControls();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actExportPalette_Update(TObject *Sender)
{
  actExportPalette_->Enabled = m_canExportPalette;
  actExportPalette_->Checked = m_exportPalette;
  cbxPaletteOutFmt_->Enabled = actExportPalette_->Enabled;
  gridPal_->Enabled = actExportPalette_->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actExportPalette_Execute(TObject *Sender)
{
  m_exportPalette = !m_exportPalette;
  updateMemoryEstimate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::cbxPaletteOutFmt_Change(TObject *Sender)
{
	updatePaletteControls();
	updateMemoryEstimate();
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actEyeDropper_Update(TObject *Sender)
{
  actEyeDropper_->Enabled = m_canUseTranspColor &&
    m_useTranspColor &&
    m_result &&
    !m_result->Empty;
  actEyeDropper_->Checked = m_eyeDropperActive;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::actEyeDropper_Execute(TObject *Sender)
{
  m_eyeDropperActive = !m_eyeDropperActive;
  m_transpColorSelected = false;
  m_curClr = TColorRec::Null;
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::img_Click(TObject *Sender)
{
  if( actEyeDropper_->Enabled && m_eyeDropperActive )
  {
    ESGUI_COLOR clr = esguiColorSet(
      (m_curClr >> 16) & 0xFF,
      (m_curClr >> 8) & 0xFF,
      m_curClr & 0xFF
    );

    esguiColorToColor(
      &m_selClr,
      static_cast<ESGUI_COLOR_FORMAT>(
        outputClrFmtGetFromCtl()
      ),
      clr.raw,
      esguiClrfmtNative,
      NULL
    );

    m_transpColorSelected = true;
  }

  m_eyeDropperActive = false;
}
//---------------------------------------------------------------------------


void __fastcall TFrmMain::actSelectedClr_Update(TObject *Sender)
{
  if( m_transpColorSelected )
  {
    if( m_transpColorIsPalIdx )
      actSelectedClr_->Caption = UnicodeString::Format(
        "IDX: %d",
        ARRAYOFCONST((
          m_selClr
        ))
      );
    else
    {
      UnicodeString fmt = "RGB: 0x%0.8X";
      int clrFmt = outputClrFmtGetFromCtl();
      switch(clrFmt)
      {
      case esguiClrfmtRGB332:
      case esguiClrfmtBGR233:
        fmt = "RGB: 0x%0.2X";
        break;
      case esguiClrfmtRGB565:
      case esguiClrfmtBGR565:
        fmt = "RGB: 0x%0.4X";
        break;
      case esguiClrfmtRGB888:
      case esguiClrfmtBGR888:
        fmt = "RGB: 0x%0.6X";
        break;
      }

      actSelectedClr_->Caption = UnicodeString::Format(
        fmt,
        ARRAYOFCONST((
          m_selClr
        ))
      );
    }
  }
  else
    actSelectedClr_->Caption = "--";
}
//---------------------------------------------------------------------------

void __fastcall TFrmMain::gridPal_MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
  if( m_transpColorIsPalIdx && m_useTranspColor )
  {
    // If we use transp. color and output format is a paletted format,
    // clicking on palette cell results in its selection as a transparent color
    //
    int idx = palIdxGetFromMousePos(
      X, Y
    );

    if( -1 < idx )
    {
      m_selClr = idx;

      const RGBQUAD& rgb = m_pal[idx];

      m_curClr = static_cast<TColor>(
        (static_cast<esU32>(rgb.rgbRed) << 16) |
        (static_cast<esU32>(rgb.rgbGreen) << 8) |
        (static_cast<esU32>(rgb.rgbBlue)) |
        0xFF000000UL
      );

      m_transpColorSelected = true;
    }
    else
      m_transpColorSelected = false;
  }
}
//---------------------------------------------------------------------------

