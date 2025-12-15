#include "simple_logger.h"

#include "monster.h"
#include "projectile.h"

void ranger_think(Entity* self); 
void ranger_update(Entity* self);
void ranger_free(Entity* self);
void ranger_collide(Entity* self, Entity* collide);

Entity* rangerenemy; 

Mesh* rangermesh = NULL;

Entity* ranger_spawn(GFC_Vector3D position, GFC_Color color, Entity *player) 
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "ranger");
	self->obj = "monster";
	if(rangerenemy == NULL)rangermesh = gf3d_mesh_load("models/ranger.obj");
	self->mesh = rangermesh;
	self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(3, 3, 3);
	self->think = ranger_think;  
	self->update = ranger_update;  
	self->free = ranger_free;   
	self->collide = ranger_collide;  

	self->attSpeed = 1; 

	self->health = 5;

	GFC_Box hitbox = gfc_box(self->position.x - 2.5, self->position.y - 2.5, self->position.z - 2.5, 5, 5, 5);

	self->bounds = hitbox;

	rangerenemy = player;  

	return self;
}

void ranger_think(Entity* self)
{
	GFC_Vector3D* dir = gfc_vector3d_new(); 
	float movemag = 0.5;
	if (!self)return;

	dir->x = (self->position.x - rangerenemy->position.x) * -1; 
	dir->y = (self->position.y - rangerenemy->position.y) * -1; 
	dir->z = 0; 
	if (gfc_vector3d_magnitude(*dir) > 50)
	{
		gfc_vector3d_normalize(dir);
		self->position.x += dir->x * movemag;
		self->position.y += dir->y * movemag;
	}

	if (self->attSpeed < 1)
	{
		self->attSpeed += 0.05; 
	} 
	if (self->attSpeed >= 1)
	{
		self->attSpeed = 0;
		dir->x = (self->position.x - rangerenemy->position.x) * -1; 
		dir->y = (self->position.y - rangerenemy->position.y) * -1;  
		dir->z = (self->position.z - rangerenemy->position.z) * -1;
		gfc_vector3d_normalize(dir); 
		create_enemy_projectile(self->position, *dir, GFC_COLOR_WHITE, self->damageMod);  
	}
}
void ranger_update(Entity* self)
{
	int i; 
	if (!self)return;

	self->bounds.x = self->position.x - 2.5;
	self->bounds.y = self->position.y - 2.5;
	self->bounds.z = self->position.z - 2.5;

	if (self->health <= 0)
	{
		GFC_Vector3D* dir = gfc_vector3d_new();
		for (i = 0; i < 3; i++)
		{
			dir->x = gfc_random_int(100) - 50;
			dir->y = gfc_random_int(100) - 50;
			dir->z = 0;
			gfc_vector3d_normalize(dir);
			item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -1, rangerenemy); 
			dir->x = gfc_random_int(100) - 50;
			dir->y = gfc_random_int(100) - 50;
			dir->z = 0;
			gfc_vector3d_normalize(dir);
			item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, -2, rangerenemy); 
		}
		crawler_free(self); 
	} 
}

void ranger_free(Entity* self)
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

void ranger_collide(Entity* self, Entity* collide) 
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