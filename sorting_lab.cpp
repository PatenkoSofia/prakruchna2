#include <iostream>
#include <iomanip>
#include <random>
#include <limits>

const int MAX_N = 100;

struct Stats {
    long long comparisons = 0;
    long long moves = 0;
    int passes = 0;
};

void insertionSort(int a[], int n, Stats& s) {
    for (int i = 1; i < n; ++i) {
        int key = a[i];
        ++s.moves;
        int j = i - 1;
        while (j >= 0) {
            ++s.comparisons;
            if (a[j] <= key) break;
            a[j + 1] = a[j];
            ++s.moves;
            --j;
        }
        a[j + 1] = key;
        ++s.moves;
    }
}

void selectionSort(int a[], int n, Stats& s) {
    for (int i = 0; i < n - 1; ++i) {
        int minIdx = i;
        for (int j = i + 1; j < n; ++j) {
            ++s.comparisons;
            if (a[j] < a[minIdx]) minIdx = j;
        }
        if (minIdx != i) {
            int tmp = a[i];             ++s.moves;
            a[i] = a[minIdx];           ++s.moves;
            a[minIdx] = tmp;            ++s.moves;
        }
    }
}

void bubbleSort(int a[], int n, Stats& s) {
    for (int pass = 0; pass < n - 1; ++pass) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - pass; ++j) {
            ++s.comparisons;
            if (a[j] > a[j + 1]) {
                int tmp = a[j];         ++s.moves;
                a[j] = a[j + 1];        ++s.moves;
                a[j + 1] = tmp;         ++s.moves;
                swapped = true;
            }
        }
        ++s.passes;
        if (!swapped) break;
    }
}

bool isSorted(const int a[], int n) {
    for (int i = 1; i < n; ++i) {
        if (a[i] < a[i - 1]) return false;
    }
    return true;
}

struct Checksum {
    long long count = 0;
    long long sum = 0;
    long long sumSq = 0;
};

Checksum makeChecksum(const int a[], int n) {
    Checksum c;
    for (int i = 0; i < n; ++i) {
        c.count += 1;
        c.sum += a[i];
        c.sumSq += static_cast<long long>(a[i]) * a[i];
    }
    return c;
}

bool sameChecksum(const Checksum& x, const Checksum& y) {
    return x.count == y.count && x.sum == y.sum && x.sumSq == y.sumSq;
}

bool sameArrays(const int a[], const int b[], int n) {
    for (int i = 0; i < n; ++i) {
        if (a[i] != b[i]) return false;
    }
    return true;
}

bool verify(const int original[], const int sorted[], int n) {
    return isSorted(sorted, n) &&
           sameChecksum(makeChecksum(original, n), makeChecksum(sorted, n));
}

void printArray(const char* title, const int a[], int n) {
    std::cout << title << ": ";
    for (int i = 0; i < n; ++i) {
        std::cout << a[i] << (i + 1 < n ? ", " : "");
    }
    std::cout << "\n";
}

void padTo(const char* s, int width) {
    int chars = 0;
    for (const char* p = s; *p; ++p) {
        if ((static_cast<unsigned char>(*p) & 0xC0) != 0x80) ++chars;
    }
    std::cout << s;
    for (int i = chars; i < width; ++i) std::cout << ' ';
}

int readInt(const char* prompt, int minV, int maxV) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= minV && value <= maxV) {
            return value;
        }
        std::cout << "Помилка: введіть ціле число від " << minV
                  << " до " << maxV << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

const int VARIANT22_N = 10;
const int VARIANT22[VARIANT22_N] = {39, 21, 28, 17, 43, 24, 15, 36, 30, 19};

int readInitialArray(int a[]) {
    std::cout << "1 - варіант 22 (за замовчуванням)\n"
              << "2 - ввести власний масив\n";
    int choice = readInt("Ваш вибір: ", 1, 2);
    if (choice == 1) {
        for (int i = 0; i < VARIANT22_N; ++i) a[i] = VARIANT22[i];
        return VARIANT22_N;
    }
    int n = readInt("Кількість елементів (1-100): ", 1, MAX_N);
    for (int i = 0; i < n; ++i) {
        std::cout << "a[" << i << "] = ";
        while (!(std::cin >> a[i])) {
            std::cout << "Помилка: введіть ціле число: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return n;
}

using SortFn = void (*)(int[], int, Stats&);

struct Algorithm {
    const char* name;
    SortFn sort;
};

const Algorithm ALGORITHMS[3] = {
    {"Вставки",     insertionSort},
    {"Вибір",       selectionSort},
    {"Бульбашкове", bubbleSort}
};

struct TestCase {
    int n;
    int input[10];
    int expected[10];
    const char* purpose;
};

const TestCase TESTS[12] = {
    {0,  {0},                       {0},                          "порожній масив"},
    {1,  {7},                       {7},                          "один елемент"},
    {5,  {1, 2, 3, 4, 5},           {1, 2, 3, 4, 5},              "уже впорядкований"},
    {5,  {5, 4, 3, 2, 1},           {1, 2, 3, 4, 5},              "зворотний порядок"},
    {5,  {4, 1, 5, 2, 3},           {1, 2, 3, 4, 5},              "змішаний порядок"},
    {5,  {3, 1, 3, 2, 1},           {1, 1, 2, 3, 3},              "повторювані значення"},
    {5,  {0, -5, 8, -2, 4},         {-5, -2, 0, 4, 8},            "від'ємні числа і нуль"},
    {4,  {6, 6, 6, 6},              {6, 6, 6, 6},                 "усі значення однакові"},
    {2,  {2, 1},                    {1, 2},                       "мінімальна перестановка"},
    {6,  {9, -1, 9, 0, -1, 5},      {-1, -1, 0, 5, 9, 9},         "комбінований випадок"},
    {4,  {-7, 12, -7, 0},           {-7, -7, 0, 12},              "власний тест: від'ємні дублікати"},
    {5,  {100, 1000, -1000, 50, 50}, {-1000, 50, 50, 100, 1000},  "власний тест: великі значення"}
};

int runAutomatedTests() {
    int passed = 0;
    int total = 0;
    std::cout << "\n=== Автоматизовані тести ===\n";
    for (const Algorithm& alg : ALGORITHMS) {
        for (int t = 0; t < 12; ++t) {
            const TestCase& tc = TESTS[t];
            int work[10];
            for (int i = 0; i < tc.n; ++i) work[i] = tc.input[i];
            Stats s;
            alg.sort(work, tc.n, s);

            bool ok = isSorted(work, tc.n) && sameArrays(work, tc.expected, tc.n);
            ++total;
            if (ok) ++passed;

            padTo(alg.name, 14);
            std::cout << "тест " << std::left << std::setw(4) << (t + 1);
            padTo(tc.purpose, 40);
            std::cout << (ok ? "успішно" : "ПОМИЛКА") << "\n";
            if (!ok) printArray("   фактичний масив", work, tc.n);
        }
    }
    std::cout << "Успішних перевірок: " << passed << " з " << total << "\n";
    return passed;
}

void buildExperimentSets(int sets[3][MAX_N]) {
    for (int i = 0; i < MAX_N; ++i) sets[0][i] = 1 + 3 * i;
    for (int i = 0; i < MAX_N; ++i) sets[1][i] = 1 + 3 * (MAX_N - 1 - i);
    std::mt19937 gen(22);
    std::uniform_int_distribution<int> dist(1, 999);
    for (int i = 0; i < MAX_N; ++i) sets[2][i] = dist(gen);
}

void runExperiment() {
    int sets[3][MAX_N];
    buildExperimentSets(sets);
    const char* setNames[3] = {"Зростання", "Спадання", "Змішаний"};

    int original[MAX_N], work[MAX_N];
    std::cout << "\n=== Порівняльний експеримент (n = 100) ===\n";
    padTo("Алгоритм", 14); padTo("Тип", 12);
    padTo("Порівняння", 14);
    padTo("Переміщення", 14);
    std::cout << "Перевірка\n";

    for (const Algorithm& alg : ALGORITHMS) {
        for (int k = 0; k < 3; ++k) {
            for (int i = 0; i < MAX_N; ++i) original[i] = sets[k][i];
            for (int i = 0; i < MAX_N; ++i) work[i] = original[i];

            Stats s;
            alg.sort(work, MAX_N, s);
            bool ok = verify(original, work, MAX_N);

            padTo(alg.name, 14);
            padTo(setNames[k], 12);
            std::cout << std::left << std::setw(14) << s.comparisons
                      << std::setw(14) << s.moves
                      << (ok ? "успішно" : "помилка");
            if (alg.sort == bubbleSort) std::cout << "  (проходів: " << s.passes << ")";
            std::cout << "\n";
        }
    }
}

void runIndividual(const int original[], int n) {
    std::cout << "\n=== Індивідуальний масив ===\n";
    printArray("Початковий масив", original, n);
    std::cout << "\n";

    int work[MAX_N];
    for (const Algorithm& alg : ALGORITHMS) {
        for (int i = 0; i < n; ++i) work[i] = original[i];
        Stats s;
        alg.sort(work, n, s);
        bool ok = verify(original, work, n);

        std::cout << alg.name << ":\n";
        printArray("   результат", work, n);
        std::cout << "   порівнянь = " << s.comparisons
                  << ", переміщень = " << s.moves;
        if (alg.sort == bubbleSort) std::cout << ", проходів = " << s.passes;
        std::cout << ", перевірка: " << (ok ? "масив упорядковано" : "ПОМИЛКА") << "\n";
    }
}

int main() {
    std::cout << "=== Практична робота 2: сортування масивів ===\n"
              << "Студентка: П'ятенко Софія, група 31-ІП, варіант 22\n\n";

    int original[MAX_N];
    int n = readInitialArray(original);

    runIndividual(original, n);
    runAutomatedTests();
    runExperiment();

    return 0;
}
