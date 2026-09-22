# Task Scheduler

Task Scheduler is a C++ library for building and executing computation graphs (a DAG: Directed Acyclic Graph) composed of typed tasks with explicit dependencies. It lets you declare tasks, wire their inputs to the outputs of other tasks, and execute the minimal required subset when results are requested.

## Overview

Many real-world problems can be expressed as a computation graph where nodes are tasks and edges are data dependencies. This library provides a class named `TTaskScheduler` to declare such graphs and run them deterministically.

Below is a simple example: computing average GPA per group from two CSV files `(isu, gpa)` and `(group, isu)`.

```cpp
TTaskScheduler scheduler;

auto students = scheduler.add(
    [](const std::string& filePath) {
        std::ifstream file(filePath);
        std::string line;
        std::getline(file, line); // skip header

        std::unordered_map<int, int> gpaByIsu;
        while (std::getline(file, line)) {
            int isu, gpa;
            std::sscanf(line.c_str(), "%d,%d", &isu, &gpa);
            gpaByIsu[isu] = gpa;
        }
        return gpaByIsu;
    },
    "students.csv"
);

auto groups = scheduler.add(
    [](const std::string& filePath) {
        std::ifstream file(filePath);
        std::string line;
        std::getline(file, line); // skip header

        std::unordered_map<std::string, std::vector<int>> groupToIsus;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string group;
            int isu;
            std::getline(ss, group, ',');
            ss >> isu;
            groupToIsus[group].push_back(isu);
        }
        return groupToIsus;
    },
    "groups.csv"
);

// Take a reference: such future results can be obtained many times
auto studentsFuture =
    students.getFutureResult<const std::unordered_map<int, int>&>();
// Take by value: move-only, no copies; therefore get() can be called exactly once
auto groupsFuture =
    groups.getFutureResult<std::unordered_map<std::string, std::vector<int>>>();

auto groupGpa = scheduler.add(
    [](
        const std::unordered_map<int, int>& gpaByIsu,
        std::unordered_map<std::string, std::vector<int>> groupToIsus // expect move
    ) {
        std::unordered_map<std::string, int> gpaByGroup;
        for (const auto& [group, isus] : groupToIsus) {
            int sum = 0, cnt = 0;
            for (const int isu : isus) {
                auto it = gpaByIsu.find(isu);
                if (it != gpaByIsu.end()) {
                    sum += it->second;
                    ++cnt;
                }
            }
            if (cnt != 0) {
                gpaByGroup[group] = sum / cnt;
            }
        }
        return gpaByGroup;
    },
    studentsFuture,
    groupsFuture
);

groupGpa.apply(
    [](std::unordered_map<std::string, int>&& gpaByGroup) {
        for (const auto& [group, gpa] : gpaByGroup) {
            std::cout << group << ": " << gpa << "\n";
        }
    }
);

scheduler.executeAll();

// This is invalid because we already moved; expect an exception
// groupsFuture.get()

// This is OK
const std::unordered_map<int, int>& gpaByIsu = studentsFuture.get();
```

## Public API: TTaskScheduler

- add - Takes a callable task and its arguments. Returns a `TTask` describing the scheduled task.
- executeAll - Executes all scheduled tasks.

## Public API: TTask

- getResultSync<T> - Returns the result of the task of type T. If the result is not computed yet, computes it without evaluating unrelated tasks.
- getFutureResult<T> - Returns an object that can later yield the result of the task (without triggering computation immediately), typed as T.
  If T is requested by value, the object is expected to be movable. A future result can be moved only once; subsequent attempts should throw an exception.
  If `cv T&` is requested, a reference is returned. Any number of references can be obtained.
- apply - Takes a callable and returns a `TTask`. Arguments for the callable are provided from the previous task output (see Task Requirements below). `apply` should support both rvalue and lvalue callables.

## Public API: TFuture

- The exact implementation is up to the library, but depending on how the future is created (see `getFutureResult` above), either a move or a reference is performed when retrieving the value.

## Task Requirements

- Callable object: https://en.cppreference.com/w/cpp/named_req/Callable
- Any number of arguments is supported.
- A task can be a pointer-to-member function. In this case, the first argument is the instance on which the method is invoked.
- `task.apply(func)` can be used only if `func` accepts exactly one argument. You may extend this to multiple arguments, e.g., when a task returns `std::tuple<Ts...>`.

## Example App: Recommendations

Using the implemented Task Scheduler, you can build a small app that:
1. Detects your location.
2. From the location:
   - Determines the weather.
   - Finds nearby interesting places: museums, parks, restaurants, etc.
3. Based on the weather, selects appropriate places (e.g., avoid parks during heavy rain).
4. Prints a short weather summary and N recommended places with distances.

You can use any APIs. Examples include:
1. 2ip - to determine latitude and longitude by IP address: https://2ip.ru/free/
2. Yandex Weather: https://yandex.ru/dev/weather/#start
3. Yandex Maps Geosearch: https://yandex.ru/maps-api/docs/geosearch-api/index.html
4. Distance Matrix or Route Details: https://yandex.ru/maps-api/docs/distancematrix-api/index.html and https://yandex.ru/maps-api/docs/router-api/index.html

## Constraints

The core implementation is designed to avoid most of the standard library except for containers and smart pointers:
- Containers: https://en.cppreference.com/w/cpp/container
- Smart pointers: https://en.cppreference.com/w/cpp/memory

## Testing

The library should be covered by tests. Focus on both correctness and the breadth of test coverage for the critical code paths.

## Notes

1. You can leverage ideas from Type Erasure: https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Type_Erasure
2. Consider where `std::forward` and perfect forwarding are appropriate in this design.
3. Some schedules may be invalid or not executable; think about error handling and detection.
4. The recommendation app is meant as a practical showcase for the library, not as an exercise in ranking quality.
5. For simpler JSON parsing, consider the nlohmann JSON macros: https://json.nlohmann.me/features/arbitrary_types/#simplify-your-life-with-macros
6. Where appropriate, some tasks may be executed asynchronously.
