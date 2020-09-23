VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "FX.DLL 1.03 Demo"
   ClientHeight    =   5775
   ClientLeft      =   150
   ClientTop       =   435
   ClientWidth     =   5055
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   186
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   385
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   337
   StartUpPosition =   2  'CenterScreen
   Begin MSComDlg.CommonDialog CD 
      Left            =   1320
      Top             =   6480
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.PictureBox Container 
      Height          =   1095
      Left            =   120
      ScaleHeight     =   1035
      ScaleWidth      =   1035
      TabIndex        =   10
      Top             =   5880
      Width           =   1095
      Begin VB.PictureBox picSrc 
         AutoRedraw      =   -1  'True
         AutoSize        =   -1  'True
         Height          =   3660
         Left            =   0
         Picture         =   "frmMain.frx":0000
         ScaleHeight     =   240
         ScaleMode       =   3  'Pixel
         ScaleWidth      =   320
         TabIndex        =   11
         Top             =   0
         Width           =   4860
      End
   End
   Begin VB.PictureBox picSrc2 
      AutoRedraw      =   -1  'True
      AutoSize        =   -1  'True
      BackColor       =   &H00FF00FF&
      Height          =   540
      Left            =   1320
      Picture         =   "frmMain.frx":382D
      ScaleHeight     =   32
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   32
      TabIndex        =   7
      Top             =   5880
      Width           =   540
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Index           =   2
      Left            =   120
      Max             =   255
      TabIndex        =   5
      Top             =   5400
      Width           =   4815
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Index           =   1
      Left            =   120
      Max             =   255
      TabIndex        =   4
      Top             =   4800
      Width           =   4815
   End
   Begin VB.PictureBox picDest 
      AutoRedraw      =   -1  'True
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   21.75
         Charset         =   186
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   3600
      Left            =   120
      MouseIcon       =   "frmMain.frx":3C25
      MousePointer    =   99  'Custom
      Picture         =   "frmMain.frx":3F2F
      ScaleHeight     =   236
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   316
      TabIndex        =   1
      Top             =   240
      Width           =   4800
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   120
      TabIndex        =   12
      Top             =   4200
      Visible         =   0   'False
      Width           =   4815
   End
   Begin VB.HScrollBar HScroll1 
      Height          =   255
      Index           =   0
      Left            =   120
      Max             =   255
      TabIndex        =   3
      Top             =   4200
      Width           =   4815
   End
   Begin VB.Label Label5 
      Alignment       =   1  'Right Justify
      Caption         =   "0 ms/frame"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   186
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   3480
      TabIndex        =   9
      Top             =   0
      Width           =   1455
   End
   Begin VB.Label Label4 
      Caption         =   "N/a"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   5160
      Width           =   1575
   End
   Begin VB.Label Label3 
      Caption         =   "N/a"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   4560
      Width           =   1575
   End
   Begin VB.Label Label2 
      Caption         =   "Blending: 0"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   3960
      Width           =   1575
   End
   Begin VB.Label Label1 
      Caption         =   "Alpha Blending"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   186
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   3135
   End
   Begin VB.Menu mnuFile 
      Caption         =   "File"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "Open..."
      End
      Begin VB.Menu mnuFileSep 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "Exit"
      End
   End
   Begin VB.Menu menuFX 
      Caption         =   "Effect"
      Begin VB.Menu mnuFX 
         Caption         =   "Alpha Blending"
         Index           =   1000
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Ambient Light"
         Index           =   1010
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Blur"
         Index           =   1020
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Brightness"
         Index           =   1030
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Draw Text"
         Index           =   1040
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Emboss"
         Index           =   1050
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Engrave"
         Index           =   1060
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Gamma"
         Index           =   1070
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Greyscale"
         Index           =   1080
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Gridelines"
         Index           =   1090
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Hue"
         Index           =   1100
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Inversion"
         Index           =   1110
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Invert"
         Index           =   1120
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Light"
         Index           =   1130
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Mosaic"
         Index           =   1140
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Reduce Colors"
         Index           =   1150
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Relief"
         Index           =   1160
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Render"
         Index           =   1170
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Replace Color"
         Index           =   1180
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Replace Colors"
         Index           =   1190
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Rotate"
         Index           =   1200
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Saturation"
         Index           =   1210
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Scanlines"
         Index           =   1220
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Scroll"
         Enabled         =   0   'False
         Index           =   1230
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Text"
         Index           =   1240
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Tone"
         Index           =   1250
      End
      Begin VB.Menu mnuFX 
         Caption         =   "Zoom"
         Index           =   1260
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "Help"
      Begin VB.Menu mnuHelpVote 
         Caption         =   "Vote at Planet-Source-Code"
      End
      Begin VB.Menu mnuHelpSep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHelpWebsite 
         Caption         =   "Visit Developer's Website"
      End
      Begin VB.Menu mnuHelpEMail 
         Caption         =   "E-Mail Developer"
      End
      Begin VB.Menu mnuHelp2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "About..."
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

    Private Const HWND_TOPMOST = -1
    Private Const HWND_NOTOPMOST = -2
    Private Const SWP_NOSIZE = &H1
    Private Const SWP_NOMOVE = &H2
    Private Const SWP_NOACTIVATE = &H10
    Private Const SWP_SHOWWINDOW = &H40
    
    Private Declare Sub SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long)
    Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
    
    Private Declare Function GetDC Lib "user32" (ByVal hwnd As Long) As Long
    Private Declare Function GetDeviceCaps Lib "gdi32" (ByVal hdc As Long, ByVal nIndex As Long) As Long
    Private Declare Function MulDiv Lib "kernel32" (ByVal nNumber As Long, ByVal nNumerator As Long, ByVal nDenominator As Long) As Long
    Private Declare Function CreateFont Lib "gdi32" Alias "CreateFontA" (ByVal H As Long, ByVal W As Long, ByVal E As Long, ByVal O As Long, ByVal W As Long, ByVal I As Long, ByVal u As Long, ByVal S As Long, ByVal C As Long, ByVal OP As Long, ByVal CP As Long, ByVal Q As Long, ByVal PAF As Long, ByVal F As String) As Long


Private Sub Form_Load()
On Error Resume Next

    hFont = CreateFontObj

    Me.Show
    DoEvents

    Load frmColor: frmColor.Left = Me.Left + Me.Width: frmColor.Top = Me.Top
    SetWindowPos frmMain.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE Or SWP_SHOWWINDOW Or SWP_NOMOVE Or SWP_NOSIZE
    DoEvents
    
    mnuFX_Click 1000
    DoEvents
    
End Sub


Private Sub Form_Unload(Cancel As Integer)
On Error Resume Next

    'Unload forms:
    Unload frmColor
    Unload Me
    
    'Release system resources:
    Set frmColor = Nothing
    Set frmMain = Nothing
    
    'Terminate all processes:
    End

End Sub

Private Sub HScroll1_Change(Index As Integer)
    HScroll1_Scroll Index
End Sub


Private Sub HScroll1_Scroll(Index As Integer)

    Select Case fxIndex
        Case 1000   'Alpha Blending
            Label2.Caption = "Blending: " & HScroll1(0).Value
            
        Case 1010   'Ambient Light
            Label2.Caption = "Intensity: " & HScroll1(0).Value
                
        Case 1020   'Blur
        
        
        Case 1030   'Brightness
            Label2.Caption = "Brightness: " & HScroll1(0).Value
            
        Case 1040   'Draw Text
        
        Case 1050   'Emboss
            Label2.Caption = "Depth: " & HScroll1(0).Value / 100

        Case 1060   'Engrave
            Label2.Caption = "Height: " & HScroll1(0).Value / 100

        Case 1070   'Gamma
            Label2.Caption = "Gamma: " & HScroll1(0).Value / 100
            
        Case 1080   'Greyscale
            
            
        Case 1090   'Gridelines
            Label2.Caption = "Transparency: " & HScroll1(0).Value
            Label3.Caption = "Size: " & HScroll1(1).Value
            
        Case 1100   'Hue
            Label2.Caption = "Hue: " & HScroll1(0).Value / 100
            
        Case 1110   'Inversion
            Label2.Caption = "Inversion: " & HScroll1(0).Value
            
        Case 1120   'Invert
        
            
        Case 1130   'Light
            Label2.Caption = "Intensity: " & HScroll1(0).Value
            Label3.Caption = "Definition: " & HScroll1(1).Value
            
        Case 1140   'Mosaic
            Label2.Caption = "Size: " & HScroll1(0).Value
            
        Case 1150   'Reduce Colors
            Label2.Caption = "Level: " & HScroll1(0).Value
            
        Case 1160   'Relief
    
        Case 1170   'Render
            Label2.Caption = "Angle: " & HScroll1(0).Value
            Label3.Caption = "Zoom: " & HScroll1(1).Value / 100
            Label4.Caption = "Blending: " & HScroll1(2).Value
            
        Case 1180   'Replace Color
            
        Case 1190   'Replace Colors
            Label2.Caption = "Similarity: " & HScroll1(0).Value
            
        Case 1200   'Rotate
            Label2.Caption = "Angle: " & HScroll1(0).Value

        Case 1210   'Saturation
            Label2.Caption = "Saturation: " & HScroll1(0).Value

        Case 1220   'Scanlines
            Label2.Caption = "Transparency: " & HScroll1(0).Value
            Label3.Caption = "Size: " & HScroll1(1).Value

        Case 1250   'Tone
            Label2.Caption = "Tone: " & HScroll1(0).Value
            
        Case 1260   'Zoom
            Label2.Caption = "Zoom: " & HScroll1(0).Value / 100

    End Select

    DoEvents

    Update
    
End Sub


Private Sub mnuFileExit_Click()
    Form_Unload 0
End Sub


Private Sub mnuFileOpen_Click()
    With CD
        .CancelError = False
        .DialogTitle = "Open..."
        .Filter = "Compatible Image Files|*.bmp;*.jpg;*.gif"
        .Flags = cdlOFNHideReadOnly
        .ShowOpen
        If Len(.FileName) > 0 Then
            picDest.Picture = LoadPicture(.FileName)
        End If
    End With
End Sub


Private Sub mnuFX_Click(Index As Integer)

    If Text1.Visible = True Then Text1.Visible = False

    HScroll1(1).Value = 0
    HScroll1(2).Value = 0
    
    If HScroll1(1).Enabled = True Then HScroll1(1).Enabled = False
    If HScroll1(2).Enabled = True Then HScroll1(2).Enabled = False
    
    DoEvents
    
    
    For fxIndex = 1000 To 1260 Step 10
        mnuFX.Item(fxIndex).Checked = False
    Next
    
    fxIndex = Index
    
    mnuFX.Item(fxIndex).Checked = True
    Label1.Caption = mnuFX(fxIndex).Caption
    
    
    Select Case fxIndex
    
        Case 1000   'Alpha Blending
            frmColor.Visible = False
            Label2.Caption = "Blending: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1010   'Ambient Light
            frmColor.Visible = True
            Label2.Caption = "Intensity: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With

        Case 1020   'Blur
            frmColor.Visible = False
        
        Case 1030   'Brightness
            frmColor.Visible = False
            Label2.Caption = "Brightness: 0"
            With HScroll1(0)
                .Max = 255
                .Min = -255
                .Value = 0
            End With

        Case 1040   'DrawText
            Label2.Caption = "Text:"
            frmColor.Visible = True
            Text1.Visible = True
            
        
        Case 1050   'Emboss
            frmColor.Visible = False
            Label2.Caption = "Depth: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1060   'Engrave
            frmColor.Visible = False
            Label2.Caption = "Height: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1070   'Gamma
            frmColor.Visible = False
            Label2.Caption = "Gamma: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 100
            End With
            
        Case 1080   'Greyscale
            frmColor.Visible = False
        
        Case 1090   'Gridelines
            frmColor.Visible = True
            Label2.Caption = "Transparency: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 128
            End With
            Label3.Caption = "Size: 0"
            With HScroll1(1)
                .Enabled = True
                .Max = 24
                .Min = 0
                .Value = 4
            End With

        Case 1100   'Hue
            frmColor.Visible = False
            Label2.Caption = "Hue: 0"
            With HScroll1(0)
                .Max = 100
                .Min = -100
                .Value = 0
            End With
            
        Case 1110   'Inversion
            frmColor.Visible = False
            Label2.Caption = "Inversion: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1120   'Invert
            frmColor.Visible = False
            
        Case 1130   'Light
            frmColor.Visible = True
            Label2.Caption = "Intensity: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            With HScroll1(1)
                .Enabled = True
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1140   'Mosaic
            frmColor.Visible = False
            Label2.Caption = "Size: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1150   'Reduce Colors
            frmColor.Visible = False
            Label2.Caption = "Level: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With

        Case 1160   'Relief
            frmColor.Visible = False
        
        Case 1170   'Render
            frmColor.Visible = False
            Label2.Caption = "Angle: 0"
            With HScroll1(0)
                .Max = 360
                .Min = -360
                .Value = 0
            End With
            Label3.Caption = "Zoom: 0"
            With HScroll1(1)
                .Enabled = True
                .Max = 400
                .Min = 0
                .Value = 100
            End With
            Label4.Caption = "Blending: 128"
            With HScroll1(2)
                .Enabled = True
                .Max = 255
                .Min = 0
                .Value = 128
            End With
        
        Case 1180   'Replace Color
            frmColor.Visible = True
        
        Case 1190   'Replace Colors
            frmColor.Visible = True
            Label2.Caption = "Similarity: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1200   'Rotate
            frmColor.Visible = False
            Label2.Caption = "Angle: 0"
            With HScroll1(0)
                .Max = 360
                .Min = -360
                .Value = 0
            End With
        
        Case 1210   'Saturation
            frmColor.Visible = False
            Label2.Caption = "Saturation: 255"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 255
            End With
        
        Case 1220   'Scanlines
            frmColor.Visible = True
            Label2.Caption = "Transparency: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 128
            End With
            Label3.Caption = "Size: 0"
            With HScroll1(1)
                .Enabled = True
                .Max = 24
                .Min = 0
                .Value = 1
            End With
            
        Case 1240   'Text
            Label2.Caption = "Text:"
            frmColor.Visible = True
            Text1.Visible = True

        Case 1250   'Tone
            frmColor.Visible = True
            Label2.Caption = "Tone: 0"
            With HScroll1(0)
                .Max = 255
                .Min = 0
                .Value = 0
            End With
            
        Case 1260   'Zoom
            frmColor.Visible = False
            Label2.Caption = "Zoom: 1"
            With HScroll1(0)
                .Max = 400
                .Min = 0
                .Value = 100
            End With
            
    End Select
    
    
    If HScroll1(1).Enabled = False Then
        Label3.Enabled = False
        Label3.Caption = "N/a"
    Else
        Label3.Enabled = True
    End If
    
    
    If HScroll1(2).Enabled = False Then
        Label4.Enabled = False
        Label4.Caption = "N/a"
    Else
        Label4.Enabled = True
    End If
    
    
    DoEvents
   
    Update
    
End Sub


Private Sub mnuHelpEMail_Click()
On Error Resume Next
    ShellExecute Me.hwnd, "open", "mailto:martins_s@mail.teliamtc.lv", vbNullString, App.Path, vbNormalFocus
End Sub


Private Sub mnuHelpVote_Click()
On Error Resume Next
    ShellExecute Me.hwnd, "open", "http://www.planet-source-code.com/vb", vbNullString, App.Path, vbMaximizedFocus
End Sub


Private Sub mnuHelpWebsite_Click()
On Error Resume Next
    ShellExecute Me.hwnd, "open", "http://www.exe.times.lv", vbNullString, App.Path, vbMaximizedFocus
End Sub


Private Sub picDest_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    
    fxX = X
    fxY = Y
    
    If fxX < 32 Then fxX = 32
    If fxY < 32 Then fxY = 32
    If fxX > picDest.ScaleWidth - 32 Then fxX = picDest.ScaleWidth - 32
    If fxY > picDest.ScaleHeight - 32 Then fxY = picDest.ScaleHeight - 32
    
    Select Case fxIndex
        Case 1170: Update
        Case 1200: Update
        Case 1260: Update
    End Select
    
End Sub


Private Sub Text1_Change()
    Update
End Sub


Private Function CreateFontObj() As Long
    CreateFontObj = CreateFont(-MulDiv(22, GetDeviceCaps(GetDC(0), 90), 72), 0, 0, 0, 700, False, False, False, 1, 0, 0, 2, 0, "Tahoma")
End Function



