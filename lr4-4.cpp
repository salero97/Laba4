#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// функция для определения победителей по методам Борда и Кондорсе
void determineWinners(int n, int k, const vector<vector<string>>& preferences, const vector<string>& candidates) {
    // --- метод борда ---
    vector<int> bordaScores(n, 0); // массив для подсчета баллов каждого кандидата, инициализирован нулями

    // подсчет баллов по предпочтениям избирателей
    for (const auto& voter : preferences) { // перебираем каждого избирателя
        for (size_t pos = 0; pos < voter.size(); ++pos) { // перебираем позиции в списке предпочтений
            string cand = voter[pos]; // текущий кандидат по предпочтениям
            // ищем индекс кандидата в списке кандидатов
            auto it = find(candidates.begin(), candidates.end(), cand); // ищем позицию кандидата
            int idx = distance(candidates.begin(), it); // получаем индекс кандидата
            // начисляем баллы: n-1 за 1-е место, n-2 за 2-е и так далее
            bordaScores[idx] += (n - 1 - pos); // добавляем баллы за позицию
        }
    }

    // ищем победителя по методу борда (максимальный балл)
    int maxBordaScore = -1; // переменная для хранения максимального балла
    string bordaWinner; // переменная для хранения имени победителя
    for (int i = 0; i < n; ++i) { // перебираем всех кандидатов
        if (bordaScores[i] > maxBordaScore) { // если текущий балл больше текущего максимума
            maxBordaScore = bordaScores[i]; // обновляем максимум
            bordaWinner = candidates[i]; // запоминаем имя победителя
        }
    }

    // --- метод кондорсе ---
    // матрица парных сравнений: сколько избирателей предпочитают i над j
    vector<vector<int>> pairwiseWins(n, vector<int>(n, 0)); // создаем матрицу n x n, заполненную нулями

    // подсчет парных побед
    for (int i = 0; i < n; ++i) { // перебираем кандидатов для сравнения
        for (int j = 0; j < n; ++j) { // сравниваем с другими кандидатами
            if (i == j) continue; // пропускаем сравнение с самим собой
            int wins = 0; // счетчик побед кандидата i над j
            for (const auto& voter : preferences) { // перебираем каждого избирателя
                // ищем позиции кандидатов i и j в предпочтениях избирателя
                auto pos_i = find(voter.begin(), voter.end(), candidates[i]);
                auto pos_j = find(voter.begin(), voter.end(), candidates[j]);
                int idx_i = distance(voter.begin(), pos_i); // позиция i
                int idx_j = distance(voter.begin(), pos_j); // позиция j
                if (idx_i < idx_j) { // если кандидат i выше j в списке
                    wins++; // увеличиваем счетчик побед i над j
                }
            }
            pairwiseWins[i][j] = wins; // записываем количество побед i над j
        }
    }

    // ищем победителя по методу кондорсе
    string condorcetWinner; // переменная для победителя по кондорсе
    bool hasCondorcet = false; // флаг наличия победителя по кондорсе
    for (int i = 0; i < n; ++i) { // перебираем кандидатов
        bool winsAll = true; // флаг, что кандидат побеждает всех
        for (int j = 0; j < n; ++j) { // сравниваем с каждым другим кандидатом
            if (i == j) continue; // пропускаем сравнение с самим собой
            // если кандидат i не выигрывает у j в большинстве
            if (pairwiseWins[i][j] <= pairwiseWins[j][i]) { // если i не побеждает j
                winsAll = false; // кандидат не побеждает всех
                break; // выходим из цикла
            }
        }
        if (winsAll) { // если кандидат побеждает всех
            hasCondorcet = true; // есть победитель по кондорсе
            condorcetWinner = candidates[i]; // запоминаем его имя
            break; // выходим из поиска
        }
    }

    // вывод результатов
    cout << "Победитель по методу борда: " << bordaWinner << " (баллы: " << maxBordaScore << ")\n"; // вывод победителя по борда
    if (hasCondorcet) { // если есть победитель по кондорсе
        cout << "Победитель по методу Кондорсе: " << condorcetWinner << "\n"; // вывод его
    } else { // если победителя нет
        cout << "Победитель по методу Кондорсе: отсутствует (парадокс Кондорсе)\n"; // сообщение
    }

    // сравнение результатов
    if (hasCondorcet && bordaWinner != condorcetWinner) { // если есть оба победителя и они разные
        cout << "Комментарий: разные алгоритмы дали разные результаты. "
             << "Борда учитывает позиции, а Кондорсе — парные сравнения.\n"; // комментарий
    }
}

int main() {
    int n, k; // переменные для количества кандидатов и избирателей
    // ввод количества кандидатов и избирателей
    cout << "Введите число кандидатов (n): ";
    cin >> n; // ввод n
    cout << "Введите число избирателей (k): ";
    cin >> k; // ввод k

    // ввод имен кандидатов
    vector<string> candidates(n); // создаем вектор для имен кандидатов
    cout << "Введите имена кандидатов:\n"; // сообщение
    for (int i = 0; i < n; ++i) { // цикл по кандидатам
        cin >> candidates[i]; // ввод имени
    }

    // ввод предпочтений избирателей
    vector<vector<string>> preferences(k, vector<string>(n)); // создаем матрицу предпочтений
    cout << "Введите предпочтения каждого избирателя (через пробел):\n"; // сообщение
    cin.ignore(); // очищаем буфер ввода
    for (int i = 0; i < k; ++i) { // цикл по избирателям
        string line; // строка для ввода предпочтений
        getline(cin, line); // считываем строку
        size_t pos = 0; // позиция для поиска пробелов
        int idx = 0; // индекс текущего кандидата в предпочтениях
        while ((pos = line.find(' ')) != string::npos) { // пока есть пробелы
            preferences[i][idx++] = line.substr(0, pos); // извлекаем кандидат
            line.erase(0, pos + 1); // удаляем обработанную часть
        }
        preferences[i][idx] = line; // последний кандидат после последнего пробела
    }

    // вызов функции для определения победителей
    determineWinners(n, k, preferences, candidates); // передаем параметры

    // тестовые сценарии
    cout << "\n--- Тестирование на различных наборах данных ---\n";

    // тест 1: все одинаковые предпочтения
    cout << "\nТест 1: все избиратели имеют одинаковые предпочтения\n";
    vector<vector<string>> test1Prefs = {{"Петя", "Вася", "Оля"}, {"Петя", "Вася", "Оля"}}; // предпочтения
    determineWinners(3, 2, test1Prefs, {"Петя", "Вася", "Оля"}); // вызов функции

    // тест 2: парадокс Кондорсе (циклические предпочтения)
    cout << "\nТест 2: парадокс Кондорсе\n";
    vector<vector<string>> test2Prefs = {
        {"Петя", "Вася", "Оля"},
        {"Вася", "Оля", "Петя"},
        {"Оля", "Петя", "Вася"}
    }; // предпочтения
    determineWinners(3, 3, test2Prefs, {"Петя", "Вася", "Оля"}); // вызов функции

    // тест 3: разные результаты
    cout << "\nТест 3: разные результаты для методов\n";
    vector<vector<string>> test3Prefs = {
        {"Петя", "Вася", "Оля"},
        {"Вася", "Оля", "Петя"},
        {"Вася", "Оля", "Петя"}
    }; // предпочтения
    determineWinners(3, 3, test3Prefs, {"Петя", "Вася", "Оля"}); // вызов функции

    return 0; // завершение программы