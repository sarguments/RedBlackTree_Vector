#include "stdafx.h"

#include "hoxy_Header.h"
#include "BinaryTree_core.h"

#define MAX_LOADSTRING 100
#define CIRCLE_SIZE 16

HWND g_hWnd;
HBRUSH g_redBrush;
HBRUSH g_oldBrush;
HPEN g_oldPen;
HFONT g_font;

int g_centerX;

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// DIALOG 전역
BOOL CALLBACK InfoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam);
HWND g_inputDlg;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	CCmdStart myCmdStart;

	// 전역 문자열을 초기화합니다.
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_BINARYTREEUSAS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BINARYTREEUSAS));

	// 브러시 생성
	g_redBrush = CreateSolidBrush(RGB(255, 0, 0));
	g_font = CreateFont(20, 0, 0, 0, FW_BOLD, 0, 0, 0,
		DEFAULT_CHARSET, 0, 0, 0, 0,
		L"D2Coding");

	// 각종 수치들
	RECT windowRect;
	GetClientRect(g_hWnd, &windowRect);
	g_centerX = windowRect.right / 2;

	// 모달리스 DIALOG //
	g_inputDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), g_hWnd, InfoDlgProc);
	ShowWindow(g_inputDlg, SW_SHOW);

	MSG msg;

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

BOOL CALLBACK InfoDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
	{
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			int inputNum = GetDlgItemInt(hDlg, IDC_NUM, NULL, FALSE);
			if (inputNum == NULL)
			{
				return FALSE;
			}

			InsertNode(inputNum);
			SetDlgItemText(hDlg, IDC_NUM, L"");

			InvalidateRect(g_hWnd, NULL, TRUE);

			return TRUE;
		}
		break;
		case IDC_DELETE:
		{
			int inputNum = GetDlgItemInt(hDlg, IDC_NUM, NULL, FALSE);
			if (inputNum == NULL)
			{
				return FALSE;
			}

			DeleteNode(inputNum);
			SetDlgItemText(hDlg, IDC_NUM, L"");

			InvalidateRect(g_hWnd, NULL, TRUE);

			return TRUE;
		}
		break;
		case IDCANCEL:
		{
			EndDialog(hDlg, IDCANCEL);
			DestroyWindow(g_hWnd);
			return TRUE;
		}
		break;
		}
	}
	break;
	}

	return FALSE;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BINARYTREEUSAS));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_BINARYTREEUSAS);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_CLEAR:
		{
			ReleaseNode(g_rootNode);
			wcout << L"RELEASE" << endl;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.

		PreorderPrint();

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void PreorderPrint()
{
	printPreorder(g_rootNode, 0);
}

void InorderPrint()
{
	printInorder(g_rootNode, 0);
}

void PostorderPrint()
{
	printPostorder(g_rootNode);
}

void printPreorder(st_Node * param, int depth)
{
	if (param == nullptr)
	{
		return;
	}

	wcout << param->_value << endl;

	RECT windowRect;
	GetClientRect(g_hWnd, &windowRect);

	// 루트노드 이면
	if (param->_parent == nullptr && param->_value != -999)
	{
		DrawNode(g_centerX, 50, param->_value);
		param->_xPos = g_centerX;
	}
	else if (param->_parent != nullptr)
	{
		// 루트노드가 아닌 경우
		// 왼족 인지 오른쪽 인지
		if (param == param->_parent->_left)
		{
			int parentX = param->_parent->_xPos;

			// 할아버지 노드가 nullptr 인지(루트의 자식)
			if (param->_parent->_parent == nullptr)
			{
				//현재 노드의 위치 = 부모 - (부모 / 2)
				param->_xPos = parentX - (parentX / 2);
			}
			else
			{
				// 현재 노드의 위치 = 부모 - (할아버지 - 부모) / 2
				int granParentX = param->_parent->_parent->_xPos;
				param->_xPos = parentX - abs(granParentX - parentX) / 2;
			}

			HDC localDC = GetDC(g_hWnd);
			MoveToEx(localDC, param->_parent->_xPos, 50 + (depth - 1) * 100, NULL);
			LineTo(localDC, param->_xPos, 50 + depth * 100);
			ReleaseDC(g_hWnd, localDC);

			DrawNode(param->_xPos, 50 + depth * 100, param->_value);
		}
		else
		{
			int parentX = param->_parent->_xPos;

			// 할아버지 노드가 nullptr 인지(루트의 자식)
			if (param->_parent->_parent == nullptr)
			{
				//현재 노드의 위치 = 부모 + (부모 / 2)
				param->_xPos = parentX + (parentX / 2);
			}
			else
			{
				// 현재 노드의 위치 = 부모 + (할아버지 - 부모) / 2
				int granParentX = param->_parent->_parent->_xPos;
				param->_xPos = parentX + abs(granParentX - parentX) / 2;
			}

			HDC localDC = GetDC(g_hWnd);
			MoveToEx(localDC, param->_parent->_xPos, 50 + (depth - 1) * 100, NULL);
			LineTo(localDC, param->_xPos, 50 + depth * 100);
			ReleaseDC(g_hWnd, localDC);

			DrawNode(param->_xPos, 50 + depth * 100, param->_value);
		}
	}
	else
	{
		wcout << L"-999 is EMPTY" << endl;
		return;
	}

	printPreorder(param->_left, depth + 1);
	printPreorder(param->_right, depth + 1);
}

void printInorder(st_Node * param, int depth)
{
	if (param == nullptr)
	{
		return;
	}

	printInorder(param->_left, depth + 1);
	wcout << param->_value << endl;
	printInorder(param->_right, depth + 1);
}

void printPostorder(st_Node * param)
{
	if (param == nullptr)
	{
		return;
	}

	printPostorder(param->_left);
	printPostorder(param->_right);
	wcout << param->_value << endl;
}

void DrawCircle(int x, int y, bool param)
{
	HDC localDC = GetDC(g_hWnd);
	///////////////////////////////

	if (param)
	{
		g_oldBrush = (HBRUSH)SelectObject(localDC, g_redBrush);
	}
	else
	{
		HBRUSH localBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		g_oldBrush = (HBRUSH)SelectObject(localDC, localBrush);
	}

	Ellipse(localDC, x - CIRCLE_SIZE, y - CIRCLE_SIZE, x + CIRCLE_SIZE, y + CIRCLE_SIZE);

	SelectObject(localDC, g_oldBrush);

	///////////////////////////////
	ReleaseDC(g_hWnd, localDC);
}

void DrawMyText(int x, int y, const WCHAR* param)
{
	HDC localDC = GetDC(g_hWnd);
	RECT textRect;
	textRect.left = x - CIRCLE_SIZE;
	textRect.right = x + CIRCLE_SIZE;
	textRect.top = y - CIRCLE_SIZE + (CIRCLE_SIZE / 2);
	textRect.bottom = y + CIRCLE_SIZE + (CIRCLE_SIZE / 2);

	HFONT oldFont = (HFONT)SelectObject(localDC, g_font);
	COLORREF oldColor = SetTextColor(localDC, RGB(255, 255, 255));
	SetBkMode(localDC, TRANSPARENT);

	///////////////////////////////

	DrawTextW(localDC, param, wcslen(param), &textRect, DT_CENTER | DT_VCENTER);

	///////////////////////////////
	SetTextColor(localDC, oldColor);
	SelectObject(localDC, oldFont);

	ReleaseDC(g_hWnd, localDC);
}

void DrawNode(int x, int y, int value, bool isRed)
{
	DrawCircle(x, y, isRed);

	WCHAR numText[10] = { 0, };
	wsprintf(numText, L"%d", value);
	DrawMyText(x, y, numText);
}