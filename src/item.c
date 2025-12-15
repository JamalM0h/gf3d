#include "simple_logger.h"

#include "item.h"

void item_update(Entity* self);
void item_free(Entity* self);
void item_collide(Entity* self, Entity* collide);

Entity* attracttoplayer;

Mesh *spheremesh = NULL, *coinmesh = NULL, *syringemesh = NULL, *bootsmesh = NULL, *clockmesh = NULL, *heartmesh = NULL, *springmesh = NULL, *armormesh = NULL, *swordmesh = NULL;

Entity* item_spawn(GFC_Vector3D position, GFC_Color color, GFC_Vector3D dir, Bool isDisplay, int itemid, Entity *attractto)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "item");

	if (itemid == -2)self->obj = "exp";
	if (itemid == -2 && spheremesh == NULL)spheremesh = gf3d_mesh_load("models/primitives/sphere.obj");
	if (itemid == -2)self->mesh = spheremesh; 
	if (itemid == -2)self->texture = gf3d_texture_load("models/primitives/flatlightblue.png");

	if (itemid == -1)self->obj = "gold";
	if (itemid == -1 && coinmesh == NULL) coinmesh = gf3d_mesh_load("models/coin.obj");
	if (itemid == -1)self->mesh = coinmesh;
	if (itemid == -1)self->texture = gf3d_texture_load("models/primitives/flatyellow.png");

	if (itemid == 0)self->obj = "syringe";
	if (itemid == 0 && syringemesh == NULL)syringemesh = gf3d_mesh_load("models/Syringe.obj");
	if (itemid == 0)self->mesh = syringemesh;
	if (itemid == 0)self->texture = gf3d_texture_load("models/primitives/flatyellow.png");

	if (itemid == 1)self->obj = "boots";
	if (itemid == 1 && bootsmesh == NULL)bootsmesh = gf3d_mesh_load("models/Boots.obj");
	if (itemid == 1)self->mesh = bootsmesh;
	if (itemid == 1)self->texture = gf3d_texture_load("models/primitives/flatgreen.png");

	if (itemid == 2)self->obj = "movCd";
	if (itemid == 2 && clockmesh == NULL)clockmesh = gf3d_mesh_load("models/Clock.obj"); 
	if (itemid == 2)self->mesh = clockmesh;
	if (itemid == 2)self->texture = gf3d_texture_load("models/primitives/flatturq.png");

	if (itemid == 3)self->obj = "specCd";
	if (itemid == 3 && clockmesh == NULL)clockmesh = gf3d_mesh_load("models/Clock.obj"); 
	if (itemid == 3)self->mesh = clockmesh;
	if (itemid == 3)self->texture = gf3d_texture_load("models/primitives/flatpurple.png");

	if (itemid == 4)self->obj = "healthUp";
	if (itemid == 4 && heartmesh == NULL)heartmesh = gf3d_mesh_load("models/Healthup.obj"); 
	if (itemid == 4)self->mesh = heartmesh;
	if (itemid == 4)self->texture = gf3d_texture_load("models/primitives/flatpink.png");

	if (itemid == 5)self->obj = "jumpUp";
	if (itemid == 5 && springmesh == NULL)springmesh = gf3d_mesh_load("models/spring.obj");
	if (itemid == 5)self->mesh = springmesh;
	if (itemid == 5)self->texture = gf3d_texture_load("models/primitives/flatorange.png");

	if (itemid == 6)self->obj = "armor";
	if (itemid == 6 && armormesh == NULL)armormesh = gf3d_mesh_load("models/Armor.obj");
	if (itemid == 6)self->mesh = armormesh;
	if (itemid == 6)self->texture = gf3d_texture_load("models/primitives/flatbrown.png");

	if (itemid == 7)self->obj = "damage";
	if (itemid == 7 && swordmesh == NULL)swordmesh = gf3d_mesh_load("models/sword.obj"); 
	if (itemid == 7)self->mesh = swordmesh;
	if (itemid == 7)self->texture = gf3d_texture_load("models/primitives/flatred.png");

	self->color = color;
	self->position = position;
	if(itemid == 0)self->rotation = gfc_vector3d(GFC_PI_HALFPI / 2, 0, 0); 
	self->scale = gfc_vector3d(2, 2, 2);
	if (itemid == -2)self->scale = gfc_vector3d(0.5,0.5,0.5);
	if (itemid == -1)self->scale = gfc_vector3d(2.5, 2.5, 2.5);
	self->free = item_free; 
	self->update = item_update;
	self->_inuse = 1;

	self->itemidnum = itemid;

	if (itemid == -1 || itemid == -2)attracttoplayer = attractto;

	if (isDisplay == false) {

		self->collide = item_collide; 
		GFC_Box hitbox = gfc_box(self->position.x - 1, self->position.y - 1, self->position.z - 1, 2, 2, 2);
		self->bounds = hitbox;
		self->dirtomove = dir; 
		self->dirtomove.z = 3;
	}

	else self->obj = "isDisplay";

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

	if ((self->itemidnum == -1 || self->itemidnum == -2) && self->position.z <= 0)
	{
		GFC_Vector3D* dir = gfc_vector3d_new();
		float movemag = 0.8;

		dir->x = (self->position.x - attracttoplayer->position.x) * -1;
		dir->y = (self->position.y - attracttoplayer->position.y) * -1;
		dir->z = 0;
		gfc_vector3d_normalize(dir);
		self->position.x += dir->x * movemag;
		self->position.y += dir->y * movemag;
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