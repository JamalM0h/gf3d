#include "simple_logger.h"

#include "deploy.h"
#include "projectile.h"

void mech_update(Entity* self);
void mech_free(Entity* self);

Entity* mech_spawn(GFC_Vector3D position, GFC_Color color, float rotation)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "mech");
	self->obj = "mech";
	self->mesh = gf3d_mesh_load("models/Mech.obj");
	self->texture = gf3d_texture_load("models/primitives/flatblue.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(4, 4, 4);
	self->update = mech_update;
	self->free = mech_free;
	self->_inuse = 1;

	self->rotation.z = rotation;

	GFC_Box hitbox = gfc_box(self->position.x - 5, self->position.y - 5, self->position.z - 5, 10, 10, 10);

	self->bounds = hitbox;

	self->ttl = 0;

	return self;
}

void mech_update(Entity* self)
{
	if (!self)return;

	self->ttl += 1;

	if (self->ttl >= 500)
	{
		mech_free(self);
	}

	self->bounds.x = self->position.x - 5;
	self->bounds.y = self->position.y - 5;
	self->bounds.z = self->position.z - 5;

	if (self->position.z != 0)
	{
		self->position.z -= 2;
	}
	else
	{
		self->position.z = 0;
	}
}

void mech_free(Entity* self)
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