/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** GUIX Component                                                        */
/**                                                                       */
/**   Touch Component    (Touch)                                          */
/**                                                                       */
/**************************************************************************/

#define GX_SOURCE_CODE

#include "gx_api.h"
#include "gx_system.h"
#include "gx_widget.h"
#include "gx_multi_line_text_view.h"
#include "gx_icon.h"

#if !defined(GX_TARGET_WIN32)

#if defined(GX_DISABLE_THREADX_BINDING)
#define GX_TOUCH_TASK_SLEEP  GX_GENERIC_TIME_DELAY
#else
#define GX_TOUCH_TASK_SLEEP tx_thread_sleep
#endif

VOID _gx_touch_driver_generic_resistive_setup(GX_RESISTIVE_TOUCH *touch, GX_RESISTIVE_TOUCH_INFO *info);
VOID _gx_touch_driver_generic_resistive_update(GX_RESISTIVE_TOUCH *touch);
VOID _gx_touch_driver_generic_resistive_calibrate(GX_RESISTIVE_TOUCH *touch);

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_pen_down_event_send              */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This internal function sends GX_EVENT_PEN_DOWN into GUIX            */
/*      event queue.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_event_send                                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_touch_driver_generic_resistive_pen_down_event_send(GX_RESISTIVE_TOUCH *touch)
{
    GX_EVENT event;
    event.gx_event_type = GX_EVENT_PEN_DOWN;
    event.gx_event_payload.gx_event_pointdata = touch -> gx_resistive_touch_current_touch_coord;
    event.gx_event_sender = 0;
    event.gx_event_target = 0;
    event.gx_event_display_handle = 0;
    _gx_system_event_send(&event);
    touch -> gx_resistive_touch_last_touch_coord = touch -> gx_resistive_touch_current_touch_coord;
    touch -> gx_resistive_touch_last_touch_state = GX_TOUCH_STATE_TOUCHED;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_pen_drag_event_send              */
/*                                                        PORTABLE C      */
/*                                                           6.1.6        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This internal function sends GX_EVENT_PEN_DRAG into GUIX            */
/*      event queue.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_event_send                                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  04-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            replace usage of abs(),     */
/*                                            resulting in version 6.1.6  */
/*                                                                        */
/**************************************************************************/

static VOID _gx_touch_driver_generic_resistive_pen_drag_event_send(GX_RESISTIVE_TOUCH *touch)
{
    GX_EVENT event;
    INT x_delta = GX_ABS(touch -> gx_resistive_touch_current_touch_coord.gx_point_x - touch -> gx_resistive_touch_last_touch_coord.gx_point_x);
    INT y_delta = GX_ABS(touch -> gx_resistive_touch_current_touch_coord.gx_point_y - touch -> gx_resistive_touch_last_touch_coord.gx_point_y);

    if (x_delta > touch -> gx_resistive_touch_min_drag_delta ||
        y_delta > touch -> gx_resistive_touch_min_drag_delta)
    {
        event.gx_event_type = GX_EVENT_PEN_DRAG;
        event.gx_event_payload.gx_event_pointdata = touch -> gx_resistive_touch_current_touch_coord;
        event.gx_event_sender = 0;
        event.gx_event_target = 0;
        event.gx_event_display_handle = 0;
        touch -> gx_resistive_touch_last_touch_coord = touch -> gx_resistive_touch_current_touch_coord;
        _gx_system_event_fold(&event);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_pen_up_event_send                */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    This internal function sends GX_EVENT_PEN_UP into GUIX              */
/*      event queue.                                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_system_event_send                                               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_touch_driver_generic_resistive_pen_up_event_send(GX_RESISTIVE_TOUCH *touch)
{
    GX_EVENT event;
    event.gx_event_type = GX_EVENT_PEN_UP;
    event.gx_event_payload.gx_event_pointdata = touch->gx_resistive_touch_current_touch_coord;
    event.gx_event_sender = 0;
    event.gx_event_target = 0;
    event.gx_event_display_handle = 0;
    touch -> gx_resistive_touch_last_touch_coord = touch -> gx_resistive_touch_current_touch_coord;
    _gx_system_event_send(&event);
    touch -> gx_resistive_touch_last_touch_state = GX_TOUCH_STATE_RELEASED;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_coord_calculate                  */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Internal function to convert x,y adc position into x,y pixel        */
/*      positio.                                                          */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*    raw_coord                             ADC coordinate measurement    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Updates touch control structure                                     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_touch_driver_generic_resistive_coord_calculate(GX_RESISTIVE_TOUCH *touch,
                                                               GX_POINT *raw_coord)
{
    UINT ret_val = (UINT) GX_SUCCESS;
    TOUCH_CALIBRATION_MATRIX *matrix_ptr = &touch->gx_resistive_touch_calibration_matrix;

    if (matrix_ptr->Divider != 0)
    {
        touch -> gx_resistive_touch_current_touch_coord.gx_point_x = 
                        (GX_VALUE) (((matrix_ptr -> An * raw_coord -> gx_point_x) +
                          (matrix_ptr -> Bn * raw_coord -> gx_point_y) +
                           matrix_ptr -> Cn) / matrix_ptr -> Divider);

        touch -> gx_resistive_touch_current_touch_coord.gx_point_y =
                        (GX_VALUE) (((matrix_ptr -> Dn * raw_coord -> gx_point_x) +
                          (matrix_ptr -> En * raw_coord -> gx_point_y) +
                           matrix_ptr -> Fn) / matrix_ptr -> Divider);
    }
    else
    {
        ret_val = (UINT) GX_FAILURE;
    }

    return(ret_val) ;
} 


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_setup            PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Assign the calibration matrix used to do raw coordinates            */
/*    translation to pixel coordinates.                                   */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*    info                                  touch configuration           */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Updates touch control structure                                     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    memset                                                              */
/*    memcpy                                                              */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application                                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_touch_driver_generic_resistive_setup(GX_RESISTIVE_TOUCH *touch, GX_RESISTIVE_TOUCH_INFO *info)
{
    memset(touch, 0, sizeof(GX_RESISTIVE_TOUCH));
    memcpy(&touch->gx_resistive_touch_info, info, sizeof(GX_RESISTIVE_TOUCH_INFO)); /* Use case of memcpy is verified. */
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_calibration_matrix_set           */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Assign the calibration matrix used to convert raw touch coordinates */
/*    to x,y pixel position.                                              */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*    raw_coord                             ADC coordinate measurement    */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Updates touch control structure                                     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static UINT _gx_touch_driver_generic_resistive_calibration_matrix_set(GX_RESISTIVE_TOUCH *touch,
                                                                     GX_POINT *display_points,
                                                                     GX_POINT *touch_points)
{
    UINT ret_val = (UINT) GX_SUCCESS ;
    TOUCH_CALIBRATION_MATRIX *matrix_ptr = &touch->gx_resistive_touch_calibration_matrix;

    matrix_ptr -> Divider = ((touch_points[0].gx_point_x - touch_points[2].gx_point_x) * (touch_points[1].gx_point_y - touch_points[2].gx_point_y)) -
        ((touch_points[1].gx_point_x - touch_points[2].gx_point_x) * (touch_points[0].gx_point_y - touch_points[2].gx_point_y)) ;

    if (matrix_ptr->Divider == 0)
    {
        ret_val = (UINT) GX_FAILURE;
    }
    else
    {
        matrix_ptr -> An = ((display_points[0].gx_point_x - display_points[2].gx_point_x) * (touch_points[1].gx_point_y - touch_points[2].gx_point_y)) -
                        ((display_points[1].gx_point_x - display_points[2].gx_point_x) * (touch_points[0].gx_point_y - touch_points[2].gx_point_y)) ;

        matrix_ptr -> Bn = ((touch_points[0].gx_point_x - touch_points[2].gx_point_x) * (display_points[1].gx_point_x - display_points[2].gx_point_x)) -
                        ((display_points[0].gx_point_x - display_points[2].gx_point_x) * (touch_points[1].gx_point_x - touch_points[2].gx_point_x)) ;

        matrix_ptr -> Cn = (touch_points[2].gx_point_x * display_points[1].gx_point_x - touch_points[1].gx_point_x * display_points[2].gx_point_x) * touch_points[0].gx_point_y +
                        (touch_points[0].gx_point_x * display_points[2].gx_point_x - touch_points[2].gx_point_x * display_points[0].gx_point_x) * touch_points[1].gx_point_y +
                        (touch_points[1].gx_point_x * display_points[0].gx_point_x - touch_points[0].gx_point_x * display_points[1].gx_point_x) * touch_points[2].gx_point_y ;

        matrix_ptr -> Dn = ((display_points[0].gx_point_y - display_points[2].gx_point_y) * (touch_points[1].gx_point_y - touch_points[2].gx_point_y)) -
                        ((display_points[1].gx_point_y - display_points[2].gx_point_y) * (touch_points[0].gx_point_y - touch_points[2].gx_point_y)) ;

        matrix_ptr -> En = ((touch_points[0].gx_point_x - touch_points[2].gx_point_x) * (display_points[1].gx_point_y - display_points[2].gx_point_y)) -
                        ((display_points[0].gx_point_y - display_points[2].gx_point_y) * (touch_points[1].gx_point_x - touch_points[2].gx_point_x)) ;

        matrix_ptr -> Fn = (touch_points[2].gx_point_x * display_points[1].gx_point_y - touch_points[1].gx_point_x * display_points[2].gx_point_y) * touch_points[0].gx_point_y +
                        (touch_points[0].gx_point_x * display_points[2].gx_point_y - touch_points[2].gx_point_x * display_points[0].gx_point_y) * touch_points[1].gx_point_y +
                        (touch_points[1].gx_point_x * display_points[0].gx_point_y - touch_points[0].gx_point_x * display_points[1].gx_point_y) * touch_points[2].gx_point_y ;
    }

    return(ret_val) ;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_raw_read                         */
/*                                                        PORTABLE C      */
/*                                                           6.1.6        */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Called to read raw touch coordinate and perform debounce logic      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*    ret_val                               Pointer to coord storage      */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Updates touch control structure                                     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    Indirect call to user-supplied hardware read function               */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*  04-02-2021     Kenneth Maxwell          Modified comment(s),          */
/*                                            replace usage of abs(),     */
/*                                            resulting in version 6.1.6  */
/*                                                                        */
/**************************************************************************/
static GX_BOOL _gx_touch_driver_generic_resistive_raw_read(GX_RESISTIVE_TOUCH *touch, GX_POINT *ret_val)
{
    GX_POINT *samples;
    INT loop;
    INT delta_x;
    INT delta_y;
    INT retries = 0;
    INT stability_limit;
    INT sample_size;
    INT max_retries;
    GX_BOOL stable_sample = GX_FALSE;
    USHORT (*read_function)(GX_VALUE axis);

    samples = touch -> gx_resistive_touch_sample_ram;
    read_function = touch -> gx_resistive_touch_sample_read;
    stability_limit = touch -> gx_resistive_touch_stability_limit;
    sample_size = touch -> gx_resistive_touch_sample_size;
    max_retries = touch -> gx_resistive_touch_max_retries;

    while(!stable_sample)
    {
        retries += 1;

        if (retries < max_retries)
        {
            stable_sample = GX_TRUE;
            loop = 0;
            retries += 1;

            while(loop < sample_size)
            {
                samples[loop].gx_point_x = (GX_VALUE) read_function(GX_TOUCH_X_AXIS);
                samples[loop].gx_point_y = (GX_VALUE) read_function(GX_TOUCH_Y_AXIS);

                if (loop > 0)
                {
                    delta_x = GX_ABS(samples[loop].gx_point_x - samples[loop - 1].gx_point_x);
                    if (delta_x > stability_limit)
                    {
                        stable_sample = GX_FALSE;
                        break;
                    }
                    delta_y = GX_ABS(samples[loop].gx_point_y - samples[loop - 1].gx_point_y);
                    if (delta_y > stability_limit)
                    {   
                        stable_sample = GX_FALSE;
                        break;
                    }
                }
                loop++;
            }      
        }
        else
        {
            break;
        }
    }
    
    if (stable_sample)
    {
        /* Average the samples: */

        delta_x = delta_y = 0;

        for (loop = 0; loop < sample_size; loop++)
        {
            delta_x += samples[loop].gx_point_x;
            delta_y += samples[loop].gx_point_y;
        }
        ret_val->gx_point_x = (GX_VALUE) (delta_x / sample_size);
        ret_val->gx_point_y = (GX_VALUE) (delta_y / sample_size);
    }
    return stable_sample;
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_sample_read                      */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Read one stable touch sample                                        */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*    ret_val                               Address to write touch sample */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Updates touch control structure                                     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_touch_driver_generic_raw_read                                   */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static GX_BOOL _gx_touch_driver_generic_resistive_sample_read(GX_RESISTIVE_TOUCH *touch, GX_POINT *ret_val)
{
    GX_POINT raw_val;
    if (_gx_touch_driver_generic_resistive_raw_read(touch, &raw_val))
    {
        ret_val -> gx_point_x = (GX_VALUE) ((raw_val.gx_point_x + 2 ) / 4);
        ret_val -> gx_point_y = (GX_VALUE) ((raw_val.gx_point_y + 2 ) / 4);
        return GX_TRUE;
    }
    return GX_FALSE;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_coordinate_update                */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Read raw sample, convert to x,y coordinates                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Updates touch control structure                                     */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_sample_read                      */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static GX_BOOL _gx_touch_driver_generic_resistive_coordinate_update(GX_RESISTIVE_TOUCH *touch)
{
    GX_POINT sample;
    GX_BOOL status = GX_TRUE;

    if (touch -> gx_resistive_touch_pen_down_detect())
    {
        status =  _gx_touch_driver_generic_resistive_sample_read(touch, &sample);
        if (status)
        {
            touch -> gx_resistive_touch_current_touch_state = GX_TOUCH_STATE_TOUCHED;
            _gx_touch_driver_generic_resistive_coord_calculate(touch, &sample);
        }
    }
    else
    {
        touch -> gx_resistive_touch_current_touch_state = GX_TOUCH_STATE_RELEASED;
    }
    return status;
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_update                           */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Update touch status, send required event(s)                         */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    Touch events generated                                              */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_coordinate_update                */
/*    _gx_touch_driver_generic_resistive_pen_drag_event_send              */
/*    _gx_touch_driver_generic_resistive_pen_down_event_send              */
/*    _gx_touch_driver_generic_resistive_pen_up_event_send                */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application                                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_touch_driver_generic_resistive_update(GX_RESISTIVE_TOUCH *touch)
{
    if (_gx_touch_driver_generic_resistive_coordinate_update(touch))
    {
        if (touch -> gx_resistive_touch_current_touch_state == GX_TOUCH_STATE_TOUCHED)
        {
            if (touch -> gx_resistive_touch_last_touch_state == GX_TOUCH_STATE_TOUCHED)
            {
                _gx_touch_driver_generic_resistive_pen_drag_event_send(touch);
            }
            else
            {
                _gx_touch_driver_generic_resistive_pen_down_event_send(touch);
            }
        }
        else
        {
            if (touch -> gx_resistive_touch_last_touch_state == GX_TOUCH_STATE_TOUCHED)
            {   
                _gx_touch_driver_generic_resistive_pen_up_event_send(touch);
            }
        }
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_wait_for_touch                   */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Wait for touch screen pen down                                      */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    Indirectly calls pen down event detection                           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_touch_driver_generic_resistive_wait_for_touch(GX_RESISTIVE_TOUCH *touch)
{
    while(!touch -> gx_resistive_touch_pen_down_detect())
    {
        GX_TOUCH_TASK_SLEEP(5);
    }
}


/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_wait_for_no_touch                */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Wait for touch screen pen not down                                  */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    Indirectly calls pen down event detection                           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Internal Logic                                                      */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
static VOID _gx_touch_driver_generic_resistive_wait_for_no_touch(GX_RESISTIVE_TOUCH *touch)
{
    while(touch -> gx_resistive_touch_pen_down_detect())
    {
        GX_TOUCH_TASK_SLEEP(5);
    }
}

/**************************************************************************/
/*                                                                        */
/*  FUNCTION                                               RELEASE        */
/*                                                                        */
/*    _gx_touch_driver_generic_resistive_calibrate                        */
/*                                                        PORTABLE C      */
/*                                                           6.1          */
/*  AUTHOR                                                                */
/*                                                                        */
/*    Kenneth Maxwell, Microsoft Corporation                              */
/*                                                                        */
/*  DESCRIPTION                                                           */
/*                                                                        */
/*    Perform touch screen calibration sequence                           */
/*                                                                        */
/*  INPUT                                                                 */
/*                                                                        */
/*    touch                                 Pointer to touch control      */
/*                                            structure.                  */
/*                                                                        */
/*  OUTPUT                                                                */
/*                                                                        */
/*    None                                                                */
/*                                                                        */
/*  CALLS                                                                 */
/*                                                                        */
/*    _gx_widget_hide                                                     */
/*    _gx_widget_show                                                     */
/*    _gx_multi_line_text_view_text_id_set                                */                        
/*    _gx_system_canvas_refresh()                                         */
/*    _gx_touch_driver_generic_resistive_wait_for_no_touch                */
/*    _gx_touch_driver_generic_resistive_wait_for_touch                   */
/*    _gx_touch_driver_generic_resistive_sample_read                      */
/*    GX_TOUCH_TASK_SLEEP                                                 */
/*    _gx_touch_driver_generic_resistive_calibration_matrix_set           */
/*                                                                        */
/*  CALLED BY                                                             */
/*                                                                        */
/*    Application                                                         */
/*                                                                        */
/*  RELEASE HISTORY                                                       */
/*                                                                        */
/*    DATE              NAME                      DESCRIPTION             */
/*                                                                        */
/*  05-19-2020     Kenneth Maxwell          Initial Version 6.0           */
/*  09-30-2020     Kenneth Maxwell          Modified comment(s),          */
/*                                            fixed compiler warnings,    */
/*                                            resulting in version 6.1    */
/*                                                                        */
/**************************************************************************/
VOID _gx_touch_driver_generic_resistive_calibrate(GX_RESISTIVE_TOUCH *touch)
{
    GX_EVENT calibration_event;

    GX_POINT raw_cal_points[3];
    GX_POINT display_cal_points[3];

    int icon_size;
    GX_RECTANGLE newsize;
    GX_BOOL (*pen_down)(VOID);

    GX_MULTI_LINE_TEXT_VIEW *text = touch->gx_resistive_touch_info.gx_touch_info_text;
    GX_WIDGET *icon = (GX_WIDGET *) touch->gx_resistive_touch_info.gx_touch_info_target;
    pen_down = touch -> gx_resistive_touch_pen_down_detect;

    icon_size = icon->gx_widget_size.gx_rectangle_right - icon->gx_widget_size.gx_rectangle_left + 1;

    display_cal_points[0].gx_point_x = (GX_VALUE) (icon_size / 2);
    display_cal_points[0].gx_point_y = (GX_VALUE) (icon_size / 2);
    display_cal_points[1].gx_point_x = (GX_VALUE) (touch->gx_resistive_touch_x_range - (icon_size / 2));
    display_cal_points[1].gx_point_y = (GX_VALUE) (touch->gx_resistive_touch_y_range / 2);
    display_cal_points[2].gx_point_x = (GX_VALUE) (icon_size / 2);
    display_cal_points[2].gx_point_y = (GX_VALUE) (touch->gx_resistive_touch_y_range - (icon_size / 2));

    if (pen_down())
    {
        _gx_widget_hide(icon);
        _gx_multi_line_text_view_text_id_set(text, touch->gx_resistive_touch_info.gx_touch_info_string_id_no_touch);
        _gx_system_canvas_refresh();
        _gx_touch_driver_generic_resistive_wait_for_no_touch(touch);

        _gx_multi_line_text_view_text_id_set(text, touch->gx_resistive_touch_info.gx_touch_info_string_id_touch_1);
        _gx_widget_show(icon);
        _gx_system_canvas_refresh();
    }

    /*  wait for a touch: */
    while(1)
    {
        _gx_touch_driver_generic_resistive_wait_for_touch(touch);
        
        if (_gx_touch_driver_generic_resistive_sample_read(touch, &raw_cal_points[0]))
        {
            break;
        }
        GX_TOUCH_TASK_SLEEP(20);
    }
    _gx_widget_hide(icon);
    _gx_multi_line_text_view_text_set_ext(text, GX_NULL);
    _gx_system_canvas_refresh();
    _gx_touch_driver_generic_resistive_wait_for_no_touch(touch);
    GX_TOUCH_TASK_SLEEP(20);

    /*  move icon to middle-right */
    _gx_widget_shift(icon,
                    (GX_VALUE) (touch->gx_resistive_touch_x_range - icon_size),
                    (GX_VALUE) ((touch->gx_resistive_touch_y_range / 2) - (icon_size / 2)),
                     GX_FALSE);

    _gx_widget_show(icon);
    _gx_multi_line_text_view_text_id_set(text, touch->gx_resistive_touch_info.gx_touch_info_string_id_touch_2);
    _gx_system_canvas_refresh();

    /*  wait for a touch: */
    while(1)
    {
        _gx_touch_driver_generic_resistive_wait_for_touch(touch);
        if (_gx_touch_driver_generic_resistive_sample_read(touch, &raw_cal_points[1]))
        {
            break;
        }
        GX_TOUCH_TASK_SLEEP(20);
    }
    _gx_widget_hide(icon);
    _gx_multi_line_text_view_text_set_ext(text, GX_NULL);
    _gx_system_canvas_refresh();
    _gx_touch_driver_generic_resistive_wait_for_no_touch(touch);
    GX_TOUCH_TASK_SLEEP(20);

    /* move icon to bottom-left */
    newsize.gx_rectangle_left = (GX_VALUE) 0;
    newsize.gx_rectangle_bottom = (GX_VALUE) (touch->gx_resistive_touch_y_range);
    newsize.gx_rectangle_top = (GX_VALUE) (newsize.gx_rectangle_bottom - icon_size + 1);
    newsize.gx_rectangle_right = (GX_VALUE) (newsize.gx_rectangle_left + icon_size - 1);
    _gx_widget_resize(icon, &newsize);
    _gx_widget_show(icon);
    _gx_multi_line_text_view_text_id_set(text, touch->gx_resistive_touch_info.gx_touch_info_string_id_touch_3);
    _gx_system_canvas_refresh();

    /* wait for a touch: */
    while(1)
    {
        _gx_touch_driver_generic_resistive_wait_for_touch(touch);
        if (_gx_touch_driver_generic_resistive_sample_read(touch, &raw_cal_points[2]))
        {
            break;
        }
        GX_TOUCH_TASK_SLEEP(20);
    }
    _gx_widget_hide(icon);
    _gx_multi_line_text_view_text_set_ext(text, GX_NULL);
    _gx_system_canvas_refresh();
    _gx_touch_driver_generic_resistive_wait_for_no_touch(touch);

    /* Initialize touch screen calibration factor matrix with new values  */
    _gx_touch_driver_generic_resistive_calibration_matrix_set(touch,
                         (GX_POINT *) &display_cal_points,   /* Display coordinates   */
                         (GX_POINT *) raw_cal_points);             /* Touch coordinates     */

    touch -> gx_resistive_touch_calibrated = GX_TRUE;

    calibration_event.gx_event_type = GX_EVENT_TOUCH_CALIBRATION_COMPLETE;
    calibration_event.gx_event_target = icon;
    _gx_system_event_send(&calibration_event);
}

#endif  /* WIN32 build conditional */

