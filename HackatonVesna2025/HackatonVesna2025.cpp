#include <iostream>
#include <conio.h>
#include <locale>
#include <cstdlib>
#include <limits>
#include <windows.h>
#include <Windows.h>
#include <iomanip>
#include <string>
#include <random>
#include <algorithm>

using namespace std;
short FinalChooseCharacter = 0; // Выбранный персонаж
int Money;
int Grade; // Успеваемость
int Satiety; // Сытость
short day = 1;

const int WIDTH = 80;
const int HEIGHT = 21;
int playerX = 46;
int playerY = 0;

bool VisitLecture = false;
bool VisitPractice = false;
bool ChooseGoHome0 = false;
bool ChooseGoHome1 = false;
bool StopGameLoop = false;
bool DontEat = true;
bool bus = true;
bool Lose = false;
bool Win = false;

enum ConsoleColor {
    Black = 0,
    Blue = 1,
    Green = 2,
    Cyan = 3,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    LightGray = 7,
    DarkGray = 8,
    LightBlue = 9,
    LightGreen = 10,
    LightCyan = 11,
    LightRed = 12,
    LightMagenta = 13,
    Yellow = 14,
    White = 15
};

// Установка цвета текста и фона
void SetColor(ConsoleColor text, ConsoleColor background = Black) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD)((background << 4) | text));
}

// Возврат к стандартному цвету (серый текст на черном фоне)
void ResetColor() {
    SetColor(LightGray, Black);
}


void ChooseCharacter();
void SetValues();
void ShowCharacterInfo(int institute);
void GoInUniver();
void DrawMap();
short Menu();
void ClearInputBuffer();
void GameLoop();
void HandleInput2();
bool CanMoveTo(int x, int y);
void EatInCanteen(); // Функция для столовой
bool CanGoHome();
void GoHome();
void LecturesIIMRT();
void LecturesIGSN();
void LecturesIPCH();
void LecturesIHZCHS();
void LecturesINEB();
void PracticeIIMRT();
void PracticeIGSN();
void PracticeIPCH();
void PracticeIHZCHS();
void PracticeINEB();

void HideConsoleCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

int main()
{
    SetColor(White);
    HideConsoleCursor();
    while (true) {
        Lose = false;
        day = 1;
        setlocale(LC_ALL, "Russian");
        if (!Menu()) {
            return 0;
        };
        system("cls");
        cout << "На дворе июль, а вы всё ещё не выбрали, на какой факультет поступить... "; _getch(); system("cls");
        cout << "Поторопитесь, времени очень мало!!!"; _getch(); system("cls");

        ChooseCharacter();
        SetValues();
        system("cls");
        cout << "Поздравляем! Вы нашли себя в списке зачисленных и с нетерпением ждёте начала учёбы..."; _getch(); system("cls");
        cout << "Ожидания так мучительны..."; _getch(); system("cls");
        cout << "Вот-вот, вот уже почти..."; _getch(); system("cls");
        cout << "Вам уже во снах снится этот фиолетовый остроугольный логотип..."; _getch(); system("cls");
        while (day <= 7 && !Lose) {
            GoInUniver();
            GameLoop();
        }
        if (Lose) {
            cout << "К сожалению вы проиграли. Начните с самого начала..."; _getch(); system("cls");
            continue;
        }
        else {
             cout << "Поздравляем! Вы прошли игру до конца и смогли успешно окончить первый курс!";
        }
    }
}

void GameLoop() {
    DrawMap();
    while (true) {
        HandleInput2();
        Sleep(10);
        if (ChooseGoHome0 && CanGoHome()) {
            GoHome();
            return;
        }

        if (playerX == 7 && playerY == 12 && DontEat) {
            EatInCanteen();
            DrawMap();
        }

        if (playerX == 78 && playerY == 20 && !VisitLecture) {
            switch (FinalChooseCharacter) {
            case 1: LecturesIIMRT(); break;
            case 2: LecturesIGSN(); break;
            case 3: LecturesIPCH(); break;
            case 4: LecturesIHZCHS(); break;
            case 5: LecturesINEB(); break;
            }
            switch (bus) {
            case 1: Satiety = Satiety - 15; break;
            case 0: Satiety = Satiety - 5; break;
            }
            VisitLecture = true;
            DrawMap();

        }

        if (playerX == 1 && playerY == 0 && !VisitPractice) {
            switch (FinalChooseCharacter) {
            case 1: PracticeIIMRT(); break;
            case 2: PracticeIGSN(); break;
            case 3: PracticeIPCH(); break;
            case 4: PracticeIHZCHS(); break;
            case 5: PracticeINEB(); break;
            }
            VisitPractice = true;  
            switch (bus) {
            case 1: Satiety = Satiety - 15; break;
            case 0: Satiety = Satiety - 5; break;
            }
            DrawMap();

        }
    }
}

void HandleInput2() {
    bool moved = false;
    int newX = playerX;
    int newY = playerY;

    // Английская раскладка + стрелки
    if (GetAsyncKeyState('W') & 0x8000 || GetAsyncKeyState(VK_UP) & 0x8000) { newY--; moved = true; }
    if (GetAsyncKeyState('A') & 0x8000 || GetAsyncKeyState(VK_LEFT) & 0x8000) { newX--; moved = true; }
    if (GetAsyncKeyState('S') & 0x8000 || GetAsyncKeyState(VK_DOWN) & 0x8000) { newY++; moved = true; }
    if (GetAsyncKeyState('D') & 0x8000 || GetAsyncKeyState(VK_RIGHT) & 0x8000) { newX++; moved = true; }
    if (GetAsyncKeyState('O') & 0x8000) {ChooseGoHome0 = true; }
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) { StopGameLoop = true; }

    // Русская раскладка (скан-коды)
    if (GetAsyncKeyState(0x43) & 0x8000) { newY--; moved = true; } // Ц
    if (GetAsyncKeyState(0x46) & 0x8000) { newX--; moved = true; } // Ф
    if (GetAsyncKeyState(0x59) & 0x8000) { newY++; moved = true; } // Ы
    if (GetAsyncKeyState(0x42) & 0x8000) { newX++; moved = true; } // В
    if (GetAsyncKeyState(0xC0) & 0x8000) { ChooseGoHome0 = true; } // Щ

    if (moved && CanMoveTo(newX, newY)) {
        playerX = newX;
        playerY = newY;
        DrawMap();
    }
}

void FlushInputBuffer() {
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hInput);
}

void EatInCanteen() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int choice = 0;
    while (choice < 1 || choice > 4) {
        cout << "Вы сели в столовой и перед вами встал тяжёлый выбор\n";
        cout << "1. Поесть в столовой (100р, +10 сытости)\n";
        cout << "2. Сходить в бургер кинг через дорогу (250р, +25 сытости)\n";
        cout << "3. Ресторан Перчини (1000р, +70 сытости\n";
        cout << "4. Уйти из столовой\n";
        cout << "Ваш выбор: ";
        cin >> choice;
        ClearInputBuffer();

        switch (choice) {
        case 1:
            if (Money >= 100) {
                Money -= 100;
                Satiety = min(100, Satiety + 30);
                cout << "В целом съедобно. Приятного аппетита!\n";
            }
            else {
                cout << "У вас недостаточно денег!\n";
            }
            _getch();
            break;
        case 2:
            if (Money >= 250) {
                Money -= 250;
                Satiety = min(100, Satiety + 25);
                cout << "Вредно, но вкусно. Приятного аппетита!\n";
            }
            else {
                cout << "У вас недостаточно денег!\n";
            }
            _getch();
            break;
        case 3:
            if (Money >= 1000) {
                Money -= 1000;
                Satiety = min(100, Satiety + 70);
                cout << "ММмм, Цезарь и карбонара, что может быть лучше. Приятного аппетита!\n";
            }
            else {
                cout << "У вас недостаточно денег!\n";
            }
            _getch();
            break;
        case 4:
            cout << "Питаться святым духом в принципе никто не запрещал.\n";
            _getch();
            break;
        default:
            cout << "Неверный выбор!\n";
            _getch();
            break;
        }
    }
    DontEat = false;
}

bool CanGoHome() {
    if ((VisitLecture && VisitPractice)) {
        return true;
    }
    else {
        system("cls");
        Sleep(50);
        FlushInputBuffer();
        ClearInputBuffer();
        int chooseCanGoHome = 0;
        while (chooseCanGoHome != 1 && chooseCanGoHome != 2) {
            cout << "Вы сходили не на все занятия сегодня!\n";
            cout << "Вы точно хотите уехать домой?\n";
            cout << "1. Да (-30 успеваемость)\n";
            cout << "2. Нет\n";
            cin >> chooseCanGoHome;
            ClearInputBuffer();
            switch (chooseCanGoHome) {
            case 1: Grade -= 30; return true; break;
            case 2: ChooseGoHome0 = false; return false; break;
            }
        }
    }

}

void SetValues(){
    switch (FinalChooseCharacter) {
    case 1:
        Grade = 40;
        Satiety = 70;
        Money = 1000;
        break;
    case 2:
        Grade = 70;
        Satiety = 30;
        Money = 500;
        break;
    case 3:
        Grade = 25;
        Satiety = 20;
        Money = 2000;
        break;
    case 4:
        Grade = 20;
        Satiety = 100;
        Money = 1500;
        break;
    case 5:
        Grade = 40;
        Satiety = 70;
        Money = 5000;
        break;
    }
}

void ShowCharacterInfo(int institute) {
    system("cls");
    switch (institute) {
    case 1:
        SetColor(Yellow);
        cout << "Ильдар Михайлович Роботов - ИИМРТ\n";
        cout << "Типичный интроверт\n";
        cout << "Неловкий\n";
        cout << "Увлекается киберспортом\n";
        cout << "Мечтает получить грант на создание собственной игры\n\n\n";
        cout << "Начальные показатели\n";
        cout << "Деньги: 1000\n";
        cout << "Сытость: 70 \n";
        cout << "Успеваемость: 40 \n\n\n";
        break;
    case 2:
        SetColor(LightMagenta);
        cout << "Галина Сергеевна Николаева  - ИГСН  \n";
        cout << "Упорная студентка\n";
        cout << "Любит театральные постановки\n";
        cout << "Читает романы\n";
        cout << "Сладкоежка\n";
        cout << "Плохо сдала ЕГЭ, из-за чего поступает на коммерцию.\n";
        cout << "Мечтает перевестись на бюджет\n\n\n";
        cout << "Начальные показатели\n";
        cout << "Деньги: 500\n";
        cout << "Сытость: 30 \n";
        cout << "Успеваемость: 70\n\n\n";
        break;
    case 3:
        SetColor(LightGreen);
        cout << "Игорь Павлович Червяков - ИПЧ \n";
        cout << "Добрый и заботливый\n";
        cout << "Ухаживает за животными\n";
        cout << "Выращивает растения\n";
        cout << "Сирота, получает социальную стипендию\n";
        cout << "Худощавый, имеет меньшую потребность в еде\n";
        cout << "Мечтает написать собственную статью\n\n\n";
        cout << "Начальные показатели\n";
        cout << "Деньги: 2000\n";
        cout << "Сытость: 20 \n";
        cout << "Успеваемость: 25 \n\n\n";
        break;
    case 4:
        SetColor(Brown);
        cout << "Изабелла Хлоя-Зенна - ИХЗЧС \n";
        cout << "Иностранка\n";
        cout << "Любит экспериментировать с реакциями веществ\n";
        cout << "Хочет стать уверенной в себе\n";
        cout << "Мечтает вступить в профбюро\n\n\n";
        cout << "Начальные показатели\n";
        cout << "Деньги: 1500\n";
        cout << "Сытость: 100 \n";
        cout << "Успеваемость: 20 \n\n\n";
        break;
    case 5:
        SetColor(LightRed);
        cout << "Илья Николаевич Борисов - ИНЭБ \n";
        cout << "Практичный карьерист\n";
        cout << "Харизматичный\n";
        cout << "Ведёт блог о финансах и инвестициях\n";
        cout << "Обеспеченные родители\n";
        cout << "Мечтает получить самую повышенную стипендию\n\n\n";
        cout << "Начальные показатели\n";
        cout << "Деньги: 5000\n";
        cout << "Сытость: 70 \n";
        cout << "Успеваемость: 40 \n\n\n";
        break;
    }
    cout << "Вам нравится ваш выбор?\n";
    cout << "1. Да\n";
    cout << "2. Нет\n";
    cout << "Ваш выбор: ";
}


short Menu() {
    while (true)
    {
        cout << "Добро пожаловать в игру 'Я учусь в УУНиТ'\n";
        cout << "1. Начать игру\n";
        cout << "2. Загрузить игру\n";
        cout << "3. Выйти из игры\n";
        cout << "Ваш выбор: ";
        short ChooseMainMenu;
        cin >> ChooseMainMenu;
        ClearInputBuffer();

        switch (ChooseMainMenu) {
        case 1: return 1; break;
        case 2: return 0; break;
        case 3: return 0; break;
        default: cout << "Данный пункт меню отсутствует"; _getch();
        };
    }
}

void ChooseCharacter() {
    while (FinalChooseCharacter == 0) {
        ResetColor();
        SetColor(White);
        cout << "Вы смогли сократить выбор до 5 факультетов. Осталось принять непростое решение.\n";
        cout << "1. Институт информатики, математики и робототехники\n";
        cout << "2. Институт гуманитарных и социальных наук\n";
        cout << "3. Институт природы и человека\n";
        cout << "4. Институт химии и защиты в чрезвычайных ситуациях\n";
        cout << "5. Институт экономики, управления и бизнеса\n";
        cout << "Ваш выбор: ";
        short ChooseCharacter;
        cin >> ChooseCharacter;

        ClearInputBuffer();
        if (ChooseCharacter < 1 || ChooseCharacter > 5) {
            cout << "Неверный выбор! Нажмите любую клавишу...";
            _getch();
            continue;
        }

        bool confirmed = false;
        while (!confirmed) {
            ShowCharacterInfo(ChooseCharacter);

            short choice;
            cin >> choice;
            ClearInputBuffer();
            ResetColor();
            SetColor(White);
            if (choice == 1) {
                FinalChooseCharacter = ChooseCharacter;
                confirmed = true;
            }
            else if (choice == 2) {
                system("cls");
                break; // Вернуться к выбору института
            }
            else {
                cout << "Неверный выбор! Нажмите любую клавишу...";
                _getch();
            }
        }
    }
}

void GoInUniver() {
    int ChooseTransport = 0;
    while (ChooseTransport != 1 && ChooseTransport != 2) {
        cout << "Доброе утро! Наступил " << day << "-ый учебный день!\n";
        cout << "На чём поедем?\n";
        cout << "1. Автобус (50 р, занятие -15 сытости)\n";
        cout << "2. Такси (250 р, занятие -5 сытости)\n";
        cin >> ChooseTransport;
        ClearInputBuffer();
        system("cls");
        switch (ChooseTransport) {
        case 1: cout << "Утомительно, но и кошелёк не бездонный. Поехали!";
            _getch(); system("cls"); cout << "Едем...";  _getch(); system("cls"); cout << "Автобус транспорт не быстрый";  _getch(); system("cls");
            cout << "О, приехали."; _getch(); system("cls");
            Money = Money - 50;
            system("cls"); break;
        case 2: cout << "Настолько хочется быстрее начать учиться, что никаких денег не жалко!"; 
            _getch(); 
            Money = Money - 250; 
            bus = false;
            system("cls"); 
            break;
        default: cout << "Нас так отчислят быстрее, чем мы дойдём до университета.\nДавай выберем что-нибудь из списка";
            _getch();
            system("cls");
            break;
        }
    }
}

void DrawMap() {
    system("cls");
    cout << "---------------------------------------------   --------------------------------\n";
    // Игровое поле
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) {
                cout << "@";
            }
            else if ((x == 0 || x == WIDTH - 1) || (x == 31 && ((y >= 0 && y <= 6) || (y >= 15 && y <= 18)))) {
                cout << "|";
            }
            else if ((x == 48 && ((y >= 0 && y <= 6) || (y >= 15 && y <= 18))) || (x == 71 && (y >= 6 && y <= 18)) ){
                cout << "|";
            }
            else if ((x == 8 && ((y >= 4 && y <= 9) || (y >= 12 && y <= 18)))) {
                cout << "|";
            }
            else if (x == 22 && y >= 4 && y <= 6) {
                cout << "|";
            }
            else if ((x >= 8 && x <= 21) && (y == 3))  {
                cout << "_"; 
            }
            else if (((x >= 23 && x <= 25) || (x >= 28 && x <= 30) || (x >= 49 && x <= 55) || (x >= 59 && x <= 70)) && y == 6) {
                cout << "_";
            }
            else if (y == 14 && ((x >= 32 && x <= 37) || (x >= 42 && x <= 47))) {
                cout << "_";
            }
            else if (y == 18 && ((x >= 9 && x <= 30) || (x >= 49 && x <= 70))) {
                cout << "_";
            }
            else if (x == 1 && y == 0 && !VisitPractice) {
                cout << "П";
            }
            else if (x == 78 && y == 20 && !VisitLecture) {
                cout << "Л";
            }
            else if (x == 7 && y == 12 && DontEat) {
                cout << "С";
            }
            else {
                cout << " ";
            }
        }
        cout << "\n";
    }

    // Нижняя панель статуса
    cout << "--------------------------------------------------------------------------------\n";
    cout << "| " << setw(7) << "День:" << setw(2) << day << setw(3) << "|";
    switch (FinalChooseCharacter) {
    case 1: cout << "  ИИМРТ |"; break;
    case 2: cout << "  ИГСН  |"; break;
    case 3: cout << "  ИПЧ   |"; break;
    case 4: cout << "  ИХЗЧС |"; break;
    case 5: cout << "  ИНЭБ  |"; break;
    };
    cout << " Деньги: " << setw(5) << Money << setw(13) << "| Сытость: " << setw(3) << Satiety << setw(20) << "| Успеваемость: " << setw(2) << Grade << setw(6) << "|\n";
    cout << "| Л - лекция | П - практика | С - столовая | O - домой | M - мануал| ESC - меню|\n";
}

bool IsKeyPressed(int vKey) {
    return GetAsyncKeyState(vKey) & 0x8000;
}

void ClearInputBuffer() {
    cin.clear();
    while (cin.get() != '\n') {}
}

bool CanMoveTo(int x, int y) {
    // Проверка границ карты
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        return false;
    }

    // Проверка стен и препятствий
    if ((x == 0 || x == WIDTH - 1) || (x == 31 && ((y >= 0 && y <= 6) || (y >= 15 && y <= 18)))) {
        return false;
    }
    if ((x == 48 && ((y >= 0 && y <= 6) || (y >= 15 && y <= 18))) || (x == 71 && (y >= 6 && y <= 18))) {
        return false;
    }
    if ((x == 8 && ((y >= 4 && y <= 9) || (y >= 12 && y <= 18)))) {
        return false;
    }
    if (x == 22 && y >= 4 && y <= 6) {
        return false;
    }
    if ((x >= 8 && x <= 21) && (y == 3)) {
        return false;
    }
    if (((x >= 23 && x <= 25) || (x >= 28 && x <= 30) || (x >= 49 && x <= 55) || (x >= 59 && x <= 70)) && y == 6) {
        return false;
    }
    if (y == 14 && ((x >= 32 && x <= 37) || (x >= 42 && x <= 47))) {
        return false;
    }
    if (y == 18 && ((x >= 9 && x <= 30) || (x >= 49 && x <= 70))) {
        return false;
    }

    return true;
}

void GoHome() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int ChooseTransport = 0;
    while (ChooseTransport != 1 && ChooseTransport != 2) {
        ;
        cout << "На чём поедем до дома?\n";
        cout << "1. Автобус (50 р)\n";
        cout << "2. Такси (250 р)\n";
        cin >> ChooseTransport;
        ClearInputBuffer();
        system("cls");
        switch (ChooseTransport) {
        case 1: cout << "Утомительно, но и кошелёк не бездонный. Поехали!";
            _getch(); system("cls"); cout << "Едем...";  _getch(); system("cls"); cout << "Автобус транспорт не быстрый";  _getch(); system("cls");
            cout << "О, приехали."; _getch(); system("cls");
            Money = Money - 50;
            system("cls"); break;
        case 2: cout << "Настолько устал, что никаких денег не жалко!";
            _getch();
            Money = Money - 250;
            system("cls");
            break;
        default: cout << "Неправильный выбор!!";
            _getch();
            system("cls");
            break;
        }
    }
    system("cls");
    cout << "Вы завершили день со следующими показателями:\n";
    cout << "Деньги:" << Money;
    cout << "\nУспеваемость:" << Grade;
    cout << "\nСытость:" << Satiety; _getch(); system("cls");

    if (Money < 50) {
        cout << "Ваших денег не хватает даже не проезд..."; _getch(); system("cls");
        cout << "К сожалению, ваша студенческая жизнь на этом закончена..."; _getch(); system("cls");
        Lose = true;
    }
    else if (Grade <= 0) {
        cout << "Ваших знаний не хватило на окончание первого курса..."; _getch(); system("cls");
        cout << "К сожалению, ваша студенческая жизнь на этом закончена..."; _getch(); system("cls");
        Lose = true;
    }
    else if (Satiety <= 0) {
        cout << "Вы настолько увлеклись учёбой, что забыли о питании..."; _getch(); system("cls");
        cout << "К сожалению, ваша студенческая жизнь на этом закончена..."; _getch(); system("cls");
        Lose = true;
    }
    else {
        day++;
        cout << "\nНаступает день " << day; _getch(); system("cls");
        playerX = 46;
        playerY = 0;
        ChooseGoHome0 = false;
        VisitLecture = false;
        VisitPractice = false;
        StopGameLoop = false;
        DontEat = true;
        bus = true;
    }
}

void LecturesIIMRT() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Какая из этих теорем относится к геометрии?\n";
        cout << "1) Теорема Ферма\n";
        cout << "2) Теорема Пифагора\n";
        cout << "3) Теорема Коши\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Что такое рекурсия в программировании?\n";
        cout << "1) Метод сортировки данных\n";
        cout << "2) Способ оптимизации кода\n";
        cout << "3) Функция, вызывающая саму себя\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Что такое игровой движок?\n";
        cout << "1) Набор инструментов и библиотек для разработки игр\n";
        cout << "2) Устройство для повышения FPS в играх\n";
        cout << "3) Программа для создания 3D-моделей\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Какой тип данных в Python изменяемый (mutable)?\n";
        cout << "1) Кортеж (tuple)\n";
        cout << "2) Список (list)\n";
        cout << "3) Строка (str)\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Чему равно значение i², где i - мнимая единица?\n";
        cout << "1) -1\n";
        cout << "2) 0\n";
        cout << "3) 1\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void PracticeIIMRT() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Что выведет этот код?\nx = [1, 2, 3]\ny = x\ny.append(4)\nprint(x)\n";
        cout << "1) [1, 2, 3]\n";
        cout << "2) [1, 2, 3, 4]\n";
        cout << "3) Ошибка\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Чему равен факториал 6?\n";
        cout << "1) 720\n";
        cout << "2) 24\n";
        cout << "3) 126\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Чему равно число 10 в двоичной системе?\n";
        cout << "1) 1100\n";
        cout << "2) 1001\n";
        cout << "3) 1010\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Какой цикл выполнится ровно 10 раз?\n";
        cout << "1) for (int i = 0; i <= 10; i++)\n";
        cout << "2) for (int i = 0; i < 10; i++)\n";
        cout << "3) for (int i = 1; i <= 10; i++)\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Как создать динамический массив на языке С++?\n";
        cout << "1) int* arr = new int[10];\n";
        cout << "2) array<int> arr(10);\n";
        cout << "3) int arr[] = new int[10];\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void LecturesIGSN() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Главное произведение Гомера, повествующее о возвращении Одиссея домой?\n";
        cout << "1) Илиада\n";
        cout << "2) Одиссея\n";
        cout << "3) Энеида\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Кто является родоначальником философского направления экзистенциализма?\n";
        cout << "1) Фридрих Ницше\n";
        cout << "2) Мартин Хайдеггер\n";
        cout << "3) Жан-Поль Сартр\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Кто автор скульптуры «Давид»?\n";
        cout << "1) Микеланджело\n";
        cout << "2) Донателло\n";
        cout << "3) Бернини\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Какой океан самый большой по площади?\n";
        cout << "1) Индийский\n";
        cout << "2) Тихий\n";
        cout << "3) Атлантический\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Название произведения Льва Толстого, рассказывающего историю семьи Ростовых и Болконских?\n";
        cout << "1) «Герой нашего времени»\n";
        cout << "2) «Отцы и дети»\n";
        cout << "3) «Война и мир»\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void PracticeIGSN() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Отрывок из «Декларации прав человека и гражданина» (1789):\n«Свобода состоит в возможности делать всё, что не вредит другому».\nКакой принцип эпохи Просвещения здесь отражён? \n";
        cout << "1) Всеобщее равенство перед законом\n";
        cout << "2) Абсолютная власть монарха\n";
        cout << "3) Ограничение свободы правами других\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Цитата из «Героя нашего времени» Лермонтова:\n«Я говорил правду — мне не верили : я начал обманывать».\nКакая черта характера Печорина здесь проявляется? \n";
        cout << "1) Идеализм\n";
        cout << "2) Альтруизм\n";
        cout << "3) Цинизм\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. «Ветер пел в проводах, будто жалуясь на свою одинокую судьбу».\nКакое средство выразительности здесь используется? \n";
        cout << "1) Олицетворение\n";
        cout << "2) Метафора\n";
        cout << "3) Гипербола\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. В каком предложении есть ошибка?\n";
        cout << "1) «Мы ели мороженое в парке»\n";
        cout << "2) «Они ложили книгу на полку»\n";
        cout << "3) «Дети играли во дворе»\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Уровень образования в странах X и Y (X – 95% грамотных, Y – 60% неграмотных).\nКакой фактор наиболее вероятно объясняет разницу?\n";
        cout << "1) Экономическое развитие\n";
        cout << "2) Климатические условия\n";
        cout << "3) Географические условия\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void LecturesIPCH() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Кто открыл существование микроорганизмов?\n";
        cout << "1) Чарльз Дарвин\n";
        cout << "2) Антони ван Левенгук\n";
        cout << "3) Луи Пастер\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Основным источником белка являются продукты какой группы?\n";
        cout << "1) Мясо и рыба\n";
        cout << "2) Крупы и макаронные изделия\n";
        cout << "3) Овощи и фрукты\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Как называется метод диагностики заболеваний сердца путем регистрации электрических импульсов?\n";
        cout << "1) МРТ\n";
        cout << "2) УЗИ\n";
        cout << "3) ЭКГ\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Какая болезнь развивается при недостаточном потреблении витамина D?\n";
        cout << "1) Куриная слепота\n";
        cout << "2) Рахит\n";
        cout << "3) Цинга\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. В переносе какого вещества заключается главная функция гемоглобина?\n";
        cout << "1) Кислорода\n";
        cout << "2) Углеводов\n";
        cout << "3) Витаминов\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void PracticeIPCH() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Почему пчёлы жужжат?\n";
        cout << "1) Так они разговаривают\n";
        cout << "2) Жужжание отпугивает врагов\n";
        cout << "3) Это звук от быстрых взмахов крыльев\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Почему вирусы не считаются живыми организмами?\n";
        cout << "1) Они не могут размножаться вне клетки-хозяина\n";
        cout << "2) У них нет ДНК\n";
        cout << "3) Они состоят только из белка\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Какой гормон выделяется при стрессе?\n";
        cout << "1) Инсулин\n";
        cout << "2) Кортизол\n";
        cout << "3) Тестостерон\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Что из перечисленного — пример симбиоза?\n";
        cout << "1) Гриб и дерево в микоризе\n";
        cout << "2) Лиса, охотящаяся на зайца\n";
        cout << "3) Берёза, затеняющая траву под собой\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Почему после бега учащается пульс?\n";
        cout << "1) Сердце устаёт и бьётся хаотично\n";
        cout << "2) Так тело охлаждается\n";
        cout << "3) Организму нужно быстрее доставлять кислород к мышцам\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void LecturesIHZCHS() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Первый химический элемент в периодической таблице — это..\n";
        cout << "1) Гелий\n";
        cout << "2) Водород\n";
        cout << "3) Кислород\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Что обозначается символом Na?\n";
        cout << "1) Натрий\n";
        cout << "2) Золото\n";
        cout << "3) Никель\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Какие два элемента образуют воду?\n";
        cout << "1) Железо и кислород\n";
        cout << "2) Углерод и кислород\n";
        cout << "3) Водород и кислород\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Какое агрегатное состояние имеет кислород при обычных условиях?\n";
        cout << "1) Газ\n";
        cout << "2) Жидкость\n";
        cout << "3) Твердое тело\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Какой символ соответствует железу?\n";
        cout << "1) Cu\n";
        cout << "2) Fe\n";
        cout << "3) Al\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void PracticeIHZCHS() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Если реакция экзотермическая, это значит, что:\n";
        cout << "1) Протекает только при нагревании\n";
        cout << "2) Она поглощает тепло\n";
        cout << "3) Выделяет тепло\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Как катализатор влияет на химическую реакцию?\n";
        cout << "1) Снижает энергию активации\n";
        cout << "2) Уменьшает скорость реакции\n";
        cout << "3) Увеличивает выход продукта\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Что происходит с веществом, которое отдаёт электроны?\n";
        cout << "1) Восстанавливается\n";
        cout << "2) Не изменяется\n";
        cout << "3) Окисляется\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Почему тесто поднимается при добавлении соды?\n";
        cout << "1) Выделяется кислород\n";
        cout << "2) Выделяется углекислый газ\n";
        cout << "3) Это магия\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Какой газ поддерживает горение?\n";
        cout << "1) Углекислый газ\n";
        cout << "2) Кислород\n";
        cout << "3) Азот\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void LecturesINEB() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Что такое инфляция?\n";
        cout << "1) Повышение уровня цен\n";
        cout << "2) Увеличение заработной платы\n";
        cout << "3) Снижение налогов\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. Что представляет собой ВВП страны?\n";
        cout << "1) Общее количество рабочих мест\n";
        cout << "2) Совокупная рыночная стоимость всех конечных товаров и услуг, произведённых страной за определённый период \n";
        cout << "3) Сумма доходов частных компаний\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Что показывает индекс потребительских цен (ИПЦ)?\n";
        cout << "1) Изменение стоимости акций\n";
        cout << "2) Уровень безработицы\n";
        cout << "3) Изменение цен на основные товары и услуги\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 3) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Что такое «естественная монополия»?\n";
        cout << "1) Отрасль, где конкуренция невозможна из-за технологий\n";
        cout << "2) Компания, которая захватила весь рынок силой\n";
        cout << "3) Государственное предприятие\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Что означает «диверсификация производства»?\n";
        cout << "1) Снижение издержек производства\n";
        cout << "2) Расширение ассортимента выпускаемых товаров\n";
        cout << "3) Увеличение объема выпускаемой продукции\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}

void PracticeINEB() {
    system("cls");
    Sleep(50);
    FlushInputBuffer();
    ClearInputBuffer();
    int a = 1, b = 5;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(a, b); //для лучшей генерации рандомных чисел
    int rand_num = dist(gen);
    switch (rand_num) {
    case 1:
        cout << "№1. Доходы бюджета — 50 трлн руб., расходы — 55 трлн руб. Какой дефицит?\n";
        cout << "1) 25 трлн рублей\n";
        cout << "2) 5 трлн рублей\n";
        cout << "3) 10 трлн рублей\n";
        int ans1;
        cin >> ans1;
        ClearInputBuffer();
        if (ans1 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 2:
        cout << "№2. В 2023 г. корзина стоила 10 000 руб., в 2024 г. — 11 500 руб. Какова инфляция?\n";
        cout << "1) 15%\n";
        cout << "2) 30%\n";
        cout << "3) 10%\n";
        int ans2;
        cin >> ans2;
        ClearInputBuffer();
        if (ans2 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 3:
        cout << "№3. Выручка — 50 000 руб., явные издержки — 30 000 руб., неявные (упущенная выгода) — 5 000 руб. Чему равна экономическая прибыль?\n";
        cout << "1) 15 000 рублей\n";
        cout << "2) 20 000 рублей\n";
        cout << "3) 10 000 рублей\n";
        int ans3;
        cin >> ans3;
        ClearInputBuffer();
        if (ans3 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 4:
        cout << "№4. Почему спрос на сезонные товары резко возрастает?\n";
        cout << "1) Производители снижают цену на товар\n";
        cout << "2) Потребность в товаре повышается\n";
        cout << "3) Появляются новые конкуренты на рынке\n";
        int ans4;
        cin >> ans4;
        ClearInputBuffer();
        if (ans4 == 2) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    case 5:
        cout << "№5. Фирма продала 200 единиц товара по цене 400 руб. за штуку. Себестоимость одной единицы — 250 руб. Какова прибыль?\n";
        cout << "1) 30 000 рублей\n";
        cout << "2) 100 000 рублей\n";
        cout << "3) 50 000 рублей\n";
        int ans5;
        cin >> ans5;
        ClearInputBuffer();
        if (ans5 == 1) {
            system("cls");
            cout << "Верный ответ! Успеваемость +10"; _getch(); Grade += 10; system("cls");
        }
        else {
            system("cls");
            cout << "Упс..неверно. Успеваемость -20"; _getch(); Grade -= 20; system("cls");
        }
        break;
    };
}