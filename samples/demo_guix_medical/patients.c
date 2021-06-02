/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include <time.h>
#include "gx_api.h"

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"

#include "demo_guix_medical.h"

int selected_patient_index = 0;

/* Define constants for patients screen.  */
#define PATIENT_LIST_VISIBLE_ROWS               10
#define PATIENT_LIST_ROW_HEIGHT                 34
#define MAX_STRING_LENGTH                       20

#include "guix_medical_resources.h"
#include "guix_medical_specifications.h"

/* Define patient row structure.  */
typedef struct PATIENT_ROW_STRUCT {
    GX_WIDGET background;
    GX_PROMPT name;
    GX_PROMPT dob;
    GX_PROMPT room_num;
    GX_PROMPT admit_date;
    GX_PROMPT mr_num;
} PATIENT_ROW;

/* Define patient information structure.  */
typedef struct PATIENT_DATA_STRUCT {
    char *name;
    char *dob;
    char *room_num;
    char *admit_date;
    char* mr_num;
} PATIENT_DATA;

/* Define patinet data.  */
PATIENT_DATA patient_list[] = {
{"Anderson, Fionna",      "7/22/1953",  "1B107", "May 23, 2020", "MRN125"},
{"Abramson, Frank",       "3/18/1967",  "1D205", "Apr 30, 2020", "MRN117"},
{"Adderiy, James",        "7/5/1954",   "1D208", "Dec 25, 2019", "MRN093"},
{"Adrian, Michaela",      "11/16/1976", "2B204", "Jun 15, 2020", "MRN043"},
{"Aldridge, Robert",      "6/30/1947",  "2B214", "May 07, 2020", "MRN108"},
{"Alsopp, Antony",        "9/20/1991",  "3A455", "May 31, 2020", "MRN071"},
{"Babcock, Margarita",    "7/11/1962",  "1D310", "Feb 24, 2020", "MRN055"},
{"Baldwin, Susan",        "4/25/1958",  "3A523", "May 30, 2020", "MRN114"},
{"Barnes, Fionna",        "12/6/1974",  "1B107", "May 23, 2020", "MRN125"},
{"Bawerman, Frank",       "5/30/1951",  "1D205", "Apr 30, 2020", "MRN117"},
{"Benson, James",         "11/4/1973",  "1D208", "Dec 25, 2019", "MRN093"},
{"Birch, Michaela",       "4/20/1944",  "2B204", "Jun 15, 2020", "MRN043"},
{"Calhoun, Robert",       "8/2/1990",   "2B214", "May 07, 2020", "MRN108"},
{"Carey, Antony",         "3/26/1980",  "3A455", "May 31, 2020", "MRN071"},
{"Carroll, Margarita",    "9/7/1961",   "1D310", "Feb 24, 2020", "MRN055"},
{"Chandter, Susan",       "7/30/1955",  "3A523", "May 30, 2020", "MRN114"},
{"Charlson, Frank",       "1/30/1995",  "1D205", "Apr 30, 2020", "MRN117"},
{"Daniels, James",        "8/18/1981",  "1D208", "Dec 25, 2019", "MRN093"},
{"Day, Michaela",         "3/11/1949",  "2B204", "Jun 15, 2020", "MRN043"},
{"Derrick, Robert",       "12/5/1953",  "2B214", "May 07, 2020", "MRN108"},
{"Dodson, Antony",        "9/19/1992",  "3A455", "May 31, 2020", "MRN071"},
{"Eddington, Margarita",  "2/23/1985",  "1D310", "Feb 24, 2020", "MRN055"},
{"Ellington, Susan",      "6/17/1979",  "3A523", "May 30, 2020", "MRN114"},
{"Faber, Fionna",         "5/27/1977",  "1B107", "May 23, 2020", "MRN125"},
{"Farmer, Frank",         "11/4/1946",  "1D205", "Apr 30, 2020", "MRN117"},
{"Ferguson, James",       "8/20/1964",  "1D208", "Dec 25, 2019", "MRN093"},
{"Fisher, Michaela",      "5/13/1972",  "2B204", "Jun 15, 2020", "MRN043"},
{"Flannagan, Robert",     "7/8/1959",   "2B214", "May 07, 2020", "MRN108"},
{"Galbraith, Antony",     "11/18/1957", "3A455", "May 31, 2020", "MRN071"},
{"Garrison, Margarita",   "4/13/1940",  "1D310", "Feb 24, 2020", "MRN055"},
{"Gerald, Susan",         "2/14/1960",  "3A523", "May 30, 2020", "MRN114"},
{"Gilbert, Frank",        "3/25/1947",  "1D205", "Apr 30, 2020", "MRN117"},
{"Hailey, James",         "9/10/1953",  "1D208", "Dec 25, 2019", "MRN093"},
{"Hancock, Michaela",     "11/24/1939", "2B204", "Jun 15, 2020", "MRN043"},
{"Harrison, Robert",      "3/5/1970",   "2B214", "May 07, 2020", "MRN108"},
{"Jacobson, Antony",      "5/19/1957",  "3A455", "May 31, 2020", "MRN071"},
{"Jeff, Margarita",       "4/25/1976",  "1D310", "Feb 24, 2020", "MRN055"},
{"Keat, Susan",           "12/27/1979", "3A523", "May 30, 2020", "MRN114"},
{"Kendal, Fionna",        "4/23/1968",  "1B107", "May 23, 2020", "MRN125"},
{"Laird, Frank",          "4/17/1974",  "1D205", "Apr 30, 2020", "MRN117"},
{"Larkins, James",        "11/16/1954", "1D208", "Dec 25, 2019", "MRN093"},
{"Leapman, Michaela",     "5/15/1980",  "2B204", "Jun 15, 2020", "MRN043"},
{"MacAdam, Robert",       "3/10/1987",  "2B214", "May 07, 2020", "MRN108"},
{"MacDonald, Antony",     "12/1/1973",  "3A455", "May 31, 2020", "MRN071"},
{"Parkinson, Margarita",  "7/18/1988",  "1D310", "Feb 24, 2020", "MRN055"},
{"Samuels, Susan",        "5/30/1990",  "3A523", "May 30, 2020", "MRN114"},
{ GX_NULL, GX_NULL, GX_NULL, GX_NULL, GX_NULL}
};

PATIENT_ROW row_memory[PATIENT_LIST_VISIBLE_ROWS + 1];

/* Declare prototypes.  */
VOID patient_list_children_create(PATIENTS_SCREEN_CONTROL_BLOCK* win);
VOID GetPatientName(GX_STRING* string);
VOID GetPatientDOB(GX_STRING* string);
VOID GetPatientAge(INT* age);

/******************************************************************************************/
/* Calculate string length.                                                               */
/******************************************************************************************/
static UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length)
{
UINT length = 0;

    if (input_string)
    {
        /* Traverse the string.  */
        for (length = 0; input_string[length]; length++)
        {
            /* Check if the string length is bigger than the max string length.  */
            if (length >= max_string_length)
            {
                break;
            }
        }
    }

    return length;
}

/******************************************************************************************/
/* Custom widget drawing function for background widgets in patient list.                 */
/******************************************************************************************/
static VOID patient_background_draw(GX_WIDGET* widget)
{

    gx_widget_draw(widget);

    /* Draw a line to the bottom of the widget.  */
    gx_context_brush_width_set(1);
    gx_context_line_color_set(GX_COLOR_ID_PATIENTS_HORZ_LINE);
    gx_canvas_line_draw(widget->gx_widget_size.gx_rectangle_left, widget->gx_widget_size.gx_rectangle_bottom,
                        widget->gx_widget_size.gx_rectangle_right, widget->gx_widget_size.gx_rectangle_bottom);
}

/******************************************************************************************/
/* Custom prompt drawing function for prompt widgets in patient list.                     */
/******************************************************************************************/
static VOID patient_prompt_draw(GX_PROMPT* prompt)
{

    /* Assume the style of the background before drawing.  */
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

/******************************************************************************************/
/* Create a row for patient list.                                                          */
/******************************************************************************************/
VOID patient_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
GX_RECTANGLE childsize;
PATIENT_ROW *row = (PATIENT_ROW *) widget;
GX_BOOL      result;
GX_STRING    string;

    /* Test if the widget is already created.  */
	gx_widget_created_test(&row -> background, &result);

    if (!result)
    {
        /* Create and configure background widget.  */
        gx_utility_rectangle_define(&childsize, list->gx_window_client.gx_rectangle_left, 0,
                                                list->gx_window_client.gx_rectangle_right, PATIENT_LIST_ROW_HEIGHT);
        gx_widget_create(&row->background, NULL, list, GX_STYLE_ENABLED, GX_ID_NONE, &childsize);
        gx_widget_fill_color_set(&row->background, GX_COLOR_ID_BLACK, GX_COLOR_ID_YELLOW, GX_COLOR_ID_BLACK);
        gx_widget_draw_set(&row->background, patient_background_draw);

        /* Create and configure name prompt.  */
        childsize.gx_rectangle_left = patients_screen.patients_screen_patient_name_label.gx_widget_size.gx_rectangle_left;
        childsize.gx_rectangle_right = patients_screen.patients_screen_DOB_label.gx_widget_size.gx_rectangle_left - 1;
        gx_prompt_create(&row->name, NULL, &row->background, 0, GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->name, patient_prompt_draw);
        gx_prompt_text_color_set(&row->name, GX_COLOR_ID_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_WHITE);

        /* Create and configure date of birth prompt.  */
        childsize.gx_rectangle_left = childsize.gx_rectangle_right + 1;
        childsize.gx_rectangle_right = patients_screen.patients_screen_room_label.gx_widget_size.gx_rectangle_left - 1;
        gx_prompt_create(&row->dob, NULL, &row->background, 0, GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->dob, patient_prompt_draw);
        gx_prompt_text_color_set(&row->dob, GX_COLOR_ID_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_WHITE);

        /* Create and configure room number prompt.  */
        childsize.gx_rectangle_left = childsize.gx_rectangle_right + 1;
        childsize.gx_rectangle_right = patients_screen.patients_screen_admission_date_label.gx_widget_size.gx_rectangle_left - 1;
        gx_prompt_create(&row->room_num, NULL, &row->background, 0, GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->room_num, patient_prompt_draw);
        gx_prompt_text_color_set(&row->room_num, GX_COLOR_ID_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_WHITE);

        /* Create and configure admission date prompt.  */
        childsize.gx_rectangle_left = childsize.gx_rectangle_right + 1;
        childsize.gx_rectangle_right = patients_screen.patients_screen_pa_num_label.gx_widget_size.gx_rectangle_left - 1;
        gx_prompt_create(&row->admit_date, NULL, &row->background, 0, GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->admit_date, patient_prompt_draw);
        gx_prompt_text_color_set(&row->admit_date, GX_COLOR_ID_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_WHITE);

        /* Create and configure mr number prompt.  */
        childsize.gx_rectangle_left = childsize.gx_rectangle_right + 1;
        childsize.gx_rectangle_right = list->gx_window_client.gx_rectangle_right;
        gx_prompt_create(&row->mr_num, NULL, &row->background, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT | GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_draw_set(&row->mr_num, patient_prompt_draw);
        gx_prompt_text_color_set(&row->mr_num, GX_COLOR_ID_WHITE, GX_COLOR_ID_BLACK, GX_COLOR_ID_WHITE);
    }

    /* Update patinet name.  */
    string.gx_string_ptr = patient_list[index].name;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->name, &string);

    /* Update date of birth.  */
    string.gx_string_ptr = patient_list[index].dob;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->dob, &string);

    /* Update rom number.  */
    string.gx_string_ptr = patient_list[index].room_num;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->room_num, &string);

    /* Update admission date.  */
    string.gx_string_ptr = patient_list[index].admit_date;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->admit_date, &string);

    /* Update mr number.  */
    string.gx_string_ptr = patient_list[index].mr_num;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_STRING_LENGTH);
    gx_prompt_text_set_ext(&row->mr_num, &string);
}

/******************************************************************************************/
/* Create children for patient list.                                                      */
/******************************************************************************************/
VOID patient_list_children_create(PATIENTS_SCREEN_CONTROL_BLOCK *win)
{
INT               count;
GX_VERTICAL_LIST *list = &win ->patients_screen_patient_list;

    for (count = 0; count <= PATIENT_LIST_VISIBLE_ROWS; count++)
    {

        /* Create one patient row.  */
        patient_row_create(list, (GX_WIDGET *) &row_memory[count], count);
    }
}

/******************************************************************************************/
/* Override the default event processing of "patients_screen" to handle signals from my   */
/* child widgets.                                                                         */
/******************************************************************************************/
UINT patients_screen_event_handler(GX_WINDOW *window, GX_EVENT *myevent)
{
    switch(myevent->gx_event_type)
    {
    case GX_SIGNAL(ID_PATIENT_LIST, GX_EVENT_LIST_SELECT):

        /* Update selected patient index.  */
        gx_vertical_list_selected_index_get(&patients_screen.patients_screen_patient_list, &selected_patient_index);
        break;

    default:
        break;
    }

    /* Pass the event to the even handler of the template on which the patients screen is based.  */
    return template_event_handler(window, myevent);;
}

/******************************************************************************************/
/* Get the name of the selected patient.                                                  */
/******************************************************************************************/
VOID GetPatientName(GX_STRING* string)
{
    string->gx_string_ptr = patient_list[selected_patient_index].name;
    string->gx_string_length = string_length_get(string->gx_string_ptr, MAX_STRING_LENGTH);
}

/******************************************************************************************/
/* Get the date of birth of the selected patient.                                         */
/******************************************************************************************/
VOID GetPatientDOB(GX_STRING* string)
{
    string->gx_string_ptr = patient_list[selected_patient_index].dob;
    string->gx_string_length = string_length_get(string->gx_string_ptr, MAX_STRING_LENGTH);
}

/******************************************************************************************/
/* Get the age of the selected patient.                                                   */
/******************************************************************************************/
VOID GetPatientAge(INT* age)
{
time_t     now;
struct tm *ts;
INT        year;
GX_STRING  string;
UINT       index;

    GetPatientDOB(&string);
    index = string.gx_string_length - 4;

    year = 0;

    while (index < string.gx_string_length)
    {
        year = year * 10;
        year += (string.gx_string_ptr[index] - '0');
        index++;
    }

    now = time(NULL);
    ts = localtime(&now);
    (*age) = ts->tm_year + 1900 - year;
}