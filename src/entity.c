#include "simple_logger.h"

#include "entity.h"

typedef struct
{
	Entity* entity_list;
	Uint32 entity_max;
}EntitySystem;

static EntitySystem entity_system = { 0 };

Entity* entity_new()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)continue;
		memset(&entity_system.entity_list[i], 0, sizeof(Entity));
		entity_system.entity_list[i]._inuse = 1;
		entity_system.entity_list[i].color = GFC_COLOR_WHITE;
		entity_system.entity_list[i].scale = gfc_vector3d(1,1,1);
		return &entity_system.entity_list[i];
	}

	slog("failed to allocate new entity: list full");
	return NULL;
}


void entity_system_close()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_free(&entity_system.entity_list[i]);
	}

	free(entity_system.entity_list);
	memset(&entity_system, 0, sizeof(EntitySystem));
}

void entity_system_init(Uint32 max_ents)
{
	if (entity_system.entity_list)
	{
		slog("cannot have more then one entity manager");
		return;
	}
	if (!max_ents)
	{
		slog("cannot init entity system with zero ents");
		return;
	}
	entity_system.entity_list = gfc_allocate_array(sizeof(Entity), max_ents);
	if (!entity_system.entity_list)
	{
		slog("failed to allocate ", max_ents);
	}
	entity_system.entity_max = max_ents;
	slog("ent system initialized");
	atexit(entity_system_close); 
}

void entity_draw(Entity* ent, GFC_Vector3D lightPos, GFC_Color lightColor)
{
	GFC_Matrix4 modelMat;
	if (!ent)return;
	gfc_matrix4_from_vectors(
		modelMat,
		ent->position,
		ent->rotation,
		ent->scale);
	gf3d_mesh_draw(
		ent->mesh,
		modelMat,
		ent->color,
		ent->texture,
		lightPos,
		lightColor);
}

void entity_draw_all(GFC_Vector3D lightPos, GFC_Color lightColor)
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (entity_system.entity_list[i]._inuse)
		{
			entity_draw(&entity_system.entity_list[i], lightPos, lightColor);
		}
	}
}

void entity_think(Entity *self)
{
	if (!self)return;
	if (self->think)self->think(self);
}

void entity_think_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_think(&entity_system.entity_list[i]);
	}
}

void entity_update(Entity* self)
{
	if (!self)return;
	if (self->update)self->update(self);
}

void entity_update_all()
{
	int i;
	for (i = 0; i < entity_system.entity_max; i++)
	{
		if (!entity_system.entity_list[i]._inuse)continue;
		entity_update(&entity_system.entity_list[i]); 
	}
}

void entity_free(Entity* ent)
{
	if (!ent)return;
	gf3d_mesh_free(ent->mesh);
	gf3d_texture_free(ent->texture);
	memset(ent, 0, sizeof(Entity));
}
