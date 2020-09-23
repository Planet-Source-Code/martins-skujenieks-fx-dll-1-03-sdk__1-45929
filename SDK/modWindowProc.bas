Attribute VB_Name = "modWindowProc"
Option Explicit

    Private Const GWL_WNDPROC = (-4)
    
    Private Const WM_GETMINMAXINFO = &H24

    Private Type POINTAPI
        X                               As Long
        Y                               As Long
    End Type
    
    Private Type MINMAXINFO
        ptReserved                      As POINTAPI
        ptMaxSize                       As POINTAPI
        ptMaxPosition                   As POINTAPI
        ptMinTrackSize                  As POINTAPI
        ptMaxTrackSize                  As POINTAPI
    End Type
    
    Private lpPrevWndProc               As Long
    
    Private Declare Sub CopyMemoryToMinMaxInfo Lib "kernel32" Alias "RtlMoveMemory" (hpvDest As MINMAXINFO, ByVal hpvSource As Long, ByVal cbCopy As Long)
    Private Declare Sub CopyMemoryFromMinMaxInfo Lib "kernel32" Alias "RtlMoveMemory" (ByVal hpvDest As Long, hpvSource As MINMAXINFO, ByVal cbCopy As Long)
    Private Declare Function SetWindowLong Lib "User32" Alias "SetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
    Private Declare Function DefWindowProc Lib "User32" Alias "DefWindowProcA" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    Private Declare Function CallWindowProc Lib "User32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    
    
Public Sub Hook(hWnd As Long)
On Error Resume Next

    lpPrevWndProc = SetWindowLong(hWnd, GWL_WNDPROC, AddressOf WindowProc)
    
End Sub


Public Sub Unhook(hWnd As Long)
On Error Resume Next

    If lpPrevWndProc <> 0 Then SetWindowLong hWnd, GWL_WNDPROC, lpPrevWndProc
    
End Sub


Private Function WindowProc(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
On Error Resume Next

    Dim MinMax As MINMAXINFO
    
    Select Case uMsg
        Case WM_GETMINMAXINFO
            CopyMemoryToMinMaxInfo MinMax, lParam, Len(MinMax)
            MinMax.ptMinTrackSize.X = 420
            MinMax.ptMinTrackSize.Y = 480
            MinMax.ptMaxTrackSize.X = 420
            CopyMemoryFromMinMaxInfo lParam, MinMax, Len(MinMax)
            WindowProc = DefWindowProc(hWnd, uMsg, wParam, lParam)
            
        Case Else
            WindowProc = CallWindowProc(lpPrevWndProc, hWnd, uMsg, wParam, lParam)
            
    End Select
    
End Function


