#include "simple_logger.h"

#include "interact.h"
#include "item.h"

void randomShrine_update(Entity* self);
void randomShrine_free(Entity* self);
void randomShrine_collide(Entity* self, Entity* collide);

int randomitemid = 0;

Entity* randomShrine_spawn(GFC_Vector3D position, GFC_Color color, GFC_Vector3D playpos)
{
	Entity* self;
	int i;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "randomShrine");
	self->obj = "randomShrine";
	self->mesh = gf3d_mesh_load("models/randomshrine.obj");
	self->texture = gf3d_texture_load("models/primitives/flatgrey.png");
	self->color = color;
	self->position = position;
	self->rotation = gfc_vector3d(0, GFC_PI, 0);
	self->scale = gfc_vector3d(-4, -4, -4);
	self->update = randomShrine_update; 
	self->free = randomShrine_free; 
	self->collide = randomShrine_collide; 
	self->_inuse = 1;

	GFC_Box hitbox = gfc_box(self->position.x - 5, self->position.y - 5, self->position.z - 5, 10, 10, 10);

	self->bounds = hitbox;

	return self;
}

void randomShrine_update(Entity* self)
{
	if (!self)return;

	self->bounds.x = self->position.x - 5;
	self->bounds.y = self->position.y - 5;
	self->bounds.z = self->position.z - 5;
}

void randomShrine_free(Entity* self)
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

void randomShrine_collide(Entity* self, Entity* collide) 
{
	if (!self)return;
	GFC_Vector3D* dir = gfc_vector3d_new();
	dir->x = (self->position.x - collide->position.x) * -1;
	dir->y = (self->position.y - collide->position.y) * -1;
	dir->z = 0;
	gfc_vector3d_normalize(dir);
	randomitemid = gfc_random_int(8); 
	item_spawn(gfc_vector3d(self->position.x, self->position.y, self->position.z + 1.25), GFC_COLOR_WHITE, *dir, false, randomitemid); 
}