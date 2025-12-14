#include "simple_logger.h"

#include "projectile.h"

void projectile_think(Entity* self);
void projectile_update(Entity* self);
void projectile_free(Entity* self);
void projectile_collide(Entity* self, Entity* collide);
void enemyprojectile_collide(Entity* self, Entity* collide); 

void explosion_think(Entity* self);
void explosion_update(Entity* self);

Entity* create_projectile(GFC_Vector3D position, GFC_Vector3D dir, GFC_Color color, float damageMod)
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

	self->damageMod = damageMod; 

	self->dirtomove = dir;

	self->ttl = 0;

	return self;
}

Entity* create_rocket(GFC_Vector3D position, GFC_Vector3D dir, GFC_Color color, Bool arc, float damageMod) 
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "rocket");
	self->obj = "rocket";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position; 
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(0.75, 0.75, 0.75);
	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;
	self->collide = projectile_collide;
	GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 1.5, 1.5, 1.5);
	self->bounds = hitbox;

	self->damageMod = damageMod;

	self->dirtomove = dir;

	self->isArc = arc; 

	if(self->isArc == true)self->dirtomove.z = 3; 

	self->ttl = 0;

	return self;
}

Entity* create_enemy_rocket(GFC_Vector3D position, GFC_Vector3D dir, GFC_Color color, Bool arc, float damageMod)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "enemyrocket");
	self->obj = "enemyrocket";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(0.75, 0.75, 0.75);
	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;
	self->collide = projectile_collide;
	GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 1.5, 1.5, 1.5);
	self->bounds = hitbox;

	self->damageMod = damageMod;

	self->dirtomove = dir;

	self->isArc = arc;

	if (self->isArc == true)self->dirtomove.z = 3;

	self->ttl = 0;

	return self;
}

Entity* create_explosion(GFC_Vector3D position, GFC_Color color, Bool friendly)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "explo");
	if (friendly == true)self->obj = "explo";
	else self->obj = "enemyexplo";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatorange.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(0.1, 0.1, 0.1);
	self->think = explosion_think;
	self->update = explosion_update;
	self->free = projectile_free;

	self->ttl = 0;

	return self;
}

create_enemy_projectile(GFC_Vector3D position, GFC_Vector3D dir, GFC_Color color, float damageMod)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "enemyprojectile");
	self->obj = "enemyprojectile";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatwhite.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(0.5, 0.5, 0.5);
	self->think = projectile_think;
	self->update = projectile_update;
	self->free = projectile_free;
	self->collide = enemyprojectile_collide; 
	GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 1, 1, 1);
	self->bounds = hitbox;

	self->damageMod = damageMod;

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

	if ((self->obj == "projectile"))
	{
		self->bounds.x = self->position.x - 0.5;
		self->bounds.y = self->position.y - 0.5;
		self->bounds.z = self->position.z - 0.5;

		self->position.x += self->dirtomove.x * 5;
		self->position.y += self->dirtomove.y * 5;
		self->position.z += self->dirtomove.z;
	}

	if ((self->obj == "enemyprojectile"))
	{
		self->bounds.x = self->position.x - 0.5;
		self->bounds.y = self->position.y - 0.5;
		self->bounds.z = self->position.z - 0.5;

		self->position.x += self->dirtomove.x * 2;
		self->position.y += self->dirtomove.y * 2;
		self->position.z += self->dirtomove.z * 2;
	}

	if (self->obj == "rocket")
	{
		self->bounds.x = self->position.x - 0.75;
		self->bounds.y = self->position.y - 0.75;
		self->bounds.z = self->position.z - 0.75; 

		self->position.x += self->dirtomove.x * 4;
		self->position.y += self->dirtomove.y * 4;
		self->position.z += self->dirtomove.z;
		if(self->isArc == true)self->dirtomove.z -= 0.50;
		if ((self->position.z <= 0)  && (self->isArc == true))
		{
			self->position.z = 0;

			create_explosion(self->position, GFC_COLOR_WHITE, true);

			projectile_free(self);
		}
	}

	if (self->obj == "enemyrocket")
	{
		self->bounds.x = self->position.x - 0.75;
		self->bounds.y = self->position.y - 0.75;
		self->bounds.z = self->position.z - 0.75;

		self->position.x += self->dirtomove.x * 4;
		self->position.y += self->dirtomove.y * 4;
		self->position.z += self->dirtomove.z;
		if (self->isArc == true)self->dirtomove.z -= 0.50;
		if ((self->position.z <= 0) && (self->isArc == true))
		{
			self->position.z = 0;

			create_explosion(self->position, GFC_COLOR_WHITE, false);

			projectile_free(self);
		}
	}

	if (self->position.z <= -3)
	{
		projectile_free(self);
	}
}

void projectile_free(Entity* self)
{
	if (!self)return;
	if(self->obj == "rocket")create_explosion(self->position, GFC_COLOR_WHITE, true);
	if (self->obj == "enemyrocket")create_explosion(self->position, GFC_COLOR_WHITE, false); 
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
	if (collide->obj == "monster" || collide->obj == "crawler")
	{
		collide->collide(collide, self); 
	}
	projectile_free(self);
} 

void enemyprojectile_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	if (collide->obj == "player")
	{
		collide->collide(collide, self);
	}
	projectile_free(self); 
}

void explosion_think(Entity* self)
{
	if (!self)return;
}

void explosion_update(Entity* self)
{
	if (!self)return;

	self->scale.x += 0.25;
	self->scale.y += 0.25;
	self->scale.z += 0.25;

	self->ttl += 1;

	if (self->ttl >= 40)
	{
		projectile_free(self);
	}
}