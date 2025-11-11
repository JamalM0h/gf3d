#include "simple_logger.h"

#include "deploy.h"
 
void health_field_update(Entity* self);
void health_field_free(Entity* self);

Entity* health_field_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "healthfield");
	self->obj = "healthfield";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatgreen.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(20, 20, 0.1);
	self->update = health_field_update; 
	self->free = health_field_free; 
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 15, self->position.y - 15, self->position.z, 30, 30, 30);

	self->bounds = hitbox;

	self->ttl = 0;

	return self;
}

void health_field_update(Entity* self)
{
	if (!self)return;

	self->ttl += 1;

	if (self->ttl >= 500)
	{
		buff_field_free(self);
	}

	self->bounds.x = self->position.x - 15;
	self->bounds.y = self->position.y - 15;
	self->bounds.z = self->position.z;
}

void health_field_free(Entity* self)
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