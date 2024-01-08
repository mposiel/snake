#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "winsys.h"
#include "cpoint.h"
#include <list>


enum game_state {
    GAME_OVER,
    PAUSE,
    PLAY,
    HOME,
    HELP
};

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class CSnake : public CFramedWindow {
private:
    int length;
    std::list<struct CPoint> snake;
    struct CPoint food;
    enum game_state state;
    enum direction dir;
    int err_counter;
    int speed; //(the lower the speed the fsster snake moves)


public:
    CSnake(CRect r, char _c = ' ');

    bool handleEvent(int c);

    void paint();

    void paint_game();

    void paint_game_over();

    void paint_home();

    void paint_pause();

    void paint_help();

    void clear_screen();

    void move_snake();

};

#endif
