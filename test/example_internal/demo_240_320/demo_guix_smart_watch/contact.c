/* This is a small demo of the high-performance GUIX graphics framework. */

#include "demo_guix_smart_watch.h"

#define CONTACT_LIST_VISIBLE_ROWS 8
#define CONTACT_INFO_LIST_TOTAL_ROWS 4

extern GX_WINDOW_ROOT    *root;

CONTACT_INFO contact_info[] = {
    { "Anderson, Fionna", "Anderson", "Fionna", "(213)123-6789", "(818)456-6789", "fionna.anderson@microsoft.com", "9 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_FEMALE},
    { "Abblecoin, Frank", "Abblecoin", "Frank", "(213)123-6789", "(818)456-6789", "frank.abblecoin@microsoft.com", "12 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Almasharon, James", "Almasharon", "James", "(213)123-6789", "(818)456-6789", "james.almasharon@microsoft.com", "8 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Balimon, Michaela", "Balimon", "Michaela", "(213)123-6789", "(818)456-6789", "michaela.balimon@microsoft.com", "11 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_FEMALE },
    { "Brown, Robert", "Brown", "Robert", "(213)123-6789", "(818)456-6789", "robert.brown@microsoft.com", "12 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Cobenstein, Antony", "Cobenstein", "Antony", "(213)123-6789", "(818)456-6789", "antony.cobenstein@microsoft.com", "16 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Donalds, Margarita", "Donalds", "Margarita", "(213)123-6789", "(818)456-6789", "margarita.donalds@microsoft.com", "12 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Larkins, James", "Larkins", "James", "(213)123-6789", "(818)456-6789", "james.larkins@microsoft.com", "18 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Leapman, Michaela", "Leapman", "Michaela", "(213)123-6789", "(818)456-6789", "mchaela.leapman@microsoft.com", "29 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "MacDonald, Antony", "MacDonald", "Antony", "(213)123-6789", "(818)456-6789", "antony.macDonald@microsoft.com", "12 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Samuels, Susan", "Samuels", "Susan", "(213)123-6789", "(818)456-6789", "susan.samuels@microsoft.com", "12 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_FEMALE },
    { "Triupool, Robert", "Triupool", "Robert", "(213)123-6789", "(818)456-6789", "robert.triupool@microsoft.com", "12 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "Weinstain, Felix", "Weinstain", "Felix", "(213)123-6789", "(818)456-6789", "felix.weinstain@microsoft.com", "12 Ash Ave., Kentfield, CA 94904", GX_PIXELMAP_ID_USER_MALE },
    { "", "", "", "", "", "", "", 0}
};

/* CITY list row structure. */
typedef struct CITY_LIST_ROW_STRUCT {
    GX_PROMPT bg;
    GX_PROMPT contact_name;
} CONTACT_LIST_ROW;

CONTACT_LIST_ROW contact_list_row_memory[CONTACT_LIST_VISIBLE_ROWS + 1];
VOID contact_info_set(int index);
VOID contact_info_edit_init(int index);
VOID contact_info_edit_save(int index);
extern VOID new_msg_data_add(INT contact_id);

/*************************************************************************************/
/* Handle contact screen                                                             */
/*************************************************************************************/
UINT contact_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    INT selected_index;

    switch (event_ptr->gx_event_type)
    {
    case GX_EVENT_SHOW:
        gx_vertical_list_selected_set(&contact_screen.contact_screen_contact_list, -1);
        return template_main_event_handler(window, event_ptr);

    case GX_SIGNAL(ID_CONTACT_LIST, GX_EVENT_LIST_SELECT):
        gx_vertical_list_selected_index_get(&contact_screen.contact_screen_contact_list, &selected_index);
        contact_info_set(selected_index);
        screen_switch((GX_WIDGET *)window->gx_widget_parent, (GX_WIDGET *)&contact_info_screen);
        break;

    default:
        return gx_window_event_process(window, event_ptr);
    }

    return 0;
}

/*************************************************************************************/
VOID contact_list_row_create(GX_VERTICAL_LIST *list, GX_WIDGET *widget, INT index)
{
    GX_RECTANGLE childsize;
    CONTACT_LIST_ROW *row = (CONTACT_LIST_ROW *)widget;
    GX_BOOL result;
    GX_STRING string;

    gx_widget_created_test(&row->bg, &result);
    if (!result)
    {
        gx_utility_rectangle_define(&childsize, 0, 5, 215, 35);
        gx_prompt_create(&row->bg, NULL, (GX_WIDGET *)list, 0, GX_STYLE_TRANSPARENT | GX_STYLE_BORDER_NONE | GX_STYLE_ENABLED, 0, &childsize);
        gx_widget_fill_color_set(&row->bg, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY, GX_COLOR_ID_DARK_GRAY);

        gx_utility_rectangle_define(&childsize, 5, 5, 215, 35);
        gx_prompt_create(&row->contact_name, NULL, (GX_WIDGET *)&row->bg, 0, GX_STYLE_TRANSPARENT | GX_STYLE_TEXT_LEFT | GX_STYLE_BORDER_NONE, 0, &childsize);
        gx_prompt_text_color_set(&row->contact_name, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE, GX_COLOR_ID_MILK_WHITE);
        gx_prompt_font_set(&row->contact_name, GX_FONT_ID_SYSTEM);
    }

    /* Set full name. */
    string.gx_string_ptr = contact_info[index].name;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_FULL_NAME_LENGTH);
    gx_prompt_text_set_ext(&row->contact_name, &string);

    if (index & 1)
    {
        gx_widget_style_add(&row->bg, GX_STYLE_TRANSPARENT);
    }
    else
    {
        gx_widget_style_remove(&row->bg, GX_STYLE_TRANSPARENT);
    }
}

/*************************************************************************************/
VOID contact_list_children_create(GX_VERTICAL_LIST *list)
{
    INT count;

    for (count = 0; count < CONTACT_LIST_VISIBLE_ROWS + 1; count++)
    {
        contact_list_row_create(list, (GX_WIDGET *)&contact_list_row_memory[count], count);
    }
}

/*************************************************************************************/
/* Handle contact info screen                                                        */
/*************************************************************************************/
UINT contact_info_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    static INT selected_index;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_EDIT, GX_EVENT_CLICKED):
        /* Forward to contact information edit screen.  */
        screen_switch((GX_WIDGET *)window->gx_widget_parent, (GX_WIDGET *)&contact_info_edit_screen);

        gx_vertical_list_selected_index_get(&contact_screen.contact_screen_contact_list, &selected_index);
        contact_info_edit_init(selected_index);
        break;

    case GX_SIGNAL(ID_BACK, GX_EVENT_CLICKED):
        /* Back to contact screen.  */
        screen_switch((GX_WIDGET *)window->gx_widget_parent, (GX_WIDGET *)&contact_screen);
        break;

    case GX_SIGNAL(ID_TEXT_MOBILE, GX_EVENT_CLICKED):
    case GX_SIGNAL(ID_TEXT_OFFICE, GX_EVENT_CLICKED):
        gx_vertical_list_selected_index_get(&contact_screen.contact_screen_contact_list, &selected_index);
        new_msg_data_add(selected_index);
        screen_switch((GX_WIDGET *)window->gx_widget_parent, (GX_WIDGET *)&msg_send_screen);
        break;
    }

    return gx_window_event_process(window, event_ptr);
}

/*************************************************************************************/
VOID contact_info_set(int index)
{
    GX_STRING string;

    /* Set first name. */
    string.gx_string_ptr = contact_info[index].firstname;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_NAME_LENGTH);
    gx_prompt_text_set_ext(&contact_info_screen.contact_info_screen_first_name, &string);

    /* Set last name. */
    string.gx_string_ptr = contact_info[index].lastname;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_NAME_LENGTH);
    gx_prompt_text_set_ext(&contact_info_screen.contact_info_screen_last_name, &string);

    /* Set background map. */
    gx_window_wallpaper_set((GX_WINDOW *)&contact_info_screen, contact_info[index].image_id, GX_NULL);

    /* Set mobile phone number. */
    string.gx_string_ptr = contact_info[index].mobile;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_PHONE_NUMBER_LENGTH);
    gx_prompt_text_set_ext(&contact_info_list.contact_info_list_mobile, &string);

    /* Set office phone number. */
    string.gx_string_ptr = contact_info[index].office;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_PHONE_NUMBER_LENGTH);
    gx_prompt_text_set_ext(&contact_info_list.contact_info_list_office, &string);

    /* Set email address. */
    string.gx_string_ptr = contact_info[index].email;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_EMAIL_LENGTH);
    gx_prompt_text_set_ext(&contact_info_list.contact_info_list_email, &string);

    /* Set address. */
    string.gx_string_ptr = contact_info[index].address;
    string.gx_string_length = string_length_get(string.gx_string_ptr, MAX_ADDRESS_LENGTH);
    gx_multi_line_text_view_text_set_ext(&contact_info_list.contact_info_list_address, &string);
}

/*************************************************************************************/
/* Handle contact info edit screen                                                   */
/*************************************************************************************/
UINT contact_info_edit_screen_event_handler(GX_WINDOW *window, GX_EVENT *event_ptr)
{
    INT selected_index;

    switch (event_ptr->gx_event_type)
    {
    case GX_SIGNAL(ID_SAVE, GX_EVENT_CLICKED):
        /* Save modifications and back to contact information screen.  */
        gx_vertical_list_selected_index_get(&contact_screen.contact_screen_contact_list, &selected_index);
        contact_info_edit_save(selected_index);

        screen_switch((GX_WIDGET *)window->gx_widget_parent, (GX_WIDGET *)&contact_info_screen);
        break;

    case GX_SIGNAL(ID_CLOSE, GX_EVENT_CLICKED):
        /* Back to contact information screen.  */
        screen_switch((GX_WIDGET *)window->gx_widget_parent, (GX_WIDGET *)&contact_info_screen);
        break;
    }

    return template_main_event_handler(window, event_ptr);
}

/*************************************************************************************/
VOID contact_info_edit_init(int index)
{
    /* Clear text input buffer.  */
    gx_single_line_text_input_buffer_clear(&contact_info_edit_list.contact_info_edit_list_firstname);
    gx_single_line_text_input_buffer_clear(&contact_info_edit_list.contact_info_edit_list_lastname);
    gx_single_line_text_input_buffer_clear(&contact_info_edit_list.contact_info_edit_list_mobile);
    gx_single_line_text_input_buffer_clear(&contact_info_edit_list.contact_info_edit_list_office);
    gx_single_line_text_input_buffer_clear(&contact_info_edit_list.contact_info_edit_list_email);
    gx_single_line_text_input_buffer_clear(&contact_info_edit_list.contact_info_edit_list_address);

    /* Initiate text input with contactor's information.  */
    gx_single_line_text_input_character_insert(&contact_info_edit_list.contact_info_edit_list_firstname, (GX_UBYTE *)contact_info[index].firstname, string_length_get(contact_info[index].firstname, MAX_NAME_LENGTH));
    gx_single_line_text_input_character_insert(&contact_info_edit_list.contact_info_edit_list_lastname, (GX_UBYTE *)contact_info[index].lastname, string_length_get(contact_info[index].lastname, MAX_NAME_LENGTH));
    gx_single_line_text_input_character_insert(&contact_info_edit_list.contact_info_edit_list_mobile, (GX_UBYTE *)contact_info[index].mobile, string_length_get(contact_info[index].mobile, MAX_PHONE_NUMBER_LENGTH));
    gx_single_line_text_input_character_insert(&contact_info_edit_list.contact_info_edit_list_office, (GX_UBYTE *)contact_info[index].office, string_length_get(contact_info[index].office, MAX_PHONE_NUMBER_LENGTH));
    gx_single_line_text_input_character_insert(&contact_info_edit_list.contact_info_edit_list_email, (GX_UBYTE *)contact_info[index].email, string_length_get(contact_info[index].email, MAX_EMAIL_LENGTH));
    gx_single_line_text_input_character_insert(&contact_info_edit_list.contact_info_edit_list_address, (GX_UBYTE *)contact_info[index].address, string_length_get(contact_info[index].address, MAX_ADDRESS_LENGTH));
}

/*************************************************************************************/
VOID contact_info_edit_save(int index)
{
    GX_CHAR *buffer;
    UINT content_size;

    /* Save contactor's infor mation.  */
    gx_single_line_text_input_buffer_get(&contact_info_edit_list.contact_info_edit_list_firstname, &buffer, &content_size, GX_NULL);
    memcpy(contact_info[index].firstname, buffer, string_length_get(buffer, MAX_NAME_LENGTH) + 1);

    gx_single_line_text_input_buffer_get(&contact_info_edit_list.contact_info_edit_list_lastname, &buffer, &content_size, GX_NULL);
    memcpy(contact_info[index].lastname, buffer, string_length_get(buffer, MAX_NAME_LENGTH) + 1);

    contact_info[index].name[0] = '\0';
    content_size = string_length_get(contact_info[index].firstname, MAX_NAME_LENGTH);
    memcpy(contact_info[index].name, contact_info[index].firstname, content_size);
    memcpy(contact_info[index].name + content_size, ", ", 1);
    content_size += 1;
    memcpy(contact_info[index].name + content_size, contact_info[index].lastname, string_length_get(contact_info[index].lastname, MAX_NAME_LENGTH) + 1);

    gx_single_line_text_input_buffer_get(&contact_info_edit_list.contact_info_edit_list_mobile, &buffer, &content_size, GX_NULL);
    memcpy(contact_info[index].mobile, buffer, string_length_get(buffer, MAX_PHONE_NUMBER_LENGTH) + 1);

    gx_single_line_text_input_buffer_get(&contact_info_edit_list.contact_info_edit_list_office, &buffer, &content_size, GX_NULL);
    memcpy(contact_info[index].office, buffer, string_length_get(buffer, MAX_PHONE_NUMBER_LENGTH) + 1);

    gx_single_line_text_input_buffer_get(&contact_info_edit_list.contact_info_edit_list_email, &buffer, &content_size, GX_NULL);
    memcpy(contact_info[index].email, buffer, string_length_get(buffer, MAX_EMAIL_LENGTH) + 1);

    gx_single_line_text_input_buffer_get(&contact_info_edit_list.contact_info_edit_list_address, &buffer, &content_size, GX_NULL);
    memcpy(contact_info[index].address, buffer, string_length_get(buffer, MAX_ADDRESS_LENGTH) + 1);
}

/*************************************************************************************/
VOID custom_text_input_draw(GX_SINGLE_LINE_TEXT_INPUT *text_input)
{
    GX_RECTANGLE dirty;

    gx_single_line_text_input_draw(text_input);

    dirty.gx_rectangle_left = text_input->gx_widget_size.gx_rectangle_left;
    dirty.gx_rectangle_right = text_input->gx_widget_size.gx_rectangle_right;
    dirty.gx_rectangle_top = text_input->gx_widget_size.gx_rectangle_bottom - 5;
    dirty.gx_rectangle_bottom = text_input->gx_widget_size.gx_rectangle_bottom - 5;

    gx_canvas_drawing_initiate(root->gx_window_root_canvas, (GX_WIDGET *)text_input, &dirty);
    gx_context_brush_width_set(0);
    gx_context_fill_color_set(GX_COLOR_ID_DARK_GRAY);
    gx_canvas_rectangle_draw(&dirty);
    gx_canvas_drawing_complete(root->gx_window_root_canvas, GX_FALSE);
}
