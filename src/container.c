#include "simple_logger.h"

#include "interact.h"
#include "item.h"

void container_update(Entity* self);
void container_free(Entity* self);
void container_collide(Entity* self, Entity* collide);

Entity* container_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	int i;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "container");
	self->obj = "container";
	self->mesh = gf3d_mesh_load("models/container.obj");
	self->texture = gf3d_texture_load("models/primitives/flatblue.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, GFC_PI, 0);
	self->scale = gfc_vector3d(3, 3, 3);
	self->update = container_update;
	self->free = container_free;
	self->collide = container_collide;
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 5, self->position.y - 5, self->position.z - 5, 10, 10, 10);

	self->bounds = hitbox;

	return self;
}

void container_update(Entity* self)
{
	if (!self)return;

	self->bounds.x = self->position.x - 5;
	self->bounds.y = self->position.y - 5;
	self->bounds.z = self->position.z - 5;
}

void container_free(Entity* self)
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

void container_collide(Entity* self, Entity* collide)
{
	int i;
	if (!self)return;
	GFC_Vector3D* dir = gfc_vector3d_new();
	for (i = 0; i < 5; i++)
	{
		dir->x = gfc_random_int(100) - 50;
		dir->y = gfc_random_int(100) - 50;
		dir->z = 0;
		gfc_vector3d_normalize(dir);
		item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -1, collide);
		dir->x = gfc_random_int(100) - 50;
		dir->y = gfc_random_int(100) - 50;
		dir->z = 0;
		gfc_vector3d_normalize(dir);
		item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -2, collide);
	}

	self->obj = "usedcontainer";   
	self->mesh = gf3d_mesh_load("models/opencontainer.obj"); 
}