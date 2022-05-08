#include "camera.h"
#include "player.h"

void camera_init(struct camera *cam, Map *map, fix16 start_pos_x, fix16 start_pos_y, fix16 velocity) {
    cam->pos_x = start_pos_x;
    cam->pos_y = start_pos_y;
    cam->map = map;
    cam->velocity = velocity;
    MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y)); 
}

void set_camera_position(struct camera *cam, fix16 pos_x, fix16 pos_y) {
    if ((cam->pos_x != pos_x) || (cam->pos_y != pos_y)) {
        cam->pos_x = pos_x; 
        cam->pos_y = pos_y;
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));  
    }
}

static void move_camera_left(struct camera *cam) {
    if (cam->pos_x > intToFix16(0)) {
        cam->pos_x = fix16Add(cam->pos_x, -cam->velocity);
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));  
    } 
}

static void move_camera_right(struct camera *cam) {
    if (fix16ToInt(cam->pos_x) < MAP_BLOCK_SIZE + 60) {
        cam->pos_x = fix16Add(cam->pos_x, cam->velocity);
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));
    }   
}

static void move_camera_horizontally(struct camera *cam, fix16 pos_x) {
    if (cam->pos_x != pos_x) {
        cam->pos_x = pos_x;
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));   
    }
}

static void move_camera_vertically(struct camera *cam, fix16 pos_y) {
    if (cam->pos_y != pos_y) {
        cam->pos_y = pos_y;
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));  
    }
}

fix16 prev = 0;
#define CAMERA_RIGHT_BORDER 15
#define CAMERA_LEFT_BORDER 165
void camera_update(struct camera* cam) {
    struct player_position p_pos = player_get_position();
    enum PLAYER_MOVE_DIRECTION p_dir = player_get_direction();

    if (fix16ToInt(p_pos.x) > CAMERA_RIGHT_BORDER && prev != p_pos.x &&  p_dir == DIRECTION_RIGHT) {
        move_camera_right(cam);
    }
    
    if (fix16ToInt(p_pos.x) < CAMERA_LEFT_BORDER && prev != p_pos.x && p_dir == DIRECTION_LEFT)
        move_camera_left(cam);

    prev = p_pos.x;
}