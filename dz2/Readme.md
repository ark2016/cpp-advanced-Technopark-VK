# Загрузка самолета

## Описание задачи

Мы хотим смоделировать задачу загрузки и контроля самолета перед вылетом.

### Условия задачи

На борту самолета находятся:
- 2 пилота
- 6 бортпроводников
- До 4 пассажиров первого класса
- До 10 пассажиров бизнес класса
- До 200 пассажиров эконом класса

### Правила провоза ручной клади

- Для пассажиров из эконом класса предоставляется право провоза ручной клади не более 10 килограмм.
- Для пассажиров бизнес класса - две позиции не более 24 килограмм.
- Для пассажиров первого класса - две позиции не более 60 килограмм.

### Правила провоза багажа

Каждый пассажир может приобрести себе место для багажа за дополнительную плату:
- Для пассажиров из эконом класса предоставляется право приобрести одну позицию для багажа весом не более 24 килограмм.
- Для пассажиров бизнес класса - две позиции не более 40 килограмм.
- Для пассажиров первого класса - две позиции не ограниченных в весе.

### Упрощения

- Члены экипажа (пилоты и бортпроводники) не имеют ручной клади и багажа.
- В случае перегрузки багажного отсека сегмента первого или бизнес класса, багаж очередного пассажира из этих сегментов располагается в отсеке эконом класса.
- В случае, если багажный отсек эконом класса заполнен настолько, что багаж очередного пассажира из первого или бизнес класса не помещается в этот сегмент, необходимо снять с рейса минимальное количество единиц багажа, чтобы разместить багаж очередного пассажира.
- Для пассажиров эконом класса действуют обычные правила по заполнению багажного отсека: зарегистрировать багаж, если под него есть место, иначе - снять багаж с рейса.
- Мы никогда не снимаем ручную кладь.

### Условия задачи

Описать систему классов пассажиров, членов экипажа и сегментов самолета, предоставляя возможность через интерфейс получить информацию о загруженности каждого сегмента класса по ручной клади и занимаемому багажу. Ваш класс самолета должен предоставлять метод посадки через абстрактный класс юнита, корректно обрабатывая конечный тип (тип пассажира, экипажа) и выбирая корректный сегмент самолета, куда его посадить и как обработать багаж. Класс самолета должен предоставлять метод получения общей загруженности багажа, при этом внутри не прибегая к цикличному обходу всего списка пассажиров, а работая с отдельными сегментами классов самолета и получая информацию через них.

### Входные данные

На вход программе будет подаваться из консоли набор строк следующего формата:
```
<TYPE> <HAND LUGGAGE WEIGHT> [<HAND LUGGAGE WEIGHT>] <LUGGAGE WEIGHT> [<LUGGAGE WEIGHT>]
```
Например:
```
FIRST_CLASS_SEGMENT 1000      # макс. вес перевозки в первом классе - 1000 кг.
BUSINESS_CLASS_SEGMENT 2000    # макс. вес перевозки в бизнес классе - 2000 кг.
ECONOMY_CLASS_SEGMENT 3000    # макс. вес перевозки в эконом классе - 3000 кг.
PILOT 0 0                      # нет багажа
FLIGHT_ATTENDANT 0 0          # нет багажа
ECONOMY 5 20                   # 1 ручная кладь: 5 кг, 1 багаж: 20 кг.
ECONOMY 5                      # 1 ручная кладь: 5 кг.
BUSINESS 0 10 34               # 2 ручные клади: 0 и 10 кг, 1 багаж: 34 кг.
BUSINESS 0 10                  # 2 ручные клади: 0 и 10 кг.
BUSINESS 0 0                   # 2 ручные клади: 0 и 0 кг.
FIRST_CLASS 30 30 40 20       # 2 ручные клади: 30 и 30 кг, 2 багажа: 40 и 20 кг.
FIRST_CLASS 0 0 50 40         # 2 ручные клади: 0 и 0 кг, 2 багажа: 50 и 40 кг.
... and so on
```

### Гарантии

- Гарантируется, что первые 11 записей будут отнесены к составу самолета: 0-2 - сегменты самолета (могут передаваться в произвольном порядке), 3 и 4 - пилоты, 5-10 - бортпроводники. Все строки далее относятся к пассажирам.
- Гарантируется, что всем членам экипажа будут поданы нулевые веса ручной клади и багажа.

### Требования

1. Каждому юниту на борту выдать свой уникальный идентификатор.
2. В случае, если очередного пассажира невозможно добавить, напечатать в консоль “!!CANT REGISTER {TYPE} PASSENGER, ID = {ID}!!”, где {TYPE} - один из [“PILOT”, “FLIGHT ATTENDANT”, “ECONOMY”, “BUSINESS”, “FIRST CLASS”], {ID} - уникальный идентификатор юнита из пункта 1.
3. В случае, если вес очередного пассажира снят с рейса, напечатать в консоль “!!PASSENGER’S LUGGAGE REMOVED FROM FLIGHT, ID = {ID}!!”, где {ID} - уникальный идентификатор юнита из пункта 1.
4. Все методы подсчета веса юнита работают за O(1).

### Что нужно реализовать?

Задачи подобного рода хорошо укладываются на паттерн “Компоновщик” (англ. - “Composite”, если есть VPN - описание с refactoring.guru) из GoF (Gang of Four). В данном случае простым объектом будут классы пилота, бортпроводника и пассажиров, а составными - сегменты первого, бизнес и эконом класса.

### Ограничения

- Можно считать, что багаж пассажира из первого или бизнес класса, который не уместился в свой сегмент, обязательно поместится в отсек эконом класса (задаем ограничение: “суммарный перевозимый вес эконом класса >>> суммарная переносимость веса в первом и бизнес классе”). Постусловия ограничения - никогда не будет ситуации, требующей вытеснения багажа пассажира из первого или бизнес класса другим багажом из этих сегментов.
- Мы заполняем максимально под завязку багажный отсек каждого сегмента. Если какой-то вес помещается полностью в свой сегмент, то он остается там, иначе - либо переносится в эконом сегмент, либо не допускается на рейс (см. тип каждого пассажира и ограничения).
- Считается, что под ручную кладь ВСЕГДА есть место, если есть место для размещения пассажира.

### Оформление

Для решения задачи необходимо оформить следующие файлы:
1. `units/unit.hpp` - файл с интерфейсом юнита.
2. `units/crew_members.hpp`, `units/crew_members.cpp` - файл для юнитов экипажа самолета.
3. `units/passengers.hpp`, `units/passengers.cpp` - файл для юнитов пассажира.
4. `units/plane.hpp`, `units/plane.cpp` - файл для самолета.
5. `tests/test_crew_members.cpp`, `tests/test_passengers.cpp`, `tests/test_plane.cpp` - файлы с юнит и интеграционными тестами ваших классов.
6. `main.cpp` - программа, реализующая обработку списка пассажиров с помощью класса самолета.

### Как будет проверяться?

1. В проверяющей системе будет установлено icu.

```sh
# https://github.com/WebKit/WebKit/blob/main/.clang-format
find ./ -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format-10 -i -style=file --dry-run --Werror
```

2. Сборка и запуск тестов:

```sh
g++ --coverage -g -O0 -fsanitize=address -std=c++20 \
    -Wall -Werror -Wextra units/crew_members.cpp \
    tests/test_crew_members.cpp -o \
    test_crew_members

g++ --coverage -g -O0 -fsanitize=address -std=c++20 \
    -Wall -Werror -Wextra units/passengers.cpp \
    tests/test_passengers.cpp -o \
    test_passengers

g++ --coverage -g -O0 -fsanitize=address -std=c++20 \
    -Wall -Werror -Wextra units/plane.cpp \
    units/passengers.cpp units/crew_members.cpp \
    tests/test_plane.cpp -o test_plane

# Должны проходить, а покрытие должно составить >= 90% в каждом
./test_crew_members
./test_passengers
./test_plane
```

3. Сборка и запуск основной программы:

```sh
g++ -std=c++20 -O2 -fsanitize=address \
    -Wall -Werror -Wextra \
    -I ./units/ main.cpp \
    units/crew_members.cpp  units/passengers.cpp units/plane.cpp \
    -o main

# Проверяются тесты с stdin,
# как в обычном спортивном программировании
./main
```

### Критерии оценки

За основу берется оценка вашего решения по верхнему уровню (10 баллов), далее с него могут сниматься баллы:
- 0 баллов сразу: копирование кода (обоим), решение не выполняет поставленные задачи в условии.
- -1 балл: дублирование кода, отсутствие разделения на функции.
- -1 балл: необоснованные копирования, неиспользование move-семантики и т.п.
- -1 балл: нечитаемый код, непонятные названия функций, переменных и т.п.
- -2 балла: допущенные UB, "вылет" за границы массива (если вдруг не поймал санитайзер), прочие грубые логические ошибки.
- -3 балла: код выполнен без учета абстракций, требуемых в задании.

To check code coverage using your CMake setup and GCC, you can follow these steps. Code coverage will help you identify which parts of your code are being tested and which are not, allowing you to improve your test suite.

### Проверка на покрытие

```bash
mkdir build
cd build
cmake -DCOVERAGE=ON ..
make
```

```bash
./test_crew_members
./test_passengers
./test_plane
```

```bash
lcov --capture --directory . --output-file coverage.info
```

```bash
lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage_filtered.info
```
```bash
genhtml coverage_filtered.info --output-directory out
```
     
```bash
xdg-open out/index.html
```

![image](https://github.com/user-attachments/assets/a8ce7d25-bbd7-4be4-8535-b38f4ebd6a15)



