#include <iostream>
#include <vector>
#include <functional>
#include <bitset>
#include <fstream>
#include <queue>
#include <set>
#include <time.h>
using namespace std;
int N;
class board {
public:

    vector< vector<bool> > mat;
    double h =0 ;
    int g= 0;

    vector<vector<int> > track;
    board(){}
    board(vector< vector<bool> >& n) {
        this->mat = n;
        evaluate();
        
    }
    board(const board& b) {
        this->mat = b.mat;
        this->g = b.g;
        this->h = b.h;
        this->track = b.track;
    }
    void showTrack() {
        vector<board> boards;
        board b = (*this);
        boards.push_back(b);
        for (auto it = track.rbegin(); it != track.rend(); it++) {
            b = b.tap((*it)[0], (*it)[1], (*it)[2],true);
            boards.push_back(b);
        }
        int step = 0;
        for (auto it = boards.rbegin(); it != boards.rend(); it++) {
            cout << "******" << step++ << "******\n";
            it->print();
        }
    }
    void printTrack() {
        for (int i = 0; i < track.size(); i++) {
            for (int j = 0; j < track[i].size(); j++) {
                cout << track[i][j] << " ";
            }
            cout << '\n';
        }
    }
    void print() {
        //cout << "******" << g << "******\n";
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                cout << mat[i][j] << " ";
            }
            cout << "\n";
        }
    }
    bool operator <(const board& other) const
    {
        double weight =0.8;
        return( weight*this->g + this->h) > (weight * other.g + other.h);
    }

    void evaluate() {

        h = 0;
        for (int i = 0; i < mat.size(); i++) {
            for (int j = 0; j < mat[i].size(); j++) {
                h += mat[i][j];
            }
        }
        if (h == 0)
            return;
        for (int i = 0; i < mat.size() - 1; i++) {
            for (int j = 0; j < mat[i].size() -1; j++) {
                if (mat[i][j] + mat[i + 1][j] + mat[i][j + 1] + mat[i + 1][j + 1] == 3) {
                    h -= 2;
                    h = h / 3.0;
                    return;
                }
            }
        }
        h = (h / 3.0);
    }
    board tap(int i, int j, int direct, bool tracking) {
        board b (* this);
        bool tapped = false;
        if (direct == 2) {
            if (i >= 1 && j >= 1) {
                if (b.mat[i][j] + b.mat[i - 1][j] + b.mat[i][j - 1] > 0 || tracking) {
                    b.mat[i][j] = !b.mat[i][j];
                    b.mat[i - 1][j] = !b.mat[i - 1][j];
                    b.mat[i][j - 1] = !b.mat[i][j - 1];
                    tapped = true;
                }
                
            }
        }else if (direct == 1) {
            if (i >=1 && j <= N-2) {
                if (b.mat[i][j] + b.mat[i - 1][j] + b.mat[i][j + 1] > 0 || tracking) {
                    b.mat[i][j] = !b.mat[i][j];
                    b.mat[i - 1][j] = !b.mat[i - 1][j];
                    b.mat[i][j + 1] = !b.mat[i][j + 1];
                    tapped = true;
                }
                
            }
        }else if (direct == 3) {
            if (i <= N -2  && j >= 1) {
                if (b.mat[i][j] + b.mat[i + 1][j] + b.mat[i][j - 1] > 0 || tracking) {
                    b.mat[i][j] = !b.mat[i][j];
                    b.mat[i + 1][j] = !b.mat[i + 1][j];
                    b.mat[i][j - 1] = !b.mat[i][j - 1];
                    tapped = true;
                }
               
            }
        }else if (direct == 4) {
            if (i <=N-2 && j <= N-2) {
                if (b.mat[i][j] + b.mat[i + 1][j] + b.mat[i][j + 1] > 0 || tracking) {
                    b.mat[i][j] = !b.mat[i][j];
                    b.mat[i + 1][j] = !b.mat[i + 1][j];
                    b.mat[i][j + 1] = !b.mat[i][j + 1];
                    tapped = true;
                }
               
            }
        }
        
        if (tapped) {
            b.track.push_back(vector<int>({ i, j, direct }));
            b.g++;
            b.evaluate();
        }
        else {
            b.g = -1;
        }
        return b;

    }
};

board readmat() {
    ifstream f("../input/input7.txt");
    if (!f) {
        cout << "aaaaaa" << endl;
    }
    f >> N;
    vector< vector<bool> > mat;
    int n;
    mat.resize(N);
    for (int i = 0; i < N; i++) {
        mat[i].resize(N);
        for (int j = 0; j < N; j++) {
            f >> n;
            mat[i][j] = (bool)n;
        }
    }
    f.close();
    return board(mat);
}
class comp
{
public:
    bool operator() (const board& a,const board& b) const{
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (a.mat[i][j] != b.mat[i][j])
                    return a.mat[i][j] < b.mat[i][j];
            }
        }
        return false;
        //return a.hashcode < b.hashcode;
    }
};
int MAX = 2500;
priority_queue<board> half(priority_queue<board>& pq) {
    priority_queue<board> newp;
    for (int i = 0; i < 1500; i++) {
        newp.push(pq.top());
        pq.pop();
    }
    return newp;
}
//priority_queue<board> half(priority_queue<board>& pq) {
//    vector<board> top_elements;
//    for (int i = 0; i < 1500 && !pq.empty(); i++) {
//        top_elements.push_back(pq.top());
//        pq.pop();
//    }
//    priority_queue<board> newp(make_move_iterator(top_elements.begin()), make_move_iterator(top_elements.end()));
//    return newp;
//}


int main()
{
    auto start = clock();
    board init = readmat();
    std::priority_queue<board> pq;
    set<board,comp> used;
    pq.push(init);

    int iters = 0;
    bool over = false;
    board res;
    while (!pq.empty()) {
        iters++;
        board b(pq.top());
        pq.pop();
        //used.insert(b);
        used.insert(b);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 1; k <= 4; k++) {
                    board neigbor = b.tap(i, j, k,false);
                    if (neigbor.g == -1)
                        continue;
                    else if (neigbor.h == 0) {
                        res = neigbor;
                        over = true;
                        break;
                    }
                    else {
                        if (used.count(neigbor) == 0) {
                            if (pq.size() >= MAX) { 
                                pq = half(pq);
                            }
                            pq.push(neigbor);
                        }
                    }
                }
                if (over) break;
            }
            if (over) break;
        }
        if (over) break;
        
    }
    auto end = clock();
    res.showTrack();
   // res.printTrack();
    cout << "time " << double(end - start) / CLOCKS_PER_SEC << "\n";
    cout << "\niters " << iters << "\n";
    cout << "used" << used.size();
}
