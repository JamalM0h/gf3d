#include "simple_logger.h"

#include "world.h"

void world_think(Entity* self);
void world_update(Entity* self);

Entity* world_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "world");
	self->obj = "world";
	self->mesh = gf3d_mesh_load("models/DaWorld.obj");
	self->texture = gf3d_texture_load("models/DaWorld.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(5, 5, 1);
	self->think = world_think;
	self->update = world_update;
	return self;
}

void world_think(Entity* self)
{
	if (!self)return;
}
void world_update(Entity* self)
{
	if (!self)return;

}