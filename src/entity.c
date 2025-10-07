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
	if (entity_system.entity_list)
	{
		for (i = 0; i < entity_system.entity_max; i++)
		{
			if (entity_system.entity_list[i]._inuse)
			{
				entity_system.entity_list[i]._inuse = 1;
				entity_system.entity_list[i].color = GFC_COLOR_WHITE;
				//entity_system.entity_list[i].
			}
		}
	}
}

void entity_system_close()
{
	int i;
	if (entity_system.entity_list)
	{
		for (i = 0; i < entity_system.entity_max; i++)
		{
			
		}
	}
}

void entity_draw(Entity* ent)
{
	if (!ent)return;

}

void entity_free(Entity* ent)
{
	if (!ent)return;
	gf3d_mesh_free(ent->mesh);
	gf3d_texture_free(ent->texture);
	memset(ent, 0, sizeof(Entity));
}

void entity_system_init(Uint32 max_ents)
{
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
}
