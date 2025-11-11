#include "deploy.h"
#include "projectile.h"

void rocket_beacon_think(Entity* self); 
void rocket_beacon_update(Entity* self);
void rocket_beacon_free(Entity* self);

int rocket_rand_x = 0, rocket_rand_y = 0;

Entity* rocket_beacon_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "rocket_beacon");
	self->obj = "rocket_beacon";
	self->mesh = gf3d_mesh_load("models/Rocket_beacon.obj");
	self->texture = gf3d_texture_load("models/primitives/flatred.png");
	self->color = color;
	self->position = position;
	self->position.z = -3;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(25, 25, 25);
	self->think = rocket_beacon_think; 
	self->update = rocket_beacon_update; 
	self->free = rocket_beacon_free;  
	self->_inuse = 1;

	//GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 15, 15, 15);

	//self->bounds = hitbox;

	self->ttl = 0;

	return self;
}

void rocket_beacon_think(Entity* self)
{
	if (!self)return;

	if (self->attSpeed < 1)
	{
		self->attSpeed += 0.1;
	}
}
void rocket_beacon_update(Entity* self)
{
	if (!self)return;

	self->ttl += 1;

	if (self->ttl >= 300)
	{
		rocket_beacon_free(self);
	}

	if (self->attSpeed >= 1)
	{
		rocket_rand_x = gfc_random_int(25);  
		rocket_rand_y = gfc_random_int(25); 
		create_rocket(gfc_vector3d(self->position.x - 12.5 + rocket_rand_x, self->position.y - 12.5 + rocket_rand_y, 100), gfc_vector3d(0, 0, 0), GFC_COLOR_WHITE, true, self->damageMod);
		self->attSpeed = 0;
	}

}

void rocket_beacon_free(Entity* self)
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