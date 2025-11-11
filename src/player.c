#include "simple_logger.h"

#include "player.h"
#include "gfc_input.h"
#include "gf3d_camera.h"
#include "projectile.h"
#include "deploy.h"

void player_think(Entity* self);
void player_update(Entity* self);
void player_collide(Entity* self, Entity* collide);

GFC_Vector3D dir = { 0 }, velocity = { 0 }, up = { 0 }, angle = { 0 };
int jump = 0, class = 0, isMech = 0, swordspin = 0, swordswing = 0;
float theta = 0, zax = 5, yax = 40;
float dashMod = 1, leaping = 0, jetFuel = 100, speedMod = 1, moveCDMod = 0, specCDMod = 0, speedpadmode = 0;
Entity* teleent = NULL, * teleext = NULL;  
Bool swordside = false, buffedfield = false;

Entity* player_init(GFC_Vector3D position, GFC_Color color)
{
	Entity* self;
	self = entity_new();
	if (!self)return;
	gfc_line_cpy(self->name, "player");
	self->obj = "player";
	self->mesh = gf3d_mesh_load("models/Player.obj");
	self->texture = gf3d_texture_load("models/primitives/flatblue.png");
	self->color = color;
	self->position = position;
	self->position.z = 0;
	self->rotation = gfc_vector3d(0, 0, 0);
	self->scale = gfc_vector3d(2.5, 2.5, 2.5);
	self->think = player_think;
	self->update = player_update;
	self->collide = player_collide;
	GFC_Box hitbox = gfc_box(self->position.x, self->position.y, self->position.z, 5, 5, 5); 
	self->bounds = hitbox; 
	self->attSpeed = 1;
	self->attMod = 3.0;
	self->speedMod = 1.0;
	self->MoveCD = 1.0;
	self->SpecCD = 1.0;
	self->health = 100;
	self->jumpForce = 1.0;
	self->armor = 5;
	self->damageMod = 1;

	self->inventory[0] = 0; 
	self->inventory[1] = 0;
	self->inventory[2] = 0;
	self->inventory[3] = 0;
	self->inventory[4] = 0;
	self->inventory[5] = 0;
	self->inventory[6] = 0;
	self->inventory[7] = 0;
	self->inventory[8] = 0;
	self->inventory[9] = 0;

	self->maxhealth = 100;

	self->canInteract = false;

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
		up.z = 2.5 * self->jumpForce;
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

	if (self->attSpeed < 1)
	{
		self->attSpeed += (0.1 * self->attMod);
		if (buffedfield)self->attSpeed += (0.5 * self->attMod);
	}

	else if ((SDL_GetMouseState(NULL, NULL) == SDL_BUTTON_LMASK) && (self->attSpeed >= 1))
	{
		self->attSpeed = 0;
		if (class == 0 || (class == 3 && isMech == 0))
		{
			create_projectile(self->position, gfc_vector3d(camforward->x, camforward->y, (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
		}
		else if (class == 1)
		{
			create_rocket(self->position, gfc_vector3d(camforward->x, camforward->y, (5 - zax) * 0.20), GFC_COLOR_WHITE, false, self->damageMod);
		}
		else if (class == 2)
		{
			swordswing = 10 / self->attMod;
		}
		else if (class == 3 && isMech > 0)
		{
			create_projectile(self->position, gfc_vector3d(camforward->x, camforward->y, (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
			create_projectile(self->position, gfc_vector3d(camforward->x + (camright->x * 0.1), camforward->y + (camright->y * 0.1), (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
			create_projectile(self->position, gfc_vector3d(camforward->x - (camright->x * 0.1), camforward->y - (camright->y * 0.1), (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
		}
		else if (class == 4)
		{
			create_projectile(self->position, gfc_vector3d(camforward->x, camforward->y, (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
			create_projectile(self->position, gfc_vector3d(camforward->x + (camright->x * 0.1), camforward->y + (camright->y * 0.1), (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
			create_projectile(self->position, gfc_vector3d(camforward->x - (camright->x * 0.1), camforward->y - (camright->y * 0.1), (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
			create_projectile(self->position, gfc_vector3d(camforward->x + (camright->x * 0.2), camforward->y + (camright->y * 0.2), (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
			create_projectile(self->position, gfc_vector3d(camforward->x - (camright->x * 0.2), camforward->y - (camright->y * 0.2), (5 - zax) * 0.20), GFC_COLOR_WHITE, self->damageMod);
		}
	}

	if (gfc_input_command_pressed("no")) 
	{
		if (class <= 3)class += 1;
		else class = 0;
		slog("class: %i", class);
		if (class == 0)self->attMod = 3.0f;
		else if (class == 1)self->attMod = 0.25f;
		else if (class == 2)
		{
			self->attMod = 1.0f;
			self->rotation.z += 1; 
			self->mesh = gf3d_mesh_load("models/PlayerSword.obj");
		}
		else if (class == 3) 
		{
			self->attMod = 1.0f;
			self->rotation.z -= 1;
			self->mesh = gf3d_mesh_load("models/Player.obj");
		}
		
		else if (class == 4)self->attMod = 0.2f;
	}

	if (gfc_input_command_pressed("cancel"))self->health -= 10;

	if(self->MoveCD < 1.0)self->MoveCD += 0.01 + moveCDMod;
	if(self->SpecCD < 1.0)self->SpecCD += 0.01 + specCDMod;

	if (class == 0 && gfc_input_command_held("movementab") && self->position.z <= 0)
	{
		dir = gfc_vector3d_multiply(dir, gfc_vector3d(1.4, 1.4, 0));
		self->MoveCD = 0;
	}
	if (class == 0 && gfc_input_command_released("movementab")) 
	{
		self->MoveCD = 1.0;
	}
	if (class == 1 && gfc_input_command_pressed("movementab") && jump == 1 && self->MoveCD >= 1.0)      
	{
		up.z = 3.5;
		//dir.x += camforward->x * 50;
		//dir.y += camforward->y * 50;
		jump = 0;
		leaping = 40;
		self->MoveCD = 0;
		create_explosion(self->position, GFC_COLOR_WHITE);
	}
	else if (class == 1 && leaping > 0)
	{
		dir.x += camforward->x * 3; 
		dir.y += camforward->y * 3; 
		up.z += 0.05;
		leaping -= 1; 
	}
	if (class == 2 && (gfc_input_command_held("movementab")) && self->MoveCD >= 1.0)
	{
		if(dashMod < 100)dashMod += 2;
		dir.x *= 0.20;
		dir.y *= 0.20;
	}
	else if (class == 2 && (gfc_input_command_released("movementab")) && self->MoveCD >= 1.0)
	{
		dir.x += camforward->x * dashMod;
		dir.y += camforward->y * dashMod; 
		dashMod = 1;
		self->MoveCD = 0;
	}
	if (class == 3 && (gfc_input_command_held("movementab")) && jetFuel > 0)
	{
		if (up.z < 1 && isMech == 0)up.z += 0.25;
		else if (isMech > 0) up.z = 0;
		if(isMech == 0)jetFuel -= 1;
	}
	else if (class == 3 && self->position.z <= 0 && jetFuel < 100)
	{
		jetFuel += 2;
	}
	if (class == 4 && (gfc_input_command_pressed("movementab")))
	{
		if (teleext == NULL) {
			teleext = tele_spawn(self->position, GFC_COLOR_WHITE);
			teleext->texture = gf3d_texture_load("models/primitives/flatred.png"); 
			teleext->position.z = -3.5;
		}
		else if (teleent == NULL)
		{
			teleent = tele_spawn(self->position, GFC_COLOR_WHITE);
			teleent->position.z = -3.5; 
		}
		else
		{
			teleext->free(teleext); 
			teleext = NULL;  
			teleext = teleent; 
			teleent = tele_spawn(self->position, GFC_COLOR_WHITE); 
			teleent->position.z = -3.5;
			teleext->texture = gf3d_texture_load("models/primitives/flatred.png"); 
		}
		self->MoveCD = 0;
	}

	if (class == 0 && gfc_input_command_pressed("specialab") && self->SpecCD >= 1.0)
	{
		buff_field_spawn(gfc_vector3d(self->position.x, self->position.y, -3.5), GFC_COLOR_WHITE);
		self->SpecCD = 0;
	}
	if (class == 1 && gfc_input_command_pressed("specialab") && self->SpecCD >= 1.0) 
	{
		rocket_beacon_spawn(self->position, GFC_COLOR_WHITE);
		self->SpecCD = 0; 
	} 
	if (class == 2 && gfc_input_command_pressed("specialab") && self->SpecCD >= 1.0)
	{
		swordspin = 60;
		self->SpecCD = 0;
	}
	if (class == 3 && gfc_input_command_held("specialab") && isMech <= 0 && self->SpecCD >= 1.0)
	{
		mech_spawn(gfc_vector3d(self->position.x + (camforward->x * 30), self->position.y + (camforward->y * 30), 200), GFC_COLOR_WHITE, self->rotation.z);  
		self->SpecCD = 0;
	}
	if (class == 3 && gfc_input_command_held("specialab") && (isMech > 0) && (self->attSpeed >= 1))
	{
		create_rocket(self->position, gfc_vector3d(camforward->x, camforward->y, 0), GFC_COLOR_WHITE, true, self->damageMod);
		self->attSpeed = 0;
	}
	if (class == 3 && isMech == 500)
	{
		self->mesh = gf3d_mesh_load("models/mech.obj");
		self->scale = gfc_vector3d(4, 4, 4);
		speedMod = 0.50;
		self->attMod += 0.5;
		isMech = 500;
	}
	else if (class == 3 && isMech == 1)
	{
		self->mesh = gf3d_mesh_load("models/Player.obj");
		self->scale = gfc_vector3d(2.5, 2.5, 2.5);
		self->attMod -= 0.2;
		speedMod = 1;
		isMech = 0;
		self->SpecCD = 0;
		jetFuel = 0;
	}
	if (class == 4 && (gfc_input_command_pressed("specialab")) && self->SpecCD >= 1.0)
	{
		turret_spawn(gfc_vector3d(self->position.x, self->position.y, 0), GFC_COLOR_WHITE);
		self->SpecCD = 0;
	}

	gfc_vector3d_scale(velocity, gfc_vector3d(dir.x * speedMod, dir.y * speedMod, 0), 0.5);
	gfc_vector3d_scale(velocity, velocity, self->speedMod);
 
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
		if (zax > 0)
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
			zax = 0;
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

	if ((swordspin > 0) && (class == 2))
	{
		self->rotation.z += (GFC_2PI * 5) / 60;
		swordspin -= 1;
		self->bounds.w = 10;
		self->bounds.d = 10;
		self->bounds.h = 10;
	}
	if ((swordspin == 0) && (self->bounds.w == 5) && (class == 2))
	{
		self->bounds.w = 5;
		self->bounds.d = 5;
		self->bounds.h = 5;
	}
	if ((swordswing > 0) && (class == 2))
	{
		if (swordside == false)
		{
			self->rotation.z -= 0.2;
		}
		else
		{
			self->rotation.z += 0.2;
		}
		if (swordswing == 1)
		{
			if (swordside == false)swordside = true; 
			else swordside = false; 
		}
		swordswing -= 1;
	}

	if ((!gfc_vector3d_is_zero(velocity)) || (!gfc_vector3d_is_zero(up)))
	{
		gfc_vector3d_add(self->position, self->position, velocity);
		gfc_vector3d_add(self->position, self->position, up);

		if (speedpadmode > 0)
		{
			gfc_vector3d_add(self->position, self->position, velocity);
			gfc_vector3d_add(self->position, self->position, velocity);
			gfc_vector3d_add(self->position, self->position, velocity);  
			gfc_vector3d_add(self->position, self->position, velocity);
			gfc_vector3d_add(self->position, self->position, velocity);
			gfc_vector3d_add(self->position, self->position, velocity);
			speedpadmode -= 1;
		}
	}

	if (self->position.y > 280)self->position.y = 280;
	else if (self->position.y < -280)self->position.y = -280;
	if (self->position.x > 280)self->position.x = 280;
	else if(self->position.x < -280)self->position.x = -280;

	*self->camera = self->position; 
	self->camera->z += zax; 
	self->camera->y += 15; 

	if (isMech > 0)
	{
		self->camera->y += 10;
		isMech -= 1;
	}

	newvec = gfc_vector2d_rotate_around_center(gfc_vector2d(self->camera->x, self->camera->y), theta, gfc_vector2d(self->position.x, self->position.y));

	self->camera->x = newvec.x;
	self->camera->y = newvec.y;

	gf3d_camera_look_at(gfc_vector3d(self->position.x, self->position.y, self->position.z + 5), self->camera); 

	self->bounds.x = self->position.x - 2.5;
	self->bounds.y = self->position.y - 2.5;
	self->bounds.z = self->position.z - 2.5;

	if ((swordspin > 0) && (class == 2))
	{
		self->bounds.x = self->position.x - 5;
		self->bounds.y = self->position.y - 5;
		self->bounds.z = self->position.z - 5;
	}

	if (self->maxhealth < self->health)
	{
		self->maxhealth = self->health;
	}

	buffedfield = false;
	self->canInteract = false;
}

void player_collide(Entity* self, Entity* collide)
{
	if (!self)return;
	if ((collide->obj == "tele") && (teleent != NULL)) 
	{
		if (gfc_vector3d_distance_between_less_than(self->position, teleent->position, 6.0)) 
		{
			self->position.x = teleext->position.x; 
			self->position.y = teleext->position.y; 
			//slog("turret collided with %s", collide->obj);  
		}
	}
	if ((collide->obj == "monster") && (swordspin > 0))
	{
		collide->collide(collide, self); 
	}
	if (collide->obj == "bufffield")buffedfield = true;
	if (collide->obj == "syringe")
	{
		self->attMod += 0.05;
		collide->free(collide);
		self->inventory[0] += 1;
	}
	if (collide->obj == "boots")
	{
		self->speedMod += 0.05;
		collide->free(collide);
		self->inventory[1] += 1;
	}
	if (collide->obj == "movCd")
	{
		moveCDMod += 0.05;
		collide->free(collide);
		self->inventory[2] += 1;
	}
	if (collide->obj == "specCd")
	{
		specCDMod += 0.05;
		collide->free(collide);
		self->inventory[3] += 1;
	}
	if (collide->obj == "healthUp")
	{
		self->health += 25;
		collide->free(collide);
		self->inventory[4] += 1;
	}
	if (collide->obj == "jumpUp")
	{
		self->jumpForce += 0.05;
		collide->free(collide);
		self->inventory[5] += 1;
	}
	if (collide->obj == "armor")
	{
		self->armor += 5;
		collide->free(collide);
		self->inventory[6] += 1;
	}
	if (collide->obj == "damage")
	{
		self->damageMod += 1;
		collide->free(collide);
		self->inventory[7] += 1;
	}
	if ((collide->obj == "itemcon") && (gfc_input_command_pressed("interact")))
	{
		collide->collide(collide, self); 
	}
	if ((collide->obj == "mech") && (gfc_input_command_pressed("interact")))
	{
		self->position.x = collide->position.x;
		self->position.y = collide->position.y;
		isMech = 500;
		collide->free(collide);
	}
	if ((collide->obj == "itemcon") || (collide->obj == "mech") || (collide->obj == "randomShrine") || (collide->obj == "healthShrine"))
	{
		self->canInteract = true; 
	}
	if (collide->obj == "jumppad")
	{
		up.z += 3;
	} 
	if (collide->obj == "speedpad")
	{
		speedpadmode = 15;
	}
	if ((collide->obj == "randomShrine") && (gfc_input_command_pressed("interact")))
	{
		collide->collide(collide, self);
	}
	if ((collide->obj == "healthShrine") && (gfc_input_command_pressed("interact")))
	{
		collide->collide(collide, self);
	}

	if ((collide->obj == "healthfield"))
	{
		if(self->health < self->maxhealth)
		self->health += 1;
	}

	//slog("player collided with %s", collide->obj);
}
