#ifndef CONFIG_H
#define CONFIG_H

struct Config
{
    std::size_t screen_width;
    std::size_t screen_height;
    int ego_thickness;
    int food_thickness;
    float wall_thickness;
    int holl_width;
};

#endif
