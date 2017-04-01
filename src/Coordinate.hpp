
#ifndef _COORDINATE_HPP_
#define _COORDINATE_HPP_

/* Store x, y coordinates in convenient way */
struct Coordinate {

    int x, y;

    Coordinate(int x, int y);
};

struct BoxCoordinate : public Coordinate {
    int width, height;
    
    BoxCoordinate(int x, int y, int width, int height);
};

/* Keeps x, y, w, h clamped to limits */
struct BoundedCoordinate : public BoxCoordinate{

    int max_x, max_y;
        
    BoundedCoordinate(int x, int y, int width, int height, int max_x, int max_y);

    void clampToMax(int max_x, int max_y);
};

#endif /* _COORDINATE_HPP_ */


