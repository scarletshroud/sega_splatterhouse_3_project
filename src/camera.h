#ifndef _CAMERA_H_
#define _CAMERA_H_ 

#include <genesis.h>

#define MAP_BLOCK_SIZE 128

struct camera {
    fix16 pos_x;
    fix16 pos_y;
    fix16 velocity;
    Map* map; 
};

void camera_init(struct camera *cam, Map *map, fix16 start_pos_x, fix16 start_pos_y, fix16 velocity);
void set_camera_position(struct camera *cam, fix16 pos_x, fix16 pos_y);
void move_camera_left(struct camera *cam);
void move_camera_right(struct camera *cam);
void move_camera_horizontally(struct camera *cam, fix16 pos_x); 
void move_camera_vertically(struct camera *cam, fix16 pos_y);   

#endif