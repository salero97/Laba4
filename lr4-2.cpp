#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <numeric>

using namespace std;

double getRandomDouble(double min, double max) {
    // создаем статический генератор случайных чисел с алгоритмом ranlux24
    static mt19937 generator(random_device{}()); // инициализация генератора случайных чисел
    static ranlux24 engine(generator()); // создание движка ranlux24 на основе генератора
    uniform_real_distribution<double> distribution(min, max); // равномерное распределение
    return distribution(engine); // возвращаем случайное число из диапазона
}

// функция для заполнения вектора случайными числами в диапазоне [min, max]
void fillVectorWithRandomNumbers(vector<double>& vec, double min, double max) {
    for (auto& element : vec) { // перебираем все элементы вектора по ссылке
        element = getRandomDouble(min, max); // присваиваем случайное число
    }
}

// функция для подсчета количества отрицательных элементов в векторе
int countNegativeElements(const vector<double>& vec) {
    int count = 0; // счетчик отрицательных элементов
    for (const auto& element : vec) { // перебираем все элементы по константной ссылке
        if (element < 0) { // если элемент отрицательный
            count++; // увеличиваем счетчик
        }
    }
    return count; // возвращаем итоговое количество
}

// функция для нахождения индекса минимального по модулю элемента в векторе
size_t findMinAbsElementIndex(const vector<double>& vec) {
    size_t minIndex = 0; // предполагаемый индекс минимального по модулю элемента
    double minAbs = abs(vec[0]); // минимальный по модулю элемент изначально первый
    for (size_t i = 1; i < vec.size(); ++i) { // перебираем остальные элементы
        if (abs(vec[i]) < minAbs) { // если текущий по модулю меньше текущего минимального
            minAbs = abs(vec[i]); // обновляем минимальное значение
            minIndex = i; // обновляем индекс
        }
    }
    return minIndex; // возвращаем индекс минимального по модулю элемента
}

// функция для вычисления суммы модулей элементов после минимального по модулю элемента
double sumAbsAfterMinAbs(const vector<double>& vec, size_t minIndex) {
    double sum = 0; // переменная для суммы
    for (size_t i = minIndex + 1; i < vec.size(); ++i) { // перебираем элементы после minIndex
        sum += abs(vec[i]); // добавляем абсолютное значение элемента к сумме
    }
    return sum; // возвращаем сумму
}

// функция для поиска самой длинной убывающей последовательности в векторе
void findLongestDecreasingSequence(const vector<double>& vec, size_t& start, size_t& end) {
    size_t currentStart = 0; // текущий старт убывающей последовательности
    size_t maxLength = 1; // длина самой длинной убывающей последовательности
    start = 0; // начальный индекс самой длинной убывающей последовательности
    end = 0; // конечный индекс самой длинной убывающей последовательности

    for (size_t i = 1; i < vec.size(); ++i) { // перебираем элементы начиная со второго
        if (vec[i] < vec[i - 1]) { // если текущий элемент меньше предыдущего
            if (i - currentStart + 1 > maxLength) { // если длина текущей последовательности больше максимальной
                maxLength = i - currentStart + 1; // обновляем длину
                start = currentStart; // обновляем старт
                end = i; // обновляем конец
            }
        } else { // если последовательность нарушается
            currentStart = i; // начинаем новую последовательность с текущего элемента
        }
    }
}

// функция для создания нового вектора с обратной последовательностью элементов из диапазона [start, end]
vector<double> createReversedSequence(const vector<double>& vec, size_t start, size_t end) {
    vector<double> reversedSeq; // создаем пустой вектор для обратной последовательности
    for (size_t i = end; i >= start; --i) { // идем с end к start
        reversedSeq.push_back(vec[i]); // добавляем элементы в обратном порядке
        if (i == 0) break; // чтобы не выйти за границы при i--, если i достиг 0
    }
    return reversedSeq; // возвращаем полученную последовательность
}

// функция для циклического сдвига вектора вправо на один элемент
void cyclicShiftRight(vector<double>& vec) {
    if (vec.empty()) return; // если вектор пустой, ничего не делаем
    double last = vec.back(); // сохраняем последний элемент
    for (size_t i = vec.size() - 1; i > 0; --i) { // с конца к началу
        vec[i] = vec[i - 1]; // сдвигаем элементы вправо
    }
    vec[0] = last; // последний элемент становится первым
}

// функция для вывода вектора на экран с сообщением
void printVector(const vector<double>& vec, const string& message) {
    cout << message << ": "; // выводим сообщение
    for (const auto& element : vec) { // перебираем все элементы
        cout << element << " "; // выводим элемент с пробелом
    }
    cout << endl; // перевод строки
}

int main() {
    // создаем и заполняем вектор случайными числами
    const int n = 20; // размер вектора
    vector<double> numbers(n); // объявляем вектор из 20 элементов
    fillVectorWithRandomNumbers(numbers, -250, 250); // заполняем его случайными числами в диапазоне [-250, 250]
    printVector(numbers, "исходный массив"); // выводим исходный массив

    // находим количество отрицательных элементов
    int negCount = countNegativeElements(numbers); // считаем отрицательные
    cout << "количество отрицательных элементов: " << negCount << endl; // выводим результат

    // находим индекс минимального по модулю элемента
    size_t minAbsIndex = findMinAbsElementIndex(numbers); // ищем индекс минимального по модулю
    // вычисляем сумму модулей элементов после этого индекса
    double sumAfterMinAbs = sumAbsAfterMinAbs(numbers, minAbsIndex);
    cout << "сумма модулей элементов после минимального по модулю: " << sumAfterMinAbs << endl; // выводим сумму

    // находим самую длинную убывающую последовательность
    size_t startSeq, endSeq; // переменные для начала и конца последовательности
    findLongestDecreasingSequence(numbers, startSeq, endSeq); // ищем последовательность
    cout << "самая длинная убывающая последовательность: ";
    for (size_t i = startSeq; i <= endSeq; ++i) { // выводим элементы этой последовательности
        cout << numbers[i] << " ";
    }
    cout << endl; // перевод строки
    cout << "индексы начала и конца: " << startSeq << " - " << endSeq << endl; // выводим индексы

    // создаем новый вектор с обратной последовательностью
    vector<double> reversedSeq = createReversedSequence(numbers, startSeq, endSeq); // создаем
    printVector(reversedSeq, "обращенная последовательность"); // выводим его

    // создаем два новых вектора и выполняем циклический сдвиг
    vector<double> arr1 = {1, 2, 3, 4, 5}; // первый массив
    vector<double> arr2 = {1, 2, 3}; // второй массив
    printVector(arr1, "первый массив до сдвига"); // выводим до сдвига
    printVector(arr2, "второй массив до сдвига"); // выводим до сдвига

    // выполняем циклический сдвиг вправо для обоих векторов
    cyclicShiftRight(arr1); // сдвиг первого массива
    cyclicShiftRight(arr2); // сдвиг второго массива
    printVector(arr1, "первый массив после сдвига");
    printVector(arr2, "второй массив после сдвига");

    return 0;
}
