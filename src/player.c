#include "simple_logger.h"

#include "player.h"
#include "gfc_input.h"
#include "gf3d_camera.h"
#include "projectile.h"
#include "monster.h"

void player_think(Entity* self);
void player_update(Entity* self);
void player_collide(Entity* self, Entity* collide);

GFC_Vector3D dir = { 0 }, velocity = { 0 }, up = { 0 }, angle = { 0 };
int jump = 0, class = 0;
float theta = 0, zax = 5, yax = 40;

Entity* player_init(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "player");
	self->obj = "player";
	self->mesh = gf3d_mesh_load("models/primitives/sphere.obj");
	self->texture = gf3d_texture_load("models/primitives/flatwhite.png");
	self->color = color;
	self->position = position;
	self->position.z = 0;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(3, 3, 3);
	self->think = player_think;
	self->update = player_update;
	self->collide = player_collide;
	GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 5, 5, 5); 
	self->bounds = hitbox; 
	return self;
}

void player_think(Entity* self)
{
	GFC_Vector3D* camforward = gfc_vector3d_new(), *camright = gfc_vector3d_new();
	SDL_Event event;
	int MouseRelX = 0, MouseRelY = 0;
	float Sens = 0.002;
	if (!self)return;
	camforward->x = self->position.x - self->camera->x;
	camforward->y = self->position.y - self->camera->y;
	gfc_vector3d_normalize(camforward);
	gfc_vector3d_cross_product(camright, gfc_vector3d(0, 0, 1), *camforward);
	gfc_vector3d_normalize(camright);
	if (dir.x < 4 && dir.x > -4 && dir.y < 4 && dir.y > -4)
	{
		if (gfc_input_command_held("walkback"))
		{
			dir.x += camforward->x * -1;
			dir.y += camforward->y * -1;
		}
		if (gfc_input_command_held("walkforward"))
		{
			dir.x += camforward->x;
			dir.y += camforward->y;
		}
		if (gfc_input_command_held("walkleft"))
		{
			dir.x += camright->x;
			dir.y += camright->y;
		}
		if (gfc_input_command_held("walkright"))
		{
			dir.x += camright->x * -1;
			dir.y += camright->y * -1;
		}
	}

	if (gfc_input_command_down("crouch"))
	{
		if (class == 0)class = 1;
		else if (class == 1) class = 0;
	}

	if (gfc_input_command_pressed("jump") && (jump == 1))
	{
		up.z = 2.5;
		jump = 0;
	}
	else if (self->position.z > 0)
	{
		up.z -= 0.20;
		dir.y = dir.y / 1.8;
		dir.x = dir.x / 1.8;
	}
	else if (self->position.z <= 0)
	{
		up.z = 0;
		self->position.z = 0;
		jump = 1;
		dir.y = dir.y / 2.0;
		dir.x = dir.x / 2.0;
	}

	if (gfc_input_command_pressed("yes"))
	{
		create_projectile(self->position, gfc_vector3d(camforward->x, camforward->y, 0), GFC_COLOR_WHITE);
	}

	if (gfc_input_command_down("movementab") && gfc_input_command_held("movementab"))
	{
		if (class == 0 && gfc_input_command_held("movementab") && self->position.z <= 0)
		{
			dir = gfc_vector3d_multiply(dir, gfc_vector3d(1.5, 1.5, 0));
		}
		else if (class == 1 && jump == 1 && gfc_input_command_down("movementab"))
		{
			dir.x *= 15;
			dir.y *= 15;
			up.z = 3.5;
			jump = 0;
		}
	}

	gfc_vector3d_scale(velocity, dir, 0.5);
 
	SDL_GetRelativeMouseState(&MouseRelX, &MouseRelY);

	//if (gfc_input_command_held("pandown"))
	if (MouseRelY > 0)
	{
		if (zax < 15)
		{
			zax += 0.05 * MouseRelY;   
		//	if (zax < 5)
		//	{
		//		yax += 2;  
		//	}
		//	else if (zax > 5)
		//	{
		//		yax -= 2; 
		//	}
		//	else
		//	{
		//		yax = 40;
		//	}
		}

		else
		{
			zax = 15;
			//yax = 20;
		}
	}
	//else if (gfc_input_command_held("panup"))
	else if (MouseRelY < 0)
	{
		if (zax > -5)
		{
			zax += 0.05 * MouseRelY; 
		//	if (zax > 5)
		//	{
		//		yax += 2; 
		//	}
		//	else if (zax < 5)
		//	{
		//		yax -= 2; 
		//	}
		//	else
		//	{
		//		yax = 40;
		//	}
		}

		else
		{
			zax = -5;
			//yax = 20;
		}
	}

	if (MouseRelX != 0)
	{
		theta -= Sens * MouseRelX;   
		self->rotation.z -= Sens * MouseRelX;   
	}
}
void player_update(Entity* self)
{
	GFC_Vector2D newvec = { 0 };
	if (!self)return;

	gfc_vector3d_add(self->position, self->position, velocity);
	gfc_vector3d_add(self->position, self->position, up);

	*self->camera = self->position;
	self->camera->z += zax;
	self->camera->y += 30; 

	newvec = gfc_vector2d_rotate_around_center(gfc_vector2d(self->camera->x, self->camera->y), theta, gfc_vector2d(self->position.x, self->position.y));

	self->camera->x = newvec.x;
	self->camera->y = newvec.y;

	gf3d_camera_look_at(gfc_vector3d(self->position.x, self->position.y, self->position.z + 5), self->camera); 

	self->bounds.x = self->position.x;
	self->bounds.y = self->position.y;
	self->bounds.z = self->position.z;
}

void player_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	slog("player collided with %s", collide->obj); 
}