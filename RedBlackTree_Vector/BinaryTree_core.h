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

void PreorderPrint();	// 전위순회 출력
void InorderPrint();	// 중위순회 출력
void PostorderPrint();	// 후위순회 출력

void printPreorder(st_Node* param, int depth);
void printInorder(st_Node* param, int depth);
void printPostorder(st_Node* param);
