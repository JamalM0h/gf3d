#include "simple_logger.h"

#include "projectile.h"

void projectile_think(Entity* self);
void projectile_update(Entity* self);
void projectile_free(Entity* self);
void projectile_collide(Entity* self, Entity* collide);

Entity* create_projectile(GFC_Vector3D position, GFC_Vector3D dir, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "projectile");
	self->obj = "projectile";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj"); 
	self->texture = gf3d_texture_load("models/primitives/flatwhite.png"); 
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0); 
	self->scale = gfc_vector3d(0.5, 0.5, 0.5);
	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;
	self->collide = projectile_collide;
	GFC_Box hitbox = gfc_box(self->position.x,  self->position.y, self->position.z, 1, 1, 1);
	self->bounds = hitbox;

	self->dirtomove = dir;

	self->ttl = 0;

	return self;
}

void projectile_think(Entity* self)
{
	if (!self)return;
}
void projectile_update(Entity* self)
{
	if (!self)return;

	self->ttl += 1;
	
	if (self->ttl >= 35)
	{
		projectile_free(self);
	}

	self->bounds.x = self->position.x - 0.5;
	self->bounds.y = self->position.y - 0.5;
	self->bounds.z = self->position.z - 0.5;

	self->position.x += self->dirtomove.x * 4;
	self->position.y += self->dirtomove.y * 4;
	self->position.z += self->dirtomove.z;

}

void projectile_free(Entity* self)
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

void projectile_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	if (collide->obj == "monster")
	{
		collide->collide(collide, self); 
	}
	projectile_free(self);
} 