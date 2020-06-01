#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>

using namespace std;

typedef pair<int, int> point; // ��� ������ ����� �� �����
const char wallChar =  'Z'; // ������ ������������ �����
const char emptyChar = ' '; // ������ ������������ ������
const char moveChar = '*'; // ������ ������������ ����
// ��� ��������� ��������, ��� ���������
vector<point> moves = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

int main(int argc, char *argv[]) {
    setlocale (0, "");

	string filename;
	cout<<"������� ��� ����� (�� ��������� ��� labirint.txt):";
	cin>>filename;

	vector<string> labyrinth; // ����� ����� ������� ��� ��������
	int width, height;	// ��������� ������� ���������
	ifstream labFile;
	labFile.open(filename, ifstream::in);
	if (labFile.is_open()) {
		string temp;
		labFile >> temp; // ������� ������ ������ ���������
		width = temp.size();
		cout << "������ ��������� = " << width << endl;
		do {
			for (char &c: temp) {
				switch (c) {
					case '0':
					case '1':
					case '2':
					case '3':
						break; // ��� � �������
					default: { // �� ��������� ������������ ������
						cout << "�������� ������� � ������������ �������, ���������� ���������� ������."
								  << endl;
						return 0;
					}
				}

			}
			if (temp.size() != width) {
				cout << "������ � ��������� ����� ������ �����, ������������ ������." << endl;
				return 0;
			}
			labyrinth.push_back(temp); // � � ����� ���� ����� ����� ���������� ��� ��������� ������
		} while (labFile >> temp); // ����� ��������� ���������� ������ ���������
		height = labyrinth.size();
		cout << "������ ��������� = " << height << endl;
	} else { // ���� �� ���������� ����� � ���������� �� ������ �� ���������
		cout << "������ ����� �� ����������." << endl;
		return 0;
	}

	// ������ ����� ������� ���������� � ��� ������������/�� ������������ �������
	vector<vector<bool>> used(height, vector<bool>(width));
	// ������ ����� ������� ����������� ���������� ��� ��������� ��������
	vector<vector<int>> dist(height, vector<int>(width));
	// ������ ����� ������� ��������� ������, �� ���� ������ �� ������, ����� ���������� ����
	// �� ���� ���������� ���������� � ��������� ��������
	vector<vector<point>> parent(height, vector<point>(width));

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			parent[i][j] = {-1, -1}; // ���������� ��� ��������
			dist[i][j] = INT_MAX; // ����������� ���������� �� �������, �� ���� ��������� �� ��� ������
			used[i][j] = 0; // �� ������������ �� ���� �������
		}
	}

	pair<int, int> startVert; // ��������� �������
	pair<int, int> endVert; // �������� �������
	// ��������� �� ���� �������� � ������ ������� �� ������� �� ��������
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (labyrinth[i][j] == '2') { // ����� �� ������ �� ��������
				startVert = {i, j};
				break;
			}
		}
	}
	// ��������� �� ���� �������� � ������ ������� � ������� �� �����������
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (labyrinth[i][j] == '3') { // ����� �� ������ �� ��������
				endVert = {i, j};
				break;
			}
		}
	}
	cout << "��������� ������� - " << startVert.first << "," << startVert.second << endl;
	cout << "�������� ������� - " << endVert.first << "," << endVert.second << endl;
	dist[startVert.first][startVert.second] = 0; // ���������� �� ��������� ������� ����� ����
	cout << "��������� �������� ��������" << endl;

	// �������� ��������
	// �������������� ������� ���, ������� ����� ������
	for (int count = 0; count < width * height; count++) {
		point curItem;
		int minD = INT_MAX;
		// ������ ������� � ����������� ����������� �� ���� ����
		// �� ������ ���� ��� ������ ����� ��������� �������
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				if (!used[i][j] and dist[i][j] < minD) {
					curItem = {i, j}; // ���� �� ��� �� ������������ � ���������� ��� ��� ������ ���������� �����, �� ������� ��
					minD = dist[i][j];
				}
			}
		}
		used[curItem.first][curItem.second] = true; // �� ������������ ��������� ������� �� ���� ����
		for (auto &move: moves) {
			point next = {curItem.first + move.first, curItem.second + move.second};
			// ���� �� ������� �� ������� ��������� ���� ���� ������ � �����, �� ���������� ���� ���
			if (next.first < 0 || next.second < 0 || next.first >= height || next.second >= width ||
				labyrinth[next.first][next.second] == '1')
				continue;
			// ���� �� ������� ��� �� ����� ������� �� ���� � ���������� ����������, �� �������
			if (dist[curItem.first][curItem.second] + 1 < dist[next.first][next.second]) {
				dist[next.first][next.second] = dist[curItem.first][curItem.second] + 1;
				parent[next.first][next.second] = curItem;
			}
		}
	}

	// ����� �� �������� ������� ������
	if (dist[endVert.first][endVert.second] == INT_MAX) {
		cout << "���������� ��������� �� �������� �������, ���������� ���������." << endl;
		return 0;
	}

	// �������� �������� ����� ��������� ���� � �������
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

	// ��������� �� ��������� ������ �� ��������� ������� �� ����� ������
	while (curVert != startVert) {
		resultMap[curVert.first + 1][curVert.second + 1] = moveChar;
		curVert = parent[curVert.first][curVert.second];
	}
	resultMap[curVert.first + 1][curVert.second + 1] = moveChar;

	// ������� �������� �����
	cout << "����� �������� ��� ������������ ����:" << endl;
	for (string &cur: resultMap) {
		cout << cur << endl;
	}

	return 0;
}
