//---------------------------------------------------------------------------

#ifndef TFrmMainH
#define TFrmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <XPMan.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <System.Actions.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.EngExt.hpp>
#include <System.Bindings.Outputs.hpp>
#include <System.Rtti.hpp>
#include <System.ImageList.hpp>
#include <Vcl.Bind.DBEngExt.hpp>
#include <Vcl.Bind.Editors.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ImgList.hpp>
#include "cspin.h"
#include <memory>
#include <vector>
#include <esgui/include/esgui.h>
//---------------------------------------------------------------------------
class TFrmMain : public TForm
{
__published:	// IDE-managed Components
	TOpenPictureDialog *dlgOpen_;
	TScrollBox *viewImage_;
	TLabel *lblImageInfo_;
	TMainMenu *mnuMain_;
	TActionList *lstCmds_;
	TAction *actFileOpen_;
	TMenuItem *File1;
	TMenuItem *Fileopen1;
	TAction *actFileExit_;
	TMenuItem *Exit1;
	TMenuItem *N1;
  TComboBox *cbxOutFmt_;
  TLabel *lblOutFmt_;
	TPaintBox *pbxOrigView_;
	TPanel *pnlTop_;
	TCSpinEdit *edLeft_;
	TCSpinEdit *edTop_;
	TCSpinEdit *edRight_;
	TLabel *lblLeft_;
	TLabel *lblTop_;
	TLabel *lblRight_;
	TCSpinEdit *edBottom_;
	TLabel *lblBottom_;
	TGroupBox *gbxSelection_;
	TBevel *bvl1_;
	TPanel *pnlContents_;
	TGroupBox *gbxExport_;
	TGroupBox *gbxInfo_;
	TGroupBox *gbxPalette_;
	TGroupBox *gbxEstimate_;
	TCheckBox *chkExportPalette_;
	TLabel *lblEstimate_;
	TLabel *lblBaseName_;
	TEdit *edBaseName_;
	TDrawGrid *gridPal_;
	TAction *actFileExport_;
	TSaveDialog *dlgExport_;
	TMenuItem *Export1;
	TPageControl *pgImages_;
	TTabSheet *tabOriginal_;
	TTabSheet *tabResult_;
	TScrollBox *viewResult_;
	TPaintBox *pbxResult_;
  TCheckBox *chkHaveTransparentColor_;
  TGridPanel *laySettings_;
  TLabel *lblTransparentColor_;
  TAction *actHaveTranspColor_;
  TAction *actExportPalette_;
  TGridPanel *layPalette_;
  TLabel *lblPalOutputFmt_;
  TComboBox *cbxPaletteOutFmt_;
  TSpeedButton *btnEyedropper_;
  TAction *actEyeDropper_;
  TImageList *imgs_;
  TLabel *lblSelectedClr_;
  TControlAction *actSelectedClr_;
	void __fastcall pbxImage_MouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y);
	void __fastcall actFileExit_Execute(TObject *Sender);
	void __fastcall actFileOpen_Execute(TObject *Sender);
	void __fastcall edLeft_Change(TObject *Sender);
	void __fastcall edTop_Change(TObject *Sender);
	void __fastcall edRight_Change(TObject *Sender);
	void __fastcall edBottom_Change(TObject *Sender);
	void __fastcall pbxOrigView_Paint(TObject *Sender);
	void __fastcall cbxOutFmt_Change(TObject *Sender);
	void __fastcall gridPal_DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall actFileExport_Execute(TObject *Sender);
	void __fastcall actFileExport_Update(TObject *Sender);
	void __fastcall pbxResult_Paint(TObject *Sender);
  void __fastcall actHaveTranspColor_Update(TObject *Sender);
  void __fastcall actHaveTranspColor_Execute(TObject *Sender);
  void __fastcall actExportPalette_Update(TObject *Sender);
  void __fastcall cbxPaletteOutFmt_Change(TObject *Sender);
  void __fastcall actExportPalette_Execute(TObject *Sender);
  void __fastcall actEyeDropper_Update(TObject *Sender);
  void __fastcall actEyeDropper_Execute(TObject *Sender);
  void __fastcall img_Click(TObject *Sender);
  void __fastcall actSelectedClr_Update(TObject *Sender);
  void __fastcall gridPal_MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);

public:		// User declarations
	__fastcall TFrmMain(TComponent* Owner);

protected:
  int outputClrFmtGetFromCtl() const;
  int palOutputClrFmtGetFromCtl() const;

  static UnicodeString esguiClrfmtStringGet(int clrfmt);

	void updateImageInfo();
	void updateMemoryEstimate();
	void updatePaletteControls();
	void updateSelectionControls();
	void updatePaletteView();

  void bitmapExport(const String& file);

  void updatePalette();
  void updateResult();

  void updateTransparencyControls();

  int palIdxGetFromMousePos(int x, int y);

  void outputBaseNameUpdate();

protected:
  UnicodeString m_srcFileName;
	std::unique_ptr<Graphics::TBitmap> m_orig;
  std::unique_ptr<Graphics::TBitmap> m_result;
  RGBQUAD m_pal[256];
  size_t m_palCnt;
  TColor m_curClr;
  esU32 m_selClr;
	int m_bpp;
  bool m_canUseTranspColor;
  bool m_useTranspColor;
  bool m_transpColorSelected;
  bool m_canExportPalette;
  bool m_exportPalette;
  bool m_eyeDropperActive;
  bool m_transpColorIsPalIdx;
};
//---------------------------------------------------------------------------

extern PACKAGE TFrmMain *FrmMain;
//---------------------------------------------------------------------------
#endif
