#include <iostream>
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

            if(n->next != nullptr)
                s += "\t\t   ";

            n = n->next;
        }

        return s;

    }

    int getSize() {
        return size;
    }

    Node<T>* get(int i) {
        Node<T>* n = head;

        for (int j = 0; j < i; j++) {
            n = n->next;
        }

        return n;
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
        long long hash = 0;

        for (int i = 0; i < q; i++) {
            hash += key[i] * pow(31, (q - 1 - i));
        }

        hash = abs(hash % capacity);
        return hash;
    }

    void rehash() {
        int old_capacity = capacity;
        capacity *= 2;

        LinkedList<Item<T>, T>* tmp_array = new LinkedList<Item<T>, T>[capacity];

        for (int i = 0; i < old_capacity; i++) {
            if (!array[i].isEmpty()) {
                for (int j = 0; j < array[i].getSize(); j++) {
                    Node<Item<T>>* n = array[i].get(j);
                    string key = n->data.key;
                    int hash = hash_func(key);
                    tmp_array[hash].pushBack(n->data);
                }
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
        array = new LinkedList<Item<T>, T>[capacity];
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
        return "Stats:\n\tlist min size: " + to_string(min_list_size) + "\n\tlist max size: " + to_string(max_list_size) + "\n\tnonempty lists: " + to_string(nonempty_lists) + "\n\taverage list size: " + to_string(average_list_size) + '\n';
    }

    string str(string(*func)(T), int x = 0) {
        string s = "Hash table: \n\tCurrent size: " + to_string(size) + "\n\tMax size: " + to_string(capacity) + "\n\tTable:\n\t{\n";

        if (x < 1)
            x = capacity;

        for (int i = 0; i < x; i++) {
            if (!array[i].isEmpty()) {
                s +=  "\t\t" + to_string(i) + ": " + array[i].str(func);
            }
        }

        s += "\t}\n";

        return s;
    }

};



string randKey(int j) {
    string s = "";
    for (int i = 0; i < j; i++)
        s += static_cast<char>(rand() % (127 + 1 - 32) + 32);

    return s;
}


int main()
{
    srand(time(NULL));

    //HashTable<simple_object>* ht = new HashTable<simple_object>();
    //simple_object o1{ "Szabat", 21 };
    //simple_object o2{ "Marcin", 33 };
    //ht->insert("pies", o1);
    //ht->insert("kotek", o2);
    //ht->insert("jan", { "jhkhjk", 25 });
    //ht->insert("jany", { "karmnik", 35 });
    //ht->insert("panar", { "kjlkjl", 35 });
    //ht->insert("monar", { "kjlkkkjl", 1115 });
    //ht->insert("money", { "kjl898789kjl", 11175 });
    //ht->insert("zut", { "inf", 1 });
    //ht->insert("korzen", { "inf", 1 });
    //ht->insert("rzymianin", { "inf", 1 });
    //ht->insert("mlodyafryk", { "inf", 1 });
    //ht->insert("nierob", { "inf", 1 });
    //ht->insert("puszka", { "inf", 1 });
    //ht->insert("mucha", { "inf", 1 });
    //ht->insert("duch", { "inf", 1 });
    //ht->insert("mlot", { "infkjkj", -199 });
    //ht->clear();
    //cout << ht->remove("pies");
    //Item<simple_object>* it = ht->search("pies");
    //if (it == nullptr) cout << "NULLPTR";
    //cout << so_str(it->data);

    //cout << ht->str(so_str);
    //ht->clear();
    //delete ht;


    const int MAX_ORDER = 7;
    HashTable<int>* ht = new HashTable<int>();

    for (int o = 1; o <= MAX_ORDER; o++) {
        const int n = pow(10, o);

        clock_t t1 = clock();
        for (int i = 0; i < n; i++)
            ht->insert(randKey(6), i);
        clock_t t2 = clock();

        cout << ht->str(to_string, 5);
        cout << "\nCzas dodawania: " << (double)(t2 - t1) / CLOCKS_PER_SEC;

        const int m = pow(10, 4);
        int hits = 0;
        t1 = clock();
        for (int i = 0; i < m; i++) {
            Item<int>* it = ht->search(randKey(6));
            if (it != nullptr)
                hits++;
        }
        t2 = clock();
        cout << "\nCzas wyszukiwania: " << (double)(t2 - t1) / CLOCKS_PER_SEC << "\n";
        cout << ht->stats();
        ht->clear();
    }
    delete ht;
}


