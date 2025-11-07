#include "simple_logger.h"

#include "deploy.h"
#include "projectile.h"

void buff_field_think(Entity* self);
void buff_field_update(Entity* self);
void buff_field_free(Entity* self);
void buff_field_collide(Entity* self, Entity* collide);

Entity* buff_field_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "bufffield");
	self->obj = "bufffield";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatgreen.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(10, 10, 0.1);
	self->think = buff_field_think;
	self->update = buff_field_update;
	self->free = buff_field_free;
	self->collide = buff_field_collide;
	self->attSpeed = 1;
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 15, 15, 15);

	self->bounds = hitbox;

	self->ttl = 0;

	return self;
}

void buff_field_think(Entity* self)
{
	if (!self)return;

	if (self->attSpeed < 1)
	{
		self->attSpeed += 0.2;
	}
}
void buff_field_update(Entity* self)
{
	if (!self)return;

	self->ttl += 1;

	if (self->ttl >= 300)
	{
		turret_free(self);
	}

	self->bounds.x = self->position.x - 7.5;
	self->bounds.y = self->position.y - 7.5;
	self->bounds.z = self->position.z;
}

void buff_field_free(Entity* self)
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

void buff_field_collide(Entity* self, Entity* collide)
{
	GFC_Vector3D* dir = gfc_vector3d_new();
	if (!self)return;
	//if (collide->obj == "player")
	//{
		//slog("fired");
		//slog("field collided with %s", collide->obj); 
	//}
}