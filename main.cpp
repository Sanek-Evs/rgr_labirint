#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

typedef pair<int, int> point; // тип данных точка на карте
const char wallChar =  'Z'; // символ обозначающий стену
const char emptyChar = ' '; // символ обозначающий проход
const char moveChar = '*'; // символ обозначающий путь
// все возможные движения, без диагонали
vector<point> moves = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

int main(int argc, char *argv[]) {
    setlocale (0, "");

	string filename;
	cout<<"Введите имя файла (по умолчанию это labirint.txt):";
	cin>>filename;

	vector<string> labyrinth; // здесь будем хранить сам лабиринт
	int width, height;	// сохраняем размеры лабиринта
	ifstream labFile;
	labFile.open(filename, ifstream::in);
	if (labFile.is_open()) {
		string temp;
		labFile >> temp; // считаем первую строку лабиринта
		width = temp.size();
		cout << "Ширина лабиринта = " << width << endl;
		do {
			for (char &c: temp) {
				switch (c) {
					case '0':
					case '1':
					case '2':
					case '3':
						break; // все в порядке
					default: { // мы встретили неправильный символ
						cout << "Лабиринт записан в некорректном формате, встретился непонятный символ."
								  << endl;
						return 0;
					}
				}

			}
			if (temp.size() != width) {
				cout << "Строки в лабиринте имеют разную длину, некорректный формат." << endl;
				return 0;
			}
			labyrinth.push_back(temp); // а в самом теле цикла будем записывать уже считанную строку
		} while (labFile >> temp); // будем считывать оставшиеся строки лабиринта
		height = labyrinth.size();
		cout << "Высота лабиринта = " << height << endl;
	} else { // если не существует файла с лабиринтом то выйдем из программы
		cout << "Такого файла не существует." << endl;
		return 0;
	}

	// вектор будет хранить информацию о том использована/не использована вершина
	vector<vector<bool>> used(height, vector<bool>(width));
	// вектор будет хранить минимальные расстояния для алгоритма дейкстры
	vector<vector<int>> dist(height, vector<int>(width));
	// вектор будет хранить родителей вершин, то есть откуда мы пришли, будем записывать сюда
	// по ходу обновления расстояний в алгоритме дейкстры
	vector<vector<point>> parent(height, vector<point>(width));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			parent[i][j] = {-1, -1}; // изначально нет родителя
			dist[i][j] = INT_MAX; // бесконечное расстояние до вершины, то есть добраться до нее нельзя
			used[i][j] = 0; // не использована ни одна вершина
		}
	}

	pair<int, int> startVert; // начальная вершина
	pair<int, int> endVert; // конечная вершина
	// пройдемся по всем вершинам и найдем вершину из которой мы начинаем
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (labyrinth[i][j] == '2') { // нашли то откуда мы начинаем
				startVert = {i, j};
				break;
			}
		}
	}
	// пройдемся по всем вершинам и найдем вершину в которой мы заканчиваем
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (labyrinth[i][j] == '3') { // нашли то откуда мы начинаем
				endVert = {i, j};
				break;
			}
		}
	}
	cout << "Начальная вершина - " << startVert.first << "," << startVert.second << endl;
	cout << "Конечная вершина - " << endVert.first << "," << endVert.second << endl;
	dist[startVert.first][startVert.second] = 0; // расстояния до начальной вершини равно нулю
	cout << "Запускаем алгоритм дейкстры" << endl;

	// алгоритм дейкстры
	// проитерируемся столько раз, сколько всего вершин
	for (int count = 0; count < width * height; count++) {
		point curItem;
		int minD = INT_MAX;
		// найдем вершину с минимальным расстоянием на этом шаге
		// на первом шаге это всегда будет начальная вершина
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (!used[i][j] and dist[i][j] < minD) {
					curItem = {i, j}; // если мы еще не использовали и расстояние для нее меньше найденного ранее, то возьмем ее
					minD = dist[i][j];
				}
			}
		}
		used[curItem.first][curItem.second] = true; // мы использовали выбранную вершину на этом шаге
		for (auto &move: moves) {
			point next = {curItem.first + move.first, curItem.second + move.second};
			// если мы выходим за границы лабиринта либо если входим в стену, то пропустить этот ход
			if (next.first < 0 || next.second < 0 || next.first >= height || next.second >= width ||
				labyrinth[next.first][next.second] == '1')
				continue;
			// если мы сделаем ход от нашей вершины до этой и расстояние уменьшится, то обновим
			if (dist[curItem.first][curItem.second] + 1 < dist[next.first][next.second]) {
				dist[next.first][next.second] = dist[curItem.first][curItem.second] + 1;
				parent[next.first][next.second] = curItem;
			}
		}
	}

	// дойти до конечной вершины нельзя
	if (dist[endVert.first][endVert.second] == INT_MAX) {
		cout << "Невозможно добраться до конечной вершины, завершение программы." << endl;
		return 0;
	}

	// заполним итоговую карту символами стен и пустоты
	vector<string> resultMap(height + 2, string(width + 2, 0));
	for (int i = 0; i < height + 2; i++) {
		for (int j = 0; j < width + 2; j++) {
			if (i > 0 && i <= height && j > 0 && j <= width)
				resultMap[i][j] = (labyrinth[i - 1][j - 1] == '1' ? wallChar : emptyChar);
			else
				resultMap[i][j] = wallChar;
		}
	}

	auto curVert = endVert;

	// пройдемся по родителям вершин от последней вершины до самой первой
	while (curVert != startVert) {
		resultMap[curVert.first + 1][curVert.second + 1] = moveChar;
		curVert = parent[curVert.first][curVert.second];
	}
	resultMap[curVert.first + 1][curVert.second + 1] = moveChar;

	// выведем конечную карту
	cout << "Карта движения для оптимального пути:" << endl;
	for (string &cur: resultMap) {
		cout << cur << endl;
	}

	return 0;
}
