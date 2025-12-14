#ifndef __MONSTER_H__
#define __MONSTER_H__ 

#include "entity.h"
#include "item.h"

Entity* monster_spawn(GFC_Vector3D position, GFC_Color color);
Entity* crawler_spawn(GFC_Vector3D position, GFC_Color color, Entity *player);
Entity* ranger_spawn(GFC_Vector3D position, GFC_Color color, Entity* player);
Entity* flyer_spawn(GFC_Vector3D position, GFC_Color color, Entity* player);

#endif