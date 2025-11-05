#include "simple_logger.h"

#include "monster.h"

void monster_think(Entity* self);
void monster_update(Entity* self);
void monster_free(Entity* self);
void monster_collide(Entity* self, Entity* collide);

Entity* monster_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new(); 
	if (!self)return;
	gfc_line_cpy(self->name, "monster");
	self->obj = "monster";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatred.png"); 
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(3, 3, 3);
	self->think = monster_think;
	self->update = monster_update;
	self->free = monster_free;
	self->collide = monster_collide;
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 1.5, self->position.y - 1.5, self->position.z - 1.5, 4, 4, 4); 

	self->bounds = hitbox;

	return self;
}

void monster_think(Entity* self)
{
	if (!self)return;
}
void monster_update(Entity* self)
{
	if (!self)return; 

	self->bounds.x = self->position.x - 1.5;
	self->bounds.y = self->position.y - 1.5;
	self->bounds.z = self->position.z - 1.5;
}

void monster_free(Entity* self)
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

void monster_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	if (collide->obj == "projectile")
	{
		self->position.x += collide->dirtomove.x * 5;
		self->position.y += collide->dirtomove.y * 5;
	}
	if (collide->obj == "player")
	{
		GFC_Vector2D *monstermovedir = gfc_vector2d_new(); 
		*monstermovedir = gfc_vector2d(self->position.x - collide->position.x, self->position.y - collide->position.y);  
		gfc_vector2d_normalize(monstermovedir);
		self->position.x += monstermovedir->x * 5; 
		self->position.y += monstermovedir->y * 5; 
	}
}