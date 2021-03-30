#include "event.h"

void event_init(event *p_event, event_trigger event_trigger, event_type event_type, void *p_param, int index_x, int index_y)
{
  p_event->p_param = p_param;
  p_event->has_triggered = false;
  p_event->o_event_trigger = event_trigger;
  p_event->o_event_type = event_type;
  p_event->index_src_x = index_x;
  p_event->index_src_y = index_y;
}

void event_free(event *p_event)
{
  switch (p_event->o_event_type)
  {
  case EVENT_TYPE_WARP:
    free((event_param_warp *)p_event->p_param);
    break;
  case EVENT_TYPE_MONEY:
    free((int *)p_event->p_param);
    break;
  default:
    break;
  }
}
