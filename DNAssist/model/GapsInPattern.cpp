#include "GapsInPattern.h"

CGapsInPattern::CGapsInPattern(int number_of_gaps, int maximum_gap_size)
{
	combinations = 0;
	gaps = number_of_gaps;
	maxgapsize = maximum_gap_size;
}

CGapsInPattern::~CGapsInPattern()
{
}

///////////////////////////////////////////////////////////////////////////////
//
// Calculates the combinations of gaps of a given maximum length using a given
// fixed number of gap places.  The combinations are placed back in the array
// result, and the total number of combinations is returned by the function

int CGapsInPattern::MakeGaps(vector<vector<int> >& result)
{
	int perm = pow(maxgapsize + 1.0f, gaps);
	result.clear();
	result.resize(perm);
	for (int k = 0; k < perm; k++) {
		vector<int> row(gaps);
		result[k] = row;
	}

	for(int member = 0; member < gaps; member++) {
		int row = 0;
		do {
			int innerloop = perm / (int) pow(maxgapsize + 1.0f, member + 1);
			for(int gap = 0; gap <= maxgapsize; gap++) {
				for(int runlength = 0; runlength < innerloop; runlength++)
					result[row++][member] = gap;
			}
		} while(row < perm);
	}
	return perm;
}

///////////////////////////////////////////////////////////////////////////
//
// Calculates the possible combinations of a given number of balls distributed
// among a given number of holes.  The combinations are returned in the array
// result, and the function returns the total number of combinations

int CGapsInPattern::HoleCombinations(int sequencelength, vector<vector<int> >& result)
{
	int holes = sequencelength-1;
	if(gaps > holes)
		return 0;
	int permutations = 1;
	for(int q = holes; q > holes-gaps;q--)
		permutations *= q;
	for(int w = gaps; w > 0; w--)
		permutations /= w;

	result.clear();
	result.resize(permutations);
	for (int x = 0; x < permutations; x++) {
		vector<int> row(gaps);
		result[x] = row;
	}

	int loop = gaps - 1;
	int iteration = 0;
	auto grid = make_unique<int[]>(gaps);

	for(int ball_number = 1; ball_number < gaps; ball_number++)
		grid[ball_number] = grid[ball_number-1]+1;

	do {
		do {
			for(int s = 0; s < gaps-1; s++)
				result[iteration][s] = grid[s];
			result[iteration][loop] = grid[loop];
			grid[loop]++;
			iteration++;
		} while(grid[loop] <= holes-gaps+loop);

		do {
			loop--;
		} while ((loop >= 0) && (grid[loop] >= holes-gaps+loop));

		if(loop >= 0) {
			grid[loop]++;
			for(int t = loop+1; t < gaps; t++)
				grid[t]= grid[t-1]+1;
			loop = gaps-1;
		}

	} while (iteration < permutations);

	return permutations;
}

//////////////////////////////////////////////////////////////////////////////
//
// Calculates the permutations and combinations of a given number of gaps, each of
// a given maximum gaplength in a sequence of given length.  The possible combinations
// are sorted (leftmost biggest gap highest value) and duplicates removed.  The
// combinations are returned in the result array, and the function returns the total
// number of unique combinations.  The array is in the form:
//
//		entry	gap1	gap2	gap3 ...
//   	  0	2		1		0
//      1	0	  	2	  ...
//     ... ...   ...	  ...
//
// If there are 4bp in pattern sequence, there are 3 gap entries
//

int CGapsInPattern::PermuteSequence(const char* /*pattern*/, int length, vector<vector<int> >& result)
{
	if(gaps == 0) {
		result.clear();
		vector<int> row(length);
		result.push_back(move(row));
		return 1;
	}

	vector<vector<int> > gapcombs, holecombs;
	int gapcomb = MakeGaps(gapcombs);
	int holecomb = HoleCombinations(length, holecombs);
	int totalcombinations = gapcomb * holecomb;
	result.clear();
	result.resize(totalcombinations);
	for (int a = 0; a < totalcombinations; a++) {
		vector<int> row(length);
		result[a] = row;
	}

	for(int x = 0; x < holecomb; x++)
		for(int y = 0; y < gapcomb; y++)
			for(int z = 0; z < gaps; z++)
				result[x*gapcomb+y][holecombs[x][z]] = gapcombs[y][z];

	SortResult(totalcombinations, length-1, result);
	gapcombs.clear();
	holecombs.clear();
	return RemoveDuplicates(totalcombinations, length-1, result);
}


////////////////////////////////////////////////////////////////////////
//
// Sort the combinations with leftmost zero gap the highest value


void CGapsInPattern::SortResult(int entries, int width, vector<vector<int> >& result)
{
	auto temp = make_unique<int[]>(width);
	for(int outerloop = 1; outerloop <= entries-1; outerloop++) {
		//Display(entries, width, result);
		int innerloop = outerloop;
		while(innerloop >= 1 && CompareEntries(width, &result[innerloop][0], &result[innerloop-1][0]) > 0) {
			for(int x = 0; x < width; x++)
				temp[x] = result[innerloop][x];
			for(int z = 0; z < width; z++)
				result[innerloop][z] = result[innerloop-1][z];
			for(int y = 0; y < width; y++)
				result[innerloop-1][y] = temp[y];
			innerloop--;
		}
	}
}

/////////////////////////////////////////////////////////////////////////
//
// Compare two combination entries.


int CGapsInPattern::CompareEntries(int width, const int* test, const int* reference)
{
	for(int x = 0; x <= width-1; x++) {
		if(test[x] < reference[x])
			return 1;
		if(test[x] > reference[x])
			return -1;
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////
//
//  Remove duplicate entries in array of combinations

int CGapsInPattern::RemoveDuplicates(int entries, int width, vector<vector<int> >& result)
{
	int number = 1;
	vector<vector<int> > temp;
	temp.reserve(entries);
	for(int x = 0; x < entries; x++) {
		if(x == 0) {
			vector<int> row(width);
			for(int z = 0; z < width; z++)
				row[z] = result[x][z];
			temp.push_back(move(row));
		}
		else if(CompareEntries(width, &result[x][0], &temp[number - 1][0]) != 0) {
			vector<int> row(width);
			for(int p = 0; p < width; p++)
				row[p] = result[x][p];
			temp.push_back(move(row));
			number++;
		}
	}

	result = temp;
	return number;
}

//////////////////////////////////////////////
// data is the pattern string
//width is the number of gaps
//max is the maximum gapsize
//format point to an array of integers, i.e.
//gap0-gap1-gap2-gap3-...
//where there are one less gap entries than bp in the pattern string

void CGapsInPattern::FormatString(string& data, char* pad_character, const int* format)
{
	size_t length = data.length();
	int formatlength = 0;
	for(int x = 0; x < length-1; x++)
		formatlength += format[x];
	string temp = data;
	data = "";
	for(int y = 0; y < length-1; y++) {
		string pad = "";
		data += temp[y];
		for(int z = 0; z < format[y]; z++)
			pad += pad_character;
		data += pad;
	}
	data += temp[length-1];
}

int CGapsInPattern::FormatStringArray(string& data, int numberofcombinations,
														 vector<string>& stringarray, char* pad_character,
														 vector<vector<int> > result)
{
	stringarray.clear();
	stringarray.resize(numberofcombinations);
	for(int x = 0; x < numberofcombinations; x++) {
		FormatString(data, pad_character, &result[x][0]);
		stringarray[x] = data;
	}
	return numberofcombinations;
}
