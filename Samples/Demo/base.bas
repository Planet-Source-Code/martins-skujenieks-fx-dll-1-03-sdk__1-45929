Attribute VB_Name = "Base"

Option Explicit


    'Demo variabls:
    Public fxIndex As Long
    
    Public fxX As Long
    Public fxY As Long
    
    Public fxColor As Long
    Public fxMaskColor As Long
    
    Public Declare Function GetTickCount Lib "kernel32" () As Long
    
    
    'Handle of font for DrawText:
    Public hFont As Long
    
    
Public Sub Update()

    Dim Ticks As Long

    With frmMain
    
        DoEvents
    
        .picDest.Cls
        
        Ticks = GetTickCount
    
        Select Case fxIndex
            Case 1000   'Alpha Blending
                fxAlphaBlend .picDest.hdc, 0, 0, 320, 240, .picSrc.hdc, 0, 0, 320, 240, .HScroll1(0).Value, vbWhite
            
            Case 1010   'Ambient Light
                fxAmbientLight .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, fxColor, .HScroll1(0).Value
                
            Case 1020   'Blur
                fxBlur .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240
            
            Case 1030   'Brightness
                fxBrightness .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value
            
            Case 1040   'Draw Text
                If Len(.Text1.Text) > 0 Then fxDrawText .picDest.hdc, 0, 0, 320, 40, .Text1.Text, fxColor, hFont, 191
            
            Case 1050   'Emboss
                fxEmboss .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value / 100
                
            Case 1060   'Engrave
                fxEngrave .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value / 100
                
            Case 1070   'Gamma
                fxGamma .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value / 100
                
            Case 1080   'Greyscale
                fxGreyscale .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240
                
            Case 1090   'Gridelines
                fxGridelines .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, fxColor, .HScroll1(0).Value, .HScroll1(1).Value
        
            Case 1100   'Hue
                fxHue .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value / 100
        
            Case 1110   'Inversion
                fxInversion .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value
        
            Case 1120   'Invert
                fxInvert .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240
        
            Case 1130   'Light
                fxLight .picDest.hdc, 160, 120, fxColor, 90, .HScroll1(0).Value, .HScroll1(1).Value, 1
        
            Case 1140   'Mosaic
                fxMosaic .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value
        
            Case 1150   'Reduce Colors
                fxReduceColors .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value
        
            Case 1160   'Relief
                fxRelief .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240
                
            Case 1170   'Render
                fxRender .picDest.hdc, fxX, fxY, .picSrc2.hdc, 0, 0, 32, 32, .HScroll1(2).Value, .HScroll1(0).Value, .HScroll1(1).Value / 100, vbMagenta
            
            Case 1180   'Replace Color
                fxReplaceColor .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, fxColor, vbBlack
                    
            Case 1190   'Replace Colors
                fxReplaceColors .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, fxColor, vbBlack, .HScroll1(0).Value

            Case 1200   'Rotate
                fxRotate .picDest.hdc, fxX, fxY, .picSrc2.hdc, 0, 0, 32, 32, .HScroll1(0).Value, vbMagenta

            Case 1210   'Saturation
                fxSaturation .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, .HScroll1(0).Value
       
            Case 1220   'Scanlines
                fxScanlines .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, fxColor, .HScroll1(0).Value, .HScroll1(1).Value, True, False

            Case 1240   'Text
                If Len(.Text1.Text) > 0 Then fxText .picDest.hdc, 0, 0, .Text1.Text, fxColor, TA_LEFT
                
            Case 1250   'Tone
                fxTone .picDest.hdc, 0, 0, 320, 240, .picDest.hdc, 0, 0, 320, 240, fxColor, .HScroll1(0).Value

            Case 1260   'Zoom
                fxZoom .picDest.hdc, fxX, fxY, .picSrc2.hdc, 0, 0, 32, 32, .HScroll1(0).Value / 100, vbMagenta

        End Select
        
        Ticks = GetTickCount - Ticks
        
        .picDest.Refresh
        
        frmMain.Label5.Caption = Ticks & " ms/frame"
            
        DoEvents
        
    End With

End Sub
