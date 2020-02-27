/*
*tos_page_calculator.c
*/
#include "newTestScreen.h"

/*Declaration of icons and fonts*/
LV_IMG_DECLARE(tos_menu_button_bg_blue);
LV_IMG_DECLARE(tos_menu_button_bg_green);
/*Design Functions Definations*/
/*Header Design Function Definations*/
void tos_calculator_header(void);
void tos_calculatorHeader_digitalClock(void);
void tos_calculatorHeader_bluetooth(void);
void tos_calculatorHeader_batary(void);
/*Base Design Function Definations*/
void tos_calculatorBase(void);
/*Style Functions*/
void tos_calculatorNumbersStyle(void);
/*Screen Design Functions*/
void tos_calculatorScreen(void);
void calculatorDisplayScreen(void);
/*Buttons Design Functions*/
void tos_calculatorBase_turnBack(void);
void tos_calculatorNumbers(void); 
void tos_calculatorOperationsOperators(void);
/*Another Design Functions*/
void calculatorLine(void);
/*Update Functions*/
void tos_calculator_BluetoothStatus(lv_task_t * task);
void tos_calculator_BataryValue(lv_task_t * task);
void tos_calculator_TimeUpdate(lv_task_t * task);
/*Button Actions Functions*/
static void tos_calculator_TurnBack_event_handler(lv_obj_t * obj, lv_event_t event);
static void tos_calculator_Numbers_event_handler(lv_obj_t * obj, lv_event_t event);
static void tos_calculator_Operators_event_handler(lv_obj_t * obj, lv_event_t event);
/*Create Objs*/
/*Screen Obj*/
lv_obj_t *calculatorMain;
lv_obj_t *tos_calculatorDisplay;
/*Another Obj*/
lv_obj_t *tos_calculatorHeader;
lv_obj_t *tos_calculatorBatary;
lv_obj_t *tos_calculatorBluetooth;
lv_obj_t *tos_calculatorClock;
lv_obj_t *tos_calculatorBases;
lv_obj_t *tos_calculatorDisplayLabel;
/*Buttons Objs*/
lv_obj_t *tos_calculator_button_turnBack;


/*Create Styles*/
static lv_style_t tos_calculator_HeaderStyle;
static lv_style_t tos_calculator_HeaderBataryStyle;
static lv_style_t tos_calculator_BaseStyle;
static lv_style_t tos_calculator_buttonsStyles;
static lv_style_t tos_Calculator_NumberBtn_rel;
static lv_style_t tos_Calculator_NumberBtn_pr;
static lv_style_t tos_Calculator_NumberBtn_tgl_rel;
static lv_style_t tos_Calculator_NumberBtn_tgl_pr;
static lv_style_t tos_Calculator_NumberBtn_ina;
static lv_style_t tos_calculator_DisplayStyle;

/*Create variables*/
/*Arrays*/
uint16_t tos_calculatorTime[2] = { 0,0 };
char calculatorBuf[32];
/*Time Variables*/
uint8_t tos_calculatorHours = 14;
uint8_t tos_calculatorMinutes = 53;
/*Battary Value and bluetooth Variables*/
uint8_t tos_calculatorBataryValue = 0;
uint8_t tos_calculatorBuluetoothStatus = 0;
/*Another Variables*/
double tos_calculatarFirstVariable=0;
double tos_calculatarLastVariable=0;
double tos_calculatarEqualVariable=0;
/*Main Function*/
void tos_calculator(void) {

	lv_theme_set_current(lv_theme_mono_init(0, 0));
	//lv_theme_set_current(lv_theme_night_init(0, 0));

	calculatorMain = lv_obj_create(0, 0);
	lv_disp_load_scr(calculatorMain);//Displayi oluşturur.
	tos_calculator_header();
	tos_calculatorBase();
	tos_calculatorScreen();

}
void tos_calculatorScreen(void) {
	tos_calculatorNumbers();
	tos_calculatorOperationsOperators();
	calculatorLine();
	calculatorDisplayScreen();
	
}
/*Calculator Functions*/
void calculatorDisplayScreen(void) {
	lv_style_copy(&tos_calculator_DisplayStyle, &lv_style_plain);
	tos_calculator_DisplayStyle.body.main_color = lv_color_hex3(0xeee);
	tos_calculator_DisplayStyle.body.grad_color = lv_color_hex3(0xeee);
	tos_calculator_DisplayStyle.body.opa = 190;
	tos_calculator_DisplayStyle.body.radius = 12;
	tos_calculator_DisplayStyle.text.color = LV_COLOR_BLACK;

	tos_calculatorDisplay = lv_cont_create(lv_disp_get_scr_act(NULL), NULL);
	lv_obj_set_width(tos_calculatorDisplay, 231);
	lv_obj_set_height(tos_calculatorDisplay, 30);
	lv_obj_align(tos_calculatorDisplay, NULL, LV_ALIGN_CENTER, 1, -70);
	lv_obj_set_style(tos_calculatorDisplay, &tos_calculator_DisplayStyle);

	tos_calculatorDisplayLabel = lv_label_create(tos_calculatorDisplay, NULL);
	lv_label_set_text(tos_calculatorDisplayLabel, "0");
	lv_obj_align(tos_calculatorDisplayLabel, NULL, LV_ALIGN_IN_RIGHT_MID, -(LV_DPI / 10), 0);

}
void calculatorLine(void) {

	static lv_point_t line_points[] = { {137,60}, {137, 210}, };
	static lv_style_t style_line;
	lv_style_copy(&style_line, &lv_style_plain);
	style_line.line.color = lv_color_hex3(0xeee);
	style_line.line.opa = 150;
	style_line.line.width = 2;
	style_line.line.rounded = 0;

	lv_obj_t * line1;
	line1 = lv_line_create(lv_scr_act(), NULL);
	lv_line_set_points(line1, line_points, 2);
	lv_line_set_style(line1, LV_LINE_STYLE_MAIN, &style_line);
	lv_obj_align(line1, NULL, LV_ALIGN_CENTER, 0, 0);
}
void tos_calculatorNumbers(void) {

	static const char * number_BtnMap[16] = { "9", "8", "7", "\n",
									   "6", "5", "4", "\n", 
									   "3", "2", "1", "\n", 
									   "0", ".", "=", "" };

	lv_obj_t * number_Btns = lv_btnm_create(lv_scr_act(), NULL);
	lv_btnm_set_map(number_Btns, number_BtnMap);

	lv_obj_set_size(number_Btns,180,160);
	lv_obj_align(number_Btns, NULL, LV_ALIGN_CENTER, -25, 30);
	lv_obj_set_event_cb(number_Btns, tos_calculator_Numbers_event_handler);

	tos_calculatorNumbersStyle();
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_REL,&tos_Calculator_NumberBtn_rel);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_PR,&tos_Calculator_NumberBtn_pr);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_TGL_REL,&tos_Calculator_NumberBtn_tgl_rel);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_TGL_PR,&tos_Calculator_NumberBtn_tgl_pr);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_INA,&tos_Calculator_NumberBtn_ina);

}
void tos_calculatorOperationsOperators(void) {

	static const char * operationsOperators_BtnMap[11] = { "Sil","\n", "/","\n", "x","\n","-","\n","+","\n" ,"" };

	lv_obj_t * number_Btns = lv_btnm_create(lv_scr_act(), NULL);
	lv_btnm_set_map(number_Btns, operationsOperators_BtnMap);
	lv_obj_set_size(number_Btns, 45, 195);
	lv_obj_align(number_Btns, NULL, LV_ALIGN_CENTER,95, 47);
	lv_obj_set_event_cb(number_Btns, tos_calculator_Operators_event_handler);

	tos_calculatorNumbersStyle();
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_REL, &tos_Calculator_NumberBtn_rel);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_PR, &tos_Calculator_NumberBtn_pr);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_TGL_REL, &tos_Calculator_NumberBtn_tgl_rel);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_TGL_PR, &tos_Calculator_NumberBtn_tgl_pr);
	lv_btnm_set_style(number_Btns, LV_BTNM_STYLE_BTN_INA, &tos_Calculator_NumberBtn_ina);

}

/*Header Design Functions*/
void tos_calculator_header(void) {

	lv_style_copy(&tos_calculator_HeaderStyle, &lv_style_plain);
	tos_calculator_HeaderStyle.body.main_color = LV_COLOR_BLACK;
	tos_calculator_HeaderStyle.body.grad_color = LV_COLOR_BLACK;
	tos_calculator_HeaderStyle.text.color = LV_COLOR_GRAY;

	tos_calculatorHeader = lv_cont_create(lv_disp_get_scr_act(NULL), NULL);
	lv_obj_set_width(tos_calculatorHeader, 272);
	lv_obj_set_height(tos_calculatorHeader, 20);
	lv_obj_align(tos_calculatorHeader, NULL, LV_ALIGN_CENTER, 0, -126);
	lv_obj_set_style(tos_calculatorHeader, &tos_calculator_HeaderStyle);

	lv_task_t * tos_calculatorDigitalTimeUpdate = lv_task_create(tos_calculator_TimeUpdate, 500, LV_TASK_PRIO_MID, NULL);
	lv_task_t * tos_calculatorBataryValueUpdate = lv_task_create(tos_calculator_BataryValue, 500, LV_TASK_PRIO_MID, NULL);
	lv_task_t * tos_calculatorBluetoothStatusUpdate = lv_task_create(tos_calculator_BluetoothStatus, 500, LV_TASK_PRIO_MID, NULL);

	tos_calculatorHeader_batary();
	tos_calculatorHeader_bluetooth();
	tos_calculatorHeader_digitalClock();
}
void tos_calculatorHeader_batary(void) {
	lv_style_copy(&tos_calculator_HeaderBataryStyle, &lv_style_plain);
	tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_GREEN;

	tos_calculatorBatary = lv_label_create(tos_calculatorHeader, NULL);
	lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_FULL);
	lv_label_set_style(tos_calculatorBatary, LV_LABEL_STYLE_MAIN, &tos_calculator_HeaderBataryStyle);
	lv_obj_align(tos_calculatorBatary, NULL, LV_ALIGN_IN_RIGHT_MID, -(LV_DPI / 10), 0);
}
void tos_calculatorHeader_bluetooth(void) {
	tos_calculatorBluetooth = lv_label_create(tos_calculatorHeader, NULL);
	lv_label_set_text(tos_calculatorBluetooth, LV_SYMBOL_BLUETOOTH);
	lv_obj_align(tos_calculatorBluetooth, NULL, LV_ALIGN_IN_LEFT_MID, LV_DPI / 10, 0);
}
void tos_calculatorHeader_digitalClock(void) {
	tos_calculatorClock = lv_label_create(tos_calculatorHeader, NULL);
	lv_label_set_text(tos_calculatorClock, "14:53");
	lv_obj_align(tos_calculatorClock, NULL, LV_ALIGN_CENTER, 0, 0);
}
/*Base Design Function */
void tos_calculatorBase(void) {
	lv_style_copy(&tos_calculator_BaseStyle, &lv_style_plain);
	tos_calculator_BaseStyle.body.main_color = LV_COLOR_BLACK;
	tos_calculator_BaseStyle.body.grad_color = LV_COLOR_BLACK;
	tos_calculator_BaseStyle.text.color = LV_COLOR_WHITE;

	tos_calculatorBases = lv_cont_create(lv_disp_get_scr_act(NULL), NULL);
	lv_obj_set_width(tos_calculatorBases, 272);
	lv_obj_set_height(tos_calculatorBases, 20);
	lv_obj_align(tos_calculatorBases, NULL, LV_ALIGN_CENTER, 0, 126);
	lv_obj_set_style(tos_calculatorBases, &tos_calculator_BaseStyle);


	tos_calculatorBase_turnBack();
}
/*Buttons Functions*/
void tos_calculatorBase_turnBack(void) {
	lv_style_copy(&tos_calculator_buttonsStyles, &lv_style_plain);
	tos_calculator_buttonsStyles.image.color = LV_COLOR_BLACK;
	tos_calculator_buttonsStyles.image.intense = LV_OPA_50;
	tos_calculator_buttonsStyles.text.color = LV_COLOR_GREEN;

	tos_calculator_button_turnBack = lv_imgbtn_create(tos_calculatorBases, NULL);
	lv_imgbtn_set_src(tos_calculator_button_turnBack, LV_BTN_STATE_REL, &tos_menu_button_bg_blue);
	lv_imgbtn_set_src(tos_calculator_button_turnBack, LV_BTN_STATE_PR, &tos_menu_button_bg_blue);
	lv_imgbtn_set_src(tos_calculator_button_turnBack, LV_BTN_STATE_TGL_REL, &tos_menu_button_bg_green);
	lv_imgbtn_set_src(tos_calculator_button_turnBack, LV_BTN_STATE_TGL_PR, &tos_menu_button_bg_green);
	lv_imgbtn_set_style(tos_calculator_button_turnBack, LV_BTN_STATE_PR, &tos_calculator_buttonsStyles);        /*Use the darker style in the pressed state*/
	lv_imgbtn_set_style(tos_calculator_button_turnBack, LV_BTN_STATE_TGL_PR, &tos_calculator_buttonsStyles);
	lv_imgbtn_set_toggle(tos_calculator_button_turnBack, true);
	lv_obj_align(tos_calculator_button_turnBack, NULL, LV_ALIGN_CENTER, 0, 0);
	lv_obj_set_event_cb(tos_calculator_button_turnBack, tos_calculator_TurnBack_event_handler);
}
/*Update Functions*/
void tos_calculator_TimeUpdate(lv_task_t * task)
{


	tos_calculatorTime[1] = tos_calculatorHours;
	tos_calculatorTime[0] = tos_calculatorMinutes;


	static uint32_t prev_value = 0;
	for (uint8_t sayac = 0; sayac < 3; sayac++) {
		if (prev_value != tos_calculatorTime[sayac]) {
			if (lv_obj_get_screen(tos_calculatorClock) == lv_scr_act()) {
				snprintf(calculatorBuf, 32, "%d:%d", tos_calculatorTime[1], tos_calculatorTime[0]);
				if (tos_calculatorTime[1] < 10 && tos_calculatorTime[0] < 10) {
					snprintf(calculatorBuf, 32, "0%d:0%d", tos_calculatorTime[1], tos_calculatorTime[0]);
				}
				else if (tos_calculatorTime[1] < 10 && tos_calculatorTime[0] >= 10) {
					snprintf(calculatorBuf, 32, "0%d:%d", tos_calculatorTime[1], tos_calculatorTime[0]);
				}
				else if (tos_calculatorTime[1] >= 10 && tos_calculatorTime[0] < 10) {
					snprintf(calculatorBuf, 32, "%d:0%d", tos_calculatorTime[1], tos_calculatorTime[0]);
				}
				else if (tos_calculatorTime[1] >= 10 && tos_calculatorTime[0] >= 10) {
					snprintf(calculatorBuf, 32, "%d:%d", tos_calculatorTime[1], tos_calculatorTime[0]);
				}
				lv_label_set_text(tos_calculatorClock, calculatorBuf);
			}
			prev_value = tos_calculatorTime[sayac];
		}
	}

}
void tos_calculator_BataryValue(lv_task_t * task) {

	static uint32_t prev_value1 = 0;
	if (tos_calculatorBataryValue == prev_value1) {
		if (lv_obj_get_screen(tos_calculatorBatary) == lv_scr_act()) {

			if (tos_calculatorBataryValue < 10 && tos_calculatorBataryValue >= 0) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_EMPTY);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_RED;
			}
			else if (tos_calculatorBataryValue > 10 && tos_calculatorBataryValue < 25) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_1);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_ORANGE;
			}
			else if (tos_calculatorBataryValue > 25 && tos_calculatorBataryValue < 50) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_2);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_ORANGE;
			}
			else if (tos_calculatorBataryValue > 50 && tos_calculatorBataryValue < 80) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_3);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_GREEN;
			}
			else if (tos_calculatorBataryValue > 80 && tos_calculatorBataryValue <= 100) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_FULL);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_GREEN;
			}
		}
	}

	if (tos_calculatorBataryValue != prev_value1) {

		if (lv_obj_get_screen(tos_calculatorBatary) == lv_scr_act()) {

			if (tos_calculatorBataryValue < 10 && tos_calculatorBataryValue >= 0) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_EMPTY);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_RED;
			}
			else if (tos_calculatorBataryValue > 10 && tos_calculatorBataryValue < 25) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_1);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_ORANGE;
			}
			else if (tos_calculatorBataryValue > 25 && tos_calculatorBataryValue < 50) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_2);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_ORANGE;
			}
			else if (tos_calculatorBataryValue > 50 && tos_calculatorBataryValue < 80) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_3);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_GREEN;
			}
			else if (tos_calculatorBataryValue > 80 && tos_calculatorBataryValue <= 100) {
				lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_BATTERY_FULL);
				tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_GREEN;
			}
		}
		if (tos_calculatorBataryValue > prev_value1) {
			if (lv_obj_get_screen(tos_calculatorBatary) == lv_scr_act()) {

				if (tos_calculatorBataryValue < 10 && tos_calculatorBataryValue >= 0) {
					lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_CHARGE LV_SYMBOL_BATTERY_EMPTY);
					tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_RED;
				}
				else if (tos_calculatorBataryValue > 10 && tos_calculatorBataryValue < 25) {
					lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_CHARGE LV_SYMBOL_BATTERY_1);
					tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_ORANGE;
				}
				else if (tos_calculatorBataryValue > 25 && tos_calculatorBataryValue < 50) {
					lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_CHARGE LV_SYMBOL_BATTERY_2);
					tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_ORANGE;
				}
				else if (tos_calculatorBataryValue > 50 && tos_calculatorBataryValue < 80) {
					lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_CHARGE LV_SYMBOL_BATTERY_3);
					tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_GREEN;
				}
				else if (tos_calculatorBataryValue > 80 && tos_calculatorBataryValue <= 100) {
					lv_label_set_text(tos_calculatorBatary, LV_SYMBOL_CHARGE LV_SYMBOL_BATTERY_FULL);
					tos_calculator_HeaderBataryStyle.text.color = LV_COLOR_GREEN;
				}

			}
		}
		prev_value1 = tos_calculatorBataryValue;

	}
}
void tos_calculator_BluetoothStatus(lv_task_t * task) {
	static uint32_t prev_value1 = -1;

	if (prev_value1 != tos_calculatorBuluetoothStatus) {
		if (lv_obj_get_screen(tos_calculatorBluetooth) == lv_scr_act()) {

			if (0 == tos_calculatorBuluetoothStatus) {
				lv_label_set_text(tos_calculatorBluetooth, "");
			}
			else if (1 == tos_calculatorBuluetoothStatus) {
				lv_label_set_text(tos_calculatorBluetooth, LV_SYMBOL_BLUETOOTH);
			}
		}
		prev_value1 = tos_calculatorBuluetoothStatus;
	}
}
/*Buttons Actions Functions*/
static void tos_calculator_TurnBack_event_handler(lv_obj_t * obj, lv_event_t event)
{
	if (event == LV_EVENT_CLICKED) {
		printf("tOSbaga Anasayfa");
	}
}
static void tos_calculator_Numbers_event_handler(lv_obj_t * obj, lv_event_t event)
{
	if (event == LV_EVENT_VALUE_CHANGED) {
		
		tos_calculatarEqualVariable = (double)atof(lv_label_get_text(tos_calculatorDisplayLabel));
		static const char * number_BtnMapValues[10] = { "9", "8", "7", 
														"6", "5", "4", 
														"3", "2", "1", 
														"0"};


			for (uint8_t sayac = 0; sayac < 10; sayac++) {
				if (0 == strcmp(number_BtnMapValues[sayac], lv_btnm_get_active_btn_text(obj))) {
					if (lv_obj_get_screen(tos_calculatorDisplayLabel) == lv_scr_act()) {
						if (0 == strcmp("0", lv_label_get_text(tos_calculatorDisplayLabel))) {
							snprintf(calculatorBuf, 32, "%s",lv_btnm_get_active_btn_text(obj));
						}
						else {
							snprintf(calculatorBuf, 32, "%s%s",
								lv_label_get_text(tos_calculatorDisplayLabel), lv_btnm_get_active_btn_text(obj));						}
						
						lv_label_set_text(tos_calculatorDisplayLabel, calculatorBuf);
						lv_obj_align(tos_calculatorDisplayLabel, NULL, LV_ALIGN_IN_RIGHT_MID, -(LV_DPI / 10) + 3, 0);
					}
					
					
				}
				
			}
			if (0 == strcmp(".", lv_btnm_get_active_btn_text(obj))) {
				if (0 == strstr(lv_label_get_text(tos_calculatorDisplayLabel), ".")) { 
					snprintf(calculatorBuf, 32, "%s%s",
						lv_label_get_text(tos_calculatorDisplayLabel), lv_btnm_get_active_btn_text(obj));
					lv_label_set_text(tos_calculatorDisplayLabel, calculatorBuf);
					lv_obj_align(tos_calculatorDisplayLabel, NULL, LV_ALIGN_IN_RIGHT_MID, -(LV_DPI / 10) + 3, 0);
				}
			}
			else if (0 == strcmp("=", lv_btnm_get_active_btn_text(obj))) {
				
				if (lv_obj_get_screen(tos_calculatorDisplayLabel) == lv_scr_act()) {
					int equalScreenControl= tos_calculatarEqualVariable;
					if (equalScreenControl==tos_calculatarEqualVariable) {
						snprintf(calculatorBuf, 32, "%d",(int)tos_calculatarEqualVariable);
					}
					else {
						snprintf(calculatorBuf, 32, "%lf", tos_calculatarEqualVariable);
					}


					lv_label_set_text(tos_calculatorDisplayLabel, calculatorBuf);
					lv_obj_align(tos_calculatorDisplayLabel, NULL, LV_ALIGN_IN_RIGHT_MID, -(LV_DPI / 10) + 3, 0);
					tos_calculatarEqualVariable = (double)atof(lv_label_get_text(tos_calculatorDisplayLabel));
				}
				
			}
	}
}

static void tos_calculator_Operators_event_handler(lv_obj_t * obj, lv_event_t event) {
	if (event == LV_EVENT_VALUE_CHANGED) {
		static const char * operationsOperators_BtnMapValues[5] = { "Sil", "/", "x","-","+" };
		char calculatorEqualBuf[32];
		if (0 == strcmp(operationsOperators_BtnMapValues[0], lv_btnm_get_active_btn_text(obj))) {
			
			static const char * number_BtnMapValues[10] = { "9", "8", "7",
														"6", "5", "4",
														"3", "2", "1",
														"0" };
			strcpy(calculatorEqualBuf, lv_label_get_text(tos_calculatorDisplayLabel));
			if (1 == strlen(calculatorEqualBuf)) {
				for (uint8_t sayac = 0; sayac < 10; sayac++) {
					if (0 == strcmp(number_BtnMapValues[sayac], calculatorEqualBuf)) {
						snprintf(calculatorEqualBuf, 32, "0");
					}
					else {
						snprintf(calculatorBuf, 32, "%s", calculatorEqualBuf);
					}
				}
			}
			memmove(&calculatorEqualBuf[strlen(calculatorEqualBuf) - 1], &calculatorEqualBuf[strlen(calculatorEqualBuf)], strlen(calculatorEqualBuf) - 1);
			snprintf(calculatorBuf, 32, "%s", calculatorEqualBuf);
		}

		else if (0 == strcmp(operationsOperators_BtnMapValues[1], lv_btnm_get_active_btn_text(obj))) {

		}
		else if (0 == strcmp(operationsOperators_BtnMapValues[2], lv_btnm_get_active_btn_text(obj))) {
		}
		else if (0 == strcmp(operationsOperators_BtnMapValues[3], lv_btnm_get_active_btn_text(obj))) {
		}
		else if (0 == strcmp(operationsOperators_BtnMapValues[4], lv_btnm_get_active_btn_text(obj))) {
		}

		lv_label_set_text(tos_calculatorDisplayLabel, calculatorEqualBuf);
		lv_obj_align(tos_calculatorDisplayLabel, NULL, LV_ALIGN_IN_RIGHT_MID, -(LV_DPI / 10) + 3, 0);
	}


}

/*Style Functions*/
void tos_calculatorNumbersStyle(void) {
	
		

	lv_style_copy(&tos_Calculator_NumberBtn_rel, &lv_style_plain);
	tos_Calculator_NumberBtn_rel.body.main_color = lv_color_hsv_to_rgb(0, 10, 40);
	tos_Calculator_NumberBtn_rel.body.grad_color = lv_color_hsv_to_rgb(0, 10, 20);
	tos_Calculator_NumberBtn_rel.body.border.color = lv_color_hex3(0x111);
	tos_Calculator_NumberBtn_rel.body.border.width = 1;
	tos_Calculator_NumberBtn_rel.body.border.opa = LV_OPA_70;
	tos_Calculator_NumberBtn_rel.body.padding.left = LV_DPI / 4;
	tos_Calculator_NumberBtn_rel.body.padding.right = LV_DPI / 4;
	tos_Calculator_NumberBtn_rel.body.padding.top = LV_DPI / 8;
	tos_Calculator_NumberBtn_rel.body.padding.bottom = LV_DPI / 8;
	tos_Calculator_NumberBtn_rel.body.shadow.type = LV_SHADOW_BOTTOM;
	tos_Calculator_NumberBtn_rel.body.shadow.color = lv_color_hex3(0x111);
	tos_Calculator_NumberBtn_rel.body.shadow.width = LV_DPI / 30;
	tos_Calculator_NumberBtn_rel.text.color = lv_color_hex3(0xeee);
	tos_Calculator_NumberBtn_rel.image.color = lv_color_hex3(0xeee);

	lv_style_copy(&tos_Calculator_NumberBtn_pr, &tos_Calculator_NumberBtn_rel);
	tos_Calculator_NumberBtn_pr.body.main_color = lv_color_hsv_to_rgb(0, 10, 30);
	tos_Calculator_NumberBtn_pr.body.grad_color = lv_color_hsv_to_rgb(0, 10, 10);

	lv_style_copy(&tos_Calculator_NumberBtn_tgl_rel, &tos_Calculator_NumberBtn_rel);
	tos_Calculator_NumberBtn_tgl_rel.body.main_color = lv_color_hsv_to_rgb(0, 10, 20);
	tos_Calculator_NumberBtn_tgl_rel.body.grad_color = lv_color_hsv_to_rgb(0, 10, 40);
	tos_Calculator_NumberBtn_tgl_rel.body.shadow.width = LV_DPI / 40;
	tos_Calculator_NumberBtn_tgl_rel.text.color = lv_color_hex3(0xddd);
	tos_Calculator_NumberBtn_tgl_rel.image.color = lv_color_hex3(0xddd);

	lv_style_copy(&tos_Calculator_NumberBtn_tgl_pr, &tos_Calculator_NumberBtn_rel);
	tos_Calculator_NumberBtn_tgl_pr.body.main_color = lv_color_hsv_to_rgb(0, 10, 10);
	tos_Calculator_NumberBtn_tgl_pr.body.grad_color = lv_color_hsv_to_rgb(0, 10, 30);
	tos_Calculator_NumberBtn_tgl_pr.body.shadow.width = LV_DPI / 30;
	tos_Calculator_NumberBtn_tgl_pr.text.color = lv_color_hex3(0xddd);
	tos_Calculator_NumberBtn_tgl_pr.image.color = lv_color_hex3(0xddd);

	lv_style_copy(&tos_Calculator_NumberBtn_ina, &tos_Calculator_NumberBtn_rel);
	tos_Calculator_NumberBtn_ina.body.main_color = lv_color_hsv_to_rgb(0, 10, 20);
	tos_Calculator_NumberBtn_ina.body.grad_color = lv_color_hsv_to_rgb(0, 10, 20);
	tos_Calculator_NumberBtn_ina.body.shadow.width = 0;
	tos_Calculator_NumberBtn_ina.text.color = lv_color_hex3(0xaaa);
	tos_Calculator_NumberBtn_ina.image.color = lv_color_hex3(0xaaa);

	
}
/*Set Functions*/
void tos_calculator_timeGet(uint8_t *hours, uint8_t *minutes) {

	tos_calculatorHours = *hours;
	tos_calculatorMinutes = *minutes;

}
void tos_calculator_BataryAndBluetoothValueGet(uint8_t *bataryvalue, uint8_t *bluetoothstatus) {

	tos_calculatorBataryValue = *bataryvalue;
	tos_calculatorBuluetoothStatus = *bluetoothstatus;
}
