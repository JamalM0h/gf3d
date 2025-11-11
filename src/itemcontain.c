#include "simple_logger.h"

#include "interact.h"
#include "item.h"

void itemcon_think(Entity* self);
void itemcon_update(Entity* self);
void itemcon_free(Entity* self);
void itemcon_collide(Entity* self, Entity* collide);


Entity* item_container_spawn(GFC_Vector3D position, GFC_Color color, GFC_Vector3D playpos, int itemid)
{
	Entity* self;
	int i; 
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "itemcon");
	self->obj = "itemcon";
	self->mesh = gf3d_mesh_load("models/itemcontainer.obj");
	self->texture = gf3d_texture_load("models/primitives/flatgrey.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(3, 3, 3);
	self->think = itemcon_think;
	self->update = itemcon_update;
	self->free = itemcon_free;
	self->collide = itemcon_collide;
	self->_inuse = 1;

	for (i = 0; i < itemid; i++)
	{
		self->rotation.z += 1;
	}

	GFC_Box hitbox = gfc_box(self->position.x - 5, self->position.y - 5, self->position.z - 5, 10, 10, 10);

	self->bounds = hitbox;

	item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, gfc_vector3d(0,0,0), true, self->rotation.z);

	return self;
}

void itemcon_think(Entity* self)
{
	if (!self)return;
}
void itemcon_update(Entity* self)
{
	if (!self)return;

	self->bounds.x = self->position.x - 5;
	self->bounds.y = self->position.y - 5;
	self->bounds.z = self->position.z - 5;

}

void itemcon_free(Entity* self)
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

void itemcon_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	GFC_Vector3D* dir = gfc_vector3d_new();
	dir->x = (self->position.x - collide->position.x) * -1; 
	dir->y = (self->position.y - collide->position.y) * -1; 
	dir->z = 0;  
	gfc_vector3d_normalize(dir);
	item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir ,false, self->rotation.z); 
}