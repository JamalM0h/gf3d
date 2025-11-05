#include "simple_logger.h"

#include "deploy.h"
#include "projectile.h"

void turret_think(Entity* self);
void turret_update(Entity* self);
void turret_free(Entity* self);
void turret_collide(Entity* self, Entity* collide);

Entity* turret_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "turret");
	self->obj = "turret";
	self->mesh = gf3d_mesh_load("models/turret.obj");
	self->texture = gf3d_texture_load("models/primitives/flatblue.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(1, 1, 1);
	self->think = turret_think;
	self->update = turret_update;
	self->free = turret_free;
	self->collide = turret_collide;
	self->attSpeed = 1;
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 50, self->position.y - 50, self->position.z - 50, 100, 100, 100);

	self->bounds = hitbox;

	self->ttl = 0;

	return self;
}

void turret_think(Entity* self)
{
	if (!self)return;

	if (self->attSpeed < 1)
	{
		self->attSpeed += 0.2;
	}
}
void turret_update(Entity* self)
{
	if (!self)return;

	self->ttl += 1;

	if (self->ttl >= 800)
	{
		turret_free(self); 
	}

	self->bounds.x = self->position.x - 50;
	self->bounds.y = self->position.y - 50;
	self->bounds.z = self->position.z - 50;

	self->rotation.z += 0.03;
}

void turret_free(Entity* self)
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

void turret_collide(Entity* self, Entity* collide)
{
	GFC_Vector3D *dir = gfc_vector3d_new(); 
	if (!self)return;
	if (collide->obj == "monster" && self->attSpeed >= 1)
	{
		self->attSpeed = 0; 
		dir->x = (self->position.x - collide->position.x) * -1;
		dir->y = (self->position.y - collide->position.y) * -1;
		dir->z = 0;
		gfc_vector3d_normalize(dir);
		create_projectile(self->position, *dir , GFC_COLOR_WHITE);    
		//slog("fired");
		//slog("turret collided with %s", collide->obj); 
	}
}