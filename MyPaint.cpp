/*
	Made in 23.05.2019 by De-R. & Pl.
	= 2020 =
	Edited in 02.02 by Le
		- finished key assigment (bottom) line
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
HPEN penPassiveFigure = CreatePen(PS_SOLID, 3, RGB(150, 75, 0)),
penActiveFigure = CreatePen(PS_SOLID, 6, RGB(255, 128, 0)),
penFrame = CreatePen(PS_SOLID, 3, RGB(0, 122, 204)),
penDefault = CreatePen(PS_SOLID, 6, RGB(0, 0, 0));
RECT rect;

/* òî÷êà - âåðøèíà ôèãóðû */
class Point {
public:
	int x, y;
	double param; // çíà÷åíèå òî÷êè íà ÷èñëîâîé îêðóæíîñòè (çà öåíòð âçÿò öåíòð ôèãóðû)
	bool isConnectToNext; // ïðè âûçîâå Figure.paint ñîåäèíÿòü òåêóùþþ òî÷êó ëèíèåé ñî ñëåäóþùåé
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

/* øàáëîíû ôèãóð */
enum class Pattern {
	equilateral_triangle,
	romb,
	square,
	cross,
	hexagon,
	curve_triangle,
	pentagon,
	long_triangle
};

/* øàáëîíû íàïðàâëåíèé */
enum class Direction {
	left,
	right,
	vertical,
	horisontal
};

/* ôèãóðà */
class Figure {
public:
	int x, y, r;
	vector<Point> ps; // ìàññèâ òî÷åê
	Pattern pattern;

	Figure() {
		this->x = this->y = this->r = 0;
		this->pattern = Pattern::square;
	}
	Figure(int x, int y, int r) {
		this->x = x;
		this->y = y;
		this->r = r;
		this->pattern = Pattern::square;
	}

	/* Çàäàíèå òî÷êè íà îêðóæíîñòè ôèãóðû */
	void createPoint(double param, bool isConnectToNext = true) {
		this->ps.push_back(Point(this->x + this->r * cos(param), this->y - this->r * sin(param), param, isConnectToNext));
	}

	/* Óñòàíîâêà çíà÷åíèé ñóùåñòâóþùåé òî÷êè*/
	void setPoint(int index, double param) {
		if (index < 0 || index >= this->ps.size())
			return;
		this->ps[index].x = this->x + this->r * cos(param);
		this->ps[index].y = this->y - this->r * sin(param);
		this->ps[index].param = param;
	}

	/* Ñîåäèíåíÿåò òî÷åêè ëèíèÿìè */
	void paint() {
		for (int i = 0; i < this->ps.size(); i++) {
			// Åñëè òî÷êà íå ñîåäèíÿåòñÿ ñî ñëåäóþùåé (â ñëó÷àå ïîñëåäíåé òî÷êè - ñ ïåðâîé)
			if (!this->ps[i].isConnectToNext) {
				// Åñëè ÍÅ ïîñëåäíÿÿ òî÷êà
				if (i != this->ps.size() - 1) {
					MoveToEx(hDC, this->ps[i + 1].x, this->ps[i + 1].y, NULL);
				}
				continue;
			}
			// Åñëè ïåðâàÿ òî÷êà
			if (i == 0) {
				MoveToEx(hDC, this->ps[0].x, this->ps[0].y, NULL);
			}
			// Åñëè ïîñëåäíÿÿ òî÷êà
			if (i == this->ps.size() - 1) {
				LineTo(hDC, this->ps[0].x, this->ps[0].y);
			}
			// Åñëè òî÷êà ìåæäó ïåðâîé è ïîñëåäíåé
			else {
				LineTo(hDC, this->ps[i + 1].x, this->ps[i + 1].y);
			}
		}
	}

	/* ïîâîðîò ôèãóðû */
	void rotate(Direction dir, double step) {
		if (dir == Direction::left) {
			for (int i = 0; i < this->ps.size(); i++) {
				ps[i].param += step;
			}
		}
		else if (dir == Direction::right) {
			for (int i = 0; i < this->ps.size(); i++) {
				ps[i].param -= step;
			}
		}
	}

	/* îòðàæåíèå îòíîñèòåëüíî ñåáÿ */
	void reflect(Direction dir) {
		if (dir == Direction::vertical) {
			for (int i = 0; i < this->ps.size(); i++) {
				this->ps[i].param = -this->ps[i].param;
			}
		}
		if (dir == Direction::horisontal) {
			for (int i = 0; i < this->ps.size(); i++) {
				this->ps[i].param = PI - this->ps[i].param;
			}
		}
	}

	/* ïåðåóñòàíîâêà âåðøèí */
	void rebuild() {
		for (int i = 0; i < this->ps.size(); i++) {
			this->setPoint(i, this->ps[i].param);
		}
	}
};

vector<Figure> figures; // ìàññèâ ôèãóð
int actI = -1, // èíäåêñ àêòèâíîé ôèãóðû
leftmostBorder = 130; // êðàéíÿÿ ëåâàÿ ãðàíèöà ðàñïîëîæåíèÿ äëÿ ôèãóðû
bool flag_shift = false; // ïåðåêëþ÷àòåëü íà êíîïêó SHIFT (çåðêàëüíîå îòðàæåíèå ôèãóðû îòíîñèòåëüíî ñåáÿ)

/* ñîçäàíèå ôèãóðû */
void createFigure(const Pattern& pat) {
	figures.push_back(Figure());
	actI = figures.size() - 1;
	Figure& f = figures[actI];
	switch (pat) {
		case Pattern::equilateral_triangle:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(0);
			f.createPoint(PI);
			break;
		case Pattern::romb:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(0);
			f.createPoint(3 * PI / 2);
			f.createPoint(PI);
			break;
		case Pattern::square:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 4);
			f.createPoint(7 * PI / 4);
			f.createPoint(5 * PI / 4);
			f.createPoint(3 * PI / 4);
			break;
		case Pattern::cross:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(3 * PI / 2, false);
			f.createPoint(PI);
			f.createPoint(0, false);
			break;
		case Pattern::hexagon:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 3);
			f.createPoint(0);
			f.createPoint(5 * PI / 3);
			f.createPoint(4 * PI / 3);
			f.createPoint(PI);
			f.createPoint(2 * PI / 3);
			break;
		case Pattern::curve_triangle:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 3);
			f.createPoint(0);
			f.createPoint(PI);
			break;
		case Pattern::pentagon:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 3);
			f.createPoint(0);
			f.createPoint(3 * PI / 2);
			f.createPoint(PI);
			f.createPoint(2 * PI / 3);
			break;
		case Pattern::long_triangle:
			f = Figure(200, 100, 50);
			f.createPoint(PI / 2);
			f.createPoint(4 * PI / 3);
			f.createPoint(5 * PI / 3);
			break;
	}
	f.pattern = pat; // çàïîìèíàåì òèï ôèãóðû
}

void paintFigures() {
	for (int i = 0; i < figures.size(); i++) {
		SelectObject(hDC, i == actI ? penActiveFigure : penPassiveFigure);
		figures[i].paint(); // îòðèñîâûâàåì i-òóþ ôèãóðó
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

int key_assigments_pos_y; // y êîîðäèíàòà ñòðî÷êè
int leftPadding = 10; // ëåâûé îòñòóï
int bottomPadding = 20; // íèæíèé îòñòóï
int leftFrame = leftPadding + 180; // òîëùèíà ëåâîé ðàìêè
int bottomFrame = bottomPadding + 52; // òîëùèíà íèæíåé ðàìêè

/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	RECT rect;
	Figure& f = figures.size() ? figures[actI] : *&Figure();

	switch (Message) {

		case WM_DESTROY: {
			PostQuitMessage(0); 
			break;
		}
		case WM_PAINT: {
			hDC = BeginPaint(hwnd, &ps);
			GetWindowRect(hwnd, &rect);

			SelectObject(hDC, penFrame);
			MoveToEx(hDC, leftFrame, 0, NULL);
			LineTo(hDC, leftFrame, height(rect) - bottomFrame);
			MoveToEx(hDC, 0, height(rect) - bottomFrame, NULL);
			LineTo(hDC, rect.right, height(rect) - bottomFrame);
			SelectObject(hDC, penDefault);

			if (figures.size()) {
				f.rebuild();
				/*
					Êîððåêöèÿ ïîëîæåíèÿ ôèãóðû ïðè ïåðåñå÷åíèè åþ ãðàíèö
				*/
				bool isLeftFrameCrossed = false, isBottomFrameCrossed = false;
				int maxLeftFrameCrossedDist = 0, maxBottomFrameCrossedDist = 0;
				// Íàéòè íàèáîëüøèå çàñòóïû: ïî õ è ïî ó
				for (int i = 0; i < f.ps.size(); i++) {
					// Åñëè òî÷êà çà ëåâîé ãðàíèöåé
					if (f.ps[i].x < leftFrame) {
						if (maxLeftFrameCrossedDist < leftFrame - f.ps[i].x) maxLeftFrameCrossedDist = leftFrame - f.ps[i].x;
						if (!isLeftFrameCrossed) isLeftFrameCrossed = true;
					}
					// Åñëè òî÷êà çà íèæíåé ãðàíèöåé
					/* debug */
					int drawnHeight = height(rect) - bottomFrame;
					/* debug end */
					if (f.ps[i].y > height(rect) - bottomFrame) {
						if (maxBottomFrameCrossedDist < f.ps[i].y - (height(rect) - bottomFrame)) maxBottomFrameCrossedDist = f.ps[i].y - (height(rect) - bottomFrame);
						if (!isBottomFrameCrossed) isBottomFrameCrossed = true;
					}
				}
				// Åñëè íàéäåí õîòü îäèí çàñòóï ïî õ/ó - ñìåñòèòü öåíòð ôèãóðû íà ðàçìåð íàèá. çàñòóïîâ ïî õ/ó
				if (isLeftFrameCrossed) {
					f.x += maxLeftFrameCrossedDist;
				}
				if (isBottomFrameCrossed) {
					f.y -= maxBottomFrameCrossedDist;
				}
				f.rebuild();
				paintFigures();
			}

			reflectMode = flag_shift ? L"H" : L"V";
			reflectModeFull = REFLECT_MODE_CATPTION + reflectMode;

			TextOut(hDC, leftPadding, 10, (INDEX_CAPTION + to_wstring(actI)).c_str(), (INDEX_CAPTION + to_wstring(actI)).size());
			TextOut(hDC, leftPadding, 40, L"X: ", 3);
			TextOut(hDC, leftPadding, 70, L"Y: ", 3);
			TextOut(hDC, leftPadding, 100, L"R: ", 3);
			TextOut(hDC, leftPadding, 160,
					(PATTERN_TYPES_CAPTION + (figures.size() ? PATTERN_TYPES[(int)f.pattern] : L"")).c_str(),
					(PATTERN_TYPES_CAPTION + (figures.size() ? PATTERN_TYPES[(int)f.pattern] : L"")).size()
			);
			TextOut(hDC, leftPadding, key_assigments_pos_y, STR_KEY_ASSIGMENTS.c_str(), STR_KEY_ASSIGMENTS.size());
			TextOut(hDC, leftPadding, 200, (reflectModeFull).c_str(), reflectModeFull.size());

			if (figures.size()) {
				SetDlgItemInt(hwnd, ID_EDIT_FIGURE_INDEX, actI, true);
				SetDlgItemInt(hwnd, ID_EDIT_X, f.x, true);
				SetDlgItemInt(hwnd, ID_EDIT_Y, f.y, true);
				SetDlgItemInt(hwnd, ID_EDIT_R, f.r, true);
				SetDlgItemInt(hwnd, ID_EDIT_TYPE, (UINT)f.pattern, true);
			}
			SetFocus(hwnd);
			EndPaint(hwnd, &ps);
			break;
		}

		// ïðè èçìåíåíèè ðàçìåðà îêíà
		case WM_SIZE:
			GetClientRect(hwnd, &rect);
			key_assigments_pos_y = rect.bottom - rect.top - bottomPadding;
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
						actI = GetDlgItemInt(hwnd, ID_EDIT_FIGURE_INDEX, NULL, true);
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
				case VK_SHIFT: flag_shift = !flag_shift;
					break;

				case VK_LEFT:
					if (actI > 0) actI--;
					else actI = figures.size() - 1;
					break;

				case VK_RIGHT:
					if (actI < figures.size() - 1) actI++;
					else actI = 0;
					break;

				case VK_UP: f.r += 10;
					break;

				case VK_DOWN:
					if (f.r > 20) f.r -= 10;
					break;

				case VK_F1: createFigure(Pattern::equilateral_triangle);
					break;

				case VK_F2: createFigure(Pattern::romb);
					break;

				case VK_F3: createFigure(Pattern::square);
					break;

				case VK_F4: createFigure(Pattern::cross);
					break;

				case VK_F5: createFigure(Pattern::hexagon);
					break;

				case VK_F6: createFigure(Pattern::curve_triangle);
					break;

				case VK_F7: createFigure(Pattern::pentagon);
					break;

				case VK_F8: createFigure(Pattern::long_triangle);
					break;

				case 0x52: // 'R' key
					if (flag_shift) f.reflect(Direction::vertical);
					else f.reflect(Direction::horisontal);
					break;

				/* óäàëåíèå ôèãóðû */
				case VK_DELETE:
					if (figures.size()) {
						figures.erase(figures.begin() + actI);
						if (!figures.size()) {
							actI = -1;
							SetDlgItemInt(hwnd, ID_EDIT_FIGURE_INDEX, actI, true);
							SetDlgItemInt(hwnd, ID_EDIT_X, 0, true);
							SetDlgItemInt(hwnd, ID_EDIT_Y, 0, true);
							SetDlgItemInt(hwnd, ID_EDIT_R, 0, true);
							SetDlgItemInt(hwnd, ID_EDIT_TYPE, 0, true);
						}
						if (figures.size() == 1) actI = 0;
						else {
							if (actI == 0) actI = figures.size() - 1;
							else actI--;
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
	key_assigments_pos_y = rect.bottom - rect.top - bottomPadding;

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
