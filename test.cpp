/**
 * @file prog2.cpp
 * @author Simon Zhang (simon.zhang@uwaterloo.ca)
 * @brief 
 * @version 0.1
 * @date 2022-07-06
 * 
 * @copyright Copyright (c) 2022 belongs to Simon Zhang @ University of Waterloo
 * 
 */


#include <iostream>
#include <vector>

using namespace std;

/**
 * @brief constants used in the program
 * 
 */

const int W = 1; // white
const int G = 2; // gray
const int B = 3; // black


const char c = 'c'; // corn
const char w = 'w'; // water
const char f = 'f'; // fired

const int INVALID_DIST = -1;


/**
 * @brief Each node in the graph
 * 
 */
class Node {
    public:
    int x;
    int y;
    int color;
    char block;
    int dist;
};

ostream& operator<<(ostream& os, const Node& node) {
    os << node.block << "{x:" << node.x << "y:" << node.y << "c:" << node.color << "}";
    return os;
}

/**
 * @brief generate a n*n table to represent graph;
 * 
 * @param field 
 */
void generateGraph(vector<vector<Node>>& field, int& cornNum) {
    int len = 0;
    cin >> len;
    for (int i = 1; i <= len; i++) {
        vector<Node> tempRow;
        for (int j = 1; j <= len; j++) {
            char tempChar = 0;
            cin >> tempChar;
            Node tempNode{i, j, W, tempChar, INVALID_DIST};
            if (tempChar == c) {
                cornNum++;
            }
            tempRow.emplace_back(tempNode);
        }
        field.emplace_back(tempRow);
    }
}

/**
 * @brief find neighbours of given node
 * 
 * @param field 
 * @param node 
 * @return vector<Node*> 
 */
vector<Node*> findNeighbours(vector<vector<Node>>& field, Node* node) {
    // using cs enumeration
    int x = (node->x) - 1;
    int y = (node->y) - 1;
    int size = field.size();
    vector<Node*> neighbours;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ((x + i >= 0) && (x + i <= (size - 1)) && (y + j >= 0) && (y + j <= (size - 1))
            && (((i != j))) && (i != (0-j))) {
                neighbours.emplace_back(&field[y + j][x + i]);
            }
        }
    }
    return neighbours;
}

/**
 * @brief Using modified BFS to go through the graph start with (x, y)
 * 
 * @param field 
 * @param start_x 
 * @param start_y 
 */
void findTimeFireOut(vector<vector<Node>>& field, int* startX, int* startY, int* cornNum) {
    vector<Node*> q;

    // starting node
    field[*startY][*startX].color = G;
    field[*startY][*startX].dist = 1;
    q.emplace_back(&field[*startY][*startX]);

    while(q.size() != 0) {
        Node* u = q.front();
        q.erase(q.begin());
        vector<Node*> neighbours = findNeighbours(field, u);
        int size = neighbours.size();
        for (int i = 0; i < size; i++) {
            if ((neighbours[i]->color == W) && (neighbours[i]->block == c)) {
                neighbours[i]->color = G;
                neighbours[i]->dist = u->dist + 1;
                q.emplace_back(neighbours[i]);
            }
        }
        // cout << "u = " << *u << endl;
        u->block = f;
        u->color = B;
        (*cornNum)--;
    }
}




int main() {
    
    // prepare graphs
    vector<vector<Node>> field;
    int cornNum = 0;
    generateGraph(field, cornNum);
    int size = field.size();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << i << " " << j << endl;
            cout << field[i][j] << endl;
        }
    }
    /*
    // These two lines remind me first is col and second is row
    // cout << field[1][2].x <<" " << field[1][2].y << endl;

    // using modified BFS to find time of fire to be over
    int startX = 0;
    int startY = 0;
    cin >> startX;
    cin >> startY;

    // using normal cs enumeration
    startX--;
    startY--;
    findTimeFireOut(field, &startX, &startY, &cornNum);
    
    // return max dist
    int time = -1;
    for (auto row : field) {
        for (auto item : row) {
            if (item.dist > time) {
                time = item.dist;
            }
        }
    }
    cout << time << endl;

    // find remaining num of corns
    cout << cornNum << endl;

    // keep firing until no corn
    int fireNum = 1;
    int size = field.size();
    while(cornNum != 0) {
        for (int i = 0; i <  size; i++) {
            for (int j = 0; j < size; j++) {
                if (field[i][j].block == c) {
                    int x = field[i][j].x - 1;
                    int y = field[i][j].y - 1;
                    findTimeFireOut(field, &x, &y, &cornNum);
                }
            }
        }
        fireNum++;
    }
    cout << fireNum << endl;
    */
}
