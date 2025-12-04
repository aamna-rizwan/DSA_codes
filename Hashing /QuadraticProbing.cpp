// QuadraticProbing.cpp
// Open addressing with quadratic probing.
// Probe sequence: (h + i^2) % size for i = 0,1,2,...
// Uses markers: EMPTY = -1, DELETED = -2
// Insert, search, delete included.

#include <iostream>
using namespace std;

class QuadraticProbing {
private:
    int* table;
    int size;
    int count;

    static const int EMPTY = -1;
    static const int DELETED = -2;

    int hashFunc(int key) const {
        if (key < 0) key = -key;
        return key % size;
    }

public:
    QuadraticProbing(int s) {
        size = s;
        table = new int[size];
        for (int i = 0; i < size; ++i) table[i] = EMPTY;
        count = 0;
    }

    ~QuadraticProbing() { delete[] table; }

    void insertKey(int key) {
        if (count == size) {
            cout << "Insert failed: table full\n";
            return;
        }

        int base = hashFunc(key);
        int i = 0;
        int idx;
        while (i < size) {
            idx = (base + i * i) % size;
            if (table[idx] == EMPTY || table[idx] == DELETED) {
                table[idx] = key;
                ++count;
                return;
            }
            ++i;
        }
        cout << "Insert failed: could not find slot\n";
    }

    bool searchKey(int key) const {
        int base = hashFunc(key);
        int i = 0;
        int idx;
        while (i < size) {
            idx = (base + i * i) % size;
            if (table[idx] == EMPTY) return false; // empty stops search
            if (table[idx] == key) return true;
            ++i;
        }
        return false;
    }

    bool deleteKey(int key) {
        int base = hashFunc(key);
        int i = 0;
        int idx;
        while (i < size) {
            idx = (base + i * i) % size;
            if (table[idx] == EMPTY) return false;
            if (table[idx] == key) {
                table[idx] = DELETED;
                --count;
                return true;
            }
            ++i;
        }
        return false;
    }

    void display() const {
        cout << "Quadratic Probing Table (size = " << size << ")\n";
        for (int i = 0; i < size; ++i) {
            cout << i << ": ";
            if (table[i] == EMPTY) cout << "EMPTY\n";
            else if (table[i] == DELETED) cout << "DELETED\n";
            else cout << table[i] << "\n";
        }
    }
};

// Demo main
int main() {
    QuadraticProbing ht(11);

    ht.insertKey(5);
    ht.insertKey(16);
    ht.insertKey(27);
    ht.insertKey(38);
    ht.insertKey(49);

    cout << "After inserts:\n";
    ht.display();

    cout << "\nSearch 27: " << (ht.searchKey(27) ? "Found" : "Not Found") << endl;
    cout << "Delete 27: " << (ht.deleteKey(27) ? "Deleted" : "Not Found") << endl;
    cout << "Search 27 again: " << (ht.searchKey(27) ? "Found" : "Not Found") << endl;

    cout << "\nFinal table:\n";
    ht.display();

    return 0;
}
