#include "simple_logger.h"

#include "item.h"

void item_update(Entity* self);
void item_free(Entity* self);
void item_collide(Entity* self, Entity* collide);

Entity* item_spawn(GFC_Vector3D position, GFC_Color color, GFC_Vector3D dir, Bool isDisplay, int itemid)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "item");
	if(itemid == 0)self->obj = "syringe";
	if (itemid == 0)self->mesh = gf3d_mesh_load("models/Syringe.obj");
	if (itemid == 0)self->texture = gf3d_texture_load("models/primitives/flatyellow.png");
	if (itemid == 1)self->obj = "boots";
	if (itemid == 1)self->mesh = gf3d_mesh_load("models/Boots.obj");
	if (itemid == 1)self->texture = gf3d_texture_load("models/primitives/flatgreen.png");
	if (itemid == 2)self->obj = "movCd";
	if (itemid == 2)self->mesh = gf3d_mesh_load("models/Clock.obj");
	if (itemid == 2)self->texture = gf3d_texture_load("models/primitives/flatturq.png");
	if (itemid == 3)self->obj = "specCd";
	if (itemid == 3)self->mesh = gf3d_mesh_load("models/Clock.obj");
	if (itemid == 3)self->texture = gf3d_texture_load("models/primitives/flatpurple.png");
	if (itemid == 4)self->obj = "healthUp";
	if (itemid == 4)self->mesh = gf3d_mesh_load("models/Healthup.obj");
	if (itemid == 4)self->texture = gf3d_texture_load("models/primitives/flatpink.png");
	if (itemid == 5)self->obj = "jumpUp";
	if (itemid == 5)self->mesh = gf3d_mesh_load("models/spring.obj");
	if (itemid == 5)self->texture = gf3d_texture_load("models/primitives/flatorange.png");
	if (itemid == 6)self->obj = "armor";
	if (itemid == 6)self->mesh = gf3d_mesh_load("models/Armor.obj");
	if (itemid == 6)self->texture = gf3d_texture_load("models/primitives/flatbrown.png");
	if (itemid == 7)self->obj = "damage";
	if (itemid == 7)self->mesh = gf3d_mesh_load("models/sword.obj");
	if (itemid == 7)self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position;
	if(itemid == 0)self->rotation = gfc_vector3d(GFC_PI_HALFPI / 2, 0, 0); 
	self->scale = gfc_vector3d(2, 2, 2);
	self->free = item_free; 
	self->update = item_update;
	self->_inuse = 1;

	if (isDisplay == false) {

		self->collide = item_collide; 
		GFC_Box hitbox = gfc_box(self->position.x - 1, self->position.y - 1, self->position.z - 1, 2, 2, 2);
		self->bounds = hitbox;
		self->dirtomove = dir; 
		self->dirtomove.z = 3;
	}
	else self->obj = NULL;

	return self;
}

void item_update(Entity* self)
{
	if (!self)return;

	self->rotation.z += 0.05;

	self->bounds.x = self->position.x - 1;
	self->bounds.y = self->position.y - 1;
	self->bounds.z = self->position.z - 1;

	if (self->position.z != 0 && (!gfc_vector3d_is_zero(self->dirtomove)))  
	{
		self->position.x += self->dirtomove.x;
		self->position.y += self->dirtomove.y;
		self->position.z += self->dirtomove.z;
		self->dirtomove.z -= 0.30; 
	}

	if (self->position.z <= 0)self->position.z = 0;
}

void item_free(Entity* self)
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

void item_collide(Entity* self, Entity* collide)
{
	if (!self)return;
}