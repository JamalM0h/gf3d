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
#include "itemcontain.h"
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
    GFC_Vector3D lightPos = {0,0,-1000};
    GFC_Vector3D *cam = gfc_vector3d_new();
    GFC_Matrix4 id;
    Entity* player;
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
    monster_spawn(gfc_vector3d(28,0,0), GFC_COLOR_WHITE); 
    monster_spawn(gfc_vector3d(20, 0, 0), GFC_COLOR_WHITE); 
    world_spawn(gfc_vector3d(0, 0, -4), GFC_COLOR_WHITE);
    player = player_init(gfc_vector3d(8, 0, 0), GFC_COLOR_WHITE);  
    player->camera = cam;
    item_spawn(gfc_vector3d(20, 20, 0), GFC_COLOR_WHITE, gfc_vector3d(0,0,0), false, 0);  
    item_container_spawn(gfc_vector3d(0, -20, 0), GFC_COLOR_WHITE, player->position,1);
    item_container_spawn(gfc_vector3d(0, -30, 0), GFC_COLOR_WHITE, player->position,0);
    item_container_spawn(gfc_vector3d(0, -40, 0), GFC_COLOR_WHITE, player->position,2);
    item_container_spawn(gfc_vector3d(0, -50, 0), GFC_COLOR_WHITE, player->position,3);
    
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
            gf2d_font_draw_line_tag("ALT+F4 to exit", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(10, 10));
           // gf2d_font_draw_line_tag("X", FT_H1, GFC_COLOR_WHITE, gfc_vector2d(631, 390));
            if (player->SpecCD >= 1.0)gf2d_font_draw_line_tag("Special Ready", FT_H1, GFC_COLOR_GREEN, gfc_vector2d(990, 620)); 
            else gf2d_font_draw_line_tag("Special Ready", FT_H1, GFC_COLOR_RED, gfc_vector2d(990, 620));   
            if (player->MoveCD >= 1.0)gf2d_font_draw_line_tag("Movement Ready", FT_H1, GFC_COLOR_GREEN, gfc_vector2d(990, 670));
            else gf2d_font_draw_line_tag("Movement Ready", FT_H1, GFC_COLOR_RED, gfc_vector2d(990, 670));
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
