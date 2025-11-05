#ifndef _PROJECTILE_H_ 
#define _PROJECTILE_H_

#include "entity.h"

Entity* create_projectile(GFC_Vector3D position, GFC_Vector3D dir, GFC_Color color);
Entity* create_rocket(GFC_Vector3D position, GFC_Vector3D dir, GFC_Color color, Bool arc); 
Entity* create_explosion(GFC_Vector3D position, GFC_Color color);

#endif