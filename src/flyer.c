#include "simple_logger.h"

#include "monster.h"
#include "projectile.h"

void flyer_think(Entity* self);
void flyer_update(Entity* self);
void flyer_free(Entity* self);
void flyer_collide(Entity* self, Entity* collide);

Entity* flyerenemy; 

Mesh* flyermesh = NULL; 

Entity* flyer_spawn(GFC_Vector3D position, GFC_Color color, Entity *player)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "flyer");
	self->obj = "monster";
	if(flyermesh == NULL)flyermesh = gf3d_mesh_load("models/primitives/icylinder.obj"); 
	self->mesh = flyermesh; 
	self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(-2, -2, -2);
	self->think = flyer_think; 
	self->update = flyer_update; 
	self->free = flyer_free; 
	self->collide = flyer_collide; 

	self->attSpeed = 1;

	self->health = 5;

	GFC_Box hitbox = gfc_box(self->position.x - 2.5, self->position.y - 2.5, self->position.z - 2.5, 5, 5, 5);

	self->bounds = hitbox;

	self->position.z = 10;

	flyerenemy = player; 

	return self;
}

void flyer_think(Entity* self)
{
	GFC_Vector3D* dir = gfc_vector3d_new(); 
	float movemag = 0.35;
	if (!self)return;

	dir->x = (self->position.x - flyerenemy->position.x) * -1; 
	dir->y = (self->position.y - flyerenemy->position.y) * -1; 
	dir->z = 0; 

	if (self->attSpeed < 1)
	{
		self->attSpeed += 0.025;
	}

	if (gfc_vector3d_magnitude(*dir) < 15 && self->attSpeed >= 1) 
	{
		self->attSpeed = 0; 
		create_enemy_rocket(self->position, gfc_vector3d(0, 0, -1), GFC_COLOR_WHITE, false, 1); 
	}

	gfc_vector3d_normalize(dir); 
	self->position.x += dir->x * movemag;
	self->position.y += dir->y * movemag; 

	self->rotation.z += 0.05;
}
void flyer_update(Entity* self)
{
	int i; 
	if (!self)return;

	self->bounds.x = self->position.x - 2.5;
	self->bounds.y = self->position.y - 2.5;
	self->bounds.z = self->position.z - 2.5;

	if (self->health <= 0)
	{
		GFC_Vector3D* dir = gfc_vector3d_new();
		for (i = 0; i < 2; i++)
		{
			dir->x = gfc_random_int(100) - 50;
			dir->y = gfc_random_int(100) - 50;
			dir->z = 0;
			gfc_vector3d_normalize(dir);
			item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -1, flyerenemy);
			dir->x = gfc_random_int(100) - 50;
			dir->y = gfc_random_int(100) - 50;
			dir->z = 0;
			gfc_vector3d_normalize(dir);
			item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -2, flyerenemy);
		}
		flyer_free(self);  
	} 
}

void flyer_free(Entity* self)
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

void flyer_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	if ((collide->obj == "projectile") || (collide->obj == "rocket"))
	{
		self->position.x += (collide->dirtomove.x * 4); 
		self->position.y += (collide->dirtomove.y * 4); 
		if (collide->obj == "rocket") { 
			self->position.x += (collide->dirtomove.x * 4);  
			self->position.y += (collide->dirtomove.y * 4);  
			self->health -= 1;
		}
		self->health -= 1;
	}
	if (collide->obj == "player")
	{
		GFC_Vector2D* monstermovedir = gfc_vector2d_new();
		*monstermovedir = gfc_vector2d(self->position.x - collide->position.x, self->position.y - collide->position.y);
		gfc_vector2d_normalize(monstermovedir);
		self->position.x += (monstermovedir->x * 5.0f);
		self->position.y += (monstermovedir->y * 5.0f);

		self->health -= 1;
	}

	if (collide->obj == "explo")
	{
		self->health -= 1;
	}
}