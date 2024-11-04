#include <iostream>
#include <fstream>



// interface
template<typename T, typename Predicat>
void validation(T& x, Predicat condition, const char* message);
bool correct_size(int x);
bool correct_choice(int x);
bool correct_source(int x);
int check_file(std::ifstream& file);
int** memory_allocation(int rows, int cols);
void free_memory(int** matrix, int cols);
int** fill_matrix_console(int rows, int cols);
int** fill_matrix_file(std::ifstream& file, int rows, int cols);
void print(std::ostream& ostream, int** matrix, int rows, int cols);

// task
void find_min_max_col(int** matrix, int rows, int cols, int& col_min, int& col_max);
void delete_col(int** matrix, int& size, int index);
int* make_column(int** matrix, int rows, int cols);
void change_matrix(int** matrix, int rows, int& cols);

// user interface
void main_menu(int& choice);
void source_menu(int& source);

// debug
void print_arr(int* arr, int size);



int main()
{
    int choice{}, source{};
    do
    {
        source_menu(source);
        switch (source)
        {
        case 1:
        {
            std::ifstream input_file("data.txt");
            switch (check_file(input_file))
            {
            case -1:
                std::cout << "'data.txt' not found!\n";
                input_file.close();
                choice = 4;
                break;
            case 0:
                std::cout << "File is empty! Check the 'data.txt' file.\n";
                input_file.close();
                choice = 4;
                break;
            default:
                int rows{}, cols{};
                input_file >> rows >> cols;
                if (correct_size(rows) && correct_size(cols))
                {
                    int** matrix = memory_allocation(rows, cols);
                    matrix = fill_matrix_file(input_file, rows, cols);
                    std::ofstream output_file("output.txt");
                    if (matrix)
                    {
                        do
                        {
                            main_menu(choice);
                            switch (choice)
                            {
                            case 1:
                                change_matrix(matrix, rows, cols);
                                std::cout << "Matrix changed\n";
                                break;
                            case 2:
                                std::cout << '\n';
                                print(std::cout, matrix, rows, cols);
                                break;
                            case 3:
                                print(output_file, matrix, rows, cols);
                                std::cout << "Printed to 'output.txt'\n";
                                break;
                            case 4:
                                break;
                            default:
                                choice = 4;
                                source = 3;
                            }
                        } while (choice != 4);
                        free_memory(matrix, cols);
                    }
                    else
                        source = 3;
                    output_file.close();
                    break;
                }
                else
                {
                    std::cout << "Inccorrect size of matrix! Check the 'data.txt' file.\n";
                    choice = 4;
                }
                break;
            }
            input_file.close();
            break;
        }
        case 2:
            int rows{}, cols{};
            validation(rows, *correct_size, "Enter a number of rows");
            validation(cols, *correct_size, "Enter a number of columns");
            int** matrix = memory_allocation(rows, cols);
            matrix = fill_matrix_console(rows, cols);
            std::ofstream output_file("output.txt");
            do
            {
                main_menu(choice);
                switch (choice)
                {
                case 1:
                    change_matrix(matrix, rows, cols);
                    std::cout << "Matrix changed\n";
                    break;
                case 2:
                    std::cout << '\n';
                    print(std::cout, matrix, rows, cols);
                    break;
                case 3:
                    print(output_file, matrix, rows, cols);
                    std::cout << "Printed to 'output.txt'\n";
                    break;
                case 4:
                    break;
                default:
                    choice = 4;
                    source = 3;
                }
            } while (choice != 4);
            free_memory(matrix, cols);
            output_file.close();
            break;
        }
    } while (source != 3);
}



// interface
template<typename T, typename Predicat>
void validation(T& x, Predicat condition, const char* message)
{
    std::cout << message << "\n>>> ";
    while (!(std::cin >> x && condition(x)))
    {
        std::cout << "Input error!" << '\n';
        std::cin.clear();
        std::cin.ignore(std::cin.rdbuf()->in_avail());
        std::cout << message << "\n>>> ";
    }
}

bool correct_size(int x)        // �������-�������� ������ ������,
{                               // ������ ��� �� �� �����-�� �������� ��������!!!
    return x > 0 && x < 100;
}

bool correct_choice(int x)
{
    return x >= 1 && x <= 5;
}

bool correct_source(int x)
{
    return x >= 1 && x <= 3;
}

int check_file(std::ifstream& file)
{
    int res = 1;
    if (!file)
    {
        res = -1; // ���� �� ������!
    }
    else
        if (file.peek() == EOF)
            res = 0; // ���� ������!
    return res;
}

int** memory_allocation(int rows, int cols)
{
    int** matrix = new int* [cols];
    for (int i = 0; i < cols; ++i)
        matrix[i] = new int[rows];
    return matrix;
}

void free_memory(int** matrix, int cols)
{
    for (int i = 0; i < cols; ++i)
        delete[] matrix[i];
    delete[] matrix;
}

int** fill_matrix_console(int rows, int cols)
{
    int** matrix = memory_allocation(rows, cols);
    for (int i{}; i < rows; i++)
    {
        for (int j{}; j < cols; j++)
        {
            bool correct_input = false;
            while (!correct_input)
            {
                if (!(std::cin >> matrix[j][i]))
                {
                    std::cin.clear();
                    std::cin.ignore(std::cin.rdbuf()->in_avail());
                    std::cout << "Invalid input. Please try again.\n";
                }
                else
                {
                    correct_input = true;
                }
            }
        }
    }
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    return matrix;
}

int** fill_matrix_file(std::ifstream& file, int rows, int cols)
{
    int** matrix = memory_allocation(rows, cols);
    for (int i{}; i < rows; ++i)
    {
        for (int j{}; j < cols; ++j)
        {
            if (!(file >> matrix[j][i]))
            {
                std::cout << "invalid file data\n";
                return nullptr;
            }
        }
    }
    return matrix;
}

void print(std::ostream& ostream, int** matrix, int rows, int cols)
{
    for (int i{}; i < rows; ++i)
    {
        for (int j{}; j < cols; ++j)
            ostream << matrix[j][i] << ' ';
        ostream << '\n';
    }
}

void find_min_max_col(int** matrix, int rows, int cols, int& col_min, int& col_max)
{
    int min = matrix[0][0];
    int max = min;
    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j)
        {
            if (matrix[i][j] < min)
            {
                min = matrix[i][j];
                col_min = i;
            }
            if (matrix[i][j] > max)
            {
                max = matrix[i][j];
                col_max = i;
            }
        }
}

void delete_col(int** matrix, int& size, int index)
{
    delete[] matrix[index];
    matrix[index] = nullptr;
    for (int i = index; i < size - 1; ++i)
        matrix[i] = matrix[i + 1];
    size--;
}

int* make_column(int** matrix, int rows, int cols)
{
    int* result = new int[rows];
    for (int i{}; i < rows; ++i)
        result[i] = 1;

    for (int i{}; i < rows; ++i)
        for (int j{}; j < cols; ++j)
            result[i] *= matrix[j][i];
    return result;
}

void change_matrix(int** matrix, int rows, int& cols)
{
    int col1{}, col2{};
    find_min_max_col(matrix, rows, cols, col1, col2);
    delete_col(matrix, cols, col1);
    delete_col(matrix, cols, col2 - 1);
    
    int* multi_column = make_column(matrix, rows, cols);
    cols++;
    matrix[cols - 1] = new int[rows];
    for (int i{}; i < rows; ++i)
        matrix[cols - 1][i] = multi_column[i];
}

void main_menu(int& choice)
{
    std::cout << "\nMENU\n" <<
        "1. Change matrix\n" <<
        "2. Print matrix to console\n" <<
        "3. Print matrix to file\n" <<
        "4. Change source\n" <<
        "5. Exit\n";
    validation(choice, *correct_choice, "");
}

void source_menu(int& source)
{
    std::cout << "\nSelect a source\n" <<
        "1. Read from file\n" <<
        "2. Read from console\n" << 
        "3. Exit\n";
    validation(source, *correct_source, "");
}

// debug
void print_arr(int* arr, int size)
{
    for (int i{}; i < size; ++i)
        std::cout << arr[i] << ' ';
    std::cout << '\n';
}
