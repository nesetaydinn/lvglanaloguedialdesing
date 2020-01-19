#include "anolog_Clock.h"


void analog_Clock(void)
{
	    lv_theme_set_current(lv_theme_mono_init(0, 0));

    lv_obj_t * scr = lv_obj_create(NULL, NULL);
    lv_disp_load_scr(scr);//Displayi oluşturur.
	
	tos_ex_analogclktest();
}
void tos_ex_analogclktest(void)
{

	
	lv_obj_t * analogtest;
	analogtest = tos_anologClock_create(lv_scr_act(), NULL);
	tos_anologClock_set_style(analogtest, TOS_ANOLOGCLOCK_STYLE_MAIN, NULL);           

	lv_obj_set_size(analogtest, 272, 272);
	lv_obj_align(analogtest, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
}
