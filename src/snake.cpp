#include "snake.h"
#include "screen.h"
#include <unistd.h>

CSnake::CSnake(CRect r, char _c /*=''*/) :
        CFramedWindow(r, _c) {
    state = HOME;
    length = 3;
    snake.push_front(CPoint(geom.topleft.x + 2, geom.topleft.y + 1));
    snake.push_front(CPoint(geom.topleft.x + 3, geom.topleft.y + 1));
    snake.push_front(CPoint(geom.topleft.x + 4, geom.topleft.y + 1));
    dir = RIGHT;
    food = CPoint(geom.topleft.x + 5, geom.topleft.y + 5);
    err_counter = 1;
    speed = 20;
}


void CSnake::paint() {
    if (state == HOME) {
        paint_home();
    } else if (state == GAME_OVER) {
        paint_game_over();
    } else if (state == PLAY) {
        paint_game();
    } else if (state == PAUSE) {
        paint_pause();
    } else if (state == HELP) {
        paint_help();
    }
}


bool CSnake::handleEvent(int c) {

    if (state == HOME) {
        if (CFramedWindow::handleEvent(c))
            return true;
        if (c == 'h') {
            state = HELP;
            return true;
        } else if (c == 'p') {
            state = PLAY;
            snake.clear();
            length = 3;
            snake.push_front(CPoint(geom.topleft.x + 2, geom.topleft.y + 1));
            snake.push_front(CPoint(geom.topleft.x + 3, geom.topleft.y + 1));
            snake.push_front(CPoint(geom.topleft.x + 4, geom.topleft.y + 1));
            dir = RIGHT;
            food = CPoint(geom.topleft.x + 5, geom.topleft.y + 5);
            speed = 20;

            return true;
        }
    } else if (state == PLAY) {
        if (c == 'p') {
            state = PAUSE;
            return true;
        } else if (c == KEY_UP) {
            if (dir != DOWN) {
                dir = UP;
                move_snake();
                paint_game();
            }
            return true;
        } else if (c == KEY_DOWN) {
            if (dir != UP) {
                dir = DOWN;
                move_snake();
                paint_game();
            }
            return true;
        } else if (c == KEY_LEFT) {
            if (dir != RIGHT) {
                dir = LEFT;
                move_snake();
                paint_game();
            }
            return true;
        } else if (c == KEY_RIGHT) {
            if (dir != LEFT) {
                dir = RIGHT;
                move_snake();
                paint_game();
            }
            return true;
        } else if (c == ERR) {
            if (err_counter % speed == 0) {
                move_snake();
                err_counter = 1;

                if (length > 5 && length < 8) {
                    speed = 18;
                } else if (length >= 8 && length < 12) {
                    speed = 16;
                } else if (length >= 12 && length < 16) {
                    speed = 12;
                } else if (length >= 16 && length < 20) {
                    speed = 8;
                } else if (length >= 20) {
                    speed = 6;
                }
            } else {
                err_counter++;
            }


            return true;
        }


    } else if (state == GAME_OVER) {
        if (CFramedWindow::handleEvent(c))
            return true;
        if (c == 'p') {
            state = PLAY;
            snake.clear();
            length = 3;
            snake.push_front(CPoint(geom.topleft.x + 2, geom.topleft.y + 1));
            snake.push_front(CPoint(geom.topleft.x + 3, geom.topleft.y + 1));
            snake.push_front(CPoint(geom.topleft.x + 4, geom.topleft.y + 1));
            dir = RIGHT;
            food = CPoint(geom.topleft.x + 5, geom.topleft.y + 5);
            speed = 20;

            return true;
        } else if (c == 'h') {
            state = HELP;
            snake.clear();
            length = 3;
            snake.push_front(CPoint(geom.topleft.x + 2, geom.topleft.y + 1));
            snake.push_front(CPoint(geom.topleft.x + 3, geom.topleft.y + 1));
            snake.push_front(CPoint(geom.topleft.x + 4, geom.topleft.y + 1));
            dir = RIGHT;
            food = CPoint(geom.topleft.x + 5, geom.topleft.y + 5);
            speed = 20;
            return true;
        }
    } else if (state == HELP) {
        if (CFramedWindow::handleEvent(c))
            return true;
        if (c == 'b') {
            state = HOME;
            return true;
        }
    } else if (state == PAUSE) {
        if (CFramedWindow::handleEvent(c))
            return true;
        if (c == 'p') {
            state = PLAY;
            return true;
        }
    }
    return false;
}

void CSnake::clear_screen() {
    for (int y = geom.topleft.y + 1; y < geom.topleft.y + geom.size.y - 1; y++) {
        for (int x = geom.topleft.x + 1; x < geom.topleft.x + geom.size.x - 1; x++) {
            gotoyx(y, x);
            printl(" ");
        }
    }
}


void CSnake::move_snake() {
    // collision detection
    std::list<struct CPoint>::iterator it;
    for (it = snake.begin(); it != snake.end(); ++it) {
        if (it != snake.begin() && it->x == snake.begin()->x && it->y == snake.begin()->y) {
            state = GAME_OVER;
            paint_game_over();
            return;
        }
    }

    // handle wall collision
    if (snake.begin()->x == geom.topleft.x) {
        snake.begin()->x = geom.topleft.x + geom.size.x - 2;
    } else if (snake.begin()->x == geom.topleft.x + geom.size.x - 1) {
        snake.begin()->x = geom.topleft.x + 1;
    } else if (snake.begin()->y == geom.topleft.y) {
        snake.begin()->y = geom.topleft.y + geom.size.y - 2;
    } else if (snake.begin()->y == geom.topleft.y + geom.size.y - 1) {
        snake.begin()->y = geom.topleft.y + 1;
    }

    // food check
    if (snake.begin()->x == food.x && snake.begin()->y == food.y) {
        length++;
        snake.push_back(CPoint(snake.back().x, snake.back().y));
        // create new food
        while (true) {
            food.x = geom.topleft.x + rand() % (geom.size.x - 2) + 1;
            food.y = geom.topleft.y + rand() % (geom.size.y - 2) + 1;

            //avoid food on snake
            bool ok = true;
            std::list<struct CPoint>::iterator it;
            for (it = snake.begin(); it != snake.end(); ++it) {
                if (it->x == food.x && it->y == food.y) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                break;
            }
        }
        gotoyx(food.y, food.x);
        printl("*");
    }

    //move snake
    if (dir == UP) {
        snake.push_front(CPoint(snake.begin()->x, snake.begin()->y - 1));
        snake.pop_back();
    } else if (dir == DOWN) {
        snake.push_front(CPoint(snake.begin()->x, snake.begin()->y + 1));
        snake.pop_back();
    } else if (dir == LEFT) {
        snake.push_front(CPoint(snake.begin()->x - 1, snake.begin()->y));
        snake.pop_back();
    } else if (dir == RIGHT) {
        snake.push_front(CPoint(snake.begin()->x + 1, snake.begin()->y));
        snake.pop_back();
    }
}


void CSnake::paint_game_over() {
    CFramedWindow::paint();

    gotoyx(geom.topleft.y + 3, geom.topleft.x + 3);
    printl("Game over!");

    gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
    printl("Score: %d", length);

    gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
    printl("p - play again");
    gotoyx(geom.topleft.y + 8, geom.topleft.x + 3);
    printl("q - quit");
    gotoyx(geom.topleft.y + 9, geom.topleft.x + 3);
    printl("h - help");

}

void CSnake::paint_home() {
    CFramedWindow::paint();
    gotoyx(geom.topleft.y + 3, geom.topleft.x + 3);
    printl("Home");


    gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
    printl("h - toggle help information");

    gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
    printl("p - toggle pause/play mode");

    gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
    printl("arrows - move snake (in play mode) or");
    gotoyx(geom.topleft.y + 8, geom.topleft.x + 3);
    printl("         move window (in pause mode)");
}

void CSnake::paint_pause() {
    CFramedWindow::paint();
    gotoyx(geom.topleft.y + 3, geom.topleft.x + 3);
    printl("Pause");
}

void CSnake::paint_help() {
    CFramedWindow::paint();
    gotoyx(geom.topleft.y + 3, geom.topleft.x + 3);
    printl("Eat food to increase your score and");
    gotoyx(geom.topleft.y + 4, geom.topleft.x + 3);
    printl("snake length.");

    gotoyx(geom.topleft.y + 5, geom.topleft.x + 3);
    printl("Be strategic in your movements to avoid");
    gotoyx(geom.topleft.y + 6, geom.topleft.x + 3);
    printl("collisions.");

    gotoyx(geom.topleft.y + 7, geom.topleft.x + 3);
    printl("The game speeds up as the snake gets");
    gotoyx(geom.topleft.y + 8, geom.topleft.x + 3);
    printl("longer, so stay alert!");

    gotoyx(geom.topleft.y + 10, geom.topleft.x + 3);
    printl("b - go home");
}

void CSnake::paint_game() {
    CFramedWindow::paint();

    //print score
    gotoyx(geom.topleft.y - 1, geom.topleft.x);
    printl("Score: %d", length);

    // paint food
    gotoyx(food.y, food.x);
    printl("*");

    // paint snake
    std::list<struct CPoint>::iterator it;
    for (it = snake.begin(); it != snake.end(); ++it) {
        if (it == snake.begin()) {
            gotoyx(it->y, it->x);
            printl("@");
        } else {
            gotoyx(it->y, it->x);
            printl("o");
        }
    }
}
