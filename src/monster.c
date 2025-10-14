#include "simple_logger.h"

#include "monster.h"

void monster_think(Entity* self);
void monster_update(Entity* self);

Entity* monster_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new(); 
	if (!self)return;
	gfc_line_cpy(self->name, "notAgumon");
	self->mesh = gf3d_mesh_load("models/dino/dino.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->think = monster_think;
	self->update = monster_update;
	return self;
}

void monster_think(Entity* self)
{
	if (!self)return;
	self->rotation.z += 0.05;
}
void monster_update(Entity* self)
{
	if (!self)return;

}