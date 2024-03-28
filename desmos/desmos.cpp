#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <stdio.h>


struct Point {
	double x;
	double y;
};

std::vector<Point> findIntersections(double a_linear, double b_linear, double a_quad, double b_quad, double c_quad) {
	std::vector<Point> intersections;

	// Приравниваем квадратное и линейное уравнения
	double a = a_quad;
	double b = b_quad - a_linear;
	double c = c_quad - b_linear;

	// Решаем квадратное уравнение, чтобы найти точки пересечения
	double discriminant = b * b - 4 * a * c;

	if (std::abs(discriminant) < 1e-10) {
		// Если дискриминант близок к нулю, то есть один корень
		double x_intersection = -b / (2 * a);
		Point intersection;
		intersection.x = x_intersection;
		intersection.y = a_linear * x_intersection + b_linear;
		intersections.push_back(intersection);
	}
	else if (discriminant > 0) {
		// Если дискриминант положителен, есть два корня
		double x1 = (-b + std::sqrt(discriminant)) / (2 * a);
		double x2 = (-b - std::sqrt(discriminant)) / (2 * a);

		// Добавляем оба корня в качестве точек пересечения
		Point intersection1, intersection2;
		intersection1.x = x1;
		intersection1.y = a_linear * x1 + b_linear;
		intersection2.x = x2;
		intersection2.y = a_linear * x2 + b_linear;
		intersections.push_back(intersection1);
		intersections.push_back(intersection2);
	}
	else {
		// Если дискриминант отрицателен, то нет действительных корней
		std::cout << "Нет действительных корней\n";
	}

	return intersections;
}


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
			if (!(std::cin >> a >> b)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Некорректный ввод. Введите числа." << std::endl;
				return;
			}
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
			if (!(std::cin >> a >> b >> c)) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Некорректный ввод. Введите числа." << std::endl;
				return;
			}
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

	void createPlot(int indexOfLin, int indexOfQuad) {
		std::ofstream gnuplotCommands("gnuplot_commands.txt");
		if (!gnuplotCommands.is_open()) {
			std::cerr << "Ошибка: Не удалось создать файл с командами для Gnuplot.\n";
			return;
		}

		gnuplotCommands << "set xlabel 'x'\n";
		gnuplotCommands << "set ylabel 'y'\n";
		gnuplotCommands << "set title 'Graphic of intersection'\n";
		gnuplotCommands << "set xrange [-20:20]\n";
		gnuplotCommands << "set yrange [-20:20]\n";

		gnuplotCommands << "set style line 11 lc rgb '#808080' lt 1\n";
		gnuplotCommands << "set border 3 back ls 11\n";
		gnuplotCommands << "set tics nomirror out scale 0.75\n";
		gnuplotCommands << "set style line 12 lc rgb '#808080' lt 0 lw 1\n";
		gnuplotCommands << "set grid back ls 12\n";

		// Находим и записываем все точки пересечения в файл
		std::vector<Point> intersections = findIntersections(history[indexOfLin].a, history[indexOfLin].b, history[indexOfQuad].a, history[indexOfQuad].b, history[indexOfQuad].c);
		if (intersections.empty()) {
			std::cout << "Нет точек пересечения\n";
		}
		else {
			// Установка меток для каждой точки пересечения
			for (const auto& intersection : intersections) {
				gnuplotCommands << "set label 'Intersection' at " << intersection.x << ", " << intersection.y << " point pointtype 7 lc rgb 'red'\n";
				gnuplotCommands << "print 'Dot of intersection : (" << intersection.x << ", " << intersection.y << ")'\n";
			}
		}
		std::cout << "Коэффициенты квадратного уравнения:\n";
		std::cout << "a = " << history[indexOfQuad].a << '\n';
		std::cout << "b = " << history[indexOfQuad].b << '\n';
		std::cout << "c = " << history[indexOfQuad].c << '\n';
		// Построение графиков после настройки точек пересечения
		gnuplotCommands << "plot " << history[indexOfLin].a << " * x + " << history[indexOfLin].b << " title 'y=" << history[indexOfLin].a << "x + " << history[indexOfLin].b << "'\n";
		gnuplotCommands << "replot " << history[indexOfQuad].a << " * x**2 + " << history[indexOfQuad].b << " * x + " << history[indexOfQuad].c << " title 'y=" << history[indexOfQuad].a << "x^2 + " << history[indexOfQuad].b << "x + " << history[indexOfQuad].c << "'\n";



		// Закрыть файл с командами Gnuplot
		gnuplotCommands.close();

		// Выполнение команды для запуска Gnuplot с файлом команд
		std::string gnuplotCmd = "gnuplot -persist gnuplot_commands.txt";
		std::system(gnuplotCmd.c_str());
	}



};

int main() {
	system("chcp 65001");
	char equationType;
	EquationSolver solver;
	do {
		std::cout << "Выберите операцию (c - создать, r - прочитать, u - обновить, d - удалить, p - рисовать график , e - выход): ";
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
		case 'p':
			int linIndex;
			int quadIndex;
			std::cout << "Введите индекс линейного уравнения: ";
			std::cin >> linIndex;
			std::cout << "Введите индекс квадратного уравнения: ";
			std::cin >> quadIndex;
			solver.createPlot(linIndex,quadIndex);
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
