// LinearProbing.cpp
// Open addressing with linear probing.
// Uses -1 to denote EMPTY, -2 to denote DELETED (dummy) per lab conventions.
// Supports insert, search, delete, and display.
// No STL used.

#include <iostream>
using namespace std;

class LinearProbing {
private:
    int* table;
    int size;
    int count; // number of actual keys stored (excludes deleted markers)

    // hash: h(key) = key % size
    int hashFunc(int key) const {
        if (key < 0) key = -key;
        return key % size;
    }

    // markers: EMPTY = -1, DELETED = -2
    static const int EMPTY = -1;
    static const int DELETED = -2;

public:
    LinearProbing(int s) {
        size = s;
        table = new int[size];
        for (int i = 0; i < size; ++i) table[i] = EMPTY;
        count = 0;
    }

    ~LinearProbing() { delete[] table; }

    // Insert key using linear probing.
    // If table is full, prints a message.
    void insertKey(int key) {
        if (count == size) {
            cout << "Insert failed: table is full\n";
            return;
        }

        int idx = hashFunc(key);
        int start = idx;

        // find slot which is EMPTY or DELETED
        while (table[idx] != EMPTY && table[idx] != DELETED) {
            idx = (idx + 1) % size;
            if (idx == start) {
                cout << "Insert failed: no slot found\n";
                return;
            }
        }

        table[idx] = key;
        ++count;
    }

    // Search for a key. Returns true if found.
    bool searchKey(int key) const {
        int idx = hashFunc(key);
        int start = idx;

        while (table[idx] != EMPTY) {
            if (table[idx] == key) return true;
            idx = (idx + 1) % size;
            if (idx == start) break;
        }
        return false;
    }

    // Delete (logical) — mark as DELETED to preserve probe chains.
    bool deleteKey(int key) {
        int idx = hashFunc(key);
        int start = idx;

        while (table[idx] != EMPTY) {
            if (table[idx] == key) {
                table[idx] = DELETED;
                --count;
                return true;
            }
            idx = (idx + 1) % size;
            if (idx == start) break;
        }
        return false;
    }

    // Display table
    void display() const {
        cout << "Linear Probing Table (size = " << size << ")\n";
        for (int i = 0; i < size; ++i) {
            cout << i << ": ";
            if (table[i] == EMPTY) cout << "EMPTY\n";
            else if (table[i] == DELETED) cout << "DELETED\n";
            else cout << table[i] << "\n";
        }
    }
};

// Demo
int main() {
    LinearProbing ht(11);

    ht.insertKey(10);
    ht.insertKey(21);
    ht.insertKey(32);
    ht.insertKey(43);
    ht.insertKey(54);

    cout << "After inserts:\n";
    ht.display();

    cout << "\nSearch 32: " << (ht.searchKey(32) ? "Found" : "Not Found") << endl;
    cout << "Delete 32: " << (ht.deleteKey(32) ? "Deleted" : "Not Found") << endl;
    cout << "Search 32 again: " << (ht.searchKey(32) ? "Found" : "Not Found") << endl;

    cout << "\nTable now:\n";
    ht.display();

    return 0;
}
