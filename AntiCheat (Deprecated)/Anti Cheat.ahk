#NoEnv
SendMode Input
SetWorkingDir %A_ScriptDir%

; Kill explorer.exe when the script starts
Run, taskkill /f /im explorer.exe

; Disable the Windows key combinations
#::Return
#!::Return
#^::Return
#+::Return

; Disable the Windows key itself
LWin::Return
RWin::Return

; Disable common Windows shortcuts
#b::Return
#d::Return
#e::Return
#f::Return
#g::Return
#h::Return
#i::Return
#k::Return
#l::Return
#m::Return
#o::Return
#p::Return
#q::Return
#r::Return
#s::Return
#t::Return
#u::Return
#v::Return
#w::Return
#x::Return
#y::Return
#z::Return

; Disable Ctrl+Alt+Del (Note: This may not be possible due to security features of Windows)
^!Delete::Return

; Disable Ctrl+D
^d::Return

; Disable Alt+Tab
!Tab::Return

; Secret way to exit the program
; The following hotkey combination Ctrl+Alt+Shift+F12 will restart explorer and exit the script
^!+F12::
Run, explorer.exe
ExitApp

; Hotkey to kill explorer.exe
^!k::  ; Ctrl+Alt+K to kill explorer
Run, taskkill /f /im explorer.exe
return

; Hotkey to restart explorer.exe
^!r::  ; Ctrl+Alt+R to restart explorer
Run, explorer.exe
return