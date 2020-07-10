#ifndef TFrmMainH
#define TFrmMainH
//---------------------------------------------------------------------------

#include <System.Actions.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ActnList.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include "cxContainer.hpp"
#include "cxControls.hpp"
#include "cxDropDownEdit.hpp"
#include "cxEdit.hpp"
#include "cxFontNameComboBox.hpp"
#include "cxGraphics.hpp"
#include "cxLookAndFeelPainters.hpp"
#include "cxLookAndFeels.hpp"
#include "cxMaskEdit.hpp"
#include "cxSpinEdit.hpp"
#include "cxTextEdit.hpp"
#include "dxSkinsCore.hpp"
#include "dxSkinsDefaultPainters.hpp"
#include <Vcl.Menus.hpp>
#include <memory>
#include <algorithm>
#include <vector>
#include <map>
#include <esgui/include/esgui.h>
//---------------------------------------------------------------------------

// char glyph storage

//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *pnlRight_;
	TActionList *actsCmds_;
	TDrawGrid *charMap_;
	TPanel *pnlLeft_;
	TGroupBox *gbxSelectFont_;
	TGroupBox *gbxFontStyle_;
	TCheckBox *chkBold_;
	TCheckBox *chkItalic_;
	TCheckBox *chkUnderline_;
	TCheckBox *chkStrikeout_;
	TGroupBox *gbxExportProps_;
	TSaveDialog *dlgExport_;
	TButton *Button1;
	TAction *actExport_;
	TLabeledEdit *edFontCName_;
	TLabel *lblHeight_;
	TcxSpinEdit *edHeight_;
	TcxFontNameComboBox *cbxFonts_;
	TcxSpinEdit *edCropTop_;
	TcxSpinEdit *edCropBottom_;
	TLabel *lblCropTop_;
	TGroupBox *gbxCropParams_;
	TLabel *lblCropBottom_;
  TLabel *lblBitDepth_;
  TComboBox *cbxBitDepth_;
  TGroupBox *gbxCharParams_;
  TLabel *lblUnicodeScript_;
  TComboBox *cbxUnicodeScripts_;
  TLabel *lblSpaceWidth_;
  TcxSpinEdit *csSpaceWidth_;
  TLabel *lblNullWidth_;
  TcxSpinEdit *csNullWidth_;
  TPopupMenu *ctxMenu_;
  TAction *actDeselectAll_;
  TMenuItem *DeselectAll1;
  TAction *actBlockDeselect_;
  TMenuItem *DeselectBlock1;
  TAction *actSymbolViewZoomed_;
  TMenuItem *N2;
  TMenuItem *Closeup1;
  TPaintBox *pbxCloseup_;
	void __fastcall charMap_DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall cbxFonts_Change(TObject *Sender);
	void __fastcall chkBold_Click(TObject *Sender);
	void __fastcall chkItalic_Click(TObject *Sender);
	void __fastcall chkUnderline_Click(TObject *Sender);
	void __fastcall chkStrikeout_Click(TObject *Sender);
	void __fastcall charMap_MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall charMap_MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall actExport_Execute(TObject *Sender);
	void __fastcall edHeight_Change(TObject *Sender);
	void __fastcall edCropTop_ValueChanged(TObject *Sender);
	void __fastcall edCropBottom_ValueChanged(TObject *Sender);
  void __fastcall cbxUnicodeScripts_Change(TObject *Sender);
  void __fastcall csSpaceWidth_PropertiesEditValueChanged(TObject *Sender);
  void __fastcall csNullWidth_PropertiesEditValueChanged(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall cbxBitDepth_Change(TObject *Sender);
  void __fastcall actDeselectAll_Execute(TObject *Sender);
  void __fastcall actDeselectAll_Update(TObject *Sender);
  void __fastcall actBlockDeselect_Update(TObject *Sender);
  void __fastcall actBlockDeselect_Execute(TObject *Sender);
  void __fastcall actSymbolViewZoomed_Update(TObject *Sender);
  void __fastcall actSymbolViewZoomed_Execute(TObject *Sender);
  void __fastcall pbxCloseup_Paint(TObject *Sender);
  void __fastcall pbxCloseup_DblClick(TObject *Sender);

protected:
  // Misc consts:
  enum {
    defaultFontHeight = 24,
    presentationFontHeight = 42,
    defaultCharmapColWidth = 70,
    defaultCharmapRowHeight = 80
  };

public:		// User declarations
	__fastcall TFrmMain(TComponent* Owner);

protected:
  void unicodePagesSelectorInit();

  void charMapLayoutRecalc();

  TRect xyToCellRect(int x, int y, int& col, int& row) const;
  TRect symbolToCellRect(esU32 symbol, int& col, int& row) const;

  esU32 symbolFromXY(int x, int y, esU32& symIdx, TRect& symRect) const;
	void selectionToggle(int x, int y);
  void selectionSet(int x, int y);
  void focusSet(int x, int y);

  void symbolCellDraw(TCanvas* canvas, const TRect& rect, bool selected, bool focused, bool drawGrid, esU32 symbol);

  static UnicodeString fontOffsetsTableExport(const UnicodeString& baseName, TStrings* code, const std::vector<esU32>& offsets, esU32 chfirst);
  void fontBlockExport(TStrings* code, esU32 first, esU32 last);
	void cFontNameUpdate();
  void spaceAndNullWidthUpdate();
  void cropRestrictionsUpdate();

  int bitDepthToClrFmt() const;
  void fontHeightUpdate();

  void symbolPageSet(int scriptId);

  bool isSelected(esU32 symbol) const;

  bool selectedAny() const;
  bool currentSymIsSelected() const;

  void deselectAll();
  void deselectBlock();

  void closeupViewShow(bool show);

  void fontViewUpdate();

protected:
	// data members
	std::unique_ptr<Graphics::TBitmap> m_fontCanvas;
	std::unique_ptr<TFont> m_fontSmall;
	std::unique_ptr<TFont> m_fontSaver;

	// char selection
  std::vector<bool> m_selection;

  // Current font height
  int m_height;
  int m_spaceWidth;
  int m_nullWidth;

	// recently toggled symbol selection
  esU32 m_symPageStart;
  esU32 m_symPageEnd;
	esU32 m_recentSymbol;
};
//---------------------------------------------------------------------------
extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
