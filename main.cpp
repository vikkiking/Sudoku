#include <iostream>
#include "getkey.h"
#include "kbhit.h"

using namespace std;

class Sudoku;

class number {
    int data;
    bool init;//是否为原题所给数字
    bool picked;
public:
    number() : init(false), picked(false) {}

    friend ostream &operator<<(ostream &, number &);

    friend Sudoku;
};

ostream &operator<<(ostream &output, number &n) {
    if (n.data) {
        if (n.picked)
            printf("\033[31m%d\033[0m", n.data);
        else if (n.init)
            printf("\033[32m%d\033[0m", n.data);
        else cout << n.data;
    } else {
        if (n.picked)
            cout << "□";
        else cout << ' ';
    }
    return output;
}

class Sudoku {
    number map[9][9];
    int space;
    pair<int, int> cursor;
    int startTime;

public:
    Sudoku() {
        cursor.first = cursor.second = 0;
        for (auto &i : map)
            for (auto &j : i)
                j.data = 0, j.init = j.picked = false;
        map[0][0].picked = true;
    };

    void init();//初始化题目
    void diy();//自定义题目
    void diy(string);//导入字符串
    bool isConflict(int, int);//判断同行同列或同宫内是否已有相同数字
    bool isConflict();//判断此局是否存在错误
    void update_page();//刷新页面
    //void autoSolve();//自动求解
    void Solve();//开始游戏
    bool end();//判断是否得出正解
    void welcome();//欢迎界面
    void gameEnd() const;//游戏结束;
};

void Sudoku::welcome() {
    cout << "***Sudoku***\n";
    cout << "(1)开始游戏\n";
    cout << "(2)自定义题目(按q完成输入)\n";
    cout << "(3)退出游戏\n";
    int ty;
    cin >> ty;
    switch (ty) {
        case 1: {
            init();
            Solve();
            break;
        }
        case 2: {
            int t;
            cout << "(1)填空\n";
            cout << "(2)导入字符串\n";
            cin >> t;
            if (t == 1)diy();
            else {
                string problem;
                cin >> problem;
                diy(problem);
            }
            Solve();
            break;
        }
        case 3: {
            exit(0);
        }
    }
}

void Sudoku::init() {
    //17至32个数
    srand((unsigned) time(NULL));
    int count_nums = rand() % 16 + 17;
    for (int i = 0; i < count_nums; i++) {
        int locx = rand() % 9, locy = rand() % 9;
        while (map[locx][locy].data) {
            locx = rand() % 9, locy = rand() % 9;
        }
        map[locx][locy].data = rand() % 9 + 1;
        while (isConflict(locx, locy)) {
            map[locx][locy].data = rand() % 9 + 1;
        }
        map[locx][locy].init = true;
    }
    update_page();
}

void Sudoku::diy() {
    update_page();
    while (1) {
        while (!kbhit()) {
            system("sleep 0.01s");
        }
        char key;
        //移动光标
        key = Getch();
        if (key == 'q') {
            if (space > 64 || space <= 49 || isConflict())
                continue;
            else
                break;
        }
        switch (key) {
            case 'w' : {
                //上
                if (cursor.first)
                    map[cursor.first][cursor.second].picked = false, cursor.first--;
                break;
            }
            case 'd' : {
                //右
                if (cursor.second != 8)
                    map[cursor.first][cursor.second].picked = false, cursor.second++;
                break;
            }
            case 's' : {
                //下
                if (cursor.first != 8)
                    map[cursor.first][cursor.second].picked = false, cursor.first++;
                break;
            }
            case 'a': {
                //左
                if (cursor.second)
                    map[cursor.first][cursor.second].picked = false, cursor.second--;
                break;
            }
        }
        map[cursor.first][cursor.second].picked = true;
        if (isdigit(key))
            map[cursor.first][cursor.second].data = key - '0';
        update_page();
    }
    for (auto &i : map)
        for (auto &j : i)
            if (j.data)j.init = true;
    update_page();
}

void Sudoku::diy(string problem) {
    if (problem.length() != 81) {
        cout << "字符串不符合要求!\n";
        exit(0);
    }
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++) {
            map[i][j].data = problem[i * 9 + j] - '0';
            map[i][j].init = (map[i][j].data) > 0;
            map[i][j].picked = false;
        }
    map[0][0].picked = true;
    update_page();
    if (space < 17 || space > 32) {
        cout << "题目有误\n";
        return;
    }
}

bool Sudoku::isConflict(int x, int y) {
    for (int i = 0; i < 9; i++)
        if ((y != i && map[x][y].data == map[x][i].data) || (x != i && map[x][y].data == map[i][y].data))
            return true;
    int housex = x / 3, housey = y / 3;
    for (int i = housex * 3; i < (housex + 1) * 3; i++)
        for (int j = housey * 3; j < (housey + 1) * 3; j++)
            if (map[i][j].data == map[x][y].data && i != x && j != y)
                return true;
    return false;
}

bool Sudoku::isConflict() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (isConflict(i, j)) {
                cout << "输入有误\n";
                return true;
            }
    return false;
}

void Sudoku::update_page() {
    system("clear");
    cout << "\nw,s,a,d分别控制上下左右\n\n";
    space = 0;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if ((j + 1) % 3 == 1)cout << "▍";
            else
                cout << '|';

            cout << map[i][j];
            if (map[i][j].data == 0)space++;

            if (j == 8)cout << "▍";
        }
        if ((i + 1) % 3 == 0)cout << endl;
        cout << endl;
    }
}

void Sudoku::Solve() {
    update_page();
    startTime = clock();
    char key = Getch();
    while (space || !end()) {
        while (!kbhit()) system("sleep 0.001s");
        //移动光标，填数
        while (1) {
            if (key == EOF)exit(0);
            if (isdigit(key))break;
            switch (key) {
                case 'w' : {
                    //上
                    if (cursor.first)
                        map[cursor.first][cursor.second].picked = false, cursor.first--;
                    break;
                }
                case 'd' : {
                    //右
                    if (cursor.second != 8)
                        map[cursor.first][cursor.second].picked = false, cursor.second++;
                    break;
                }
                case 's' : {
                    //下
                    if (cursor.first != 8)
                        map[cursor.first][cursor.second].picked = false, cursor.first++;
                    break;
                }
                case 'a': {
                    //左
                    if (cursor.second)
                        map[cursor.first][cursor.second].picked = false, cursor.second--;
                    break;
                }
            }
            map[cursor.first][cursor.second].picked = true;
            update_page();
            key = Getch();
        }
        if (!map[cursor.first][cursor.second].init) {
            if (key == '0' && map[cursor.first][cursor.second].data)space++;
            else if (key != '0' && map[cursor.first][cursor.second].data == 0)space--;
            map[cursor.first][cursor.second].data = key - '0';
        }
        update_page();
    }
}

bool Sudoku::end() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (isConflict(i, j)) {
                cout << "请检查!\n";
                return false;
            }
    map[cursor.first][cursor.second].picked = false;
    update_page();
    gameEnd();
    return true;
}

void Sudoku::gameEnd() const {
    cout << "恭喜你答对了!\n" << "总共花了" << (clock() - startTime) / 1000 << "秒\n";
}

int main() {
    Sudoku newGame;
    newGame.welcome();
}
