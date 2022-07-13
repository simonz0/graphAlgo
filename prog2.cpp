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
#include <algorithm>
#include <queue>

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
const int INVALID_TIME = -1;
const int INVALID_COMP = -1;


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
    int dtime;
    int ftime;
    int comp;
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
            Node tempNode{i, j, W, tempChar, INVALID_DIST, INVALID_TIME, INVALID_TIME, INVALID_COMP};
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
vector<Node> findNeighbours(vector<vector<Node>>& field, Node& node) {
    // using cs enumeration
    int x = (node.x) - 1;
    int y = (node.y) - 1;
    int size = field.size();
    vector<Node> neighbours;
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if ((x + i >= 0) && (x + i <= (size - 1)) && (y + j >= 0) && (y + j <= (size - 1))
                && (((i != j))) && (i != (0-j))) {
                neighbours.emplace_back(field[x + i][y + j]);
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
void findTimeFireOut(vector<vector<Node>>& field, int& startX, int& startY, int& cornNum) {
    queue<Node> q;

    // starting node
    field[startX][startY].color = G;
    field[startX][startY].dist = 1;
    q.push(field[startX][startY]);

    while(q.size() != 0) {
        Node u = q.front();
        q.pop();
        vector<Node> neighbours = findNeighbours(field, u);
        int size = neighbours.size();
        for (int i = 0; i < size; i++) {
            if ((field[neighbours[i].x - 1][neighbours[i].y - 1].color == W) && (field[neighbours[i].x - 1][neighbours[i].y - 1].block == c)) {
                field[neighbours[i].x - 1][neighbours[i].y - 1].color = G;
                field[neighbours[i].x - 1][neighbours[i].y - 1].dist = field[u.x - 1][u.y-1].dist + 1;
                q.push(field[neighbours[i].x - 1][neighbours[i].y - 1]);
            }
        }
        field[u.x - 1][u.y - 1].block = f;
        field[u.x - 1][u.y - 1].color = B;
        (cornNum)--;
    }
}

/**
 * @brief DFS Algo recursive body
 * 
 * @param field 
 * @param v 
 * @param time 
 */
void DFSVisit(vector<vector<Node>>& field, Node& v, int& time) {
    field[v.x - 1][v.y - 1].color = G;
    (time)++;
    field[v.x - 1][v.y - 1].dtime = time;

    vector<Node> neighbours = findNeighbours(field, v);
    for (auto  &neighbour : neighbours) {
        if (field[neighbour.x - 1][neighbour.y - 1].color == W && field[neighbour.x - 1][neighbour.y - 1].block != w) {
            DFSVisit(field, field[neighbour.x - 1][neighbour.y - 1], time);
        }
    }
    field[v.x - 1][v.y - 1].color = B;
    (time)++;
    field[v.x - 1][v.y - 1].ftime = time;
}

/**
 * @brief DFS main function body
 * 
 * @param field 
 */
void depthFirstSearch(vector<vector<Node>>& field) {
    int size = field.size();
    int time = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (field[i][j].color == W && field[i][j].block != w) {
                DFSVisit(field, field[i][j], time);
            }
        }
    }
}

/**
 * @brief SCC algo recursive body
 * 
 * @param field 
 * @param v 
 * @param sccCount 
 */

void SCCVisit(vector<vector<Node>>& field, Node& v, int& sccCount) {
    field[v.x - 1][v.y - 1].color = G;
    field[v.x - 1][v.y - 1].comp = sccCount;
    vector<Node> neighbours = findNeighbours(field, v);
    int size = neighbours.size();
    for (int i = 0; i < size; i++) {
        if (neighbours[i].color == W && neighbours[i].block != w) {
            SCCVisit(field, field[neighbours[i].x - 1][neighbours[i].y - 1], sccCount);
        }
    }
    field[v.x - 1][v.y - 1].color = B;
}

/**
 * @brief compare two nodes
 * 
 * @param n1 
 * @param n2 
 * @return true 
 * @return false 
 */
bool compareNode(Node& n1, Node& n2) {
    return n1.ftime < n2.ftime;
}

/**
 * @brief scc algo main body
 * 
 * @param field 
 */
void scc(vector<vector<Node>>& field) {
    depthFirstSearch(field);
    vector<Node> sorted;
    int s = field.size();
    for (int i = 0; i < s; i++) {
        for (int j = 0; j < s; j++) {
            sorted.emplace_back(field[i][j]);
        }
    }
    sort(sorted.begin(), sorted.end(), compareNode);

    int sccCount = 0;
    for (int i =  0; i < s; i++) {
        for (int j = 0 ; j < s; j++) {
            field[i][j].color = W;
        }
    }
    int size = sorted.size();
    for (int i = size - 1; i > 0; i--) {
        Node v = sorted[i];
        if (field[v.x - 1][v.y - 1].color == W && field[v.x - 1][v.y - 1].block != w) {
            sccCount++;
            SCCVisit(field, field[v.x - 1][v.y - 1], sccCount);
        }
    }
    cout << sccCount << endl;
}



int main() {
    // prepare graphs
    vector<vector<Node>> field;
    int cornNum = 0;
    generateGraph(field, cornNum);

    // using modified BFS to find time of fire to be over
    int startX = 0;
    int startY = 0;
    cin >> startX;
    cin >> startY;
    startX--;
    startY--;
    findTimeFireOut(field, startX, startY, cornNum);

    // print max dist
    int time = -1;
    for (auto& row : field) {
        for (auto& item : row) {
            if (item.dist > time) {
                time = item.dist;
            }
        }
    }
    cout << time << endl;

    // print remaining num of corns
    cout << cornNum << endl;


    // run strongly connected components algo
    // first set all node to be white
    int size = field.size();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            field[i][j].color = W;
        }
    }

    // run scc
    scc(field);
}
