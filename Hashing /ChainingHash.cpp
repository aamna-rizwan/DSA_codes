// ChainingHash.cpp
// Separate chaining implementation (linked list at each index).
// - Inserts at head as required by the lab manual.
// - Supports insert, search, delete, display.
// - No STL used. Only iostream.
//
// Concepts included (per manual):
// - Key, Hash Function (h(key) = key % tableSize), Hash Table
// - Collision resolution: Separate Chaining
// - Insert at head (explicit in manual)

#include <iostream>
using namespace std;

struct Node {
    int key;
    Node* next;
};

// Hash table class using Separate Chaining
class ChainingHash {
private:
    Node** table;   // array of Node* (each is head of linked list)
    int size;       // number of slots

    // Simple hash function per manual: h(key) = key % size
    int hashFunc(int key) const {
        if (key < 0) key = -key;
        return key % size;
    }

public:
    ChainingHash(int s) {
        size = s;
        table = new Node*[size];
        for (int i = 0; i < size; ++i) table[i] = NULL;
    }

    ~ChainingHash() {
        // free all lists
        for (int i = 0; i < size; ++i) {
            Node* cur = table[i];
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
        }
        delete[] table;
    }

    // Insert at head (manual requires insertion at head)
    void insertKey(int key) {
        int idx = hashFunc(key);
        Node* newNode = new Node;
        newNode->key = key;
        newNode->next = table[idx]; // head insertion
        table[idx] = newNode;
    }

    // Search: returns true if found
    bool searchKey(int key) const {
        int idx = hashFunc(key);
        Node* cur = table[idx];
        while (cur) {
            if (cur->key == key) return true;
            cur = cur->next;
        }
        return false;
    }

    // Delete (remove) first occurrence of key in chain
    bool deleteKey(int key) {
        int idx = hashFunc(key);
        Node* cur = table[idx];
        Node* prev = NULL;

        while (cur) {
            if (cur->key == key) {
                if (prev == NULL) {
                    // deleting head
                    table[idx] = cur->next;
                } else {
                    prev->next = cur->next;
                }
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false; // not found
    }

    // Display full table
    void display() const {
        cout << "Hash Table (Chaining). Size = " << size << "\n";
        for (int i = 0; i < size; ++i) {
            cout << i << ": ";
            Node* cur = table[i];
            while (cur) {
                cout << cur->key << " -> ";
                cur = cur->next;
            }
            cout << "NULL\n";
        }
    }
};

// Small demo / test
int main() {
    // choose table size (can be any integer)
    ChainingHash ht(11);

    // Insert keys
    ht.insertKey(15);
    ht.insertKey(26);
    ht.insertKey(37);
    ht.insertKey(26); // duplicate allowed; appears at head of chain
    ht.insertKey(48);

    cout << "After inserts:\n";
    ht.display();

    cout << "\nSearch 26: " << (ht.searchKey(26) ? "Found" : "Not Found") << endl;
    cout << "Delete 26: " << (ht.deleteKey(26) ? "Deleted" : "Not Found") << endl;
    cout << "Search 26 again: " << (ht.searchKey(26) ? "Found" : "Not Found") << endl;

    cout << "\nFinal table:\n";
    ht.display();

    return 0;
}
