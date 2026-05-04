#include <raylib.h>
#include <iostream>
#include <random>
#include <time.h>

const int SPEED = 20;

struct Snake{
    int size;
    int posx;
    int posy;
    struct Snake* sNext;
    int direction;
    Rectangle box;
};

struct Apple{
    int size;
    int posx;
    int posy;
    Rectangle box;
};

void expandSnake (struct Snake* s){
    struct Snake* p = s;
    while(p->sNext != nullptr){
        p = p->sNext;
    }
    switch(p->direction){
        case 0:
            p->sNext = new Snake{p->size,p->posx - p->size,p->posy,nullptr,p->direction};
            p->sNext->box.height=p->sNext->size;
            p->sNext->box.width=p->sNext->size;
            p->sNext->box.x=p->sNext->posx;
            p->sNext->box.y=p->sNext->posy;
            break;
        case 1:
            p->sNext = new Snake{p->size,p->posx + p->size,p->posy,nullptr,p->direction};
            p->sNext->box.height=p->sNext->size;
            p->sNext->box.width=p->sNext->size;
            p->sNext->box.x=p->sNext->posx;
            p->sNext->box.y=p->sNext->posy;
            break;
        case 2:
            p->sNext = new Snake{p->size,p->posx,p->posy - p->size,nullptr,p->direction};
            p->sNext->box.height=p->sNext->size;
            p->sNext->box.width=p->sNext->size;
            p->sNext->box.x=p->sNext->posx;
            p->sNext->box.y=p->sNext->posy;
            break;
        case 3:
            p->sNext = new Snake{p->size,p->posx,p->posy + p->size,nullptr,p->direction};
            p->sNext->box.height=p->sNext->size;
            p->sNext->box.width=p->sNext->size;
            p->sNext->box.x=p->sNext->posx;
            p->sNext->box.y=p->sNext->posy;
            break;
    }
}

struct Snake* newSnake(int size, int posx, int posy, int direction, int qt){
    struct Snake* s = new Snake{size,posx,posy,nullptr,direction};
    s->box.height=size;
    s->box.width=size;
    s->box.x=posx;
    s->box.y=posy;
    for(int i=1; i<qt; i++){
        expandSnake(s);
    }
    return s;
}

void renderSnake(struct Snake* s){
    struct Snake* p = s;
    while(p != nullptr){
        DrawRectangle(p->posx,p->posy,p->size,p->size,GREEN);
        p = p->sNext;
    }
}

void updateSnake(struct Snake* s, int ndir){
    struct Snake* p = s;
    int olddir = p->direction;
    p->direction = ndir;
    while(p != nullptr){
        switch(p->direction){
            case 0:
                p->posx += SPEED;
                p->box.x=p->posx;
                p->box.y=p->posy;
                break;
            case 1:
                p->posx -= SPEED;
                p->box.x=p->posx;
                p->box.y=p->posy;
                break;
            case 2:
                p->posy += SPEED;
                p->box.x=p->posx;
                p->box.y=p->posy;
                break;
            case 3:
                p->posy -= SPEED;
                p->box.x=p->posx;
                p->box.y=p->posy;
                break;
        }
        p = p->sNext;
        if(p!=nullptr){
            int tr = p->direction;
            p->direction = olddir;
            olddir = tr;
        }
    }
}

bool checkCollision(struct Snake* s){
    struct Snake* p = s;
    p = p->sNext;
    while(p!=nullptr){
        if(CheckCollisionRecs(s->box, p->box)){
            return true;
        }
        p = p->sNext;
    }
    return false;
}

struct Apple* newApple(int posx, int posy, int size){
    struct Apple* a = new Apple{size,posx,posy};
    a->box.height = a->size;
    a->box.width = a->size;
    a->box.x = a->posx;
    a->box.y = a->posy;
    return a;
}

struct Apple* randomApple(){
    return newApple((rand() % (800/20))*20 ,(rand() % (600/20))*20 ,20);
}

void renderApple(struct Apple* a){
    DrawRectangle(a->posx,a->posy,a->size,a->size,RED);
}

int main() {
    InitWindow(800, 600, "Snake");
    SetTargetFPS(60);

    while(!WindowShouldClose()){

        srand(time(NULL));
    
        int score = 0;
        int direction = 0;
        struct Snake* sn = newSnake(20,400,300,direction,3);
        struct Apple* app = randomApple();

        float t = 0.0;

        while (!WindowShouldClose()) {
            
            BeginDrawing();
            ClearBackground(DARKGRAY);

            std::string Txt = "SCORE: " + std::to_string(score);
            DrawText(Txt.c_str(),0,0,30,PURPLE);

            t+=GetFrameTime();

            renderSnake(sn);

            if(IsKeyDown(KEY_D) && direction != 1){
                direction = 0;
            }
            else if(IsKeyDown(KEY_A) && direction != 0){
                direction = 1;
            }
            else if(IsKeyDown(KEY_S) && direction != 3){
                direction = 2;
            }
            else if(IsKeyDown(KEY_W) && direction != 2){
                direction = 3;
            }

            if(t>0.18){

                if(sn->direction==1 && direction==0){
                    direction = 1;
                }
                else if(sn->direction==0 && direction==1){
                    direction = 0;
                }
                else if(sn->direction==2 && direction==3){
                    direction = 2;
                }
                else if(sn->direction==3 && direction==2){
                    direction = 3;
                }

                updateSnake(sn,direction);
                t = 0.0;
            }

            if(checkCollision(sn)){
                EndDrawing();
                break;
            }

            renderApple(app);

            if(CheckCollisionRecs(sn->box,app->box)){
                score++;
                app = randomApple();
                expandSnake(sn);
            }

            if(sn->posx>780 || sn->posx<0 || sn->posy>580 || sn->posy<0){
                EndDrawing();
                break;
            }

            EndDrawing();
        }

        while(!WindowShouldClose()){
            BeginDrawing();
            ClearBackground(DARKGRAY);
            std::string Txt = "YOUR SCORE WAS: " + std::to_string(score);
            DrawText("YOU LOSE",250,250,60,WHITE);
            DrawText(Txt.c_str(),250,325,30,WHITE);
            DrawText("PRES SPACE TO RETRY",215,405,30,RED);

            if(IsKeyDown(KEY_SPACE)){
                EndDrawing();
                break;
            }

            EndDrawing();
        }
    }

    CloseWindow();
}