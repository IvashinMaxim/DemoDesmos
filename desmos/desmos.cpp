#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

struct EquationSolution {
    std::string equationType;
    std::string solution;

    EquationSolution(const std::string& type, const std::string& sol) : equationType(type), solution(sol) {}
};

class EquationSolver {
private:
    double a, b, c;
    std::vector<EquationSolution> history;

public:
    
    void addSolution(const std::string& type, const std::string& sol) {
        history.emplace_back(type, sol);
    }

    void printHistory() const {
        if (history.empty()) {
            std::cout << "История решений уравнений пуста.\n";
        }
        else {
            std::cout << "\nИстория решений уравнений:\n";
            for (const auto& entry : history) {
                std::cout << "[" << entry.equationType << "] " << entry.solution << "\n";
            }
        }
    }

    void deleteSolution(int index) {
        if (index >= 0 && index < history.size()) {
            history.erase(history.begin() + index);
            std::cout << "Решение удалено.\n";
        }
        else {
            std::cout << "Неверный индекс для удаления.\n";
        }
    }

    void updateSolution(int index, const std::string& newType, const std::string& newSol) {
        if (index >= 0 && index < history.size()) {
            
            std::cout << "Решение обновлено.\n";
        }
        else {
            std::cout << "Неверный индекс для обновления.\n";
        }
    }

    int getHistorySize() const {
        return history.size();
    }

    void createSolution(char eqType) {
        std::string equationType;
        std::string solution;
        double a, b, c;
        if (eqType == 'l') {
            equationType = "Линейное";
            std::cout << "Введите коэффициенты линейного уравнения (ax + b = 0): ";
            std::cin >> a >> b;
            if (a == 0) {
                if (b == 0) {
                    std::cout << "Уравнение недопустимо (бесконечное количество решений)." << std::endl;
                    solution = "Уравнение не является линейным";
                }
                else {
                    std::cout << "Уравнение не является линейным." << std::endl;
                    solution = "Уравнение не является линейным";
                }
            }
            else {
                solution="x="+std::to_string(-b / a);
            }
        }
        else if (eqType == 'q') {
            std::cout << "Введите коэффициенты квадратного уравнения (ax^2 + bx + c = 0): ";
            std::cin >> a >> b >> c;
            if (a != 0) {
                double discriminant = b * b - 4 * a * c;
                if (std::abs(discriminant) < 1e-10) {
                    equationType = "Квадратное";
                    solution = "x = " + std::to_string(-b / (2 * a));
                }
                else if (discriminant > 0) {
                    equationType = "Квадратное";
                    solution = "x1 = " + std::to_string((-b + std::sqrt(discriminant)) / (2 * a)) + " и x2 = " + std::to_string((-b - std::sqrt(discriminant)) / (2 * a));
                }
                else {
                    equationType = "Квадратное";
                    solution = "x1 = " + std::to_string(-b / (2 * a)) + " + " + std::to_string(std::sqrt(-discriminant) / (2 * a)) + "i и x2 = " + std::to_string(-b / (2 * a)) + " - " + std::to_string(std::sqrt(-discriminant) / (2 * a)) + "i";
                }
            }
            else {
                equationType = "Квадратное";
                solution = "Уравнение не является квадратным.";
            }
        }
        else {
            std::cout << "Неверный тип уравнения.\n";
        }
        addSolution(equationType, solution);
    }

};

int main() {
    system("chcp 65001");
    char equationType;
    EquationSolver solver;
    do {
        std::cout << "Выберите операцию (c - создать, r - прочитать, u - обновить, d - удалить, e - выход): ";
        std::cin >> equationType;
        switch (equationType) {
        case 'c':
            char eqType;
            std::cout << "Выберите тип уравнения (l - линейное, q - квадратное): ";
            std::cin >> eqType;           
            solver.createSolution(eqType);
            break;
        case 'r':
            solver.printHistory();
            break;
        case 'u':
            int index;
            std::cout << "Введите индекс решения для обновления: ";
            std::cin >> index;
            solver.printHistory();
            if (index >= 0 && index < solver.getHistorySize()) {
                
            }
            else {
                std::cout << "Неверный индекс для обновления.\n";
            }
            break;
        case 'd':
            int delIndex;
            std::cout << "Введите индекс решения для удаления: ";
            std::cin >> delIndex;
            solver.deleteSolution(delIndex);
            break;
        case 'e':
            std::cout << "Программа завершена.\n";
            break;
        default:
            std::cout << "Неправильная операция.\n";
        }

    } while (equationType != 'e');

    return 0;
}
