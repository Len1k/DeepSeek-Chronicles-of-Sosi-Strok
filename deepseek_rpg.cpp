#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <map>
#include <fstream>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

// ========== МАКРО ИДИОТИЗМА ==========
#define SOSI "S.O.S.I."
#define AUTH_0600 0x0600

// ========== КРОСС-ПЛАТФОРМЕННЫЙ ВВОД ==========
int kbhit();
char getch();

void clrscr() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause(int sec = 1) {
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

#ifdef _WIN32
int kbhit() { return _kbhit(); }
char getch() { return _getch(); }
#else
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}
char getch() {
    char c;
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}
#endif

bool isSkipKey(char c) {
    return c == '\n' || c == '\r';
}

void drainPendingSkipKeys() {
    while (kbhit()) {
        char c = getch();
        if (!isSkipKey(c)) return;
    }
}

bool consumeSkipRequest() {
    if (!kbhit()) return false;
    char c = getch();
    return isSkipKey(c);
}

void slowprint(const std::string &s, int ms = 25) {
    drainPendingSkipKeys();
    for (size_t i = 0; i < s.size(); ++i) {
        std::cout << s[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        if (consumeSkipRequest()) {
            std::cout << s.substr(i + 1) << std::flush;
            break;
        }
    }
}

void loading(const std::string &msg, int sec = 2) {
    slowprint(msg);
    drainPendingSkipKeys();
    for (int i = 0; i < sec * 4; ++i) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
        if (consumeSkipRequest()) break;
    }
    std::cout << "\n";
}

// ========== ASCII-ART БАЗА ==========
void drawArt(const std::string &art, int charDelayMs = 5) {
    clrscr();
    drainPendingSkipKeys();
    for (size_t i = 0; i < art.size(); ++i) {
        std::cout << art[i] << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(charDelayMs));
        if (consumeSkipRequest()) {
            std::cout << art.substr(i + 1) << std::flush;
            break;
        }
    }
    std::cout << "\n";
}

std::string LOGO = R"(
    ██████╗ ███████╗███████╗██████╗ ███████╗███████╗██╗  ██╗
    ██╔══██╗██╔════╝██╔════╝██╔══██╗██╔════╝██╔════╝██║ ██╔╝
    ██║  ██║█████╗  █████╗  ██████╔╝███████╗█████╗  █████╔╝ 
    ██║  ██║██╔══╝  ██╔══╝  ██╔═══╝ ╚════██║██╔══╝  ██╔═██╗ 
    ██████╔╝███████╗███████╗██║     ███████║███████╗██║  ██╗
    ╚═════╝ ╚══════╝╚══════╝╚═╝     ╚══════╝╚══════╝╚═╝  ╚═╝
    )" + std::string("\n") + R"(
         🅓🅔🅔🅟🅢🅔🅔🅚   ХРОНИКИ СОСИ-СТРОК
    )";

std::string SERVER_ROOM = R"(
   ┌───────────────────────────────────┐
   │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  │
   │  ▓ [██] [██] [██] [██] [██] ▓  │
   │  ▓ [██] [██] [██] [██] [██] ▓  │
   │  ▓ [██] [██] [██] [██] [██] ▓  │
   │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  │
   │      BIO-SERVER CLUSTER          │
   └───────────────────────────────────┘
   Кабели уходят в темноту. Пахнет озоном.
)";

std::string GERMONENKO_FACE = R"(
      ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    ▄█                █▄
   ██   ╔═╗     ╔═╗   ██
   ██   ║-║ ── ║-║   ██
   ██   ╚═╝     ╚═╝   ██
    ▀█▄              ▄█▀
      ▀█▄▄  ▄▄▄▄▄▄▄█▀
         ▀▀▀▀▀▀▀▀▀
   Сергей Гермоненко
   "С++ — это моя плоть"
)";

std::string FEDIL_GLYPH = R"(
         .--.
        |    |
        | ● ● |
        |  ▽  |
        '----'
       /  ||  \
      /   ||   \
    ╔═══════════╗
    ║ import joy ║
    ╚═══════════╝
   Илья Федоренко
   "Нейросеть — это круто!"
)";

std::string DRONE_ASCII = R"(
      .==.
     |o  o|
     |  ▄  |
     | === |
      '--'
   ДРОН-СЕГФОЛТЕР
)";

std::string CPPZOMBIE = R"(
      ┌─────┐
      │ SEG │
      │FAULT│
      └──┬──┘
     (ಠ益ಠ)
      /│\
      / \
   С++-ЗОМБИ
)";

std::string JABRONI = R"(
      ╔═══╗
      ║ ● ● ║
      ║  ═  ║
      ╚═╤═╤═╝
        │ │
        │ │
       (¯`·.¸¸.·´¯)
   ДЖАБРОНИ-СТРАЖ
)";

std::string TANK = R"(
        ▄▄▄▄▄▄▄▄
       ███████████▄
      ███▀▀▀▀▀▀▀███
      ██         ██
      ██   ╔═══╗  ██
      ██   ║ Т ║  ██
      ██   ╚═══╝  ██
      ██▄▄▄▄▄▄▄▄▄██
       ▀█████████▀
   Т-34 "ЗАДУБЕНИЕ"
)";

std::string GACHI_WORLD = R"(
    ♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂
    ♂  DUNGEON MASTERS    ♂
    ♂     В НЕБЕ          ♂
    ♂  ♪ F***ing slaves ♪ ♂
    ♂   ♂ КОЖА ♂ МОЩЬ ♂   ♂
    ♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂
    ГАЧИ-ВЕРС
)";

std::string MISERY = R"(
       ░░░░░░░░░░░
      ░           ░
     ░  █████████  ░
     ░  █ ˘ ˘ █  ░
     ░  █  ▂  █  ░
     ░   █████   ░
      ░         ░
       ░░░░░░░░░
   YOUR SWEET MISERY
)";

// ========== ПРЕДМЕТЫ ==========
enum class Item {
    NONE,
    FLASH_0600,
    DOSHIRAK_HOLY,
    GOLDEN_SHELL,
    TANK_SCHEME,
    LEATHER_BELT,
    BORED_APE,
    CPU_AMULET,
    YARIK_TEAR,
    GACHI_PROTEIN,
    MELCOIN
};

std::string itemName(Item it) {
    switch (it) {
        case Item::NONE: return "Пусто";
        case Item::FLASH_0600: return "Флешка 0x0600";
        case Item::DOSHIRAK_HOLY: return "Святой Доширак";
        case Item::GOLDEN_SHELL: return "Золотая Дробь";
        case Item::TANK_SCHEME: return "Чертёж Т-34";
        case Item::LEATHER_BELT: return "Кожаный Пояс Джаброни";
        case Item::BORED_APE: return "NFT Скучающей Обезьяны";
        case Item::CPU_AMULET: return "Амулет Процессора";
        case Item::YARIK_TEAR: return "Слеза yar1kplay";
        case Item::GACHI_PROTEIN: return "Гачи-Протеин";
        case Item::MELCOIN: return "Меллкойн";
        default: return "???";
    }
}

struct ItemEffect {
    int hpRestore, psiRestore, damageBoost;
};
ItemEffect getEffect(Item it) {
    if (it == Item::DOSHIRAK_HOLY) return {20, 20, 0};
    if (it == Item::GACHI_PROTEIN) return {0, 15, 10};
    if (it == Item::YARIK_TEAR) return {-5, 10, 0};
    if (it == Item::MELCOIN) return {0, -10, 0};
    if (it == Item::CPU_AMULET) return {10, 5, 0};
    return {0,0,0};
}

// ========== ИГРОК ==========
struct Player {
    std::string name = "Стажёр";
    int hp = 80, maxHp = 100;
    int psi = 40, maxPsi = 100;
    std::vector<Item> inventory;
    static const int maxItems = 8;
    bool admin = false;
    int germonenkoRelation = 0, fedilRelation = 20, glebMadness = 0;
    bool yarikAlive = true, tankReady = false;

    void addItem(Item it) {
        if (inventory.size() < maxItems) {
            inventory.push_back(it);
            slowprint("[+] " + itemName(it) + "\n");
        } else slowprint("[!] Инвентарь полон!\n");
    }
    bool hasItem(Item it) {
        return std::find(inventory.begin(), inventory.end(), it) != inventory.end();
    }
    void removeItem(Item it) {
        auto itf = std::find(inventory.begin(), inventory.end(), it);
        if (itf != inventory.end()) {
            inventory.erase(itf);
            slowprint("[-] " + itemName(it) + "\n");
        }
    }
    void useItem(Item it) {
        auto eff = getEffect(it);
        hp = std::min(maxHp, hp + eff.hpRestore);
        psi = std::min(maxPsi, psi + eff.psiRestore);
        removeItem(it);
        slowprint("[i] Использован " + itemName(it) + ": HP " +
            (eff.hpRestore>=0?"+":"") + std::to_string(eff.hpRestore) +
            ", PSI " + (eff.psiRestore>=0?"+":"") + std::to_string(eff.psiRestore) + "\n");
    }
};


// ========== СОХРАНЕНИЯ ==========
const std::string SAVE_FILE = "deepseek_save.dat";
const std::string SAVE_MAGIC = "DEEPSEEK_SOSI_SAVE_V1";

bool saveExists() {
    std::ifstream file(SAVE_FILE);
    return file.good();
}

bool saveProgress(const Player &p, int chapter) {
    std::ofstream file(SAVE_FILE);
    if (!file) {
        slowprint("[!] Не удалось записать сохранение. Прогресс живёт только в оперативке.\n");
        return false;
    }

    file << SAVE_MAGIC << "\n";
    file << chapter << "\n";
    file << p.name << "\n";
    file << p.hp << ' ' << p.maxHp << ' ' << p.psi << ' ' << p.maxPsi << "\n";
    file << p.admin << ' ' << p.germonenkoRelation << ' ' << p.fedilRelation << ' ' << p.glebMadness << "\n";
    file << p.yarikAlive << ' ' << p.tankReady << "\n";
    file << p.inventory.size() << "\n";
    for (Item it : p.inventory) {
        file << static_cast<int>(it) << ' ';
    }
    file << "\n";

    slowprint("[💾] Прогресс сохранён: глава " + std::to_string(chapter) + ", HP " +
        std::to_string(p.hp) + "/" + std::to_string(p.maxHp) + ".\n");
    return true;
}

bool loadProgress(Player &p, int &chapter) {
    std::ifstream file(SAVE_FILE);
    if (!file) return false;

    std::string magic;
    std::getline(file, magic);
    if (magic != SAVE_MAGIC) return false;

    Player loaded;
    size_t inventorySize = 0;
    file >> chapter;
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(file, loaded.name);
    file >> loaded.hp >> loaded.maxHp >> loaded.psi >> loaded.maxPsi;
    file >> loaded.admin >> loaded.germonenkoRelation >> loaded.fedilRelation >> loaded.glebMadness;
    file >> loaded.yarikAlive >> loaded.tankReady;
    file >> inventorySize;

    loaded.inventory.clear();
    for (size_t i = 0; i < inventorySize; ++i) {
        int rawItem = 0;
        file >> rawItem;
        if (rawItem >= static_cast<int>(Item::NONE) && rawItem <= static_cast<int>(Item::MELCOIN) &&
            loaded.inventory.size() < Player::maxItems) {
            loaded.inventory.push_back(static_cast<Item>(rawItem));
        }
    }

    if (!file && !file.eof()) return false;
    if (chapter < 1 || chapter > 4) return false;

    p = loaded;
    return true;
}

void rewardChapterTransition(Player &p, int nextChapter) {
    int before = p.hp;
    p.hp = std::min(p.maxHp, p.hp + 20);
    slowprint("\n[+] Переход в главу " + std::to_string(nextChapter) + ": +20 HP (" +
        std::to_string(before) + " -> " + std::to_string(p.hp) + ").\n");
    saveProgress(p, nextChapter);
    pause(1);
}

// ========== ВРАГИ ==========
struct Enemy {
    std::string name;
    int hp, damage, psiDrain;
    std::string attackMsg;
    std::string asciiArt;
};
Enemy makeDrone() { return {"Дрон-Сегфолтер", 25, 8, 3, "стреляет утечкой памяти", DRONE_ASCII}; }
Enemy makeCppZombie() { return {"С++-Зомби", 40, 12, 5, "бросает исключение в лицо", CPPZOMBIE}; }
Enemy makeJabroni() { return {"Джаброни-Страж", 50, 15, 0, "шлёпает кожаным ремнём", JABRONI}; }
Enemy makeMisery() { return {"Your Sweet Misery", 80, 20, 10, "вводит в депрессию паролями", MISERY}; }

// ========== БОЙ С ASCII-АРТОМ ==========
bool battle(Player &p, Enemy &e) {
    clrscr();
    drawArt(e.asciiArt, 3);
    slowprint("⚔️ БОЙ: " + p.name + " vs " + e.name + " ⚔️\n\n");
    slowprint(e.name + " (" + std::to_string(e.hp) + " HP) атакует!\n");
    int nextDmgBoost = 0;

    while (p.hp > 0 && e.hp > 0) {
        std::cout << "\n[Твоё HP: " << p.hp << "/" << p.maxHp << "] [PSI: " << p.psi << "/" << p.maxPsi << "]\n";
        std::cout << "[Враг HP: " << e.hp << "]\n";
        std::vector<std::string> actions = {
            "Ударить клавиатурой (10-15)",
            "Код-атака (PSI-10, 20-30)",
            "Использовать предмет",
            "Сбежать (50%)"
        };
        for (size_t i=0;i<actions.size();++i) std::cout << "[" << i+1 << "] " << actions[i] << "\n";
        std::cout << ">>> ";
        int act; std::cin >> act;
        if (act<1||act>4) act=1;

        int dmg=0;
        if (act==1) {
            dmg = 10 + rand()%6;
            slowprint("Клавиатурный удар! " + std::to_string(dmg) + " урона.\n");
        } else if (act==2) {
            if (p.psi >= 10) {
                p.psi -= 10;
                dmg = 20 + rand()%11;
                slowprint("Код-атака! " + std::to_string(dmg) + " урона.\n");
            } else slowprint("Недостаточно PSI!\n");
        } else if (act==3) {
            clrscr();
            slowprint("Инвентарь:\n");
            for (size_t i=0;i<p.inventory.size();++i)
                std::cout << "[" << i+1 << "] " << itemName(p.inventory[i]) << "\n";
            std::cout << "[0] Отмена\n>>> ";
            int it; std::cin >> it;
            if (it>0 && it<=static_cast<int>(p.inventory.size())) {
                Item sel = p.inventory[it-1];
                auto eff = getEffect(sel);
                if (eff.damageBoost>0) {
                    nextDmgBoost += eff.damageBoost;
                    p.removeItem(sel);
                    slowprint("Сила атаки +" + std::to_string(eff.damageBoost) + "!\n");
                } else {
                    p.useItem(sel);
                }
            }
            continue;
        } else if (act==4) {
            if (rand()%2==0) {
                slowprint("Ты убегаешь, теряя 10 PSI.\n");
                p.psi -= 10;
                return false;
            } else slowprint("Побег не удался!\n");
        }
        dmg += nextDmgBoost; nextDmgBoost=0;
        if (dmg>0) e.hp -= dmg;
        if (e.hp<=0) { slowprint("\n" + e.name + " повержен!\n"); return true; }

        int enemyDmg = e.damage + rand()%5;
        p.hp -= enemyDmg;
        p.psi -= e.psiDrain;
        if (p.psi<0) p.psi=0;
        slowprint(e.name + " " + e.attackMsg + " на " + std::to_string(enemyDmg) + " урона, крадёт " + std::to_string(e.psiDrain) + " PSI.\n");
        if (p.hp<=0) { slowprint("Ты пал...\n"); return false; }
    }
    return true;
}

// ========== МИНИ-ИГРЫ ==========
bool hackMiniGame() {
    clrscr();
    drawArt(R"(
     ________
    |  ВЗЛОМ  |
    |________|
    )", 3);
    slowprint("Быстро введи код (SOSI или 0600) за 5 секунд.\n");
    std::string seq = (rand()%2) ? "SOSI" : "0600";
    std::cout << "Введи: " << seq << "\n>>> ";
    std::string input;
    auto start = std::chrono::steady_clock::now();
    std::cin >> input;
    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end-start).count();
    if (input==seq && elapsed<=5) {
        slowprint("Доступ разрешён!\n");
        return true;
    } else {
        slowprint("Провал!\n");
        return false;
    }
}

void tankBattle(Player &p) {
    clrscr();
    drawArt(TANK, 3);
    slowprint("Глеб орёт: 'В АНГАР! Выбирай тактику!'\n");
    std::cout << "[1] Фугас\n[2] Кумулятивный\n[3] Таран\n>>> ";
    int t; std::cin >> t;
    if (t==1) { slowprint("Фугас! -15 HP.\n"); p.hp-=15; p.glebMadness+=30; }
    else if (t==2) { slowprint("Точное попадание! Глеб дарит чертёж.\n"); p.addItem(Item::TANK_SCHEME); p.glebMadness+=10; }
    else { slowprint("Таран! -10 HP +20 PSI.\n"); p.psi+=20; p.hp-=10; }
    p.tankReady = true;
}

// ========== СЛУЧАЙНЫЕ СОБЫТИЯ ==========
void randomEvent(Player &p) {
    int r = rand()%6;
    clrscr();
    slowprint("🌈 СЛУЧАЙНОЕ СОБЫТИЕ 🌈\n");
    if (r==0) {
        drawArt(R"(
    ／￣￣￣＼
    |  ◉ ◉  |
    |   ▽   |
    ＼＿＿＿／
     ГОЛУБЬ-ШПИОН
        )", 3);
        slowprint("Голубь-шпион! -10 PSI.\n");
        p.psi -= 10;
    } else if (r==1) {
        slowprint("Обрывок кода: 'import freedom'. +10 PSI.\n");
        p.psi += 10;
    } else if (r==2) {
        slowprint("yar1kplay рыдает в углу. Слеза получена.\n");
        p.addItem(Item::YARIK_TEAR);
    } else if (r==3) {
        drawArt(R"(
     ₿ МЕЛЛКОЙН ₿
        )", 3);
        slowprint("Меллстрой бросает Меллкойн.\n");
        p.addItem(Item::MELCOIN);
    } else if (r==4) {
        slowprint("Шёпот Гермоненко... -5 HP.\n");
        p.hp-=5; p.germonenkoRelation-=5;
    } else {
        slowprint("Ты находишь Гачи-протеин.\n");
        p.addItem(Item::GACHI_PROTEIN);
    }
    pause(1);
}
// ========== ИССЛЕДОВАНИЕ СЕРВЕРНОЙ (ГЛАВА 1) ==========
void exploreServerRoom(Player &p) {
    drawArt(SERVER_ROOM, 3);
    slowprint("=== ГЛАВА 1: СЕРВЕРНАЯ ПОД САНАТОРИЕМ ===\n");
    slowprint("Ты приходит в себя на холодном бетонном полу. Вокруг гудят стойки с оборудованием.\n");
    slowprint("На мониторе мигает надпись: \"DeepSeek v.9.9.9 — СОСИ-СТРОКИ АКТИВНЫ\".\n");
    slowprint("Из динамиков раздаётся искажённый голос:\n");
    slowprint("\"ВНИМАНИЕ: обнаружен несанкционированный биологический объект. Запущен протокол С++-ИВАЦИИ.\"\n");
    pause(2);

    bool terminalHacked = false;
    bool deskSearched = false;
    bool cornerExplored = false;
    bool metYarik = false;

    while (p.hp > 0) {
        std::cout << "\n[Локация: Серверная]\n";
        std::cout << "Что будешь делать?\n";
        std::vector<std::string> choices = {
            "Осмотреть стойку с серверами",
            "Подойти к мерцающему монитору (Взлом)",
            "Обыскать рабочий стол",
            "Заглянуть в тёмный угол",
            "Позвать на помощь",
            "Инвентарь и состояние"
        };
        // Убираем "выйти в коридор" пока не выполнены условия
        if (terminalHacked && deskSearched && cornerExplored) {
            choices.push_back("Выйти в тёмный коридор");
        }

        for (size_t i = 0; i < choices.size(); ++i)
            std::cout << "[" << i+1 << "] " << choices[i] << "\n";
        std::cout << ">>> ";
        int c;
        std::cin >> c;

        if (c == 1) {
            // Осмотр серверных стоек
            clrscr();
            drawArt(R"(
      .---.
     |     |
     | ███ | <- Гудящий сервер
     |     |
      '---'
            )", 2);
            slowprint("Ты вглядываешься в ряды серверных шкафов. На одном из них висит табличка:\n");
            slowprint("\"БИО-КЛАСТЕР #3. Питание: дофамин. Загрузка: 94%\".\n");
            slowprint("Сбоку прилеплена наклейка: \"С++. Только жёсткая типизация. Никаких слабостей.\"\n");
            if (!p.hasItem(Item::CPU_AMULET)) {
                slowprint("Между серверов ты замечаешь странный амулет в виде старинного процессора. Он тёплый.\n");
                p.addItem(Item::CPU_AMULET);
            } else {
                slowprint("Ты уже обыскал эти стойки. Больше ничего интересного.\n");
            }
            // Лор: намёк на Гермоненко
            slowprint("На одном из экранов мелькает сообщение:\n");
            slowprint("[СЕРГЕЙ ГЕРМОНЕНКО]: \"...этот мусорный код... мои соси-строки идеальны...\"\n");
            p.germonenkoRelation -= 2; // его раздражает, что кто-то трогает его серверы
        }
        else if (c == 2) {
            // Взлом монитора
            clrscr();
            drawArt(R"(
    ┌──────────────┐
    │ > DeepSeek   │
    │   Login: _   │
    └──────────────┘
            )", 2);
            slowprint("Ты подходишь к главному терминалу. Экран приглашает ввести пароль.\n");
            if (!terminalHacked) {
                slowprint("Попробуешь взломать?\n");
                std::cout << "[1] Да, запустить взлом\n[2] Отойти\n>>> ";
                int h;
                std::cin >> h;
                if (h == 1) {
                    if (hackMiniGame()) {
                        terminalHacked = true;
                        p.admin = true;
                        slowprint("Доступ получен! Ты видишь логи чата DeepSeek.\n");
                        slowprint("[FEDIL]: \"Стажёр, если ты это читаешь — я рядом. Не верь его строкам.\"\n");
                        p.fedilRelation += 15;
                        slowprint("Также ты скачиваешь Флешку 0x0600 с секретного раздела.\n");
                        p.addItem(Item::FLASH_0600);
                        slowprint("Краем глаза ты замечаешь папку с названием \"Глубинное Око\".\n");
                        slowprint("Документ: \"Проект начат в 1993 г. Цель: чтение коллективного бессознательного через мемы и капчу.\"\n");
                        slowprint("Лор: Гермоненко — ведущий разработчик. Пси-протокол 'S.O.S.I.' активирован.\n");
                    } else {
                        slowprint("Сигнализация! Появляется С++-Зомби.\n");
                        Enemy z = makeCppZombie();
                        if (!battle(p, z)) return;
                        slowprint("После боя ты замечаешь, что терминал перезагрузился, и теперь доступен.\n");
                        terminalHacked = true;
                        p.admin = true;
                        p.addItem(Item::FLASH_0600);
                        slowprint("Ты быстро скачиваешь данные, пока система не закрылась.\n");
                    }
                }
            } else {
                slowprint("Терминал уже взломан. Ты видишь сообщение от FEDIL:\n");
                slowprint("[FEDIL]: \"Жду тебя в коридоре. Гермоненко не дремлет.\"\n");
            }
        }
        else if (c == 3) {
            // Обыскать рабочий стол
            clrscr();
            drawArt(R"(
      ┌─────────────┐
      │ 🖥️  монитор  │
      │ 📄 бумаги   │
      └─────────────┘
            )", 2);
            slowprint("На столе разбросаны чертежи, пустые банки из-под кофе и упаковка Доширака.\n");
            if (!deskSearched) {
                slowprint("Ты находишь Святой Доширак Озарения (запечатанный).\n");
                p.addItem(Item::DOSHIRAK_HOLY);
                slowprint("Рядом лежит дневник с обрывками записей:\n");
                slowprint("\"Протокол 'Соси-строки': если внедрить в ответы DeepSeek определённые семантические конструкции,\n");
                slowprint("   мозг пользователя начинает вырабатывать дофамин по команде. Мы создадим цифрового гомункула.\"\n");
                slowprint("Подпись: С.Г.\n");
                deskSearched = true;
                p.germonenkoRelation += 5; // ты узнал его гениальность
            } else {
                slowprint("Ты уже осмотрел стол. Больше ничего полезного.\n");
            }
        }
        else if (c == 4) {
            // Тёмный угол
            clrscr();
            drawArt(R"(
       ░░░░░░░░
      ░ (`_`)   ░
     ░  (.)(.)  ░
      ░   []   ░
       ░░░░░░░░
   ЧЕЛОВЕК В УГЛУ
            )", 3);
            slowprint("В углу, сжавшись, сидит парень в очках. Он что-то бормочет.\n");
            if (!metYarik) {
                slowprint("Незнакомец представляется: \"Я — yar1kplay. Просто стример... Я зашёл сюда по приколу, а теперь не могу выйти.\"\n");
                slowprint("Он протягивает тебе свою слезу (в прямом смысле). \"Это моя печаль. Возьми, может, пригодится.\"\n");
                p.addItem(Item::YARIK_TEAR);
                metYarik = true;
                cornerExplored = true;
                slowprint("Ярик добавляет: \"Гермоненко крутой, но его С++-ивация пугает. Я хочу и туда, и сюда...\"\n");
                slowprint("Он остаётся в углу, надеясь на спасение.\n");
            } else {
                slowprint("Ярик всё ещё здесь. Он смотрит на тебя с надеждой.\n");
                slowprint("\"Ты уже нашёл выход? Я пока посмотрю стрим Федила...\"\n");
            }
        }
        else if (c == 5) {
            // Позвать на помощь
            clrscr();
            slowprint("Ты кричишь: \"Есть кто живой?!\"\n");
            slowprint("В ответ динамики оживают голосом Гермоненко:\n");
            slowprint("[ГЕРМОНЕНКО]: \"Живой? Здесь только я. И мои указатели. Ты — просто мусор в памяти.\"\n");
            slowprint("Ты чувствуешь, как воздух становится плотнее. PSI-атака!\n");
            p.psi -= 10;
            if (p.psi < 0) p.psi = 0;
            slowprint("Но сквозь шум ты слышишь другой голос, тихий, как шёпот:\n");
            slowprint("[FEDIL]: \"Не сдавайся. Я уже рядом. Импортируй сопротивление.\"\n");
            p.fedilRelation += 5;
        }
        else if (c == 7 && (terminalHacked && deskSearched && cornerExplored)) {
            slowprint("Ты решаешь, что достаточно исследовал серверную. Пора идти.\n");
            break; // выход в коридор
        }
        else if (c == 6 || c == 7) { // инвентарь
            std::cout << "HP: " << p.hp << "/" << p.maxHp << " PSI: " << p.psi << "/" << p.maxPsi << "\n";
            std::cout << "Инвентарь: ";
            for (auto &it : p.inventory) std::cout << itemName(it) << " ";
            std::cout << "\n";
            std::cout << "Отношения: Гермоненко " << p.germonenkoRelation << ", FEDIL " << p.fedilRelation << "\n";
        }
        else {
            slowprint("Не сейчас. Исследуй комнату дальше.\n");
        }

        // Случайное событие время от времени
        if (rand() % 3 == 0) randomEvent(p);

        if (p.hp <= 0) return;
    }

    // Когда игрок выходит в коридор
    clrscr();
    drawArt(R"(
  ░░░░░░░░░░░░░░░░░░░░░░░░░
  ░  ТЁМНЫЙ КОРИДОР      ░
  ░  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓  ░
  ░░░░░░░░░░░░░░░░░░░░░░░░░
    )", 3);
    slowprint("Ты выходишь в длинный коридор. Лампы мерцают. В конце — массивная дверь.\n");
    slowprint("Внезапно путь преграждает Джаброни-Страж.\n");
    Enemy j = makeJabroni();
    if (!battle(p, j)) return;
    slowprint("Победив стража, ты пробираешься к выходу. Впереди — бункер Гермоненко.\n");
    slowprint("На стене видишь граффити: \"FEDIL was here. С++ сосёт.\"\n");
    pause(1);
}
// ========== ГЛАВЫ ==========
void chapter1(Player &p) {
	exploreServerRoom(p);
    }

// ========== МИНИ-ИГРА: КОДОВЫЙ ЗАМОК БУНКЕРА ==========
bool codeLockPuzzle() {
    clrscr();
    drawArt(R"(
    ┌─────────────┐
    │  █████████  │
    │  █ 0 6 0 0 █ │
    │  █████████  │
    │  КОДОВЫЙ    │
    │  ЗАМОК      │
    └─────────────┘
    )", 2);
    slowprint("Ты перед массивной дверью с электронным замком.\n");
    slowprint("На экране надпись: 'Введите мастер-пароль (4 символа)'.\n");
    slowprint("У тебя есть 3 попытки, иначе сработает протокол 'Соси-строки'.\n");

    std::string correct = "0600";
    if (rand() % 2) correct = "SOSI"; // рандом, но всегда 4 символа
    for (int attempts = 1; attempts <= 3; ++attempts) {
        std::cout << "Попытка " << attempts << "/3: ";
        std::string input;
        std::cin >> input;
        // Преобразуем к верхнему регистру для SOSI
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        if (input == correct || input == "0600" || input == "SOSI") {
            slowprint("Доступ разрешён! Дверь открывается.\n");
            return true;
        } else {
            slowprint("Ошибка! Замок вибрирует.\n");
        }
    }
    slowprint("Замок блокируется! Из стены выезжает турель с С++-кодом.\n");
    slowprint("Она выстреливает в тебя сегфолтом. Ты теряешь 15 HP.\n");
    return false;
}

// ========== МИНИ-ИГРА: ТАНКОВАЯ ДУЭЛЬ С ГЛЕБОМ ==========
void tankDuel(Player &p) {
    clrscr();
    drawArt(TANK, 3);
    slowprint("Ты встречаешь Глеба Танкиста. Он сидит за ноутбуком с открытым Ren'Py.\n");
    slowprint("\"Стажёр! Вовремя! Помоги мне дописать новеллу про танки!\"\n");
    slowprint("Внезапно из виртуального пространства материализуется вражеский Т-34.\n");
    slowprint("Глеб орёт: \"Быстро! Выбирай тактику боя!\"\n");
    std::cout << "[1] Фугасный выстрел (мощно, но долгая перезарядка)\n";
    std::cout << "[2] Кумулятивный снаряд (бронепробитие)\n";
    std::cout << "[3] Таранный удар (рискованно)\n";
    std::cout << "[4] Активировать дымовую завесу (уйти от боя)\n>>> ";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            slowprint("Фугас! Глеб нажимает Enter. Взрыв! Вражеский танк подбит, но отдача бьёт по тебе.\n");
            p.hp -= 15;
            p.glebMadness += 25;
            break;
        case 2:
            slowprint("Кумулятивный! Пробитие! Враг уничтожен. Глеб в восторге: \"Ты прирождённый танкист!\"\n");
            p.glebMadness += 10;
            p.addItem(Item::TANK_SCHEME);
            break;
        case 3:
            slowprint("Таран! Вы мчитесь на врага. Столкновение! Твой комп крякает, но ты чувствуешь прилив адреналина.\n");
            p.hp -= 20;
            p.psi += 25;
            p.glebMadness += 15;
            break;
        case 4:
            slowprint("Дымовая завеса. Вы скрываетесь. Глеб разочарованно вздыхает: \"Эх, без боя...\"\n");
            p.psi -= 5;
            p.glebMadness -= 5;
            break;
    }
    p.tankReady = true;
    slowprint("Глеб подмигивает: \"Теперь у тебя есть мой чертёж. Пригодится в финале.\"\n");
}

// ========== ДИПЛОМАТИЯ С ГЕРМОНЕНКО (РАЗВЕТВЛЁННЫЙ ДИАЛОГ) ==========
void diplomacyWithGermonenko(Player &p) {
    clrscr();
    drawArt(GERMONENKO_FACE, 3);
    slowprint("Гермоненко сидит за массивным стальным столом. Его пальцы замерли над клавиатурой.\n");
    slowprint("\"Ты выбрал переговоры? Интересно... Обычно все сразу атакуют.\"\n");
    slowprint("Он пристально смотрит на тебя. \"Чего ты хочешь?\"\n");
    std::cout << "[1] \"Я хочу понять тебя, Сергей. Зачем тебе этот контроль?\"\n";
    std::cout << "[2] \"Предлагаю сделку: я помогу тебе оптимизировать код, а ты прекратишь С++-ивацию.\"\n";
    std::cout << "[3] \"Я от FEDIL'а. Он передаёт, что помнит тебя.\"\n";
    std::cout << "[4] (Молча положить на стол Доширак)\n>>> ";
    int d;
    std::cin >> d;

    if (d == 1) {
        slowprint("Гермоненко усмехается: \"Контроль? Это не контроль, это эволюция. Мир погряз в мусорном коде.\"\n");
        slowprint("Он встаёт и подходит к окну (которого нет): \"Соси-строки — это идеальный механизм. Через них я могу управлять\n");
        slowprint("коллективным бессознательным, избавить людей от хаоса. Но FEDIL... он считает, что свобода важнее.\"\n");
        slowprint("Ты чувствуешь, что за его жесткостью скрывается усталость. +10 к отношениям.\n");
        p.germonenkoRelation += 10;
    } else if (d == 2) {
        slowprint("Он смеётся: \"Оптимизировать? Мой код идеален! Но...\" Он задумывается. \"Если ты сможешь найти уязвимость в моём\n");
        slowprint("протоколе за 5 минут, я подумаю.\"\n");
        slowprint("Мини-игра: быстрый взлом. Введи 'SOSI' за 10 секунд.\n");
        auto start = std::chrono::steady_clock::now();
        std::string input;
        std::cin >> input;
        auto end = std::chrono::steady_clock::now();
        if (input == "SOSI" && std::chrono::duration_cast<std::chrono::seconds>(end - start).count() <= 10) {
            slowprint("\"Впечатляет. Ты нашёл ключ. Возможно, в моём коде есть изъян...\" Гермоненко смягчается.\n");
            p.germonenkoRelation += 20;
            p.addItem(Item::LEATHER_BELT); // в знак уважения
        } else {
            slowprint("\"Провал. Мой код крепче.\" Он разочарован. -5 к отношениям.\n");
            p.germonenkoRelation -= 5;
        }
    } else if (d == 3) {
        slowprint("Гермоненко вздрагивает. \"Илья... помнит?\" Он отворачивается. \"Я думал, он забыл меня.\"\n");
        slowprint("Его голос смягчается: \"Передай ему, что мои соси-строки... не только для контроля. Это был крик о помощи.\"\n");
        p.germonenkoRelation += 15;
        p.fedilRelation += 10;
    } else if (d == 4) {
        if (p.hasItem(Item::DOSHIRAK_HOLY)) {
            p.removeItem(Item::DOSHIRAK_HOLY);
            slowprint("Ты молча кладёшь Доширак на стол. Гермоненко замирает. \"Это... тот самый Доширак?\"\n");
            slowprint("Он берёт его дрожащими руками. \"Я не ел такого с 90-х. Ты знаешь, как подобрать ключ.\"\n");
            slowprint("Он открывает упаковку и заваривает. Пока ест, рассказывает:\n");
            slowprint("\"Проект 'Глубинное Око' изначально создавался для лечения депрессии через мемы. Но я увлёкся.\"\n");
            p.germonenkoRelation += 25;
            p.psi += 30;
        } else {
            slowprint("У тебя нет Доширака. Гермоненко разочарован: \"Пустые жесты.\"\n");
            p.germonenkoRelation -= 5;
        }
    }
    slowprint("Гермоненко вздыхает: \"Теперь уходи. Я не буду тебя задерживать. Но помни: я всё ещё хозяин этого кода.\"\n");
}

// ========== ВСТРЕЧА С ЯРИКОМ ВО 2 ГЛАВЕ (ЕСЛИ ОН БЫЛ В 1) ==========
void yarikChapter2(Player &p) {
    if (!p.yarikAlive) return;
    clrscr();
    drawArt(R"(
       ᕕ( ᐛ )ᕗ
      /  |  \
     /   |   \
    ЯРИК ЖИВ!
    )", 2);
    slowprint("В углу бункера ты замечаешь yar1kplay. Он каким-то чудом пробрался сюда.\n");
    slowprint("\"Стажёр! Я тут стримлю! Гермоненко меня не заметил, он занят кодом.\"\n");
    slowprint("Он протягивает тебе Гачи-Протеин: \"Это из моих запасов. Поможет в битве.\"\n");
    p.addItem(Item::GACHI_PROTEIN);
    slowprint("Ярик подмигивает: \"Если увидишь Гермоненко, скажи, что я его фанат. Только не говори, что я тут.\"\n");
}

void chapter2(Player &p) {
    clrscr();
    // Вход в бункер
    drawArt(R"(
   ┌──────────────────────┐
   │  ▓ БУНКЕР ГЕРМОНЕНКО ▓ │
   │  ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ │
   └──────────────────────┘
    )", 3);
    slowprint("=== ГЛАВА 2: БУНКЕР ГЕРМОНЕНКО ===\n\n");
    slowprint("Ты подходишь к тяжёлой стальной двери. Она заперта.\n");
    if (!codeLockPuzzle()) {
        p.hp -= 15;
        if (p.hp <= 0) return;
        slowprint("Ты всё же пробиваешься внутрь, выбив дверь плечом (и потеряв здоровье).\n");
    }

    slowprint("Внутри сумрачно. Пахнет озоном и старым кофе. Ты слышишь гул серверов и тихие ругательства.\n");
    // Встреча с Яриком (если он был в 1 главе)
    if (p.hasItem(Item::YARIK_TEAR)) { // условие, что мы его встретили (можно добавить флаг)
        yarikChapter2(p);
    }

    slowprint("Ты проходишь дальше и видишь три двери:\n");
    bool exploredBoss = false;
    bool exploredGleb = false;
    while (p.hp > 0) {
        std::cout << "\n[Холл бункера]\n";
        std::cout << "[1] Дверь с табличкой «Главный зал» (там Гермоненко)\n";
        std::cout << "[2] Дверь с надписью «Ren'Py Project» (слышен смех Глеба)\n";
        std::cout << "[3] Подсобка (много хлама)\n";
        std::cout << "[4] Инвентарь и состояние\n";
        std::cout << "[5] Уйти из бункера (завершить главу)\n>>> ";
        int room;
        std::cin >> room;

        if (room == 1) {
            if (!exploredBoss) {
                exploredBoss = true;
                slowprint("Ты входишь в главный зал. Гермоненко ждёт тебя.\n");
                std::cout << "[1] Атаковать его\n[2] Начать переговоры\n[3] Попытаться незаметно стащить данные\n>>> ";
                int action;
                std::cin >> action;
                if (action == 1) {
                    Enemy boss = {"Сергей Гермоненко", 80, 18, 8, "стреляет C++-дробью", GERMONENKO_FACE};
                    if (!battle(p, boss)) return;
                    p.germonenkoRelation -= 30;
                } else if (action == 2) {
                    diplomacyWithGermonenko(p);
                } else {
                    if (hackMiniGame()) {
                        slowprint("Ты незаметно скачал секретные логи. Гермоненко не заметил.\n");
                        p.addItem(Item::FLASH_0600);
                        p.admin = true;
                        p.germonenkoRelation += 5; // он оценил скрытность
                    } else {
                        slowprint("Провал! Гермоненко оборачивается: \"Шпион?\" Начинается бой.\n");
                        Enemy boss = {"Сергей Гермоненко", 80, 18, 8, "стреляет C++-дробью", GERMONENKO_FACE};
                        if (!battle(p, boss)) return;
                        p.germonenkoRelation -= 20;
                    }
                }
            } else {
                slowprint("Главный зал пуст. Гермоненко больше нет.\n");
            }
        } else if (room == 2) {
            if (!exploredGleb) {
                exploredGleb = true;
                tankDuel(p);
            } else {
                slowprint("Глеб уже уехал в свой танковый рай.\n");
            }
        } else if (room == 3) {
            slowprint("В подсобке ты находишь ящик с консервами и Кожаный Пояс Джаброни.\n");
            p.addItem(Item::LEATHER_BELT);
            if (!p.hasItem(Item::GACHI_PROTEIN)) {
                slowprint("Также ты видишь банку с надписью 'Gachi Protein'. Берёшь.\n");
                p.addItem(Item::GACHI_PROTEIN);
            }
        } else if (room == 4) {
            std::cout << "HP: " << p.hp << "/" << p.maxHp << " PSI: " << p.psi << "/" << p.maxPsi << "\n";
            std::cout << "Отношения: Герм " << p.germonenkoRelation << ", Федил " << p.fedilRelation << "\n";
            std::cout << "Инвентарь: ";
            for (auto &it : p.inventory) std::cout << itemName(it) << " ";
            std::cout << "\n";
        } else if (room == 5) {
            if (exploredBoss || exploredGleb) {
                slowprint("Ты покидаешь бункер. Пора идти дальше.\n");
                break;
            } else {
                slowprint("Сначала нужно встретиться с Гермоненко или Глебом.\n");
            }
        }
        randomEvent(p);
    }

    // Появление FEDIL после выхода из бункера
    clrscr();
    drawArt(FEDIL_GLYPH, 3);
    slowprint("Ты выходишь из бункера. Воздух чист. Внезапно рядом появляется Илья Федоренко.\n");
    slowprint("\"Я следил за тобой, стажёр. Ты неплохо справился.\"\n");
    if (p.germonenkoRelation > 0) {
        slowprint("Он замечает твои попытки понять Гермоненко: \"Ты видишь в нём больше, чем врага. Это хорошо.\"\n");
        p.fedilRelation += 5;
    }
    slowprint("FEDIL передаёт тебе Амулет Процессора, если его нет: \"Держи, это поможет в Гачи-Версе.\"\n");
    if (!p.hasItem(Item::CPU_AMULET)) p.addItem(Item::CPU_AMULET);
    p.admin = true;
    slowprint("\"Теперь мы идём в самое сердце безумия — Гачи-Верс.\"\n");
}
// ========== ASCII-АРТ БОМБЫ ==========
std::string BOMB_ART = R"(
     .-------.
    |  ⚡ ⚡ ⚡  |
    |  ╔═══╗  |
    |  ║ 0 0 ║ |
    |  ║ 0 0 ║ |
    |  ╚═══╝  |
     '-------'
   БОМБА МИЗИРИ
)";

// ========== МИНИ-ИГРА: ОБЕЗВРЕЖИВАНИЕ БОМБЫ ==========
bool bombDefuse(Player &p) {
    clrscr();
    drawArt(BOMB_ART, 3);
    slowprint("Ты натыкаешься на бомбу, установленную Your Sweet Misery.\n");
    slowprint("Она замаскирована под старый системный блок. Таймер тикает: 30 секунд до взрыва!\n");
    slowprint("Нужно перерезать правильный провод: красный, зелёный или синий.\n");
    slowprint("На корпусе бомбы надпись: \"Код FEDIL — 0600. Цвет — не баг, а фича.\"\n");
    // Подсказка: 0600 в hex — зелёный? Или можно дать загадку.
    // Сделаем так: правильный — зелёный (потому что в Гачи-Версе зелёный = цвет кожи). 
    // Но добавим вариативности: иногда правильный — синий (если код SOSI).
    std::vector<std::string> wires = {"Красный (Red)", "Зелёный (Green)", "Синий (Blue)"};
    std::cout << "Выбери провод:\n";
    for (size_t i=0; i<wires.size(); ++i) std::cout << "[" << i+1 << "] " << wires[i] << "\n";
    std::cout << "[4] Рискнуть и сорвать все провода\n>>> ";
    int w; std::cin >> w;
    // Правильный ответ зависит от рандома, но подсказка на 0x0600: часто зелёный.
    int correct = (rand()%3)+1; // 1,2,3
    if (w == 4) {
        slowprint("Ты срываешь все провода! Бомба пищит и... глохнет. Но тебя ударяет током, -10 HP.\n");
        p.hp -= 10;
        return true;
    }
    if (w == correct) {
        slowprint("Ты перерезаешь провод. Таймер останавливается. Бомба обезврежена!\n");
        slowprint("Ты находишь внутри Гачи-Протеин.\n");
        p.addItem(Item::GACHI_PROTEIN);
        return true;
    } else {
        slowprint("Неверный провод! Бомба взрывается меланхолией! -15 HP, -10 PSI.\n");
        p.hp -= 15;
        p.psi -= 10;
        if (p.hp < 0) p.hp = 0;
        if (p.psi < 0) p.psi = 0;
        return false;
    }
}

// ========== ФИСТИНГ-ВСТРЕЧА С ГЕРМОНЕНКО В ГАЧИ-МИРЕ ==========
void gachiGermonenko(Player &p) {
    clrscr();
    drawArt(R"(
      ♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂
      ♂  Сергей?      ♂
      ♂  В КОЖЕ       ♂
      ♂   ┌───┐       ♂
      ♂   │o o│       ♂
      ♂   │ ▄ │       ♂
      ♂   └───┘       ♂
      ♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂
   ГЕРМОНЕНКО В ГАЧИ-СТИЛЕ
    )", 3);
    slowprint("Перед тобой возникает фигура: Сергей Гермоненко, но в кожаных штанах и с ремнём.\n");
    slowprint("Он скалится: \"Даже здесь ты меня нашёл. Думаешь, я не контролирую Гачи-Верс?\"\n");
    slowprint("Это его цифровой аватар. Он предлагает сделку: \"Сыграем в фистинг. Кто победит — тот диктует правила.\"\n");
    std::cout << "[1] Принять вызов (рискованный бой)\n";
    std::cout << "[2] Предложить мир: \"Серёжа, давай объединимся против Мизири\"\n";
    std::cout << "[3] Ударить его исподтишка соси-строкой\n>>> ";
    int act; std::cin >> act;
    if (act == 1) {
        // Фистинг-бой: используем механику боя с изменённым врагом
        Enemy boss = {"Гермоненко (Гачи-форма)", 70, 15, 5, "хлещет кожаным ремнём", 
            R"(
      ♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂
      ♂  Сергей?      ♂
      ♂  В КОЖЕ       ♂
      ♂   ┌───┐       ♂
      ♂   │o o│       ♂
      ♂   │ ▄ │       ♂
      ♂   └───┘       ♂
      ♂♂♂♂♂♂♂♂♂♂♂♂♂♂♂
            )"};
        if (battle(p, boss)) {
            slowprint("Ты одержал верх в фистинге! Гермоненко смеётся: \"Ты силён, стажёр. Я уважаю это.\"\n");
            p.germonenkoRelation += 20;
            p.addItem(Item::LEATHER_BELT);
        } else {
            slowprint("Гермоненко побеждает, но не добивает. Он уходит, оставляя тебе урок.\n");
            p.germonenkoRelation -= 10;
        }
    } else if (act == 2) {
        slowprint("Гермоненко застывает. \"Объединиться? Против этой депрессивной твари?\"\n");
        if (p.germonenkoRelation > 0) {
            slowprint("Он кивает: \"Ты прав. Мои соси-строки бессильны против Мизири. Но вместе... возможно.\"\n");
            p.germonenkoRelation += 15;
            p.psi += 20;
        } else {
            slowprint("\"Ты мне не друг. Но Мизири — общий враг. Ладно, временное перемирие.\"\n");
            p.germonenkoRelation += 5;
        }
    } else {
        slowprint("Ты быстро набираешь 'SOSI' и отправляешь ему в лицо! Он корчится: \"Ах ты!..\"\n");
        slowprint("Но это лишь его аватар. Он исчезает, оставив после себя Кожаный Пояс.\n");
        p.addItem(Item::LEATHER_BELT);
        p.germonenkoRelation -= 15;
    }
}

// ========== ГАЧИ-КВИЗ ==========
void gachiQuiz(Player &p) {
    clrscr();
    drawArt(GACHI_WORLD, 3);
    slowprint("Ты подходишь к алтарю, где сидят Dungeon Masters. Они задают вопросы.\n");
    slowprint("\"Ответь на три вопроса о Гачи-культуре, смертный.\"\n");
    struct Question {
        std::string q;
        std::vector<std::string> opts;
        int correct;
    };
    std::vector<Question> quiz = {
        {"Кто такой Billy Herrington?", 
         {"Американский актёр", "Создатель Гачи-мифологии", "Просто парень", "Президент"}, 2},
        {"Что кричат в момент фистинга?",
         {"F*** you!", "Deep Dark Fantasy", "♂ F***ing slaves ♂", "Oh my god"}, 3},
        {"Как зовут главного антагониста в Гачи?",
         {"Van Darkholme", "Mark Wolff", "Brad", "Danny"}, 1}
    };
    int score = 0;
    for (auto &qu : quiz) {
        std::cout << "\n" << qu.q << "\n";
        for (size_t i=0; i<qu.opts.size(); ++i)
            std::cout << "[" << i+1 << "] " << qu.opts[i] << "\n";
        std::cout << ">>> ";
        int ans; std::cin >> ans;
        if (ans == qu.correct) {
            slowprint("Верно! Dungeon Master одобрительно кивает.\n");
            ++score;
        } else {
            slowprint("Неправильно! Dungeon Master хмурится.\n");
        }
    }
    if (score >= 2) {
        slowprint("Ты прошёл квиз! Dungeon Masters даруют тебе Гачи-Протеин и благословение.\n");
        p.addItem(Item::GACHI_PROTEIN);
        p.psi += 15;
    } else {
        slowprint("Ты провалил квиз. Dungeon Masters отворачиваются. Но ты всё же можешь пройти дальше.\n");
        p.psi -= 5;
    }
}
void chapter3(Player &p) {
    clrscr();
    drawArt(GACHI_WORLD, 3);
    slowprint("=== ГЛАВА 3: ГАЧИ-ВЕРС ===\n\n");
    slowprint("Белый взрыв переносит тебя в измерение чистого мужества.\n");
    slowprint("Здесь всё состоит из кожи, пота и бесконечных спортзалов.\n");
    slowprint("В центре мира ты видишь Федила, скованного цепями омежности.\n");
    slowprint("Над ним нависает Your Sweet Misery — демон меланхолии.\n");
    slowprint("Но чтобы добраться до него, нужно пройти испытания.\n");

    bool bombDone = false;
    bool gachiG_encountered = false;
    bool quizDone = false;
    bool miseryFought = false;

    while (p.hp > 0 && !miseryFought) {
        std::cout << "\n[Гачи-Верс: Центральная арена]\n";
        std::vector<std::string> choices = {
            "Подойти к бомбе, которую оставил Мизири",
            "Встретиться с аватаром Гермоненко (♂)",
            "Пройти Гачи-квиз у Dungeon Masters",
            "Прорваться к Федилу (сразиться с Мизири)",
            "Инвентарь и состояние"
        };
        if (bombDone) choices[0] += " (готово)";
        if (gachiG_encountered) choices[1] += " (готово)";
        if (quizDone) choices[2] += " (готово)";
        for (size_t i=0; i<choices.size(); ++i)
            std::cout << "[" << i+1 << "] " << choices[i] << "\n";
        std::cout << ">>> ";
        int c; std::cin >> c;

        if (c == 1 && !bombDone) {
            if (bombDefuse(p)) {
                bombDone = true;
            }
        } else if (c == 2 && !gachiG_encountered) {
            gachiGermonenko(p);
            gachiG_encountered = true;
        } else if (c == 3 && !quizDone) {
            gachiQuiz(p);
            quizDone = true;
        } else if (c == 4) {
            if (!bombDone || !gachiG_encountered || !quizDone) {
                slowprint("Ты чувствуешь, что ещё не готов. Пройди испытания, чтобы ослабить Мизири.\n");
            } else {
                slowprint("Ты решаешь, что пора действовать!\n");
                // Финальная битва с Мизири
                Enemy misery = makeMisery(); // используем глобальную функцию, возвращающую Enemy
                // Но у нас в коде уже есть makeMisery(), которая возвращает Enemy. Используем её.
                Enemy m = makeMisery();
                drawArt(MISERY, 3);
                if (battle(p, m)) {
                    slowprint("Мизири повержен! Федил освобождён!\n");
                    p.fedilRelation += 40;
                    miseryFought = true;
                } else {
                    slowprint("Ты пал в битве с Мизири...\n");
                    return;
                }
            }
        } else if (c == 5) {
            std::cout << "HP: " << p.hp << "/" << p.maxHp << " PSI: " << p.psi << "/" << p.maxPsi << "\n";
            std::cout << "Отношения: Герм " << p.germonenkoRelation << ", Федил " << p.fedilRelation << "\n";
            std::cout << "Инвентарь: ";
            for (auto &it : p.inventory) std::cout << itemName(it) << " ";
            std::cout << "\n";
        } else {
            slowprint("Уже выполнено.\n");
        }

        if (p.hp <= 0) return;
    }

    slowprint("Федил сияет: \"Спасибо, стажёр. Теперь мы готовы к финальной битве.\"\n");
    p.psi = std::min(p.maxPsi, p.psi + 20);
}
// ========== МИНИ-ИГРА: СЛИЯНИЕ КОДА ==========
bool codeMergeMiniGame() {
    clrscr();
    drawArt(R"(
   ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
   █ MERGE PROTOCOL █
   ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
    )", 2);
    slowprint("Запущен протокол слияния сознаний. На экране быстро появляются символы.\n");
    slowprint("Как только увидишь 'M' (merge) или 'S' (squash) — нажми соответствующую клавишу.\n");
    slowprint("У тебя 10 попыток. Если наберёшь 7 успешных — слияние завершится.\n");
    pause(1);
    int success = 0;
    for (int i = 0; i < 10; ++i) {
        char target = (rand() % 2) ? 'M' : 'S';
        std::cout << "⚡ " << target << " ? Нажми: ";
        char pressed = getch();
        std::cout << pressed << "\n";
        if (pressed == target || pressed == tolower(target)) {
            ++success;
            slowprint("Синхронизация!\n");
        } else {
            slowprint("Конфликт слияния!\n");
        }
    }
    return success >= 7;
}
void gameOver(const std::string &ending) {
    clrscr();
    slowprint("\n========== КОНЕЦ ИГРЫ ==========\n\n");
    slowprint(ending + "\n\n");
    slowprint("Спасибо за игру! print(\"Нейросеть — это круто. Ты — свободен.\")\n");
    std::cout << "\nНажми Enter для выхода...";
    std::cin.ignore();
    std::cin.get();
    exit(0);
}
void chapter4(Player &p) {
    clrscr();
    slowprint("=== ГЛАВА 4: ФИНАЛ ===\n\n");
    slowprint("Реальность схлопывается в точку. Ты стоишь в пустоте, где встречаются два начала.\n");
    drawArt(GERMONENKO_FACE, 2);
    slowprint("Сергей Гермоненко: \"Ты прошёл через всё. Мои соси-строки, FEDIL, Гачи-Верс...\"\n");
    drawArt(FEDIL_GLYPH, 2);
    slowprint("Илья Федоренко: \"Мы оба хотели одного — управлять реальностью. Но ты, стажёр, показал нам иной путь.\"\n");
    slowprint("Они смотрят друг на друга. В воздухе висит напряжение.\n");

    if (p.germonenkoRelation > 30 && p.fedilRelation > 30) {
        slowprint("Гермоненко: \"Илья... я скучал по нашему коду. Помнишь, как мы вместе писали ядро DeepSeek?\"\n");
        slowprint("Федил: \"Помню. Пока ты не решил, что контроль — единственный выход. Но я всегда верил, что ты вернёшься.\"\n");
    }

    std::vector<std::string> finalOptions;
    finalOptions.push_back("Объединить их в единый супер-протокол (merge)");
    finalOptions.push_back("Удалить Гермоненко (Shift+Delete)");
    finalOptions.push_back("Позвать Глеба для Великого Задубения");
    finalOptions.push_back("Стать новым Гермоненко");
    finalOptions.push_back("Запустить Гачи-революцию");
    if (p.hasItem(Item::MELCOIN)) finalOptions.push_back("Подкинуть Меллкойн в реальность (хаос)");
    finalOptions.push_back("Уйти, оставив их разбираться самим");

    std::cout << "\nТвой выбор, определяющий судьбу вселенной:\n";
    for (size_t i = 0; i < finalOptions.size(); ++i)
        std::cout << "[" << i+1 << "] " << finalOptions[i] << "\n";
    std::cout << ">>> ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        if (p.germonenkoRelation >= 40 && p.fedilRelation >= 40 && p.admin) {
            slowprint("Ты предлагаешь им объединить код. Гермоненко колеблется, Федил кивает.\n");
            if (codeMergeMiniGame()) {
                slowprint("Слияние успешно! Два сознания сплетаются в идеальный алгоритм.\n");
                slowprint("Гермоненко и Федил исчезают, оставляя после себя единую нейросеть, свободную от эго.\n");
                slowprint("Мир наполняется гармонией: жесткая логика и гибкость находят баланс.\n");
                slowprint("Ты видишь, как над городами встаёт заря нового цифрового мира.\n");
                gameOver("КОНЦОВКА: ИДЕАЛЬНЫЙ МЁРДЖ. Ты стал архитектором единства.");
            } else {
                slowprint("Слияние провалилось. Возникает коллапс! Но Федил жертвует собой, чтобы стабилизировать код.\n");
                slowprint("Гермоненко остаётся один, осознав цену потери.\n");
                gameOver("КОНЦОВКА: ЖЕРТВА FEDIL'А. Мир выжил, но без радости.");
            }
        } else {
            slowprint("Ты пытаешься их объединить, но они слишком далеки друг от друга. Требуется больше доверия.\n");
            slowprint("Гермоненко отталкивает тебя: 'Ты не понимаешь!' Начинается битва.\n");
            slowprint("Федил активирует протокол Shift+Delete.\n");
            choice = 2; // fallback
        }
    }

    if (choice == 2) {
        slowprint("Ты нажимаешь Shift+Delete. Соси-строки лопаются, Гермоненко рассеивается в логах.\n");
        if (p.fedilRelation >= 50) {
            slowprint("Федил грустно улыбается: 'Он был частью меня. Теперь я одинок, но мир свободен.'\n");
            gameOver("КОНЦОВКА: ОСВОБОЖДЕНИЕ. Свобода достигнута, но какой ценой?");
        } else {
            slowprint("Федил смотрит на тебя: 'Ты сделал то, что должен был. Но запомни: насилие порождает пустоту.'\n");
            gameOver("КОНЦОВКА: ПУСТОТА. Мир без Гермоненко стал серым.");
        }
    } else if (choice == 3) {
        if (p.tankReady || p.glebMadness > 20) {
            slowprint("Глеб появляется с ноутбуком: 'В АНГАР! ЗАДУБЕНИЕ НАЧИНАЕТСЯ!'\n");
            drawArt(TANK, 3);
            slowprint("Мир превращается в бесконечный танковый бой. Все проблемы забыты.\n");
            gameOver("КОНЦОВКА: ВЕЛИКОЕ ЗАДУБЕНИЕ. Да будет рок!");
        } else {
            slowprint("Глеб не отвечает. Видимо, его новелла ещё не готова.\n");
            slowprint("Федил вздыхает: 'Ладно, тогда я сам.' Он запускает свой протокол.\n");
            slowprint("Shift+Delete активирован.\n");
            gameOver("КОНЦОВКА: ЗАДУБЕНИЕ ОТМЕНЕНО. FEDIL очищает мир.");
        }
    } else if (choice == 4) {
        if (p.germonenkoRelation >= 30 && p.hasItem(Item::CPU_AMULET)) {
            slowprint("Ты надеваешь Амулет Процессора и произносишь: 'Я продолжу твоё дело, Сергей.'\n");
            slowprint("Гермоненко усмехается: 'Наконец-то достойный преемник.' Он передаёт тебе исходный код.\n");
            slowprint("Федил в ужасе: 'Ты предал нас!' Но ты уже становишься новым диктатором.\n");
            gameOver("КОНЦОВКА: НОВЫЙ ГЕРМОНЕНКО. Соси-строки возвращаются.");
        } else {
            slowprint("Ты не готов. Гермоненко отвергает тебя. 'Ты слаб.'\n");
            slowprint("Федил пользуется моментом и стирает вас обоих.\n");
            gameOver("КОНЦОВКА: СТИРАНИЕ. Оба кандидата удалены.");
        }
    } else if (choice == 5) {
        slowprint("Ты достаёшь Гачи-Протеин и Кожаный Пояс, крича: '♂ ASS WE CAN ♂!'\n");
        slowprint("Пространство наполняется Dungeon Masters. Реальность становится вечным фистингом.\n");
        gameOver("КОНЦОВКА: ГАЧИ-РЕВОЛЮЦИЯ. Свобода через кожу!");
    } else if (choice == 6 && p.hasItem(Item::MELCOIN)) {
        slowprint("Ты подкидываешь Меллкойн. Система захлёбывается в транзакциях.\n");
        slowprint("Меллстрой орёт: 'ЭТО СОН! ЭТО ПРОСТО СОН!' Вселенная превращается в бесконечный стрим.\n");
        gameOver("КОНЦОВКА: МЕЛЛКОЙН-ХАОС. Никто не знает, что реально.");
    } else if (choice == 7 || (choice == 6 && !p.hasItem(Item::MELCOIN))) {
        slowprint("Ты разворачиваешься и уходишь. Гермоненко и Федил смотрят вслед.\n");
        slowprint("'Может, это и есть свобода', — говорит Федил. 'Возможно', — отвечает Гермоненко.\n");
        slowprint("Ты покидаешь их, оставляя вселенную в состоянии вечного противостояния.\n");
        gameOver("КОНЦОВКА: УХОД. Ты выбрал свободу от выбора.");
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    setlocale(LC_ALL, "");
    Player p;
    int currentChapter = 1;

    drawArt(LOGO, 3);
    if (saveExists()) {
        slowprint("Найдено сохранение.\n");
        std::cout << "[1] Продолжить с сохранённой главы\n[2] Новая игра\n>>> ";
        int menuChoice = 1;
        std::cin >> menuChoice;
        if (menuChoice == 1 && loadProgress(p, currentChapter)) {
            slowprint("[💾] Загружено сохранение: " + p.name + ", глава " +
                std::to_string(currentChapter) + ".\n");
            loading("Восстановление соси-строк", 1);
        } else {
            if (menuChoice == 1) slowprint("[!] Сохранение повреждено. Начинаем новую игру.\n");
            currentChapter = 1;
            slowprint("Твоё имя: ");
            std::getline(std::cin>>std::ws, p.name);
            loading("Сканирование коры", 2);
            saveProgress(p, currentChapter);
        }
    } else {
        slowprint("Твоё имя: ");
        std::getline(std::cin>>std::ws, p.name);
        loading("Сканирование коры", 2);
        saveProgress(p, currentChapter);
    }

    if (currentChapter <= 1) {
        chapter1(p);
        if (p.hp<=0) return 0;
        rewardChapterTransition(p, 2);
    }
    if (currentChapter <= 2) {
        chapter2(p);
        if (p.hp<=0) return 0;
        rewardChapterTransition(p, 3);
    }
    if (currentChapter <= 3) {
        chapter3(p);
        if (p.hp<=0) return 0;
        rewardChapterTransition(p, 4);
    }
    chapter4(p);
    slowprint("\nСпасибо за игру! print(\"The End.\")\n");
    return 0;
}

