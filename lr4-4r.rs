use std::io::{self, Write}; // подключение стандартных библиотек для ввода-вывода

// функция для определения победителей по методам борда и кондорсе
fn opredelit_pobediteliv(n: usize, k: usize, predpocnenia: &Vec<Vec<String>>, kandidati: &Vec<String>) {
    // --- метод борда ---
    let mut balli = vec![0; n]; // создаём вектор для хранения баллов каждого кандидата, изначально все нули

    // перебираем каждого избирателя
    for izbiratel in predpocnenia { // цикл по спискам предпочтений избирателей
        // перебираем позиции и кандидатов в предпочтениях текущего избирателя
        for (pos, kandidata) in izbiratel.iter().enumerate() { // pos — позиция, kandidata — имя кандидата
            // находим индекс текущего кандидата в списке всех кандидатов
            let ind_k = kandidati.iter().position(|c| c == kandidata).unwrap(); // ищем индекс по имени
            // начисляем баллы: n-1 за первое место, n-2 за второе и так далее
            balli[ind_k] += (n - 1 - pos) as i32; // добавляем баллы за текущую позицию
        }
    }

    // ищем победителя по борду — кандидата с максимальным количеством баллов
    let mut max_balli = 0; // переменная для хранения максимальных баллов
    let mut pobed_bord = String::new(); // переменная для хранения имени победителя по борду
    for (i, &score) in balli.iter().enumerate() { // перебираем всех кандидатов и их баллы
        if score > max_balli { // если текущий кандидат набрал больше баллов
            max_balli = score; // обновляем максимум
            pobed_bord = kandidati[i].clone(); // запоминаем имя победителя
        }
    }

    // --- метод кондорсе ---
    // создаём матрицу парных побед: сколько раз кандидат i лучше j
    let mut pari_wins = vec![vec![0; n]; n]; // матрица n x n, изначально нули

    // считаем парные сравнения для каждого кандидата с каждым
    for i in 0..n { // цикл по кандидатам
        for j in 0..n { // цикл по кандидатам
            if i == j { continue; } // пропускаем сравнение с самим собой
            let mut pobedil = 0; // счётчик побед кандидата i над j
            // перебираем всех избирателей
            for izbiratel in predpocnenia {
                // находим позиции кандидатов i и j в предпочтениях текущего избирателя
                let pos_i = izbiratel.iter().position(|c| c == &kandidati[i]).unwrap();
                let pos_j = izbiratel.iter().position(|c| c == &kandidati[j]).unwrap();
                // если кандидат i выше j в списке предпочтений
                if pos_i < pos_j {
                    pobedil += 1; // кандидат i побеждает в парном сравнении
                }
            }
            // записываем количество побед i над j
            pari_wins[i][j] = pobedil;
        }
    }

    // ищем победителя по методу кондорсе — кандидат, побеждающий всех остальных
    let mut pobed_cond = String::new(); // переменная для победителя по кондорсе
    let mut est_condorcet = false; // флаг наличия победителя по кондорсе
    for i in 0..n { // перебираем всех кандидатов
        let mut pobedil_vse = true; // предполагаем, что кандидат побеждает всех
        for j in 0..n { // сравниваем с каждым другим кандидатом
            if i == j { continue; } // пропускаем сравнение с самим собой
            // если кандидат i не побеждает j в большинстве случаев
            if pari_wins[i][j] <= pari_wins[j][i] {
                pobedil_vse = false; // кандидат i не побеждает всех
                break; // выходим из цикла
            }
        }
        // если кандидат побеждает всех
        if pobedil_vse {
            est_condorcet = true; // есть победитель по кондорсе
            pobed_cond = kandidati[i].clone(); // запоминаем его имя
            break; // больше искать не нужно
        }
    }

    // выводим результаты
    println!("победитель по методу борда: {} (баллы: {})", pobed_bord, max_balli);
    if est_condorcet {
        println!("победитель по методу кондорсе: {}", pobed_cond);
    } else {
        println!("победитель по методу кондорсе: отсутствует (парадокс кондорсе)");
    }

    // сравниваем результаты и выводим комментарий
    if est_condorcet && pobed_bord != pobed_cond {
        println!("комментарий: разные алгоритмы дают разные результаты");
    }
}

fn main() {
    // просим пользователя ввести число кандидатов
    print!("введите число кандидатов (n): ");
    io::stdout().flush().unwrap(); // очищаем буфер вывода
    let mut vvod = String::new(); // создаём строку для ввода
    io::stdin().read_line(&mut vvod).unwrap(); // читаем строку
    let n: usize = vvod.trim().parse().unwrap(); // парсим число

    // просим ввести число избирателей
    print!("введите число избирателей (k): ");
    io::stdout().flush().unwrap();
    vvod.clear(); // очищаем строку
    io::stdin().read_line(&mut vvod).unwrap();
    let k: usize = vvod.trim().parse().unwrap();

    // вводим имена кандидатов
    let mut kandidati = Vec::with_capacity(n); // создаём вектор для кандидатов
    println!("введите имена кандидатов:");
    for _ in 0..n { // цикл по кандидатам
        vvod.clear();
        io::stdin().read_line(&mut vvod).unwrap(); // читаем имя
        kandidati.push(vvod.trim().to_string()); // добавляем в список
    }

    // ввод предпочтений избирателей
    let mut predpocnenia = Vec::with_capacity(k); // список предпочтений
    println!("введите предпочтения каждого избирателя (через пробел):");
    for _ in 0..k { // цикл по избирателям
        vvod.clear();
        io::stdin().read_line(&mut vvod).unwrap(); // читаем предпочтения
        // разбиваем строку по пробелам и создаём список
        let pred: Vec<String> = vvod.trim().split_whitespace().map(|s| s.to_string()).collect();
        predpocnenia.push(pred); // добавляем в список
    }

    // вызываем функцию для определения победителей
    opredelit_pobediteliv(n, k, &predpocnenia, &kandidati);

    // тестовые примеры
    println!("\n--- тестовые примеры ---\n");

    // пример 1: все одинаковые предпочтения
    println!("пример 1: все одинаковые предпочтения");
    let pred1 = vec![
        vec!["Петя".to_string(), "Вася".to_string(), "Оля".to_string()],
        vec!["Петя".to_string(), "Вася".to_string(), "Оля".to_string()],
    ];
    // вызываем функцию для теста
    opredelit_pobediteliv(3, 2, &pred1, &vec!["Петя".to_string(), "Вася".to_string(), "Оля".to_string()]);

    // пример 2: цикл Кондорсе
    println!("\nпример 2: цикл Кондорсе");
    let pred2 = vec![
        vec!["Петя".to_string(), "Вася".to_string(), "Оля".to_string()],
        vec!["Вася".to_string(), "Оля".to_string(), "Петя".to_string()],
        vec!["Оля".to_string(), "Петя".to_string(), "Вася".to_string()],
    ];
    // вызываем функцию для теста
    opredelit_pobediteliv(3, 3, &pred2, &vec!["Петя".to_string(), "Вася".to_string(), "Оля".to_string()]);

    // пример 3: разные результаты
    println!("\nпример 3: разные результаты");
    let pred3 = vec![
        vec!["Петя".to_string(), "Вася".to_string(), "Оля".to_string()],
        vec!["Вася".to_string(), "Оля".to_string(), "Петя".to_string()],
        vec!["Вася".to_string(), "Оля".to_string(), "Петя".to_string()],
    ];
    // вызываем функцию для теста
    opredelit_pobediteliv(3, 3, &pred3, &vec!["Петя".to_string(), "Вася".to_string(), "Оля".to_string()]);
}
