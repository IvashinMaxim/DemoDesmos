#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>

struct EquationSolution {
	std::string equationType;
	std::string solution;
	double a, b, c;

	EquationSolution(const std::string& type, const std::string& sol, double a_val, double b_val, double c_val = 0.0)
		: equationType(type), solution(sol), a(a_val), b(b_val), c(c_val) {}
};

class EquationSolver {
private:
	std::vector<EquationSolution> history;

public:

	void addSolution(const std::string& type, const std::string& sol, double a, double b, double c) {
		history.emplace_back(type, sol, a, b, c);
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

	void updateSolution(int index) {
		double a, b, c;
		char eqType;
		std::cout << "Выберите новый тип уравнения (l - линейное, q - квадратное): ";
		std::cin >> eqType;
		std::string solution;
		if (eqType == 'l') {
			std::cout << "Введите новые значения коэффициентов a, b: ";
			std::cin >> history[index].a >> history[index].b;
			history[index].equationType = "Линейное";
			history[index].solution = "x=" + std::to_string(-history[index].b / history[index].a);
			std::cout << "Решение обновлено.\n";
		}
		else if (eqType == 'q') {
			std::cout << "Введите новые значения коэффициентов a, b и c: ";
			std::cin >> history[index].a >> history[index].b >> history[index].c;
			a = history[index].a;
			b = history[index].b;
			c = history[index].c;
			history[index].equationType = "Квадратное";
			if (a != 0) {
				double discriminant = b * b - 4 * a * c;
				if (std::abs(discriminant) < 1e-10) {
					history[index].solution = "x = " + std::to_string(-b / (2 * a));
				}
				else if (discriminant > 0) {
					history[index].solution = "x1 = " + std::to_string((-b + std::sqrt(discriminant)) / (2 * a)) + " и x2 = " + std::to_string((-b - std::sqrt(discriminant)) / (2 * a));
				}
				else {
					history[index].solution = "x1 = " + std::to_string(-b / (2 * a)) + " + " + std::to_string(std::sqrt(-discriminant) / (2 * a)) + "i и x2 = " + std::to_string(-b / (2 * a)) + " - " + std::to_string(std::sqrt(-discriminant) / (2 * a)) + "i";
				}
			}
			else {
				history[index].solution = "Уравнение не является квадратным.";
			}
			std::cout << "Решение обновлено.\n";
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
				solution = "x=" + std::to_string(-b / a);
				addSolution(equationType, solution, a, b, 0);
			}
		}
		else if (eqType == 'q') {
			equationType = "Квадратное";
			std::cout << "Введите коэффициенты квадратного уравнения (ax^2 + bx + c = 0): ";
			std::cin >> a >> b >> c;
			if (a != 0) {
				double discriminant = b * b - 4 * a * c;
				if (std::abs(discriminant) < 1e-10) {
					solution = "x = " + std::to_string(-b / (2 * a));
				}
				else if (discriminant > 0) {
					solution = "x1 = " + std::to_string((-b + std::sqrt(discriminant)) / (2 * a)) + " и x2 = " + std::to_string((-b - std::sqrt(discriminant)) / (2 * a));
				}
				else {
					solution = "x1 = " + std::to_string(-b / (2 * a)) + " + " + std::to_string(std::sqrt(-discriminant) / (2 * a)) + "i и x2 = " + std::to_string(-b / (2 * a)) + " - " + std::to_string(std::sqrt(-discriminant) / (2 * a)) + "i";
				}
			}
			else {
				equationType = "Квадратное";
				solution = "Уравнение не является квадратным.";
			}
			addSolution(equationType, solution, a, b, c);
		}
		else {
			std::cout << "Неверный тип уравнения.\n";
		}
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
			solver.printHistory();
			int index;
			std::cout << "Введите индекс решения для обновления: ";
			std::cin >> index;
			if (index >= 0 && index < solver.getHistorySize()) {
				solver.updateSolution(index);
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
