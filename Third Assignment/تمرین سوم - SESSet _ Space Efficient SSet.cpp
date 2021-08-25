#include <iostream>
#include <assert.h>
#include <cstring>
#include <string>
#include <algorithm>

using namespace std;

template<class T>
class Array {
protected:
    T *a;
public:
    int length;
    Array(int l) {
        length = l;
        a = new T[length];
    }
    T& operator[](int i) {
        assert(i >= 0 && i<length);
        return a[i];
    }
    Array<T>& operator=(Array<T>& b) {
        if (a != NULL) delete[] a;
        a = b.a;
        length = b.length;
        b.a = NULL;
        return *this;
    }
};

template<class T>
class arrayDeque {
protected:
    Array<T> a;
    int n;
    int j;
    void resize() {
        Array<T> b(max(2 * n, 1));
        for (int i = 0; i<n; i++) {
            b[i] = a[i];
        }
        a = b;
    }
public:
    arrayDeque() :a(100) {
        n = 0;
        j = 0;
    }
    T get(int i) {
        return a[(i + j) % a.length];
    }
    T set(int i, T x) {
        T y = get(i);
        a[(i + j) % a.length] = x;
        return y;
    }
    T remove(int i) {
        T y = a[(j+i)%a.length];
        if (i<n / 2) {
            for (int k = i; k>0; k--) {
                a[(k + j ) % a.length] = a[(k + j-1) % a.length];
            }
            j = (j + 1) % a.length;
        }
        else {
            for (int k = i; k<n - 1; k++) {
                a[(k + j) % a.length] = a[(k + 1 + j) % a.length];
            }
        }
        n--;
        if (3 * n<a.length) resize();
        return y;
    }
    void add(int i, T x) {
        if (n + 1>a.length) resize();
        if (i<n / 2) {
            for (int k = 0; k<i; k++) {
                a[(k + j) % a.length] = a[(k + j + 1) % a.length];
            }
            j = j == 0 ? a.length - 1 : j - 1;
        }
        else {
            for (int k = n; k>i; k--) {
                a[(k + j) % a.length] = a[(k + j - 1) % a.length];
            }
        }
        a[(i + j) % a.length] = x;
        n++;
    }
    int size() {
        return n;
    }
};


class x {
public:
    string key;
    double value;

    friend bool operator<(const x &lhs, const x &rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator<=(const x &lhs, const x &rhs) {
        return lhs.key <= rhs.key;
    }
    friend bool operator==(const x &lhs, const x &rhs) {
        return lhs.key == rhs.key;
    }
    friend bool operator>(const x &lhs, const x &rhs) {
        return lhs.key > rhs.key;
    }
    friend bool operator!=(const x &lhs, const x &rhs) {
        return lhs.key != rhs.key;
    }
    x() {};
    x(string s, double v) {
        key = s;
        value = v;
    }
};
x null("", 0);

template<typename T>
class SESSet {
protected:
    int n;
    int b;
    int h;

    class Bdeque : public arrayDeque<T> {
        using arrayDeque<T>::j;
        using arrayDeque<T>::a;
        using arrayDeque<T>::n;
    public:
        Bdeque(int b) {
            n = 0;
            j = 0;
            Array<T> z(b + 1);
            a = z;
        }
        virtual ~Bdeque() {};
        virtual bool add(T x) {
            int i = 0;
            while (x>arrayDeque<T>::get(i) && i<size())
                i++;
            arrayDeque<T>::add(i, x);
            return true;
        }
        void resize() { };
    };
    class Node{
    public:
        Bdeque d;
        int height;
        Node *pre;
        Node **next;
        Node(int b, int h) :d(b) {
            height = pickHeight();
            next = new Node*[height];
        };
        Node():d(30) {
            height = 1;
            next = new Node*[height];
        };
    };
    Node *sentinel;
    Node **stack;
    int compare(T a, T b) {
        if (a == b)
            return 0;
        if (a>b)
            return 1;
        if (a<b)
            return -1;
    }
    Node* findPredNode(T x) {
        Node *u = sentinel;
        int r = h;
        while (r >= 0) {
            while (u->next[r] != NULL && compare(u->next[r]->d.get(u->next[r]->d.size() - 1), x) < 0)
                u = u->next[r];
            r--;
        }
        return u;
    }
    Node* newNode(Bdeque d, int h) {
        Node *u = (Node*)malloc(sizeof(Node) + (h + 2) * sizeof(Node*));
        u->d = d;
        u->height = h;
        return u;
    }
    void remove(Node *w) {
        Node *u = sentinel, *del=nullptr;
        int r = h;
        while (r >= 0) {
            while (u->next[r] != NULL && (compare(u->next[r]->d.get(0), w->d.get(0))) < 0) {
                u = u->next[r];
            }
            if (u->next[r] != NULL && u->next[r] == w) {
                del = u->next[r];
                u->next[r] = u->next[r]->next[r];
                u->next[r]->next[r]->pre = u;
                if (u == sentinel && u->next[r] == NULL)
                    h--;
            }
            r--;
        }
        delete del;
    }

    Node *addBefore(Node *k) {
        Node *w = newNode(b, pickHeight());
        Node *u = sentinel;
        int r = h;
        while (r >= 0) {
            while (u->next[r] != NULL && (compare(u->next[r]->d.get(0), k->d.get(0))) < 0)
                u = u->next[r];
            stack[r--] = u;
        }
        while (h < w->height)
            stack[++h] = sentinel;
        for (int i = 0; i < w->height; i++) {
            w->next[i] = stack[i]->next[i];
            stack[i]->next[i] = w;
        }
        w->pre=stack[0];
        stack[0]->next[0]->next[0]->pre = w;
        return w;
    }
    void gather(Node *u) {
        Node *w = u;
        for (int j = 0; j < b - 1; j++) {
            while (w->next[0]->d.size() < b)
                w->next[0]->d.add(w->next[0]->d.remove(0));
            w = w->next[0];
        }
        remove(w);
    }
    void spread(Node *u) {
        Node *w = u;
        for (int j = 0; j < b; j++) {
            w = w->next[0];
        }
        w = addBefore(w);
        while (w != u) {
            while (w->d.size() < b)
                w->d.add(w->pre->d.remove(w->pre->d.size() - 1));
            w = w->pre;
        }
    }


public:
    //T null;
    SESSet(int b) {
        //  null=(T) NULL;
        this->b = b;
        sentinel = new Node(b, sizeof(int) * 8);
        memset(sentinel->next, '\0', sizeof(Node*)*sentinel->height);
        stack = (Node**)new Node*[sentinel->height];
        h = 0;
        n = 0;
        sentinel->next[0] = sentinel;
        sentinel->pre = sentinel;
    }
    T find(T x) {
        Node *w = findPredNode(x);
        int i = 0;
        while (x>w->next[0]->d.get(i)) {
            i++;
        }
        if (x == w->next[0]->d.get(i)) {
            return w->next[0]->d.get(i);
        }
        else {
            return null;
        }
    }
    static int pickHeight() {
        int m = 1;
        int z = rand();
        int k = 0;
        while ((m&z) != 0) {
            k++;
            m <<= 1;
        }
        return k;
    }
    int size() {
        return n;
    }
    bool add(T x) {
        Node *u = sentinel;
        int r = h;
        bool t = false;
        int j = 0, i = 0;
        while (r >= 0) {
            while (u->next[r] != NULL && (compare(u->next[r]->d.get(u->next[r]->d.size() - 1), x)) < 0)
                u = u->next[r];
            r--;
        }
        Node *k = u->next[0];
      // while (i <= b && u->next[0]->d.get(i) <= x) {
      //     if (u->next[0]->d.get(i) == x)
      //         t = true;
      //     i++;
      // }
        while (j < b && u->next[0] != sentinel && u->next[0]->d.size() == b + 1) {
            u->next[0] = u->next[0]->next[0];
            j++;
        }
        if (j == b) {
            spread(k);
            u->next[0] = k;
        }
        if (u->next[0] == sentinel) {
            u->next[0] = addBefore(u->next[0]);
        }
        while (u->next[0] != k) {
            u->next[0]->d.add(u->d.remove(u->d.size() - 1));
            u->next[0] = u;
        }
        u->next[0]->d.add(x);
        n++;
        return t;
    }
    bool remove(T x) {
        bool removed = false;
        Node *u = sentinel;
        int r = h;
        int i = 0, j = 0;
        while (r >= 0) {
            while (u->next[r] != NULL && (compare(u->next[r]->d.get(u->next[r]->d.size() - 1), x)) < 0) {
                u = u->next[r];
            }
            r--;
        }
        Node *w = u->next[0];
        while (i <= b && u->next[0]->d.get(i) == x) {
            removed = true;
            i++;
        }
        if (removed) {
            while (j < b && u->next[0] != sentinel && u->next[0]->d.size() == b - 1) {
                u->next[0] = u->next[0]->next[0];
                j++;
            }
            if (j == b) {
                gather(w);
            }
            u->next[0] = w;
            u->next[0]->d.remove(i - 1);
            while (u->next[0]->d.size() < b - 1 && u->next[0]->next[0] != sentinel) {
                u->next[0]->d.add(u->next[0]->next[0]->d.remove(0));
                u->next[0] = u->next[0]->next[0];
            }
            if (u->next[0]->d.size() == 0)
                remove(u);
            n--;
        }
        return removed;
    }
};


int main() {
    SESSet<x> sesset(30);
    int n, value;
    string s, tmp;
    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> s;
        if (s == "add") {
            cin >> tmp;
            cin >> value;
            x data(tmp, value);
            bool t = sesset.add(data);
            if (t)
                cout << "already in there" << endl;
            else
                cout << "added" << endl;
        }
        if (s == "remove") {
            cin >> tmp;
            x data(tmp, 0);
            bool t = sesset.remove(data);
            if (t)
                cout << "removed" << endl;
            else
                cout << "does not exist" << endl;
        }
        if (s == "find") {
            cin >> tmp;
            x data(tmp, 0);
            if (sesset.find(data) != null) {
                cout << sesset.find(data).value;
            }
            else {
                cout << "not found" << endl;

            }
        }
        return 0;
    }
}
