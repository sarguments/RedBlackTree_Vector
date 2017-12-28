#include "stdafx.h"

#include "hoxy_Header.h"
#include "BinaryTree_core.h"
#include "MemoryPool_G41.h"

CMemoryPool<st_Node> g_memPool(200, true);
st_Node* g_rootNode;
int g_nodeCount;

st_Node Nil;	// 끝 리프노드. 무조건 블랙 / 데이터 무 / NULL 의 같은 용도.

void InitTree(void)
{
	Nil._color = NODE_COLOR::BLACK;
}

bool InsertNode(int param)
{
	// TODO : 루트가 Nil? (추가한거 싹 비운다음 처음으로 추가할때)
	// 루트가 널인가 ?
	if (g_rootNode == nullptr || g_rootNode == &Nil)
	{
		g_rootNode = g_memPool.Alloc();
		g_rootNode->_value = param;
		g_rootNode->_color = NODE_COLOR::BLACK;

		wcout << L"inserted rootNode : " << g_rootNode->_value << endl;
		++g_nodeCount;

		// 넣고 끝냄
		return true;
	}

	//루트부터 현재 노드 값 보다 큰지 작은지 판단해서
	st_Node* localNode = g_rootNode;
	st_Node* parentNode = nullptr;

	while (1)
	{
		if (localNode == &Nil)
		{
			st_Node* newNode = g_memPool.Alloc();
			newNode->_value = param;

			// 부모의 왼쪽 노드인지 오른쪽 노드인지
			if (newNode->_value < parentNode->_value)
			{
				parentNode->_left = newNode;
			}
			else if (newNode->_value > parentNode->_value)
			{
				parentNode->_right = newNode;
			}
			else
			{
				// 같은 경우
				return false;
			}

			wcout << L"inserted Node : " << param << endl;

			newNode->_parent = parentNode;
			++g_nodeCount;

			// 넣고 끝냄
			return true;
		}

		parentNode = localNode;

		if (param < localNode->_value)
		{
			localNode = localNode->_left;
		}
		else if (param > localNode->_value)
		{
			localNode = localNode->_right;
		}
		else
		{
			// 같은 값 들어온 경우 break
			break;
		}
	}

	return false;
}

bool DeleteNode(int param)
{
	// 빈 트리일 경우
	if (g_rootNode == nullptr || g_rootNode == &Nil)
	{
		return false;
	}

	st_Node* delParent = nullptr; // 초기 부모 포인터
	st_Node* delNode = g_rootNode; // 초기 현재 포인터(루트)

	// data가 일치하는 노드 찾는다.
	while (delNode != &Nil && delNode->_value != param)
	{
		// 현재포인터를 부모포인터로 설정
		delParent = delNode;

		// 찾으려는 데이터가 노드의 데이터보다 작은 경우
		if (param < delNode->_value)
		{
			delNode = delNode->_left;
		}
		// 찾으려는 데이터가 노드의 데이터보다 큰 경우
		else
		{
			delNode = delNode->_right;
		}
	}

	// 못찾은 경우
	if (delNode == &Nil)
	{
		wprintf(L"NOT FOUND.. delete data\n");
		return false;
	}
	else
	{
		// 찾았으면 지운다.
		inDeleteNode(delParent, delNode);
	}

	return true;
}

void inDeleteNode(st_Node * parent, st_Node * node)
{
	st_Node* toDelNode = node;

	// 왼쪽이나 오른쪽 자식만 있는 경우
	if (toDelNode->_left == &Nil || toDelNode->_right == &Nil)
	{
		st_Node* delNodeChild = &Nil;

		// 삭제할 노드의 자식노드
		if (toDelNode->_left != &Nil)
		{
			// 왼쪽만 있는 경우
			delNodeChild = toDelNode->_left;
		}
		else if (toDelNode->_right != &Nil)
		{
			// 오른쪽만 있는 경우
			delNodeChild = toDelNode->_right;
		}
		// TODO : 자식노드가 없는 경우는 &Nil

		// 삭제할 노트가 루트노드이면
		if (toDelNode == g_rootNode)
		{
			g_rootNode = delNodeChild;

			// 루트의 부모 X
			if (g_rootNode != nullptr)
			{
				g_rootNode->_parent = nullptr;
			}
		}
		else
		{
			// 루트가 아니라면..
			// 삭제하려는 노드가 왼쪽 자식이었을 경우
			if (parent->_left == toDelNode)
			{
				// 삭제하려는 노드의 자식을 부모의 왼쪽에 꽂는다
				parent->_left = delNodeChild;
			}
			else
			{
				// 삭제하려는 노드의 자식을 부모의 오른쪽에 꽂는다
				parent->_right = delNodeChild;
			}

			// 부모 연결
			if (delNodeChild != &Nil)
			{
				delNodeChild->_parent = parent;
			}
		}
	}
	// 자식이 둘인 경우
	else
	{
		st_Node* rightLowNodeParent = toDelNode; // 오른쪽에서 가장 작은 노드의 부모
		st_Node* rightLowNode = toDelNode->_right; // 오른쪽에서 가장 작은 노드

		// 오른쪽에서 가장 작은 노드를 찾음
		while (rightLowNode->_left != &Nil)
		{
			rightLowNodeParent = rightLowNode;
			rightLowNode = rightLowNode->_left;
		}

		// '오른쪽 자식중 제일 작은 노드'가 왼쪽 자식이었을 경우
		if (rightLowNode == rightLowNodeParent->_left)
		{
			// '오른쪽 자식중 제일 작은 노드'의 오른쪽 자식을 자신의 위치로
			rightLowNodeParent->_left = rightLowNode->_right;
		}
		// '오른쪽 자식중 제일 작은 노드'가 오른쪽 자식이었을 경우
		else
		{
			rightLowNodeParent->_right = rightLowNode->_right;
		}

		// 부모 연결
		if (rightLowNode->_right != &Nil)
		{
			rightLowNode->_right->_parent = rightLowNodeParent;
		}

		// 원래 삭제될 노드에 교체될 노드의 값 복사
		toDelNode->_value = rightLowNode->_value;

		// 교체될 노드의 포인터로 변경
		toDelNode = rightLowNode;
	}

	// 삭제
	g_memPool.Free(toDelNode);
	--g_nodeCount;
}

bool ReleaseNode(st_Node * param)
{
	if (param == &Nil)
	{
		return false;
	}

	ReleaseNode(param->_left);
	ReleaseNode(param->_right);

	g_memPool.Free(param);
	--g_nodeCount;

	return true;
}

st_Node * FindNode(int param)
{
	// 빈 트리일 경우
	if (g_rootNode == nullptr || g_rootNode == &Nil)
	{
		return nullptr;
	}

	st_Node* foundNode = g_rootNode; // 초기 현재 포인터(루트)

	// data가 일치하는 노드 찾는다.
	while (foundNode != &Nil && foundNode->_value != param)
	{
		// 찾으려는 데이터가 노드의 데이터보다 작은 경우
		if (param < foundNode->_value)
		{
			foundNode = foundNode->_left;
		}
		// 찾으려는 데이터가 노드의 데이터보다 큰 경우
		else
		{
			foundNode = foundNode->_right;
		}
	}

	// 못찾은 경우
	if (foundNode == &Nil)
	{
		wprintf(L"NOT FOUND..\n");
		return nullptr;
	}

	return foundNode;
}

void rotateLeft(st_Node * param)
{
	wcout << L"rotateLEFT" << endl;

	// 원래 N
	st_Node* toRotateNode = param;

	// 원래 N의 오른 자식의 왼편
	st_Node* rightChildLeft = toRotateNode->_right->_left;

	// N의 위치를 원래 N의 부모로 나타냄
	// 일단 N이 어느방향 자식이었는지

	// N이 루트노드일 경우
	if (toRotateNode == g_rootNode)
	{
		g_rootNode = toRotateNode->_right;
		g_rootNode->_parent = nullptr;
	}
	else if (toRotateNode == toRotateNode->_parent->_left)
	{
		// N의 위치 = N의 오른자식
		toRotateNode->_parent->_left = toRotateNode->_right;
		toRotateNode->_right->_parent = toRotateNode->_parent;
	}
	else
	{
		toRotateNode->_parent->_right = toRotateNode->_right;
		toRotateNode->_right->_parent = toRotateNode->_parent;
	}
	
	// N의 오른자식의 왼편 = 원래 N
	toRotateNode->_right->_left = toRotateNode;
	toRotateNode->_parent = toRotateNode->_right;

	// N의 오른편 = 원래 N의 오른자식의 왼편자식
	toRotateNode->_right = rightChildLeft;
	rightChildLeft->_parent = toRotateNode;
}

// TODO : nullptr 예외처리 없음
void rotateRight(st_Node * param)
{
	wcout << L"rotateRIGHT" << endl;

	// 원래 N
	st_Node* toRotateNode = param;

	// 원래 N의 왼쪽 자식의 오른편
	st_Node* leftChildRight = toRotateNode->_left->_right;

	// N의 위치를 원래 N의 부모로 나타냄
	// 일단 N이 어느방향 자식이었는지
	// N이 루트노드일 경우
	if (toRotateNode == g_rootNode)
	{
		g_rootNode = toRotateNode->_left;
		g_rootNode->_parent = nullptr;
	}
	else if (toRotateNode == toRotateNode->_parent->_left)
	{
		// N의 위치 = N의 왼 자식
		toRotateNode->_parent->_left = toRotateNode->_left;
		toRotateNode->_left->_parent = toRotateNode->_parent;
	}
	else
	{
		toRotateNode->_parent->_right = toRotateNode->_left;
		toRotateNode->_left->_parent = toRotateNode->_parent;
	}

	// N의 왼쪽 자식의 오른편 = 원래 N
	toRotateNode->_left->_right = toRotateNode;
	toRotateNode->_parent = toRotateNode->_left;

	// N의 왼편 = 원래 N의 왼쪽 자식의 오른편 자식
	toRotateNode->_left = leftChildRight;
	leftChildRight->_parent = toRotateNode;
}

// 할아버지 기준으로 내 부모가 왼쪽
void grandLeftAlign(st_Node * param)
{
	st_Node* nowNode = param;

	while (1)
	{
		/* ////////////////////////////////////////////////
		신규 노드의 부모가 블랙이라면 전혀 문제가 없음.
		아래의 문제 상황은 부모가 레드인 경우 해당됨.
		*/ ////////////////////////////////////////////////
		if (nowNode->_parent->_color == NODE_COLOR::BLACK)
		{
			wcout << L"nowNode is BLACK" << endl;
			break;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*/////////////////////////////////////////
		//		  GP        |		  GP         //
		//		 /  \		|		 /  \		 //
		//		P(R) U(R)	|		P(R) U(R)	 //
		//	   / 			|	     \			 //
		//	  N(R)			|	      N(R)		 //
		*//////////////////////////////////////////
		//1. 나도 부모도 삼촌도 레드.
		st_Node* localParent = nowNode->_parent;
		if (nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_right->_color == NODE_COLOR::RED)
		{
			wcout << L"type 1." << endl;

			//	부모와 삼촌을 블랙으로 바꾸고, 할아버지는 레드로 바꾼다.
			localParent->_color = NODE_COLOR::BLACK;
			localParent->_parent->_right->_color = NODE_COLOR::BLACK;
			localParent->_parent->_color = NODE_COLOR::RED;

			//	할아버지가 레드로 바뀜으로 인해서, 할아버지 상단의 색상도 모두 비교를 해보아야 한다.
			//	그래서 결국 ROOT 노드까지 올라가면서 이를 반복 확인 해야한다.
			//	할아버지를 새 노드로 잡고 다시 확인.
			nowNode = localParent->_parent;

			if (nowNode->_parent == nullptr)
			{
				wcout << L"nowNode's parent is nullptr(ROOT)" << endl;
				break;
			}

			continue;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*///////////////////
		//		 GP        //
		//		/  \	   //
		//	   P(R) U(B)   //
		//	    \		   //
		//		 N(R)	   //
		*////////////////////
		//2. 나는 오른레드, 부모도 레드, 삼촌은 블랙 상황.
		localParent = nowNode->_parent;
		if (nowNode == localParent->_right &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_right->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 2." << endl;

			//	색상만 맞추는걸로 끝내지 않고, 회전을 통해서 밸런스를 맞춰 준다.
			//	회전을 위해선 좌측 또는 우측의 한쪽 방향으로 노드가 몰려있는게 좋다.
			//	그러므로 우선  3번조건 상황으로 만든다.

			//	나는 레드, 부모의 오른쪽 노드, 부모도 레드, 인 경우에는
			//	부모기준으로 좌회전하여 신규노드를 부모의 위치로 올리고,
			//	부모를 신노드 왼쪽 자식으로 옮겨서 밸런스를 맞춘다.
			rotateLeft(localParent);

			//	이로서 2번 상황은 3번 상황으로 변경 되었음.
			nowNode = localParent;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*///////////////////
		//	 	  GP       //
		//	   	 /  \	   //
		//	    P(R) U(B)  //
		//	   /		   //
		//	  N(R)		   //
		*////////////////////
		//3. 나는 왼레드, 부모도 레드, 삼촌은 블랙
		localParent = nowNode->_parent;
		if (nowNode == localParent->_left &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_right->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 3." << endl;

			//	부모를 블랙으로
			localParent->_color = NODE_COLOR::BLACK;

			//	할아버지는 레드로
			localParent->_parent->_color = NODE_COLOR::RED;

			//	할아버지 기준으로 우회전!
			rotateRight(localParent->_parent);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	
	// 마지막에 루트는 블랙
	g_rootNode->_color = NODE_COLOR::BLACK;
}

// 할아버지 기준으로 내 부모가 오른쪽
void grandRightAlign(st_Node * param)
{
	st_Node* nowNode = param;

	while (1)
	{
		/* ////////////////////////////////////////////////
		신규 노드의 부모가 블랙이라면 전혀 문제가 없음.
		아래의 문제 상황은 부모가 레드인 경우 해당됨.
		*/ ////////////////////////////////////////////////
		if (nowNode->_parent->_color == NODE_COLOR::BLACK)
		{
			wcout << L"nowNode is BLACK" << endl;
			break;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*///////////////////////////////////////////
		//		  GP        |	 	  GP           //
		//		 /  \		|	 	 /  \		   //
		//	   U(R)  P(R)	|	   U(R)  P(R)	   //
		//	        / 		|	          \ 	   //
		//	       N(R)	  	|	           N(R)	   //
		*////////////////////////////////////////////
		//1. 나도 부모도 삼촌도 레드.
		st_Node* localParent = nowNode->_parent;
		if (nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_left->_color == NODE_COLOR::RED)
		{
			wcout << L"type 1." << endl;

			//	부모와 삼촌을 블랙으로 바꾸고, 할아버지는 레드로 바꾼다.
			localParent->_color = NODE_COLOR::BLACK;
			localParent->_parent->_left->_color = NODE_COLOR::BLACK;
			localParent->_parent->_color = NODE_COLOR::RED;

			//	할아버지가 레드로 바뀜으로 인해서, 할아버지 상단의 색상도 모두 비교를 해보아야 한다.
			//	그래서 결국 ROOT 노드까지 올라가면서 이를 반복 확인 해야한다.
			//	할아버지를 새 노드로 잡고 다시 확인.
			nowNode = localParent->_parent;

			if (nowNode->_parent == nullptr)
			{
				wcout << L"nowNode's parent is nullptr(ROOT)" << endl;
				break;
			}

			continue;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*///////////////////
		//		 GP        //
		//		/  \	   //
		//	  U(B)  P(R)   //
		//	       /	   //
		//	      N(R)	   //
		*////////////////////
		//2. 나는 왼레드, 부모도 레드, 삼촌은 블랙 상황.
		localParent = nowNode->_parent;
		if (nowNode == localParent->_left &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_left->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 2." << endl;

			//	색상만 맞추는걸로 끝내지 않고, 회전을 통해서 밸런스를 맞춰 준다.
			//	회전을 위해선 좌측 또는 우측의 한쪽 방향으로 노드가 몰려있는게 좋다.
			//	그러므로 우선  3번조건 상황으로 만든다.

			//	나는 레드, 부모의 왼쪽 노드, 부모도 레드, 인 경우에는
			//	부모기준으로 우회전하여 신규노드를 부모의 위치로 올리고,
			//	부모를 신노드 오른쪽 자식으로 옮겨서 밸런스를 맞춘다.
			rotateRight(localParent);

			//	이로서 2번 상황은 3번 상황으로 변경 되었음.
			nowNode = localParent;
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*///////////////////
		//		 GP        //
		//		/  \	   //
		//	  U(B)  P(R)   //
		//	         \	   //
		//	          N(R) //
		*////////////////////
		//3. 나는 오른레드, 부모도 레드, 삼촌은 블랙
		localParent = nowNode->_parent;
		if (nowNode == localParent->_right &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_left->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 3." << endl;

			//	부모를 블랙으로
			localParent->_color = NODE_COLOR::BLACK;

			//	할아버지는 레드로
			localParent->_parent->_color = NODE_COLOR::RED;

			//	할아버지 기준으로 좌회전!
			rotateLeft(localParent->_parent);
		}

		// TODO : 마지막에 루트는 블랙
		g_rootNode->_color = NODE_COLOR::BLACK;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	// 마지막에 루트는 블랙
	g_rootNode->_color = NODE_COLOR::BLACK;
}

void AlignInsert(int param)
{
	// TODO : 루트가 Nil? (추가한거 싹 비운다음 처음으로 추가할때)
	// 루트가 널인가 ?
	if (g_rootNode == nullptr || g_rootNode == &Nil)
	{
		g_rootNode = g_memPool.Alloc();
		g_rootNode->_value = param;
		g_rootNode->_color = NODE_COLOR::BLACK;

		wcout << L"inserted rootNode : " << g_rootNode->_value << endl;
		++g_nodeCount;

		// 넣고 끝냄
		return;
	}

	//루트부터 현재 노드 값 보다 큰지 작은지 판단해서
	st_Node* localNode = g_rootNode;
	st_Node* parentNode = nullptr;

	while (1)
	{
		if (localNode == &Nil)
		{
			st_Node* newNode = g_memPool.Alloc();
			newNode->_value = param;

			// 부모의 왼쪽 노드인지 오른쪽 노드인지
			if (newNode->_value < parentNode->_value)
			{
				parentNode->_left = newNode;
			}
			else if (newNode->_value > parentNode->_value)
			{
				parentNode->_right = newNode;
			}
			else
			{
				// 같은 경우
				return;
			}

			wcout << L"inserted Node : " << param << endl;

			newNode->_parent = parentNode;
			++g_nodeCount;

			// 생성해서 넣었으면 localNode를 newNode로 갱신
			localNode = newNode;

			break;
		}

		parentNode = localNode;

		if (param < localNode->_value)
		{
			localNode = localNode->_left;
		}
		else if (param > localNode->_value)
		{
			localNode = localNode->_right;
		}
		else
		{
			// 같은 값 들어온 경우 break
			return;
		}
	}

	// 넣은게 루트였으면 바로 리턴
	if (localNode->_parent == nullptr)
	{
		return;
	}
	
	// TODO : 루트 블랙 언제?
	if (localNode->_parent == g_rootNode)
	{
		localNode->_parent->_color = NODE_COLOR::BLACK;
	}

	// 부모가 블랙인 경우 바로 리턴
	if (localNode->_parent->_color == NODE_COLOR::BLACK)
	{
		wcout << L"parent is BLACK" << endl;
		return;
	}

	if (localNode->_parent == localNode->_parent->_parent->_left)
	{
		wcout << L"parent is Grand's Left.. leftAlign" << endl;
		grandLeftAlign(localNode);
	}
	else
	{
		wcout << L"parent is Grand's Right.. ReftAlign" << endl;
		grandRightAlign(localNode);
	}
}
