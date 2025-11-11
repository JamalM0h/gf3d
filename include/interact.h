#ifndef __INTERACT_H__
#define __INTERACT_H__ 

#include "entity.h"
#include "deploy.h"

Entity* item_container_spawn(GFC_Vector3D position, GFC_Color color, GFC_Vector3D playpos, int itemid);
Entity* jumppad_spawn(GFC_Vector3D position, GFC_Color color);
Entity* speedpad_spawn(GFC_Vector3D position, GFC_Color color);
Entity* randomShrine_spawn(GFC_Vector3D position, GFC_Color color, GFC_Vector3D playpos);
Entity* healthShrine_spawn(GFC_Vector3D position, GFC_Color color); 


#endif