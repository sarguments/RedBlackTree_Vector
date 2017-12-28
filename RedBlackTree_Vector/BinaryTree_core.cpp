#include "stdafx.h"

#include "hoxy_Header.h"
#include "BinaryTree_core.h"
#include "MemoryPool_G41.h"

CMemoryPool<st_Node> g_memPool(200, true);
st_Node* g_rootNode;
int g_nodeCount;

st_Node Nil;	// �� �������. ������ �� / ������ �� / NULL �� ���� �뵵.

void InitTree(void)
{
	Nil._color = NODE_COLOR::BLACK;
}

bool InsertNode(int param)
{
	// ��Ʈ�� ���ΰ� ?
	if (g_rootNode == nullptr)
	{
		g_rootNode = g_memPool.Alloc();
		g_rootNode->_value = param;
		g_rootNode->_color = NODE_COLOR::BLACK;

		wcout << L"inserted rootNode : " << g_rootNode->_value << endl;
		++g_nodeCount;

		// �ְ� ����
		return true;
	}

	//��Ʈ���� ���� ��� �� ���� ū�� ������ �Ǵ��ؼ�
	st_Node* localNode = g_rootNode;
	st_Node* parentNode = nullptr;

	while (1)
	{
		if (localNode == &Nil)
		{
			st_Node* newNode = g_memPool.Alloc();
			newNode->_value = param;

			// �θ��� ���� ������� ������ �������
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
				// ���� ���
				return false;
			}

			wcout << L"inserted Node : " << param << endl;

			newNode->_parent = parentNode;
			++g_nodeCount;

			// �ְ� ����
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
			// ���� �� ���� ��� break
			break;
		}
	}

	return false;
}

bool DeleteNode(int param)
{
	// �� Ʈ���� ���
	if (g_rootNode == nullptr || g_rootNode == &Nil)
	{
		return false;
	}

	st_Node* delParent = nullptr; // �ʱ� �θ� ������
	st_Node* delNode = g_rootNode; // �ʱ� ���� ������(��Ʈ)

	// data�� ��ġ�ϴ� ��� ã�´�.
	while (delNode != &Nil && delNode->_value != param)
	{
		// ���������͸� �θ������ͷ� ����
		delParent = delNode;

		// ã������ �����Ͱ� ����� �����ͺ��� ���� ���
		if (param < delNode->_value)
		{
			delNode = delNode->_left;
		}
		// ã������ �����Ͱ� ����� �����ͺ��� ū ���
		else
		{
			delNode = delNode->_right;
		}
	}

	// ��ã�� ���
	if (delNode == &Nil)
	{
		wprintf(L"NOT FOUND.. delete data\n");
		return false;
	}
	else
	{
		// ã������ �����.
		inDeleteNode(delParent, delNode);
	}

	return true;
}

void inDeleteNode(st_Node * parent, st_Node * node)
{
	st_Node* toDelNode = node;

	// �����̳� ������ �ڽĸ� �ִ� ���
	if (toDelNode->_left == &Nil || toDelNode->_right == &Nil)
	{
		st_Node* delNodeChild = &Nil;

		// ������ ����� �ڽĳ��
		if (toDelNode->_left != &Nil)
		{
			// ���ʸ� �ִ� ���
			delNodeChild = toDelNode->_left;
		}
		else if (toDelNode->_right != &Nil)
		{
			// �����ʸ� �ִ� ���
			delNodeChild = toDelNode->_right;
		}
		// TODO : �ڽĳ�尡 ���� ���� &Nil

		// ������ ��Ʈ�� ��Ʈ����̸�
		if (toDelNode == g_rootNode)
		{
			g_rootNode = delNodeChild;

			// ��Ʈ�� �θ� X
			if (g_rootNode != nullptr)
			{
				g_rootNode->_parent = nullptr;
			}
		}
		else
		{
			// ��Ʈ�� �ƴ϶��..
			// �����Ϸ��� ��尡 ���� �ڽ��̾��� ���
			if (parent->_left == toDelNode)
			{
				// �����Ϸ��� ����� �ڽ��� �θ��� ���ʿ� �ȴ´�
				parent->_left = delNodeChild;
			}
			else
			{
				// �����Ϸ��� ����� �ڽ��� �θ��� �����ʿ� �ȴ´�
				parent->_right = delNodeChild;
			}

			// �θ� ����
			if (delNodeChild != &Nil)
			{
				delNodeChild->_parent = parent;
			}
		}
	}
	// �ڽ��� ���� ���
	else
	{
		st_Node* rightLowNodeParent = toDelNode; // �����ʿ��� ���� ���� ����� �θ�
		st_Node* rightLowNode = toDelNode->_right; // �����ʿ��� ���� ���� ���

		// �����ʿ��� ���� ���� ��带 ã��
		while (rightLowNode->_left != &Nil)
		{
			rightLowNodeParent = rightLowNode;
			rightLowNode = rightLowNode->_left;
		}

		// '������ �ڽ��� ���� ���� ���'�� ���� �ڽ��̾��� ���
		if (rightLowNode == rightLowNodeParent->_left)
		{
			// '������ �ڽ��� ���� ���� ���'�� ������ �ڽ��� �ڽ��� ��ġ��
			rightLowNodeParent->_left = rightLowNode->_right;
		}
		// '������ �ڽ��� ���� ���� ���'�� ������ �ڽ��̾��� ���
		else
		{
			rightLowNodeParent->_right = rightLowNode->_right;
		}

		// �θ� ����
		if (rightLowNode->_right != &Nil)
		{
			rightLowNode->_right->_parent = rightLowNodeParent;
		}

		// ���� ������ ��忡 ��ü�� ����� �� ����
		toDelNode->_value = rightLowNode->_value;

		// ��ü�� ����� �����ͷ� ����
		toDelNode = rightLowNode;
	}

	// ����
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

	// ��Ʈ�� ������ ��� ��Ʈ�� nullptr��
	if (param == g_rootNode)
	{
		g_rootNode = nullptr;
	}

	--g_nodeCount;

	return true;
}

st_Node * FindNode(int param)
{
	// �� Ʈ���� ���
	if (g_rootNode == nullptr || g_rootNode == &Nil)
	{
		return nullptr;
	}

	st_Node* foundNode = g_rootNode; // �ʱ� ���� ������(��Ʈ)

	// data�� ��ġ�ϴ� ��� ã�´�.
	while (foundNode != &Nil && foundNode->_value != param)
	{
		// ã������ �����Ͱ� ����� �����ͺ��� ���� ���
		if (param < foundNode->_value)
		{
			foundNode = foundNode->_left;
		}
		// ã������ �����Ͱ� ����� �����ͺ��� ū ���
		else
		{
			foundNode = foundNode->_right;
		}
	}

	// ��ã�� ���
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

	// ���� N
	st_Node* toRotateNode = param;

	// ���� N�� ���� �ڽ��� ����
	st_Node* rightChildLeft = toRotateNode->_right->_left;

	// N�� ��ġ�� ���� N�� �θ�� ��Ÿ��
	// �ϴ� N�� ������� �ڽ��̾�����

	// N�� ��Ʈ����� ���
	if (toRotateNode == g_rootNode)
	{
		g_rootNode = toRotateNode->_right;
		g_rootNode->_parent = nullptr;
	}
	else if (toRotateNode == toRotateNode->_parent->_left)
	{
		// N�� ��ġ = N�� �����ڽ�
		toRotateNode->_parent->_left = toRotateNode->_right;
		toRotateNode->_right->_parent = toRotateNode->_parent;
	}
	else
	{
		toRotateNode->_parent->_right = toRotateNode->_right;
		toRotateNode->_right->_parent = toRotateNode->_parent;
	}
	
	// N�� �����ڽ��� ���� = ���� N
	toRotateNode->_right->_left = toRotateNode;
	toRotateNode->_parent = toRotateNode->_right;

	// N�� ������ = ���� N�� �����ڽ��� �����ڽ�
	toRotateNode->_right = rightChildLeft;
	rightChildLeft->_parent = toRotateNode;
}

// TODO : nullptr ����ó�� ����
void rotateRight(st_Node * param)
{
	wcout << L"rotateRIGHT" << endl;

	// ���� N
	st_Node* toRotateNode = param;

	// ���� N�� ���� �ڽ��� ������
	st_Node* leftChildRight = toRotateNode->_left->_right;

	// N�� ��ġ�� ���� N�� �θ�� ��Ÿ��
	// �ϴ� N�� ������� �ڽ��̾�����
	// N�� ��Ʈ����� ���
	if (toRotateNode == g_rootNode)
	{
		g_rootNode = toRotateNode->_left;
		g_rootNode->_parent = nullptr;
	}
	else if (toRotateNode == toRotateNode->_parent->_left)
	{
		// N�� ��ġ = N�� �� �ڽ�
		toRotateNode->_parent->_left = toRotateNode->_left;
		toRotateNode->_left->_parent = toRotateNode->_parent;
	}
	else
	{
		toRotateNode->_parent->_right = toRotateNode->_left;
		toRotateNode->_left->_parent = toRotateNode->_parent;
	}

	// N�� ���� �ڽ��� ������ = ���� N
	toRotateNode->_left->_right = toRotateNode;
	toRotateNode->_parent = toRotateNode->_left;

	// N�� ���� = ���� N�� ���� �ڽ��� ������ �ڽ�
	toRotateNode->_left = leftChildRight;
	leftChildRight->_parent = toRotateNode;
}

// �Ҿƹ��� �������� �� �θ� ����
void grandLeftAlign(st_Node * param)
{
	st_Node* nowNode = param;

	while (1)
	{
		/* ////////////////////////////////////////////////
		�ű� ����� �θ� ���̶�� ���� ������ ����.
		�Ʒ��� ���� ��Ȳ�� �θ� ������ ��� �ش��.
		*/ ////////////////////////////////////////////////
		if (nowNode->_parent->_color == NODE_COLOR::BLACK)
		{
			wcout << L"nowNode's parent is BLACK : align END" << endl;
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
		//1. ���� �θ� ���̵� ����.
		st_Node* localParent = nowNode->_parent;
		if (nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_right->_color == NODE_COLOR::RED)
		{
			wcout << L"type 1. nowNode is " << nowNode->_value << endl;

			//	�θ�� ������ ������ �ٲٰ�, �Ҿƹ����� ����� �ٲ۴�.
			localParent->_color = NODE_COLOR::BLACK;
			localParent->_parent->_right->_color = NODE_COLOR::BLACK;
			localParent->_parent->_color = NODE_COLOR::RED;

			//	�Ҿƹ����� ����� �ٲ����� ���ؼ�, �Ҿƹ��� ����� ���� ��� �񱳸� �غ��ƾ� �Ѵ�.
			//	�׷��� �ᱹ ROOT ������ �ö󰡸鼭 �̸� �ݺ� Ȯ�� �ؾ��Ѵ�.
			//	�Ҿƹ����� �� ���� ��� �ٽ� Ȯ��.
			nowNode = localParent->_parent;

			// �� ��尡 ��Ʈ����̸� break
			if (nowNode == g_rootNode)
			{
				wcout << L"nowNode is rootNode.. Align End" << endl;
				break;
			}

			return;
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
		//2. ���� ��������, �θ� ����, ������ �� ��Ȳ.
		localParent = nowNode->_parent;
		if (nowNode == localParent->_right &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_right->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 2. nowNode is " << nowNode->_value << endl;

			//	���� ���ߴ°ɷ� ������ �ʰ�, ȸ���� ���ؼ� �뷱���� ���� �ش�.
			//	ȸ���� ���ؼ� ���� �Ǵ� ������ ���� �������� ��尡 �����ִ°� ����.
			//	�׷��Ƿ� �켱  3������ ��Ȳ���� �����.

			//	���� ����, �θ��� ������ ���, �θ� ����, �� ��쿡��
			//	�θ�������� ��ȸ���Ͽ� �űԳ�带 �θ��� ��ġ�� �ø���,
			//	�θ� �ų�� ���� �ڽ����� �Űܼ� �뷱���� �����.
			rotateLeft(localParent);

			//	�̷μ� 2�� ��Ȳ�� 3�� ��Ȳ���� ���� �Ǿ���.
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
		//3. ���� �޷���, �θ� ����, ������ ��
		localParent = nowNode->_parent;
		if (nowNode == localParent->_left &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_right->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 3. nowNode is " << nowNode->_value << endl;

			//	�θ� ������
			localParent->_color = NODE_COLOR::BLACK;

			//	�Ҿƹ����� �����
			localParent->_parent->_color = NODE_COLOR::RED;

			//	�Ҿƹ��� �������� ��ȸ��!
			rotateRight(localParent->_parent);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	
	// �������� ��Ʈ�� ��
	g_rootNode->_color = NODE_COLOR::BLACK;
}

// �Ҿƹ��� �������� �� �θ� ������
void grandRightAlign(st_Node * param)
{
	st_Node* nowNode = param;

	while (1)
	{
		/* ////////////////////////////////////////////////
		�ű� ����� �θ� ���̶�� ���� ������ ����.
		�Ʒ��� ���� ��Ȳ�� �θ� ������ ��� �ش��.
		*/ ////////////////////////////////////////////////
		if (nowNode->_parent->_color == NODE_COLOR::BLACK)
		{
			wcout << L"nowNode's parent is BLACK : align END" << endl;
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
		//1. ���� �θ� ���̵� ����.
		st_Node* localParent = nowNode->_parent;
		if (nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_left->_color == NODE_COLOR::RED)
		{
			wcout << L"type 1. nowNode is " << nowNode->_value << endl;

			//	�θ�� ������ ������ �ٲٰ�, �Ҿƹ����� ����� �ٲ۴�.
			localParent->_color = NODE_COLOR::BLACK;
			localParent->_parent->_left->_color = NODE_COLOR::BLACK;
			localParent->_parent->_color = NODE_COLOR::RED;

			//	�Ҿƹ����� ����� �ٲ����� ���ؼ�, �Ҿƹ��� ����� ���� ��� �񱳸� �غ��ƾ� �Ѵ�.
			//	�׷��� �ᱹ ROOT ������ �ö󰡸鼭 �̸� �ݺ� Ȯ�� �ؾ��Ѵ�.
			//	�Ҿƹ����� �� ���� ��� �ٽ� Ȯ��.
			nowNode = localParent->_parent;

			// �� ��尡 ��Ʈ����̸� break
			if (nowNode == g_rootNode)
			{
				wcout << L"nowNode is rootNode.. Align End" << endl;
				break;
			}

			return;
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
		//2. ���� �޷���, �θ� ����, ������ �� ��Ȳ.
		localParent = nowNode->_parent;
		if (nowNode == localParent->_left &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_left->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 2. nowNode is " << nowNode->_value << endl;

			//	���� ���ߴ°ɷ� ������ �ʰ�, ȸ���� ���ؼ� �뷱���� ���� �ش�.
			//	ȸ���� ���ؼ� ���� �Ǵ� ������ ���� �������� ��尡 �����ִ°� ����.
			//	�׷��Ƿ� �켱  3������ ��Ȳ���� �����.

			//	���� ����, �θ��� ���� ���, �θ� ����, �� ��쿡��
			//	�θ�������� ��ȸ���Ͽ� �űԳ�带 �θ��� ��ġ�� �ø���,
			//	�θ� �ų�� ������ �ڽ����� �Űܼ� �뷱���� �����.
			rotateRight(localParent);

			//	�̷μ� 2�� ��Ȳ�� 3�� ��Ȳ���� ���� �Ǿ���.
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
		//3. ���� ��������, �θ� ����, ������ ��
		localParent = nowNode->_parent;
		if (nowNode == localParent->_right &&
			nowNode->_color == NODE_COLOR::RED &&
			localParent->_color == NODE_COLOR::RED &&
			localParent->_parent->_left->_color == NODE_COLOR::BLACK)
		{
			wcout << L"type 3. nowNode is " << nowNode->_value << endl;

			//	�θ� ������
			localParent->_color = NODE_COLOR::BLACK;

			//	�Ҿƹ����� �����
			localParent->_parent->_color = NODE_COLOR::RED;

			//	�Ҿƹ��� �������� ��ȸ��!
			rotateLeft(localParent->_parent);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	// �������� ��Ʈ�� ��
	g_rootNode->_color = NODE_COLOR::BLACK;
}

void AlignInsert(int param)
{
	wcout << L"--------------------------------------------" << endl;

	// ��Ʈ�� ���ΰ� ?
	if (g_rootNode == nullptr)
	{
		g_rootNode = g_memPool.Alloc();
		g_rootNode->_value = param;
		g_rootNode->_color = NODE_COLOR::BLACK;

		wcout << L"inserted rootNode : " << g_rootNode->_value << L"no Align.." << endl;
		++g_nodeCount;

		// �ְ� ����
		return;
	}

	//��Ʈ���� ���� ��� �� ���� ū�� ������ �Ǵ��ؼ�
	st_Node* localNode = g_rootNode;
	st_Node* parentNode = nullptr;

	while (1)
	{
		if (localNode == &Nil)
		{
			st_Node* newNode = g_memPool.Alloc();
			newNode->_value = param;

			// �θ��� ���� ������� ������ �������
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
				// ���� ���
				g_memPool.Free(newNode);
				return;
			}

			wcout << L"inserted Node : " << param << endl;

			newNode->_parent = parentNode;
			++g_nodeCount;

			// �����ؼ� �־����� localNode�� newNode�� ����
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
			// ���� �� ���� ��� return
			return;
		}
	}

	// TODO : ���� �����ؾ���.. 141������ ���� �߻�
	// Ÿ�� �ö󰥶� �θ� �Ҿƹ����� ��� �������� ��� ���
	while (1)
	{
		// �θ� ���� ��� �ٷ� ����
		if (localNode->_parent->_color == NODE_COLOR::BLACK)
		{
			wcout << L"parent is BLACK.. NO Align" << endl;
			return;
		}

		if (localNode->_parent == localNode->_parent->_parent->_left)
		{
			wcout << L"parent is Grand's Left.. leftAlign" << endl;
			grandLeftAlign(localNode);
		}
		else
		{
			wcout << L"parent is Grand's Right.. RightAlign" << endl;
			grandRightAlign(localNode);
		}
	}

}
