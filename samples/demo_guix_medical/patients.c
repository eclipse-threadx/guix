/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"

#include "demo_guix_medical.h"


VOID patient_prompt_draw(GX_PROMPT *prompt);
extern UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);

int selected_patient_index = 0;

/* Define constants for patients screen.  */
#define MR_NUM_SHIFT                            208
#define ROOM_NUM_SHIFT                          128
#define ADMIT_DATE_SHIFT                        112
#define PATIENT_LIST_VISIBLE_ROWS               9
#define MAX_STRING_LENGTH                       20

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"

typedef struct PATIENT_DATA_STRUCT {
    char *name;
    char *mr_num;
    char *room_num;
    char *admit_date;
} PATIENT_DATA;
  

typedef struct PATIENT_ROW_STRUCT {
    GX_WIDGET background;
    GX_PROMPT name;
    GX_PROMPT mr_num;
    GX_PROMPT room_num;
    GX_PROMPT admit_date;
} PATIENT_ROW;

PATIENT_DATA patient_list[] = {
{ "Anderson, Fionna",       "MRN125",       "1B107",    "May 23, 2016"},
{ "Abramson, Frank",        "MRN117",       "1D205",    "Apr 30, 2016"},
{ "Adderiy, James",         "MRN093",       "1D208",    "Dec 25, 2015"},  
{ "Adrian, Michaela",       "MRN043",       "2B204",    "Jun 15, 2016"},
{ "Aldridge, Robert",       "MRN108",       "2B214",    "May 07, 2016"},
{ "Alsopp, Antony",         "MRN071",       "3A455",    "May 31, 2016"},
{ "Babcock, Margarita",     "MRN055",       "1D310",    "Feb 24, 2016"},
{ "Baldwin, Susan",         "MRN114",       "3A523",    "May 30, 2016"},
{ "Barnes, Fionna",         "MRN125",       "1B107",    "May 23, 2016"},
{ "Bawerman, Frank",        "MRN117",       "1D205",    "Apr 30, 2016"},
{ "Benson, James",   	    "MRN093",       "1D208",    "Dec 25, 2015"},
{ "Birch, Michaela",        "MRN043",       "2B204",    "Jun 15, 2016"},
{ "Calhoun, Rober", 	    "MRN108",       "2B214",    "May 07, 2016"},
{ "Carey, Antony",          "MRN071",       "3A455",    "May 31, 2016"},
{ "Carroll, Margarita",     "MRN055",       "1D310",    "Feb 24, 2016"},
{ "Chandter, Susan",        "MRN114",       "3A523",    "May 30, 2016"},
{ "Charlson, Frank",       	"MRN117",       "1D205",    "Apr 30, 2016"},
{ "Daniels, James",     	"MRN093",       "1D208",    "Dec 25, 2015"},
{ "Day, Michaela",      	"MRN043",       "2B204",    "Jun 15, 2016"},
{ "Derrick, Robert",       	"MRN108",       "2B214",    "May 07, 2016"},
{ "Dodson, Antony",         "MRN071",       "3A455",    "May 31, 2016"},
{ "Eddington, Margarita",   "MRN055",       "1D310",    "Feb 24, 2016"},
{ "Ellington, Susan",       "MRN114",       "3A523",    "May 30, 2016"},
{ "Faber, Fionna",          "MRN125",       "1B107",    "May 23, 2016"},
{ "Farmer, Frank",	        "MRN117",       "1D205",    "Apr 30, 2016"},
{ "Ferguson, James",        "MRN093",       "1D208",    "Dec 25, 2015"},
{ "Fisher, Michaela",       "MRN043",       "2B204",    "Jun 15, 2016"},
{ "Flannagan, Robert",      "MRN108",       "2B214",    "May 07, 2016"},
{ "Galbraith, Antony",      "MRN071",       "3A455",    "May 31, 2016"},
{ "Garrison, Margarita",    "MRN055",       "1D310",    "Feb 24, 2016"},
{ "Gerald, Susan",          "MRN114",       "3A523",    "May 30, 2016"},
{ "Gilbert, Frank",         "MRN117",       "1D205",    "Apr 30, 2016"},
{ "Hailey, James",          "MRN093",       "1D208",    "Dec 25, 2015"},
{ "Hancock, Michaela",      "MRN043",       "2B204",    "Jun 15, 2016"},
{ "Harrison, Robert",       "MRN108",       "2B214",    "May 07, 2016"},
{ "Jacobson, Antony",       "MRN071",       "3A455",    "May 31, 2016"},
{ "Jeff, Margarita",        "MRN055",       "1D310",    "Feb 24, 2016"}, 
{ "Keat, Susan",	        "MRN114",       "3A523",    "May 30, 2016"},
{ "Kendal, Fionna",	        "MRN125",       "1B107",    "May 23, 2016"},
{ "Laird, Frank",           "MRN117",       "1D205",    "Apr 30, 2016"},
{ "Larkins, James",         "MRN093",       "1D208",    "Dec 25, 2015"},
{ "Leapman, Michaela",      "MRN043",       "2B204",    "Jun 15, 2016"},
{ "MacAdam, Robert",        "MRN108",       "2B214",    "May 07, 2016"},
{ "MacDonald, Antony",      "MRN071",       "3A455",    "May 31, 2016"},
{ "Parkinson, Margarita",   "MRN055",       "1D310",    "Feb 24, 2016"},
{ "Samuels, Susan",         "MRN114",       "3A523",    "May 30, 2016"},
{ GX_NULL, GX_NULL, GX_NULL, GX_NULL}
};

PATIENT_ROW row_memory[PATIENT_LIST_VISIBLE_ROWS + 1];

VOID GetPatientName(GX_STRING *string)
{
    string->gx_string_ptr = patient_list[selected_patient_index].name;
    string->gx_string_length = string_length_get(string->gx_string_ptr, MAX_STRING_LENGTH);
}

VOID GetPatientAdmitDate(GX_STRING *string)
{
    string->gx_string_ptr = patient_list[selected_patient_index].admit_date;
    string->gx_string_length = string_length_get(string->gx_string_ptr, MAX_STRING_LENGTH);
}

void patient_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    PATIENT_ROW *row = (PATIENT_ROW *) widget;
    GX_RESOURCE_ID bkgnd_color = GX_COLOR_ID_ODD_ROW;
	GX_BOOL result;
    GX_STRING string;

    if (index & 1)
    {
        bkgnd_color = GX_COLOR_ID_WINDOW_FILL;
    }

	gx_widget_created_test(&row -> background, &result);

    if (!result)
    {
        gx_utility_rectangle_define(&childsize, list->gx_window_client.gx_rectangle_left + 10, 0,
                                                list->gx_window_client.gx_rectangle_right - 10, 27);
        gx_widget_create(&row->background, NULL, list, GX_STYLE_ENABLED, GX_ID_NONE, &childsize);
        gx_widget_status_add(&row -> background, GX_STATUS_SELECTABLE | GX_STATUS_ACCEPTS_FOCUS);

        childsize.gx_rectangle_left += 10;
        childsize.gx_rectangle_right = childsize.gx_rectangle_left + 190;
        gx_prompt_create(&row->name, NULL, &row->background, 0, 
            GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &childsize);
        gx_widget_draw_set(&row->name, patient_prompt_draw);
        gx_prompt_text_color_set(&row->name, GX_COLOR_ID_OFF_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_OFF_WHITE);

        gx_utility_rectangle_shift(&childsize, MR_NUM_SHIFT, 0);
        gx_prompt_create(&row->mr_num, NULL, &row->background, 0, 
            GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &childsize);
        gx_widget_draw_set(&row->mr_num, patient_prompt_draw);
        gx_prompt_text_color_set(&row->mr_num, GX_COLOR_ID_OFF_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_OFF_WHITE);

        gx_utility_rectangle_shift(&childsize, ROOM_NUM_SHIFT, 0);
        gx_prompt_create(&row->room_num, NULL, &row->background, 0, 
            GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &childsize);
        gx_widget_draw_set(&row->room_num, patient_prompt_draw);
        gx_prompt_text_color_set(&row->room_num, GX_COLOR_ID_OFF_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_OFF_WHITE);

        gx_utility_rectangle_shift(&childsize, ADMIT_DATE_SHIFT, 0);
        gx_prompt_create(&row->admit_date, NULL, &row->background, 0, 
            GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &childsize);
        gx_widget_draw_set(&row->admit_date, patient_prompt_draw);
        gx_prompt_text_color_set(&row->admit_date, GX_COLOR_ID_OFF_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_OFF_WHITE);
    }
    gx_widget_fill_color_set(&row->background, bkgnd_color, GX_COLOR_ID_GREEN, bkgnd_color);

    string.gx_string_ptr = patient_list[index].name;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->name, &string);

    string.gx_string_ptr = patient_list[index].mr_num;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->mr_num, &string);

    string.gx_string_ptr = patient_list[index].room_num;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->room_num, &string);

    string.gx_string_ptr = patient_list[index].admit_date;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->admit_date, &string);
}

VOID patient_prompt_draw(GX_PROMPT *prompt)
{
    // assume the style of the background before drawing:
    if (prompt->gx_widget_parent)
    {
        if (prompt->gx_widget_parent->gx_widget_style & GX_STYLE_DRAW_SELECTED)
        {
            prompt->gx_widget_style |= GX_STYLE_DRAW_SELECTED;
        }
        else
        {
            prompt->gx_widget_style &= ~GX_STYLE_DRAW_SELECTED;
        }
    }
    gx_prompt_draw(prompt);
}

VOID patient_list_children_create(PATIENTS_SCREEN_CONTROL_BLOCK *win)
{
    INT count;
    GX_VERTICAL_LIST *list = &win ->patients_screen_patient_list;

    for (count = 0; count <= PATIENT_LIST_VISIBLE_ROWS; count++)
    {
        patient_row_create(list, (GX_WIDGET *) &row_memory[count], count);
    }
}

UINT patients_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
GX_VERTICAL_LIST *list;
PATIENTS_SCREEN_CONTROL_BLOCK *screen = (PATIENTS_SCREEN_CONTROL_BLOCK *) window;

    switch(myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_PATIENT_LIST, GX_EVENT_LIST_SELECT):
        list = &screen ->patients_screen_patient_list;
        gx_vertical_list_selected_index_get(list, &selected_patient_index);
        
        myevent->gx_event_type = GX_SIGNAL(ID_MEDS_BUTTON, GX_EVENT_CLICKED);
        break;
    }
    return med_screen_base_event_handler(window, myevent);;
}
