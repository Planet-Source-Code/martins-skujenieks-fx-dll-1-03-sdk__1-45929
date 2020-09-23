Attribute VB_Name = "FX"


'   ========================================================================
'    FX Module Information
'   ========================================================================
'
'    File Version:      1.03
'    Description:       FX.DLL Procedure Declarations, Constant and Tag
'                       Definitions
'    Copyright:         Copyright © Martins Skujenieks 2003
'    Product Name:      FX.DLL
'    Product Version:   1.03
'
'
'   ========================================================================
'    End User License Agreement (EULA)
'   ========================================================================
'
'    This product is provided "as is", with no guarantee of completeness or
'    accuracy and without warranty of any kind, express or implied.
'
'    In no event will developer be liable for damages of any kind that may
'    be incurred with your hardware, peripherals or software programs.
'
'    This product and all of its parts may not be copied, emulated, cloned,
'    rented, leased, sold, reproduced, modified, decompiled, disassembled,
'    otherwise reverse engineered, republished, uploaded, posted,
'    transmitted or distributed in any way, without prior written consent
'    of the developer.
'
'
'   ========================================================================
'    Contact Developer
'   ========================================================================
'
'    Website:           http://www.exe.times.lv
'    E-Mail:            martins_s@mail.teliamtc.lv
'
'   ========================================================================


Option Explicit


    '/* Ternary Raster Operations */
    Public Const SRCCOPY = &HCC0020
    Public Const SRCPAINT = &HEE0086
    Public Const SRCAND = &H8800C6
    Public Const SRCINVERT = &H660046
    Public Const SRCERASE = &H440328
    Public Const NOTSRCCOPY = &H330008
    Public Const NOTSRCERASE = &H1100A6
    Public Const MERGECOPY = &HC000CA
    Public Const MERGEPAINT = &HBB0226
    Public Const PATCOPY = &HF00021
    Public Const PATPAINT = &HFB0A09
    Public Const PATINVERT = &H5A0049
    Public Const DSTINVERT = &H550009
    Public Const BLACKNESS = &H42
    Public Const WHITENESS = &HFF0062


    '/* Text Alignment Options */
    Public Const TA_NOUPDATECP = 0
    Public Const TA_UPDATECP = 1
    Public Const TA_LEFT = 0
    Public Const TA_RIGHT = 2
    Public Const TA_CENTER = 6
    Public Const TA_TOP = 0
    Public Const TA_BOTTOM = 8
    Public Const TA_BASELINE = 24
    Public Const TA_RTLREADING = 256
    Public Const TA_MASK = (TA_BASELINE + TA_CENTER + TA_UPDATECP + TA_RTLREADING)
    
    
    '/* Vertical Text Alignment Options */
    Public Const VTA_BASELINE = TA_BASELINE
    Public Const VTA_LEFT = TA_BOTTOM
    Public Const VTA_RIGHT = TA_TOP
    Public Const VTA_CENTER = TA_CENTER
    Public Const VTA_BOTTOM = TA_RIGHT
    Public Const VTA_TOP = TA_LEFT


    '/* struct tagPOINT */
    Public Type POINT
        X       As Long
        Y       As Long
    End Type

    
    '/* struct tagRECT */
    Public Type RECT
        Left    As Long
        Top     As Long
        Right   As Long
        Bottom  As Long
    End Type


    '/* Function prototypes */
    Public Declare Function fxAlphaBlend Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Blending As Long, ByVal TransparentColor As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxAmbientLight Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long, ByVal Intensity As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxAntiAlias Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxBitBlt Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal RasterOperation As Long) As Long
    Public Declare Function fxBlur Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxBrightness Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Brightness As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxCMYK Lib "fx.dll" (ByVal C As Long, ByVal M As Long, ByVal Y As Long, ByVal K As Long) As Long
    Public Declare Function fxDrawText Lib "fx.dll" (ByVal DC As Long, ByVal X As Long, ByVal Y As Long, ByVal Width As Long, ByVal Height As Long, ByVal Text As String, ByVal Color As Long, ByVal Font As Long, ByVal Blending As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxEmboss Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Depth As Double, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxEngrave Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Height As Double, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxGamma Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Gamma As Double, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxGetBlue Lib "fx.dll" (ByVal RGB As Long) As Long
    Public Declare Function fxGetGreen Lib "fx.dll" (ByVal RGB As Long) As Long
    Public Declare Function fxGetRed Lib "fx.dll" (ByVal RGB As Long) As Long
    Public Declare Function fxGreyscale Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxGridelines Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long, ByVal Transparency As Long, ByVal Step As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxHSLtoRGB Lib "fx.dll" (ByVal H As Double, ByVal S As Double, ByVal L As Double) As Long
    Public Declare Function fxHue Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Hue As Double, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxInversion Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Inversion As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxInvert Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxLight Lib "fx.dll" (ByVal DC As Long, ByVal X As Long, ByVal Y As Long, ByVal Color As Long, ByVal Extension As Long, ByVal Intensity As Long, ByVal Definition As Long, Optional ByVal Flags As Long = 1) As Long
    Public Declare Function fxMirror Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Horizontal As Boolean, ByVal Vertical As Boolean, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxMosaic Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Size As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxPaletteIndex Lib "fx.dll" (ByVal Index As Long) As Long
    Public Declare Function fxPaletteRGB Lib "fx.dll" (ByVal R As Long, ByVal G As Long, ByVal B As Long) As Long
    Public Declare Function fxReduceColors Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Level As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxRelief Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxRender Lib "fx.dll" (ByVal DestDC As Long, ByVal CenterX As Long, ByVal CenterY As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Blending As Long, ByVal Angle As Double, ByVal Zoom As Double, ByVal TransparentColor As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxReplaceColor Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long, ByVal ByColor As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxReplaceColors Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long, ByVal ByColor As Long, ByVal Similarity As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxResample Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Horizontal As Double, ByVal Vertical As Double, ByVal TransparentColor As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxRGB Lib "fx.dll" (ByVal R As Long, ByVal G As Long, ByVal B As Long) As Long
    Public Declare Function fxRGBtoHSL Lib "fx.dll" (ByVal RGB As Long, ByRef H As Double, ByRef S As Double, ByRef L As Double) As Long
    Public Declare Function fxRotate Lib "fx.dll" (ByVal DestDC As Long, ByVal CenterX As Long, ByVal CenterY As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Angle As Double, ByVal TransparentColor As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxSaturation Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Saturation As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxScanlines Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long, ByVal Transparency As Long, ByVal Step As Long, ByVal Horizontal As Boolean, ByVal Vertical As Boolean, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxScreenShot Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal GetCursor As Boolean, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxScroll Lib "fx.dll" (ByVal DC As Long, ByVal X As Long, ByVal Y As Long, ByVal Scroll As RECT, Clip As RECT, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxSemitransparentBlt Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long, ByVal Transparency As Long) As Long
    Public Declare Function fxShadeColors Lib "fx.dll" (ByVal DestColor As Long, ByVal SrcColor As Long, ByVal Shade As Long) As Long
    Public Declare Function fxStretchBlt Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal RasterOperation As Long) As Long
    Public Declare Function fxText Lib "fx.dll" (ByVal DC As Long, ByVal X As Long, ByVal Y As Long, ByVal Text As String, ByVal Color As Long, ByVal Alignment As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxTone Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long, ByVal Tone As Long, Optional ByVal Flags As Long = 0) As Long
    Public Declare Function fxTransparentBlt Lib "fx.dll" (ByVal DestDC As Long, ByVal DestX As Long, ByVal DestY As Long, ByVal DestWidth As Long, ByVal DestHeight As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Color As Long) As Long
    Public Declare Function fxZoom Lib "fx.dll" (ByVal DestDC As Long, ByVal CenterX As Long, ByVal CenterY As Long, ByVal SrcDC As Long, ByVal SrcX As Long, ByVal SrcY As Long, ByVal SrcWidth As Long, ByVal SrcHeight As Long, ByVal Zoom As Double, ByVal TransparentColor As Long, Optional ByVal Flags As Long = 0) As Long

