
; applies sens to mouse input
; rewriting to apply mouse input to character controller before it gets crunched into an int
; v0.1 - rhett
.model flat
patch1 segment ; 00422267

; If I set the int dx,dy to 0 then the function that moves the camera might not get called
mov    ecx,dword ptr [esi+011ch] ; loads PlanetsideGame to ecx
mov    ecx,dword ptr [ecx+088h]  ; loads ClientAvatar to ecx
mov    ecx,dword ptr [ecx+06ch]  ; loads LocalPlayerBehavior to ecx

;  esi          PlanetsideUI*
; [esp+024h]    int dx
; [esp+028h]    int dy
;  eax          Settings*
;  ecx          LocalPlayerBehavior*
; [eax+0xac]    mouse sensitivity
;-- dx
mov    eax,DWORD PTR [esi+0144h] ; loads settings into eax
fld    DWORD PTR [eax+0ach]      ; loads mouse sens to fp stack, st(0) I think?
; fimul or fmul?
fimul  DWORD PTR [esp+024h]      ; multiplies mouse sens by int dx
fst   DWORD PTR [ecx+084h]       ; stores mouse dx into LocalPlayerBehavior
fistp qword ptr [esp+018h]       ; store int mouse dx back where the original does
mov eax, dword ptr [esp+018h]
mov dword ptr [esp+024h], eax
;-- dy
mov    eax,DWORD PTR [esi+0144h] ; loads settings into eax
fld    DWORD PTR [eax+0ach]      ; loads mouse sens to fp stack, st(0) I think?
fimul  DWORD PTR [esp+028h]      ; multiplies mouse sens by int dy
fst   DWORD PTR [ecx+088h]       ; stores mouse dy into LocalPlayerBehavior
fistp qword ptr [esp+018h]       ; store int mouse dy back where the original does
mov eax, dword ptr [esp+018h]
mov dword ptr [esp+028h], eax
; I think that's what it expects to get back
mov ecx, eax

patch1 ends

original segment
mov    eax,DWORD PTR [esi+144h]
;;dx
fld    DWORD PTR [eax+0ach]
fimul  DWORD PTR [esp+24h]
fistp  QWORD PTR [esp+18h]
mov    ecx,DWORD PTR [esp+18h]
mov    DWORD PTR [esp+24h],ecx
;;dy
fld    DWORD PTR [eax+0ach]
fimul  DWORD PTR [esp+28h]
fistp  QWORD PTR [esp+18h]
mov    eax,DWORD PTR [esp+18h]
original ends

patch2 segment ; 004222ac

patch2 ends

end