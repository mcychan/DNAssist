#include "ProteinProperties.h"
#include <iomanip>

CProteinProperties::CProteinProperties(const string& sequence)
{
	proteinsequence = sequence;
	aminoacidlist = convertobject.GetListOfAminoAcids(aminoacidlist);
	numberofcommonaminoacids = aminoacidlist.length();
	molecularweight = make_unique<float[]>(numberofcommonaminoacids);
}

CProteinProperties::~CProteinProperties()
{
}

size_t* CProteinProperties::GetNumberOfAminoAcidResidues()
{
	numberofaminoacidresidues = make_unique<size_t[]>(numberofcommonaminoacids);
	for(int x = 0; x < numberofcommonaminoacids; x++)
		numberofaminoacidresidues[x] = GetNumberOfSpecificResidues(aminoacidlist[x]);
	return numberofaminoacidresidues.get();
}

size_t* CProteinProperties::GetMolePercentageAminoAcidResidues()
{
	if(!numberofaminoacidresidues.get())
		GetNumberOfAminoAcidResidues();

	molepercentageaminoacidresidues = make_unique<size_t[]>(numberofcommonaminoacids);
	for(int x = 0; x < numberofcommonaminoacids; x++)
		molepercentageaminoacidresidues[x] = (100*numberofaminoacidresidues[x])/ proteinsequence.length();
	return molepercentageaminoacidresidues.get();
}

string CProteinProperties::GetMolarAbsoptivityString(string& data)
{
	int absorptivity = GetMolarAbsorptivity();
	if(!absorptivity)
		data = "Incalculable";
	else
		data = to_string(absorptivity) + " AU/M/cm.";
	return data;
}

string CProteinProperties::GetOnePercentSolutionAbsorptivityString(string& data)
{
	float MW = GetMolecularWeight();
	float MAU = (float)GetMolarAbsorptivity();
	float E01;
	if((bool)MW)
		E01 = MAU/MW;
	else
		E01 = 0;

	ostringstream out;
	if(!(bool)E01)
		out << "Incalculable";
	else
		out << setprecision(2) << E01 << " AU/mg/ml/cm";
	data = out.str();
	return data;
}

string CProteinProperties::GetMolecularWeightString(string& data)
{
	int weight = (int) GetMolecularWeight();
	if(weight > 10000)
		data = to_string((weight+500)/1000) + " kD";
	else
		data = to_string(weight) + " D";
	return data;
}

string CProteinProperties::GetpIString(string& data)
{
	float pI = GetpI();
	ostringstream out;
	if(!(bool)pI || pI == 2)
		out << "Incalculable";
	else
		out << setprecision(1) << pI;
	data = out.str();
	return data;
}

string CProteinProperties::GetChargeAtpHString(string& data)
{
	float charge = GetChargeAtpH();
	ostringstream out;
	if(!(bool)charge)
		out << "Incalculable";
	else
		out << setprecision(1) << charge;
	data = out.str();
	return data;
}


int CProteinProperties::GetAminoAcidIndex(char aa)
{
	int x = 0;
	while(x < numberofcommonaminoacids && aminoacidlist[x] != aa)
		x++;
	return (x < numberofcommonaminoacids) ? x : -1;
}

int CProteinProperties::GetMolarAbsorptivity()
{
	//	Calculate with the algorithm of Pace et al. (1995) Protein Science, 4, 2411-2423.

	if(!numberofaminoacidresidues.get())
		GetNumberOfAminoAcidResidues();
	int absoptivity, residues;

	//	Get the number of tryptophans
	residues = numberofaminoacidresidues[GetAminoAcidIndex('W')];
	absoptivity = 5500 * residues;

	//	Get the number of tyrosines
	residues = numberofaminoacidresidues[GetAminoAcidIndex('Y')];
	absoptivity += 1490 * residues;

	//	Get the number of tryptophans
	residues = numberofaminoacidresidues[GetAminoAcidIndex('C')];
	absoptivity += 63*residues;
	return absoptivity;
}

int CProteinProperties::GetOnePercentSolutionAbsorptivity()
{
	return 0;
}

float CProteinProperties::GetMolecularWeight()
{
	if(!numberofaminoacidresidues.get())
		GetNumberOfAminoAcidResidues();
	molecularweight[GetAminoAcidIndex('A')] = 89.09f;
	molecularweight[GetAminoAcidIndex('R')] = 174.2f;
	molecularweight[GetAminoAcidIndex('N')] = 132.1f;
	molecularweight[GetAminoAcidIndex('D')] = 133.1f;
	molecularweight[GetAminoAcidIndex('C')] = 121.12f;
	molecularweight[GetAminoAcidIndex('Q')] = 146.15f;
	molecularweight[GetAminoAcidIndex('E')] = 147.13f;
	molecularweight[GetAminoAcidIndex('G')] = 75.07f;
	molecularweight[GetAminoAcidIndex('H')] = 155.16f;
	molecularweight[GetAminoAcidIndex('I')] = 131.17f;
	molecularweight[GetAminoAcidIndex('L')] = 131.17f;
	molecularweight[GetAminoAcidIndex('K')] = 146.19f;
	molecularweight[GetAminoAcidIndex('M')] = 149.21f;
	molecularweight[GetAminoAcidIndex('F')] = 165.19f;
	molecularweight[GetAminoAcidIndex('P')] = 115.13f;
	molecularweight[GetAminoAcidIndex('S')] = 105.09f;
	molecularweight[GetAminoAcidIndex('T')] = 119.12f;
	molecularweight[GetAminoAcidIndex('W')] = 204.22f;
	molecularweight[GetAminoAcidIndex('Y')] = 181.19f;
	molecularweight[GetAminoAcidIndex('V')] = 117.15f;

	float weight = 0.0f;
	for(int x = 0; x < numberofcommonaminoacids; x++)
		weight += numberofaminoacidresidues[x]*molecularweight[x];
	weight -= (proteinsequence.length() - 1) * 18;
	return weight;
}

float  CProteinProperties::GetChargeAtpH()
{
	//	Reference: Bjellqvist, B. et al. (1994) Electrophoresis, 15, 529-539

	//	Make an array with the number of internal residues for each amino acid
	if(!numberofaminoacidresidues.get())
		GetNumberOfAminoAcidResidues();
	auto numberinternalresidues = make_unique<int[]>(numberofcommonaminoacids);
	for(int residue = 0; residue < numberofcommonaminoacids; residue++)
		numberinternalresidues[residue] = numberofaminoacidresidues[residue];
	char Ntermres = proteinsequence[0];
	char Ctermres = proteinsequence[proteinsequence.length() - 1];
	int indexNterm = GetAminoAcidIndex(Ntermres);
	if(numberinternalresidues[indexNterm])
		numberinternalresidues[indexNterm]--;
	int indexCterm = GetAminoAcidIndex(Ctermres);
	if(numberinternalresidues[indexCterm])
		numberinternalresidues[indexCterm]--;

	//	Set up the array of pKa values for the amino acids that
	// will be charged at internal positions in the protein
	auto internal = make_unique<float[]>(numberofcommonaminoacids);
	internal[GetAminoAcidIndex('D')] = 4.05f;
	internal[GetAminoAcidIndex('E')] = 4.45f;
	internal[GetAminoAcidIndex('H')] = 5.98f;
	internal[GetAminoAcidIndex('C')] = 9.0f;
	internal[GetAminoAcidIndex('Y')] = 10.0f;
	internal[GetAminoAcidIndex('K')] = 10.0f;
	internal[GetAminoAcidIndex('R')] = 12.0f;

	//	Set up the array of pKa values for the amino acids that
	// will be charged at the C-terminal positions in the protein
	vector<float> C_terminal(numberofcommonaminoacids, 3.55f);
	C_terminal[GetAminoAcidIndex('D')] = 4.55f;
	C_terminal[GetAminoAcidIndex('E')] = 4.75f;

	//	Set up the array of pKa values for the amino acids that
	// will be charged at the N-terminal position in the protein
	vector<float> N_terminal(numberofcommonaminoacids, 7.5f);
	N_terminal[GetAminoAcidIndex('A')] = 7.59f;
	N_terminal[GetAminoAcidIndex('M')] = 7.00f;
	N_terminal[GetAminoAcidIndex('S')] = 6.93f;
	N_terminal[GetAminoAcidIndex('P')] = 8.36f;
	N_terminal[GetAminoAcidIndex('T')] = 6.82f;
	N_terminal[GetAminoAcidIndex('V')] = 7.44f;
	N_terminal[GetAminoAcidIndex('E')] = 7.70f;

	//	Set up the array of the charge of amino acids at pH values
	//	below the pKa values
	vector<int> charge(numberofcommonaminoacids, 7);
	charge[GetAminoAcidIndex('A')] = 0;
	charge[GetAminoAcidIndex('R')] = 1;
	charge[GetAminoAcidIndex('N')] = 0;
	charge[GetAminoAcidIndex('D')] = -1;
	charge[GetAminoAcidIndex('C')] = -1;
	charge[GetAminoAcidIndex('Q')] = 0;
	charge[GetAminoAcidIndex('E')] = -1;
	charge[GetAminoAcidIndex('G')] = 0;
	charge[GetAminoAcidIndex('H')] = 1;
	charge[GetAminoAcidIndex('I')] = 0;
	charge[GetAminoAcidIndex('L')] = 0;
	charge[GetAminoAcidIndex('K')] = 1;
	charge[GetAminoAcidIndex('M')] = 0;
	charge[GetAminoAcidIndex('F')] = 0;
	charge[GetAminoAcidIndex('P')] = 0;
	charge[GetAminoAcidIndex('S')] = 0;
	charge[GetAminoAcidIndex('T')] = 0;
	charge[GetAminoAcidIndex('W')] = 0;
	charge[GetAminoAcidIndex('Y')] = -1;
	charge[GetAminoAcidIndex('V')] = 0;

	//	Declare array that will have the charge of the protein at the different
	//	pH values.

	auto nettcharge = make_unique<float[]>(101);

	float hydrogen_ion;
	float K;
	float pH;
	float fractional_charge;
	float fraction;
	for(int x = 20; x <= 120; x++) {
		pH = ((float)x)/ 10.0f;
		hydrogen_ion = pow(10, pH);
		//	Calculate the charge for each internal amino acid
		for(int y = 0; y < 20; y++) {
			//	Calculate fractional charge only for amino acids that
			//	will be charged
			if(internal[y] && numberinternalresidues[y]) {
				K = pow(10.0f, internal[y]);
				fraction = hydrogen_ion/(hydrogen_ion + K);
				fractional_charge = charge[y] <= 0 ? charge[y]*fraction : charge[y]*(1 - fraction);
				nettcharge[x - 20] += fractional_charge*numberinternalresidues[y];
			}
		}
		//	Still need to add charges for N-terminal and C-terminal residues
		K = pow(10.0f, N_terminal[indexNterm]);
		fraction = hydrogen_ion/(hydrogen_ion + K);
		fractional_charge = charge[indexNterm] <= 0 ? charge[indexNterm]*fraction : charge[indexNterm]*(1 - fraction);
		nettcharge[x - 20] += fractional_charge;
		K = pow(10.0f, C_terminal[indexCterm]);
		fraction = hydrogen_ion/(hydrogen_ion + K);
		fractional_charge = charge[indexCterm] <= 0 ? charge[indexCterm]*fraction : charge[indexCterm]*(1 - fraction);
		nettcharge[x - 20] += fractional_charge;
	}

	//	Now find the charge at pH 7.4
	// i.e. index 74-20
	float charge_at_74 = nettcharge[74-20];
	return charge_at_74;
}

float CProteinProperties::GetpI()
{
	//	Reference: Bjellqvist, B. et al. (1994) Electrophoresis, 15, 529-539

	//	Make an array with the number of internal residues for each amino acid
	if(!numberofaminoacidresidues.get())
		GetNumberOfAminoAcidResidues();
	auto numberinternalresidues = make_unique<int[]>(numberofcommonaminoacids);
	for(int residue = 0; residue < numberofcommonaminoacids; residue++)
		numberinternalresidues[residue] = numberofaminoacidresidues[residue];
	char Ntermres = proteinsequence[0];
	char Ctermres = proteinsequence[proteinsequence.length()-1];
	int indexNterm = GetAminoAcidIndex(Ntermres);
	if(numberinternalresidues[indexNterm])
		numberinternalresidues[indexNterm]--;
	int indexCterm = GetAminoAcidIndex(Ctermres);
	if(numberinternalresidues[indexCterm])
		numberinternalresidues[indexCterm]--;

	//	Set up the array of pKa values for the amino acids that
	// will be charged at internal positions in the protein
	auto internal = make_unique<float[]>(numberofcommonaminoacids);
	internal[GetAminoAcidIndex('D')] = 4.05f;
	internal[GetAminoAcidIndex('E')] = 4.45f;
	internal[GetAminoAcidIndex('H')] = 5.98f;
	internal[GetAminoAcidIndex('C')] = 9.0f;
	internal[GetAminoAcidIndex('Y')] = 10.0f;
	internal[GetAminoAcidIndex('K')] = 10.0f;
	internal[GetAminoAcidIndex('R')] = 12.0f;

	//	Set up the array of pKa values for the amino acids that
	// will be charged at the C-terminal positions in the protein
	vector<float> C_terminal(numberofcommonaminoacids, 3.55f);
	C_terminal[GetAminoAcidIndex('D')] = 4.55f;
	C_terminal[GetAminoAcidIndex('E')] = 4.75f;

	//	Set up the array of pKa values for the amino acids that
	// will be charged at the N-terminal position in the protein
	vector<float> N_terminal(numberofcommonaminoacids, 7.5f);
	N_terminal[GetAminoAcidIndex('A')] = 7.59f;
	N_terminal[GetAminoAcidIndex('M')] = 7.00f;
	N_terminal[GetAminoAcidIndex('S')] = 6.93f;
	N_terminal[GetAminoAcidIndex('P')] = 8.36f;
	N_terminal[GetAminoAcidIndex('T')] = 6.82f;
	N_terminal[GetAminoAcidIndex('V')] = 7.44f;
	N_terminal[GetAminoAcidIndex('E')] = 7.70f;

	//	Set up the array of the charge of amino acids at pH values
	//	below the pKa values
	vector<int> charge(numberofcommonaminoacids, 7);
	charge[GetAminoAcidIndex('A')] = 0;
	charge[GetAminoAcidIndex('R')] = 1;
	charge[GetAminoAcidIndex('N')] = 0;
	charge[GetAminoAcidIndex('D')] = -1;
	charge[GetAminoAcidIndex('C')] = -1;
	charge[GetAminoAcidIndex('Q')] = 0;
	charge[GetAminoAcidIndex('E')] = -1;
	charge[GetAminoAcidIndex('G')] = 0;
	charge[GetAminoAcidIndex('H')] = 1;
	charge[GetAminoAcidIndex('I')] = 0;
	charge[GetAminoAcidIndex('L')] = 0;
	charge[GetAminoAcidIndex('K')] = 1;
	charge[GetAminoAcidIndex('M')] = 0;
	charge[GetAminoAcidIndex('F')] = 0;
	charge[GetAminoAcidIndex('P')] = 0;
	charge[GetAminoAcidIndex('S')] = 0;
	charge[GetAminoAcidIndex('T')] = 0;
	charge[GetAminoAcidIndex('W')] = 0;
	charge[GetAminoAcidIndex('Y')] = -1;
	charge[GetAminoAcidIndex('V')] = 0;

	//	Declare array that will have the charge of the protein at the different
	//	pH values.

	auto nettcharge = make_unique<float[]>(101);

	float hydrogen_ion;
	float K;
	float pH;
	float fractional_charge;
	float fraction;
	for(int x = 20; x <= 120; x++) {
		pH = (float)x / 10.0f;
		hydrogen_ion = pow(10.0f, pH);
		//	Calculate the charge for each internal amino acid
		for(int y = 0; y < 20; y++) {
			//	Calculate fractional charge only for amino acids that
			//	will be charged
			if(internal[y] && numberinternalresidues[y]) {
				K = pow(10.0f, internal[y]);
				fraction = hydrogen_ion/(hydrogen_ion + K);
				fractional_charge = charge[y] <= 0 ? charge[y]*fraction : charge[y]*(1 - fraction);
				nettcharge[x - 20] += fractional_charge*numberinternalresidues[y];
			}
		}
		//	Still need to add charges for N-terminal and C-terminal residues
		K = pow(10.0f, N_terminal[indexNterm]);
		fraction = hydrogen_ion/(hydrogen_ion + K);
		fractional_charge = charge[indexNterm] <= 0 ? charge[indexNterm]*fraction : charge[indexNterm]*(1 - fraction);
		nettcharge[x - 20] += fractional_charge;
		K = pow(10.0f, C_terminal[indexCterm]);
		fraction = hydrogen_ion/(hydrogen_ion + K);
		fractional_charge = charge[indexCterm] <= 0 ? charge[indexCterm]*fraction : charge[indexCterm]*(1 - fraction);
		nettcharge[x - 20] += fractional_charge;
	}

	//	Now find the two values bracketing 0;
	int z = 0;
	while((z < 100) && (nettcharge[z] - nettcharge[z+1] < nettcharge[z]))
		z++;
	float pI;
	if(z >= 100)
		pI = 0;
	else
		pI = (float)(20+z)/ 10.0f;

	return pI;
}

int CProteinProperties::GetNumberOfSpecificResidues(char residue)
{
	int count = 0;
	for(int y = 0; y < proteinsequence.length(); y++) {
		if(proteinsequence[y] == residue)
			count++;
	}
	return count;
}

string CProteinProperties::GetListOfCommonAminoAcids()
{
	return listofaminoacids;
}

string CProteinProperties::GetNumberOfResiduesString(int index, string& data)
{
	if(!numberofaminoacidresidues.get())
		GetNumberOfAminoAcidResidues();
	data = to_string(numberofaminoacidresidues[index]);
	return data;
}

string CProteinProperties::GetMolePercentOfResiduesString(int index, string& data)
{
	if(!molepercentageaminoacidresidues.get())
		GetMolePercentageAminoAcidResidues();
	data = to_string(molepercentageaminoacidresidues[index]) + "%";
	return data;
}

int CProteinProperties::GetNumberOfCommonAminoAcids()
{
	return numberofcommonaminoacids;
}
