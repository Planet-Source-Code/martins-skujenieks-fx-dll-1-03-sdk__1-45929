VERSION 5.00
Begin VB.Form frmColor 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Color"
   ClientHeight    =   1320
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   2310
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
   ScaleHeight     =   88
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   154
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Visible         =   0   'False
   Begin VB.PictureBox Picture1 
      BackColor       =   &H0000FFFF&
      Height          =   255
      Left            =   1680
      ScaleHeight     =   195
      ScaleWidth      =   435
      TabIndex        =   4
      Top             =   960
      Width           =   495
   End
   Begin VB.HScrollBar HScroll2 
      Height          =   225
      Index           =   2
      Left            =   120
      Max             =   255
      TabIndex        =   2
      Top             =   600
      Width           =   2055
   End
   Begin VB.HScrollBar HScroll2 
      Height          =   225
      Index           =   1
      Left            =   120
      Max             =   255
      TabIndex        =   1
      Top             =   360
      Value           =   255
      Width           =   2055
   End
   Begin VB.HScrollBar HScroll2 
      Height          =   225
      Index           =   0
      Left            =   120
      Max             =   255
      TabIndex        =   0
      Top             =   120
      Value           =   255
      Width           =   2055
   End
   Begin VB.Label Label3 
      Caption         =   "Color:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   960
      Width           =   1455
   End
End
Attribute VB_Name = "frmColor"
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


Private Sub Form_Load()
    SetWindowPos Me.hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOACTIVATE Or SWP_SHOWWINDOW Or SWP_NOMOVE Or SWP_NOSIZE
    fxColor = vbYellow
End Sub


Private Sub HScroll2_Change(Index As Integer)
    HScroll2_Scroll Index
End Sub


Private Sub HScroll2_Scroll(Index As Integer)
    fxColor = fxRGB(HScroll2(0).Value, HScroll2(1).Value, HScroll2(2).Value)
    Picture1.BackColor = fxColor
    Update
End Sub
