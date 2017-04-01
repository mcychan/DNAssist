#pragma once

#include <algorithm>
#include <cstring>
#include <cctype>
#include <fstream>      // std::ifstream, std::ofstream
#include <memory>
#include <sstream>
#include <vector>
using namespace std;

#define MAXLINE 512			/* Max. line length */
#define MAXNAMES		30	/* Max chars read for seq. names */
#define MAXTITLES		60      /* Title length */
#define FILENAMELEN 	256             /* Max. file name length */

#define UNKNOWN   0
#define EMBLSWISS 1
#define PIR 	  2
#define PEARSON   3
#define GDE    	  4
#define CLUSTAL   5	/* DES */
#define MSF       6 /* DES */
#define RSF       7	/* JULIE */
#define USER      8	/* DES */
#define PHYLIP    9	/* DES */

#define NONE      0
#define SECST     1
#define GMASK     2

#define PROFILE 0
#define SEQUENCE 1

#define BS_NODE_LABELS 2
#define BS_BRANCH_LABELS 1

#define PAGE_LEN       22   /* Number of lines of help sent to screen */

#define PAGEWIDTH	80  /* maximum characters on output file page */
#define LINELENGTH     	60  /* Output file line length */
#define GCG_LINELENGTH 	50

#ifdef VMS						/* Defaults for VAX VMS */
#define COMMANDSEP '/'
#define DIRDELIM ']'		/* Last character before file name in full file
							   specs */
#define INT_SCALE_FACTOR 1000 /* Scaling factor to convert float to integer for profile scores */

#elif MAC
#define COMMANDSEP '/'
#define DIRDELIM ':'
#define INT_SCALE_FACTOR 100  /* Scaling factor to convert float to integer for profile scores */

#elif UNIX
#define COMMANDSEP '-'
#define DIRDELIM '/'
#define INT_SCALE_FACTOR 1000 /* Scaling factor to convert float to integer for profile scores */

#else
#define COMMANDSEP '/'
#define DIRDELIM '\\'
#define INT_SCALE_FACTOR 100  /* Scaling factor to convert float to integer for profile scores */
#endif

#define NUMRES 32		/* max size of comparison matrix */

#define INPUT 0
#define ALIGNED 1

#define LEFT 1
#define RIGHT 2

#define NODE 0
#define LEAF 1

#define GAPCOL 32		/* position of gap open penalty in profile */
#define LENCOL 33		/* position of gap extension penalty in profile */

#define ENDALN 127

#define gap(k)  ((k) <= 0 ? 0 : g + gh * (k))
#define tbgap(k)  ((k) <= 0 ? 0 : tb + gh * (k))
#define tegap(k)  ((k) <= 0 ? 0 : te + gh * (k))

typedef struct node : public enable_shared_from_this<node> {		/* phylogenetic tree structure */
	shared_ptr<node> left;
	shared_ptr<node> right;
	node* parent;
	float dist = 0.0f;
	int leaf = 0;
	int order = 0;
	string name;
} *treeptr;

class CMultAlignment
{
private:

	bool check_tree = true, distance_tree;
	bool explicit_dnaflag, isEmpty;
	bool output_nbrf, output_phylip, output_gcg, output_gde;
	bool output_tree_clustal, output_tree_phylip, output_tree_distances;
	bool percent, profile1_empty, profile2_empty;   /* whether or not profiles   */
	bool usemenu, interactive, kimura, lowercase;
	bool neg_matrix, new_tree_file = false, nhyd_pen, npref_pen, quick_pairalign;
	bool reset_alignments, rooted_tree, showaln, save_parameters, switch_profiles, tossgaps;
	bool use_ss1, use_ss2, use_tree_file = false, verbose;
	int alignment_len, boot_ntrials, bootstrap_format, curr_frag, dna_wind_gap, dna_ktup, dna_window, dna_signif, first_seq;
	int g, gh, gapcoef1, gapcoef2, gap_pos1, gap_pos2, gdist;
	int helix_end_minus, helix_end_plus, helix_penalty, helix_end_penalty;
	int int_scale, last_print, last_seq, lencoef1, lencoef2, loop_penalty, mat_avscore, max_aa, max_aln_length, max_names, maxsf, maxscore;
	int new_seq, next_frag, nseqs, nseqs1, nseqs2, nsets;
	int numseq, nnodes, ntotal, output_order, output_struct_penalties;
	int print_ptr, prf_length1, prf_length2, profile_no, profile1_nseqs, prot_wind_gap,prot_ktup,prot_window, prot_signif;
	int sb1, sb2, se1, se2, seq1, seq2, seqFormat;
	int strand_end_plus = 1, strand_penalty, strand_end_penalty, struct_penalties, struct_penalties1, struct_penalties2, strand_end_minus;
	int vatend, wind_gap, ktup, window, signif;	
	size_t boot_ran_seed;	
	float dna_gap_open, dna_gap_extend;
	float prot_gap_open, prot_gap_extend;
	float pw_go_penalty, pw_ge_penalty;
	float dna_pw_go_penalty, dna_pw_ge_penalty;
	float prot_pw_go_penalty, prot_pw_ge_penalty, mm_score, reduced_gap = 0.3f;

	string aln_path1, aln_path2;
	string chartab, gap_penalty_mask, gap_penalty_mask1, gap_penalty_mask2;
	string lin1, lin2, lin3;
	string pw_mtrxname;
	string profile1_name, profile2_name;
	string sec_struct_mask, sec_struct_mask1, sec_struct_mask2;
	string ss_name, ss_name1, ss_name2, slopes;
	string seqname, tree_gaps;
	wstring clustal_outname, gcg_outname, outfile_name, phylip_name, phylip_outname, nbrf_outname, gde_outname;	

	unique_ptr<short[]> aa_xref;
	unique_ptr<short[]> def_aa_xref, def_dna_xref, dna_xref;
	unique_ptr<short[]> pw_aa_xref, pw_dna_xref, pw_userdnamat, userdnamat;
	unique_ptr<int[]> aln_len, boot_positions, boot_totals, tkill;
	unique_ptr<int[]> displ, diag_index, seq_weight, zza, zzb, zzc, zzd, DD, HH, RR, SS, gS;
	unique_ptr<double[]> av, left_branch, right_branch, save_left_branch, save_right_branch;
	vector<int> seqlen_array, output_index;	
	vector<string> alignment, seq_array, names, titles;
	treeptr root;
	unique_ptr<treeptr[]> lptr, nptr, olptr, ptrs;
	short usermat[NUMRES][NUMRES], pw_usermat[NUMRES][NUMRES];
	vector<vector<int> > accum, profile1, profile2, matrix, sets;
	vector<vector<double> > tmat;

	ifstream fd, fin;
	ofstream clustal_phy_tree_file, clustal_outfile, distances_phy_tree_file, gcg_outfile, gde_outfile, nbrf_outfile, phylip_outfile, phylip_phy_tree_file;

	int count_gaps(int s1, int s2, int l);
	void create_parameter_output();
	int readseqs(int first_seq);
	void reset_align();
	short align(wstring& phylip_name);
	inline short multiple_align_menu()
	{
		return align(phylip_name) ? 1 : 0;
	}	

	/* alnscore.c */
	void aln_score();
	/* interface.c */
	void init_interface();	
	bool open_output_file(ofstream& prompt, const wstring& path, wstring& file_name, const string& file_extension);
	bool open_explicit_file(ofstream&, const wstring& );
	int seq_input(bool);
	bool open_alignment_output(const wstring& );
	void create_alignment_output(int fseq,int lseq);
	void profile_align(string& p1_tree_name,string& p2_tree_name);/* Align 2 alignments */
	void make_tree(wstring& phylip_name);
	void get_tree(wstring& phylip_name);
	int profile_input();                        /* read a profile */
	void new_sequence_align(wstring& phylip_name);
	bool user_mat(string& , short *, short *);
	void clustal_out(vector<string>&, int*, int, int, int, int);
	void nbrf_out(ofstream& , int, int, int, int);
	void gcg_out(ofstream& , int, int, int, int);
	void phylip_out(ofstream& , int, int, int, int);
	void gde_out(ofstream& , int, int, int, int);
	void print_sec_struct_mask(int prf_length, const string& mask, string& struct_mask);
	void fix_gaps();

	/* calctree.c */
	void calc_seq_weights(int first_seq, int last_seq, int* sweight);
	void create_sets(int first_seq, int last_seq);
	int read_tree(wstring& treefile, int first_seq, int last_seq);
	void clear_tree(treeptr p);
	int calc_similarities(int nseqs);
	/* gcgcheck.c */
	int SeqGCGCheckSum(string& seq, int len);
	/* malign.c */
	int malign(int istart, wstring& phylip_name);
	int seqalign(int istart, wstring& phylip_name);
	int palign1();
	float countid(int s1, int s2);
	int palign2(wstring& p1_tree_name, wstring& p2_tree_name);
	/* pairalign.c */
	int pairalign(int istart, int iend, int jstart, int jend);
	/* prfalign.c */
	int prfalign(int *group, int *aligned);
	/* random.c */
	void addrandinit(unsigned long s);
	/* readmat.c */
	void init_matrix();
	int get_matrix(const short* matptr, short* xref, vector<vector<int> >& matrix, bool neg_flag,
	                int scale);
	int read_user_matrix(const string& filename, short *usermat, short *xref);
	int getargs(const string& inline1, vector<string>& args, int max);
	/* sequence.c */
	void fill_chartab();
	/* showpair.c */
	void show_pair();
	/* trees.c */
	void phylogenetic_tree(wstring& phylip_name,wstring& clustal_name,wstring& dist_name);
	void bootstrap_tree(wstring& phylip_name,wstring& clustal_name);
	int dna_distance_matrix(ofstream& tree);
	int prot_distance_matrix(ofstream& tree);
	void guide_tree(int first_seq, int nseqs);

	/* util.c */

	void alloc_aln(int nseqs);
	void realloc_aln(int first_seq,int nseqs);
	void free_aln(int nseqs);
	void alloc_seq(int seq_no,int length);
	void realloc_seq(int seq_no,int length);
	void free_seq(int seq_no);

	string& blank_to_(string& str);
	bool blankline(string& line);
	void get_path(const string& str, string& path);
	void n_encode(const string& seq, string& naseq, int l);
	void p_encode(const string& seq, string& naseq, int l);
	void get_clustal_ss(int length);
	void get_gde_ss(int length);
	bool blankline(const string& line);
	bool linetype(const string& line, const string& code);
	void get_swiss_feature(const string& line);
	void get_swiss_mask(const string& line);
	void get_embl_ss(int length);
	bool keyword(const string& line, const string& code);
	void get_rsf_feature(const string& line);
	void get_rsf_ss(int length);
	void reset_prf1();
	void reset_prf2();
	void calc_p_penalties(vector<string>& aln, int n, int fs, int ls, int *weight);
	void calc_h_penalties(vector<string>& aln, int n, int fs, int ls, int *weight);
	int local_penalty(int penalty, int n, int *pweight, int *hweight);

	/* calcgapcoeff.c */
	void calc_gap_coeff(vector<string>& alignment, int *gaps, vector<vector<int> >& profile, bool struct_penalties,
		                   string& gap_penalty_mask, int first_seq, int last_seq,
		                   int prf_length, int gapcoef, int lencoef);
	void calc_gap_penalty_mask(int prf_length, const string& mask, string& gap_mask);
	void calc_prf1(vector<vector<int> >& profile, vector<string>& alignment, int *gaps, vector<vector<int> >& matrix,
		               int *seq_weight, int prf_length, int first_seq, int last_seq);
	void calc_prf2(vector<vector<int> >& profile, vector<string>& alignment, int *seq_weight, int prf_length,
		               int first_seq, int last_seq);
	int calc_weight(int leaf);
	void mark_group1(treeptr p, int *groups, int n);
	void mark_group2(treeptr p, int *groups, int n);
	void save_set(int n, int *groups);
	int reverse_pass(const string& ia, const string& ib);
	int forward_pass(const string& ia, const string& ib, int n, int m);
	void skip_space(ifstream& fd1);
	void group_seqs(treeptr p, int *next_groups, int nseqs);
	void set_info(treeptr p, treeptr parent, int pleaf, const char* pname, float pdist);
	shared_ptr<node> avail();
	void create_node(treeptr pptr, treeptr parent);
	treeptr insert_node(treeptr pptr);
	void create_tree(treeptr ptree, treeptr parent);
	float calc_root_mean(treeptr root, float *maxdist);
	float calc_mean(treeptr nptr, float *maxdist, int nseqs);
	void order_nodes();
	treeptr insert_root(treeptr p, float diff);
	treeptr reroot(treeptr ptree, int nseqs);
	int calc_score(int iat,int jat,int v1,int v2);
	float tracepath(int tsb1, int tsb2);
	void ptracepath(int *alen);
	int gap_penalty1(int i, int j, int k);
	int gap_penalty2(int i, int j, int k);
	int open_penalty1(int i, int j);
	int open_penalty2(int i, int j);
	int prfscore(int n, int m);
	void palign();
	int ext_penalty1(int i, int j);
	int ext_penalty2(int i, int j);
	int pdiff(int A, int B, int M, int N, int go1, int go2);
	string add_ggaps_mask(string& mask, int len, const string& path1, const string& path2);
	void add_ggaps();
	unsigned long mult(unsigned long p, unsigned long q);
	void nj_tree(vector<string>& tree_description, ofstream& tree);
	void del(int k);
	void add(int v);
	int diff(int A,int B,int M,int N,int tb,int te);
	void pdel(int k);
	void padd(int k);
	int two_way_split(vector<string>& tree_description, ofstream& tree, int start_row, int flag, int bootstrap);
	void print_phylip_tree(vector<string>& tree_description, ofstream& tree, int bootstrap);
	void make_p_ptrs(int *tptr, int *pl, int naseq, int l);
	void make_n_ptrs(int *tptr, int *pl, int naseq, int len);
	void des_quick_sort(int *array1, int *array2, int array_size);
	void put_frag(int fs,int v1,int v2,int flen);
	bool frag_rel_pos(int a1, int b1, int a2, int b2);
	void pair_align(int seq_no,int l1,int l2);
	void distance_matrix_output(ofstream& ofile);
	void print_tree(vector<string>& tree_description, ofstream& tree, int *totals);
	unsigned long addrand(unsigned long r);
	void compare_tree(vector<string>& tree1, vector<string>& tree2, int *hits, int n);
	void tree_gap_delete();
	bool transition(int base1, int base2);

public:
	bool dnaflag, no_hyd_penalties, no_pref_penalties, output_clustal, use_endgaps;
	float gap_open, gap_extend, transition_weight;
	int* pPercent;
	int divergence_cutoff, gap_dist, numberofseqs;
	int dnamatnum, matnum;
	wstring* pCaption;
	unique_ptr<int[]> order;
	string hyd_residues, pw_dnamtrxname, mtrxname;
	vector<string> hpsequences, result;
	
	CMultAlignment();
	int main_menu();
	inline void copy_chars(const wstring& data)
	{
		int i = 0;
		for (; i < data.size() && i < 256; ++i)
			(*pCaption)[i] = data[i];
		if (i < 256)
			(*pCaption)[i] = '\0';
	}
};
