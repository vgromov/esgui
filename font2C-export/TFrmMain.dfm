object FrmMain: TFrmMain
  Left = 0
  Top = 0
  Caption = 'Export Windows font as C code.'
  ClientHeight = 500
  ClientWidth = 930
  Color = clBtnFace
  Constraints.MinHeight = 500
  Constraints.MinWidth = 800
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object pnlRight_: TPanel
    AlignWithMargins = True
    Left = 246
    Top = 3
    Width = 681
    Height = 494
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object pbxCloseup_: TPaintBox
      Left = 0
      Top = 0
      Width = 681
      Height = 494
      Align = alClient
      Visible = False
      OnDblClick = pbxCloseup_DblClick
      OnPaint = pbxCloseup_Paint
      ExplicitLeft = 424
      ExplicitTop = 216
      ExplicitWidth = 105
      ExplicitHeight = 105
    end
    object charMap_: TDrawGrid
      Left = 0
      Top = 0
      Width = 681
      Height = 494
      Align = alClient
      ColCount = 16
      DefaultColWidth = 40
      DefaultRowHeight = 48
      DefaultDrawing = False
      FixedCols = 0
      RowCount = 16
      FixedRows = 0
      PopupMenu = ctxMenu_
      TabOrder = 0
      OnDrawCell = charMap_DrawCell
      OnMouseDown = charMap_MouseDown
      OnMouseMove = charMap_MouseMove
    end
  end
  object pnlLeft_: TPanel
    AlignWithMargins = True
    Left = 3
    Top = 3
    Width = 237
    Height = 494
    Align = alLeft
    BevelOuter = bvNone
    TabOrder = 1
    object gbxSelectFont_: TGroupBox
      Left = 0
      Top = 0
      Width = 237
      Height = 161
      Align = alTop
      Caption = 'Font parameters:'
      TabOrder = 0
      object lblHeight_: TLabel
        Left = 11
        Top = 50
        Width = 35
        Height = 13
        Caption = 'Height:'
      end
      object lblBitDepth_: TLabel
        Left = 11
        Top = 84
        Width = 101
        Height = 13
        Caption = 'Font glyph bit depth:'
      end
      object gbxFontStyle_: TGroupBox
        Left = 136
        Top = 42
        Width = 93
        Height = 110
        Caption = 'Style:'
        TabOrder = 0
        object chkBold_: TCheckBox
          Left = 8
          Top = 18
          Width = 77
          Height = 17
          Caption = 'Bold'
          TabOrder = 0
          OnClick = chkBold_Click
        end
        object chkItalic_: TCheckBox
          Left = 8
          Top = 41
          Width = 77
          Height = 17
          Caption = 'Italic'
          TabOrder = 1
          OnClick = chkItalic_Click
        end
        object chkUnderline_: TCheckBox
          Left = 8
          Top = 64
          Width = 77
          Height = 17
          Caption = 'Underline'
          TabOrder = 2
          OnClick = chkUnderline_Click
        end
        object chkStrikeout_: TCheckBox
          Left = 8
          Top = 87
          Width = 77
          Height = 17
          Caption = 'Strike-out'
          TabOrder = 3
          OnClick = chkStrikeout_Click
        end
      end
      object edHeight_: TcxSpinEdit
        Left = 61
        Top = 47
        Properties.ImmediatePost = True
        Properties.MaxValue = 1024.000000000000000000
        Properties.MinValue = 1.000000000000000000
        Properties.OnEditValueChanged = edHeight_Change
        Style.LookAndFeel.NativeStyle = True
        StyleDisabled.LookAndFeel.NativeStyle = True
        StyleFocused.LookAndFeel.NativeStyle = True
        StyleHot.LookAndFeel.NativeStyle = True
        TabOrder = 1
        Value = 1
        Width = 63
      end
      object cbxFonts_: TcxFontNameComboBox
        Left = 11
        Top = 18
        Properties.DropDownSizeable = True
        Properties.FontPreview.Alignment = taLeftJustify
        Properties.FontPreview.ShowButtons = False
        Properties.ImmediatePost = True
        Properties.ImmediateUpdateText = True
        Properties.OnChange = cbxFonts_Change
        Style.LookAndFeel.NativeStyle = True
        StyleDisabled.LookAndFeel.NativeStyle = True
        StyleFocused.LookAndFeel.NativeStyle = True
        StyleHot.LookAndFeel.NativeStyle = True
        TabOrder = 2
        Width = 218
      end
      object cbxBitDepth_: TComboBox
        Left = 11
        Top = 103
        Width = 112
        Height = 21
        Style = csDropDownList
        TabOrder = 3
        OnChange = cbxBitDepth_Change
        Items.Strings = (
          '1 bpp (monochrome)'
          '2 bpp (antialias)'
          '4 bpp (antialias)'
          '8 bpp (antialias)')
      end
    end
    object gbxExportProps_: TGroupBox
      Left = 0
      Top = 296
      Width = 237
      Height = 119
      Align = alTop
      Caption = 'Export properties:'
      TabOrder = 1
      object edFontCName_: TLabeledEdit
        Left = 11
        Top = 91
        Width = 218
        Height = 21
        EditLabel.Width = 116
        EditLabel.Height = 13
        EditLabel.Caption = 'Font C data base name:'
        TabOrder = 0
      end
      object gbxCropParams_: TGroupBox
        Left = 11
        Top = 17
        Width = 218
        Height = 48
        Caption = 'Crop:'
        TabOrder = 1
        object lblCropTop_: TLabel
          Left = 8
          Top = 22
          Width = 22
          Height = 13
          Caption = 'Top:'
        end
        object lblCropBottom_: TLabel
          Left = 108
          Top = 22
          Width = 38
          Height = 13
          Caption = 'Bottom:'
        end
        object edCropTop_: TcxSpinEdit
          Left = 36
          Top = 19
          Properties.AssignedValues.MaxValue = True
          Properties.AssignedValues.MinValue = True
          Properties.ImmediatePost = True
          Properties.OnEditValueChanged = edCropTop_ValueChanged
          Style.LookAndFeel.NativeStyle = True
          StyleDisabled.LookAndFeel.NativeStyle = True
          StyleFocused.LookAndFeel.NativeStyle = True
          StyleHot.LookAndFeel.NativeStyle = True
          TabOrder = 0
          Width = 63
        end
        object edCropBottom_: TcxSpinEdit
          Left = 150
          Top = 19
          Properties.AssignedValues.MaxValue = True
          Properties.AssignedValues.MinValue = True
          Properties.ImmediatePost = True
          Properties.OnEditValueChanged = edCropBottom_ValueChanged
          Style.LookAndFeel.NativeStyle = True
          StyleDisabled.LookAndFeel.NativeStyle = True
          StyleFocused.LookAndFeel.NativeStyle = True
          StyleHot.LookAndFeel.NativeStyle = True
          TabOrder = 1
          Width = 63
        end
      end
    end
    object Button1: TButton
      Left = 6
      Top = 421
      Width = 75
      Height = 25
      Action = actExport_
      TabOrder = 2
    end
    object gbxCharParams_: TGroupBox
      Left = 0
      Top = 161
      Width = 237
      Height = 135
      Align = alTop
      Caption = 'Character parameters'
      TabOrder = 3
      object lblUnicodeScript_: TLabel
        AlignWithMargins = True
        Left = 5
        Top = 18
        Width = 227
        Height = 13
        Align = alTop
        Caption = 'Unicode script:'
        ExplicitWidth = 71
      end
      object lblSpaceWidth_: TLabel
        AlignWithMargins = True
        Left = 5
        Top = 67
        Width = 85
        Height = 13
        Caption = 'Space width (px):'
      end
      object lblNullWidth_: TLabel
        AlignWithMargins = True
        Left = 6
        Top = 98
        Width = 73
        Height = 13
        Caption = 'Null width (px):'
      end
      object cbxUnicodeScripts_: TComboBox
        AlignWithMargins = True
        Left = 5
        Top = 37
        Width = 227
        Height = 21
        Align = alTop
        Style = csDropDownList
        TabOrder = 0
        OnChange = cbxUnicodeScripts_Change
      end
      object csSpaceWidth_: TcxSpinEdit
        AlignWithMargins = True
        Left = 96
        Top = 64
        Properties.ImmediatePost = True
        Properties.MaxValue = 128.000000000000000000
        Properties.MinValue = 2.000000000000000000
        Properties.OnEditValueChanged = csSpaceWidth_PropertiesEditValueChanged
        Style.LookAndFeel.NativeStyle = True
        StyleDisabled.LookAndFeel.NativeStyle = True
        StyleFocused.LookAndFeel.NativeStyle = True
        StyleHot.LookAndFeel.NativeStyle = True
        TabOrder = 1
        Value = 2
        Width = 63
      end
      object csNullWidth_: TcxSpinEdit
        AlignWithMargins = True
        Left = 96
        Top = 91
        Properties.ImmediatePost = True
        Properties.MaxValue = 128.000000000000000000
        Properties.MinValue = 4.000000000000000000
        Properties.OnEditValueChanged = csNullWidth_PropertiesEditValueChanged
        Style.LookAndFeel.NativeStyle = True
        StyleDisabled.LookAndFeel.NativeStyle = True
        StyleFocused.LookAndFeel.NativeStyle = True
        StyleHot.LookAndFeel.NativeStyle = True
        TabOrder = 2
        Value = 4
        Width = 63
      end
    end
  end
  object actsCmds_: TActionList
    Left = 357
    Top = 439
    object actExport_: TAction
      Caption = 'Export'
      ShortCut = 16453
      OnExecute = actExport_Execute
    end
    object actDeselectAll_: TAction
      Caption = 'Deselect All'
      OnExecute = actDeselectAll_Execute
      OnUpdate = actDeselectAll_Update
    end
    object actBlockDeselect_: TAction
      Caption = 'Deselect Block'
      OnExecute = actBlockDeselect_Execute
      OnUpdate = actBlockDeselect_Update
    end
    object actSymbolViewZoomed_: TAction
      Caption = 'Closeup view'
      Hint = 'View symbol cell zoomed'
      OnExecute = actSymbolViewZoomed_Execute
      OnUpdate = actSymbolViewZoomed_Update
    end
  end
  object dlgExport_: TSaveDialog
    DefaultExt = '*.cc'
    Filter = 'C code include file|*.cc'
    Left = 284
    Top = 434
  end
  object ctxMenu_: TPopupMenu
    Left = 424
    Top = 440
    object DeselectAll1: TMenuItem
      Action = actDeselectAll_
    end
    object DeselectBlock1: TMenuItem
      Action = actBlockDeselect_
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object Closeup1: TMenuItem
      Action = actSymbolViewZoomed_
    end
  end
end
