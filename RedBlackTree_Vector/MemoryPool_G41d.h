#pragma once

/*---------------------------------------------------------------

메모리 풀 클래스 (오브젝트 풀)
특정 데이타(구조체,클래스,변수)를 일정량 할당 후 나눠쓴다.

- 사용법.

CMemoryPool<DATA> MemPool(300, false);
DATA *pData = MemPool.Alloc();

pData 사용

MemPool.Free(pData);

----------------------------------------------------------------*/

template <class DATA>
class CMemoryPool
{
private:
	// 구조체 sizeof 할 경우 패딩까지 같이 계산 됨. pragma pack 안해도 됨
	struct st_BLOCK_NODE
	{
		__int64 _code;
		st_BLOCK_NODE* _nextNode;
		DATA _block;
	};

public:
	//////////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters:	(int) 최대 블럭 개수.
	//				(bool) 생성자 호출 여부.
	// Return:
	//////////////////////////////////////////////////////////////////////////
	CMemoryPool(int iBlockNum, bool bPlacementNew = false);
	virtual	~CMemoryPool();

	//////////////////////////////////////////////////////////////////////////
	// 블럭 하나를 할당받는다.
	//
	// Parameters: 없음.
	// Return: (DATA *) 데이타 블럭 포인터.
	//////////////////////////////////////////////////////////////////////////
	DATA	*Alloc(void);

	//////////////////////////////////////////////////////////////////////////
	// 사용중이던 블럭을 해제한다.
	//
	// Parameters: (DATA *) 블럭 포인터.
	// Return: (BOOL) TRUE, FALSE.
	//////////////////////////////////////////////////////////////////////////
	bool	Free(DATA *pData);

	//////////////////////////////////////////////////////////////////////////
	// 현재 확보 된 블럭 개수를 얻는다. (메모리풀 내부의 전체 개수)
	//
	// Parameters: 없음.
	// Return: (int) 메모리 풀 내부 전체 개수
	//////////////////////////////////////////////////////////////////////////
	int		GetAllocCount(void) { return _allocCount; }

	//////////////////////////////////////////////////////////////////////////
	// 현재 사용중인 블럭 개수를 얻는다.
	//
	// Parameters: 없음.
	// Return: (int) 사용중인 블럭 개수.
	//////////////////////////////////////////////////////////////////////////
	int		GetUseCount(void) { return _useCount; }

private:
	int _useCount;
	int _allocCount;
	st_BLOCK_NODE* _topPtr;
	char* _poolBuf;
	bool _isPlacement;
	bool _isDynamic;
};

// 생성자. iBlockNum 이 0이면 동적(프리리스트)
template <class DATA>
CMemoryPool<DATA>::CMemoryPool(int iBlockNum, bool bPlacementNew)
	: _useCount(0), _allocCount(iBlockNum), _topPtr(nullptr),
	_poolBuf(nullptr), _isPlacement(bPlacementNew), _isDynamic(false)
{
	if (_allocCount == 0)
	{
		_isDynamic = true;
		return;
	}

	// 할당 한 다음 구조체[] 로 접근해서 노드정보만 초기화(new 안쓰고)
	_poolBuf = (char*)malloc(sizeof(st_BLOCK_NODE) * _allocCount);
	st_BLOCK_NODE* localNodePtr = (st_BLOCK_NODE*)_poolBuf;

	// plcaement 일 경우 초기화 X
	if (_isPlacement == true)
	{
		for (int i = 0; i < _allocCount; i++)
		{
			localNodePtr[i]._code = 0x2525252525252525;
			++_useCount;
		}
	}
	else
	{
		for (int i = 0; i < _allocCount; i++)
		{
			localNodePtr[i]._code = 0x2525252525252525;
			new (&localNodePtr[i]._block) DATA;
			++_useCount;
		}
	}

	// 싱글 리스트 연결
	for (int i = 0; i < _allocCount - 1; i++)
	{
		// <top>
		// localNodePtr[0] --> localNodePtr[1] --> localNodePtr[2] ...
		localNodePtr[i]._nextNode = &localNodePtr[i + 1];
	}

	// 끝의 nextNode는 nullptr
	localNodePtr[_allocCount - 1]._nextNode = nullptr;

	// 초기 topPtr은 첫 노드
	_topPtr = &localNodePtr[0];
}

template <class DATA>
CMemoryPool<DATA>::~CMemoryPool()
{
	free(_poolBuf);
}

template <class DATA>
DATA * CMemoryPool<DATA>::Alloc(void)
{
	if (_isDynamic == true)
	{
		// 프리리스트는 이미 할당한 것 중에 쓸수 있는게 있으면 빼서 쓰는 것.. top이 널인지 확인
		if (_topPtr == nullptr)
		{
			// top이 널이면 새로 동적 메모리 할당
			++_allocCount;
			++_useCount;

			char* newMem = (char*)malloc(sizeof(st_BLOCK_NODE));
			new (newMem) st_BLOCK_NODE;

			st_BLOCK_NODE* newNode = (st_BLOCK_NODE*)newMem;
			newNode->_code = 0x2525252525252525;
			newNode->_nextNode = nullptr;

			return &newNode->_block;
		}
	}

	// 노드가 아닌 블럭 리턴
	DATA* toReturnPtr = &(_topPtr->_block);

	// placement true일 경우
	if (_isPlacement == true)
	{
		new (toReturnPtr) DATA;
	}

	// topPtr 다음 노드로
	_topPtr = _topPtr->_nextNode;

	// 사용중인 사이즈 증가
	++_useCount;

	// 오브젝트 반환
	return toReturnPtr;
}

template <class DATA>
bool CMemoryPool<DATA>::Free(DATA * pData)
{
	// 블럭 포인터로 노드 포인터 구한다.
	st_BLOCK_NODE* toFreeNode = (st_BLOCK_NODE*)((char*)pData - (sizeof(__int64) + sizeof(st_BLOCK_NODE*)));

	// 코드 체크(내가 만든 오브젝트 인지)
	if (toFreeNode->_code != 0x2525252525252525)
	{
		return false;
	}

	// placement new 옵션 true이면 Free할때도 명시적 소멸자 호출
	if (_isPlacement == true)
	{
		pData->~DATA();
	}

	// 새로 회수되는 노드의 next는 기존 topPtr
	toFreeNode->_nextNode = _topPtr;

	// topPtr 갱신
	_topPtr = toFreeNode;

	// 사용중인 사이즈 감소
	--_useCount;

	return true;
}