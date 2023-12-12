#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

enum State {movingLeft,
        movingRight,
        movingUp,
        movingDown,
};


class Coordinates {
private:
    int x;
    int y;
public:
    Coordinates(int newX, int newY) {
        x = newX;
        y = newY;
    }
    int getX() {
        return x;
    }
    int getY() {
        return y;
    }
};

int interiorPoints(int area, int boundary_points) {
    int inter_points = area + 1 - (boundary_points/2);
    return inter_points;
}

int area(list<Coordinates> &points) {
    int areaVal = 0;
    list<Coordinates>::iterator current, next;
    current = points.begin();
    next = points.begin();
    int firstX = current->getX();
    int firstY = current->getY();
    for (next++; next != points.end(); current++, next++) {
        int x1 = current->getX();
        int y2 = next->getY();
        int x2 = next->getX();
        int y1 = current->getY();
        int currEval = (x1 * y2) - (x2 * y1);
        areaVal += currEval;
    }

    int x2 = current->getX();
    int y2 = current->getY();

    int currEval = (x2 * firstY) - (firstX * y2);

    areaVal += currEval;

    return areaVal/2;
}

int main(int argc, char **argv) {
    ifstream input_file;
    input_file.open("input.txt");
    string line;

    list<string> grid;
    while(getline(input_file, line)) {
        grid.push_back(line);
    }
    input_file.close();

    list<string>::iterator startLine;
    int startPos = 0;
    bool startFound = false;

    int height=0;

    list<Coordinates> points;

    // find start position
    for (startLine = grid.begin(); startLine != grid.end() && !startFound; startLine++) {
        for (startPos=0; startPos < startLine->length() && !startFound; startPos++) {
            if ((*startLine)[startPos] == 'S') {
                startFound = true;
            }
        }
        height++;
    }
    startLine--;
    startPos--;


    State state;
    State startState;
    State endState;
    int distance = 0;
    list<string>::iterator row = startLine, rowAbove = startLine, rowBelow = startLine;
    rowAbove--;
    rowBelow++;
    int column = startPos;
    if ((column < row->length()-1) && ((*row)[column+1] == '-' || (*row)[column+1] == 'J' || (*row)[column+1] == '7')) {
        state = movingRight;
        column++;
    }
    else if ((column > 0) && ((*row)[column-1] == '-' || (*row)[column-1] == 'L' || (*row)[column-1] == 'F')) {
        state = movingLeft;
        column--;
    }
    else if ((row != grid.begin()) && ((*rowAbove)[column] == '|' || (*rowAbove)[column] == '7' || (*rowAbove)[column] == 'F')) {
        state = movingUp;
        row--;
        height--;
    }
    else if (rowBelow != grid.end() && ((*rowBelow)[column] == '|' || (*rowBelow)[column] == 'L' || (*rowBelow)[column] == 'J')) {
        state = movingDown;
        row++;
        height++;
    }
    else {
        cerr << "No viable path found!\n";
    }
    startState = state;
    distance++;

    // Part 1

    while ((*row)[column] != 'S') {
        switch(state) {
            case movingLeft:
                if ((*row)[column] == '-') {
                    column--;
                }
                else if ((*row)[column] == 'F') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingDown;
                    row++;
                    height++;
                }
                else if ((*row)[column] == 'L') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingUp;
                    row--;
                    height--;
                }
                break;
            case movingRight:
                if ((*row)[column] == '-') {
                    column++;
                }
                else if ((*row)[column] == '7') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingDown;
                    row++;
                    height++;
                }
                else if ((*row)[column] == 'J') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingUp;
                    row--;
                    height--;
                }
                break;
            case movingUp:
                if ((*row)[column] == '|') {
                    row--;
                    height--;
                }
                else if ((*row)[column] == '7') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingLeft;
                    column--;
                }
                else if ((*row)[column] == 'F') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingRight;
                    column++;
                }
                break;
            case movingDown:
                if ((*row)[column] == '|') {
                    row++;
                    height++;
                }
                else if ((*row)[column] == 'J') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingLeft;
                    column--;
                }
                else if ((*row)[column] == 'L') {
                    points.push_back(Coordinates(column+1, height+1));
                    state = movingRight;
                    column++;
                }
                break;
        }
        distance++;
    }
    endState = state;

    if (startState != endState) {
        points.push_front(Coordinates(column+1, height+1));
    }

    cout << "Part 1: Steps = " << (distance/2) << endl;

    // Part 2


    int shape_area = area(points);
    if (shape_area < 0) {
        shape_area = shape_area * -1;
    }
    int innerTiles = interiorPoints(shape_area, distance);

    cout << "Part 2: Inner tiles = " << innerTiles << endl;

    return 0;
}
