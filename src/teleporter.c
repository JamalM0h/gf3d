#include "simple_logger.h"

#include "interact.h"
#include "monster.h"

void teleporter_update(Entity* self);
void teleporter_free(Entity* self);
void teleporter_collide(Entity* self, Entity* collide);

Entity* teleporter_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	int i;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "teleporter");
	self->obj = "teleporter";
	self->mesh = gf3d_mesh_load("models/teleporter.obj");
	self->texture = gf3d_texture_load("models/primitives/flatgrey.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(20, 20, 20);
	self->update = teleporter_update; 
	self->free = teleporter_free; 
	self->collide = teleporter_collide; 
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 5, self->position.y - 5, self->position.z - 5, 10, 10, 10);

	self->bounds = hitbox;

	return self;
}

void teleporter_update(Entity* self)
{
	if (!self)return;

	self->bounds.x = self->position.x - 5;
	self->bounds.y = self->position.y - 5;
	self->bounds.z = self->position.z - 5;
}

void teleporter_free(Entity* self)
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

void teleporter_collide(Entity* self, Entity* collide)
{
	int i;
	if (!self)return;
	if (collide->obj == "player" && self->obj == "teleporter") {
		slog("teleporter activated");
		self->obj = "teleporteractivated";
		for (i = 0; i < 8; i++)
		{
			int monsterid = gfc_random_int(5);
			GFC_Vector3D monspos = gfc_vector3d(gfc_random_int(300) - 150, gfc_random_int(300) - 150, 0); 
			if (monsterid == 0) 
			{
				crawler_spawn(monspos, GFC_COLOR_WHITE, collide); 
			}
			else if (monsterid == 1) 
			{
				ranger_spawn(monspos, GFC_COLOR_WHITE, collide); 
			}
			else if (monsterid == 2) 
			{
				flyer_spawn(monspos, GFC_COLOR_WHITE, collide);
			}
			else if (monsterid == 3) 
			{
				crusher_spawn(monspos, GFC_COLOR_WHITE, collide); 
			}
			else if (monsterid == 4) 
			{
				rocketbot_spawn(monspos, GFC_COLOR_WHITE, collide); 
			}
		}
		return;
	}
	if ((collide->obj == "player") && (self->obj == "teleporteractivated") && (enemycount() == 0))
	{
		clearstage();
		collide->position = gfc_vector3d(8,0,0);
		self->obj = "teleporterdone";
	}
}