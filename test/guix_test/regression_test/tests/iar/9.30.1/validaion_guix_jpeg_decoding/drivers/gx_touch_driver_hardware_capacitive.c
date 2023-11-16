
#include   "tx_api.h"
#include   "gx_api.h"
#include   "gx_system.h"
#include   "gx_display.h"
#include   "gx_display_driver_hardware.h"

#define TOUCH_STATE_TOUCHED  1
#define TOUCH_STATE_RELEASED 2
#define MIN_DRAG_DELTA       10


static int last_pos_x;
static int last_pos_y;
static int curpos_x;
static int curpos_y;
static int touch_state;

/**************************************************************************/
VOID SendPenDownEvent(VOID)
{
    GX_EVENT event;
    event.gx_event_type = GX_EVENT_PEN_DOWN;
    event.gx_event_payload.gx_event_pointdata.gx_point_x = curpos_x;
    event.gx_event_payload.gx_event_pointdata.gx_point_y = curpos_y;
    event.gx_event_sender = 0;
    event.gx_event_target = 0;
    event.gx_event_display_handle = BOARD_SCREEN_HANDLE;
    last_pos_x = curpos_x;
    last_pos_y = curpos_y;
    gx_system_event_send(&event);
}

/**************************************************************************/
VOID SendPenDragEvent(VOID)
{
    GX_EVENT event;
    int x_delta = abs(curpos_x - last_pos_x);
    int y_delta = abs(curpos_y - last_pos_y);

    if (x_delta > MIN_DRAG_DELTA || y_delta > MIN_DRAG_DELTA)
    {
        event.gx_event_type = GX_EVENT_PEN_DRAG;
        event.gx_event_payload.gx_event_pointdata.gx_point_x = curpos_x;
        event.gx_event_payload.gx_event_pointdata.gx_point_y = curpos_y;
        event.gx_event_sender = 0;
        event.gx_event_target = 0;
        event.gx_event_display_handle = BOARD_SCREEN_HANDLE;
        last_pos_x = curpos_x;
        last_pos_y = curpos_y;
    
        gx_system_event_fold(&event);
    }
}

/**************************************************************************/
VOID SendPenUpEvent(VOID)
{
    GX_EVENT event;
    event.gx_event_type = GX_EVENT_PEN_UP;
    event.gx_event_payload.gx_event_pointdata.gx_point_x = curpos_x;
    event.gx_event_payload.gx_event_pointdata.gx_point_y = curpos_y;
    event.gx_event_sender = 0;
    event.gx_event_target = 0;
    event.gx_event_display_handle = BOARD_SCREEN_HANDLE;
    last_pos_x = curpos_x;
    last_pos_y = curpos_y;
    gx_system_event_send(&event);
}

/*
  *                     ReadBuffer [0]: The Byte count (0x0A)   
  *                     ReadBuffer [1]: X1 coordinate [15,8]  
  *                     ReadBuffer [2]: X1 coordinate [7,0]
  *                     ReadBuffer [3]: Y1 coordinate [15,8]
  *                     ReadBuffer [4]: Y1 coordinate [7,0]
  *                     ReadBuffer [5]: X2 coordinate [15,8]
  *                     ReadBuffer [6]: X2 coordinate [7,0]
  *                     ReadBuffer [7]: Y2 coordinate [15,8]
  *                     ReadBuffer [8]: Y2 coordinate [7,0]
  *                     ReadBuffer [9]: Gesture command (01: Zoom In 02: Zoom out) 
  *                     ReadBuffer [10]: unused byte
*/
VOID  touch_thread_entry(ULONG thread_input)
{

    TS_StateTypeDef TS_State;
    touch_state = TOUCH_STATE_RELEASED;

    BSP_TS_Init(BOARD_SCREEN_WIDTH, BOARD_SCREEN_HEIGHT);
    
    while(1)
    {
        tx_thread_sleep(2);

        BSP_TS_GetState(&TS_State);
        
#ifdef  STM32F769I_EVAL
        if (!(TS_State.touchDetected))
#else
        if (!(TS_State.TouchDetected))  
#endif
        {
            // no touch, check so see if last was touched
            if (touch_state == TOUCH_STATE_TOUCHED)
            {
                touch_state = TOUCH_STATE_RELEASED;
                SendPenUpEvent();
            }
        }
        else
        {

#ifdef STM32F429I_DISCO
            if (TS_State.X > BOARD_SCREEN_WIDTH || 
                TS_State.Y > BOARD_SCREEN_HEIGHT )
                continue;

            // screen is touched, update coords:
            // screen is rotated, exchange x-coordinate with y-coordinate
              curpos_y = TS_State.X;
            
              curpos_x = ILI9341_LCD_PIXEL_HEIGHT - TS_State.Y;
#elif STM32F769I_EVAL
               
            if (TS_State.touchX[0] > BOARD_SCREEN_WIDTH ||
            TS_State.touchY[0] > BOARD_SCREEN_HEIGHT)
                continue;

            // screen is touched, update coords:
            curpos_x = TS_State.touchX[0];

            curpos_y = TS_State.touchY[0];
#else
             if (TS_State.x > BOARD_SCREEN_WIDTH || 
                TS_State.y > BOARD_SCREEN_HEIGHT )
                continue;
                        // screen is touched, update coords:
              curpos_x = TS_State.x;
            
              curpos_y = TS_State.y;
#endif

              if (touch_state == TOUCH_STATE_RELEASED)
              {
                  touch_state = TOUCH_STATE_TOUCHED;
                  SendPenDownEvent();
              }
              else
              {
                  // test and send pen drag
                  SendPenDragEvent();
              }
        }
    }
}
