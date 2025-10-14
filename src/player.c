#include "simple_logger.h"

#include "player.h"
#include "gfc_input.h"

void player_think(Entity* self);
void player_update(Entity* self);

GFC_Vector3D dir = { 0 }, velocity = { 0 };

Entity* player_init(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "notAgumon");
	self->mesh = gf3d_mesh_load("models/dino/dino.obj");
	self->texture = gf3d_texture_load("models/dino/dino.png");
	self->color = color;
	self->position = position;
	self->position.z = 0;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->think = player_think;
	self->update = player_update;
	return self;
}

void player_think(Entity* self)
{
	if (!self)return; 
	if (gfc_input_command_down("walkback"))
	{
		dir.y = 1;
		self->rotation.z = 3.14;
	}
	else if (gfc_input_command_down("walkforward"))
	{
		dir.y = -1;
		self->rotation.z = 0;
	}
	else
	{
		dir.y = 0;
	}

	if (gfc_input_command_down("walkleft"))
	{
		dir.x = 1;
		self->rotation.z = 3.14/2;
	}
	else if (gfc_input_command_down("walkright"))
	{
		dir.x = -1;
		self->rotation.z = -3.14/2;
	}
	else
	{
		dir.x = 0;
	}

	if (gfc_input_command_pressed("jump"))
	{
		dir.z = 3.0;
	}
	else if (self->position.z > 0)
	{
		dir.z -= 0.25;
	}
	else if (self->position.z <= 0)
	{
		dir.z = 0;
		self->position.z = 0;
	}

	gfc_vector3d_scale(velocity, dir, 1);
	gfc_vector3d_add(self->position, self->position, velocity);

}
void player_update(Entity* self)
{
	if (!self)return;
}