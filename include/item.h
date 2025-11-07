#ifndef __ITEM_H__
#define __ITEM_H__ 

#include "entity.h"

Entity* item_spawn(GFC_Vector3D position, GFC_Color color, GFC_Vector3D dir, Bool isDisplay, int itemid);

#endif