#ifndef __DEPLOY__
#define __DEPLOY__ 

#include "entity.h"

Entity* tele_spawn(GFC_Vector3D position, GFC_Color color);
Entity* turret_spawn(GFC_Vector3D position, GFC_Color color);
Entity* buff_field_spawn(GFC_Vector3D position, GFC_Color color); 
Entity* health_field_spawn(GFC_Vector3D position, GFC_Color color); 
Entity* mech_spawn(GFC_Vector3D position, GFC_Color color, float rotation); 
Entity* rocket_beacon_spawn(GFC_Vector3D position, GFC_Color color);

#endif