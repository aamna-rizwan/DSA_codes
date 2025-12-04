// RehashingOpenAddressing.cpp
// Linear probing with automatic rehash when load factor >= 0.7
// Rehash steps implemented according to the lab manual:
// 1) calculate new size (next prime >= 2 * oldSize)
// 2) create new table
// 3) re-insert all existing keys (skip EMPTY/DELETED)
// 4) replace old table, free old memory

#include <iostream>
using namespace std;

class RehashingLinear {
private:
    int* table;
    int size;
    int count; // number of actual keys (not including deleted markers)
    static const int EMPTY = -1;
    static const int DELETED = -2;

    // hash function
    int hashFunc(int key) const {
        if (key < 0) key = -key;
        return key % size;
    }

    // check prime
    bool isPrime(int n) const {
        if (n <= 1) return false;
        if (n <= 3) return true;
        if (n % 2 == 0) return false;
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }

    // next prime >= n
    int nextPrime(int n) const {
        while (!isPrime(n)) ++n;
        return n;
    }

    // current load factor
    double loadFactor() const {
        return (double)count / (double)size;
    }

    // rehash to new prime size (approx double)
    void rehash() {
        int oldSize = size;
        int* oldTable = table;

        int target = size * 2;
        int newSize = nextPrime(target);
        // allocate new table
        table = new int[newSize];
        size = newSize;
        for (int i = 0; i < size; ++i) table[i] = EMPTY;
        count = 0;

        // re-insert old keys
        for (int i = 0; i < oldSize; ++i) {
            if (oldTable[i] != EMPTY && oldTable[i] != DELETED) {
                insertKey(oldTable[i]); // uses current insert (linear probing)
            }
        }

        delete[] oldTable;
        // Note: After rehash, load factor will be lowered.
        cout << "Rehashed to new size " << size << endl;
    }

public:
    RehashingLinear(int initialSize) {
        int s = initialSize;
        if (s < 3) s = 3;
        size = nextPrime(s);
        table = new int[size];
        for (int i = 0; i < size; ++i) table[i] = EMPTY;
        count = 0;
    }

    ~RehashingLinear() { delete[] table; }

    void insertKey(int key) {
        // If inserting will cause load factor >= 0.7, rehash first
        if ((count + 1) > (int)(0.7 * size)) {
            rehash();
        }

        int idx = hashFunc(key);
        int start = idx;
        while (table[idx] != EMPTY && table[idx] != DELETED) {
            idx = (idx + 1) % size;
            if (idx == start) {
                cout << "Insert failed: table full\n";
                return;
            }
        }
        table[idx] = key;
        ++count;
    }

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

    void display() const {
        cout << "Rehashing Linear Table (size = " << size << ", count = " << count
             << ", loadFactor = " << loadFactor() << ")\n";
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
    // initial small size to demonstrate rehashing
    RehashingLinear ht(5);

    // Insert enough items to trigger rehash
    ht.insertKey(10);
    ht.insertKey(21);
    ht.insertKey(32);
    ht.insertKey(43); // should trigger rehash on next insert possibly
    ht.insertKey(54); // this may trigger rehash

    cout << "After inserts (with rehash when needed):\n";
    ht.display();

    cout << "\nSearch 21: " << (ht.searchKey(21) ? "Found" : "Not Found") << endl;
    cout << "Delete 21: " << (ht.deleteKey(21) ? "Deleted" : "Not Found") << endl;
    cout << "Search 21 again: " << (ht.searchKey(21) ? "Found" : "Not Found") << endl;

    cout << "\nFinal table:\n";
    ht.display();

    return 0;
}
