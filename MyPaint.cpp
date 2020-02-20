/*
	= 2019 =
	Made in 23.05 by De-R. & Pl.
	= 2020 =
*/

/*
	WHAT TO ADD:
		
	WHAT TO FIX:
		
*/

#include <windows.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
using namespace std;

const double PI = 3.14159;
HDC hDC;
LPARAM lParam;
HPEN passiveFigure_pen = CreatePen(PS_SOLID, 3, RGB(150, 75, 0)),
activeFigure_pen = CreatePen(PS_SOLID, 6, RGB(255, 128, 0)),
frame_pen = CreatePen(PS_SOLID, 3, RGB(0, 122, 204)),
default_pen = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
RECT rect;

/* Point */
class Point {
public:
	int x, y;
	double param; // положение точки на числовой окружности относительно центра фигуры
	bool isConnectToNext; // сообщает методу Figure.paint о необходимости проводить линию от данной точки к следующей
	Point() {
		this->x = 0;
		this->y = 0;
		this->param = 0;
		this->isConnectToNext = true;
	}
	Point(int x, int y, bool isConnectToNext = true) {
		this->x = x;
		this->y = y;
		this->param = 0;
		this->isConnectToNext = isConnectToNext;
	}
	Point(int x, int y, double param, bool isConnectToNext = true) {
		this->x = x;
		this->y = y;
		this->param = param;
		this->isConnectToNext = isConnectToNext;
	}
};

/* тип фигуры  */
enum class Pattern {
	EQUILATERAL_TRIANGLE,
	ROMB,
	SQUARE,
	CROSS,
	HEXAGON,
	CURVE_TRIANGLE,
	PENTAGON,
	LONG_TRIANGLE
};

/* направление */
enum class Direction {
	left,
	right,
	vertical,
	horisontal
};

/* фигура */
class Figure {
public:
	int x, y, r;
	vector<Point> points; // точки
	Pattern pattern;

	Figure() {
		this->x = this->y = this->r = 0;
		this->pattern = Pattern::SQUARE;
	}
	Figure(int x, int y, int r) {
		this->x = x;
		this->y = y;
		this->r = r;
		this->pattern = Pattern::SQUARE;
	}

	/* создает и добавляет точку в this.points */
	void createPoint(double param, bool isConnectToNext = true) {
		this->points.push_back(Point(this->x + this->r * cos(param), this->y - this->r * sin(param), param, isConnectToNext));
	}

	/* изменяет существующюю точку */
	void setPoint(int index, double param) {
		if (index < 0 || index >= this->points.size())
			return;
		this->points[index].x = this->x + this->r * cos(param);
		this->points[index].y = this->y - this->r * sin(param);
		this->points[index].param = param;
	}

	/* рисует фигуру */
	void paint() {
		for (int i = 0; i < this->points.size(); i++) {
			if (!this->points[i].isConnectToNext) {
				if (i != this->points.size() - 1) {
					MoveToEx(hDC, this->points[i + 1].x, this->points[i + 1].y, NULL);
				}
				continue;
			}
			if (i == 0) {
				MoveToEx(hDC, this->points[0].x, this->points[0].y, NULL);
			}
			if (i == this->points.size() - 1) {
				LineTo(hDC, this->points[0].x, this->points[0].y);
			}
			else {
				LineTo(hDC, this->points[i + 1].x, this->points[i + 1].y);
			}
		}
	}

	/* поворачивает фигуру */
	void rotate(Direction dir, double step) {
		if (dir == Direction::left) {
			for (int i = 0; i < this->points.size(); i++) {
				points[i].param += step;
			}
		}
		else if (dir == Direction::right) {
			for (int i = 0; i < this->points.size(); i++) {
				points[i].param -= step;
			}
		}
	}

	/* отзеркаливает фигуру */
	void reflect(Direction dir) {
		if (dir == Direction::vertical) {
			for (int i = 0; i < this->points.size(); i++) {
				this->points[i].param = -this->points[i].param;
			}
		}
		if (dir == Direction::horisontal) {
			for (int i = 0; i < this->points.size(); i++) {
				this->points[i].param = PI - this->points[i].param;
			}
		}
	}

	/* перестраивает точки фигуры по их param */
	void rebuild() {
		for (int i = 0; i < this->points.size(); i++) {
			this->setPoint(i, this->points[i].param);
		}
	}
};

vector<Figure> figures; // массив фигур
int acti = -1; // индекс активной фигуры
bool shift_flag = false; // флаг на нажатие клавиши shift

/* добавляет фигуру в массив */
void createFigure(const Pattern& pat) {
	figures.push_back(Figure());
	acti = figures.size() - 1;
	Figure& f = figures[acti];
	switch (pat) {
		case Pattern::EQUILATERAL_TRIANGLE:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(0);
			f.createPoint(PI);
			break;
		case Pattern::ROMB:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(0);
			f.createPoint(3 * PI / 2);
			f.createPoint(PI);
			break;
		case Pattern::SQUARE:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 4);
			f.createPoint(7 * PI / 4);
			f.createPoint(5 * PI / 4);
			f.createPoint(3 * PI / 4);
			break;
		case Pattern::CROSS:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(3 * PI / 2, false);
			f.createPoint(PI);
			f.createPoint(0, false);
			break;
		case Pattern::HEXAGON:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 3);
			f.createPoint(0);
			f.createPoint(5 * PI / 3);
			f.createPoint(4 * PI / 3);
			f.createPoint(PI);
			f.createPoint(2 * PI / 3);
			break;
		case Pattern::CURVE_TRIANGLE:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 3);
			f.createPoint(0);
			f.createPoint(PI);
			break;
		case Pattern::PENTAGON:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 3);
			f.createPoint(0);
			f.createPoint(3 * PI / 2);
			f.createPoint(PI);
			f.createPoint(2 * PI / 3);
			break;
		case Pattern::LONG_TRIANGLE:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(4 * PI / 3);
			f.createPoint(5 * PI / 3);
			break;
	}
	f.pattern = pat;
}

void paintFigures() {
	for (int i = 0; i < figures.size(); i++) {
		SelectObject(hDC, i == acti ? activeFigure_pen : passiveFigure_pen);
		figures[i].paint();
	}
}

long height(const RECT& r) {
	return r.bottom - r.top;
}

HWND  h_EditX, h_EditY, h_EditR, h_Button;
const int EDITS_OFFSET_X = 60;
const int ID_EDIT_FIGURE_INDEX = 1,
ID_EDIT_X = 2,
ID_EDIT_Y = 3,
ID_EDIT_R = 4,
ID_EDIT_TYPE = 5,
ID_BUTTON = 6;
const wstring STR_KEY_ASSIGMENTS = L"Create_figure=F1-F8, Move=LMB, Resize=Up/Down, Toggle=Left/Right, Rotate (left=Q, rigth=E), Reflect=R, Toggle_reflect=Shift, Delete=Delete";
const wstring REFLECT_MODE_CATPTION = L"Reflect mode: ";
wstring reflectMode;
wstring reflectModeFull;
const wstring PATTERN_TYPES_CAPTION = L"Type: ";
const vector<wstring> PATTERN_TYPES = {
	L"equilateral_triangle",
	L"romb",
	L"square",
	L"cross",
	L"hexagon",
	L"curve_triangle",
	L"pentagon",
	L"long_triangle"
};
const wstring INDEX_CAPTION = L"Index: ";

int KEY_ASSIGMENTS_pos_y; // позиция линии "key assigments" по у
int leftPadding = 10; // левый отступ в окне
int bottomPadding = 20; // нижний отступ в окне
int leftFrame = leftPadding + 180; // размер левой рамки в окне
int bottomFrame = bottomPadding + 52; // размер нижней рамки в окне

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT points;
	RECT rect;
	Figure& f = figures.size() ? figures[acti] : *&Figure();

	switch (Message) {

		case WM_DESTROY: {
			PostQuitMessage(0); 
			break;
		}
		case WM_PAINT: {
			hDC = BeginPaint(hwnd, &points);
			GetWindowRect(hwnd, &rect);

			SelectObject(hDC, frame_pen);
			MoveToEx(hDC, leftFrame, 0, NULL);
			LineTo(hDC, leftFrame, height(rect) - bottomFrame);
			MoveToEx(hDC, 0, height(rect) - bottomFrame, NULL);
			LineTo(hDC, rect.right, height(rect) - bottomFrame);
			SelectObject(hDC, default_pen);

			if (figures.size()) {
				f.rebuild();
				/*
					корректировка положения фигуры при выходе за допустимые границы
				*/
				bool isLeftFrameCrossed = false, isBottomFrameCrossed = false;
				int maxLeftFrameCrossedDist = 0, maxBottomFrameCrossedDist = 0;
				for (int i = 0; i < f.points.size(); i++) {
					if (f.points[i].x < leftFrame) {
						if (maxLeftFrameCrossedDist < leftFrame - f.points[i].x) maxLeftFrameCrossedDist = leftFrame - f.points[i].x;
						if (!isLeftFrameCrossed) isLeftFrameCrossed = true;
					}
					if (f.points[i].y > height(rect) - bottomFrame) {
						if (maxBottomFrameCrossedDist < f.points[i].y - (height(rect) - bottomFrame)) maxBottomFrameCrossedDist = f.points[i].y - (height(rect) - bottomFrame);
						if (!isBottomFrameCrossed) isBottomFrameCrossed = true;
					}
				}
				if (isLeftFrameCrossed) {
					f.x += maxLeftFrameCrossedDist;
				}
				if (isBottomFrameCrossed) {
					f.y -= maxBottomFrameCrossedDist;
				}
				f.rebuild();
				paintFigures();
			}

			reflectMode = shift_flag ? L"H" : L"V";
			reflectModeFull = REFLECT_MODE_CATPTION + reflectMode;

			TextOut(hDC, leftPadding, 10, (INDEX_CAPTION + to_wstring(acti)).c_str(), (INDEX_CAPTION + to_wstring(acti)).size());
			TextOut(hDC, leftPadding, 40, L"X: ", 3);
			TextOut(hDC, leftPadding, 70, L"Y: ", 3);
			TextOut(hDC, leftPadding, 100, L"R: ", 3);
			TextOut(hDC, leftPadding, 160,
					(PATTERN_TYPES_CAPTION + (figures.size() ? PATTERN_TYPES[(int)f.pattern] : L"")).c_str(),
					(PATTERN_TYPES_CAPTION + (figures.size() ? PATTERN_TYPES[(int)f.pattern] : L"")).size()
			);
			TextOut(hDC, leftPadding, KEY_ASSIGMENTS_pos_y, STR_KEY_ASSIGMENTS.c_str(), STR_KEY_ASSIGMENTS.size());
			TextOut(hDC, leftPadding, 200, (reflectModeFull).c_str(), reflectModeFull.size());

			if (figures.size()) {
				SetDlgItemInt(hwnd, ID_EDIT_FIGURE_INDEX, acti, true);
				SetDlgItemInt(hwnd, ID_EDIT_X, f.x, true);
				SetDlgItemInt(hwnd, ID_EDIT_Y, f.y, true);
				SetDlgItemInt(hwnd, ID_EDIT_R, f.r, true);
				SetDlgItemInt(hwnd, ID_EDIT_TYPE, (UINT)f.pattern, true);
			}
			SetFocus(hwnd);
			EndPaint(hwnd, &points);
			break;
		}

		// при изменении размера окна
		case WM_SIZE:
			GetClientRect(hwnd, &rect);
			KEY_ASSIGMENTS_pos_y = rect.bottom - rect.top - bottomPadding;
			InvalidateRect(hwnd, &rect, -1);
			break;

		case WM_LBUTTONDOWN:
			if (figures.size()) {
				f.x = LOWORD(lParam);
				f.y = HIWORD(lParam);
			}

			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, -1);
			UpdateWindow(hwnd);
			break;

		/* Child windows. Information about active figure */
		case WM_COMMAND: {
			switch (LOWORD(wParam)) {
				case ID_BUTTON:
					if (GetDlgItemInt(hwnd, ID_EDIT_FIGURE_INDEX, NULL, true) <= figures.size() - 1) {
						acti = GetDlgItemInt(hwnd, ID_EDIT_FIGURE_INDEX, NULL, true);
					}
					if (GetDlgItemInt(hwnd, ID_EDIT_R, NULL, true) < 20) {
						f.r = 20;
					}
					else { f.r = GetDlgItemInt(hwnd, ID_EDIT_R, NULL, true); }
					f.x = GetDlgItemInt(hwnd, ID_EDIT_X, NULL, true);
					f.y = GetDlgItemInt(hwnd, ID_EDIT_Y, NULL, true);
					f.r = GetDlgItemInt(hwnd, ID_EDIT_R, NULL, true);

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);
					break;
			}
			break;
		}
		case WM_KEYDOWN: {
			switch (wParam) {
				case VK_SHIFT: shift_flag = !shift_flag;
					break;

				case VK_LEFT:
					if (acti > 0) acti--;
					else acti = figures.size() - 1;
					break;

				case VK_RIGHT:
					if (acti < figures.size() - 1) acti++;
					else acti = 0;
					break;

				case VK_UP: f.r += 10;
					break;

				case VK_DOWN:
					if (f.r > 20) f.r -= 10;
					break;

				case VK_F1: createFigure(Pattern::EQUILATERAL_TRIANGLE);
					break;

				case VK_F2: createFigure(Pattern::ROMB);
					break;

				case VK_F3: createFigure(Pattern::SQUARE);
					break;

				case VK_F4: createFigure(Pattern::CROSS);
					break;

				case VK_F5: createFigure(Pattern::HEXAGON);
					break;

				case VK_F6: createFigure(Pattern::CURVE_TRIANGLE);
					break;

				case VK_F7: createFigure(Pattern::PENTAGON);
					break;

				case VK_F8: createFigure(Pattern::LONG_TRIANGLE);
					break;

				/* 'R' key */
				case 0x52:
					if (shift_flag) f.reflect(Direction::vertical);
					else f.reflect(Direction::horisontal);
					break;

				/* 
					удаление фигуры
				*/
				case VK_DELETE:
					if (figures.size()) {
						figures.erase(figures.begin() + acti);
						if (!figures.size()) {
							acti = -1;
							SetDlgItemInt(hwnd, ID_EDIT_FIGURE_INDEX, acti, true);
							SetDlgItemInt(hwnd, ID_EDIT_X, 0, true);
							SetDlgItemInt(hwnd, ID_EDIT_Y, 0, true);
							SetDlgItemInt(hwnd, ID_EDIT_R, 0, true);
							SetDlgItemInt(hwnd, ID_EDIT_TYPE, 0, true);
						}
						if (figures.size() == 1) acti = 0;
						else {
							if (acti == 0) acti = figures.size() - 1;
							else acti--;
						}
					}
					break;

				/* 'Q' */
				case 0x51:
					if (figures.size()) {
						f.rotate(Direction::left, PI / 32);
					}
					break;

				/* 'E' */
				case 0x45:
					if (figures.size()) {
						f.rotate(Direction::right, PI / 32);
					}
					break;
			}
			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, -1);
			UpdateWindow(hwnd);
			break;
		}
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG msg; /* A temporary location for all messages */

	/* zero out the struct and set the stuff we want to modify */
	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; /* This is where we will send messages to */
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	//wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hbrBackground = CreateSolidBrush(RGB(30, 30, 30));
	wc.lpszClassName = L"WindowClass";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); /* Load a standard icon */
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION); /* use the name "A" to use the project icon */

	if (!RegisterClassEx(&wc)) {
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"WindowClass", L"My Paint", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
						  0,  /*x*/ 0,  /*y*/
						  1280, /* width */ 720, /* height */
						  NULL, NULL, hInstance, NULL
	);

	GetClientRect(hwnd, &rect);
	KEY_ASSIGMENTS_pos_y = rect.bottom - rect.top - bottomPadding;

	h_EditX = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD, EDITS_OFFSET_X, 40, 50, 20, hwnd, (HMENU)ID_EDIT_X, hInstance, NULL);
	h_EditY = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD, EDITS_OFFSET_X, 70, 50, 20, hwnd, (HMENU)ID_EDIT_Y, hInstance, NULL);
	h_EditR = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD, EDITS_OFFSET_X, 100, 50, 20, hwnd, (HMENU)ID_EDIT_R, hInstance, NULL);
	h_Button = CreateWindow(L"BUTTON", L"Calculate", WS_BORDER | WS_CHILD, leftPadding, 130, 100, 20, hwnd, (HMENU)ID_BUTTON, hInstance, NULL);

	ShowWindow(h_EditX, nCmdShow);
	ShowWindow(h_EditY, nCmdShow);
	ShowWindow(h_EditR, nCmdShow);
	ShowWindow(h_Button, nCmdShow);

	if (hwnd == NULL) {
		MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while (GetMessage(&msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&msg); /* Translate key codes to chars if present */
		DispatchMessage(&msg); /* Send it to WndProc */
	}
	return msg.wParam;
}
