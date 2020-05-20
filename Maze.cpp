#include <bits/stdc++.h>
using namespace std;

class Cell
{
    int weight;

public:
    int x, y;
    bool isBlocked;
    Cell(int x, int y, int weight = 0)
    {
        this->x = x;
        this->y = y;
        assert(weight >= 0 && weight <= 9);
        this->weight = weight;
        isBlocked = false;
    }
    void setWeight(int val)
    {
        if (val <= 0 || val > 9)
        {
            cout << "\n\t\t\tValue Can Only Be Between 1 And 9." << endl;
            return;
        }
        if (isBlocked)
        {
            cout << "\n\t\t\tThe Cell Is Blocked." << endl;
            return;
        }
        weight = val;
    }
    int getWeight()
    {
        return weight;
    }
    char getCharCellValue()
    {
        return isBlocked ? '#' : weight + '0';
    }
};

class Board
{
    int rows, cols;
    vector<Cell> grid;
    int getRandomValue(int low, int high)
    {
        int diff = abs(high - low) + 1;
        return rand() % diff + min(low, high);
    }
    void recusriveDivision(int rs, int re, int cs, int ce)
    {
        if ((abs(rs - re) < 2) || (abs(cs - ce) < 2))
            return;
        int row = getRandomValue(rs, re);
        int col = getRandomValue(cs, ce);
        for (int i = rs; i <= re; i++)
        {
            blockCell(i, col);
            assert(getCharCellValue(i, col) == '#');
        }
        for (int j = cs; j <= ce; j++)
        {
            blockCell(row, j);
            assert(getCharCellValue(row, j) == '#');
        }
        unblockCell(getRandomValue(rs, re), col);
        unblockCell(getRandomValue(rs, re), col);
        unblockCell(row, getRandomValue(cs, ce));
        unblockCell(row, getRandomValue(cs, ce));
        recusriveDivision(rs, row - 1, cs, col - 1);
        recusriveDivision(row + 1, re, cs, col - 1);
        recusriveDivision(rs, row - 1, col + 1, ce);
        recusriveDivision(row + 1, re, col + 1, ce);
    }

public:
    Board(int rows, int cols)
    {
        srand((unsigned)time(NULL));
        this->rows = rows;
        this->cols = cols;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int val = getRandomValue(1, 9);
                Cell thisCell(i, j, val);
                grid.push_back(thisCell);
            }
        }
    }
    bool isValidCoordinate(int x, int y)
    {
        if (x < 0 || x >= rows || y < 0 || y >= cols)
            return false;
        return true;
    }
    string getStringVal(int point)
    {
        int x = point / cols, y = point % cols;
        return "(" + to_string(x + 1) + "," + to_string(y + 1) + ")";
    }
    bool isValidCell(int x, int y)
    {
        return isValidCoordinate(x, y) && (!grid.at(x * cols + y).isBlocked);
    }
    char getCharCellValue(int x, int y)
    {
        return grid.at(x * cols + y).getCharCellValue();
    }
    void blockCell(int x, int y)
    {
        grid.at(x * cols + y).isBlocked = true;
    }
    void unblockCell(int x, int y)
    {
        grid.at(x * cols + y).isBlocked = false;
    }
    void changeWeight(int x, int y, int val)
    {
        grid.at(x * cols + y).setWeight(val);
    }
    void print()
    {
        cout << "\n\n\n";
        for (int i = 0; i < rows; i++)
        {
            cout << "\t\t\t";
            for (int j = 0; j < cols; j++)
            {
                cout << getCharCellValue(i, j) << "\t";
            }
            cout << "\n\n";
        }
        cout << "\n\n";
    }
    void recusriveDivision()
    {
        recusriveDivision(0, rows - 1, 0, cols - 1);
    }
    vector<pair<int, int>> getNeighbours(int p)
    {
        pair<int, int> point({p / cols, p % cols});
        vector<pair<int, int>> neighbours;
        vector<pair<int, int>> directions;
        directions.push_back({-1, 0});
        directions.push_back({1, 0});
        directions.push_back({0, 1});
        directions.push_back({0, -1});
        for (pair<int, int> direction : directions)
        {
            if (isValidCell(point.first + direction.first, point.second + direction.second))
            {
                neighbours.push_back({point.first + direction.first, point.second + direction.second});
            }
        }
        return neighbours;
    }

    pair<int, string> getShortestDistance(pair<int, int> src, pair<int, int> destn)
    {
        int srcPoint = src.first * cols + src.second;
        int destnPoint = destn.first * cols + destn.second;
        if (grid.at(srcPoint).isBlocked || grid.at(destnPoint).isBlocked)
        {
            return {INT_MAX, "\t\t\tPATH NOT FOUND."};
        }
        if (srcPoint == destnPoint)
        {
            return {0, getStringVal(destnPoint)};
        }
        //point to key mapping
        unordered_map<int, int> distance;
        unordered_map<int, int> parent;
        for (int i = 0; i < rows * cols; i++)
        {
            distance[i] = INT_MAX;
            parent[i] = i;
        }
        distance[srcPoint] = 0;
        // pair{distance,point}
        set<pair<int, int>> pq;
        pq.insert({0, srcPoint});
        while (!pq.empty())
        {
            pair<int, int> smallestPoint = *pq.begin();
            pq.erase(pq.begin());
            vector<pair<int, int>> neighbhours = getNeighbours(smallestPoint.second);
            for (const pair<int, int> &neighbhour : neighbhours)
            {
                int thisPoint = neighbhour.first * cols + neighbhour.second;
                if (smallestPoint.first + grid.at(thisPoint).getWeight() < distance[thisPoint])
                {
                    // delete the point from pq
                    auto f = pq.find({distance[thisPoint], thisPoint});
                    if (f != pq.end())
                    {
                        pq.erase(f);
                    }
                    distance[thisPoint] = smallestPoint.first + grid.at(thisPoint).getWeight();
                    parent[thisPoint] = smallestPoint.second;
                    pq.insert({smallestPoint.first + grid.at(thisPoint).getWeight(), thisPoint});
                }
            }
        }
        stack<int> pathPoints;
        pathPoints.push(destnPoint);
        int shortestPathDistance = distance[destnPoint];
        while (parent[destnPoint] != destnPoint)
        {
            destnPoint = parent[destnPoint];
            pathPoints.push(destnPoint);
        }
        if (pathPoints.size() < 2)
        {
            return {INT_MAX, "\t\t\tPATH NOT FOUND."};
        }
        string path;
        while (!pathPoints.empty())
        {
            path.append(getStringVal(pathPoints.top()));
            path.append(" ");
            pathPoints.pop();
        }
        return {shortestPathDistance, path};
    }
};
class Play
{
    Board *board;
    int x, y;
    void Printinstructions()
    {
        cout << "\n\n\n\t\t\tWELCOME TO THE MAZE!!" << endl;
        cout << "\n\t\t\t-> You Can Calculate The Shortest Distance From A Cell To Another" << endl;
        cout << "\n\t\t\t-> You Can Check Via Different Algorithms" << endl;
        cout << "\n\t\t\t-> You Can Change The Contents Of Maze As Well" << endl;
        cout << "\n\t\t\t-> '#' Represents Blocked Cell, Numeric Value Means Weight Of That Cell" << endl;
        cout << "\n\t\t\t-> Weight Of A Cell Can Only Be Between 0 - 9" << endl;
        cout << "\n\t\t\t-> Start By Entering Dimensions Of The Maze (M x N)";
    }
    bool isValidDimension(int x, int y)
    {
        if (x <= 0 || y <= 0 || x > this->x || y > this->y)
            return false;
        return true;
    }
    bool isValidWeight(int weight)
    {
        return weight >= 1 && weight <= 9;
    }
    void printMenu()
    {
        cout << "\n\t\t\tPress 1 To Find Distances." << endl;
        cout << "\t\t\tPress 2 To Unblock A Cell" << endl;
        cout << "\t\t\tPress 3 To Block A Cell" << endl;
        cout << "\t\t\tPress 4 To Change Cell Weight" << endl;
        cout << "\t\t\tPress 5 To Reprint The Matrix" << endl;
        cout << "\t\t\tPress 6 To Exit" << endl;
    }
    bool isValidChoice(int choice)
    {
        return ((choice >= 1) && (choice <= 6));
    }
    pair<int, int> takeValidInputCoordinates()
    {
        int x, y;
        do
        {
            cout << "\n\t\t\tEnter Co-ordinates : ";
            cin >> x >> y;
        } while (!isValidDimension(x, y));
        return {x - 1, y - 1};
    }
    bool cellWeight()
    {
        pair<int, int> point = takeValidInputCoordinates();
        int weight;
        while (true)
        {
            cout << "\n\t\t\tEnter The New Weight : ";
            cin >> weight;
            if (isValidWeight(weight))
                break;
        }
        board->changeWeight(point.first, point.second, weight);
        return true;
    }
    bool exit()
    {
        cout << "\n\n\t\t\tTHANK YOU FOR PLAYING\n\n" << endl;
        return false;
    }
    bool unblock()
    {
        pair<int, int> point = takeValidInputCoordinates();
        board->unblockCell(point.first, point.second);
        board->print();
        return true;
    }
    bool block()
    {
        pair<int, int> point = takeValidInputCoordinates();
        board->blockCell(point.first, point.second);
        board->print();
        return true;
    }

    bool findDistance()
    {
        cout << "\n\t\t\tEnter Source Coordinates : ";
        pair<int, int> src = takeValidInputCoordinates();
        cout << "\n\t\t\tEnter Destination Coordinates : ";
        pair<int, int> dstn = takeValidInputCoordinates();
        pair<int, string> path = board->getShortestDistance(src, dstn);
        cout << "\n\t\t\tPath Is :- ";
        cout << path.second << "\n\t\t\tDistance : " << path.first << endl;
        return true;
    }
    bool choiceFxn(int choice)
    {
        if (choice == 0)
        {
            return findDistance();
        }
        else if (choice == 1)
        {
            return unblock();
        }
        else if (choice == 2)
        {
            return block();
        }
        else if (choice == 3)
        {
            return cellWeight();
        }
        else if (choice == 4)
        {
            board->print();
            return true;
        }
        return exit();
    }

public:
    Play()
    {
        Printinstructions();
        int x, y;
        do
        {
            cout << "\n\n\n\t\t\tEnter Valid Dimensions : ";
            cin >> x >> y;
            this->x = x;
            this->y = y;
        } while (!isValidDimension(x, y));
        board = new Board(x, y);
        board->recusriveDivision();
        board->print();
    }
    void play()
    {
        bool flag = true;
        while (flag)
        {
            printMenu();
            int choice;
            while (true)
            {
                cout << "\n\n\t\t\tEnter Valid Choice : ";
                cin >> choice;
                if (isValidChoice(choice))
                    break;
            }
            flag = choiceFxn(choice - 1);
        }
    }
};
void play()
{
    Play p;
    p.play();
}
int main()
{
    play();
    return 0;
}
