#include "pch.h"
#include "CppUnitTest.h"
#include "../desmos/desmos.cpp"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(AddSolution_WhenAddingLinearSolution_HistorySizeIncreases)
		{
			// Arrange
			EquationSolver obj;
			int initialHistorySize = obj.getHistorySize();

			// Act
			obj.addSolution("linear", "x=5", 1.0, 0.0, 5.0);

			// Assert
			Assert::AreEqual(initialHistorySize + 1, obj.getHistorySize());
		}

		TEST_METHOD(AddSolution_WhenAddingQuadraticSolution_HistorySizeIncreases)
		{
			// Arrange
			EquationSolver obj;
			int initialHistorySize = obj.getHistorySize();

			// Act
			obj.addSolution("quadratic", "x1=2, x2=3", 1.0, -5.0, 6.0);

			// Assert
			Assert::AreEqual(initialHistorySize + 1, obj.getHistorySize());
		}

		TEST_METHOD(AddSolution_WhenAddingMultipleSolutions_HistorySizeIncreasesCorrectly)
		{
			// Arrange
			EquationSolver obj;
			int initialHistorySize = obj.getHistorySize();

			// Act
			obj.addSolution("linear", "x=5", 1.0, 0.0, 5.0);
			obj.addSolution("quadratic", "x1=2, x2=3", 1.0, -5.0, 6.0);

			// Assert
			Assert::AreEqual(initialHistorySize + 2, obj.getHistorySize());
		}
	};

	TEST_CLASS(PrintHistoryTest)
	{
	public:
		TEST_METHOD(PrintHistory_WhenHistoryIsEmpty_ShouldPrintEmptyMessage)
		{
			// Arrange
			EquationSolver obj;
			std::stringstream buffer;
			auto original = std::cout.rdbuf(buffer.rdbuf());

			// Act
			obj.printHistory();

			// Assert
			Assert::AreEqual(std::string("История решений уравнений пуста.\n"), buffer.str());

			// Восстанавливаем оригинальный поток вывода
			std::cout.rdbuf(original);
		}

		TEST_METHOD(PrintHistory_WhenHistoryIsNotEmpty_ShouldPrintCorrectHistory)
		{
			// Arrange
			EquationSolver obj;
			obj.addSolution("linear", "x=5", 1.0, 0.0, 5.0);
			obj.addSolution("quadratic", "x1=2, x2=3", 1.0, -5.0, 6.0);
			std::stringstream buffer;
			auto original = std::cout.rdbuf(buffer.rdbuf());

			// Act
			obj.printHistory();

			// Assert
			Assert::AreEqual(std::string("\nИстория решений уравнений:\n[linear] x=5\n[quadratic] x1=2, x2=3\n"), buffer.str());

			// Восстанавливаем оригинальный поток вывода
			std::cout.rdbuf(original);
		}
	};

	TEST_CLASS(DeleteSolutionTest)
	{
	public:
		TEST_METHOD(DeleteSolution_WhenIndexIsInRange_ShouldDeleteSolutionAndPrintMessage)
		{
			// Arrange
			EquationSolver obj;
			obj.addSolution("linear", "x=5", 1.0, 0.0, 5.0);
			std::stringstream buffer;
			auto original = std::cout.rdbuf(buffer.rdbuf());

			// Act
			obj.deleteSolution(0);

			// Assert
			Assert::AreEqual(std::string("Решение удалено.\n"), buffer.str());
			Assert::AreEqual(0, obj.getHistorySize());

			// Восстанавливаем оригинальный поток вывода
			std::cout.rdbuf(original);
		}

		TEST_METHOD(DeleteSolution_WhenIndexIsOutOfRange_ShouldPrintErrorMessage)
		{
			// Arrange
			EquationSolver obj;
			std::stringstream buffer;
			auto original = std::cout.rdbuf(buffer.rdbuf());

			// Act
			obj.deleteSolution(0);

			// Assert
			Assert::AreEqual(std::string("Неверный индекс для удаления.\n"), buffer.str());

			// Восстанавливаем оригинальный поток вывода
			std::cout.rdbuf(original);
		}
	};

	TEST_CLASS(UpdateSolutionTest)
	{
	public:
		TEST_METHOD(UpdateSolution_WhenUpdatingToLinear_ShouldUpdateSolution)
		{
			// Arrange
			EquationSolver solver;
			solver.addSolution("quadratic", "x1=-1.0, x2=-4.0", 1.0, 5.0, 4.0);

			std::string input = "l\n2 2\n";
			std::istringstream inputStream(input);

			std::ostringstream outputStream;
			auto originalCin = std::cin.rdbuf(inputStream.rdbuf());
			auto originalCout = std::cout.rdbuf(outputStream.rdbuf());

			// Act
			solver.updateSolution(0);

			// Assert

			Assert::AreEqual(std::string("Линейное"), solver.getHistory()[0].equationType,false);
			Assert::AreEqual(std::string("x = -1.0\n"), solver.getHistory()[0].solution,false);

			// Cleanup
			std::cin.rdbuf(originalCin);
			std::cout.rdbuf(originalCout);
		}

		TEST_METHOD(UpdateSolution_WhenUpdatingToQuadratic_ShouldUpdateSolution)
		{
			// Arrange
			EquationSolver solver;
			solver.addSolution("Линейное", "x=2", 2.0, 2.0, 0.0);

			std::string input = "q\n1 5 4\n";
			std::istringstream inputStream(input);

			std::ostringstream outputStream;
			auto originalCin = std::cin.rdbuf(inputStream.rdbuf());
			auto originalCout = std::cout.rdbuf(outputStream.rdbuf());

			// Act
			solver.updateSolution(0);

			// Assert
			Assert::AreEqual(std::string("Квадратное"), solver.getHistory()[0].equationType,false);
			Assert::AreEqual(std::string("x1 = -1.000000 и x2 = -4.000000\n"), solver.getHistory()[0].solution,false);

			// Cleanup
			std::cin.rdbuf(originalCin);
			std::cout.rdbuf(originalCout);
		}
	};

	TEST_CLASS(GetHistorySizeTest)
	{
	public:
		TEST_METHOD(GetHistorySize_WhenHistoryIsEmpty_ShouldReturnZero)
		{
			// Arrange
			EquationSolver solver;

			// Act
			int size = solver.getHistorySize();

			// Assert
			Assert::AreEqual(0, size);
		}

		TEST_METHOD(GetHistorySize_WhenHistoryIsNotEmpty_ShouldReturnCorrectSize)
		{
			// Arrange
			EquationSolver solver;
			solver.addSolution("linear", "x=2", 2.0, 2.0, 0.0);
			solver.addSolution("quadratic", "x1=-1.0, x2=-4.0", 1.0, 5.0, 4.0);

			// Act
			int size = solver.getHistorySize();

			// Assert
			Assert::AreEqual(2, size);
		}
	};

	TEST_CLASS(CreateSolutionTest)
	{
	public:
		TEST_METHOD(CreateLinearSolution_ValidInput_ShouldAddSolution)
		{
			// Arrange
			EquationSolver solver;
			std::stringstream input("2 3\n");
			std::cin.rdbuf(input.rdbuf());
			char eqType = 'l';

			// Act
			solver.createSolution(eqType);

			// Assert
			Assert::AreEqual(1, solver.getHistorySize());
		}

		TEST_METHOD(CreateQuadraticSolution_ValidInput_ShouldAddSolution)
		{
			// Arrange
			EquationSolver solver;
			std::stringstream input("1 -5 6\n");
			std::cin.rdbuf(input.rdbuf());
			char eqType = 'q';

			// Act
			solver.createSolution(eqType);

			// Assert
			Assert::AreEqual(1, solver.getHistorySize());
		}

		TEST_METHOD(CreateSolution_InvalidInput_ShouldNotAddSolution)
		{
			// Arrange
			EquationSolver solver;
			std::stringstream input("a b\n");
			std::cin.rdbuf(input.rdbuf());
			char eqType = 'l';

			// Act
			solver.createSolution(eqType);

			// Assert
			Assert::AreEqual(0, solver.getHistorySize());
		}
	};

	TEST_CLASS(CreatePlotTest)
	{
	public:
		TEST_METHOD(PlotFileGeneratedSuccessfully)
		{
			// Arrange
			EquationSolver solver;
			int indexOfLin = 0;
			int indexOfQuad = 1;

			// Act
			solver.createPlot(indexOfLin, indexOfQuad);

			// Assert
			std::ifstream plotFile("gnuplot_commands.txt");
			Assert::IsTrue(plotFile.is_open());
			plotFile.close();
		}

		TEST_METHOD(PlotCommandsCorrect)
		{
			// Arrange
			EquationSolver solver;
			int indexOfLin = 0;
			int indexOfQuad = 1;

			// Act
			solver.createPlot(indexOfLin, indexOfQuad);

			// Assert
			std::ifstream plotFile("gnuplot_commands.txt");
			Assert::IsTrue(plotFile.is_open());

			std::string line;
			std::getline(plotFile, line); // Read the first line
			Assert::AreEqual(std::string("set xlabel 'x'"), line);

			std::getline(plotFile, line); // Read the second line
			Assert::AreEqual(std::string("set ylabel 'y'"), line);


			plotFile.close();
		}
	};

	TEST_CLASS(FindIntersectionsTest)
	{
	public:
		TEST_METHOD(FindIntersections_TwoIntersections)
		{
			// Arrange
			double a_linear = 2.0;
			double b_linear = 4.0;
			double a_quad = 1.0;
			double b_quad = 5.0;
			double c_quad = 4.0;

			// Act
			std::vector<Point> intersections = findIntersections(a_linear, b_linear, a_quad, b_quad, c_quad);

			// Assert
			Assert::AreEqual<size_t>(2u, intersections.size());
			Assert::AreEqual(0.0, intersections[0].x);
			Assert::AreEqual(4.0, intersections[0].y);
			Assert::AreEqual(-3.0, intersections[1].x);
			Assert::AreEqual(-2.0, intersections[1].y);
		}

		TEST_METHOD(FindIntersections_NoIntersections)
		{
			// Arrange
			double a_linear = -1.0;
			double b_linear = 3.0;
			double a_quad = 1.0;
			double b_quad = -3.0;
			double c_quad = 5.0;

			// Act
			std::vector<Point> intersections = findIntersections(a_linear, b_linear, a_quad, b_quad, c_quad);

			// Assert
			Assert::AreEqual<size_t>(0u, intersections.size());
		}
	};
}
