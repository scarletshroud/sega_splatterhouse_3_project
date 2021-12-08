#include "camera.h"

void camera_init(struct camera *cam, Map *map, fix16 start_pos_x, fix16 start_pos_y) {
    cam->pos_x = start_pos_x;
    cam->pos_y = start_pos_y;
    cam->map = map;
    MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y)); 
}

void set_camera_position(struct camera *cam, fix16 pos_x, fix16 pos_y) {
    if ((cam->pos_x != pos_x) || (cam->pos_y != pos_y)) {
        cam->pos_x = pos_x; 
        cam->pos_y = pos_y;
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));  
    }
}

void move_camera_horizontally(struct camera *cam, fix16 pos_x) {
    if (cam->pos_x != pos_x) {
        cam->pos_x = pos_x;
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));   
    }
}

void move_camera_vertically(struct camera *cam, fix16 pos_y) {
    if (cam->pos_y != pos_y) {
        cam->pos_y = pos_y;
        MAP_scrollTo(cam->map, fix16ToInt(cam->pos_x), fix16ToInt(cam->pos_y));  
    }
} 