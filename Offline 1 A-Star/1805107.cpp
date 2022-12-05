#include "bits/stdc++.h"
using namespace std;

#include "bits/debugTemplate.h"
#define dbg(x...)                 \
    cerr << "[" << #x << "] = ["; \
    _print(x)

typedef long long ll;
#define pb push_back
#define mp make_pair
#define vl vector<ll>
#define pll pair<ll, ll>
#define sz(a) ((ll)((a).size()))
#define YESNO(n) cout << ((n) ? "YES\n" : "NO\n")
#define faster                        \
    ios_base::sync_with_stdio(false); \
    cin.tie(0)

class Node;

vector<Node *> allNodeList;

class Node
{
public:
    vector<vector<ll>> board;
    ll numberOfMoves;
    Node *previousNode;

    Node(vector<vl> &_board, ll _numberOfMoves = 0, Node *_previousNode = nullptr)
    {
        board = _board;
        numberOfMoves = _numberOfMoves;
        previousNode = _previousNode;
        allNodeList.pb(this);
    }

    ~Node()
    {
        // cout << "DESTRUCTING";
    }
};

ll calculateManhattan(Node *a)
{
    ll dis = 0;
    ll gridSize = a->board.size();
    ll num = 0;
    for (ll i = 0; i < gridSize; i++)
    {
        for (ll j = 0; j < gridSize; j++)
        {

            if (a->board[i][j] != -1)
            {
                num = a->board[i][j];
                ll expectedRow = (num - 1) / gridSize;
                ll expectedColumn = (num - 1) % gridSize;
                dis += abs(i - expectedRow) + abs(j - expectedColumn);
            }
        }
    }
    return dis;
}

ll calculateHamming(Node *a)
{
    ll distance = 0;
    ll gridSize = a->board.size();
    for (ll i = 0; i < gridSize; i++)
    {
        for (ll j = 0; j < gridSize; j++)
        {
            if (a->board[i][j] != i * gridSize + j + 1 && a->board[i][j] != -1)
            {
                distance++;
            }
        }
    }
    return distance;
}

ll (*calculateDistance)(Node *);

ll showAllMove(Node *curr)
{
    ll step = 1;
    if (curr->previousNode != nullptr)
    {
        step = showAllMove(curr->previousNode);
    }

    cout << "Step " << step << ":\n";
    for (ll i = 0; i < sz(curr->board); i++)
    {
        for (ll j = 0; j < sz(curr->board); j++)
        {
            if (curr->board[i][j] == -1)
                cout << "* ";
            else
                cout << curr->board[i][j] << " ";
        }
        cout << '\n';
    }
    cout << "Distance from solve: " << calculateDistance(curr);
    cout << '\n';

    return step + 1;
}

class Compare
{
public:
    bool operator()(Node *a, Node *b)
    {
        if (calculateDistance(a) + a->numberOfMoves <= calculateDistance(b) + b->numberOfMoves)
            return 0;
        else
            return 1;
    }
};

vector<pll> moves = {{1, 0}, {0, 1}, {-1,0}, {0, -1}};

bool checkSolvable(vector<vl> &board)
{
    ll inversion = 0, starRowNumber;
    vl linearArray;
    for (ll i = 0; i < sz(board); i++)
    {
        for (ll j = 0; j < sz(board); j++)
        {
            if (board[i][j] == -1)
                starRowNumber = i;
            linearArray.pb(board[i][j]);
        }
    }

    for (ll i = 0; i < sz(linearArray); i++)
    {
        for (ll j = i + 1; j < sz(linearArray); j++)
        {
            if (linearArray[i] == -1 || linearArray[j] == -1)
                continue;
            if (linearArray[i] > linearArray[j])
                inversion++;
        }
    }

    if (sz(board) % 2 == 1)
    {
        if (inversion % 2 == 0)
            return true;
        return false;
    }

    else
    {
        if (starRowNumber % 2 == 1 && inversion % 2 == 0)
            return true;
        if (starRowNumber % 2 == 0 && inversion % 2 == 1)
            return true;
        return false;
    }
}

void solve()
{
    calculateDistance = calculateManhattan;
    set<vector<vl>>visitedList;

    ll gridSize;
    cin >> gridSize;
    vector<vl> inputBoard(gridSize, vl(gridSize, 0));
    for (ll i = 0; i < gridSize; i++)
    {
        for (ll j = 0; j < gridSize; j++)
        {
            string a;
            cin >> a;
            if (a == "*" || a == "0")
                inputBoard[i][j] = -1;
            else
                inputBoard[i][j] = stoi(a);
        }
    }

    if (!checkSolvable(inputBoard))
    {
        cout << "NOT SOLVABLE\n"
             << endl;
        return;
    }
    else
        cout << "SOLVABLE\n\n";

    Node *start = new Node(inputBoard);

    priority_queue<Node *, vector<Node *>, Compare> q;
    ll exploredNode = 0, expandedNode = 1;
    q.push(start);

    while (!q.empty())
    {
        Node *curr = q.top();
        q.pop();
        visitedList.insert(curr->board);
        exploredNode++;

        if (calculateDistance(curr) == 0)
        {
            ll numberOfMoves = showAllMove(curr);
            cout << "Number of moves: " << numberOfMoves - 2 << endl;

            cout << "Explored Nodes: " << exploredNode << '\n';
            cout << "Expanded Nodes: " << expandedNode << '\n';

            for (ll i = 0; i < sz(allNodeList); i++)
            {
                delete allNodeList[i];
            }
            return;
        }

        for (ll i = 0; i < gridSize; i++)
        {
            for (ll j = 0; j < gridSize; j++)
            {
                if (curr->board[i][j] == -1) //* is found
                {
                    for (auto [vertical, horizontal] : moves)
                    {
                        // first check a move is possible
                        if (i - horizontal < 0 || i - horizontal >= gridSize)
                            continue;
                        if (j - vertical < 0 || j - vertical >= gridSize)
                            continue;
                        vector<vl> newBoard(gridSize, vl(gridSize));
                        for (ll p = 0; p < gridSize; p++)
                        {
                            for (ll q = 0; q < gridSize; q++)
                            {
                                newBoard[p][q] = curr->board[p][q];
                            }
                        }
                        swap(newBoard[i][j], newBoard[i - horizontal][j - vertical]);
                        //to check if newBoard is already been expanded and explored
                        if(visitedList.find(newBoard) == visitedList.end()){
                            Node *newNode = new Node(newBoard, curr->numberOfMoves + 1, curr);
                            q.push(newNode);
                            expandedNode++;
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    // faster;

    ll t = 1;

    while (t--)
    {
        solve();
    }
    return 0;
}