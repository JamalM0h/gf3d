#include <SDL.h>            

#include "simple_json.h"
#include "simple_logger.h"

#include "gfc_input.h"
#include "gfc_config_def.h"
#include "gfc_vector.h"
#include "gfc_matrix.h"
#include "gfc_audio.h"
#include "gfc_string.h"
#include "gfc_actions.h"

#include "gf2d_sprite.h"
#include "gf2d_font.h"
#include "gf2d_actor.h"
#include "gf2d_mouse.h"

#include "gf3d_vgraphics.h"
#include "gf3d_pipeline.h"
#include "gf3d_swapchain.h"

#include "gf3d_camera.h"
#include "gf3d_mesh.h"
#include "entity.h"
#include "monster.h"
#include "world.h"
#include "item.h"
#include "interact.h"
#include "player.h"

extern int __DEBUG;

static int _done = 0;
static Uint32 frame_delay = 33;
static float fps = 0;

void parse_arguments(int argc,char *argv[]);
void game_frame_delay();

void exitGame()
{
    _done = 1;
}

int main(int argc,char *argv[])
{
    //local variables
    Mesh *mesh;
    Texture *texture;
    float theta = 0;
    int offset = 0;
    GFC_Vector3D lightPos = {0,0,-1000};
    GFC_Vector3D *cam = gfc_vector3d_new();
    GFC_Vector2D* healthbarscale = gfc_vector2d_new(), * healthbarbackscale = gfc_vector2d_new(), * armorbarbackscale = gfc_vector2d_new(), * moveCdscale = gfc_vector2d_new(), * specCdscale = gfc_vector2d_new(), *attCdscale = gfc_vector2d_new(), *goldscale = gfc_vector2d_new();
    GFC_Matrix4 id;
    Entity* player;
    Sprite* itembox, * syringe, * boot, * moveCd, * specCd, * healthUp, * jumpUp, * armor, * sword, * healthbar, *healthbarback, *armorbarback, *leftmouse, *attackicon, *movementicon, *shiftkey, *staricon, *ekey;
    char array[5];
    //initializtion  
    parse_arguments(argc,argv);
    init_logger("gf3d.log",0);
    slog("gf3d begin");
    //gfc init
    gfc_input_init("config/input.cfg");
    gfc_config_def_init();
    gfc_action_init(1024);
    //gf3d init
    gf3d_vgraphics_init("config/setup.cfg");
    gf2d_font_init("config/font.cfg");
    gf2d_actor_init(1000);

    entity_system_init(8000); 
    
    //game init
    srand(SDL_GetTicks());
    slog_sync();

    world_spawn(gfc_vector3d(0, 0, -4), GFC_COLOR_WHITE);

    jumppad_spawn(gfc_vector3d(25, 25, -3), GFC_COLOR_WHITE);
    speedpad_spawn(gfc_vector3d(10, 80, -3), GFC_COLOR_WHITE);
    speedpad_spawn(gfc_vector3d(10, 90, -3), GFC_COLOR_WHITE);
    speedpad_spawn(gfc_vector3d(10, 100, -3), GFC_COLOR_WHITE);
    speedpad_spawn(gfc_vector3d(10, 110, -3), GFC_COLOR_WHITE);

    player = player_init(gfc_vector3d(8, 0, 0), GFC_COLOR_WHITE);  
    player->camera = cam;

    healthbarscale->y = 1;

    healthbarbackscale->x = 10.0f;
    healthbarbackscale->y = 1;

    armorbarbackscale->x = 3.0f;
    armorbarbackscale->y = 1;

    attCdscale->x = 2; 

    moveCdscale->x = 2;

    specCdscale->x = 2; 
    
    goldscale->x = 3;
    goldscale->y = 0.75;

    monster_spawn(gfc_vector3d(28, 0, 0), GFC_COLOR_WHITE);
    monster_spawn(gfc_vector3d(20, 0, 0), GFC_COLOR_WHITE);

    item_container_spawn(gfc_vector3d(0, -20, 0), GFC_COLOR_WHITE, player->position,0); 
    item_container_spawn(gfc_vector3d(0, -40, 0), GFC_COLOR_WHITE, player->position,1);
    item_container_spawn(gfc_vector3d(0, -60, 0), GFC_COLOR_WHITE, player->position,2);
    item_container_spawn(gfc_vector3d(0, -80, 0), GFC_COLOR_WHITE, player->position,3);
    item_container_spawn(gfc_vector3d(0, -100, 0), GFC_COLOR_WHITE, player->position,4);
    item_container_spawn(gfc_vector3d(-50, -20, 0), GFC_COLOR_WHITE, player->position,5);
    item_container_spawn(gfc_vector3d(-50, -40, 0), GFC_COLOR_WHITE, player->position,6);
    item_container_spawn(gfc_vector3d(-50, -60, 0), GFC_COLOR_WHITE, player->position,7); 
    
    container_spawn(gfc_vector3d(100, 100, -3), GFC_COLOR_WHITE, player->position);

    randomShrine_spawn(gfc_vector3d(50, 50, -3), GFC_COLOR_WHITE, player->position);
    healthShrine_spawn(gfc_vector3d(-50, 50, -3), GFC_COLOR_WHITE);  
    
    SDL_SetRelativeMouseMode(SDL_TRUE); 

    //mesh = gf3d_mesh_load("models/dino/dino.obj");  
    //texture = gf3d_texture_load("models/dino/dino.png");
    mesh = gf3d_mesh_load("models/primitives/sphere.obj");
    texture = gf3d_texture_load("models/alienCave.png"); 
    gfc_matrix2_identity(id); 
    gfc_matrix4_from_vectors(
        id, 
        gfc_vector3d(0,0,0),
        gfc_vector3d(GFC_PI, 0, 0), 
        gfc_vector3d(-5000, -5000, -5000));

    itembox = gf2d_sprite_load("models/ui/itembox.png", 640, 64, 0);
    syringe = gf2d_sprite_load("models/ui/syringerender.png", 64, 64, 0);
    boot = gf2d_sprite_load("models/ui/bootrender.png", 64, 64, 0);
    moveCd = gf2d_sprite_load("models/ui/moveCdrender.png", 64, 64, 0); 
    specCd = gf2d_sprite_load("models/ui/specCdrender.png", 64, 64, 0); 
    healthUp = gf2d_sprite_load("models/ui/healthuprender.png", 64, 64, 0); 
    jumpUp = gf2d_sprite_load("models/ui/springrender.png", 64, 64, 0);
    armor = gf2d_sprite_load("models/ui/armorrender.png", 64, 64, 0);
    sword = gf2d_sprite_load("models/ui/swordrender.png", 64, 64, 0);
    healthbar = gf2d_sprite_load("models/primitives/flatred.png", 64, 64, 0);
    healthbarback = gf2d_sprite_load("models/primitives/flatgrey.png", 64, 64, 0);
    armorbarback = gf2d_sprite_load("models/primitives/darkgrey.png", 64, 64, 0);
    leftmouse = gf2d_sprite_load("models/ui/mouse-left-click-icon.png", 64, 64, 0);
    attackicon = gf2d_sprite_load("models/ui/sword.png", 128, 128, 0); 
    movementicon = gf2d_sprite_load("models/ui/movement.png", 128, 128, 0);
    shiftkey = gf2d_sprite_load("models/ui/shiftkey.png", 64, 64, 0); 
    staricon = gf2d_sprite_load("models/ui/star.png", 128, 128, 0);
    ekey = gf2d_sprite_load("models/ui/ekey.png", 64, 64, 0);
    
    // main game loop    
    while(!_done)
    {
        gfc_input_update();
        gf2d_mouse_update();
        gf2d_font_update();
        entity_think_all();  
        entity_update_all();
        entity_system_collision(); 
        theta += 0.1;
        //camera updaes
        gf3d_camera_update_view();
        gf3d_vgraphics_render_start();
            //3d draws   
            gf3d_sky_draw(mesh, id, GFC_COLOR_WHITE, texture); 
            entity_draw_all(lightPos, GFC_COLOR_WHITE);   
            //gf2d_font_draw_line_tag("ALT+F4 to exit", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
           // gf2d_font_draw_line_tag("X", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(631, 390));
            //if (player->SpecCD >= 1.0)gf2d_font_draw_line_tag("Special Ready", FT_H1, GFC_COLOR_GREEN, gfc_vector2d(1536, 922));
            //else gf2d_font_draw_line_tag("Special Ready", FT_H1, GFC_COLOR_RED, gfc_vector2d(1536, 922));   
            //if (player->MoveCD >= 1.0)gf2d_font_draw_line_tag("Movement Ready", FT_H1, GFC_COLOR_GREEN, gfc_vector2d(1536, 972));
            //else gf2d_font_draw_line_tag("Movement Ready", FT_H1, GFC_COLOR_RED, gfc_vector2d(1536, 972));

            if (player->canInteract == true)
            {
                gf2d_font_draw_line_tag("Press  Enter  Key", FT_H1, GFC_COLOR_GREY, gfc_vector2d(1240, 540));
            }

            gf2d_sprite_draw(armorbarback, gfc_vector2d(65, 925), armorbarbackscale, NULL, NULL, NULL, NULL, NULL, NULL);   

            gf2d_sprite_draw(healthbarback, gfc_vector2d(65, 972), healthbarbackscale, NULL, NULL, NULL, NULL, NULL, NULL);

            healthbarscale->x = ((float)player->health / (float)player->maxhealth) * 10.0f; 
            gf2d_sprite_draw(healthbar, gfc_vector2d(65, 972), healthbarscale, NULL, NULL, NULL, NULL, NULL, NULL);

            snprintf(array, sizeof(array), "%i", player->health); 
            gf2d_font_draw_line_tag("Health: ", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(265, 989));
            gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_WHITE, gfc_vector2d(390, 990));  

            snprintf(array, sizeof(array), "%i", player->maxhealth);
            gf2d_font_draw_line_tag(" | ", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(455, 988)); 
            gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_WHITE, gfc_vector2d(490, 990)); 

            snprintf(array, sizeof(array), "%i", player->armor); 
            gf2d_font_draw_line_tag("Armor: ", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(71, 933));  
            gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_WHITE, gfc_vector2d(180, 935));    

            attCdscale->y = ((float)player->attSpeed / 1.0f) * -2.0f; 
            if (attCdscale->y < -2.0f)attCdscale->y = -2.0f;
            gf2d_sprite_draw(healthbarback, gfc_vector2d(1300, 951), attCdscale, NULL, NULL, NULL, NULL, NULL, NULL);
            gf2d_sprite_draw(attackicon, gfc_vector2d(1300, 822), NULL, NULL, NULL, NULL, NULL, NULL, NULL);   
            gf2d_sprite_draw(leftmouse, gfc_vector2d(1335, 982), NULL, NULL, NULL, NULL, NULL, NULL, NULL);  

            moveCdscale->y = ((float)player->MoveCD / 1.0f) * -2.0f; 
            if (moveCdscale->y < -2.0f)moveCdscale->y = -2.0f; 
            gf2d_sprite_draw(healthbarback, gfc_vector2d(1450, 951), moveCdscale, NULL, NULL, NULL, NULL, NULL, NULL);
            gf2d_sprite_draw(movementicon, gfc_vector2d(1450, 822), NULL, NULL, NULL, NULL, NULL, NULL, NULL); 
            gf2d_sprite_draw(shiftkey, gfc_vector2d(1482, 982), NULL, NULL, NULL, NULL, NULL, NULL, NULL);

            specCdscale->y = ((float)player->SpecCD / 1.0f) * -2.0f; 
            if (specCdscale->y < -2.0f)specCdscale->y = -2.0f;
            gf2d_sprite_draw(healthbarback, gfc_vector2d(1598, 951), specCdscale, NULL, NULL, NULL, NULL, NULL, NULL);
            gf2d_sprite_draw(staricon, gfc_vector2d(1598, 822), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
            gf2d_sprite_draw(ekey, gfc_vector2d(1630, 982), NULL, NULL, NULL, NULL, NULL, NULL, NULL); 

            gf2d_sprite_draw(itembox, gfc_vector2d(640,20),NULL,NULL,NULL,NULL,NULL,NULL,NULL); 

            offset = 0;

            if (player->inventory[0] != 0)
            {
                gf2d_sprite_draw(syringe, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL); 
                snprintf(array, sizeof(array), "%i", player->inventory[0]); 
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94)); 
                offset += 64;
            }
            if (player->inventory[1] != 0)
            {
                gf2d_sprite_draw(boot, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                snprintf(array, sizeof(array), "%i", player->inventory[1]);
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94));
                offset += 64;
            }
            if (player->inventory[2] != 0)
            {
                gf2d_sprite_draw(moveCd, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                snprintf(array, sizeof(array), "%i", player->inventory[2]); 
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94));
                offset += 64;
            }
            if (player->inventory[3] != 0)
            {
                gf2d_sprite_draw(specCd, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                snprintf(array, sizeof(array), "%i", player->inventory[3]);
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94));
                offset += 64;
            }
            if (player->inventory[4] != 0)
            {
                gf2d_sprite_draw(healthUp, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                snprintf(array, sizeof(array), "%i", player->inventory[4]);
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94));
                offset += 64;
            }
            if (player->inventory[5] != 0)
            {
                gf2d_sprite_draw(jumpUp, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                snprintf(array, sizeof(array), "%i", player->inventory[5]);
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94));
                offset += 64;
            }
            if (player->inventory[6] != 0)
            {
                gf2d_sprite_draw(armor, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                snprintf(array, sizeof(array), "%i", player->inventory[6]); 
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94));
                offset += 64; 
            }
            if (player->inventory[7] != 0)
            {
                gf2d_sprite_draw(sword, gfc_vector2d(640 + offset, 20), NULL, NULL, NULL, NULL, NULL, NULL, NULL);
                snprintf(array, sizeof(array), "%i", player->inventory[7]);
                gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_GREY, gfc_vector2d(642 + offset, 94));
                offset += 64;
            }

            snprintf(array, sizeof(array), "%i", player->money); 
            gf2d_sprite_draw(healthbarback, gfc_vector2d(238, 30), goldscale, NULL, NULL, NULL, NULL, NULL, NULL);  
            gf2d_font_draw_line_tag("Gold: ", FT_H1, GFC_COLOR_YELLOW, gfc_vector2d(250, 40));  
            gf2d_font_draw_line_tag(array, FT_H1, GFC_COLOR_YELLOW, gfc_vector2d(345, 40)); 

        gf3d_vgraphics_render_end();
        if (gfc_input_command_down("exit"))_done = 1; // exit condition
        game_frame_delay();
    }
    vkDeviceWaitIdle(gf3d_vgraphics_get_default_logical_device());    
    //cleanup
    exit(0);
    slog("gf3d program end");
    slog_sync();
    return 0;
}

void parse_arguments(int argc,char *argv[])
{
    int a;

    for (a = 1; a < argc;a++)
    {
        if (strcmp(argv[a],"--debug") == 0)
        {
            __DEBUG = 1;
        }
    }    
}

void game_frame_delay()
{
    Uint32 diff;
    static Uint32 now;
    static Uint32 then;
    then = now;
    slog_sync();// make sure logs get written when we have time to write it
    now = SDL_GetTicks();
    diff = (now - then);
    if (diff < frame_delay)
    {
        SDL_Delay(frame_delay - diff);
    }
    fps = 1000.0/MAX(SDL_GetTicks() - then,0.001);
//     slog("fps: %f",fps);
}
/*eol@eof*/
