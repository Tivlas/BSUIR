#pragma once
#include <QPainter>

class Rect
{
public:
    int m_x;
    int m_y;
    int m_wid;
    int m_len;
public:
    Rect(int x=0, int y=0, int width=0,int length=0);
    void draw(QPainter*);
    void openDoorLeft();
    void openDoorRight();
};

class Bus : public Rect {
private:

    Rect door1, door2, door3,door4;
public:
    Bus(int m_x = 0,int m_y=0);
    void draw(QPainter*);
    void drawLights(QPainter*);
    void openL();
    void openR();
};

