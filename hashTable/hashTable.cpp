﻿#include <iostream>
#include <string>

using namespace std;

template <typename T>
struct Node {
    struct Node<T>* prev = nullptr;
    T data;
    struct Node<T>* next = nullptr;
};

template <typename T, typename Y>
struct LinkedList {
    struct Node<T>* head = nullptr;
    struct Node<T>* tail = nullptr;
    int size = 0;

    void pushBack(T data) {

        // tworzenie węzła
        Node<T>* n = new Node<T>();
        n->data = data;

        n->prev = tail;

        // jeśli nie został wcześniej dodany żaden element
        if (tail != nullptr) {
            tail->next = n;
        }
        else {
            head = n;
        }

        tail = n;

        // zwiększenie informacji o ilości elementów znajdujących się w liście
        size++;
    }

    bool isEmpty() {
        if (head == nullptr)
            return true;

        return false;
    }

    Node<T>* search(string key) {
        Node<T>* n = head;

        while (n != nullptr) {

            if (n->data.key == key) return n;

            n = n->next;
        }

        return nullptr;
    }

    bool searchAndDelete(string key) {
        Node<T>* n = search(key);

        if (n == nullptr) return false;
        else {
            if (n == head) {
                if (n->next != nullptr) {
                    head = n->next;
                    head->prev = nullptr;
                }
                else {
                    head = tail = nullptr;
                }

            }
            else if (n == tail) {
                tail = n->prev;
                tail->next = nullptr;
            }
            else {
                n->next->prev = n->prev;
                n->prev->next = n->next;
            }

            delete n;
            size--;

            return true;
        }
    }


    void clear() {
        Node<T>* n = head;
        Node<T>* lastFound = nullptr;

        if (head == nullptr) {
            return;
        }

        while (true) {
            lastFound = n;

            n = n->next;

            if (n == nullptr)
                break;

            delete n->prev;

            size--;
        }

        delete lastFound;

        // ustawienie tail i head na nullptr
        head = tail = nullptr;
    }

    string str(string(*func)(Y)) {
        string s = "";

        Node<T>* n = head;

        while (n != nullptr) {
            s += n->data.key + " -> " + func(n->data.data) + "; \n";
            n = n->next;
        }

        return s;

    }

    int getSize() {
        return size;
    }
};

template <typename T>
struct Item {
    string key;
    T data;
};

struct simple_object {
    string surname;
    int age;
};

string so_str(simple_object so) {
    return so.surname + ' ' + to_string(so.age);
}

const float FILL_LIMIT = 0.75;

template <typename T>
struct HashTable {

    int size = 0;
    int capacity = 10;
    LinkedList<Item<T>, T>* array = new LinkedList<Item<T>, T>[capacity];
    //T* array = new T[capacity];

    int hash_func(string key) {
        int q = key.length();
        int hash = 0;

        for (int i = 0; i < q; i++) {
            hash += key[i] * pow(31, (q - 1 - i));
        }

        hash = abs(hash % capacity);

        return hash;
    }

    void rehash() {
        int old_capacity = capacity;
        capacity += 10;

        LinkedList<Item<T>, T>* tmp_array = new LinkedList<Item<T>, T>[capacity];

        for (int i = 0; i < old_capacity; i++) {
            if (array[i].head != nullptr) {
                Node<Item<T>>* n = array[i].head;
                string key = n->data.key;

                int hash = hash_func(key);

                tmp_array[hash] = array[i];
            }
        }
        delete array;
        array = tmp_array;
    }

    void insert(string key, T data) {

        Item<T>* is = search(key);
        if (is != nullptr) {
            is->data = data;
        }
        else {
            Item<T> it = Item<T>{ key, data };
            int hash = hash_func(key);
            array[hash].pushBack(it);
            size++;
        }

        if (capacity * FILL_LIMIT < size) {
            rehash();
        }

    }

    Item<T>* search(string key) {
        int hash = hash_func(key);
        Node<Item<T>>* n = array[hash].search(key);

        if (n != nullptr)
            return &n->data;
        else
            return nullptr;
    }

    bool remove(string key) {
        int hash = hash_func(key);
        bool flag = array[hash].searchAndDelete(key);

        if (flag)
            size--;

        return flag;
    }

    void clear() {
        for (int i = 0; i < capacity; i++) {
            array[i].clear();
        }

        delete[] array;
        size = 0;
        capacity = 10;
        array = new LinkedList<Item<T>>[capacity];
    }

    string stats() {
        int min_list_size = array[0].getSize();
        int max_list_size = array[0].getSize();
        int nonempty_lists = 0;
        int item_counter = 0;

        for (int i = 0; i < capacity; i++) {
            if (!array[i].isEmpty())
            {
                nonempty_lists++;
                int current_list_size = array[i].getSize();
                item_counter += current_list_size;

                if (current_list_size > max_list_size)
                    max_list_size = current_list_size;
                else if (current_list_size < max_list_size)
                    min_list_size = current_list_size;

            }
                
        }

        float average_list_size = item_counter / static_cast<float>(capacity);
        return "\tlist min size: " + to_string(min_list_size) + "\n\tlist max size: " + to_string(max_list_size) + "\n\tnonempty lists: " + to_string(nonempty_lists) + "\n\taverage list size: " + to_string(average_list_size);
    }

    string str() {
        string s = "Hash table: \n\tCurrent size: " + to_string(capacity) + "\n\tMax size: " + to_string(INT_MAX) + "\n\tTable:\n\t{\n";

        for (int i = 0; i < capacity; i++) {
            if (!array[i].isEmpty()) {
                s += "\t\t" + to_string(i) + ": " + array[i].str(so_str);


            }
        }

        s += "\t}\nStats:\n" + stats();

        return s;
    }

};






int main()
{
    HashTable<simple_object>* ht = new HashTable<simple_object>();
    simple_object o1{ "Szabat", 21 };
    simple_object o2{ "Marcin", 33 };
    ht->insert("pies", o1);
    ht->insert("kotek", o2);

    //ht->clear();
    //cout << ht->remove("pies");
    //Item<simple_object>* it = ht->search("pies");
    //if (it == nullptr) cout << "NULLPTR";
    //cout << so_str(it->data);

    cout << ht->str();
    delete ht;
}

