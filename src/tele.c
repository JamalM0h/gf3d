#include "simple_logger.h"

#include "deploy.h"

void tele_think(Entity* self);
void tele_update(Entity* self);
void tele_free(Entity* self);
void tele_collide(Entity* self, Entity* collide);

Entity* tele_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "tele");
	self->obj = "tele";
	self->mesh = gf3d_mesh_load("models/primitives/cube.obj");
	self->texture = gf3d_texture_load("models/primitives/flatgreen.png");
	self->color = color;
	self->position = position;
	self->position.z -= 3.5;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(2, 2, 0.1);
	self->think = tele_think; 
	self->update = tele_update; 
	self->free = tele_free; 
	self->collide = tele_collide; 
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 0.5, self->position.y - 0.5, self->position.z - 0.5, 5, 10, 5);

	self->bounds = hitbox;

	return self;
}

void tele_think(Entity* self)
{
	if (!self)return;
}
void tele_update(Entity* self)
{
	if (!self)return;

	self->bounds.x = self->position.x - 0.5;
	self->bounds.y = self->position.y - 0.5;
	self->bounds.z = self->position.z - 0.5;
}

void tele_free(Entity* self)
{
	if (!self)return;
	if (self->mesh)
	{
		gf3d_mesh_free(self->mesh);
	}
	if (self->texture)
	{
		gf3d_texture_free(self->texture);
	}
	memset(self, 0, sizeof(Entity));
}

void tele_collide(Entity* self, Entity* collide)
{
	if (!self)return;
}