#pragma once

#include "resource.h"

struct st_Node;

// extern
extern st_Node* g_rootNode;
extern st_Node Nil;

enum NODE_COLOR
{
	NONE = -1,
	BLACK = 0,
	RED
};

struct st_Node
{
	st_Node()
		:_left(&Nil), _right(&Nil), _parent(nullptr), _value(-999), _xPos(-999), _color(NODE_COLOR::RED)
	{
	}

	~st_Node()
	{
		_left = &Nil;
		_right = &Nil;
		_parent = nullptr;
		_value = -999;
		_xPos = -999;
		_color = NODE_COLOR::NONE;
	}

	st_Node* _left;
	st_Node* _right;
	st_Node* _parent;

	NODE_COLOR _color;

	int _value;
	int _xPos;
};

void InitTree(void);
bool InsertNode(int param);
bool DeleteNode(int param);
void inDeleteNode(st_Node* parent, st_Node* node);
bool ReleaseNode(st_Node* param);
st_Node* FindNode(int param);

void rotateLeft(st_Node* param);
void rotateRight(st_Node* param);

/////////////////////////////////////////
void fixedAlign(st_Node * param);
/////////////////////////////////////////

void AlignInsert(int param);
void AlignDelete(int param);

void PreorderPrint();	// 전위순회 출력
void InorderPrint();	// 중위순회 출력
void PostorderPrint();	// 후위순회 출력

void printPreorder(st_Node* param, int depth);
void printInorder(st_Node* param, int depth);
void printPostorder(st_Node* param);

/*

# 모든 문제는 부모가 레드인 경우이므로 할아버지는 블랙이다.

회전 하면서 밸런스 맞추는 과정에서 닐까지 내려가서 닐에 대한 부모를 탐색하는 경우가 나옴. 근데 닐은 부모가 존재할수 없다.
닐의 부모는 필요할때마다 연결시켜서 쓰는 용도. 닐의 부모가 누구냐를 생각할 필요 없음. 만약 닐이 부모를 탐색해야 할 필요가
생기면 걔는 이미 페어런츠가 그 노드로 꽂혀있을 것. 그럼 그 노드를 다시 널로 하는게 아니라 그냥 두고 나중에 할일이 있으면
걔가 다시 부모를 연결할 것.

*/