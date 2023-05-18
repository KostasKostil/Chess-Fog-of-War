#include "visual.h"
#include "mystockfish.h"
#define SLIDE_TIME 0.5

using namespace std;

const int d = 32;
const int l = 64;

vector<Board> boards;
bool ReadMove(ifstream& fin, Move& m)
{
    string s;
    if (!(fin>>s))
        return false;

    int id = 0;
    if (s[0] >= 'A' && s[0] <= 'Z')
    {
        m.piece.type = Type::None;
        id = 1;
    }
    else
    {
        m.piece.type = Type::Pawn;
    }

    if (s.size() <= size_t(id+4))
        return false;
    m.from.y = s[id] - 'a'; id++;
    m.from.x = 8 - (s[id] - '0'); id++; id++;
    m.to.y   = s[id] - 'a'; id++;
    m.to.x   = 8 - (s[id] - '0');

    return true;
}

void ReadGame(string filename)
{
    string id;
    ifstream fin(filename);
    Board b = InitialBoard();
    Move m;
    boards = {b};

    while (true)
    {
        fin>>id;
        if (!ReadMove(fin, m))
            break;
        b = ApplyMove(b, m);
        boards.push_back(b);

        if (!ReadMove(fin, m))
            break;
        b = ApplyMove(b, m);
        boards.push_back(b);
    }
    cout<<boards.size()<<"\n";
}

vector<pair<Piece, sf::Image> > images;
void ReadImages()
{
    sf::Image img;

    img.loadFromFile("textures/wp.png"); images.push_back({Piece{Color::White, Type::Pawn}, img});
    img.loadFromFile("textures/wn.png"); images.push_back({Piece{Color::White, Type::Knight}, img});
    img.loadFromFile("textures/wb.png"); images.push_back({Piece{Color::White, Type::Bishop}, img});
    img.loadFromFile("textures/wr.png"); images.push_back({Piece{Color::White, Type::Rook}, img});
    img.loadFromFile("textures/wq.png"); images.push_back({Piece{Color::White, Type::Queen}, img});
    img.loadFromFile("textures/wk.png"); images.push_back({Piece{Color::White, Type::King}, img});

    img.loadFromFile("textures/bp.png"); images.push_back({Piece{Color::Black, Type::Pawn}, img});
    img.loadFromFile("textures/bn.png"); images.push_back({Piece{Color::Black, Type::Knight}, img});
    img.loadFromFile("textures/bb.png"); images.push_back({Piece{Color::Black, Type::Bishop}, img});
    img.loadFromFile("textures/br.png"); images.push_back({Piece{Color::Black, Type::Rook}, img});
    img.loadFromFile("textures/bq.png"); images.push_back({Piece{Color::Black, Type::Queen}, img});
    img.loadFromFile("textures/bk.png"); images.push_back({Piece{Color::Black, Type::King}, img});
}

void Render(Board b, sf::RenderWindow* window, int xfrom, int yfrom, int xlabel = -1, int ylabel = -1)
{
    for (int i=0; i<8; i++)
        for (int j=0; j<8; j++)
    {
        int x = xfrom + j*l;
        int y = yfrom + i*l;

        sf::RectangleShape r;
        r.setPosition(x, y);
        r.setSize(sf::Vector2f(l, l));
        if (i == xlabel && j == ylabel)
            r.setFillColor(b[i][j].type == Type::Fog ? sf::Color(128, 128, 128) : (i+j)%2 == 0 ? sf::Color(178, 178, 150) : sf::Color(58, 90, 26) );
        else
            r.setFillColor(b[i][j].type == Type::Fog ? sf::Color(192, 192, 192) : (i+j)%2 == 0 ? sf::Color(238, 238, 210) : sf::Color(118, 150, 86) );

        window->draw(r);

        if (b[i][j].type != Type::Fog and b[i][j].type != Type::None)
        {
            sf::Texture t;
            for (auto& pa : images)
                if (b[i][j].type == pa.first.type && b[i][j].color == pa.first.color)
                {
                    t.loadFromImage(pa.second);
                    break;
                }
            sf::Sprite sp;
            t.setSmooth(true);
            sp.setTexture(t);
            sp.setScale(l * 1.0 / t.getSize().x, l * 1.0 / t.getSize().y);
            sp.setPosition(x, y);
            window->draw(sp);
        }
    }
}

void Visualize(std::string filename)
{
    ReadImages();
    ReadGame(filename);

    bool slides = false;
    int current_board = 0;
    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(4*d+3*8*l, 2*d+8*l), "CFoW");
    auto timestamp = clock();
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                    current_board--;
                if (event.key.code == sf::Keyboard::Right)
                    current_board++;
                if (event.key.code == sf::Keyboard::S)
                    slides = !slides;
            }
        }
        if (slides && (clock() - timestamp) > SLIDE_TIME * CLOCKS_PER_SEC)
        {
            current_board++;
            timestamp = clock();
        }
        current_board = max(current_board, 0);
        current_board = min(current_board, int(boards.size()) - 1);

        window->clear(sf::Color::White);
        Render(GetFoggedBoard(boards[current_board], Color::White), window, d, d);
        Render(boards[current_board], window, 2*d+8*l, d);
        Render(GetFoggedBoard(boards[current_board], Color::Black), window, 3*d+16*l, d);

        window->display();
    }
}

void VisPlay(std::shared_ptr<Player> p)
{
    p->Initialize(Color::Black);

    ReadImages();
    boards.clear();
    boards.push_back({InitialBoard()});

    bool full = false;
    bool slides = false;
    int current_board = 0;
    int choosen_i = -1, choosen_j = -1;
    int wait = 0;
    bool going = 1;

    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(4*d+3*8*l, 2*d+8*l), "CFoW");
    auto timestamp = clock();
    auto timestamp2 = clock();
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (wait && going)
                continue;
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                    current_board--;
                if (event.key.code == sf::Keyboard::Right)
                    current_board++;
                if (event.key.code == sf::Keyboard::V)
                    full = !full;
                if (event.key.code == sf::Keyboard::S)
                    slides = !slides;
            }
            if (wait)
                continue;
            if (event.type == sf::Event::MouseButtonPressed && current_board + 1 == int(boards.size()))
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                if (x < 8*l+2*d || x >= 2*d + 16*l || y < d || y >= d + 8*l)
                {
                    choosen_i = -1;
                    choosen_j = -1;
                    continue;
                }

                int i = (y-d)/l;
                int j = (x-2*d-8*l)/l;
                if (choosen_i == -1)
                {
                    choosen_i = i;
                    choosen_j = j;
                }
                else
                {
                    Move m;
                    m.from = Square{choosen_i, choosen_j};
                    m.to = Square{i, j};
                    bool ok = false;
                    for (Move mx : GetMoves(boards.back(), Color::White))
                        if (mx.from.x == m.from.x && mx.from.y == m.from.y && mx.to.x == m.to.x && mx.to.y == m.to.y)
                        {
                            ok = true;
                            if (boards.back()[m.to.x][m.to.y].type == Type::King)
                                going = false;

                            boards.push_back(ApplyMove(boards.back(), mx));
                            current_board++;
                            wait = 1;
                            timestamp = clock();
                            choosen_i = -1;
                            choosen_j = -1;
                            break;
                        }
                    if (!ok)
                    {
                        choosen_i = i;
                        choosen_j = j;
                    }
                }
            }
        }
        if (wait && going && (clock() - timestamp) > 0.1 * CLOCKS_PER_SEC)
        {
            int s = boards.size();
            Move m = p->GetMove(GetFoggedBoard(boards[s-2], Color::Black), GetFoggedBoard(boards[s-1], Color::Black) );
            wait = 0;
            if (boards.back()[m.to.x][m.to.y].type == Type::King)
                going = false, wait = 1;
            boards.push_back(ApplyMove(boards.back(), m));
            current_board++;
        }
        if (slides && (clock() - timestamp2) > SLIDE_TIME * CLOCKS_PER_SEC)
        {
            current_board++;
            timestamp2 = clock();
        }
        current_board = max(current_board, 0);
        current_board = min(current_board, int(boards.size()) - 1);

        if (wait && going)
            window->clear(sf::Color::Black);
        else
            window->clear(sf::Color::White);

        if (full)
        {
            Render(GetFoggedBoard(boards[current_board], Color::White), window, d, d);
            Render(boards[current_board], window, 2*d+8*l, d, choosen_i, choosen_j);
            Render(GetFoggedBoard(boards[current_board], Color::Black), window, 3*d+16*l, d);
        }
        else
            Render(GetFoggedBoard(boards[current_board], Color::White), window, 2*d+8*l, d, choosen_i, choosen_j);


        window->display();
    }
}

void PlayStockfish(int depth)
{
    ReadImages();
    boards.clear();
    boards.push_back({InitialBoard()});

    bool full = true;
    int current_board = 0;
    int choosen_i = -1, choosen_j = -1;
    int wait = 0;
    bool going = 1;

    sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(2*d+8*l, 2*d+8*l), "CFoW");
    auto timestamp = clock();
    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (wait && going)
                continue;
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Left)
                    current_board--;
                if (event.key.code == sf::Keyboard::Right)
                    current_board++;
                if (event.key.code == sf::Keyboard::V)
                    full = !full;
            }
            if (wait)
                continue;
            if (event.type == sf::Event::MouseButtonPressed && current_board + 1 == int(boards.size()))
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                if (x < d || x >= d + 8*l || y < d || y >= d + 8*l)
                {
                    choosen_i = -1;
                    choosen_j = -1;
                    continue;
                }

                int i = (y-d)/l;
                int j = (x-d)/l;
                if (choosen_i == -1)
                {
                    choosen_i = i;
                    choosen_j = j;
                }
                else
                {
                    Move m;
                    m.from = Square{choosen_i, choosen_j};
                    m.to = Square{i, j};
                    bool ok = false;
                    for (Move mx : GetMoves(boards.back(), Color::White))
                        if (mx.from.x == m.from.x && mx.from.y == m.from.y && mx.to.x == m.to.x && mx.to.y == m.to.y)
                        {
                            ok = true;
                            if (boards.back()[m.to.x][m.to.y].type == Type::King)
                                going = false;

                            boards.push_back(ApplyMove(boards.back(), mx));
                            current_board++;
                            wait = 1;
                            timestamp = clock();
                            choosen_i = -1;
                            choosen_j = -1;
                            break;
                        }
                    if (!ok)
                    {
                        choosen_i = i;
                        choosen_j = j;
                    }
                }
            }
        }
        if (wait && going && (clock() - timestamp) > 0.1 * CLOCKS_PER_SEC)
        {
            int s = boards.size();
            auto pa = Stockfish::Run(boards[s-1], Color::Black, depth);
            Move m = pa.second;
            cout<<pa.first<<"\n";

            wait = 0;
            if (boards.back()[m.to.x][m.to.y].type == Type::King)
                going = false, wait = 1;
            boards.push_back(ApplyMove(boards.back(), m));
            current_board++;
        }
        current_board = max(current_board, 0);
        current_board = min(current_board, int(boards.size()) - 1);

        if (wait && going)
            window->clear(sf::Color::Black);
        else
            window->clear(sf::Color::White);

        if (full)
            Render(boards[current_board], window, d, d, choosen_i, choosen_j);
        else
            Render(GetFoggedBoard(boards[current_board], Color::White), window, d, d, choosen_i, choosen_j);


        window->display();
    }
}
