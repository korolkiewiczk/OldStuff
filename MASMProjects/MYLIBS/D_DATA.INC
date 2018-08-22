IFNDEF __D_DATA_INC
__D_DATA_INC=1

;struktura punktu:
;0-x  (q) ;wsp. x
;4-y  (q) ;wsp. y
;8-z  (q) ;wsp. z
;12-w (d) ;czy konwertowac
;16-c (d) ;kolor
;20-u (d) ;wsp. u
;24-v (d) ;wsp. v
;28-nx(d) ;normalna x
;32-ny(d) ;normalna y
;36-nz(d) ;normalna z
;40---
;40*3=120

__T_SIZE=40

;zmienne pomocnicze

__d_a1          dd 0
__d_a2          dd 0
__d_b1          dd 0
__d_b2          dd 0
__d_c1          dd 0
__d_c2          dd 0

__d_au          dd 0
__d_av          dd 0
__d_bu          dd 0
__d_bv          dd 0
__d_cu          dd 0
__d_cv          dd 0

__d_int         dd 0
__d_cnt         dd 0
__d_cntx        dd 0

__d_dxac        dd 0
__d_dxab        dd 0
__d_dxbc        dd 0
__d_dxac2       dd 0

__d_duac        dd 0
__d_dvac        dd 0
__d_duab        dd 0
__d_dvab        dd 0
__d_dubc        dd 0
__d_dvbc        dd 0
__d_du          dd 0
__d_dv          dd 0
__d_duac2       dd 0
__d_dvac2       dd 0

__d_x1          dd 0
__d_x2          dd 0
__d_u1          dd 0
__d_u2          dd 0
__d_u           dd 0
__d_v           dd 0
__d_v1          dd 0
__d_v2          dd 0
__d_ix1         dd 0
__d_ix2         dd 0
__d_y           dd 0
__d_col         dd 0

__d_SWidth      dd 0
__d_SHeight     dd 0
__d_SBits       dd 0

__d_TBits       dd 0
__d_TWidth      dd 0

__d_minx        dd 0
__d_maxx        dd 0

__d_65536       dd 65536.0
__d_05          dd 0.5

ENDIF
