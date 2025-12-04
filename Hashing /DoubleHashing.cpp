// DoubleHashing.cpp
// Double hashing: probe = (h1 + i * h2) % size
// h1 = key % size
// h2 = R - (key % R) where R is a prime smaller than size (choose R=previous prime)
// Uses EMPTY = -1, DELETED = -2 markers
// Insert, search, delete included.

#include <iostream>
using namespace std;

class DoubleHash {
private:
    int* table;
    int size;
    int count;
    static const int EMPTY = -1;
    static const int DELETED = -2;

    int h1(int key) const {
        if (key < 0) key = -key;
        return key % size;
    }

    // Choose h2 such that it's non-zero and relatively prime to size.
    // A common choice: h2 = R - (key % R) where R is prime < size (here pick R=7 or nearest)
    int h2(int key) const {
        int R = 7;
        if (R >= size) R = size - 1;
        if (R < 2) R = 2;
        return R - (key % R);
    }

public:
    DoubleHash(int s) {
        size = s;
        table = new int[size];
        for (int i = 0; i < size; ++i) table[i] = EMPTY;
        count = 0;
    }

    ~DoubleHash() { delete[] table; }

    void insertKey(int key) {
        if (count == size) {
            cout << "Insert failed: table full\n";
            return;
        }
        int base1 = h1(key);
        int step = h2(key);
        int i = 0;
        int idx;
        while (i < size) {
            idx = (base1 + i * step) % size;
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
        int base1 = h1(key);
        int step = h2(key);
        int i = 0;
        int idx;
        while (i < size) {
            idx = (base1 + i * step) % size;
            if (table[idx] == EMPTY) return false;
            if (table[idx] == key) return true;
            ++i;
        }
        return false;
    }

    bool deleteKey(int key) {
        int base1 = h1(key);
        int step = h2(key);
        int i = 0;
        int idx;
        while (i < size) {
            idx = (base1 + i * step) % size;
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
        cout << "Double Hashing Table (size = " << size << ")\n";
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
    DoubleHash ht(11);

    ht.insertKey(10);
    ht.insertKey(22);
    ht.insertKey(31);
    ht.insertKey(4);
    ht.insertKey(15);

    cout << "After inserts:\n";
    ht.display();

    cout << "\nSearch 31: " << (ht.searchKey(31) ? "Found" : "Not Found") << endl;
    cout << "Delete 31: " << (ht.deleteKey(31) ? "Deleted" : "Not Found") << endl;
    cout << "Search 31 again: " << (ht.searchKey(31) ? "Found" : "Not Found") << endl;

    cout << "\nFinal table:\n";
    ht.display();

    return 0;
}
