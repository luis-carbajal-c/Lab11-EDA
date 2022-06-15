#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <vector>

#define WINDOW_N 700    // square window dimension
#define MAX_LEVEL 8     // max level of the hilbert curve 

// struct to store coordinates, each coordinate has x and y positions
struct Coord {
    int x;
    int y;
    Coord(int x, int y) : x(x), y(y) {}
};

std::vector<Coord> hilbert_vec; // vector used to store coordinates to be traversed by the curve

// recursively compute hilbert curve traversal and store it in hilbert_vec
void hilbertUtil(int x, int y, int xi, int xj, int yi, int yj, int level) {
    if (level <= 0) {
        hilbert_vec.push_back({x + (xi + yi)/2, y + (xj + yj)/2});
        //std::cout << x << " " << y << " <-> " << x + (xi + yi)/2 << " " << y + (xj + yj)/2 << std::endl;
    } else {
        hilbertUtil(x,           y,              yi/2,   yj/2,   xi/2,   xj/2,   level-1);
        hilbertUtil(x+xi/2,      y+xj/2,         xi/2,   xj/2,   yi/2,   yj/2,   level-1);
        hilbertUtil(x+xi/2+yi/2, y+xj/2+yj/2,    xi/2,   xj/2,   yi/2,   yj/2,   level-1);
        hilbertUtil(x+xi/2+yi,   y+xj/2+yj,      -yi/2,  -yj/2,  -xi/2,  -xj/2,  level-1);
    }
}

// draw hilbert curve using vector of coordinates as reference
void drawHilbert(cv::Mat image, int level) {

    /* for (auto i : hilbert_vec) {
        std::cout << i.x << " " << i.y << std::endl;
    }
    std::cout << "--------" << std::endl; */
    
    double dist = WINDOW_N/pow(2, level+1);
    for (int i = 0; i < hilbert_vec.size()-1; i++) {
        cv::Point p1(hilbert_vec[i].x*dist, hilbert_vec[i].y*dist);
        cv::Point p2(hilbert_vec[i+1].x*dist, hilbert_vec[i+1].y*dist);
        cv::line(image, p1, p2, cv::Scalar(0,0,255), 1);
    }
    imshow("Space Filling Curves", image);
}

// main hilbert function
void hilbert(cv::Mat image, int level, int type) {
    int origin_x = 0, origin_y = 0;

    switch (type) {
        case 0: origin_x = pow(2, level); origin_y = pow(2, level); break;
        case 1: origin_x = pow(2, level); origin_y = 0; break;
        case 2: origin_x = 0; origin_y = 0; break;
        case 3: origin_x = 0; origin_y = 0; break;
        default: return;
    }

    hilbert_vec.clear();
    hilbertUtil(origin_x, origin_y, pow(2, level+1), 0, 0, pow(2, level+1), level);
    drawHilbert(image, level);
}

int main(int argc, char** argv) {
    int level = 1;  // level of the curve, by default 1
    int type = 0;   // type of curve, from lessons: 0 = A, 1 = B, 2 = C, 3 = D

    // display initial window and hilbert curve
    cv::Mat image(WINDOW_N, WINDOW_N, CV_8UC3, cv::Scalar(0,0,0));
    hilbert(image, level, type);

    while (true) {
        // wait for keyboard input
        int key = (cv::waitKey(0) & 0xFF);

        switch (key) {
            case 'a': // if a pressed, decrease curve level
                if (level>1) level--; break;
            case 's': // if s pressed, increase curve level
                if (level<MAX_LEVEL) level++; break;
            case 'z': // if z pressed, change curve type to 0 (A)
                type = 0; break;
            case 'x': // if x pressed, change curve type to 1 (B)
                type = 1; break;
            case 'c': // if c pressed, change curve type to 2 (C)
                type = 2; break;
            case 'v': // if v pressed, change curve type to 3 (D)
                type = 3; break;
            case 'q': // if q pressed, exit
                goto exit_loop;
            default: continue;  // other inputs are ignored
        }

        // clear window and draw hilbert curve
        image = cv::Mat(WINDOW_N, WINDOW_N, CV_8UC3, cv::Scalar(0,0,0));
        hilbert(image, level, type);
    }
    
    exit_loop:;
    return 0;
}
