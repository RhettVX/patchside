.model flat
.code

; applies sens to mouse input
; rewriting to apply mouse input to character controller before it gets crunch into an int
; v0.1 - rhett

; esi: PlanetsideUI*
; [esp+0x24]: int dx
; [esp+0x28]: int dy

;mov    eax,DWORD PTR [esi+0x144] ; loads settings into eax
;mov    ecx,dword ptr [eax+0x11c] ; loads PlanetsideGame to ecx
;mov    ecx,dword ptr [ecx+0x88]  ; loads ClientAvatar to ecx
;mov    ecx,dword ptr [ecx+0x6c]  ; loads LocalPlayerBehavior to ecx
;fld    DWORD PTR [eax+0xac] ; loads mouse sens to fp stack, st(0) I think?
;fimul  DWORD PTR [esp+0x24] ; multiplies mouse sens by int dx
;fstp   DWORD PTR [ecx+0x84] ; stores mouse dx into LocalPlayerBehavior

; original
mov    eax,DWORD PTR [esi+144h]
fld    DWORD PTR [eax+0ach]
fimul  DWORD PTR [esp+24h]
fistp  QWORD PTR [esp+18h]
mov    ecx,DWORD PTR [esp+18h]
mov    DWORD PTR [esp+24h],ecx
fld    DWORD PTR [eax+0ach]
fimul  DWORD PTR [esp+28h]
fistp  QWORD PTR [esp+18h]
mov    eax,DWORD PTR [esp+18h] 


end