#pragma once

class Car {
public:
    int x;
    int y;
    

    Car(int x_, int y_) {
        x = x_;
        y = y_;
        
    }
    
    int getX() {
        return x;
    }

    int getY() {
        return y;
    }

   

    void setX(int tall) {
        this->x = tall;
    }

    void setY(int tall) {
        this->y = tall;
    }

    void kjorBilX() {
        this->x = x + 4;
    }

    void kjorBilY() {
        this->y = y + 4;
    }
    
};