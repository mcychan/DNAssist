#include "DocVarManager.h"

CDocVarManager::CDocVarManager(size_t top)
{
	limit = top;
}

void CDocVarManager::AddToUndoList(int type, const string& data, int write_position, int selection_start, int selection_extent)
{
	if(data.empty())
		return;

	if((undoList.size() + 1 > limit) && !undoList.empty())
		undoList.erase(undoList.begin());

	undoList.emplace_back(make_shared<DocVar>(type, data, write_position, selection_start, selection_extent));
}

DocVar* CDocVarManager::TakeFromUndoList()
{
	if(undoList.empty())
		return NULL;

	auto pDocVar = undoList.back();
	undoList.pop_back();
	PutInRedoOperation(pDocVar);
	return pDocVar.get();
}

DocVar* CDocVarManager::GetFromUndoList()
{
	if(undoList.empty())
		return NULL;

	auto pDocVar = undoList.back();
	PutInRedoOperation(pDocVar);
	return pDocVar.get();
}

void CDocVarManager::PutInRedoOperation(shared_ptr<DocVar> pDocVar)
{
	redoList.push_back(move(pDocVar));
}

DocVar* CDocVarManager::GetRedoOperation()
{
	if(redoList.empty())
		return NULL;

	auto pDocVar = redoList.back();
	redoList.pop_back();
	undoList.push_back(move(pDocVar));
	return pDocVar.get();
}
