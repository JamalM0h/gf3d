#include "simple_logger.h"

#include "deploy.h"

void shockwave_update(Entity* self);
void shockwave_free(Entity* self);

Entity* shockwave_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "shockwave");
	self->obj = "shockwave";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(2, 2, 0.01);
	self->update = shockwave_update; 
	self->free = shockwave_free; 
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 15, 15, 15);

	self->bounds = hitbox;

	self->ttl = 0;

	return self;
}

void shockwave_update(Entity* self)
{
	if (!self)return;

	self->ttl += 1;

	if (self->ttl >= 15)
	{
		shockwave_free(self);  
	}

	self->scale.x += 0.5;
	self->scale.y += 0.5;

	self->bounds.x = self->position.x - 7.5;
	self->bounds.y = self->position.y - 7.5;
	self->bounds.z = self->position.z;
}

void shockwave_free(Entity* self)
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