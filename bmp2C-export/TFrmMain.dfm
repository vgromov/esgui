object FrmMain: TFrmMain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  Caption = 'Export bitmaps as C code'
  ClientHeight = 572
  ClientWidth = 873
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = mnuMain_
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object pnlContents_: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 867
    Height = 566
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object pnlTop_: TPanel
      Left = 0
      Top = 0
      Width = 867
      Height = 49
      Align = alTop
      BevelOuter = bvNone
      TabOrder = 0
      object bvl1_: TBevel
        Left = 408
        Top = 0
        Width = 5
        Height = 49
        Align = alLeft
        Shape = bsSpacer
        ExplicitLeft = 414
      end
      object gbxSelection_: TGroupBox
        Left = 0
        Top = 0
        Width = 408
        Height = 49
        Align = alLeft
        Caption = 'Selection:'
        TabOrder = 0
        object lblLeft_: TLabel
          Left = 9
          Top = 22
          Width = 23
          Height = 13
          Caption = 'Left:'
          FocusControl = edLeft_
        end
        object lblBottom_: TLabel
          Left = 295
          Top = 22
          Width = 38
          Height = 13
          Caption = 'Bottom:'
          FocusControl = edBottom_
        end
        object lblRight_: TLabel
          Left = 195
          Top = 22
          Width = 29
          Height = 13
          Caption = 'Right:'
          FocusControl = edRight_
        end
        object lblTop_: TLabel
          Left = 103
          Top = 22
          Width = 22
          Height = 13
          Caption = 'Top:'
          FocusControl = edTop_
        end
        object edTop_: TCSpinEdit
          Left = 127
          Top = 18
          Width = 60
          Height = 22
          TabOrder = 1
          OnChange = edTop_Change
        end
        object edLeft_: TCSpinEdit
          Left = 38
          Top = 18
          Width = 60
          Height = 22
          TabOrder = 0
          OnChange = edLeft_Change
        end
        object edRight_: TCSpinEdit
          Left = 228
          Top = 19
          Width = 60
          Height = 22
          TabOrder = 2
          OnChange = edRight_Change
        end
        object edBottom_: TCSpinEdit
          Left = 336
          Top = 18
          Width = 60
          Height = 22
          TabOrder = 3
          OnChange = edBottom_Change
        end
      end
      object gbxInfo_: TGroupBox
        Left = 413
        Top = 0
        Width = 454
        Height = 49
        Align = alClient
        Caption = 'Image and selection info:'
        Padding.Left = 2
        Padding.Top = 2
        Padding.Right = 2
        Padding.Bottom = 2
        TabOrder = 1
        object lblImageInfo_: TLabel
          Left = 4
          Top = 17
          Width = 446
          Height = 28
          Align = alClient
          AutoSize = False
          ExplicitLeft = 408
          ExplicitTop = 0
          ExplicitWidth = 245
          ExplicitHeight = 49
        end
      end
    end
    object gbxExport_: TGroupBox
      Left = 511
      Top = 49
      Width = 356
      Height = 517
      Align = alRight
      Caption = 'Export settings:'
      Padding.Left = 4
      Padding.Top = 4
      Padding.Right = 4
      Padding.Bottom = 4
      TabOrder = 1
      object gbxPalette_: TGroupBox
        Left = 6
        Top = 193
        Width = 344
        Height = 200
        Align = alBottom
        Caption = 'Palette:'
        TabOrder = 0
        object gridPal_: TDrawGrid
          Left = 2
          Top = 80
          Width = 340
          Height = 118
          Align = alBottom
          ColCount = 1
          DefaultColWidth = 20
          DefaultRowHeight = 20
          DefaultDrawing = False
          FixedCols = 0
          RowCount = 1
          FixedRows = 0
          TabOrder = 0
          Visible = False
          OnDrawCell = gridPal_DrawCell
          OnMouseUp = gridPal_MouseUp
        end
        object layPalette_: TGridPanel
          Left = 2
          Top = 15
          Width = 340
          Height = 65
          Align = alClient
          BevelOuter = bvNone
          ColumnCollection = <
            item
              SizeStyle = ssAbsolute
              Value = 100.000000000000000000
            end
            item
              Value = 100.000000000000000000
            end>
          ControlCollection = <
            item
              Column = 0
              ColumnSpan = 2
              Control = chkExportPalette_
              Row = 0
            end
            item
              Column = 0
              Control = lblPalOutputFmt_
              Row = 1
            end
            item
              Column = 1
              Control = cbxPaletteOutFmt_
              Row = 1
            end>
          ExpandStyle = emFixedSize
          RowCollection = <
            item
              SizeStyle = ssAbsolute
              Value = 30.000000000000000000
            end
            item
              Value = 100.000000000000000000
            end>
          ShowCaption = False
          TabOrder = 1
          object chkExportPalette_: TCheckBox
            AlignWithMargins = True
            Left = 3
            Top = 3
            Width = 334
            Height = 24
            Action = actExportPalette_
            Align = alClient
            TabOrder = 0
          end
          object lblPalOutputFmt_: TLabel
            AlignWithMargins = True
            Left = 3
            Top = 33
            Width = 94
            Height = 29
            Align = alClient
            Caption = 'Palette output format:'
            WordWrap = True
            ExplicitWidth = 72
            ExplicitHeight = 26
          end
          object cbxPaletteOutFmt_: TComboBox
            AlignWithMargins = True
            Left = 103
            Top = 33
            Width = 232
            Height = 21
            Style = csDropDownList
            TabOrder = 1
            OnChange = cbxPaletteOutFmt_Change
            Items.Strings = (
              '8 bpp (RGB332)'
              '8 bpp (BGR233)'
              '16 bpp (RGB565)'
              '16 bpp (BGR565)'
              '24 bpp (RGB888)'
              '24 bpp (BGR888)'
              '32 bpp (RGBA8888)'
              '32 bpp (BGRA8888)'
              '32 bpp (ARGB8888)'
              '32 bpp (ABGR8888)')
          end
        end
      end
      object gbxEstimate_: TGroupBox
        Left = 6
        Top = 393
        Width = 344
        Height = 118
        Align = alBottom
        Caption = 'Memory estimate:'
        Padding.Left = 2
        Padding.Top = 2
        Padding.Right = 2
        Padding.Bottom = 2
        TabOrder = 1
        object lblEstimate_: TLabel
          Left = 4
          Top = 17
          Width = 336
          Height = 97
          Align = alClient
          AutoSize = False
          ExplicitTop = 4
          ExplicitWidth = 420
          ExplicitHeight = 55
        end
      end
      object laySettings_: TGridPanel
        Left = 6
        Top = 19
        Width = 344
        Height = 174
        Align = alClient
        BevelOuter = bvNone
        ColumnCollection = <
          item
            SizeStyle = ssAbsolute
            Value = 100.000000000000000000
          end
          item
            Value = 100.000000000000000000
          end>
        ControlCollection = <
          item
            Column = 0
            Control = lblOutFmt_
            Row = 0
          end
          item
            Column = 1
            Control = cbxOutFmt_
            Row = 0
          end
          item
            Column = 0
            Control = lblBaseName_
            Row = 1
          end
          item
            Column = 1
            Control = edBaseName_
            Row = 1
          end
          item
            Column = 0
            ColumnSpan = 2
            Control = chkHaveTransparentColor_
            Row = 2
          end
          item
            Column = 0
            ColumnSpan = 2
            Control = lblTransparentColor_
            Row = 3
          end
          item
            Column = 1
            Control = btnEyedropper_
            Row = 4
          end
          item
            Column = 0
            Control = lblSelectedClr_
            Row = 4
          end>
        ExpandStyle = emFixedSize
        RowCollection = <
          item
            SizeStyle = ssAbsolute
            Value = 30.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 30.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 30.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 34.000000000000000000
          end
          item
            SizeStyle = ssAbsolute
            Value = 34.000000000000000000
          end>
        ShowCaption = False
        TabOrder = 2
        object lblOutFmt_: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 3
          Width = 94
          Height = 24
          Align = alClient
          Caption = 'Output Format:'
          FocusControl = cbxOutFmt_
          ExplicitWidth = 75
          ExplicitHeight = 13
        end
        object cbxOutFmt_: TComboBox
          AlignWithMargins = True
          Left = 103
          Top = 3
          Width = 236
          Height = 21
          Style = csDropDownList
          TabOrder = 0
          OnChange = cbxOutFmt_Change
          Items.Strings = (
            '1 bpp'
            '4 bpp (palette)'
            '8 bpp (palette)'
            '2 bpp (grayscale)'
            '4 bpp (grayscale)'
            '8 bpp (grayscale)'
            '8 bpp (RGB332)'
            '8 bpp (RGB233)'
            '16 bpp (RGB565)'
            '16 bpp (BGR565)'
            '24 bpp (RGB888)'
            '24 bpp (BGR888)'
            '32 bpp (RGBA8888)'
            '32 bpp (BGRA8888)'
            '32 bpp (ARGB8888)'
            '32 bpp (ABGR8888)')
        end
        object lblBaseName_: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 33
          Width = 94
          Height = 24
          Align = alClient
          Caption = 'C base name:'
          FocusControl = edBaseName_
          ExplicitWidth = 66
          ExplicitHeight = 13
        end
        object edBaseName_: TEdit
          AlignWithMargins = True
          Left = 103
          Top = 33
          Width = 236
          Height = 21
          TabOrder = 1
          Text = 'bitmap'
        end
        object chkHaveTransparentColor_: TCheckBox
          AlignWithMargins = True
          Left = 3
          Top = 63
          Width = 338
          Height = 24
          Hint = 'Bitmap has distinct color specified as a transparent one'
          Action = actHaveTranspColor_
          Align = alClient
          TabOrder = 2
        end
        object lblTransparentColor_: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 98
          Width = 338
          Height = 23
          Margins.Top = 8
          Align = alClient
          Caption = 'Select transparent color:'
          WordWrap = True
          ExplicitWidth = 119
          ExplicitHeight = 13
        end
        object btnEyedropper_: TSpeedButton
          AlignWithMargins = True
          Left = 103
          Top = 127
          Width = 238
          Height = 28
          Action = actEyeDropper_
          Align = alClient
          ExplicitLeft = 9
          ExplicitTop = 7
          ExplicitWidth = 34
          ExplicitHeight = 34
        end
        object lblSelectedClr_: TLabel
          AlignWithMargins = True
          Left = 3
          Top = 127
          Width = 94
          Height = 28
          Align = alClient
          ExplicitWidth = 3
          ExplicitHeight = 13
        end
      end
    end
    object pgImages_: TPageControl
      Left = 0
      Top = 49
      Width = 511
      Height = 517
      ActivePage = tabResult_
      Align = alClient
      TabOrder = 2
      object tabOriginal_: TTabSheet
        Caption = 'Original:'
        object viewImage_: TScrollBox
          Left = 0
          Top = 0
          Width = 503
          Height = 489
          Align = alClient
          BorderStyle = bsNone
          ParentBackground = True
          TabOrder = 0
          object pbxOrigView_: TPaintBox
            Left = 0
            Top = 0
            Width = 105
            Height = 105
            OnMouseMove = pbxImage_MouseMove
            OnPaint = pbxOrigView_Paint
          end
        end
      end
      object tabResult_: TTabSheet
        Caption = 'Result:'
        ImageIndex = 1
        object viewResult_: TScrollBox
          Left = 0
          Top = 0
          Width = 503
          Height = 489
          Align = alClient
          BorderStyle = bsNone
          ParentBackground = True
          TabOrder = 0
          object pbxResult_: TPaintBox
            Left = 0
            Top = 0
            Width = 105
            Height = 105
            OnClick = img_Click
            OnMouseMove = pbxImage_MouseMove
            OnPaint = pbxResult_Paint
          end
        end
      end
    end
  end
  object dlgOpen_: TOpenPictureDialog
    Left = 16
    Top = 336
  end
  object mnuMain_: TMainMenu
    Images = imgs_
    Left = 68
    Top = 336
    object File1: TMenuItem
      Caption = 'File'
      object Fileopen1: TMenuItem
        Action = actFileOpen_
      end
      object Export1: TMenuItem
        Action = actFileExport_
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Action = actFileExit_
      end
    end
  end
  object lstCmds_: TActionList
    Images = imgs_
    Left = 116
    Top = 336
    object actFileOpen_: TAction
      Category = 'File'
      Caption = 'File open'
      OnExecute = actFileOpen_Execute
    end
    object actFileExit_: TAction
      Category = 'File'
      Caption = 'Exit'
      OnExecute = actFileExit_Execute
    end
    object actFileExport_: TAction
      Category = 'File'
      Caption = 'Export'
      OnExecute = actFileExport_Execute
      OnUpdate = actFileExport_Update
    end
    object actHaveTranspColor_: TAction
      Category = 'Controls'
      Caption = 'Bitmap Has transparent color'
      OnExecute = actHaveTranspColor_Execute
      OnUpdate = actHaveTranspColor_Update
    end
    object actExportPalette_: TAction
      Category = 'Controls'
      Caption = 'Export palette'
      OnExecute = actExportPalette_Execute
      OnUpdate = actExportPalette_Update
    end
    object actEyeDropper_: TAction
      Category = 'Controls'
      Caption = 'Eye-dropper tool'
      Hint = 'Activate to select RGB color under the mouse pointer'
      OnExecute = actEyeDropper_Execute
      OnUpdate = actEyeDropper_Update
    end
    object actSelectedClr_: TControlAction
      Category = 'Controls'
      OnUpdate = actSelectedClr_Update
    end
  end
  object dlgExport_: TSaveDialog
    DefaultExt = '*.cc'
    Filter = 'C code files|*.cc'
    Left = 15
    Top = 289
  end
  object imgs_: TImageList
    Left = 160
    Top = 336
  end
end
