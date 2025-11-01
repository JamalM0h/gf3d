#ifndef __ENTITY_H__
#define __ENTITY_H__ 

#include "gfc_text.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"

#include "gf3d_mesh.h"
#include "gf3d_texture.h"



typedef struct Entity_S
{ 
	Uint32          _inuse;
	GFC_TextLine	name;
	char            *obj; 
	Mesh			*mesh;
	Texture			*texture;
	GFC_Color       color;
	GFC_Matrix4     matrix;
	GFC_Vector3D    position;
	GFC_Vector3D    rotation;
	GFC_Vector3D    scale;
	GFC_Box         bounds;
	GFC_Vector3D    dirtomove;
	int				ttl;
	Bool			temp;
	GFC_Vector3D    *camera;
	void			(*draw)(struct Entity_S *self);
	void			(*think)(struct Entity_S *self);
	void			(*update)(struct Entity_S *self);
	void            (*free)(struct Entity_S* self);
	void            (*collide)(struct Entity_S *self, struct Entity_S *collider);

}Entity; 

Entity* entity_new();

void entity_free(Entity* ent);

void entity_system_init(Uint32 max_ents);

void entity_draw(Entity* ent, GFC_Vector3D lightPos, GFC_Color lightColor);

void entity_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor);

void entity_system_collision(); 

void entity_think_all();

void entity_update_all();



#endif
