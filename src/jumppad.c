#include "simple_logger.h"

#include "interact.h"

void jumppad_update(Entity* self);
void jumppad_free(Entity* self);

Entity* jumppad_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "jumppad");
	self->obj = "jumppad";
	self->mesh = gf3d_mesh_load("models/primitives/cube.obj");
	self->texture = gf3d_texture_load("models/primitives/flatorange.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(3, 3, 0.3);
	self->update = jumppad_update;
	self->free = jumppad_free;
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 1.5, self->position.y - 1.5, self->position.z - 2.5, 3, 3, 10);

	self->bounds = hitbox;

	return self;
}

void jumppad_update(Entity* self)
{
	if (!self)return;

	self->bounds.x = self->position.x - 1.5;
	self->bounds.y = self->position.y - 1.5;
	self->bounds.z = self->position.z - 5;
}

void jumppad_free(Entity* self)
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