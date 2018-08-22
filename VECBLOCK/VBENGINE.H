/*
*********************************************************************
 ##       ## #######
 ##       ## ##    ##
 ##      ##  ##     ##
  ##    ##   ##    ##
   ##  ##    ##     ##
    ####     ##    ##
     ##      #######    ENGINE BY KAMIL KOROLKIEWICZ 2006
*********************************************************************
*/


//===========================================

//h
#include <stdlib.h>
#include <stdio.h>
#include <dpmi.h>
#include <go32.h>
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include "asm\xstub32.h"
#include "asm\time.h"
#include "asm\zerocpy.h"
#include "vb_mem.h"
#include "vb_key.h"
#include "vb_expr.h"
#include "vb_vr.h"
#include "vb_cf.h"
#include "vb_surf.h"
#include "vb_vid.h"
#include "vb_con.h"
#include "vb_snd.h"
#include "vb_cfg.h"
#include "vb_menu.h"
#include "vb_loop.h"
//user
#include "biosfont.h"
//c
#include "vb_glb.c"
#include "vb_mem.c"
#include "vb_key.c"
#include "vb_cf.c"
#include "vb_surf.c"
#include "vb_vid.c"
#include "vb_con.c"
#include "vb_expr.c"
#include "vb_vr.c"
#include "vb_cfg.c"
#include "vb_menu.c"
#include "vb_loop.c"
#include "vb_snd.c"
