#include "simple_logger.h"

#include "interact.h"
#include "item.h"

void healthShrine_update(Entity* self);
void healthShrine_free(Entity* self);
void healthShrine_collide(Entity* self, Entity* collide);

Entity* healthShrine_spawn(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	int i;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "healthShrine");
	self->obj = "healthShrine";
	self->mesh = gf3d_mesh_load("models/healthshrine.obj");
	self->texture = gf3d_texture_load("models/primitives/flatgrey.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, GFC_PI, 0);
	self->scale = gfc_vector3d(-4, -4, -4);
	self->update = healthShrine_update; 
	self->free = healthShrine_free; 
	self->collide = healthShrine_collide; 
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 5, self->position.y - 5, self->position.z - 5, 10, 10, 10);

	self->bounds = hitbox;

	return self;
}

void healthShrine_update(Entity* self)
{
	if (!self)return;

	self->bounds.x = self->position.x - 5;
	self->bounds.y = self->position.y - 5;
	self->bounds.z = self->position.z - 5;
}

void healthShrine_free(Entity* self)
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

void healthShrine_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	health_field_spawn(self->position, GFC_COLOR_WHITE); 
	
}