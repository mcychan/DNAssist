#pragma once

#include <memory>
#include <string>
#include <vector>
using namespace std;

struct DocVar
{
	int type;
	string data;
	int writePosition;
	int selectionStart;
	int selectionExtent;

	DocVar(int dataType, const string& seq, int write_position, int selection_start, int selection_extent) :
		type(dataType), data(seq), writePosition(write_position), selectionStart(selection_start), selectionExtent(selection_extent) {
	}
};

class CDocVarManager
{
public:
	CDocVarManager(size_t top = 10);
	void AddToUndoList(int type, const string& data, int writeposition, int selectionstart, int selectionextent);
	DocVar* TakeFromUndoList();
	DocVar* GetFromUndoList();
	void PutInRedoOperation(shared_ptr<DocVar> pDocVar);
	DocVar* GetRedoOperation();
	inline bool CanRedo() const {
		return !redoList.empty();
	}
	inline bool CanUndo() const {
		return !undoList.empty();
	}
	inline void Clear() {
		undoList.clear();
		redoList.clear();
	}

protected:
	vector<shared_ptr<DocVar> > undoList;
	vector<shared_ptr<DocVar> > redoList;
	size_t limit;
};
