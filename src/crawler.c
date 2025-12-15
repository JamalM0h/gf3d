#include "simple_logger.h"

#include "monster.h"

void crawler_think(Entity* self);
void crawler_update(Entity* self);
void crawler_free(Entity* self);
void crawler_collide(Entity* self, Entity* collide);

Entity* crawlerenemy; 

Mesh* crawlermesh = NULL;

Entity* crawler_spawn(GFC_Vector3D position, GFC_Color color, Entity *player) 
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "crawler");
	self->obj = "monster";
	if(crawlermesh == NULL)crawlermesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->mesh = crawlermesh;
	self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(2, 2, 2);
	self->think = crawler_think; 
	self->update = crawler_update; 
	self->free = crawler_free; 
	self->collide = crawler_collide; 

	self->health = 5;

	GFC_Box hitbox = gfc_box(self->position.x - 2.5, self->position.y - 2.5, self->position.z - 2.5, 5, 5, 5);

	self->bounds = hitbox;

	crawlerenemy = player; 

	return self;
}

void crawler_think(Entity* self)
{
	GFC_Vector3D* dir = gfc_vector3d_new(); 
	float movemag = 0.5;
	if (!self)return;

	dir->x = (self->position.x - crawlerenemy->position.x) * -1; 
	dir->y = (self->position.y - crawlerenemy->position.y) * -1; 
	dir->z = 0; 
	gfc_vector3d_normalize(dir); 
	self->position.x += dir->x * movemag;
	self->position.y += dir->y * movemag; 
}
void crawler_update(Entity* self)
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
			item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -1, crawlerenemy);
			dir->x = gfc_random_int(100) - 50;
			dir->y = gfc_random_int(100) - 50;
			dir->z = 0;
			gfc_vector3d_normalize(dir);
			item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -2, crawlerenemy);
		}
		crawler_free(self); 
	} 
}

void crawler_free(Entity* self)
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

void crawler_collide(Entity* self, Entity* collide)
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