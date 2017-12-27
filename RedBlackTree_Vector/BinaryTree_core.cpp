#include "stdafx.h"

#include "hoxy_Header.h"
#include "BinaryTree_core.h"
#include "MemoryPool_G41.h"

CMemoryPool<st_Node> g_memPool(100, true);
st_Node* g_rootNode;
int g_nodeCount;

st_Node Nil;	// �� �������. ������ �� / ������ �� / NULL �� ���� �뵵.

void InitTree(void)
{
	Nil._color = NODE_COLOR::BLACK;
	//g_rootNode = &Nil;
}

bool InsertNode(int param)
{
	// ��Ʈ�� ���ΰ� ?
	if (g_rootNode == nullptr)
	{
		g_rootNode = g_memPool.Alloc();
		g_rootNode->_value = param;

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
		if (localNode == nullptr)
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
	if (g_rootNode == nullptr)
	{
		return false;
	}

	st_Node* delParent = nullptr; // �ʱ� �θ� ������
	st_Node* delNode = g_rootNode; // �ʱ� ���� ������(��Ʈ)

	// data�� ��ġ�ϴ� ��� ã�´�.
	while (delNode != nullptr && delNode->_value != param)
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
	if (delNode == nullptr)
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
	if (toDelNode->_left == nullptr || toDelNode->_right == nullptr)
	{
		st_Node* delNodeChild = nullptr;

		// ������ ����� �ڽĳ��
		if (toDelNode->_left != nullptr)
		{
			// ���ʸ� �ִ� ���
			delNodeChild = toDelNode->_left;
		}
		else if (toDelNode->_right != nullptr)
		{
			// �����ʸ� �ִ� ���
			delNodeChild = toDelNode->_right;
		}
		// �ڽĳ�尡 ���� ���� nullptr

		// ������ ��Ʈ�� ��Ʈ����̸�
		if (toDelNode == g_rootNode)
		{
			g_rootNode = delNodeChild;

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

			if (delNodeChild != nullptr)
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
		while (rightLowNode->_left != nullptr)
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

		if (rightLowNode->_right != nullptr)
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
	if (param == nullptr)
	{
		return false;
	}

	ReleaseNode(param->_left);
	ReleaseNode(param->_right);

	g_memPool.Free(param);
	--g_nodeCount;

	return true;
}