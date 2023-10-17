/* This is a small demo of the high-performance GUIX graphics framework. */

#include <stdio.h>
#include "gx_api.h"

#include "medical_resources.h"
#include "medical_specifications.h"

void ToggleScreen(GX_WIDGET *new_win, GX_WIDGET *old_win);
extern UINT string_length_get(GX_CONST GX_CHAR* input_string, UINT max_string_length);
char current_patient_name[128];

/* Define constants for patients screen.  */
#define MR_NUM_SHIFT                            350
#define ROOM_NUM_SHIFT                          150
#define VERTICAL_LIST_BOTTOM                    222
#define PATIENT_LIST_VISIBLE_ROWS               8
#define MAX_NAME_LENGTH                         30
#define MR_NUM_LENGTH                           6
#define ROOM_NUM_LENTH                          5

typedef struct PATIENT_DATA_STRUCT {
    char *name;
    char *mr_num;
    char *room_num;
} PATIENT_DATA;
    

typedef struct PATIENT_ROW_STRUCT {
    GX_ICON icon;
    GX_PROMPT name;
    GX_PROMPT mr_num;
    GX_PROMPT room_num;
} PATIENT_ROW;

PATIENT_DATA patient_list[] = {
{    "Abdelkader, Justin",      "MRN008",   "3C200" },
{    "Adams, Carl",             "MRN295",   "2A331" },
{    "Brown, Amy",              "MRN399",   "3C201" },
{    "Brunner, Damien",         "MRN117",   "3C202" },
{    "Cleary, Daniel",          "MRN062",   "3C203" },
{    "Davidson, Howard",        "MRN288",   "3C204" },
{    "Datsyuk, Pavel",          "MRN013",   "1B105" },
{    "Eaves, Patrick",          "MRN017",   "1B106" },
{    "Emmerton, Cory",          "MRN025",   "1B107" },
{    "Ericsson, Jonathan",      "MRN052",   "1B108" },
{    "Franzen, Johan",          "MRN093",   "2D211" },
{    "Helm, Darren",            "MRN043",   "2D212" },
{    "Howard, Jimmy",           "MRN035",   "2D213" },
{    "Jones, Curtis",           "MRN243",   "2D214" },
{    "Klein, Leonard",          "MRN141",   "2D215" },
{    "Kronwall, Niklas",        "MRN055",   "3A336" },
{    "Lamie, Bill",             "MRN001",   "3A333" },
{    "Miller, Christopher",     "MRN014",   "3A334" },
{    "Miller, Drew",            "MRN020",   "3A335" },
{    "Pinker, Lewis",           "MRN234",   "5D002" },
{    "Smith, Brendan",          "MRN002",   "3A337" },
{    "White, Ian",              "MRN234",   "3A338" },
{    "Zetterberg, Henrik",      "MRN040",   "3A339" }
};

PATIENT_ROW row_memory[PATIENT_LIST_VISIBLE_ROWS + 1];

void patient_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    PATIENT_ROW *row = (PATIENT_ROW *) widget;
    GX_RESOURCE_ID background = GX_PIXELMAP_ID_BACKGROUND_ODD_ROW;
	GX_BOOL result;
    GX_STRING string;

    if (index & 1)
    {
        background = GX_PIXELMAP_ID_BACKGROUND_EVEN_ROW;
    }

	gx_widget_created_test(&row -> icon, &result);
    if (!result)
    {
        gx_icon_create(&row -> icon, NULL, list, background, GX_STYLE_ENABLED, GX_ID_NONE, 0, 0);
        gx_widget_status_add(&row ->icon, GX_STATUS_SELECTABLE | GX_STATUS_ACCEPTS_FOCUS);

        gx_utility_rectangle_define(&childsize, 10, 3, 300, 36);
        gx_prompt_create(&row ->name, NULL, &row ->icon, 0, 
            GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &childsize);

        gx_utility_rectangle_shift(&childsize, MR_NUM_SHIFT, 0);
        childsize.gx_rectangle_right = childsize.gx_rectangle_left + 150;
        gx_prompt_create(&row -> mr_num, NULL, &row -> icon, 0, 
            GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &childsize);

        gx_utility_rectangle_shift(&childsize, ROOM_NUM_SHIFT, 0);
        gx_prompt_create(&row -> room_num, NULL, &row -> icon, 0, 
            GX_STYLE_TRANSPARENT|GX_STYLE_TEXT_LEFT, 0, &childsize);
    }
    gx_icon_pixelmap_set(&row ->icon, background, GX_PIXELMAP_ID_SELECTED_ROW);
    string.gx_string_ptr = patient_list[index].name;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_NAME_LENGTH);
    gx_prompt_text_set_ext(&row->name, &string);

    string.gx_string_ptr = patient_list[index].mr_num;
    string.gx_string_length = MR_NUM_LENGTH;
    gx_prompt_text_set_ext(&row->mr_num, &string);

    string.gx_string_ptr = patient_list[index].room_num;
    string.gx_string_length = ROOM_NUM_LENTH;
    gx_prompt_text_set_ext(&row->room_num, &string);
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
INT              row;
GX_BUTTON        *patients_button;
PATIENTS_SCREEN_CONTROL_BLOCK *screen = (PATIENTS_SCREEN_CONTROL_BLOCK *) window;
GX_WIDGET *widget = (GX_WIDGET *) window;

    switch(myevent ->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_window_event_process(window, myevent);
        gx_widget_find(widget, ID_PATIENTS_BUTTON, 0, (GX_WIDGET **) &patients_button);
        patients_button->gx_button_select_handler((GX_WIDGET *) patients_button);
        break;
            
    case GX_SIGNAL(ID_VITALS_BUTTON, GX_EVENT_RADIO_SELECT):
        ToggleScreen((GX_WIDGET *) &vitals_screen, widget);
        break;

    case GX_SIGNAL(ID_MEDS_BUTTON, GX_EVENT_RADIO_SELECT):
        ToggleScreen((GX_WIDGET *) &meds_screen, widget);
        break;

    case GX_SIGNAL(ID_PATIENT_LIST, GX_EVENT_LIST_SELECT):
        list = &screen ->patients_screen_patient_list;
        gx_vertical_list_selected_index_get(list, &row);
        strcpy(current_patient_name, patient_list[row].name);
        break;
    }
    return gx_window_event_process(window, myevent);
}
