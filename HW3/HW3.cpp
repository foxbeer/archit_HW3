#include <iostream>
#include <thread>
#include <math.h>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
using namespace std;
using namespace std::chrono;

int result;

void crewFind(bool* islandPart, int size, int islandI) {
    for (int i = 0; i < size; i++)
        if (islandPart[i])
            result = islandI * size + i;
}

void find(int crewN, bool* island, int size) {
    auto startTime = high_resolution_clock::now();

    cout << "# of threads = " << crewN << endl;
    // thread is a crew group
    int tileCrN = ceil(size * 1.0 / crewN);

    bool** islandParts = new bool* [crewN];
    for (int i = 0; i < crewN; i++)
        islandParts[i] = new bool[tileCrN];

    int begin = 0;
    int end = tileCrN;

    for (int i = 0; i < crewN; i++) {
        for (int j = begin; j < end; j++) {
            if (j >= size)
                islandParts[i][j - begin] = false;
            else islandParts[i][j - begin] = island[j];
        }
        begin = end;
        end = begin + tileCrN;
    }

    for (int i = 0; i < crewN; i++) {
        for (int j = 0; j < tileCrN; j++)
            cout << (islandParts[i][j] ? "1 " : "0 ");
        cout << endl;
    }
    int i = 0;
    thread* crews = new thread[crewN];
    while (true)
    {
        auto t = thread(crewFind, islandParts[i], tileCrN, i);
        t.join();
        if (i == crewN - 1)
        {
            break;
        }
        i++;
    }
   /* for (int i = 0; i < crewN; i++)
        crews[i] = thread(crewFind, islandParts[i], tileCrN, i);
    for (int i = 0; i < crewN; i++)
        crews[i].join();*/

    auto stopTime = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stopTime - startTime);

    cout << "Found treasure at " << result << endl;
    cout << "Spent time: \t" << duration.count() * 1.0 / 1000000 << endl;

    for (int i = 0; i < crewN; i++)
        delete[] islandParts[i];
    delete[] islandParts;
}

void print(bool* island, int size) {
    for (int i = 0; i < size; i++)
        cout << to_string(i) << ") " << (island[i] ? "1" : "0") << endl;
    cout <<"\n";
}

bool check(string line) {
    for (int i = 0; i < line.length(); i++)
        if (line[i] < '0' || line[i] > '9')
            return false;
    return true;
}

int main()
{
    srand(time(NULL));
    string line;
    cout << "Enter the size of the island: ";
    cin >> line;
    if (!check(line)) {
        cout << "Enter a positive integer pls" << endl;
        return 0;
    }
    int islandSize = stoi(line);
    bool* island = new bool[islandSize] {};
    
    island[rand() % islandSize] = true;
 
    print(island, islandSize);

    for (int i = 1; i < 9; i++) {
        find(i, island, islandSize);
        cout << "\n";
    }
    
    delete[] island;
    return 0;
}

