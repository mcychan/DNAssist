#include "MultAlignment.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdlib>

static const string amino_acid_codes = "ABCDEFGHIKLMNPQRSTUVWXYZ-";  /* DES */
static const string amino_acid_order = "ABCDEFGHIKLMNPQRSTVWXYZ";
static const string nucleic_acid_order = "ABCDGHKMNRSTUVWXY";

static const char pr[] = { 'A' , 'C', 'D', 'E', 'F', 'G', 'H', 'K', 'I', 'L',
'M' , 'N', 'P', 'Q', 'R', 'S', 'T', 'V', 'Y', 'W' };

static const short blosum30mt[] = {
	4,
	0,  5,
	-3, -2, 17,
	0,  5, -3,  9,
	0,  0,  1,  1,  6,
	-2, -3, -3, -5, -4, 10,
	0,  0, -4, -1, -2, -3,  8,
	-2, -2, -5, -2,  0, -3, -3, 14,
	0, -2, -2, -4, -3,  0, -1, -2,  6,
	0,  0, -3,  0,  2, -1, -1, -2, -2,  4,
	-1, -1,  0, -1, -1,  2, -2, -1,  2, -2,  4,
	1, -2, -2, -3, -1, -2, -2,  2,  1,  2,  2,  6,
	0,  4, -1,  1, -1, -1,  0, -1,  0,  0, -2,  0,  8,
	-1, -2, -3, -1,  1, -4, -1,  1, -3,  1, -3, -4, -3, 11,
	1, -1, -2, -1,  2, -3, -2,  0, -2,  0, -2, -1, -1,  0,  8,
	-1, -2, -2, -1, -1, -1, -2, -1, -3,  1, -2,  0, -2, -1,  3,  8,
	1,  0, -2,  0,  0, -1,  0, -1, -1,  0, -2, -2,  0, -1, -1, -1,  4,
	1,  0, -2, -1, -2, -2, -2, -2,  0, -1,  0,  0,  1,  0,  0, -3,  2,  5,
	1, -2, -2, -2, -3,  1, -3, -3,  4, -2,  1,  0, -2, -4, -3, -1, -1,  1,  5,
	-5, -5, -2, -4, -1,  1,  1, -5, -3, -2, -2, -3, -7, -3, -1,  0, -3, -5, -3, 20,
	0, -1, -2, -1, -1, -1, -1, -1,  0,  0,  0,  0,  0, -1,  0, -1,  0,  0,  0, -2, -1,
	-4, -3, -6, -1, -2,  3, -3,  0, -1, -1,  3, -1, -4, -2, -1,  0, -2, -1,  1,  5, -1,  9,
	0,  0,  0,  0,  5, -4, -2,  0, -3,  1, -1, -1, -1,  0,  4,  0, -1, -1, -3, -1,  0, -2,  4 };

static const short blosum40mt[] = {
	5,
	-1,  5,
	-2, -2, 16,
	-1,  6, -2,  9,
	-1,  1, -2,  2,  7,
	-3, -3, -2, -4, -3,  9,
	1, -1, -3, -2, -3, -3,  8,
	-2,  0, -4,  0,  0, -2, -2, 13,
	-1, -3, -4, -4, -4,  1, -4, -3,  6,
	-1,  0, -3,  0,  1, -3, -2, -1, -3,  6,
	-2, -3, -2, -3, -2,  2, -4, -2,  2, -2,  6,
	-1, -3, -3, -3, -2,  0, -2,  1,  1, -1,  3,  7,
	-1,  4, -2,  2, -1, -3,  0,  1, -2,  0, -3, -2,  8,
	-2, -2, -5, -2,  0, -4, -1, -2, -2, -1, -4, -2, -2, 11,
	0,  0, -4, -1,  2, -4, -2,  0, -3,  1, -2, -1,  1, -2,  8,
	-2, -1, -3, -1, -1, -2, -3,  0, -3,  3, -2, -1,  0, -3,  2,  9,
	1,  0, -1,  0,  0, -2,  0, -1, -2,  0, -3, -2,  1, -1,  1, -1,  5,
	0,  0, -1, -1, -1, -1, -2, -2, -1,  0, -1, -1,  0,  0, -1, -2,  2,  6,
	0, -3, -2, -3, -3,  0, -4, -4,  4, -2,  2,  1, -3, -3, -3, -2, -1,  1,  5,
	-3, -4, -6, -5, -2,  1, -2, -5, -3, -2, -1, -2, -4, -4, -1, -2, -5, -4, -3, 19,
	0, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -2, -1, -1,  0,  0, -1, -2, -1,
	-2, -3, -4, -3, -2,  4, -3,  2,  0, -1,  0,  1, -2, -3, -1, -1, -2, -1, -1,  3, -1,  9,
	-1,  2, -3,  1,  5, -4, -2,  0, -4,  1, -2, -2,  0, -1,  4,  0,  0, -1, -3, -2, -1, -2,  5 };

static const short blosum45mt[] = {
	5,
	-1,  4,
	-1, -2, 12,
	-2,  5, -3,  7,
	-1,  1, -3,  2,  6,
	-2, -3, -2, -4, -3,  8,
	0, -1, -3, -1, -2, -3,  7,
	-2,  0, -3,  0,  0, -2, -2, 10,
	-1, -3, -3, -4, -3,  0, -4, -3,  5,
	-1,  0, -3,  0,  1, -3, -2, -1, -3,  5,
	-1, -3, -2, -3, -2,  1, -3, -2,  2, -3,  5,
	-1, -2, -2, -3, -2,  0, -2,  0,  2, -1,  2,  6,
	-1,  4, -2,  2,  0, -2,  0,  1, -2,  0, -3, -2,  6,
	-1, -2, -4, -1,  0, -3, -2, -2, -2, -1, -3, -2, -2,  9,
	-1,  0, -3,  0,  2, -4, -2,  1, -2,  1, -2,  0,  0, -1,  6,
	-2, -1, -3, -1,  0, -2, -2,  0, -3,  3, -2, -1,  0, -2,  1,  7,
	1,  0, -1,  0,  0, -2,  0, -1, -2, -1, -3, -2,  1, -1,  0, -1,  4,
	0,  0, -1, -1, -1, -1, -2, -2, -1, -1, -1, -1,  0, -1, -1, -1,  2,  5,
	0, -3, -1, -3, -3,  0, -3, -3,  3, -2,  1,  1, -3, -3, -3, -2, -1,  0,  5,
	-2, -4, -5, -4, -3,  1, -2, -3, -2, -2, -2, -2, -4, -3, -2, -2, -4, -3, -3, 15,
	0, -1, -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  0, -1, -2, -1,
	-2, -2, -3, -2, -2,  3, -3,  2,  0, -1,  0,  0, -2, -3, -1, -1, -2, -1, -1,  3, -1,  8,
	-1,  2, -3,  1,  4, -3, -2,  0, -3,  1, -2, -1,  0, -1,  4,  0,  0, -1, -3, -2, -1, -2,  4 };

static const short blosum62mt2[] = {
	8,
	-4,  8,
	0, -6, 18,
	-4,  8, -6, 12,
	-2,  2, -8,  4, 10,
	-4, -6, -4, -6, -6, 12,
	0, -2, -6, -2, -4, -6, 12,
	-4,  0, -6, -2,  0, -2, -4, 16,
	-2, -6, -2, -6, -6,  0, -8, -6,  8,
	-2,  0, -6, -2,  2, -6, -4, -2, -6, 10,
	-2, -8, -2, -8, -6,  0, -8, -6,  4, -4,  8,
	-2, -6, -2, -6, -4,  0, -6, -4,  2, -2,  4, 10,
	-4,  6, -6,  2,  0, -6,  0,  2, -6,  0, -6, -4, 12,
	-2, -4, -6, -2, -2, -8, -4, -4, -6, -2, -6, -4, -4, 14,
	-2,  0, -6,  0,  4, -6, -4,  0, -6,  2, -4,  0,  0, -2, 10,
	-2, -2, -6, -4,  0, -6, -4,  0, -6,  4, -4, -2,  0, -4,  2, 10,
	2,  0, -2,  0,  0, -4,  0, -2, -4,  0, -4, -2,  2, -2,  0, -2,  8,
	0, -2, -2, -2, -2, -4, -4, -4, -2, -2, -2, -2,  0, -2, -2, -2,  2, 10,
	0, -6, -2, -6, -4, -2, -6, -6,  6, -4,  2,  2, -6, -4, -4, -6, -4,  0,  8,
	-6, -8, -4, -8, -6,  2, -4, -4, -6, -6, -4, -2, -8, -8, -4, -6, -6, -4, -6, 22,
	0, -2, -4, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -4, -2, -2,  0,  0, -2, -4, -2,
	-4, -6, -4, -6, -4,  6, -6,  4, -2, -4, -2, -2, -4, -6, -2, -4, -4, -4, -2,  4, -2, 14,
	-2,  2, -6,  2,  8, -6, -4,  0, -6,  2, -6, -2,  0, -2,  6,  0,  0, -2, -4, -6, -2, -4,  8 };

static const short blosum80mt[] = {
	7,
	-3,  6,
	-1, -6, 13,
	-3,  6, -7, 10,
	-2,  1, -7,  2,  8,
	-4, -6, -4, -6, -6, 10,
	0, -2, -6, -3, -4, -6,  9,
	-3, -1, -7, -2,  0, -2, -4, 12,
	-3, -6, -2, -7, -6, -1, -7, -6,  7,
	-1, -1, -6, -2,  1, -5, -3, -1, -5,  8,
	-3, -7, -3, -7, -6,  0, -7, -5,  2, -4,  6,
	-2, -5, -3, -6, -4,  0, -5, -4,  2, -3,  3,  9,
	-3,  5, -5,  2, -1, -6, -1,  1, -6,  0, -6, -4,  9,
	-1, -4, -6, -3, -2, -6, -5, -4, -5, -2, -5, -4, -4, 12,
	-2, -1, -5, -1,  3, -5, -4,  1, -5,  2, -4, -1,  0, -3,  9,
	-3, -2, -6, -3, -1, -5, -4,  0, -5,  3, -4, -3, -1, -3,  1,  9,
	2,  0, -2, -1, -1, -4, -1, -2, -4, -1, -4, -3,  1, -2, -1, -2,  7,
	0, -1, -2, -2, -2, -4, -3, -3, -2, -1, -3, -1,  0, -3, -1, -2,  2,  8,
	-1, -6, -2, -6, -4, -2, -6, -5,  4, -4,  1,  1, -5, -4, -4, -4, -3,  0,  7,
	-5, -8, -5, -8, -6,  0, -6, -4, -5, -6, -4, -3, -7, -7, -4, -5, -6, -5, -5, 16,
	-1, -3, -4, -3, -2, -3, -3, -2, -2, -2, -2, -2, -2, -3, -2, -2, -1, -1, -2, -5, -2,
	-4, -5, -5, -6, -5,  4, -6,  3, -3, -4, -2, -3, -4, -6, -3, -4, -3, -3, -3,  3, -3, 11,
	-2,  0, -7,  1,  6, -6, -4,  0, -6,  1, -5, -3, -1, -2,  5,  0, -1, -2, -4, -5, -1, -4,  6 };

static const short clustalvdnamt[] = {
	10,
	0,  0,
	0,  0, 10,
	0,  0,  0,  0,
	0,  0,  0,  0, 10,
	0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 10,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };

static const short gon40mt[] = {
	92,
	0,   0,
	-31,   0, 163,
	-56,   0,-135, 111,
	-37,   0,-140,  16, 105,
	-92,   0, -64,-152,-143, 126,
	-32,   0, -91, -51, -76,-152, 105,
	-65,   0, -67, -41, -40, -50, -81, 145,
	-76,   0, -87,-150,-106, -39,-158, -94, 104,
	-54,   0,-132, -47, -13,-127, -79, -34, -86, 103,
	-68,   0, -85,-155,-108, -13,-141, -85,   5, -85,  89,
	-45,   0, -63,-130, -80, -16,-114, -60,  10, -57,  16, 140,
	-62,   0, -83,   6, -38,-104, -40,  -7, -99, -20,-112, -91, 115,
	-37,   0,-137, -69, -60,-128, -87, -71,-108, -62, -83,-119, -78, 124,
	-43,   0,-113, -32,  10,-100, -71,   0, -91,   2, -60, -35, -25, -46, 118,
	-61,   0, -86, -77, -50,-130, -69, -31,-103,  19, -84, -81, -47, -73,  -6, 112,
	0,   0, -35, -36, -41,-111, -37, -48, -95, -43, -95, -64, -11, -35, -35, -51,  99,
	-25,   0, -59, -47, -52, -90, -85, -46, -51, -34, -78, -44, -27, -42, -39, -52,  13, 100,
	-22,   0, -43,-133, -74, -58,-122, -98,  28, -82, -18, -22,-103, -86, -79, -88, -74, -25,  97,
	-120,   0, -68,-171,-131,  -6,-108, -70, -93,-127, -71, -72,-119,-149, -87, -63, -98,-120,-115, 181,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	-95,   0, -56, -98,-107,  31,-129,   5, -76, -88, -64, -66, -62,-106, -81, -75, -69, -87, -73,   1,   0, 135,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };

static const short gon80mt[] = {
	75,
	0,   0,
	-10,   0, 154,
	-31,   0, -93,  96,
	-17,   0, -94,  31,  88,
	-64,   0, -39,-111,-102, 114,
	-11,   0, -61, -26, -47,-115,  97,
	-39,   0, -43, -17, -17, -26, -53, 127,
	-43,   0, -54,-106, -73, -15,-114, -64,  86,
	-30,   0, -88, -21,   4, -89, -50, -12, -59,  85,
	-43,   0, -55,-109, -75,   7,-104, -57,  22, -58,  77,
	-26,   0, -39, -88, -53,   3, -83, -38,  25, -37,  31, 117,
	-34,   0, -55,  21, -13, -75, -18,   9, -71,  -2, -79, -62,  97,
	-16,   0, -93, -42, -35, -93, -58, -45, -75, -37, -58, -78, -48, 114,
	-22,   0, -76,  -9,  23, -70, -44,  14, -60,  17, -39, -19,  -6, -24,  95,
	-36,   0, -60, -44, -23, -90, -43, -10, -71,  33, -58, -53, -22, -45,  11,  97,
	14,   0, -15, -14, -19, -77, -16, -25, -62, -20, -64, -41,   5, -14, -15, -27,  78,
	-5,   0, -34, -24, -27, -62, -52, -24, -28, -15, -49, -25,  -7, -20, -18, -27,  25,  81,
	-6,   0, -21, -89, -51, -31, -86, -65,  41, -54,   3,   1, -69, -57, -51, -60, -43,  -9,  80,
	-87,   0, -43,-124, -98,  16, -81, -43, -63, -89, -44, -45, -86,-112, -62, -41, -72, -87, -80, 173,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	-65,   0, -32, -69, -74,  49, -94,  21, -47, -60, -35, -37, -39, -76, -53, -50, -46, -58, -47,  23,   0, 123,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };

static const short gon120mt[] = {
	59,
	0,   0,
	-1,   0, 144,
	-18,   0, -69,  82,
	-9,   0, -68,  35,  72,
	-48,   0, -26, -87, -78, 102,
	-3,   0, -45, -14, -31, -92,  90,
	-26,   0, -31,  -7,  -6, -14, -37, 110,
	-27,   0, -36, -80, -55,  -3, -87, -48,  72,
	-19,   0, -64,  -8,  11, -67, -34,  -2, -44,  69,
	-30,   0, -39, -82, -57,  15, -82, -42,  28, -44,  66,
	-17,   0, -26, -64, -40,  11, -65, -28,  29, -27,  34,  95,
	-20,   0, -41,  26,  -1, -58,  -7,  14, -55,   5, -61, -46,  80,
	-6,   0, -68, -28, -22, -72, -41, -31, -56, -24, -44, -56, -32, 105,
	-12,   0, -56,   1,  25, -53, -30,  17, -43,  20, -30, -14,   1, -14,  74,
	-23,   0, -45, -27, -10, -68, -30,  -1, -53,  36, -44, -38, -10, -30,  16,  83,
	16,   0,  -7,  -5,  -9, -58,  -6, -14, -44, -10, -47, -29,  10,  -5,  -7, -15,  60,
	2,   0, -21, -13, -15, -47, -35, -14, -17,  -6, -34, -16,   0, -10,  -9, -16,  26,  64,
	0,   0, -11, -65, -38, -17, -65, -47,  42, -39,  13,  10, -50, -42, -36, -44, -28,  -3,  65,
	-68,   0, -29, -96, -78,  27, -66, -28, -46, -68, -29, -31, -68, -89, -49, -30, -57, -67, -59, 166,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	-48,   0, -20, -53, -56,  55, -74,  26, -31, -44, -20, -22, -28, -59, -38, -37, -35, -42, -33,  33,   0, 111,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };

static const short gon160mt[] = {
	46,
	0,   0,
	3,   0, 135,
	-11,   0, -53,  70,
	-4,   0, -52,  34,  59,
	-38,   0, -18, -70, -62,  91,
	2,   0, -34,  -7, -21, -76,  82,
	-18,   0, -23,  -1,  -1,  -7, -27,  93,
	-18,   0, -25, -62, -43,   3, -70, -37,  59,
	-12,   0, -48,  -1,  13, -53, -24,   2, -35,  55,
	-22,   0, -29, -65, -45,  19, -67, -32,  30, -34,  57,
	-12,   0, -19, -50, -31,  14, -52, -21,  29, -21,  34,  76,
	-12,   0, -31,  26,   5, -47,  -2,  15, -44,   8, -48, -36,  65,
	-1,   0, -52, -19, -14, -58, -30, -22, -43, -16, -35, -42, -22,  96,
	-7,   0, -42,   6,  23, -41, -21,  17, -32,  20, -24, -12,   5,  -8,  56,
	-16,   0, -35, -16,  -3, -53, -21,   3, -41,  35, -35, -29,  -4, -21,  17,  71,
	16,   0,  -2,   0,  -3, -45,  -1,  -8, -33,  -4, -36, -23,  11,   0,  -2,  -9,  44,
	5,   0, -14,  -6,  -8, -36, -24,  -8, -12,  -2, -24, -11,   3,  -4,  -4,  -9,  23,  50,
	1,   0,  -6, -49, -30,  -8, -52, -35,  40, -30,  17,  14, -38, -32, -27, -34, -20,   0,  53,
	-55,   0, -21, -78, -64,  32, -55, -19, -34, -54, -20, -22, -55, -74, -40, -24, -47, -54, -45, 158,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	-37,   0, -13, -42, -44,  56, -60,  27, -20, -35, -11, -13, -22, -48, -29, -29, -28, -32, -24,  38,   0, 100,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };

static const short gon250mt[] = {
	24,
	0,   0,
	5,   0, 115,
	-3,   0, -32,  47,
	0,   0, -30,  27,  36,
	-23,   0,  -8, -45, -39,  70,
	5,   0, -20,   1,  -8, -52,  66,
	-8,   0, -13,   4,   4,  -1, -14,  60,
	-8,   0, -11, -38, -27,  10, -45, -22,  40,
	-4,   0, -28,   5,  12, -33, -11,   6, -21,  32,
	-12,   0, -15, -40, -28,  20, -44, -19,  28, -21,  40,
	-7,   0,  -9, -30, -20,  16, -35, -13,  25, -14,  28,  43,
	-3,   0, -18,  22,   9, -31,   4,  12, -28,   8, -30, -22,  38,
	3,   0, -31,  -7,  -5, -38, -16, -11, -26,  -6, -23, -24,  -9,  76,
	-2,   0, -24,   9,  17, -26, -10,  12, -19,  15, -16, -10,   7,  -2,  27,
	-6,   0, -22,  -3,   4, -32, -10,   6, -24,  27, -22, -17,   3,  -9,  15,  47,
	11,   0,   1,   5,   2, -28,   4,  -2, -18,   1, -21, -14,   9,   4,   2,  -2,  22,
	6,   0,  -5,   0,  -1, -22, -11,  -3,  -6,   1, -13,  -6,   5,   1,   0,  -2,  15,  25,
	1,   0,   0, -29, -19,   1, -33, -20,  31, -17,  18,  16, -22, -18, -15, -20, -10,   0,  34,
	-36,   0, -10, -52, -43,  36, -40,  -8, -18, -35,  -7, -10, -36, -50, -27, -16, -33, -35, -26, 142,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	-22,   0,  -5, -28, -27,  51, -40,  22,  -7, -21,   0,  -2, -14, -31, -17, -18, -19, -19, -11,  41,   0,  78,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };

static const short gon300mt[] = {
	16,
	0,   0,
	5,   0, 104,
	-1,   0, -24,  37,
	1,   0, -23,  23,  27,
	-18,   0,  -5, -37, -31,  60,
	5,   0, -15,   3,  -4, -42,  58,
	-6,   0, -10,   5,   4,   0, -10,  45,
	-6,   0,  -7, -30, -21,  11, -36, -16,  33,
	-2,   0, -21,   6,  11, -26,  -7,   5, -17,  24,
	-9,   0, -10, -32, -22,  19, -36, -14,  25, -17,  33,
	-5,   0,  -6, -24, -16,  15, -28, -10,  22, -11,  24,  31,
	-1,   0, -14,  18,   9, -25,   5,  10, -22,   8, -24, -17,  27,
	3,   0, -23,  -4,  -2, -30, -11,  -8, -20,  -3, -18, -19,  -6,  66,
	-1,   0, -18,   9,  14, -20,  -6,   9, -15,  13, -13,  -8,   7,  -1,  18,
	-4,   0, -17,   0,   5, -25,  -6,   6, -19,  22, -18, -13,   4,  -6,  13,  37,
	8,   0,   1,   5,   3, -22,   4,  -1, -14,   2, -17, -11,   7,   4,   2,   0,  15,
	5,   0,  -3,   1,   1, -17,  -7,  -1,  -4,   2,  -9,  -5,   4,   2,   1,  -1,  11,  17,
	0,   0,   1, -23, -15,   4, -26, -15,  26, -13,  17,  15, -17, -14, -12, -15,  -8,   0,  26,
	-29,   0,  -7, -42, -36,  36, -34,  -5, -13, -28,  -4,  -6, -30, -41, -23, -14, -27, -28, -19, 132,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
	-17,   0,  -3, -22, -22,  46, -33,  18,  -3, -17,   3,   1, -12, -25, -14, -14, -15, -15,  -7,  40,   0,  67,
	0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 };

static const short idmat[] = {
	10,
	0, 10,
	0, 0, 10,
	0, 0, 0, 10,
	0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,10 };

static const short pam350mt[] = {
	2,
	1,  3,
	-2, -5, 18,
	1,  3, -6,  4,
	1,  3, -6,  4,  4,
	-4, -5, -5, -6, -6, 13,
	2,  1, -4,  1,  1, -6,  5,
	-1,  1, -4,  1,  1, -2, -2,  7,
	0, -2, -3, -2, -2,  2, -2, -2,  5,
	-1,  1, -6,  1,  0, -6, -1,  1, -2,  5,
	-2, -4, -7, -4, -4,  3, -4, -2,  4, -3,  8,
	-1, -2, -6, -3, -2,  1, -3, -2,  3,  0,  5,  6,
	0,  2, -4,  2,  2, -4,  1,  2, -2,  1, -3, -2,  2,
	1,  0, -3,  0,  0, -5,  0,  0, -2, -1, -3, -2,  0,  6,
	0,  2, -6,  2,  3, -5, -1,  3, -2,  1, -2, -1,  1,  1,  4,
	-1,  0, -4, -1,  0, -5, -2,  2, -2,  4, -3,  0,  1,  0,  2,  7,
	1,  1,  0,  1,  0, -4,  1, -1, -1,  0, -3, -2,  1,  1,  0,  0,  1,
	1,  0, -2,  0,  0, -3,  1, -1,  0,  0, -2, -1,  1,  1,  0, -1,  1,  2,
	0, -2, -2, -2, -2, -1, -1, -2,  4, -2,  3,  2, -2, -1, -2, -3, -1,  0,  5,
	-7, -6,-10, -8, -8,  1, -8, -3, -6, -4, -2, -5, -5, -7, -5,  4, -3, -6, -7, 27,
	0,  0, -3, -1,  0, -2, -1,  0,  0, -1, -1,  0,  0,  0,  0, -1,  0,  0,  0, -5, -1,
	-4, -4,  1, -5, -5, 11, -6,  0,  0, -5,  0, -2, -3, -6, -5, -5, -3, -3, -2,  1, -2, 14,
	0,  2, -6,  3,  3, -6,  0,  2, -2,  1, -3, -2,  2,  0,  3,  1,  0,  0, -2, -7,  0, -5,  3 };

static const short pam20mt[] = {
	6,
	-5,  6,
	-8,-14, 10,
	-4,  6,-16,  8,
	-3,  0,-16,  2,  8,
	-9,-12,-15,-17,-16,  9,
	-3, -4,-11, -4, -5,-10,  7,
	-8, -2, -8, -5, -6, -7,-10,  9,
	-6, -7, -7, -9, -6, -3,-13,-11,  9,
	-8, -3,-16, -6, -5,-16, -8, -8, -7,  7,
	-7,-10,-17,-15,-10, -4,-12, -7, -2, -9,  7,
	-6,-12,-16,-13, -8, -5,-10,-13, -2, -3,  0, 11,
	-5,  6,-13,  1, -3,-10, -4, -1, -6, -2, -8,-11,  8,
	-2, -8, -9, -9, -7,-11, -7, -5,-10, -8, -8, -9, -7,  8,
	-5, -4,-16, -4,  0,-15, -8,  0, -9, -4, -6, -5, -5, -4,  9,
	-8, -9, -9,-12,-11,-10,-11, -3, -6, -1,-10, -5, -7, -5, -2,  9,
	-1, -2, -4, -5, -5, -7, -3, -7, -8, -5, -9, -6, -1, -3, -6, -4,  7,
	-1, -4, -9, -6, -7,-10, -7, -8, -3, -4, -8, -5, -3, -5, -7, -8,  0,  7,
	-3, -9, -7, -9, -8, -9, -7, -7,  1,-10, -3, -2, -9, -7, -8, -9, -8, -4,  7,
	-16,-11,-18,-17,-19, -6,-17, -8,-16,-14, -7,-15, -9,-16,-15, -3, -6,-15,-18, 13,
	-4, -6,-11, -7, -6, -9, -6, -6, -6, -6, -7, -6, -4, -6, -6, -7, -4, -5, -6,-13, -6,
	-9, -7, -5,-13, -9,  1,-16, -4, -7,-10, -8,-13, -5,-16,-14,-11, -8, -7, -8, -6, -9, 10,
	-4, -1,-16,  0,  6,-16, -6, -2, -7, -5, -8, -6, -4, -5,  7, -5, -6, -7, -8,-17, -6,-11,  6 };

static const short pam60mt[] = {
	5,
	-2,  5,
	-5, -9,  9,
	-2,  5,-10,  7,
	-1,  2,-10,  3,  7,
	-6, -8, -9,-11,-10,  8,
	0, -2, -7, -2, -2, -7,  6,
	-5,  0, -6, -2, -3, -4, -6,  8,
	-3, -4, -4, -5, -4, -1, -7, -6,  7,
	-5, -1,-10, -2, -3,-10, -5, -4, -4,  6,
	-4, -7,-11, -9, -7, -1, -8, -4,  0, -6,  6,
	-3, -6,-10, -7, -5, -2, -6, -7,  1,  0,  2, 10,
	-2,  5, -7,  2,  0, -6, -1,  1, -4,  0, -5, -6,  6,
	0, -4, -6, -5, -3, -7, -4, -2, -6, -4, -5, -6, -4,  7,
	-3, -1,-10, -1,  2, -9, -5,  2, -5, -1, -3, -2, -2, -1,  7,
	-5, -5, -6, -6, -6, -7, -7,  0, -4,  2, -6, -2, -3, -2,  0,  8,
	1,  0, -1, -2, -2, -5,  0, -4, -4, -2, -6, -4,  1,  0, -3, -2,  5,
	1, -2, -5, -3, -4, -6, -3, -5, -1, -2, -5, -2, -1, -2, -4, -4,  1,  6,
	-1, -5, -4, -6, -4, -5, -4, -5,  3, -6, -1,  0, -5, -4, -5, -5, -4, -1,  6,
	-10, -8,-12,-11,-12, -3,-11, -5,-10, -8, -4, -9, -6,-10, -9,  0, -4, -9,-11, 13,
	-2, -3, -6, -3, -3, -5, -3, -3, -3, -3, -4, -3, -2, -3, -3, -4, -2, -2, -3, -8, -3,
	-6, -5, -2, -8, -7,  3,-10, -2, -4, -7, -5, -7, -3,-10, -8, -8, -5, -5, -5, -3, -5,  9,
	-2,  1,-10,  2,  5,-10, -3,  0, -4, -2, -5, -4, -1, -2,  6, -2, -3, -4, -5,-11, -3, -7,  5 };

static const short pam120mt[] = {
	3,
	0,  4,
	-3, -6,  9,
	0,  4, -7,  5,
	0,  3, -7,  3,  5,
	-4, -5, -6, -7, -7,  8,
	1,  0, -4,  0, -1, -5,  5,
	-3,  1, -4,  0, -1, -3, -4,  7,
	-1, -3, -3, -3, -3,  0, -4, -4,  6,
	-2,  0, -7, -1, -1, -7, -3, -2, -3,  5,
	-3, -4, -7, -5, -4,  0, -5, -3,  1, -4,  5,
	-2, -4, -6, -4, -3, -1, -4, -4,  1,  0,  3,  8,
	-1,  3, -5,  2,  1, -4,  0,  2, -2,  1, -4, -3,  4,
	1, -2, -4, -3, -2, -5, -2, -1, -3, -2, -3, -3, -2,  6,
	-1,  0, -7,  1,  2, -6, -3,  3, -3,  0, -2, -1,  0,  0,  6,
	-3, -2, -4, -3, -3, -5, -4,  1, -2,  2, -4, -1, -1, -1,  1,  6,
	1,  0,  0,  0, -1, -3,  1, -2, -2, -1, -4, -2,  1,  1, -2, -1,  3,
	1,  0, -3, -1, -2, -4, -1, -3,  0, -1, -3, -1,  0, -1, -2, -2,  2,  4,
	0, -3, -3, -3, -3, -3, -2, -3,  3, -4,  1,  1, -3, -2, -3, -3, -2,  0,  5,
	-7, -6, -8, -8, -8, -1, -8, -3, -6, -5, -3, -6, -4, -7, -6,  1, -2, -6, -8, 12,
	-1, -1, -4, -2, -1, -3, -2, -2, -1, -2, -2, -2, -1, -2, -1, -2, -1, -1, -1, -5, -2,
	-4, -3, -1, -5, -5,  4, -6, -1, -2, -5, -2, -4, -2, -6, -5, -5, -3, -3, -3, -2, -3,  8,
	-1,  2, -7,  3,  4, -6, -2,  1, -3, -1, -3, -2,  0, -1,  4, -1, -1, -2, -3, -7, -1, -5,  4 };

static const short swgapdnamt[] = {
	10,
	-9, 10,
	-9, 10, 10,
	10, 10, -9, 10,
	-9, 10, -9, 10, 10,
	10, 10, 10, 10, -9, 10,
	-9, 10, -9, 10, 10, 10, 10,
	10, 10, 10, 10, -9, 10, -9, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10,
	10, 10, -9, 10, 10, 10, 10, 10, 10, 10,
	-9, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	-9, 10, -9, 10, -9, 10, 10, -9, 10, -9, -9, 10,
	-9, 10, -9, 10, -9, 10, 10, -9, 10, -9, -9, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, -9, -9, 10,
	10, 10, -9, 10, -9, 10, 10, 10, 10, 10, -9, 10, 10, 10, 10,
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	-9, 10, 10, 10, -9, 10, 10, 10, 10, -9, 10, 10, 10, 10, 10, 10, 10 };

/* DAYHOFF.H

Table of estimated PAMS (actual no. of substitutions per 100 residues)
for a range of observed amino acid distances from 75.0% (the first entry
in the array), in 0.1% increments, up to 93.0%.

These values are used to correct for multiple hits in protein alignments.
The values below are for observed distances above 74.9%.  For values above
93%, an arbitrary value of 1000 PAMS (1000% substitution) is used.

These values are derived from a Dayhoff model (1978) of amino acid
substitution and assume average amino acid composition and that amino
acids replace each other at the same rate as in the original Dayhoff model.

Up to 75% observed distance, use Kimura's emprical formula to derive
the correction.  For 75% or greater, use this table.  Kimura's formula
is accurate up to about 75% and fails completely above 85%.
*/

static const int dayhoff_pams[] = {
	195,   /* 75.0% observed d; 195 PAMs estimated = 195% estimated d */
	196,   /* 75.1% observed d; 196 PAMs estimated */
	197,    198,    199,    200,    200,    201,    202,  203,
	204,    205,    206,    207,    208,    209,    209,    210,    211,  212,
	213,    214,    215,    216,    217,    218,    219,    220,    221,  222,
	223,    224,    226,    227,    228,    229,    230,    231,    232,  233,
	234,    236,    237,    238,    239,    240,    241,    243,    244,  245,
	246,    248,    249,    250,    /* 250 PAMs = 80.3% observed d */
	252,    253,    254,    255,    257,  258,
	260,    261,    262,    264,    265,    267,    268,    270,    271,  273,
	274,    276,    277,    279,    281,    282,    284,    285,    287,  289,
	291,    292,    294,    296,    298,    299,    301,    303,    305,  307,
	309,    311,    313,    315,    317,    319,    321,    323,    325,  328,
	330,    332,    335,    337,    339,    342,    344,    347,    349,  352,
	354,    357,    360,    362,    365,    368,    371,    374,    377,  380,
	383,    386,    389,    393,    396,    399,    403,    407,    410,  414,
	418,    422,    426,    430,    434,    438,    442,    447,    451,  456,
	461,    466,    471,    476,    482,    487,    493,    498,    504,  511,
	517,    524,    531,    538,    545,    553,    560,    569,    577,  586,
	595,    605,    615,    626,    637,    649,    661,    675,    688,  703,
	719,    736,    754,    775,    796,    819,    845,    874,    907,  945,
	/* 92.9% observed; 945 PAMs */
	988    /* 93.0% observed; 988 PAMs */
};

static const int pas_op[] = { 87, 87,104, 69, 80,139,100,104, 68, 79, 71,137,126, 93,128,124,111, 75,100, 77 };

static const string res_cat1[] = {
	"STA",
	"NEQK",
	"NHQK",
	"NDEQ",
	"QHRK",
	"MILV",
	"MILF",
	"HY",
	"FYW",
	"" };

static const string res_cat2[] = {
	"CSA",
	"ATV",
	"SAG",
	"STNK",
	"STPA",
	"SGND",
	"SNDEQK",
	"NDEQHK",
	"NEQHRK",
	"FVLIM",
	"HFY",
	"" };

CMultAlignment::CMultAlignment()
{
	/*
		multiple alignment parameters
	*/
	dna_gap_open = 15.0f, dna_gap_extend = 6.66f;
	prot_gap_open = 10.0f, prot_gap_extend = 0.05f;
	gap_dist = 8;
	output_order = ALIGNED;
	divergence_cutoff = 40;
	matnum = 1;
	dnamatnum = 1;
	hyd_residues = "GPSNDQEKR";
	neg_matrix = false;
	no_hyd_penalties = false;
	no_pref_penalties = false;
	use_endgaps = false;
	reset_alignments = false;		/* DES */
	output_struct_penalties = 0;
	struct_penalties1 = NONE;
	struct_penalties2 = NONE;
	use_ss1 = true;
	use_ss2 = true;
	helix_penalty = 4;
	strand_penalty = 4;
	loop_penalty = 1;
	helix_end_minus = 3;
	helix_end_plus = 0;
	strand_end_minus = 1;
	strand_end_plus = 1;
	helix_end_penalty = 2;
	strand_end_penalty = 2;

	/*
		pairwise alignment parameters
	*/
	dna_pw_go_penalty = 15.0f, dna_pw_ge_penalty = 6.66f;
	prot_pw_go_penalty = 10.0f, prot_pw_ge_penalty = 0.1f;
	quick_pairalign = false;
	transition_weight = 0.5f;

	/*
		quick pairwise alignment parameters
	*/
	dna_ktup = 2;	/* default parameters for DNA */
	dna_wind_gap = 5;
	dna_signif = 4;
	dna_window = 4;

	prot_ktup = 1;	/* default parameters for proteins */
	prot_wind_gap = 3;
	prot_signif = 5;
	prot_window = 5;
	percent - true;
	tossgaps = false;
	kimura = false;

	boot_ntrials = 1000;
	boot_ran_seed = 111;

	explicit_dnaflag = false; /* Explicit setting of sequence type on comm.line*/
	lowercase = true; /* Flag for GDE output - set on comm. line*/

	output_clustal = true;
	output_gcg = false;
	output_phylip = false;
	output_nbrf = false;
	output_gde = false;
	showaln = true;
	save_parameters = false;

	/* DES */
	output_tree_clustal = false;
	output_tree_phylip = true;
	output_tree_distances = false;
	bootstrap_format = BS_NODE_LABELS;
}

void CMultAlignment::aln_score()
{	
	int maxres;
	int s1, s2, c1, c2;
	int ngaps;
	int i,l1,l2;
	int score;
	short  *mat_xref;
	const short *matptr;
	vector<vector<int> >	matrix;

	/* calculate an overall score for the alignment by summing the
	scores for each pairwise alignment */

	matptr = blosum45mt;
	mat_xref = def_aa_xref.get();
	maxres = get_matrix(matptr, mat_xref, matrix, true, 100);
	if (maxres == 0) {
		cerr << "Error: matrix blosum30 not found\n";
		return;
	}

	score = 0;
	for (s1 = 1; s1 <= nseqs; s1++) {
		for (s2 = 1;s2 < s1; s2++) {

			l1 = seqlen_array[s1];
			l2 = seqlen_array[s2];
			for (i = 1; i < l1 && i<l2; ++i) {
				c1 = seq_array[s1][i];
				c2 = seq_array[s2][i];
				if ((c1 >= 0) && (c1 <= max_aa) && (c2 >= 0) && (c2 <= max_aa))
					score += matrix[c1][c2];
			}

			ngaps = count_gaps(s1, s2, l1);

			score -= 100 * gap_open * ngaps;

		}
	}

	  score /= 100;
}

void CMultAlignment::init_interface()
{
	isEmpty = profile1_empty = profile2_empty = true;
}

int CMultAlignment::main_menu()
{
	init_interface();
	init_matrix();

	fill_chartab();
	usemenu = true;
	interactive = false;
	seq_input(false);
	int value = multiple_align_menu();
	for(int j = 0; j < numberofseqs; ++j)
		free_seq(j);
	free_aln(nseqs);
	return value;
}

bool CMultAlignment::open_output_file(ofstream& prompt, const wstring& path, wstring& file_name, const string& file_extension)
{
	size_t len = 260;
	char directory[260];
	getenv_s(&len, directory, "temp");
	string fileName(directory);
	fileName += "\\$temp.";
	fileName += file_extension;
	wstring wTmpPath(fileName.begin(), fileName.end());

	file_name = wTmpPath;

	prompt.open(file_name);
	return prompt.good();
}

bool CMultAlignment::open_explicit_file(ofstream& ofs, const wstring& file_name)
{
	if (file_name.length() == 0) {
		cerr << "Bad output file [%s]" << file_name.c_str();
		return false;
	}

	ofs.open(file_name.c_str());
	return ofs.good();
}

int CMultAlignment::seq_input(bool append)
{
	int local_nseqs;

	if (append)
		local_nseqs = readseqs(nseqs + 1);
	else
		local_nseqs = readseqs(1);  /*  1 is the first seq to be read */

	if(local_nseqs < 0)					/* file could not be opened */
		return local_nseqs;

	if(local_nseqs == 0) {
		cerr << "No sequences in file!  Bad format?";
		return local_nseqs;
	}

	struct_penalties1 = struct_penalties2 = NONE;
	sec_struct_mask1.clear();
	sec_struct_mask2.clear();
	gap_penalty_mask1.clear();
	gap_penalty_mask2.clear();
	ss_name1.clear();
	ss_name2.clear();

	if(append)
		nseqs += local_nseqs;
	else
		nseqs = local_nseqs;

	if(dnaflag) {
		gap_open = dna_gap_open;
		gap_extend = dna_gap_extend;
	}
	else {
		gap_open = prot_gap_open;
		gap_extend = prot_gap_extend;
	}
	isEmpty = false;

	return local_nseqs;
}


bool CMultAlignment::open_alignment_output(const wstring& path)
{
	if(!output_clustal && !output_nbrf && !output_gcg && !output_phylip && !output_gde) {
		cerr << "You must select an alignment output format";
		return false;
	}

	if(output_clustal) {
		if (outfile_name.length() > 0) {
			clustal_outname = outfile_name;
			if(!open_explicit_file(clustal_outfile, clustal_outname))
				return false;
		}
	}

	if(output_nbrf) {
		if (outfile_name.length() > 0) {
			nbrf_outname = outfile_name;
			if(!open_explicit_file(nbrf_outfile, nbrf_outname))
				return false;
		}
	}

	if(output_gcg) {
		if (outfile_name.length() > 0) {
			gcg_outname = outfile_name;
			if(!open_explicit_file(gcg_outfile, gcg_outname))
				return false;
		}
	}

	if(output_phylip) {
		if (outfile_name.length() > 0) {
			phylip_outname = outfile_name;
			if(!open_explicit_file(phylip_outfile, phylip_outname))
				return false;
		}
	}
	if(output_gde) {
		if (outfile_name.length() > 0) {
			gde_outname = outfile_name;
			if(!open_explicit_file(gde_outfile, gde_outname))
				return false;
		}
	}
	return true;
}

void CMultAlignment::print_sec_struct_mask(int prf_length, const string& mask, string& struct_mask)
{
	int i, j;

/*
	calculate the gap penalty mask from the secondary structures
*/
	i = 0;
	while (i < prf_length) {
		if (tolower(mask[i]) == 'a' || mask[i] == '$') {
			for (j = 0; j < helix_end_minus; ++j) {
				if (i + j >= prf_length || (tolower(mask[i + j]) != 'a'
									&& mask[i + j] != '$')) break;
				struct_mask[i + j] = 'a';
			}
			i += j;
			while (tolower(mask[i]) == 'a' || mask[i] == '$') {
				if (i >= prf_length)
					break;
				if (mask[i] == '$') {
					struct_mask[i] = 'A';
					i++;
					break;
				}
				else struct_mask[i] = mask[i];
				i++;
			}
			for (j = 0; j < helix_end_minus; ++j) {
				if ((i - j - 1>=0) && (tolower(mask[i - j - 1]) == 'a'
									|| mask[i - j - 1] == '$'))
					struct_mask[i - j - 1] = 'a';
			}
		}
		else if (tolower(mask[i]) == 'b' || mask[i] == '%') {
			for (j = 0; j < strand_end_minus; ++j) {
				if (i + j >= prf_length || (tolower(mask[i + j]) != 'b'
									&& mask[i + j] != '%')) break;
				struct_mask[i + j] = 'b';
			}
			i += j;
			while (tolower(mask[i]) == 'b'
									|| mask[i] == '%') {
				if (i>=prf_length) break;
				if (mask[i] == '%') {
					struct_mask[i] = 'B';
					i++;
					break;
				}
				else struct_mask[i] = mask[i];
				i++;
			}
			for (j = 0; j < strand_end_minus; ++j) {
				if ((i - j - 1 >= 0) && (tolower(mask[i - j - 1]) == 'b'
									|| mask[i - j - 1] == '%'))
				struct_mask[i - j - 1] = 'b';
			}
		}
		else
			i++;
	}
}

void CMultAlignment::create_alignment_output(int fseq, int lseq)
{
	int length = 0;
	order = make_unique<int[]>(lseq);
	for (int i = fseq; i <= lseq; ++i) {
		if (length < seqlen_array[i])
			length = seqlen_array[i];
	}
	result.clear();
	clustal_out(result, order.get(), 1, length, fseq, lseq);
}

size_t res_index(const string& t, char c)
{
	size_t found = t.find(c);
	if(found == string::npos)
		return -1;
	return found;
}

bool check_dnaflag(const string& seq, int slen)
/* check if DNA or Protein
	The decision is based on counting all A,C,G,T,U or N.
	If >= 85% of all characters (except -) are as above => DNA  */
{
	int i, c, nresidues, nbases;
	float ratio;
	const string dna_codes = "ACGTUN";

	nresidues = nbases = 0;
	for(i = 1; i <= slen; ++i) {
		if(seq[i] != '-') {
			nresidues++;
			if(seq[i] == 'N')
				nbases++;
			else {
				c = res_index(dna_codes, seq[i]);
				if(c >= 0)
					nbases++;
			}
		}
	}
	if(nbases == 0 || nresidues == 0)
		return false;
	ratio = (float) nbases / (float)nresidues;
	return ratio >= 0.85;
}

void CMultAlignment::n_encode(const string& seq, string& naseq, int l)
{				/* code seq as ints .. use gap_pos2 for gap */
	register int i;
/*	static char *nucs="ACGTU";	*/

	for(i = 1; i <= l; ++i) {
		if(seq[i] == '-')			 		/* if a gap character -> code = gap_pos2 */
			naseq[i] = gap_pos2;	/* this is the code for a gap in */
		else {							/* the input files */
			naseq[i] = res_index(amino_acid_codes, seq[i]);
		}
	}
	naseq[i] = -3;
}

void CMultAlignment::p_encode(const string& seq, string& naseq, int l)
{
	n_encode(seq, naseq, l);
}

bool CMultAlignment::blankline(const string& line)
{
	for(int i = 0; line[i] != '\n' && i < line.length(); ++i) {
		if( isdigit(line[i]) ||
			isspace(line[i]) ||
			(line[i] == '*') ||
			(line[i] == ':') ||
					(line[i] == '.'))
			continue;
		return false;
	}
	return true;
}

void CMultAlignment::get_clustal_ss(int length)
/* read the structure data from a clustal multiple alignment file */
{
	string title, line, lin2, tseq, sname;
	int i, j, len, ix, struct_index = 0;
	char c;

	ifstream fin;
	fin.seekg(0); 		/* start at the beginning */

	len = 0;				/* initialise length to zero */
	getline(fin, line);
	if (line.length() == 0)
		return;	/* read the title line...ignore it */

	getline(fin, line);
	if (line.length() == 0)
		return;  /* read the next line... */
/* skip any blank lines */
	for (; getline(fin, line);) {
		if(!blankline(line))
			break;
	}

/* look for structure table lines */
	ix = -1;
	do {
		if(line[0] != '!')
			break;
		string header = line.substr(0, 3);
		if(header == "!SS") {
			ix++;
			line.resize(3);
			line += sname + tseq;
			for(j = 0; j < MAXNAMES; ++j)
				if(sname[j] == ' ')
					break;
			sname.resize(j);
			blank_to_(sname);

			struct_penalties = SECST;
			struct_index = ix;
			for (i = 0; i < length; ++i) {
				sec_struct_mask[i] = '.';
				gap_penalty_mask[i] = '.';
			}
			ss_name = sname;
			for(i = 0; len < length; ++i) {
				c = tseq[i];
				if(c == '\n' || c == '\0')
					break; /* EOL */
				if (!isspace(c))
					sec_struct_mask[len++] = c;
			}

		}
		else if(header == "!GM") {
			ix++;
			line.resize(3);
			line += sname + tseq;
			for(j = 0; j < MAXNAMES; ++j) 
				if(sname[j] == ' ')
					break;
			sname.resize(j);
			blank_to_(sname);
			struct_penalties = GMASK;
			struct_index = ix;
			for (i = 0; i < length; ++i)
				gap_penalty_mask[i] = '1';
			ss_name = sname;
			for(i = 0; len < length; ++i) {
				c = tseq[i];
				if(c == '\n' || c == '\0')
					break; /* EOL */
				if (!isspace(c))
					gap_penalty_mask[len++] = c;
			}

		}
		if (struct_penalties != NONE)
			break;

	} while (getline(fin, line));

	if (struct_penalties == NONE)
		return;

/* skip any more comment lines */
	while (line[0] == '!') {
		if(!getline(fin, line))
			return;
	}

/* skip the sequence lines and any comments after the alignment */
	for (;;) {
		if(isspace(line[0]))
			break;
		if(!getline(fin, line))
			return;
	}


/* read the rest of the alignment */

	for (;;) {
/* skip any blank lines */
			for (;;) {
				if(!blankline(line))
					break;
				if(!getline(fin, line))
					return;
			}
/* get structure table line */
			for(ix = 0; ix < struct_index; ++ix) {
				if (line[0] != '!') {
					if(struct_penalties == SECST)
						cerr << "bad secondary structure format";
					else
						cerr << "bad gap penalty mask format";
					struct_penalties = NONE;
					return;
				}
				if(!getline(fin, line)) return;
			}
			string header = line.substr(0, 3);
			if(struct_penalties == SECST) {
				if (header == "!SS",3) {
					cerr << "bad secondary structure format";
					struct_penalties = NONE;
					return;
				}

				istringstream line_stream(line.substr(3));
				line_stream >> sname >> tseq;
				for(i = 0; len < length;i++) {
					c = tseq[i];
					if(c == '\n' || c == '\0')
						break; /* EOL */
					if (!isspace(c))
						sec_struct_mask[len++] = c;
				}
			}
			else if (struct_penalties == GMASK) {
				if (header == "!GM",3) {
					cerr << "bad gap penalty mask format";
					struct_penalties = NONE;
					return;
				}
				istringstream line_stream(line.substr(3));
				line_stream >> sname >> tseq;
				for(i = 0; len < length; ++i) {
					c = tseq[i];
					if(c == '\n' || c == '\0')
						break; /* EOL */
					if (!isspace(c))
						gap_penalty_mask[len++] = c;
				}
			}

/* skip any more comment lines */
		while (line[0] == '!') {
			if(!getline(fin, line))
				return;
		}

/* skip the sequence lines */
		for (; getline(fin, line); ) {
			if(isspace(line[0]))
				break;
		}
	}
}

void CMultAlignment::get_gde_ss(int length)
{
	string title, line, lin2, sname;
	int i, len, offset = 0;
	unsigned char c;

	for (;;) {
		line[0] = '\0';
/* search for the next comment line */
		while(line[0] != '"')
			if (!getline(fin, line))
				return;

/* is it a secondary structure entry? */
		string header = line.substr(1, 3);
		if (header == "SS_") {
			for (i = 1; i <= MAXNAMES - 3; ++i) {
				if (line[i + 3] == '(' || line[i + 3] == '\n')
						break;
				sname[i-1] = line[i+3];
			}
			i--;
			sname.resize(i);
			if (sname[i - 1] == '(')
				offset = stoi(line.substr(i + 3));
			else
				offset = 0;
			for(i--; i > 0; --i)
				if(isspace(sname[i])) {
					sname.resize(i);
				}
				else break;
			blank_to_(sname);

			struct_penalties = SECST;
			for (i = 0; i < length; ++i)
				sec_struct_mask[i] = '.';
			len = 0;
			while(getline(fin, line)) {
				if(line[0] == '%' || line[0] == '#' || line[0] == '"')
					break;
				for(i = offset; i < length; ++i) {
					c = line[i];
					if(c == '\n' || c == '\0')
						break;			/* EOL */
					sec_struct_mask[len++]=c;
				}
				if (len > length)
					break;
			}
			ss_name = sname;
		}
/* or is it a gap penalty mask entry? */
		else if (header == "GM_") {
			for (i = 1; i <= MAXNAMES - 3; ++i) {
				if (line[i + 3] == '(' || line[i + 3] == '\n')
						break;
				sname[i - 1] = line[i+3];
			}
			i--;
			sname.resize(i);
			if (sname[i - 1] == '(')
				offset = stoi(line.substr(i + 3));
			else
				offset = 0;
			for(i--; i > 0; --i)
				if(isspace(sname[i])) {
					sname.resize(i);
				}
				else
					break;
			blank_to_(sname);

			struct_penalties = GMASK;
			for (i=0;i<length;i++)
				gap_penalty_mask[i] = '1';
			len = 0;
			while(getline(fin, line)) {
				if(line[0] == '%' || line[0] == '#' || line[0] == '"')
					break;
				for(i=offset;i < length;i++) {
					c=line[i];
					if(c == '\n' || c == '\0')
						break;			/* EOL */
					gap_penalty_mask[len++]=c;
				}
				if (len > length)
					break;
			}
			ss_name = sname;
		}

		if (struct_penalties != NONE)
			break;
	}

}

bool CMultAlignment::linetype(const string& line, const string& code)
{
	return line == code;
}

void CMultAlignment::get_swiss_feature(const string& line)
{
	char c, s;
	int  i, start_pos, end_pos;
	string feature;

	stringstream ss(line);
	ss >> feature >> start_pos >> end_pos;
	if (ss.fail())
		return;

	if (feature == "HELIX") {
		c = 'A';
		s = '$';
	}
	else if (feature == "STRAND") {
		c = 'B';
		s = '%';
	}
	else
		return;

	sec_struct_mask[start_pos - 1] = s;
	for (i = start_pos; i < end_pos - 1; ++i)
		sec_struct_mask[i] = c;
	sec_struct_mask[end_pos - 1] = s;

}

void CMultAlignment::get_swiss_mask(const string& line)
{
	int i, value, start_pos, end_pos;
	stringstream ss(line);
	ss >> value >> start_pos >> end_pos;
	if (ss.fail())
		return;

	if (value < 1 || value > 9)
		return;

	for (i = start_pos - 1; i< end_pos; ++i)
		gap_penalty_mask[i] = value+'0';
}

void CMultAlignment::get_embl_ss(int length)
{
	string title, line, lin2, sname;
	int i;

/* find the start of the sequence entry */
	for (;;) {
		while(!linetype(line, "ID"))
			if (!getline(fin, line))
				return;

		for(i = 5; i <= line.length(); ++i)  /* DES */
			if(line[i] != ' ')
				break;
			sname = line.substr(i); /* remember entryname */
			for(i = 0; i <= sname.length(); ++i)
				if(sname[i] == ' ') {
					sname.resize(i);
					break;
				}

		blank_to_(sname);

/* look for secondary structure feature table / gap penalty mask */
		while(getline(fin, line)) {
			if (linetype(line, "FT")) {
				struct_penalties = SECST;
				for (i = 0; i < length; ++i)
					sec_struct_mask[i] = '.';
				do {
					get_swiss_feature(line.substr(2));
					getline(fin, line);
				} while( linetype(line,"FT") );

				ss_name = sname;
			}
			else if (linetype(line, "GM")) {
				struct_penalties = GMASK;
				for (i = 0; i < length; ++i)
					gap_penalty_mask[i] = '1';
				do {
					get_swiss_mask(line.substr(2));
					getline(fin, line);
				} while(linetype(line, "GM"));

				ss_name = sname;
			}
			else if (linetype(line, "SQ"))
				break;

			if (struct_penalties != NONE)
				break;
		}

	}

}

bool CMultAlignment::keyword(const string& line, const string& code)
{
	int i;
	string key;

	for(i = 0; !isspace(line[i]) && i < line.length(); ++i)
		key[i] = line[i];
	key.resize(i);
	return key == code;
}

void CMultAlignment::get_rsf_feature(const string& line)
{
	char c, s;
	int  i, start_pos, end_pos;
	string str1, str2, feature;

	stringstream ss(line);
	ss >> start_pos >> end_pos >> str1 >> str2 >> feature;
	if (ss.fail())
		return;

	if (feature == "HELIX") {
		c = 'A';
		s = '$';
	}
	else if (feature == "STRAND") {
		c = 'B';
		s = '%';
	}
	else
		return;

	sec_struct_mask[start_pos - 1] = s;
	for (i = start_pos; i < end_pos - 1; ++i)
		sec_struct_mask[i] = c;
	sec_struct_mask[end_pos - 1] = s;

}

void CMultAlignment::get_rsf_ss(int length)
{
	string title, line, lin2, sname;
	int i;

/* skip the comments */
	while (getline(fin, line)) {
		if(line[line.length() - 2] == '.' && line[line.length() - 3] == '.')
			break;
	}

/* find the start of the sequence entry */
	for (;;) {
		while (getline(fin, line))
			if(line[0] == '{' ) break;

		while( !keyword(line,"name") )
			if (!getline(fin, line)) return;

		for(i = 5; i <= line.length(); ++i)  /* DES */
			if(line[i] != ' ') break;
		sname = line.substr(i); /* remember entryname */
		for(i = 0; i <= sname.length(); ++i)
			if(sname[i] == ' ') {
				sname.resize(i);
				break;
			}
		blank_to_(sname);

/* look for secondary structure feature table / gap penalty mask */
		while(getline(fin, line)) {
			if (keyword(line, "feature")) {
				struct_penalties = SECST;
				for (i = 0; i < length; ++i)
					sec_struct_mask[i] = '.';
				do {
					if(keyword(line, "feature"))
						get_rsf_feature(line.substr(7));
					getline(fin, line);
				} while( !keyword(line,"sequence") );

				ss_name = sname;
			}
			else if (keyword(line, "sequence"))
				break;

			if (struct_penalties != NONE)
				break;
		}

	}

}

void CMultAlignment::alloc_aln(int nseqs)
{
	seqlen_array.resize(nseqs + 1);

	seq_array.resize(nseqs + 1);

	names.resize(nseqs + 1);
	titles.resize(nseqs + 1);
	output_index.resize(nseqs + 1);
	tmat.resize(nseqs + 1);
	for(int i = 0; i <= nseqs; ++i) {
		vector<double> row;
		row.resize(nseqs + 1);
		tmat[i] = row;
	}
}

void CMultAlignment::realloc_aln(int first_seq, int nseqs)
{
	int i;

	seqlen_array.resize(first_seq + nseqs + 1);
	seq_array.resize(first_seq + nseqs + 1);

	names.resize(first_seq + nseqs + 1);

	titles.resize(first_seq + nseqs + 1);

	output_index.resize(first_seq + nseqs + 1);

	tmat.resize(first_seq + nseqs + 1);
	for(i = 1; i < first_seq; ++i)
		tmat[i].resize(first_seq + nseqs + 1);
	for(i = first_seq; i < first_seq + nseqs; ++i) {
		vector<double> row;
		row.resize(first_seq + nseqs + 1);
		tmat[i] = row;
	}
}

int CMultAlignment::count_gaps(int s1, int s2, int l)
{
	int g;
	unique_ptr<int[]> Q, R;

	Q = make_unique<int[]>(l+2);
	R = make_unique<int[]>(l+2);

	Q[0] = R[0] = g = 0;

	for (int i = 1; i < l; ++i) {
		int q = (seq_array[s1][i] > max_aa) ? 1 : 0;
		int r = (seq_array[s2][i] > max_aa) ? 1 : 0;

		if (((Q[i - 1] <= R[i - 1]) && (q != 0) && (1 - r != 0)) ||
			 ((Q[i - 1] >= R[i - 1]) && (1 - q != 0) && (r != 0)))
			 g++;

		Q[i] = (q != 0) ? Q[i-1]+1 : 0;
		R[i] = (r != 0) ? R[i-1]+1 : 0;
	}

	return g;
}

void CMultAlignment::init_matrix()
{
	char c1, c2;
	short i, j, maxres;

	max_aa = amino_acid_codes.length() - 2;
	gap_pos1 = NUMRES - 2;			 /* code for gaps inserted by clustalw */
	gap_pos2 = NUMRES - 1;			  /* code for gaps already in alignment */

	def_aa_xref = make_unique<short[]>(NUMRES);
	def_dna_xref = make_unique<short[]>(NUMRES);

/*
	set up cross-reference for default matrices hard-coded in matrices.h
*/
	for (i = 0; i < NUMRES; ++i)
		def_aa_xref[i] = -1;
	for (i = 0; i < NUMRES; ++i)
		def_dna_xref[i] = -1;

	maxres = 0;
	for (i = 0; (c1 = amino_acid_order[i]); ++i) {
		for (j = 0; (c2 = amino_acid_codes[j]); ++j) {
			if (c1 == c2) {
				def_aa_xref[i] = j;
				maxres++;
				break;
			}
		}
		if ((def_aa_xref[i] == -1) && (amino_acid_order[i] != '*'))
			cerr << "residue %c in matrices.h is not recognised" << amino_acid_order[i];
	}

	maxres = 0;
	for (i = 0; (c1 = nucleic_acid_order[i]); ++i) {
		for (j = 0; (c2 = amino_acid_codes[j]); ++j) {
			if (c1 == c2) {
				def_dna_xref[i] = j;
				maxres++;
				break;
			}
		}
		if ((def_dna_xref[i] == -1) && (nucleic_acid_order[i] != '*'))
			cerr << "nucleic acid %c in matrices.h is not recognised" << nucleic_acid_order[i];
	}
}

void CMultAlignment::get_path(const string& str, string& path)
{
	register int i;

	path = str;
	for(i=path.length() - 1; i > -1; --i) {
		if(str[i] == DIRDELIM) {
			i = -1;
			break;
		}
		if(str[i] == '.')
			break;
	}
	if(i < 0)
		path += '.';
}

void CMultAlignment::create_parameter_output()
{
	string parname, temp;
	string path;
	ofstream parout;

	get_path(seqname, path);
	parname = path;
	parname += "par";

/* create a file with execute permissions first */
	remove(parname.c_str());

	wstring wParname(parname.begin(), parname.end());
	if(!open_explicit_file(parout, wParname))
		return;

	parout << "clustalw \\\n";
	if (!isEmpty && profile1_empty)
		parout << "/infile=" << seqname << " \\\n";
	if (!profile1_empty)
		parout << "/profile1=" << profile1_name << " \\\n";
	if (!profile2_empty)
		parout << "/profile2=" << profile2_name << " \\\n";
	if (dnaflag)
		parout << "/type=dna \\\n";
	else
		parout << "/type=protein \\\n";

	if (quick_pairalign) {
		parout << "/quicktree \\\n";
		parout << "/ktuple=" << ktup << " \\\n";
		parout << "/window=" << window << " \\\n";
		parout << "/pairgap=" << wind_gap << " \\\n";
		parout << "/topdiags=" << signif << " \\\n";
		if (percent)
			parout << "/score=percent \\\n";
		else
			parout << "/score=absolute \\\n";
	}
	else {
		if (!dnaflag) {
			parout << "/pwmatrix=" << pw_mtrxname << " \\\n";
			parout << "/pwgapopen=" << fixed << setprecision(2) << prot_pw_go_penalty << " \\\n";
			parout << "/pwgapext=" << prot_pw_ge_penalty << " \\\n";
		}
		else {
			parout << "/pwgapopen=" << fixed << setprecision(2) << pw_go_penalty << " \\\n";
			parout << "/pwgapext=" << pw_ge_penalty << " \\\n";
		}
	}

	if (!dnaflag) {
		parout << "/matrix=" << mtrxname << " \\\n";
		parout << "/gapopen=" << fixed << setprecision(2) << prot_gap_open << " \\\n";
		parout << "/gapext=" << prot_gap_extend << " \\\n";
	}
	else {
		parout << "/gapopen=" << fixed << setprecision(2) << dna_gap_open << " \\\n";
		parout << "/gapext=" << dna_gap_extend << "\\\n";
	}

	parout << "/maxdiv=" << divergence_cutoff << "\\\n";
	if (!use_endgaps)
		parout << "/endgaps \\\n";

	if (!dnaflag) {
		if (neg_matrix)
			parout << "/negative \\\n";
		if (no_pref_penalties)
			parout << "/nopgap \\\n";
		if (no_hyd_penalties)
			parout << "/nohgap \\\n";
		parout << "/hgapresidues=" << hyd_residues << " \\\n";
		parout << "/gapdist=" << gap_dist << " \\\n";
	}
	else
		parout << "/transweight=" << fixed << setprecision(2) << transition_weight << " \\\n";

	if (output_gcg)
		parout << "/output=gcg \\\n";
	else if (output_gde)
		parout << "/output=gde \\\n";
	else if (output_nbrf)
		parout << "/output=pir \\\n";
	else if (output_phylip)
		parout << "/output=phylip \\\n";

	if (outfile_name.length() > 0)
		parout << "/outfile=" << outfile_name.c_str() << " \\\n";
	if (output_order == ALIGNED)
		parout << "/outorder=aligned \\\n";
	else
		parout << "/outorder=input \\\n";

	if (output_gde) {
		if (lowercase)
			parout << "/case=lower \\\n";
		else
			parout << "/case=upper \\\n";
	}

	parout << "/interactive\n";

	parout.close();
}

void CMultAlignment::reset_align()	/* remove gaps from older alignments (code = gap_pos1) */
{								/* EXCEPT for gaps that were INPUT with the seqs.*/
	register int sl;				  /* which have  code = gap_pos2  */
	int i, j;

	for(i = 1; i <= nseqs; ++i) {
		sl = 0;
		for(j = 1; j <= seqlen_array[i]; ++j) {
			if(seq_array[i][j] == gap_pos1)
				continue;
			++sl;
			seq_array[i][sl] = seq_array[i][j];
		}
		seqlen_array[i] = sl;
	}
}

short CMultAlignment::align(wstring& phylip_name)
{
	string path;
	ofstream tree;
	int count;

	struct_penalties1 = struct_penalties2 = NONE;
	sec_struct_mask1.clear();
	sec_struct_mask2.clear();
	gap_penalty_mask1.clear();
	gap_penalty_mask2.clear();
	ss_name1.clear();
	ss_name2.clear();

	get_path(seqname, path);
	wstring wPath(path.begin(), path.end());

	if(!open_alignment_output(wPath))
		return 0;

	if (nseqs > 3)
		get_path(seqname, path);

	if (phylip_name.length() > 0) {
		if(!open_explicit_file(tree, phylip_name))
			return 0;

		if(open_output_file(tree, wPath, phylip_name, "dnd"))
			return 0;
	}

	if (save_parameters)
		create_parameter_output();

	if(reset_alignments)
		reset_align();

	if(dnaflag) {
		gap_open = dna_gap_open;
		gap_extend = dna_gap_extend;
		pw_go_penalty = dna_pw_go_penalty;
		pw_ge_penalty = dna_pw_ge_penalty;
		ktup = dna_ktup;
		window = dna_window;
		signif = dna_signif;
		wind_gap = dna_wind_gap;
	}
	else {
		gap_open = prot_gap_open;
		gap_extend = prot_gap_extend;
		pw_go_penalty  = prot_pw_go_penalty;
		pw_ge_penalty  = prot_pw_ge_penalty;
		ktup = prot_ktup;
		window = prot_window;
		signif = prot_signif;
		wind_gap = prot_wind_gap;
	}

	if (quick_pairalign)
		show_pair();
	else if(!pairalign(0, nseqs, 0, nseqs)) {
		clustal_outfile.close();
		return 0;
	}

	if (nseqs > 3)
		guide_tree(1, nseqs);

	count = malign(0, phylip_name);

	if (count <= 0) {
		clustal_outfile.close();
		return 0;
	}

	create_alignment_output(1, nseqs);
	phylip_name.clear();
	clustal_outfile.close();
	return 1;
}

void CMultAlignment::profile_align(string& p1_tree_name,string& p2_tree_name)
{
	//nothing ??
}

void CMultAlignment::make_tree(wstring& phylip_name)
{
	//all commented out
}

void CMultAlignment::get_tree(wstring& phylip_name)
{
	//nothing ??
}

int CMultAlignment::profile_input()	/* read a profile	*/
{														 /* profile_no is 1 or 2  */
	int local_nseqs;

	if(profile_no == 2 && profile1_empty)  {
		cerr << "You must read in profile number 1 first";
		return 0;
	}

	if(profile_no == 1) {	 /* for the 1st profile */
		local_nseqs = readseqs(1); /* (1) means 1st seq to be read = no. 1 */
		if(local_nseqs == 0) {			/* no sequences  */
			cerr << "No sequences in file!  Bad format?";
		}
		else if (local_nseqs > 0) { /* success; found some seqs. */
			struct_penalties1 = NONE;
			sec_struct_mask1.clear();
			gap_penalty_mask1.clear();
			ss_name1.clear();
			if (struct_penalties != NONE) { /* feature table / mask in alignment */
				struct_penalties1 = struct_penalties;
				if (struct_penalties == SECST)
					sec_struct_mask1 = sec_struct_mask;

				gap_penalty_mask1 = gap_penalty_mask;
				ss_name1 = ss_name;
			}
			nseqs = profile1_nseqs = local_nseqs;
			cout << "No. of seqs=" << nseqs << '\n';
			profile1_empty=false;
			profile2_empty=true;
		}
	}
	else { /* first seq to be read = profile1_nseqs + 1 */
		local_nseqs = readseqs(profile1_nseqs + 1);
		if(local_nseqs == 0) { /* no sequences */
			cerr << "No sequences in file!  Bad format?";
		}
		else if(local_nseqs > 0) {
			struct_penalties2 = NONE;
			sec_struct_mask2.clear();
			gap_penalty_mask2.clear();
			ss_name2.clear();
			if (struct_penalties != NONE) { /* feature table / mask in alignment */
				struct_penalties2 = struct_penalties;
				if (struct_penalties == SECST)
					sec_struct_mask2 = sec_struct_mask;

				gap_penalty_mask2 = gap_penalty_mask;
				ss_name2 = ss_name;
			}
			cout << "No. of seqs in profile=" << local_nseqs << '\n';
			nseqs = profile1_nseqs + local_nseqs;
			cout << "Total no. of seqs	  =" << nseqs << '\n';
			profile2_empty=false;
			isEmpty = false;
		}

	}
	sec_struct_mask.clear();
	gap_penalty_mask.clear();
	ss_name.clear();

	if(local_nseqs <= 0)
		return local_nseqs;

	cout << "Sequences assumed to be " << (dnaflag ? "DNA" : "PROTEIN") << '\n';
	if(dnaflag) {
		gap_open = dna_gap_open;
		gap_extend = dna_gap_extend;
	}
	else {
		gap_open = prot_gap_open;
		gap_extend = prot_gap_extend;
	}

	return nseqs;
}

float CMultAlignment::countid(int s1, int s2)
{
	int count, total;
	float score;

	count = total = 0;
	for (int i = 1; i <= seqlen_array[s1] && i <= seqlen_array[s2]; ++i) {
		char c1 = seq_array[s1][i];
		char c2 = seq_array[s2][i];
		if ((c1>=0) && (c1<max_aa)) {
			total++;
			if (c1 == c2)
				count++;
		}
	}

	score = 100.0f * count / (float) total;
	return score;
}

void CMultAlignment::reset_prf1()	/* remove gaps from older alignments (code = gap_pos1) */
{								/* EXCEPT for gaps that were INPUT with the seqs.*/
	register int sl;				  /* which have  code = gap_pos2  */
	int i, j;

	if (struct_penalties1 != NONE) {
		sl = 0;
		for (j = 0; j < seqlen_array[1]; ++j) {
			if (gap_penalty_mask1[j] == gap_pos1)
				continue;
			gap_penalty_mask1[sl]=gap_penalty_mask1[j];
			++sl;
		}
	}

	if (struct_penalties1 == SECST) {
		sl=0;
		for (j = 0; j < seqlen_array[1]; ++j) {
			if (sec_struct_mask1[j] == gap_pos1)
				continue;
			sec_struct_mask1[sl]=sec_struct_mask1[j];
			++sl;
		}
	}

	for(i = 1; i <= profile1_nseqs; ++i) {
		sl = 0;
		for(j = 1; j <= seqlen_array[i]; ++j) {
			if(seq_array[i][j] == gap_pos1)
				continue;
			++sl;
			seq_array[i][sl]=seq_array[i][j];
		}
		seqlen_array[i]=sl;
	}

}

void CMultAlignment::reset_prf2()	/* remove gaps from older alignments (code = gap_pos1) */
{								/* EXCEPT for gaps that were INPUT with the seqs.*/
	register int sl;				  /* which have  code = gap_pos2  */

	if (struct_penalties2 != NONE) {
		sl = 0;
		for (int j = 0; j < seqlen_array[profile1_nseqs + 1]; ++j) {
			if (gap_penalty_mask2[j] == gap_pos1)
				continue;
			gap_penalty_mask2[sl] = gap_penalty_mask2[j];
			++sl;
		}
	}

	if (struct_penalties2 == SECST) {
		sl = 0;
		for (int j = 0; j < seqlen_array[profile1_nseqs + 1]; ++j) {
			if (sec_struct_mask2[j] == gap_pos1)
				continue;
			sec_struct_mask2[sl] = sec_struct_mask2[j];
			++sl;
		}
	}

	for(int i = profile1_nseqs + 1; i <= nseqs; ++i) {
		sl = 0;
		for(int j = 1; j <= seqlen_array[i]; ++j) {
			if(seq_array[i][j] == gap_pos1)
				continue;
			++sl;
			seq_array[i][sl] = seq_array[i][j];
		}
		seqlen_array[i] = sl;
	}

}

void CMultAlignment::fix_gaps()	/* fix gaps introduced in older alignments (code = gap_pos1) */
{
	if (struct_penalties1 != NONE) {
		for (int j = 0; j < seqlen_array[1]; ++j) {
			if (gap_penalty_mask1[j] == gap_pos1)
				gap_penalty_mask1[j] = gap_pos2;
		}
	}

	if (struct_penalties1 == SECST) {
		for (int j = 0; j < seqlen_array[1]; ++j) {
			if (sec_struct_mask1[j] == gap_pos1)
				sec_struct_mask1[j] = gap_pos2;
		}
	}

	for(int i = 1; i <= nseqs; ++i) {
		for(int j = 1; j <= seqlen_array[i]; ++j) {
			if(seq_array[i][j] == gap_pos1)
				seq_array[i][j] = gap_pos2;
		}
	}
}

void CMultAlignment::calc_p_penalties(vector<string>& aln, int n, int fs, int ls, int *weight)
{
	char ix;
	int j, k, numseq;
	int i;

	numseq = ls - fs;
	for (i = 0; i < n; ++i)
	{
		weight[i] = 0;
		for (k = fs; k < ls; k++)
		{
			for (j = 0; j < 22; ++j)
			{
				ix = aln[k][i];
				if (ix < 0 || ix > max_aa)
					continue;
				if (amino_acid_codes[ix] == pr[j])
				{
					weight[i] += (200 - pas_op[j]);
					break;
				 }
			}
		}
	  weight[i] /= numseq;
	}

}

void CMultAlignment::calc_h_penalties(vector<string>& aln, int n, int fs, int ls, int *weight)
{

/*
	weight[] is the length of the hydrophilic run of residues.
*/
	char ix;
	int nh, j, k;
	int i, e, s;
	unique_ptr<int[]> hyd;
	float scale;

	hyd = make_unique<int[]>(n + 2);
	nh = hyd_residues.length();
	for (i = 0; i < n; ++i)
		weight[i] = 0;

	for (k = fs; k < ls; k++)
	{
		for (i = 0; i < n; ++i)
		{
			hyd[i] = 0;
			for (j = 0 ; j < nh; ++j) {
				ix = aln[k][i];
				if (ix < 0 || ix > max_aa)
					continue;
				if (amino_acid_codes[ix] == hyd_residues[j])
				{
					hyd[i] = 1;
					break;
				}
			}
		}
		i = 0;
		while (i < n)
		{
			if (hyd[i] == 0)
				i++;
			else {
				s = i;
				while (hyd[i] != 0 && i < n)
					i++;
				e = i;
				if (e - s > 3)
					for (j = s; j < e; ++j)
						weight[j] += 100;
			}
		 }
	}

	scale = ls - fs;
	for (i = 0; i < n; ++i)
		weight[i] /= scale;

	hyd.reset();
}

int CMultAlignment::local_penalty(int penalty, int n, int *pweight, int *hweight)
{
	if (dnaflag)
		return 1;

	bool h = false;
	float gw = 1.0;
	if (nhyd_pen == false)
	{
		if (hweight[n] > 0)
		{
			gw *= reduced_gap;
			h = true;
		}
	}
	if ((npref_pen == false) && (h == false))
	{
		gw *= ((float) pweight[n] / 100.0);
	}

	gw *= penalty;
	return gw;

}

void CMultAlignment::calc_gap_coeff(vector<string>& alignment, int *gaps, vector<vector<int> >& profile, bool struct_penalties,
	string& gap_penalty_mask, int first_seq, int last_seq, int prf_length, int gapcoef, int lencoef)
{
	char c;
	int i, j;
	int is, ie;
	int numseq, val;
	unique_ptr<int[]> gap_pos, p_weight, h_weight;
	float scale;

	numseq = last_seq - first_seq;

	for (j = 0; j < prf_length; ++j)
		gaps[j] = 0;
/*
	Check for a gap penalty mask
*/
	if (struct_penalties != NONE) {
		nhyd_pen = npref_pen = true;
		gdist = 0;
	}
	else {
		nhyd_pen = no_hyd_penalties;
		npref_pen = no_pref_penalties;
		gdist = gap_dist;
	}

	for (i = first_seq; i < last_seq; ++i) {
/*
	Include end gaps as gaps ?
*/
		is = 0;
		ie = prf_length;
		if (!use_endgaps) {
			for (j = 0; j < prf_length; ++j) {
				c = alignment[i][j];
				if ((c < 0) || (c > max_aa))
					is++;
				else
					break;
			}
			for (j = prf_length - 1; j >= 0; --j) {
				c = alignment[i][j];
				if ((c < 0) || (c > max_aa))
					ie--;
				else
					break;
			}
		}

		for (j = is; j < ie; ++j) {
			if (alignment[i][j] < 0 || alignment[i][j] > max_aa)
				gaps[j]++;
		}
	}

	if ((!dnaflag) && (npref_pen == false)) {
		p_weight = make_unique<int[]>(prf_length + 2);
		calc_p_penalties(alignment, prf_length, first_seq, last_seq, p_weight.get());
	}

	if ((!dnaflag) && (nhyd_pen == false)) {
		h_weight = make_unique<int[]>(prf_length + 2);
		calc_h_penalties(alignment, prf_length, first_seq, last_seq, h_weight.get());
	}

	gap_pos = make_unique<int[]>(prf_length + 2);
/*
	mark the residues close to an existing gap (set gaps[i] = -ve)
*/
	if (dnaflag || (gdist <= 0)) {
		for (i = 0; i < prf_length; ++i)
			gap_pos[i] = gaps[i];
	}
	else {
		i = 0;
		while (i < prf_length) {
			if (gaps[i] <= 0) {
				gap_pos[i] = gaps[i];
				i++;
			}
			else {
				for (j = -gdist + 1; j < 0; ++j) {
					if ((i + j >= 0) && (i + j < prf_length) && ((gaps[i + j] == 0) || (gaps[i + j] < j)))
						gap_pos[i + j] = j;
				}
				while (gaps[i] > 0) {
					if (i >= prf_length)
						break;
					gap_pos[i] = gaps[i];
					i++;
				}
				for (j = 0; j < gdist; ++j) {
					if (gaps[i + j] > 0)
						break;
					if ((i + j >= 0) && (i + j < prf_length) && ((gaps[i + j] == 0) || (gaps[i + j] < -j)))
						gap_pos[i + j] = - j - 1;
				}
				i += j;
			}
		}
	}

	for (j = 0; j < prf_length; ++j) {
		if (gap_pos[j] <= 0) {
/*
		apply residue-specific and hydrophilic gap penalties.
*/
			if (!dnaflag) {
				profile[j + 1][GAPCOL] = local_penalty(gapcoef, j, p_weight.get(), h_weight.get());
				profile[j + 1][LENCOL] = lencoef;
			}
			else {
				profile[j + 1][GAPCOL] = gapcoef;
				profile[j + 1][LENCOL] = lencoef;
			}
/*
		increase gap penalty near to existing gaps.
*/
			if (gap_pos[j] < 0)
				profile[j + 1][GAPCOL] *= 2.0 + 2.0 * (gdist + gap_pos[j]) / gdist;
		}
		else {
			scale = ((float)(numseq - gaps[j]) / (float) numseq) * reduced_gap;
			profile[j + 1][GAPCOL] = scale*gapcoef;
			profile[j + 1][LENCOL] = 0.5 * lencoef;
		}
/*
		apply the gap penalty mask
*/
		if (struct_penalties != NONE) {
			val = gap_penalty_mask[j] - '0';
			if (val > 0 && val < 10) {
				profile[j + 1][GAPCOL] *= val;
				profile[j + 1][LENCOL] *= val;
			}
		}
/*
	make sure no penalty is zero - even for all-gap positions
*/
		if (profile[j + 1][GAPCOL] <= 0)
			profile[j + 1][GAPCOL] = 1;
		if (profile[j + 1][LENCOL] <= 0)
			profile[j + 1][LENCOL] = 1;
	}

	profile[0][GAPCOL] = 0;
	profile[0][LENCOL] = 0;

	profile[prf_length][GAPCOL] = 0;
	profile[prf_length][LENCOL] = 0;

	if (!dnaflag && !npref_pen)
		p_weight.reset();

	if (!dnaflag && !nhyd_pen)
		h_weight.reset();

	gap_pos.reset();
}

void CMultAlignment::calc_gap_penalty_mask(int prf_length, const string& mask, string& gap_mask)
{
	int i,j;
	string struct_mask;

	struct_mask.resize(prf_length + 1);
/*
	calculate the gap penalty mask from the secondary structures
*/
	i = 0;
	while (i < prf_length) {
		if (tolower(mask[i]) == 'a' || mask[i] == '$') {
			for (j = -helix_end_plus; j < 0; ++j) {
				if ((i + j>=0) && (tolower(struct_mask[i + j]) != 'a')
					&& (tolower(struct_mask[i + j]) != 'b'))
					struct_mask[i + j] = 'a';
			}
			for (j = 0; j < helix_end_minus; ++j) {
				if (i + j >= prf_length || (tolower(mask[i + j]) != 'a'
					&& mask[i + j] != '$'))
					break;
				struct_mask[i + j] = 'a';
			}
			i += j;
			while (tolower(mask[i]) == 'a' || mask[i] == '$') {
				if (i>=prf_length)
					break;
				if (mask[i] == '$') {
					struct_mask[i] = 'A';
					i++;
					break;
				}
				else
					struct_mask[i] = mask[i];
				i++;
			}
			for (j = 0; j < helix_end_minus; ++j) {
				if ((i - j - 1>=0) && (tolower(mask[i - j - 1]) == 'a'
					|| mask[i - j - 1] == '$'))
					struct_mask[i - j - 1] = 'a';
			}
			for (j = 0; j < helix_end_plus; ++j) {
				if (i + j >= prf_length)
					break;
				struct_mask[i + j] = 'a';
			}
		}
		else if (tolower(mask[i]) == 'b' || mask[i] == '%') {
			for (j = -strand_end_plus; j < 0; ++j) {
				if ((i + j>=0) && (tolower(struct_mask[i + j]) != 'a')
					&& (tolower(struct_mask[i + j]) != 'b'))
					struct_mask[i + j] = 'b';
			}
			for (j = 0; j<strand_end_minus; ++j) {
				if (i + j>=prf_length || (tolower(mask[i + j]) != 'b'
					&& mask[i + j] != '%'))
					break;
				struct_mask[i + j] = 'b';
			}
			i += j;
			while (tolower(mask[i]) == 'b' || mask[i] == '%') {
				if (i >= prf_length)
					break;
				if (mask[i] == '%') {
					struct_mask[i] = 'B';
					i++;
					break;
				}
				else
					struct_mask[i] = mask[i];
				i++;
			}
			for (j = 0; j<strand_end_minus; ++j) {
				if ((i - j - 1 >= 0) && (tolower(mask[i - j - 1]) == 'b'
					|| mask[i - j - 1] == '%'))
				struct_mask[i - j - 1] = 'b';
			}
			for (j = 0; j < strand_end_plus; ++j) {
				if (i + j >= prf_length)
					break;
				struct_mask[i + j] = 'b';
			}
		}
		else
			i++;
	}

	for(i=0;i<prf_length;i++) {
		switch (struct_mask[i]) {
			case 'A':
				gap_mask[i] = helix_penalty + '0';
				break;
			case 'a':
				gap_mask[i] = helix_end_penalty + '0';
				break;
			case 'B':
				gap_mask[i] = strand_penalty + '0';
				break;
			case 'b':
				gap_mask[i] = strand_end_penalty + '0';
				break;
			default:
				gap_mask[i] = loop_penalty + '0';
				break;
		}
	}

	struct_mask.clear();
}

void CMultAlignment::calc_prf1(vector<vector<int> >& profile, vector<string>& alignment, int *gaps, vector<vector<int> >& matrix,
	int *seq_weight, int prf_length, int first_seq, int last_seq)
{
	int sum2, i, res;
	int numseq;
	int f;
	float scale;
	vector<vector<int> > weighting;

	weighting.resize(NUMRES + 2);
	for (i = 0; i < NUMRES + 2; ++i) {
		vector<int> row(prf_length + 2);
		weighting[i] = row;
	}

	numseq = last_seq - first_seq;

	sum2 = 0;
	for (i = first_seq; i < last_seq; ++i)
		sum2 += seq_weight[i];

	for (int r = 0; r < prf_length; r++) {
		for (int d = 0; d <= max_aa; d++) {
			weighting[d][r] = 0;
			for (i = first_seq; i < last_seq; ++i)
				if (d == alignment[i][r])
					weighting[d][r] += seq_weight[i];
		}
		weighting[gap_pos1][r] = 0;
		for (i = first_seq; i < last_seq; ++i)
		if (gap_pos1 == alignment[i][r])
			weighting[gap_pos1][r] += seq_weight[i];
		weighting[gap_pos2][r] = 0;
		for (i = first_seq; i < last_seq; ++i) {
			if (gap_pos2 == alignment[i][r])
				weighting[gap_pos2][r] += seq_weight[i];
		}
	}

	for (int pos = 0; pos < prf_length; pos++) {
		if (gaps[pos] == numseq) {
			for (res = 0; res <= max_aa; res++)
				profile[pos+1][res] = matrix[res][gap_pos1];
			profile[pos+1][gap_pos1] = matrix[gap_pos1][gap_pos1];
			profile[pos+1][gap_pos2] = matrix[gap_pos2][gap_pos1];
		}
		else {
			scale = (float)(numseq-gaps[pos]) / (float)numseq;
			for (res = 0; res <= max_aa; res++) {
				f = 0;
				for (int d = 0; d <= max_aa; d++)
					f += (weighting[d][pos] * matrix[d][res]);
				f += (weighting[gap_pos1][pos] * matrix[gap_pos1][res]);
				f += (weighting[gap_pos2][pos] * matrix[gap_pos2][res]);
				profile[pos+1][res] = (int)(((float) f / (float)sum2)*scale);
			}
			f = 0;
			for (int d = 0; d <= max_aa; d++)
				f += (weighting[d][pos] * matrix[d][gap_pos1]);
			f += (weighting[gap_pos1][pos] * matrix[gap_pos1][gap_pos1]);
			f += (weighting[gap_pos2][pos] * matrix[gap_pos2][gap_pos1]);
			profile[pos+1][gap_pos1] = (int)(((float)f / (float)sum2)*scale);
			f = 0;
			for (int d = 0; d <= max_aa; d++)
				f += (weighting[d][pos] * matrix[d][gap_pos2]);
			f += (weighting[gap_pos1][pos] * matrix[gap_pos1][gap_pos2]);
			f += (weighting[gap_pos2][pos] * matrix[gap_pos2][gap_pos2]);
			profile[pos+1][gap_pos2] = (int)(((float)f / (float)sum2)*scale);
		}
	}

	weighting.clear();
}

void CMultAlignment::calc_prf2(vector<vector<int> >& profile, vector<string>& alignment, int *seq_weight, int prf_length,
	int first_seq, int last_seq)
{
	int sum1, sum2;
	int i;

	for (int r = 0; r < prf_length; r++) {
/*
		calculate sum2 = number of residues found in this column
*/
		sum2 = 0;
		for (i = first_seq; i < last_seq; ++i)
			sum2 += seq_weight[i];
/*
		only include matrix comparison scores for those residue types found in this
		column
*/
		if (sum2 == 0) {
			for (int d = 0; d <= max_aa; d++)
				profile[r + 1][d] = 0;
			profile[r + 1][gap_pos1] = 0;
			profile[r + 1][gap_pos2] = 0;
		}
		else {
			for (int d = 0; d <= max_aa; d++) {
				sum1 = 0;
				for (i = first_seq; i < last_seq; ++i) {
					if (d == alignment[i][r])
						sum1 += seq_weight[i];
				}
				profile[r + 1][d] = (int)(10 * (float)sum1 / (float)sum2);
			}
			sum1 = 0;
			for (i = first_seq; i < last_seq; ++i) {
				if (gap_pos1 == alignment[i][r])
					sum1 += seq_weight[i];
			}
			profile[r + 1][gap_pos1] = (int)(10 * (float) sum1 / (float) sum2);
			sum1 = 0;
			for (i = first_seq; i < last_seq; ++i) {
				if (gap_pos2 == alignment[i][r])
					sum1 += seq_weight[i];
			}
			profile[r + 1][gap_pos2] = (int)(10 * (float) sum1 / (float) sum2);
		}
	}
}

int CMultAlignment::calc_weight(int leaf)
{
	float weight = 0.0;

	auto p = olptr[leaf];
	while (p->parent != nullptr)
	{
		weight += p->dist / p->order;
		p = p->parent;
	}

	weight *= 100.0;
	return weight;
}

void CMultAlignment::calc_seq_weights(int first_seq, int last_seq, int* sweight)
{
	int i, nseqs;
	int temp;
	unique_ptr<int[]> weight;

/*
	If there are more than three sequences....
*/
	nseqs = last_seq-first_seq;
	if (nseqs > 3 && distance_tree) {
/*
	Calculate sequence weights based on Phylip tree.
*/
		weight = make_unique<int[]>(last_seq+1);

		for (i = first_seq; i < last_seq; ++i)
			weight[i] = calc_weight(i);

/*
	Normalise the weights, such that the sum of the weights = INT_SCALE_FACTOR
*/

		int sum = 0;
		for (i = first_seq; i < last_seq; ++i)
			sum += weight[i];

		if (sum == 0) {
			for (i = first_seq; i < last_seq; ++i)
				weight[i] = 1;
			sum = i;
		}

		for (i = first_seq; i < last_seq; ++i) {
			sweight[i] = (weight[i] * INT_SCALE_FACTOR) / sum;
			if (sweight[i] < 1)
				sweight[i] = 1;
		}

	}

	else {
/*
  Otherwise, use identity weights.
*/
		temp = INT_SCALE_FACTOR / nseqs;
		for (i = first_seq; i < last_seq; ++i)
			sweight[i] = temp;
	}

}

void CMultAlignment::mark_group1(treeptr p, int *groups, int n)
{
	for (int i = 0; i < n; ++i) {
		if (olptr.get()[i] == p)
			groups[i] = 1;
		else
			groups[i] = 0;
	}
}

void CMultAlignment::mark_group2(treeptr p, int *groups, int n)
{
	for (int i = 0; i < n; ++i) {
		if (olptr.get()[i] == p)
			groups[i] = 2;
		else if (groups[i] != 0)
			groups[i] = 1;
	}
}

void CMultAlignment::save_set(int n, int *groups)
{
	for (int i = 0; i < n; ++i)
		sets[nsets + 1][i + 1] = groups[i];
	nsets++;
}

void CMultAlignment::skip_space(ifstream& fd1)
{
	char c;
	do {
		c = fd1.get();
	} while(isspace(c));

	fd1.unget();
}

void CMultAlignment::group_seqs(treeptr p, int *next_groups, int nseqs)
{
	int i;

	auto tmp_groups = make_unique<int[]>(nseqs + 1);

	if (p->left) {
		if (p->left->leaf == NODE) {
			group_seqs(p->left.get(), next_groups, nseqs);
			for (i = 0; i < nseqs; ++i)
			 if (next_groups[i] != 0)
				 tmp_groups[i] = 1;
		}
		else {
			mark_group1(p->left.get(), tmp_groups.get(), nseqs);
		}
	}

	if (p->right) {
		 if (p->right->leaf == NODE) {
			group_seqs(p->right.get(), next_groups, nseqs);
			for (i = 0; i < nseqs; ++i)
				if (next_groups[i] != 0)
					tmp_groups[i] = 2;
		}
		else {
			mark_group2(p->right.get(), tmp_groups.get(), nseqs);
		}
		save_set(nseqs, tmp_groups.get());
		}
	for (i = 0; i < nseqs; ++i)
		next_groups[i] = tmp_groups[i];

}

void CMultAlignment::create_sets(int first_seq, int last_seq)
{
	nsets = 0;
	nseqs = last_seq - first_seq;
	if (nseqs > 3) {
/*
	If there are more than three sequences....
*/
		auto groups = make_unique<int[]>(nseqs + 1);
		group_seqs(root, groups.get(), nseqs);

	}
	else {
		auto groups = make_unique<int[]>(nseqs + 1);
		for (int i = 0;i < nseqs - 1; ++i) {
			for (int j = 0; j < nseqs; ++j) {
				if (j <= i)
					groups[j] = 1;
				else if (j == i + 1)
					groups[j] = 2;
				else
					groups[j] = 0;
			}
			save_set(nseqs, groups.get());
		}
	}
}

void CMultAlignment::set_info(treeptr p, treeptr parent, int pleaf, const char* pname, float pdist)
{
	p->parent = parent;
	p->leaf = pleaf;
	p->dist = pdist;
	p->order = 0;
	p->name = pname;
	if (p->leaf) {
		p->left = nullptr;
		p->right = nullptr;
	}
}

shared_ptr<node> CMultAlignment::avail()
{
	return make_shared<node>();
}

void CMultAlignment::create_node(treeptr pptr, treeptr parent)
{
	pptr->parent = parent;
	pptr->left = avail();
	pptr->right = avail();
}

treeptr CMultAlignment::insert_node(treeptr pptr)
{
	auto newnode = avail();
	create_node(newnode.get(), pptr->parent);

	newnode->left = pptr->shared_from_this();
	pptr->parent = newnode.get();

	set_info(newnode.get(), pptr->parent, NODE, "", 0.0f);

	return newnode.get();
}

void CMultAlignment::create_tree(treeptr ptree, treeptr parent)
{
	treeptr p;

	int i, type;
	float dist;
	string name;

/*
  is this a node or a leaf ?
*/
	skip_space(fd);
	char ch = fd.get();
	if (ch == '(') {
/*
	this must be a node....
*/
		type = NODE;
		ptrs[ntotal] = nptr[nnodes] = ptree;
		nnodes++;
		ntotal++;

		create_node(ptree, parent);

		p = ptree->left.get();
		create_tree(p, ptree);

		if (ch == ',') {
			p = ptree->right.get();
			create_tree(p, ptree);
			if (ch == ',') {
				ptree = insert_node(ptree);
				ptrs[ntotal] = nptr[nnodes] = ptree;
				nnodes++;
				ntotal++;
				p = ptree->right.get();
				create_tree(p, ptree);
				rooted_tree = false;
			}
		}

		skip_space(fd);
		ch = fd.get();
	}
/*
	...otherwise, this is a leaf
*/
	else
	{
		type = LEAF;
		ptrs[ntotal++] = lptr[numseq++] = ptree;
/*
	get the sequence name
*/
		name[0] = ch;
		ch = fd.get();
		i = 1;
		while ((ch != ':') && (ch != ',') && (ch != ')')) {
			if (i < MAXNAMES)
				name[i++] = ch;
			ch = fd.get();
		}
		name[i] = '\0';
		if (ch != ':') {
			distance_tree = false;
			dist = 0.0;
		}
	}

/*
	get the distance information
*/
	dist = 0.0;
	if (ch == ':') {
		skip_space(fd);
		fd >> dist;
		skip_space(fd);
		ch = fd.get();
	}
	set_info(ptree, parent, type, name.c_str(), dist);

}

float CMultAlignment::calc_root_mean(treeptr root, float *maxdist)
{
	float dist, lsum = 0.0, rsum = 0.0, lmean, rmean, diff;
	int i;
	int nl, nr;
	int direction;
/*
	for each leaf, determine whether the leaf is left or right of the root.
*/
	dist = (*maxdist) = 0;
	nl = nr = 0;
	for(i = 0; i < numseq; ++i)
	{
		auto p = lptr[i];
		dist = 0.0;
		while (p->parent != root) {
			dist += p->dist;
			p = p->parent;
		}

		if (p == root->left.get())
			direction = LEFT;
		else
			direction = RIGHT;
		dist += p->dist;

		if (direction == LEFT) {
			lsum += dist;
			nl++;
		}
		else {
			rsum += dist;
			nr++;
		}
		if (dist > (*maxdist))
			*maxdist = dist;
	}

	lmean = lsum / nl;
	rmean = rsum / nr;

	diff = lmean - rmean;
	return diff;
}

float CMultAlignment::calc_mean(treeptr nptr, float *maxdist, int nseqs)
{
	float dist , lsum = 0.0, rsum = 0.0, lmean, rmean, diff;
	int depth = 0, i, j , n = 0;
	int nl , nr;
	int direction, found;

	auto path2root = make_unique<treeptr[]>(nseqs);
	auto dist2node = make_unique<float[]>(nseqs);
/*
	determine all nodes between the selected node and the root;
*/
	depth = (*maxdist) = dist = 0;
	nl = nr = 0;
	auto p = nptr;
	while (p != nullptr) {
		path2root[depth] = p;
		dist += p->dist;
		dist2node[depth] = dist;
		p = p->parent;
		depth++;
	}

/*
	*nl = *nr = 0;
	for each leaf, determine whether the leaf is left or right of the node.
	(RIGHT = descendant, LEFT = not descendant)
*/
	for (i = 0; i < numseq; ++i) {
		p = lptr[i];
		if (p == nptr) {
			direction = RIGHT;
			dist = 0.0;
		}
		else {
			direction = LEFT;
			dist = 0.0;
/*
		find the common ancestor.
*/
			found = false;
			n = 0;
			while (!found && p->parent != nullptr) {
				for (j = 0; j < depth; ++j)
					if (p->parent == path2root[j]) {
						found = true;
						n = j;
					}
				dist += p->dist;
				p = p->parent;
			}
			if (p == nptr)
				direction = RIGHT;
		}

		if (direction == LEFT) {
			lsum += dist;
			lsum += dist2node[n - 1];
			nl++;
		}
		else {
			rsum += dist;
			nr++;
		}

		if (dist > (*maxdist))
			*maxdist = dist;
	}

	lmean = lsum / nl;
	rmean = rsum / nr;

	diff = lmean - rmean;
	return(diff);
}

void CMultAlignment::order_nodes()
{
	int i;
	treeptr p;

	for (i = 0; i < numseq; ++i) {
		p = lptr[i];
		while (p != nullptr) {
			p->order++;
			p = p->parent;
		}
	}
}

treeptr CMultAlignment::insert_root(treeptr p, float diff)
{
	treeptr newp, prev, q, t;
	float dist, prevdist, td;

	newp = avail().get();

	t = p->parent;
	prevdist = t->dist;

	p->parent = newp;

	dist = p->dist;

	p->dist = diff / 2;
	if (p->dist < 0.0)
		p->dist = 0.0;
	if (p->dist > dist)
		p->dist = dist;

	t->dist = dist - p->dist;

	newp->left = t->shared_from_this();
	newp->right = p->shared_from_this();
	newp->parent = nullptr;
	newp->dist = 0.0;
	newp->leaf = NODE;

	if (t->left.get() == p)
		t->left = t->parent->shared_from_this();
	else
		t->right = t->parent->shared_from_this();

	prev = t;
	q = t->parent;

	t->parent = newp;

	while (q != nullptr) {
		if (q->left.get() == prev) {
			q->left = q->parent->shared_from_this();
			q->parent = prev;
			td = q->dist;
			q->dist = prevdist;
			prevdist = td;
			prev = q;
			q = q->left.get();
		}
		else {
			q->right = q->parent->shared_from_this();
			q->parent = prev;
			td = q->dist;
			q->dist = prevdist;
			prevdist = td;
			prev = q;
			q = q->right.get();
		}
	}

/*
	remove the old root node
*/
	q = prev;
	if (!q->left) {
		dist = q->dist;
		q = q->right.get();
		q->dist += dist;
		q->parent = prev->parent;
		if (prev->parent->left.get() == prev)
			prev->parent->left = q->shared_from_this();
		else
			prev->parent->right = q->shared_from_this();
		prev->right = nullptr;
	}
	else {
		dist = q->dist;
		q = q->left.get();
		q->dist += dist;
		q->parent = prev->parent;
		if (prev->parent->left.get() == prev)
			prev->parent->left = q->shared_from_this();
		else
			prev->parent->right = q->shared_from_this();
		prev->left = nullptr;
	}

	return newp;
}

treeptr CMultAlignment::reroot(treeptr ptree, int nseqs)
{

	treeptr p, rootnode, rootptr;
	float diff, mindiff = 0.0, mindepth = 1.0, maxdist;
	int i;
	bool first = true;

/*
  find the difference between the means of leaf->node
  distances on the left and on the right of each node
*/
	rootptr = ptree;
	for (i = 0; i < ntotal; ++i) {
		p = ptrs[i];
		if (p->parent == nullptr)
			diff = calc_root_mean(p, &maxdist);
		else
			diff = calc_mean(p, &maxdist, nseqs);

		if ((diff == 0) || ((diff > 0) && (diff < 2 * p->dist))) {
			if (maxdist < mindepth || first) {
				first = false;
				rootptr = p;
				mindepth = maxdist;
				mindiff = diff;
			}
		}

	}

/*
	insert a new node as the ancestor of the node which produces the shallowest
	tree.
*/
	if (rootptr == ptree) {
		mindiff = rootptr->left->dist + rootptr->right->dist;
		rootptr = rootptr->right.get();
	}
	rootnode = insert_root(rootptr, mindiff);

	diff = calc_root_mean(rootnode, &maxdist);

	return rootnode;
}

int CMultAlignment::read_tree(wstring& treefile, int first_seq, int last_seq)
{

	char c;
	string name1, name2;
	int i, j, k;
	bool found;

	numseq = 0;
	nnodes = 0;
	ntotal = 0;
	rooted_tree = true;

	fd.open(treefile.c_str(), ios::in);
	if (fd.bad()) {
		cout << "cannot open " << treefile.c_str();
		return 0;
	}

	skip_space(fd);
	char ch = fd.get();
	if (ch != '(') {
		cerr << "Wrong format in tree file " << treefile.c_str();
		return 0;
	}
	fd.clear();
	fd.seekg(0);

	distance_tree = true;

/*
	Allocate memory for tree
*/
	nptr = make_unique<treeptr[]>(3 * (last_seq-first_seq + 1));
	ptrs = make_unique<treeptr[]>(3 * (last_seq-first_seq + 1));
	lptr = make_unique<treeptr[]>((last_seq-first_seq + 1));
	olptr = make_unique<treeptr[]>((last_seq + 1));

	treeptr seq_tree = avail().get();
	set_info(seq_tree, nullptr, 0, "", 0.0);

	create_tree(seq_tree, nullptr);
	fd.close();


	if (numseq != last_seq-first_seq) {
		cerr << "tree not compatible with alignment\n " << last_seq-first_seq << " sequences in alignment and " << numseq << " in tree";
		return 0;
	}

/*
	If the tree is unrooted, reroot the tree - ie. minimise the difference
	between the mean root->leaf distances for the left and right branches of
	the tree.
*/

	if (distance_tree == false) {
		if (rooted_tree == false) {
			cerr << "input tree is unrooted and has no distances.\nCannot align sequences";
			return 0;
		}
	}

	if (rooted_tree == false) {
		root = reroot(seq_tree, last_seq - first_seq + 1);
	}
	else
	{
		root = seq_tree;
	}

/*
	calculate the 'order' of each node.
*/
	order_nodes();

	if (numseq > 3) {
/*
	If there are more than three sequences....
*/
/*
	assign the sequence nodes (in the same order as in the alignment file)
*/
		for (i = first_seq; i < last_seq; ++i) {
			if (names[i + 1].length() > MAXNAMES)
				cout << "name " << names[i + 1] << " is too long for PHYLIP tree format (max " << MAXNAMES << " chars)";

			for (k = 0; k < names[i + 1].length() && k < MAXNAMES; k++) {
				c = names[i + 1][k];
				if ((c > 0x40) && (c < 0x5b))
					c |= 0x20;
				if (c == ' ')
					c = '_';
				name2[k] = c;
			}
			name2[k] = '\0';
			found = false;
			for (j=0; j<numseq; ++j) {
				for (k = 0; k < lptr[j]->name.length() && k < MAXNAMES; k++) {
					c = lptr[j]->name[k];
					if ((c > 0x40) && (c < 0x5b))
						c |= 0x20;
					name1[k] = c;
				}
				name1[k] = '\0';
				if (name1 == name2) {
					olptr[i] = lptr[j];
					found = true;
				}
			}
			if (found == false) {
				cerr << "tree not compatible with alignment:\n" << name2 << " not found";
				return 0;
			}
		}

	}
	return 1;
}

void CMultAlignment::clear_tree(treeptr p)
{
	//handle itself
}

int CMultAlignment::calc_similarities(int nseqs)
{
	int depth = 0, i, j, k, n;
	int found;
	int nerrs;
	unique_ptr<int[]> seq1, seq2;
	treeptr p;
	unique_ptr<treeptr[]> path2root;
	float dist;
	unique_ptr<float[]> dist2node, bad_dist;
	vector<vector<double> > dmat;
	string err_mess, err1, reply;

	path2root = make_unique<treeptr[]>(nseqs);
	dist2node = make_unique<float[]>(nseqs);
	dmat.clear();
	dmat.resize(nseqs);
	for (i = 0; i < nseqs; ++i) {
		vector<double> row;
		row.resize(nseqs);
		dmat[i] = row;
	}
	seq1 = make_unique<int[]>(nseqs);
	seq2 = make_unique<int[]>(nseqs);
	bad_dist = make_unique<float[]>(nseqs);

	if (nseqs > 3) {
/*
	for each leaf, determine all nodes between the leaf and the root;
*/
		for (i = 0; i < nseqs; ++i) {
			depth = dist = 0;
			p = olptr[i];
			while (p != nullptr) {
				path2root[depth] = p;
				dist += p->dist;
				dist2node[depth] = dist;
				p = p->parent;
				depth++;
			}

/*
	for each pair....
*/
			for (j = 0; j < i; ++j) {
				p = olptr[j];
				dist = 0.0;
/*
	find the common ancestor.
*/
				found = false;
				n = 0;
				while (!found && p->parent != nullptr) {
					for (k=0; k< depth; k++)
						if (p->parent == path2root[k]) {
							found = true;
							n = k;
						}
					dist += p->dist;
					p = p->parent;
				}

				dmat[i][j] = dist + dist2node[n - 1];
			}
		}

		nerrs = 0;
		for (i = 0; i < nseqs; ++i) {
			dmat[i][i] = 0.0;
			for (j = 0; j < i; ++j) {
				if (dmat[i][j] < 0.01)
					dmat[i][j] = 0.01;
				if (dmat[i][j] > 1.0) {
					if (dmat[i][j] > 1.1) {
						seq1[nerrs] = i;
						seq2[nerrs] = j;
						bad_dist[nerrs] = dmat[i][j];
						nerrs++;
					}
					dmat[i][j] = 1.0;
				}
			}
		}
		if (nerrs > 0) {
			err_mess = "The following sequences are too divergent to be aligned:\n";
			for (i = 0; i < nerrs && i < 5; ++i) {
				ostringstream errStream;
				errStream << "           " << names[seq1[i]+1];
				errStream << " and " << names[seq2[i]+1] << " (distance ";
				errStream << bad_dist[i] << ")\n",
				err_mess += errStream.str();
			}
			err_mess += "(All distances should be between 0.0 and 1.0)\n";
			err_mess += "This may not be fatal but you have been warned!\n";
			err_mess += "SUGGESTION: Remove one or more problem sequences and try again";
			}
		}
	else {
		for (i = 0; i < nseqs; ++i) {
			for (j = 0; j < i; ++j) {
				dmat[i][j] = tmat[i + 1][j + 1];
			}
		}
	}

	path2root.reset();
	dist2node.reset();
	for (i = 0; i < nseqs; ++i) {
		tmat[i + 1][i + 1] = 0.0;
		for (j = 0; j < i; ++j) {
			tmat[i + 1][j + 1] = 100.0 - (dmat[i][j]) * 100.0;
			tmat[j + 1][i + 1] = tmat[i + 1][j + 1];
		}
	}

	dmat.clear();
	return 1;
}

int CMultAlignment::SeqGCGCheckSum(string& seq, int len)
{
	transform(seq.begin(), seq.end(), seq.begin(), ::toupper);
	long check = 0L;

	for(int i = 0; i < len; ++i)
		check += ((i % 57) + 1) * seq[i];

	return check % 10000;
}

int CMultAlignment::malign(int istart, wstring& phylip_name) /* full progressive alignment*/
{
	int ix;

	int max, sum;
	int i, j, set, iseq = 0;
	int status,entries;
	int score = 0;
	int range;

	/* get the phylogenetic tree from *.ph */

	if (nseqs > 3) {
		status = read_tree(phylip_name, 0, nseqs);
		if (status == 0)
			return 0;
	}

	/* calculate sequence weights according to branch lengths of the tree -
	weights in global variable seq_weight normalised to sum to 100 */

	seq_weight = make_unique<int[]>(nseqs);
	calc_seq_weights(0, nseqs, seq_weight.get());

	/* recalculate tmat matrix as percent similarity matrix */

	status = calc_similarities(nseqs);
	if (status == 0)
		return 0;

	/* for each sequence, find the most closely related sequence */

	auto maxid = make_unique<int[]>(nseqs + 1);
	for (i = 1; i <= nseqs; ++i) {
		maxid[i] = 0;
		for (j = 1; j <= nseqs; ++j)
			if (maxid[i] < tmat[i][j])
				maxid[i] = tmat[i][j];
	}

	/* group the sequences according to their relative divergence */
	unique_ptr<int[]> aligned;
	if (istart == 0) {
		sets.clear();
		sets.resize(nseqs + 1);
		for (i = 0; i <= nseqs; ++i) {
			vector<int> row(nseqs + 1);
			sets[i] = row;
		}

		create_sets(0, nseqs);

	/* clear the memory used for the phylogenetic tree */
		if (nseqs > 3)
			clear_tree(nullptr);

	/* start the multiple alignments.........  */

	/* first pass, align closely related sequences first.... */

		ix = 0;
		aligned = make_unique<int[]>(nseqs + 1);
		range = nsets * nseqs;
		for(set = 1; set <= nsets; ++set) {
			entries=0;
			for (i = 1; i <= nseqs; ++i) {
				ostringstream textStream;
				textStream << "Multiple alignment: Aligning group " << set << " of " << nsets;
				string text = textStream.str();
				wstring caption(text.begin(), text.end());
				copy_chars(caption);
				*pPercent = 100 * (set* i) / range;
				if ((sets[set][i] != 0) && (maxid[i] > divergence_cutoff)) {
					entries++;
					if (aligned[i] == 0) {
						if (output_order == INPUT) {
							++ix;
							output_index[i] = i;
						}
						else
							output_index[++ix] = i;
						aligned[i] = 1;
					}
				}
			}
			if(entries > 0)
				score = prfalign(&(sets[set][0]), aligned.get());
			else
				score = 0;

			/* negative score means fatal error... exit now!  */

			if (score < 0)
				return -1;
		}

		sets.clear();
	}
	else {
	/* clear the memory used for the phylogenetic tree */

		if (nseqs > 3)
			clear_tree(nullptr);

		aligned = make_unique<int[]>(nseqs + 1);
		ix = 0;
		for (i = 1; i <= istart + 1; ++i) {
			aligned[i] = 1;
			++ix;
			output_index[i] = i;
		}
		for (i = istart + 2; i <= nseqs; ++i)
			aligned[i] = 0;
	}

	/* second pass - align remaining, more divergent sequences..... */

	/* if not all sequences were aligned, for each unaligned sequence,
		find it's closest pair amongst the aligned sequences.  */

	auto group = make_unique<int[]>(nseqs + 1);
	auto tree_weight = make_unique<int[]>(nseqs);
	for (i = 0; i < nseqs; ++i)
		tree_weight[i] = seq_weight[i];

	while (ix < nseqs) {
		if (ix > 0) {
			for (i = 1; i <= nseqs; ++i) {
				if (aligned[i] == 0) {
					maxid[i] = 1;
					for (j = 1; j <= nseqs; ++j) {
						if ((maxid[i] < tmat[i][j]) && (aligned[j] != 0))
							maxid[i] = tmat[i][j];
					}
				}
			}
		}

		/* find the most closely related sequence to those already aligned */

		max = 0;
		iseq = 0;
		for (i = 1; i <= nseqs; ++i) {
			if ((aligned[i] == 0) && (maxid[i] > max)) {
				max = maxid[i];
				iseq = i;
			}
		}

	/* align this sequence to the existing alignment */
	/* weight sequences with percent identity with profile*/
	/* OR...., multiply sequence weights from tree by percent identity with new sequence */
		for (j = 0; j < nseqs; ++j) {
			if (aligned[j + 1] != 0)
				seq_weight[j] = tree_weight[j] * tmat[j + 1][iseq];
		}
	/*
	  Normalise the weights, such that the sum of the weights = INT_SCALE_FACTOR
	*/

		sum = 0;
		for (j = 0; j < nseqs; ++j) {
			if (aligned[j + 1] != 0)
				sum += seq_weight[j];
		}
		if (sum == 0) {
			for (j=0; j<nseqs; ++j)
				seq_weight[j] = 1;
			sum = j;
		}
		for (j = 0; j < nseqs; ++j) {
			if (aligned[j + 1] != 0) {
				seq_weight[j] = (seq_weight[j] * INT_SCALE_FACTOR) / sum;
				if (seq_weight[j] < 1)
					seq_weight[j] = 1;
			}
		}

		entries = 0;
		for (j = 1; j <= nseqs; ++j) {
			if (aligned[j] != 0) {
				group[j] = 1;
				entries++;
			}
			else if (iseq == j) {
				group[j] = 2;
				entries++;
			}
		}
		aligned[iseq] = 1;

		score = prfalign(group.get(), aligned.get());
		if (output_order == INPUT) {
			++ix;
			output_index[iseq] = iseq;
		}
		else
			output_index[++ix] = iseq;
	}

	aln_score();

	/* make the rest (output stuff) into routine clustal_out in file amenu.c */

	return nseqs;
}

int CMultAlignment::seqalign(int istart, wstring& phylip_name)
{
	int ix;

	int max;
	int i, j, status, iseq;
	int sum, entries;
	int score = 0;
	unique_ptr<int[]> aligned;

	cout << "Start of Multiple Alignment";

	auto seq_weight = make_unique<int[]>(nseqs);

/* get the phylogenetic tree from *.ph */

	if (nseqs > 3) {
		status = read_tree(phylip_name, 0, nseqs);
		if (status == 0)
			return 0;
	}

/* calculate sequence weights according to branch lengths of the tree -
	weights in global variable seq_weight normalised to sum to 100 */

	calc_seq_weights(0, nseqs, seq_weight.get());

	auto tree_weight = make_unique<int[]>(nseqs);
	for (i = 0; i < nseqs; ++i)
		tree_weight[i] = seq_weight[i];

/* recalculate tmat matrix as percent similarity matrix */

	status = calc_similarities(nseqs);
	if (status == 0)
		return 0;

/* for each sequence, find the most closely related sequence */

	auto maxid = make_unique<int[]>(nseqs + 1);
	for (i = 1; i <= nseqs; ++i) {
		maxid[i] = 0;
		for (j = 1; j <= nseqs; ++j)
			if (maxid[i] < tmat[i][j])
				maxid[i] = tmat[i][j];
	}

/* clear the memory used for the phylogenetic tree */

	if (nseqs > 3)
		 clear_tree(nullptr);

	aligned = make_unique<int[]>(nseqs + 1);
	ix = 0;
	for (i = 1; i <= istart + 1; ++i) {
		aligned[i] = 1;
		++ix;
		output_index[i] = i;
	}
	for (i = istart + 2; i <= nseqs; ++i)
		aligned[i] = 0;

/* for each unaligned sequence, find it's closest pair amongst the
	aligned sequences.  */

	auto group = make_unique<int[]>(nseqs + 1);

	while (ix < nseqs) {
		if (ix > 0) {
			for (i = 1; i <= nseqs; ++i) {
				if (aligned[i] == 0)
				{
					maxid[i] = 0;
					for (j = 1; j <= nseqs; ++j)
						if ((maxid[i] < tmat[i][j]) && (aligned[j] != 0))
							maxid[i] = tmat[i][j];
				}
			}
		}

/* find the most closely related sequence to those already aligned */

		max = 0;
		for (i = 1; i <= nseqs; ++i) {
			if (aligned[i] == 0 && maxid[i] > max) {
				max = maxid[i];
				iseq = i;
			}
		}

/* align this sequence to the existing alignment */

		entries = 0;
		for (j = 1; j <= nseqs; ++j)
			if (aligned[j] != 0) {
				group[j] = 1;
				entries++;
			}
			else if (iseq == j) {
				group[j] = 2;
				entries++;
			}
		aligned[iseq] = 1;


/* EITHER....., set sequence weights equal to percent identity with new sequence */
/*
			for (j = 0; j < nseqs; ++j)
			  seq_weight[j] = tmat[j + 1][iseq];
*/
/* OR...., multiply sequence weights from tree by percent identity with new sequence */
			for (j = 0; j < nseqs; ++j)
				seq_weight[j] = tree_weight[j] * tmat[j + 1][iseq];

/*
  Normalise the weights, such that the sum of the weights = INT_SCALE_FACTOR
*/

		sum = 0;
		for (j = 0; j < nseqs; ++j)
			if (group[j + 1] == 1) sum += seq_weight[j];
		if (sum == 0)
		{
			for (j = 0; j < nseqs; ++j)
				seq_weight[j] = 1;
				sum = j;
		}
		for (j = 0; j < nseqs; ++j)
		{
			seq_weight[j] = (seq_weight[j] * INT_SCALE_FACTOR) / sum;
			if (seq_weight[j] < 1) seq_weight[j] = 1;
		}


		score = prfalign(group.get(), aligned.get());
		//info("Sequence:%d	  Score:%d",(pint)iseq,(pint)score);
		if (output_order == INPUT)
		{
			++ix;
			output_index[iseq] = iseq;
		}
		else
			output_index[++ix] = iseq;
	}

	aln_score();
/* make the rest (output stuff) into routine clustal_out in file amenu.c */

	return nseqs;

}

int CMultAlignment::palign1()
{
	int i, j;
	int entries;
	float dscore;
	int score;

	cout << "Start of Initial Alignment";

/* calculate sequence weights according to branch lengths of the tree -
	weights in global variable seq_weight normalised to sum to INT_SCALE_FACTOR */

	auto seq_weight = make_unique<int[]>(nseqs);

	int temp = INT_SCALE_FACTOR / nseqs;
	for (i = 0; i < nseqs; ++i)
		seq_weight[i] = temp;

	distance_tree = false;

/* do the initial alignment.........  */

	auto group = make_unique<int[]>(nseqs + 1);

	for(i = 1; i <= profile1_nseqs; ++i)
		group[i] = 1;
	for(i = profile1_nseqs + 1; i <= nseqs; ++i)
		group[i] = 2;
	entries = nseqs;

	auto aligned = make_unique<int[]>(nseqs + 1);
	for (i = 1; i <= nseqs; ++i)
		aligned[i] = 1;

	score = prfalign(group.get(), aligned.get());
	cout << "Sequences:" << entries << "	  Score:" << score;

	for (i = 1; i <= nseqs; ++i) {
		for (j = i + 1; j <= nseqs; ++j) {
			dscore = countid(i,j);
			tmat[i][j] = (100.0 - (double) dscore) / 100.0;
			tmat[j][i] = tmat[i][j];
		}
	}

	return nseqs;
}

int CMultAlignment::palign2(wstring& p1_tree_name, wstring& p2_tree_name)
{
	int i, j, sum, entries, status;
	int score;

	//info("Start of Multiple Alignment");

	/* get the phylogenetic trees from *.ph */

	if (profile1_nseqs > 3) {
		status = read_tree(p1_tree_name, 0, profile1_nseqs);
		if (status == 0)
			return 0;
	}

	/* calculate sequence weights according to branch lengths of the tree -
	weights in global variable seq_weight normalised to sum to 100 */

	auto p1_weight = make_unique<int[]>(profile1_nseqs);

	calc_seq_weights(0, profile1_nseqs, p1_weight.get());

	/* clear the memory for the phylogenetic tree */

	if (profile1_nseqs > 3)
		clear_tree(nullptr);

	if (nseqs - profile1_nseqs > 3) {
		status = read_tree(p2_tree_name, profile1_nseqs, nseqs);
		if (status == 0)
			return 0;
	}

	auto p2_weight = make_unique<int[]>(nseqs);

	calc_seq_weights(profile1_nseqs,nseqs, p2_weight.get());


	/* clear the memory for the phylogenetic tree */

	if (nseqs - profile1_nseqs > 3)
		clear_tree(nullptr);

	/* convert tmat distances to similarities */

	for (i = 1; i < nseqs; ++i) {
		for (j = i + 1; j <= nseqs; ++j) {
			tmat[i][j] = 100.0 - tmat[i][j] * 100.0;
			tmat[j][i] = tmat[i][j];
		}
	}


	/* weight sequences with max percent identity with other profile*/
	auto seq_weight = make_unique<int[]>(nseqs);

	auto maxid = make_unique<int[]>(nseqs + 1);
	for (i = 0; i < profile1_nseqs; ++i) {
		maxid[i] = 0;
		for (j = profile1_nseqs + 1; j <= nseqs; ++j) {
			if (maxid[i] < tmat[i + 1][j])
				maxid[i] = tmat[i + 1][j];
		}
		seq_weight[i] = maxid[i] * p1_weight[i];
	}

	for (i = profile1_nseqs; i < nseqs; ++i) {
		maxid[i] = 0;
		for (j = 1; j <= profile1_nseqs; ++j) {
			if (maxid[i] < tmat[i + 1][j])
				maxid[i] = tmat[i + 1][j];
		}
		seq_weight[i] = maxid[i] * p2_weight[i];
	}
/*
  Normalise the weights, such that the sum of the weights = INT_SCALE_FACTOR
*/

	sum = 0;
	for (j = 0; j < nseqs; ++j)
		sum += seq_weight[j];
	if (sum == 0) {
		for (j = 0; j < nseqs; ++j)
			seq_weight[j] = 1;
		sum = j;
	}
	for (j = 0; j < nseqs; ++j) {
		seq_weight[j] = (seq_weight[j] * INT_SCALE_FACTOR) / sum;
		if (seq_weight[j] < 1)
			seq_weight[j] = 1;
	}

	/* do the alignment.........  */

	auto group = make_unique<int[]>(nseqs + 1);

	for(i = 1;  i <= profile1_nseqs; ++i)
		group[i] = 1;
	for(i = profile1_nseqs + 1; i <= nseqs; ++i)
		group[i] = 2;
	entries = nseqs;

	auto aligned = make_unique<int[]>(nseqs + 1);
	for (i = 1; i <= nseqs; ++i)
		aligned[i] = 1;

	score = prfalign(group.get(), aligned.get());

	/* DES	output_index = (int *)ckalloc( (nseqs + 1) * sizeof (int)); */
	for (i = 1; i <= nseqs; ++i)
		output_index[i] = i;

	return nseqs;
}

int CMultAlignment::forward_pass(const string& ia, const string& ib, int n, int m)
{
	int i, j;
	int f, hh, p, t;
	int fraction;

	maxscore = 0;
	se1 = se2 = 0;
	for (i = 0;i <= m; ++i) {
		HH[i] = 0;
		DD[i] = -g;
	}
	int full_range = n * m;
	for (i = 1; i <= n; ++i) {
		hh = p = 0;
		f = -g;

		for (j = 1;j <= m; ++j) {
			fraction = i * m + j;
			*pPercent = 100 * (float) fraction / full_range;
			f -= gh;
			t = hh - g - gh;
			if (f < t)
				f = t;

			DD[j] -= gh;
			t = HH[j] - g - gh;
			if (DD[j] < t)
				DD[j] = t;

			hh = p + matrix[(int)ia[i]][(int)ib[j]];
			if (hh < f)
				hh = f;
			if (hh < DD[j])
				hh = DD[j];
			if (hh < 0)
				hh = 0;

			p = HH[j];
			HH[j] = hh;

			if (hh > maxscore) {
				maxscore = hh;
				se1 = i;
				se2 = j;
			}
		}
	}
	return 1;
}


int CMultAlignment::reverse_pass(const string& ia, const string& ib)
{
	int i, j;
	int f, hh, p, t;
	int fraction;
	int range;

	int cost = 0;
	sb1 = sb2 = 0;
	range = se2 * se1;	
	for (i = se2; i > 0; --i) {
		HH[i] = -1;
		DD[i] = -1;
	}

	for (i = se1; i > 0; --i) {
		hh = f = -1;
		p = (i == se1) ? 0 : -1;

		for (j = se2; j > 0; --j) {
			fraction = i * se2 + j;
			*pPercent = 100 * (1 - (float) fraction / range);
			f -= gh;
			t = hh - g - gh;
			if (f < t)
				f = t;

			DD[j] -= gh;
			t = HH[j] - g - gh;
			if (DD[j] < t)
				DD[j] = t;

			hh = p + matrix[(int)ia[i]][(int)ib[j]];
			if (hh < f)
				hh = f;
			if (hh < DD[j])
				hh = DD[j];

			p = HH[j];
			HH[j] = hh;

			if (hh > cost) {
				cost = hh;
				sb1 = i;
				sb2 = j;
				if (cost >= maxscore)
					break;
			}
		}
		if (cost >= maxscore)
			break;
	}

	return 1;
}

void CMultAlignment::del(int k)
{
	if(last_print < 0)
		last_print = displ[print_ptr - 1] -= k;
	else
		last_print = displ[print_ptr++] = -k;
}

void CMultAlignment::add(int v)
{
	if(last_print < 0) {
		displ[print_ptr - 1] = v;
		displ[print_ptr++] = last_print;
	}
	else
		last_print = displ[print_ptr++] = v;
}

int CMultAlignment::calc_score(int iat, int jat, int v1, int v2)
{
	int ipos = v1 + iat;
	int jpos = v2 + jat;

	return matrix[(int)seq_array[seq1][ipos]][(int)seq_array[seq2][jpos]];
}


float CMultAlignment::tracepath(int tsb1, int tsb2)
{
	char c1, c2;
	int i1, i2;
	int i, k, pos, to_do;
	int count;
	float score;
/*	char *s1, *s2;
*/
		to_do = print_ptr - 1;
		i1 = tsb1;
		i2 = tsb2;

	pos = 0;
	count = 0;
	for(i = 1; i <= to_do; ++i) {
		if(displ[i] == 0) {
			c1 = seq_array[seq1][i1];
			c2 = seq_array[seq2][i2];
/*
if (debug>1)
{
if (c1>max_aa) s1[pos] = '-';
else s1[pos]=amino_acid_codes[c1];
if (c2>max_aa) s2[pos] = '-';
else s2[pos]=amino_acid_codes[c2];
}
*/
			if ((c1 != gap_pos1) && (c1 != gap_pos2) && (c1 == c2))
				count++;
			++i1;
			++i2;
			++pos;
		}
		else {
			if((k = displ[i]) > 0) {
/*
if (debug>1)
for (r=0;r<k;r++)
{
s1[pos+r]='-';
if (seq_array[seq2][i2+r]>max_aa) s2[pos+r] = '-';
else s2[pos+r]=amino_acid_codes[seq_array[seq2][i2+r]];
}
*/
				i2 += k;
				pos += k;
				count--;
			}
			else {
/*
if (debug>1)
for (r=0;r<(-k);r++)
{
s2[pos+r]='-';
if (seq_array[seq1][i1+r]>max_aa) s1[pos+r] = '-';
else s1[pos+r]=amino_acid_codes[seq_array[seq1][i1+r]];
}
*/
				i1 -= k;
				pos -= k;
				count--;
			}
		}
	}
/*
if (debug>1) fprintf(stdout,"\n");
if (debug>1)
{
for (i=0;i<pos;i++) fprintf(stdout,"%c",s1[i]);
fprintf(stdout,"\n");
for (i=0;i<pos;i++) fprintf(stdout,"%c",s2[i]);
fprintf(stdout,"\n");
}
*/
	if (count <= 0)
		count = 1;
	score = 100.0 * (float)count;
	return score;
}

int CMultAlignment::diff(int A, int B, int M, int N, int tb, int te)
{
	int type;
	int midi, midj, i, j;
	int midh;
	int hh, e;

	if(N <= 0) {
		if(M > 0)
			del(M);

		return -(int) tbgap(M);
	}

	if(M <= 1) {
		if(M <= 0) {
			add(N);
			return -(int) tbgap(N);
		}

		midh = -(tb + gh) - tegap(N);
		hh = -(te + gh) - tbgap(N);
		if (hh > midh)
			midh = hh;
		midj = 0;
		for(j = 1; j <= N; ++j) {
			hh = calc_score(1, j, A, B) - tegap(N - j) - tbgap(j - 1);
			if(hh > midh) {
				midh = hh;
				midj = j;
			}
		}

		if(midj == 0) {
			del(1);
			add(N);
		}
		else {
			if(midj > 1)
				add(midj - 1);
			displ[print_ptr++] = last_print = 0;
			if(midj < N)
				add(N - midj);
		}
		return midh;
	}

	/* Divide: Find optimum midpoint (midi,midj) of cost midh */

	midi = M / 2;
	HH[0] = 0.0;
	int t = -tb;
	for(j = 1; j <= N; ++j) {
		HH[j] = t = t - gh;
		DD[j] = t - g;
	}

	t = -tb;
	for(i = 1; i <= midi; ++i) {
		int s = HH[0];
		HH[0] = hh = t = t - gh;
		int f = t - g;

		for(j = 1; j <= N; ++j) {
			if ((hh = hh - g - gh) > (f = f - gh))
				f = hh;
			if ((hh = HH[j] - g - gh) > (e = DD[j] - gh))
				e = hh;
			hh = s + calc_score(i, j, A, B);
			if (f > hh)
				hh = f;
			if (e > hh)
				hh = e;

			s = HH[j];
			HH[j] = hh;
			DD[j] = e;
		}
	}

	DD[0] = HH[0];

	RR[N] = 0;
	t = -te;
	for(j = N - 1; j >= 0; --j) {
		RR[j] = t = t - gh;
		SS[j] = t-g;
	}

	t = -te;
	for(i = M - 1; i >= midi; --i) {
		int s = RR[N];
		RR[N] = hh = t = t - gh;
		int f = t - g;

		for(j = N - 1; j >= 0; --j) {
			if ((hh = hh - g - gh) > (f = f - gh))
				f = hh;
			if ((hh = RR[j] - g - gh) > (e = SS[j] - gh))
				e = hh;
			hh = s + calc_score(i + 1, j + 1, A, B);
			if (f > hh)
				hh = f;
			if (e > hh)
				hh = e;

			s = RR[j];
			RR[j] = hh;
			SS[j] = e;
		}
	}

	SS[N] = RR[N];

	midh = HH[0] + RR[0];
	midj = 0;
	type = 1;
	for (j = 0; j <= N; ++j) {
		hh = HH[j] + RR[j];
		if (hh >= midh) {
			if (hh > midh || (HH[j] != DD[j] && RR[j] == SS[j])) {
				midh = hh;
				midj = j;
			}
		}
	}

	for (j = N; j >= 0; --j) {
		hh = DD[j] + SS[j] + g;
		if (hh > midh) {
			midh = hh;
			midj = j;
			type = 2;
		}
	}


	/* Conquer recursively around midpoint  */

	if(type == 1) {				 /* Type 1 gaps  */
		diff(A, B, midi, midj, tb, g);
		diff(A + midi, B + midj, M - midi, N - midj, g, te);
	}
	else {
		diff(A, B, midi - 1, midj, tb, 0);
		del(2);
		diff(A + midi + 1, B + midj, M - midi - 1, N  -midj, 0, te);
	}

	return midh;		 /* Return the score of the best alignment */
}

int CMultAlignment::pairalign(int istart, int iend, int jstart, int jend)
{
	char c;
	int si, sj;
	int n, m, len1, len2;
	int maxres;
	float gscale, ghscale;

	const short* matptr;
	short* mat_xref;
	displ = make_unique<int[]>(2 * max_aln_length + 1);
	HH = make_unique<int[]>(max_aln_length);
	DD = make_unique<int[]>(max_aln_length);
	RR = make_unique<int[]>(max_aln_length);
	SS = make_unique<int[]>(max_aln_length);

#ifdef MAC
	int_scale = 10;
#else
	int_scale = 100;
#endif
	gscale = ghscale = 1.0f;
	if (dnaflag) {
		if (pw_dnamtrxname == "iub") {
			matptr = swgapdnamt;
			mat_xref = def_dna_xref.get();
		}
		else if (pw_dnamtrxname == "clustalw") {
			matptr = clustalvdnamt;
			mat_xref = def_dna_xref.get();
			gscale = 0.6667f;
			ghscale = 0.751f;
		}
		else {
			matptr = pw_userdnamat.get();
			mat_xref = pw_dna_xref.get();
		}
		maxres = get_matrix(matptr, mat_xref, matrix, true, int_scale);
		if (maxres == 0)
			return -1;

		matrix[0][4] = transition_weight * matrix[0][0];
		matrix[4][0] = transition_weight * matrix[0][0];
		matrix[2][11] = transition_weight * matrix[0][0];
		matrix[11][2] = transition_weight * matrix[0][0];
		matrix[2][12] = transition_weight * matrix[0][0];
		matrix[12][2] = transition_weight * matrix[0][0];
	}
	else {
		if (pw_mtrxname == "blosum") {
			matptr = blosum30mt;
			mat_xref = def_aa_xref.get();
		}
		else if (pw_mtrxname == "pam") {
			matptr = pam350mt;
			mat_xref = def_aa_xref.get();
		}
		else if (pw_mtrxname == "gonnet") {
			matptr = gon250mt;
			int_scale /= 10;
			mat_xref = def_aa_xref.get();
		}
		else if (pw_mtrxname == "id") {
			matptr = idmat;
			mat_xref = def_aa_xref.get();
		}
		else {
			matptr = pw_usermat[0];
			mat_xref = pw_aa_xref.get();
		}

		maxres = get_matrix(matptr, mat_xref, matrix, true, int_scale);
		if (maxres == 0)
			return -1;
	}

	for (si = max(0, istart); si < nseqs && si < iend; ++si) {
		*pPercent = si / (min(nseqs, iend) - max(0, istart)) * 100.0;
		n = seqlen_array[si + 1];
		len1 = 0;
		for (int i = 1; i <= n; ++i) {
			c = seq_array[si + 1][i];
			if (c != gap_pos1 && c != gap_pos2)
				len1++;
		}

		for (sj = max(si + 1, jstart + 1); sj < nseqs && sj < jend; ++sj) {
			m = seqlen_array[sj + 1];
			len2 = 0;

			ostringstream textStream;
			textStream << "Pairwise alignment: sequence " << si + 1 << " to sequence " << sj + 1;
			string text = textStream.str();
			wstring caption(text.begin(), text.end());
			copy_chars(caption);

			for (int i = 1; i <= m; ++i) {
				c = seq_array[sj + 1][i];
				if (c != gap_pos1 && c != gap_pos2)
					len2++;
			}

			if (dnaflag) {
				g = 200 * (float) pw_go_penalty * gscale;
				gh = pw_ge_penalty * int_scale * ghscale;
			}
			else {
				if (mat_avscore <= 0)
					g = 200 * (float)(pw_go_penalty + log((double)(min(n,m))));
				else
					g = 2 * mat_avscore * (float)(pw_go_penalty + log((double)(min(n,m))));
				gh = pw_ge_penalty * int_scale;
			}

	/*
		align the sequences
	*/
			seq1 = si + 1;
			seq2 = sj + 1;
			caption = L"Forward pass";
			copy_chars(caption);
			if(!forward_pass(seq_array[seq1], seq_array[seq2], n, m)) {
				(*pCaption)[0] = '\0';
				return 0;
			}

			caption = L"Reverse pass";
			copy_chars(caption);
			if(!reverse_pass(seq_array[seq1], seq_array[seq2])) {
				(*pCaption)[0] = '\0';
				return 0;
			}

			last_print = 0;
			print_ptr = 1;
			/*
				sb1 = sb2 = 1;
				se1 = n-1;
				se2 = m-1;
			*/

			/* use Myers and Miller to align two sequences */
			caption = L"Myers and Miller alignment";
			copy_chars(caption);
			maxscore = diff(sb1 - 1, sb2 - 1, se1 - sb1 + 1, se2 - sb2 + 1, 0, 0);

			/* calculate percentage residue identity */

			mm_score = tracepath(sb1, sb2);

			if(len1 == 0 || len2 == 0)
				mm_score = 0;
			else
				mm_score /= (float) min(len1, len2);

			tmat[si + 1][sj + 1] = (100.0f - mm_score) / 100.0f;
			tmat[sj + 1][si + 1] = (100.0f - mm_score) / 100.0f;

		}
	}

	return 1;
}

void CMultAlignment::pdel(int k)
{
	if(last_print < 0)
		last_print = displ[print_ptr - 1] -= k;
	else
		last_print = displ[print_ptr++] = -(k);
}

void CMultAlignment::padd(int k)
{
	if(last_print < 0) {
		displ[print_ptr - 1] = k;
		displ[print_ptr++] = last_print;
	}
	else
		last_print = displ[print_ptr++] = k;
}

/* calculate the score for a gap of length k, at residues A[i] and B[j]  */
int CMultAlignment::gap_penalty1(int i, int j, int k)
{
	if (k <= 0)
		return 0;
	if (i == 0 || i == prf_length1)
		return 0;

	int h = 0;
	int g = profile2[j][GAPCOL] + profile1[i][GAPCOL];
	for (int ix = 0; ix < k && ix + j < prf_length2; ++ix)
		h = profile2[ix + j][LENCOL];

	return g + h * k;
}

/* calculate the score for opening a gap at residues A[i] and B[j]		 */
int CMultAlignment::gap_penalty2(int i, int j, int k)
{
	if (k <= 0)
		return 0;
	if (j == 0 || j == prf_length2)
		return 0;

	int h = 0;
	int g = profile1[i][GAPCOL] + profile2[j][GAPCOL];
	for (int ix = 0; ix < k && ix + i < prf_length1; ++ix)
		h = profile1[ix + i][LENCOL];

	return g + h * k;
}

/* calculate the score for opening a gap at residues A[i] and B[j]		 */
int CMultAlignment::open_penalty1(int i, int j)
{
	if (i == 0 || i == prf_length1)
		return 0;

	return profile2[j][GAPCOL] + profile1[i][GAPCOL];
}

/* calculate the score for opening a gap at residues A[i] and B[j]		 */
int CMultAlignment::open_penalty2(int i, int j)
{
	if (j == 0 || j == prf_length2)
		return 0;

	return profile1[i][GAPCOL] + profile2[j][GAPCOL];
}

int CMultAlignment::prfscore(int n, int m)
{
	int score = 0;
	for (int ix = 0; ix <= max_aa; ++ix)
		score += profile1[n][ix] * profile2[m][ix];

	score += profile1[n][gap_pos1] * profile2[m][gap_pos1];
	score += profile1[n][gap_pos2] * profile2[m][gap_pos2];
	return score / 10;
}

void CMultAlignment::palign()
{
	displ[print_ptr++] = last_print = 0;
}

/* calculate the score for extending an existing gap at A[i] and B[j]	 */
int CMultAlignment::ext_penalty1(int i, int j)
{
	if (i == 0 || i == prf_length1)
		return 0;

	return profile2[j][LENCOL];
}

/* calculate the score for extending an existing gap at A[i] and B[j]	 */
int CMultAlignment::ext_penalty2(int i, int j)
{
	if (j == 0 || j == prf_length2)
		return 0;

	return profile1[i][LENCOL];
}

int CMultAlignment::pdiff(int A, int B, int M, int N, int go1, int go2)
{
	int midi, midj, type;
	int midh;

	int t, tl, g, h;

	{
		int i,j;
		int hh, f, e, s;

/* Boundary cases: M <= 1 or N == 0 */

/* if sequence B is empty....														  */

		if(N <= 0) {

/* if sequence A is not empty....													 */

			if(M > 0) {

/* delete residues A[1] to A[M]														*/

				pdel(M);
			}
			return -gap_penalty1(A, B, M);
		}

/* if sequence A is empty....														  */

		if(M <= 1) {
			if(M <= 0) {

/* insert residues B[1] to B[N]														*/

				padd(N);
				return -gap_penalty2(A, B, N);
			}

/* if sequence A has just one residue....										  */

			if (go1 == 0)
				midh = -gap_penalty1(A + 1, B + 1, N);
			else
				midh = -gap_penalty2(A + 1, B, 1) - gap_penalty1(A + 1, B + 1, N);
			if (go2 == 0)
				hh = -gap_penalty1(A, B + 1, N);
			else
				hh = -gap_penalty1(A, B + 1, N) - gap_penalty2(A + 1, B + N, 1);
			if(hh > midh)
				midh = hh;
			midj = 0;
			for(j = 1; j <= N; ++j) {
				hh = -gap_penalty1(A, B + 1, j - 1) + prfscore(A + 1, B + j) - gap_penalty1(A + 1, B + j + 1, N - j);
				if(hh > midh) {
					midh = hh;
					midj = j;
				}
			}

			if(midj == 0) {
				pdel(1);
				padd(N);
			}
			else {
				if(midj > 1)
					padd(midj - 1);
				palign();
				if(midj < N)
					padd(N - midj);
			}
			return midh;
		}


/* Divide sequence A in half: midi */

		midi = M / 2;

/* In a forward phase, calculate all HH[j] and HH[j] */

		HH[0] = 0;
		t = -open_penalty1(A, B + 1);
		tl = -ext_penalty1(A, B + 1);
		for(j = 1; j <= N; ++j) {
			HH[j] = t = t + tl;
			DD[j] = t - open_penalty2(A + 1, B + j);
		}

		if (go1 == 0)
			t = 0;
		else
			t = -open_penalty2(A + 1, B);
		tl = -ext_penalty2(A + 1, B);
		for(i = 1; i <= midi; ++i) {
			s = HH[0];
			HH[0] = hh = t = t + tl;
			f = t - open_penalty1(A + i, B + 1);

			for(j = 1; j <= N; ++j) {
				g = open_penalty1(A + i, B + j);
				h = ext_penalty1(A + i, B + j);
				if ((hh = hh - g - h) > (f = f - h))
					f = hh;
				g = open_penalty2(A + i, B + j);
				h = ext_penalty2(A + i, B + j);
				if ((hh = HH[j] - g - h) > (e = DD[j] - h))
					e = hh;
				hh = s + prfscore(A + i, B + j);
				if (f > hh)
					hh = f;
				if (e > hh)
					hh = e;

				s = HH[j];
				HH[j] = hh;
				DD[j] = e;

			}
		}

		DD[0] = HH[0];

/* In a reverse phase, calculate all RR[j] and SS[j] */

		RR[N] = 0;
		tl = 0;
		for(j = N - 1; j >= 0; --j) {
			g = -open_penalty1(A + M, B + j + 1);
			tl -= ext_penalty1(A + M, B + j + 1);
			RR[j] = g + tl;
			SS[j] = RR[j] - open_penalty2(A + M, B + j);
			gS[j] = open_penalty2(A + M, B + j);
		}

		tl = 0;
		for(i = M - 1; i >= midi; --i) {
			s = RR[N];
			if (go2 == 0)
				g = 0;
			else
				g = -open_penalty2(A + i + 1, B + N);
			tl -= ext_penalty2(A + i + 1, B + N);
			RR[N] = hh = g + tl;
			t = open_penalty1(A + i, B + N);
			f = RR[N] - t;

			for(j = N - 1; j >= 0; --j) {
				g = open_penalty1(A + i, B + j + 1);
				h = ext_penalty1(A + i, B + j + 1);
				if ((hh = hh - g - h) > (f = f - h - g + t))
					f = hh;
				t = g;
				g = open_penalty2(A + i + 1, B + j);
				h = ext_penalty2(A + i + 1, B + j);
				hh = RR[j] - g - h;
				if (i == (M - 1))
					e = SS[j] - h;
				else {
					e = SS[j] - h - g + open_penalty2(A + i + 2, B + j);
					gS[j] = g;
				}
				if (hh > e)
					e = hh;
				hh = s + prfscore(A + i + 1, B + j + 1);
				if (f > hh)
					hh = f;
				if (e > hh)
					hh = e;

				s = RR[j];
				RR[j] = hh;
				SS[j] = e;
			}
		}
		SS[N] = RR[N];
		if (go2 == 0)
			gS[N] = 0;
		else
			gS[N] = open_penalty2(A + midi + 1, B + N);

/*		find midj, such that HH[j]+RR[j] or DD[j]+SS[j]+gap is the maximum */

		midh = HH[0] + RR[0];
		midj = 0;
		type = 1;
		for(j = 0; j <= N; ++j) {
			hh = HH[j] + RR[j];
			if (hh >= midh) {
				if (hh > midh || (HH[j] != DD[j] && RR[j] == SS[j])) {
					midh = hh;
					midj = j;
				}
			}
		}

		for(j = N; j >= 0; --j) {
			g = open_penalty2(A + midi + 1, B + j);
			hh = DD[j] + SS[j] + gS[j];
			if(hh > midh) {
				midh = hh;
				midj = j;
				type = 2;
			}
		}
	}

/*		Conquer recursively around midpoint											  */

	if(type == 1) {				 /* Type 1 gaps  */
		pdiff(A, B, midi, midj, 1, 1);
		pdiff(A + midi, B + midj, M - midi, N - midj, 1, 1);
	}
	else {
		pdiff(A, B, midi - 1, midj, 1, 0);
		pdel(2);
		pdiff(A + midi + 1, B + midj, M - midi - 1, N - midj, 0, 1);
	}

	return midh;		 /* Return the score of the best alignment */
}

void CMultAlignment::ptracepath(int *alen)
{
	int i, j, k, pos, to_do;

	pos = 0;

	to_do = print_ptr - 1;

	for(i = 1; i <= to_do; ++i) {
		if(displ[i] == 0) {
			aln_path1[pos] = 2;
			aln_path2[pos] = 2;
			++pos;
		}
		else {
			if((k = displ[i]) > 0) {
				for(j = 0; j <= k - 1; ++j) {
					aln_path2[pos + j] = 2;
					aln_path1[pos + j] = 1;
				}
				pos += k;
			}
			else {
				k = (displ[i] < 0) ? displ[i] * -1 : displ[i];
				for(j = 0; j <= k - 1; ++j) {
					aln_path1[pos + j] = 2;
					aln_path2[pos + j] = 1;
				}
				pos += k;
			}
		}
	}

	(*alen) = pos;
}

string CMultAlignment::add_ggaps_mask(string& mask, int len, const string& path1, const string& path2)
{
	int i,ix;
	string ta;

	ta.resize(len + 1);

	ix = 0;
	if (!switch_profiles) {
		for (i = 0; i < len; ++i) {
			if (path1[i] == 2) {
				ta[i] = mask[ix];
				ix++;
			}
			else if (path1[i] == 1)
				ta[i] = gap_pos1;
		}
	}
	else {
		for (i = 0; i < len; ++i) {
			if (path2[i] == 2) {
				ta[i] = mask[ix];
				ix++;
			}
			else if (path2[i] == 1)
				ta[i] = gap_pos1;
		}
	}

	mask = ta;
	return mask;
}

void CMultAlignment::add_ggaps()
{
	int j;
	int i, ix;
	int len;
	string ta;

	ta.resize(alignment_len + 1);

	for (j = 0; j < nseqs1; ++j) {
		ix = 0;
		for (i = 0; i < alignment_len; ++i) {
			if (aln_path1[i] == 2) {
				if (ix < aln_len[j])
					ta[i] = alignment[j][ix];
				else
					ta[i] = ENDALN;
				ix++;
			}
			else if (aln_path1[i] == 1) {
/*
		insertion in first alignment...
*/
				ta[i] = gap_pos1;
			}
			else
				cerr << "Error in aln_path\n";
		}
		ta[i] = ENDALN;

		len = alignment_len;
		alignment[j].resize(len+2);
		for (i = 0; i < len; ++i)
			alignment[j][i] = ta[i];
		alignment[j][i] = ENDALN;
		aln_len[j] = len;
	}

	for (j = nseqs1; j < nseqs1 + nseqs2; ++j) {
		ix = 0;
		for (i=0; i<alignment_len; ++i) {
			if (aln_path2[i] == 2) {
				if (ix < aln_len[j])
					ta[i] = alignment[j][ix];
				else
					ta[i] = ENDALN;
				ix++;
			}
			else if (aln_path2[i] == 1) {
/*
		insertion in second alignment...
*/
				ta[i] = gap_pos1;
			}
			else
				cerr << "Error in aln_path\n";
		}
		ta[i] = ENDALN;

		len = alignment_len;
		alignment[j].resize(len+2);
		for (i = 0; i < len; ++i)
			alignment[j][i] = ta[i];
		alignment[j][i] = ENDALN;
		aln_len[j] = len;
	}

	ta.clear();

	if (struct_penalties1 != NONE)
		gap_penalty_mask1 = add_ggaps_mask(gap_penalty_mask1, alignment_len, aln_path1, aln_path2);
	if (struct_penalties1 == SECST)
		sec_struct_mask1 = add_ggaps_mask(sec_struct_mask1, alignment_len, aln_path1, aln_path2);

	if (struct_penalties2 != NONE)
		gap_penalty_mask2 = add_ggaps_mask(gap_penalty_mask2, alignment_len, aln_path2, aln_path1);
	if (struct_penalties2 == SECST)
		sec_struct_mask2 = add_ggaps_mask(sec_struct_mask2, alignment_len, aln_path2, aln_path1);
}

int CMultAlignment::prfalign(int *group, int *aligned)
{
	char c;
	int i, j, count = 0;
	int NumSeq;
	int len, len1, len2, is, minlen;
	int se1, se2, sb1, sb2;
	int maxres;
	int int_scale;	
	int score;
	float scale;
	double logmin = 0.0, logdiff;
	double pcid;
	const short *matptr;
	short *mat_xref;

	alignment.clear();
	alignment.resize(nseqs);
	aln_len = make_unique<int[]>(nseqs);
	auto aln_weight = make_unique<int[]>(nseqs);

	for (i = 0; i < nseqs; ++i) {
		if (aligned[i + 1] == 0)
			group[i + 1] = 0;
	}

	nseqs1 = nseqs2 = 0;
	for (i = 0; i < nseqs; ++i) {
		if (group[i + 1] == 1)
			nseqs1++;
		else if (group[i + 1] == 2)
			nseqs2++;
	}

	if (nseqs1 == 0 || nseqs2 == 0)
		return 0;

	if (nseqs2 > nseqs1) {
		switch_profiles = true;
		for (i = 0; i < nseqs; ++i) {
			if (group[i + 1] == 1)
				group[i + 1] = 2;
			else if (group[i + 1] == 2)
				group[i + 1] = 1;
		}
	}
	else
		switch_profiles = false;

	int_scale = 100;
	if (dnaflag) {
		scale = 1.0f;
		if (pw_dnamtrxname == "iub") {
			matptr = swgapdnamt;
			mat_xref = def_dna_xref.get();
		}
		else if (pw_dnamtrxname == "clustalw") {
			matptr = clustalvdnamt;
			mat_xref = def_dna_xref.get();
			scale = 0.66f;
		}
		else {
			matptr = userdnamat.get();
			mat_xref = dna_xref.get();
		}
		maxres = get_matrix(matptr, mat_xref, matrix, neg_matrix, int_scale);
		if (maxres == 0)
			return -1;
		matrix[0][4] = transition_weight * matrix[0][0];
		matrix[4][0] = transition_weight * matrix[0][0];
		matrix[2][11] = transition_weight * matrix[0][0];
		matrix[11][2] = transition_weight * matrix[0][0];
		matrix[2][12] = transition_weight * matrix[0][0];
		matrix[12][2] = transition_weight * matrix[0][0];
	}
	else {
/*
		calculate the mean of the sequence pc identities between the two groups
*/
		count = 0;
		pcid = 0.0;
		for (i = 0; i < nseqs; ++i) {
			if (group[i + 1] == 1) {
				for (j = 0; j < nseqs; ++j) {
					if (group[j + 1] == 2) {
						count++;
						if (pcid < tmat[i + 1][j + 1])
							pcid = tmat[i + 1][j + 1];
						/*
							pcid += tmat[i + 1][j + 1];
						*/
					}
				}
			}
		}

		scale = 0.75f;
		if (mtrxname == "blosum") {
			if (distance_tree == false)
				matptr = blosum40mt;
			else if (pcid > 80.0) {
				scale = 0.5f;
				matptr = blosum80mt;
				scale *= -3.5f + pcid / 20.0f;
			}
			else if (pcid > 60.0) {
				scale = 0.5f;
				matptr = blosum62mt2;
				scale *= -2.5f + pcid / 20.0f;
			}
			else if (pcid > 30.0) {
				scale = 0.5f;
				matptr = blosum45mt;
				scale *= -0.4f + pcid / 30.0f;
			}
			else if (pcid > 10.0) {
				matptr = blosum30mt;
				scale *= pcid / 30.0f;
			}
			else {
				matptr = blosum30mt;
				scale *= 0.30f;
			}
			mat_xref = def_aa_xref.get();
		}
		else if (mtrxname == "pam") {
			if (distance_tree == false)
				matptr = pam120mt;
			else if (pcid > 80.0)
				matptr = pam20mt;
			else if (pcid > 60.0)
				matptr = pam60mt;
			else if (pcid > 40.0)
				matptr = pam120mt;
			else
				matptr = pam350mt;
			mat_xref = def_aa_xref.get();
		}
		else if (mtrxname == "gonnet") {
			if (distance_tree == false)
				matptr = gon120mt;
			else if (pcid > 65.0) {
				matptr = gon40mt;
				scale *= -0.5f + pcid / 65.0f;
			}
			else if (pcid > 45.0) {
				matptr = gon80mt;
				scale *= -1.5f + pcid / 20.0f;
			}
			else if (pcid > 35.0) {
				matptr = gon120mt;
				scale *= -2.5f + pcid / 10.0f;
			}
			else if (pcid > 25.0) {
				matptr = gon160mt;
				scale *= -1.5f + pcid / 10.0f;
			}
			else if (pcid > 15.0) {
				matptr = gon250mt;
				scale *= -0.5f + pcid  / 10.0f;
			}
			else {
				matptr = gon300mt;
				scale *= 0.5f;
			}
			mat_xref = def_aa_xref.get();
			int_scale /= 10;
		}
		else if (mtrxname == "id") {
			matptr = idmat;
			mat_xref = def_aa_xref.get();
		}
		else {
			matptr = usermat[0];
			mat_xref = aa_xref.get();
		}

		maxres = get_matrix(matptr, mat_xref, matrix, neg_matrix, int_scale);
		if (maxres == 0) {
			cerr << "Error: matrix " << mtrxname << " not found\n";
			return -1;
		}
	}

/*
	Make the first profile.
*/
	prf_length1 = 0;
	nseqs1 = 0;

	for (i = 0; i < nseqs; ++i) {
		if (group[i + 1] == 1) {
			len = seqlen_array[i + 1];
			alignment[nseqs1].clear();
			alignment[nseqs1].resize(len+2);
			for (j = 0; j < len; ++j)
				alignment[nseqs1][j] = seq_array[i + 1][j + 1];
			alignment[nseqs1][j] = ENDALN;
			aln_len[nseqs1] = len;
			aln_weight[nseqs1] = seq_weight[i];
			if (len > prf_length1)
				prf_length1 = len;
			nseqs1++;
		}
	}

/*
	Make the second profile.
*/
	prf_length2 = 0;
	nseqs2 = 0;

	for (i = 0; i < nseqs; ++i) {
		if (group[i + 1] == 2) {
			len = seqlen_array[i + 1];
			alignment[nseqs1 + nseqs2].clear();
			alignment[nseqs1 + nseqs2].resize(len + 2);
			for (j = 0; j < len; ++j)
				alignment[nseqs1 + nseqs2][j] = seq_array[i + 1][j + 1];
			alignment[nseqs1 + nseqs2][j] = ENDALN;
			aln_len[nseqs1 + nseqs2] = len;
			aln_weight[nseqs1 + nseqs2] = seq_weight[i];
			if (len > prf_length2)
				prf_length2 = len;
			nseqs2++;
		}
	}

	max_aln_length = prf_length1 + prf_length2 + 2;

	len1 = 0;
	for (i = 0; i < nseqs1; ++i) {
		is = 0;
		for (j = 0; j < aln_len[i]; ++j) {
			c = alignment[i][j];
			if (c != gap_pos1 && c != gap_pos2)
				is++;
		}
		if(is > len1)
			len1 = is;
	}

	len2 = 0;
	for (i = 0; i < nseqs2; ++i) {
		is = 0;
		for (j=0; j < aln_len[i]; ++j) {
			c = alignment[i][j];
			if (c != gap_pos1 && c != gap_pos2)
				is++;
		}
		if(is > len2)
			len2 = is;
	}

	if(len1 == 0 || len2 == 0) {
		logmin = 0;
		logdiff = 1.0;
	}
	else {
		minlen = min(len1, len2);
		if (logmin <= 1000.0)
			logmin = 0;
		else
			logmin = log((double) minlen);
		if (len1 <= len2)
			logdiff = 1.0 - log((double)((float) len1 / (float) len2));
		else
			logdiff = 1.0 - log((double)((float) len2 / (float) len1));
	}
/*
	round logdiff to the nearest integer
*/
	if ((logdiff - (int) logdiff) > 0.5)
		logdiff = ceil(logdiff);
	else
		logdiff = floor(logdiff);


	if (dnaflag) {
		gapcoef1 = gapcoef2 = 100 * gap_open * scale;
		lencoef1 = lencoef2 = 100 * gap_extend * scale;
	}
	else {
		if (neg_matrix) {
			gapcoef1 = gapcoef2 = 200 * (gap_open + logmin);
			lencoef1 = lencoef2 = 200 * gap_extend;
		}
		else {
			if (mat_avscore <= 0)
				gapcoef1 = gapcoef2 = 100 * (gap_open + logmin);
			else
				gapcoef1 = gapcoef2 = scale * mat_avscore * (gap_open + logmin);
			lencoef1 = lencoef2 = 100 * gap_extend;
		}

		gapcoef2 *= logdiff;
		lencoef2 *= logdiff * 15;
	}

	profile1.clear();
	profile1.resize(prf_length1 + 2);
	for (i = 0; i < prf_length1 + 2; ++i) {
		vector<int> row(LENCOL + 2);
		profile1[i] = row;
	}
	
	profile2.clear();
	profile2.resize(prf_length2 + 2);
	for (i = 0; i < prf_length2 + 2; ++i) {
		vector<int> row(LENCOL + 2);
		profile2[i] = row;
	}

/*
	calculate the Gap Coefficients.
*/
	auto gaps = make_unique<int[]>(max_aln_length + 1);

	if (!switch_profiles)
		calc_gap_coeff(alignment, gaps.get(), profile1, (struct_penalties1 && use_ss1), gap_penalty_mask1,
			0, nseqs1, prf_length1, gapcoef1, lencoef1);
	else
		calc_gap_coeff(alignment, gaps.get(), profile1, (struct_penalties2 && use_ss2), gap_penalty_mask2,
			0, nseqs1, prf_length1, gapcoef1, lencoef1);
/*
	calculate the profile matrix.
*/
	calc_prf1(profile1, alignment, gaps.get(), matrix, aln_weight.get(), prf_length1, 0, nseqs1);

/*
	calculate the Gap Coefficients.
*/

	if (!switch_profiles)
		calc_gap_coeff(alignment, gaps.get(), profile2, (struct_penalties2 && use_ss2), gap_penalty_mask2,
			nseqs1, nseqs1 + nseqs2, prf_length2, gapcoef2, lencoef2);
	else
		calc_gap_coeff(alignment, gaps.get(), profile2, (struct_penalties1 && use_ss1), gap_penalty_mask1,
			nseqs1, nseqs1 + nseqs2, prf_length2, gapcoef2, lencoef2);
/*
  calculate the profile matrix.
*/
	calc_prf2(profile2, alignment, aln_weight.get(), prf_length2, nseqs1, nseqs1 + nseqs2);

	aln_path1.clear();
	aln_path1.resize(max_aln_length + 1);
	aln_path2.clear();
	aln_path2.resize(max_aln_length + 1);

/*
	align the profiles
*/
/* use Myers and Miller to align two sequences */

	last_print = 0;
	print_ptr = 1;

	sb1 = sb2 = 0;
	se1 = prf_length1;
	se2 = prf_length2;

	HH = make_unique<int[]>(max_aln_length + 1);
	DD = make_unique<int[]>(max_aln_length + 1);
	RR = make_unique<int[]>(max_aln_length + 1);
	SS = make_unique<int[]>(max_aln_length + 1);
	gS = make_unique<int[]>(max_aln_length + 1);
	displ = make_unique<int[]>(max_aln_length + 1);

	score = pdiff(sb1, sb2, se1 - sb1, se2 - sb2, 0, 0);

	ptracepath(&alignment_len);

	displ.reset();

	add_ggaps();

	prf_length1 = alignment_len;

	NumSeq = 0;
	for (j = 0; j < nseqs; ++j) {
		if (group[j + 1] == 1) {
			seqlen_array[j + 1] = prf_length1;
			realloc_seq(j + 1, prf_length1);
			for (i = 0; i < prf_length1; ++i)
				seq_array[j + 1][i + 1] = alignment[NumSeq][i];
			NumSeq++;
		}
	}
	for (j = 0; j < nseqs; ++j) {
		if (group[j + 1] == 2) {
			seqlen_array[j + 1] = prf_length1;
			seq_array[j + 1].resize(prf_length1 + 2);
			realloc_seq(j + 1, prf_length1);
			for (i = 0; i < prf_length1; ++i)
				seq_array[j + 1][i + 1] = alignment[NumSeq][i];
			NumSeq++;
		}
	}

	profile1.clear();
	profile2.clear();

	alignment.clear();
	return score / 100;
}

int CMultAlignment::get_matrix(const short* matptr, short* xref, vector<vector<int> >& matrix, bool neg_flag, int scale)
{
	int gg_score = 1;
	int gr_score = 0;
	int i, j, k, ix = 0;
	int ti, tj;
	int maxres;
	int av1, av2, av3, min, max;
/*
	default - set all scores to 0
*/
	matrix.resize(NUMRES + 2);
	for (i = 0; i < NUMRES + 2; ++i) {
		vector<int> row(NUMRES + 2);
		matrix[i] = row;
	}

	ix = 0;
	maxres = 0;
	for (i = 0; i <= max_aa; ++i) {
		ti = xref[i];
		for (j = 0; j <= i; ++j) {
			tj = xref[j];
			if ((ti != -1) && (tj != -1)) {
				k = matptr[ix];
				if (ti == tj) {
					matrix[ti][ti] = k * scale;
					maxres++;
				}
				else {
					matrix[ti][tj] = k * scale;
					matrix[tj][ti] = k * scale;
				}
				ix++;
			}
		}
	}

	av1 = av2 = av3 = 0;
	for (i = 0; i <= max_aa; ++i) {
		for (j = 0; j <= i; ++j) {
			av1 += matrix[i][j];
			if (i == j)
				av2 += matrix[i][j];
			else
				av3 += matrix[i][j];
		}
	}

	--maxres;
	av1 /= (maxres * maxres) / 2;
	av2 /= maxres;
	av3 /= ((float)(maxres * maxres - maxres)) / 2;

	min = max = matrix[0][0];
	for (i = 0; i <= max_aa; ++i) {
		for (j = 1; j <= i; ++j) {
			if (matrix[i][j] < min)
				min = matrix[i][j];
			if (matrix[i][j] > max)
				max = matrix[i][j];
		}
	}
	/*
		if requested, make a positive matrix - add -(lowest score) to every entry
	*/
	if (neg_flag == false) {
		if (min < 0) {
			for (i = 0; i <= max_aa; ++i) {
				ti = xref[i];
				if (ti != -1) {
					for (j = 0; j <= max_aa; ++j) {
						tj = xref[j];
	/*
						if (tj != -1) matrix[ti][tj] -= (2*av3);
	*/
						if (tj != -1)
							matrix[ti][tj] -= min;
					}
				}
			}
		}
/*
		gr_score = av3;
		gg_score = -av3;
*/

	}

	mat_avscore = -av3;


	for (i = 0; i < gap_pos1; ++i) {
		matrix[i][gap_pos1] = gr_score;
		matrix[gap_pos1][i] = gr_score;
		matrix[i][gap_pos2] = gr_score;
		matrix[gap_pos2][i] = gr_score;
	}
	matrix[gap_pos1][gap_pos1] = gg_score;
	matrix[gap_pos2][gap_pos2] = gg_score;
	matrix[gap_pos2][gap_pos1] = gg_score;
	matrix[gap_pos2][gap_pos1] = gg_score;

	maxres += 2;
	return maxres;
}

unsigned long CMultAlignment::mult(unsigned long p, unsigned long q)
{
	const size_t m = 100000000;
	const size_t m1 = 10000;
	unsigned long p1, p0, q1, q0;

	p1 = p / m1; p0 = p % m1;
	q1 = q / m1; q0 = q % m1;
	return (((p0 * q1 + p1 * q0) % m1) * m1 + p0 * q0) % m;
}

void CMultAlignment::addrandinit(unsigned long s)
{
	unsigned long j;
	unsigned long a[55];
	const size_t m = 100000000;

	a[0] = s;
	j = 0;
	do {
		++j;
		a[j] = (mult(31, a[j - 1]) + 1) % m;
	} while (j < 54);
}

void CMultAlignment::nj_tree(vector<string>& tree_description, ofstream& tree)
{
	register int i;
	int l[4], nude, k;
	int nc, mini, minj, j, ii, jj;
	double fnseqs, fnseqs2=0, sumd;
	double diq, djq, dij, d2r, dr, dio, djo, da;
	double tmin, total, dmin;
	double bi, bj, b1, b2, b3, branch[4];
	int typei, typej;				 /* 0 = node; 1 = OTU */

	fnseqs = (double)last_seq-first_seq+1;

/*********************** First initialisation ***************************/
	if(verbose)  {
		tree << "\n\n\t\t\tNeighbor-joining Method\n";
		tree << "\n Saitou, N. and Nei, M. (1987)";
		tree << " The Neighbor-joining Method:";
		tree << "\n A New Method for Reconstructing Phylogenetic Trees.";
		tree << "\n Mol. Biol. Evol., 4(4), 406-425\n";
		tree << "\n\n This is an UNROOTED tree\n";
		tree << "\n Numbers in parentheses are branch lengths\n\n";
	}

	mini = minj = 0;

	left_branch = make_unique<double[]>(nseqs + 2);
	right_branch = make_unique<double[]>(nseqs + 2);
	tkill = make_unique<int[]>(nseqs + 1);
	av = make_unique<double[]>(nseqs + 1);


/*********************** Enter The Main Cycle ***************************/

/**start main cycle**/
	for(nc = 1; nc <= (last_seq - first_seq + 1 - 3); ++nc) {
		sumd = 0.0;
		for(j = 2; j <= last_seq-first_seq + 1; ++j)
			for(i = 1; i < j; ++i) {
				tmat[j][i] = tmat[i][j];
				sumd += tmat[i][j];
			}

		tmin = 99999.0;

/*.................compute SMATij values and find the smallest one ........*/

		for(jj = 2; jj <= last_seq - first_seq + 1; ++jj)
			if(tkill[jj] != 1)
				for(ii = 1; ii < jj; ++ii)
					if(tkill[ii] != 1) {
						diq = djq = 0.0;

						for(i = 1; i <= last_seq - first_seq + 1; ++i) {
							diq = diq + tmat[i][ii];
							djq = djq + tmat[i][jj];
						}

						dij = tmat[ii][jj];
						d2r = diq + djq - (2.0 * dij);
						dr  = sumd - dij - d2r;
						fnseqs2 = fnseqs - 2.0;
							total= d2r + fnseqs2 * dij + dr * 2.0;
						total /= 2.0 * fnseqs2;

						if(total < tmin) {
							tmin = total;
							mini = ii;
							minj = jj;
						}
					}


/*.................compute branch lengths and print the results ........*/


		dio = djo = 0.0;
		for(i = 1; i <= last_seq - first_seq + 1; ++i) {
			dio += tmat[i][mini];
			djo += tmat[i][minj];
		}

		dmin = tmat[mini][minj];
		dio = (dio - dmin) / fnseqs2;
		djo = (djo - dmin) / fnseqs2;
		bi = (dmin + dio - djo) * 0.5;
		bj = dmin - bi;
		bi -= av[mini];
		bj =- av[minj];

		if(av[mini] > 0.0)
			typei = 0;
		else
			typei = 1;
		if(av[minj] > 0.0)
			typej = 0;
		else
			typej = 1;


/*
	set negative branch lengths to zero.  Also set any tiny positive
	branch lengths to zero.
*/		if(fabs(bi) < 0.0001)
			bi = 0.0;
		if(fabs(bj) < 0.0001)
			bj = 0.0;

		if(verbose) {
			if(typei == 0)
				tree << "Node:" << setw(4) << mini << "(" << setprecision(5) << bi << ") joins ";
			else
				tree << "SEQ:" << setw(4) << mini << "(" << setprecision(5) << bi << ") joins ";

			if(typej == 0)
				tree << "Node:" << setw(4) << minj << "(" << setprecision(5) << bj << ") joins ";
			else
				tree << "SEQ:" << setw(4) << minj << "(" << setprecision(5) << bj << ") joins ";

			tree << '\n';
		}


		left_branch[nc] = bi;
		right_branch[nc] = bj;

		for(i = 1; i <= last_seq - first_seq + 1; ++i)
			tree_description[nc][i] = 0;

			if(typei == 0) {
				for(i = nc - 1; i >= 1; --i)
					if(tree_description[i][mini] == 1) {
						for(j = 1; j <= last_seq - first_seq + 1; ++j)
							if(tree_description[i][j] == 1)
								tree_description[nc][j] = 1;
						break;
					}
			}
			else
				tree_description[nc][mini] = 1;

			if(typej == 0) {
				for(i = nc - 1; i >= 1; --i)
					if(tree_description[i][minj] == 1) {
						for(j = 1; j <= last_seq - first_seq + 1; ++j)
							if(tree_description[i][j] == 1)
								tree_description[nc][j] = 1;
						break;
					}
			}
			else
				tree_description[nc][minj] = 1;


/*
	Here is where the -0.00005 branch lengths come from for 3 or more
	identical seqs.
*/
/*		if(dmin <= 0.0) dmin = 0.0001; */
		if(dmin <= 0.0)
			dmin = 0.000001;
		av[mini] = dmin * 0.5;

/*........................Re-initialisation................................*/

		fnseqs = fnseqs - 1.0;
		tkill[minj] = 1;

		for(j = 1; j <= last_seq - first_seq + 1; ++j)
			if(tkill[j] != 1) {
				da = (tmat[mini][j] + tmat[minj][j]) * 0.5;
				if((mini - j) < 0)
					tmat[mini][j] = da;
				if((mini - j) > 0)
					tmat[j][mini] = da;
			}

		for(j = 1; j <= last_seq - first_seq + 1; ++j)
			tmat[minj][j] = tmat[j][minj] = 0.0;


	}						/**end main cycle**/

/******************************Last Cycle (3 Seqs. left)********************/

	nude = 1;

	for(i = 1; i <= last_seq - first_seq + 1; ++i)
		if(tkill[i] != 1) {
			l[nude] = i;
			nude = nude + 1;
		}

	b1 = (tmat[l[1]][l[2]] + tmat[l[1]][l[3]] - tmat[l[2]][l[3]]) * 0.5;
	b2 =  tmat[l[1]][l[2]] - b1;
	b3 =  tmat[l[1]][l[3]] - b1;

	branch[1] = b1 - av[l[1]];
	branch[2] = b2 - av[l[2]];
	branch[3] = b3 - av[l[3]];

/* Reset tiny negative and positive branch lengths to zero */
	if(fabs(branch[1]) < 0.0001)
		branch[1] = 0.0;
	if(fabs(branch[2]) < 0.0001)
		branch[2] = 0.0;
	if(fabs(branch[3]) < 0.0001)
		branch[3] = 0.0;

	left_branch[last_seq - first_seq - 1] = branch[1];
	left_branch[last_seq - first_seq] = branch[2];
	left_branch[last_seq - first_seq + 1] = branch[3];

	for(i = 1; i <= last_seq - first_seq + 1; ++i)
		tree_description[last_seq - first_seq - 1][i] = 0;

	if(verbose)
		tree << "\n Cycle" << setw(4) << nc << " (Last cycle, trichotomy):\n";

	for(i = 1; i <= 3; ++i) {
		if(av[l[i]] > 0.0) {
			if(verbose)
				tree << "\n\t\t Node:" << setw(4) << l[i] << "(" << setprecision(5) << branch[i] << ") ";
		for(k = last_seq - first_seq - 2; k >= 1; k--)
			if(tree_description[k][l[i]] == 1) {
				for(j = 1; j <= last_seq - first_seq + 1; ++j)
					if(tree_description[k][j] == 1)
						tree_description[last_seq - first_seq - 1][j] = i;
				break;
			}
		}
		else {
			if(verbose)
				tree << "\n\t\t  SEQ:" << setw(4) << l[i] << "(" << setprecision(5) << branch[i] << ") ";
			tree_description[last_seq - first_seq - 1][l[i]] = i;
		}
		if(i < 3) {
			if(verbose)
				tree << "joins";
		}
	}

	if(verbose)
		tree << "\n";
}

int CMultAlignment::two_way_split(vector<string>& tree_description, ofstream& tree, int start_row, int flag, int bootstrap)
{
	int row, new_row = 0, old_row, col, test_col = 0;
	bool single_seq;

	if(start_row != last_seq - first_seq - 1)
		tree << "(\n";

	for(col = 1; col <= last_seq - first_seq + 1; col++) {
		if(tree_description[start_row][col] == flag) {
			test_col = col;
			break;
		}
	}

	single_seq = true;
	for(row=start_row - 1; row >= 1; row--)
		if(tree_description[row][test_col] == 1) {
			single_seq = false;
			new_row = row;
			break;
		}

	if(single_seq) {
		tree_description[start_row][test_col] = 0;
		tree << names[test_col + first_seq - 1];
		if(start_row == last_seq - first_seq - 1) {
			return 0;
		}

		tree << ":" << setprecision(5) << left_branch[start_row] << ",\n";
	}
	else {
		for(col = 1; col <= last_seq - first_seq + 1; col++) {
			if((tree_description[start_row][col] == 1)&&
				(tree_description[new_row][col] == 1))
				tree_description[start_row][col] = 0;
		}
		old_row = two_way_split(tree_description, tree, new_row, 1, bootstrap);
		if(start_row == last_seq - first_seq - 1) {
			return new_row;
		}

		tree << ":" << setprecision(5) << left_branch[start_row];
		if ((bootstrap == BS_BRANCH_LABELS) && (boot_totals[old_row] > 0))
			tree << "[" << boot_totals[old_row] << "]";

		tree << ",\n";
	}


	for(col = 1; col <= last_seq - first_seq + 1; col++)
		if(tree_description[start_row][col] == flag) {
			test_col = col;
			break;
		}

	single_seq = true;
	new_row = 0;
	for(row = start_row - 1; row >= 1; row--)
		if(tree_description[row][test_col] == 1) {
			single_seq = false;
			new_row = row;
			break;
		}

	if(single_seq) {
		tree_description[start_row][test_col] = 0;
		tree << names[test_col + first_seq - 1];
		tree << ":" << setprecision(5) << right_branch[start_row] << '\n';
	}
	else {
		for(col = 1; col <= last_seq - first_seq + 1; col++) {
			if((tree_description[start_row][col] == 1)&&
				(tree_description[new_row][col] == 1))
				tree_description[start_row][col] = 0;
		}
		old_row=two_way_split(tree_description, tree, new_row, 1, bootstrap);
		tree << ":" << setprecision(5) << right_branch[start_row];
		if (bootstrap == BS_BRANCH_LABELS && boot_totals[old_row] > 0)
			tree << "[" << boot_totals[old_row] << "]";

		tree << ")\n";
	}
	if (bootstrap == BS_NODE_LABELS && boot_totals[start_row] > 0)
		tree << boot_totals[start_row];

	return start_row;
}

void CMultAlignment::print_phylip_tree(vector<string>& tree_description, ofstream& tree, int bootstrap)
{
	tree << "(\n";

	int old_row = two_way_split(tree_description, tree, last_seq - first_seq - 1, 1, bootstrap);
	tree << ":" << setprecision(5) << left_branch[last_seq - first_seq - 1];
	if (bootstrap == BS_BRANCH_LABELS && old_row > 0 && boot_totals[old_row] > 0)
		tree << "[" << boot_totals[old_row] << "]";
	tree << ",\n";

	old_row = two_way_split(tree_description, tree, last_seq - first_seq - 1, 2, bootstrap);
	tree << ":" << setprecision(5) << left_branch[last_seq - first_seq];
	if (bootstrap == BS_BRANCH_LABELS && old_row > 0 && boot_totals[old_row] > 0)
		tree << "[" << boot_totals[old_row] << "]";
	tree << ",\n";

	old_row = two_way_split(tree_description, tree, last_seq - first_seq - 1, 3, bootstrap);
	tree << ":" << setprecision(5) << left_branch[last_seq-first_seq+1];
	if (bootstrap == BS_BRANCH_LABELS && old_row > 0 && boot_totals[old_row] > 0)
		tree << "[" << boot_totals[old_row] << "]";
	tree << ")";
	if (bootstrap == BS_NODE_LABELS)
		tree << "TRICHOTOMY";
	tree << ";\n";
}

void CMultAlignment::new_sequence_align(wstring& phylip_name)
{
	string path, temp;
	wstring tree_name;
	bool use_tree;
	ofstream tree;
	int i,j,count;
	float dscore;
	bool save_ss2;

	if(profile1_empty && usemenu) {
		cerr << "No profile in memory. Input 1st profile first.";
		return;
	}

	if(profile2_empty && usemenu) {
		cerr << "No sequences in memory. Input sequences first.";
		return;
	}

	get_path(profile2_name, path);
	wstring wPath(path.begin(), path.end());

	if(usemenu || !interactive) {
		if(!open_alignment_output(wPath))
			return;
	}

	new_seq = profile1_nseqs + 1;

	/* check for secondary structure information for list of sequences */

	save_ss2 = use_ss2;
	if (struct_penalties2 != NONE && use_ss2 && (nseqs - profile1_nseqs > 1)) {
		if (struct_penalties2 == SECST)
			cout << "Warning: ignoring secondary structure for a list of sequences" << '\n';
		else if (struct_penalties2 == GMASK)
			cout << "Warning: ignoring gap penalty mask for a list of sequences" << '\n';
			use_ss2 = false;
		}

		for (i = 1; i <= new_seq; ++i) {
			for (j = i + 1; j <= new_seq; ++j) {
				dscore = countid(i,j);
				tmat[i][j] = (100.0 - (double) dscore) / 100.0;
				tmat[j][i] = tmat[i][j];
			}
		}

		tree_name.clear();
		use_tree = false;
		if (nseqs > 3) {
			if (check_tree && usemenu) {
				tree_name = wPath + L"dnd";
			tree.open(tree_name);
			if(tree.good()) {
				phylip_name = tree_name;
				use_tree = true;
				tree.close();
			}
			else if (!usemenu && use_tree_file)
				use_tree = true;
		}

		if (save_parameters)
			create_parameter_output();

		if(reset_alignments) {
			reset_prf1();
			reset_prf2();
		}
		else
			fix_gaps();

		if (struct_penalties1 == SECST)
			calc_gap_penalty_mask(seqlen_array[1], sec_struct_mask1, gap_penalty_mask1);

		if (struct_penalties2 == SECST)
			calc_gap_penalty_mask(seqlen_array[profile1_nseqs+1], sec_struct_mask2, gap_penalty_mask2);


	/* create the new tree file, if necessary */

		if (!use_tree) {
			if (nseqs > 3) {
				get_path(profile2_name, path);
				if (phylip_name.length() > 0) {
					if(!open_explicit_file(tree, phylip_name))
						return;
				}
			}
			cout << "Start of Pairwise alignments\n";
			cout << "Aligning...\n";

			if(dnaflag) {
				gap_open = dna_gap_open;
				gap_extend = dna_gap_extend;
				pw_go_penalty = dna_pw_go_penalty;
				pw_ge_penalty = dna_pw_ge_penalty;
				ktup = dna_ktup;
				window = dna_window;
				signif = dna_signif;
				wind_gap = dna_wind_gap;
			}
			else {
				gap_open = prot_gap_open;
				gap_extend = prot_gap_extend;
				pw_go_penalty = prot_pw_go_penalty;
				pw_ge_penalty = prot_pw_ge_penalty;
				ktup  = prot_ktup;
				window = prot_window;
				signif = prot_signif;
				wind_gap = prot_wind_gap;
			}
			if (quick_pairalign)
				show_pair();
			else
				pairalign(0,nseqs,new_seq - 2,nseqs);

			if (nseqs > 3) {
				guide_tree(1, nseqs);
				cout << "Guide tree        file created:   [" << "]" << phylip_name.c_str() << '\n';
			}
		}
	}

	if (new_tree_file)
		return;

	count = seqalign(new_seq - 2, phylip_name);

	use_ss2 = save_ss2;

	if (count <= 0)
		return;

	if (usemenu)
		cout << "\n\n\n";

	create_alignment_output(1, nseqs);
	phylip_name.clear();
}

bool CMultAlignment::user_mat(string& str, short *mat, short *xref)
{
	int maxres;

	lin2 = str;

	if(lin2.length() == 0)
		return false;

	ifstream infile(lin2);
	if(infile.bad()) {
		cerr << "Cannot find matrix file [" << lin2 << "]";
		return false;
	}

	str = lin2;

	maxres = read_user_matrix(str, mat, xref);
	return maxres > 0;
}

void CMultAlignment::clustal_out(vector<string>& result, int* order, int fres, int len, int fseq, int lseq)
{
	string seq1;
	string ss_mask1, ss_mask2;
	char c;
	int val;
	int ii, lv1, ident, chunks;
	int i, j, k, l;
	int pos,ptr;
	int line_length;
	int catident1[NUMRES], catident2[NUMRES];

	string temp;
	temp.resize(len+1);
	result.resize(lseq);

	unique_ptr<int[]> seq_no = make_unique<int[]>(nseqs + 1);
	unique_ptr<int[]> print_seq_no = make_unique<int[]>(nseqs + 1);

	seq1.resize(max_aln_length + 2);

	if (struct_penalties1 == SECST && use_ss1) {
		ss_mask1.resize(seqlen_array[1] + 10);
		for (i = 0; i < seqlen_array[1]; ++i)
			ss_mask1[i] = sec_struct_mask1[i];
		print_sec_struct_mask(seqlen_array[1], sec_struct_mask1,ss_mask1);
	}
	if (struct_penalties2 == SECST && use_ss2) {
		ss_mask2.resize(seqlen_array[profile1_nseqs + 1] + 10);
		for (i = 0; i < seqlen_array[profile1_nseqs + 1]; ++i)
			ss_mask2[i] = sec_struct_mask2[i];
		print_sec_struct_mask(seqlen_array[profile1_nseqs + 1], sec_struct_mask2, ss_mask2);
	}

/* decide the line length for this alignment - maximum is LINELENGTH */
	line_length = PAGEWIDTH - max_names;
	line_length -= line_length % 10; /* round to a multiple of 10*/
	if (line_length > LINELENGTH)
		line_length = LINELENGTH;

	chunks = 1;/* len / line_length;
	if(len % line_length != 0)
		++chunks; */

	for(lv1 = 1; lv1 <= chunks; ++lv1) {
		pos = 1;
		ptr = len;
		if (output_struct_penalties == 0 || output_struct_penalties == 2) {
			if (struct_penalties1 == SECST && use_ss1) {
				for(i = pos; i <= ptr; ++i) {
					val = ss_mask1[i + fres - 2];
					if (val == gap_pos1)
						temp[i - pos] = '-';
					else
						temp[i - pos] = val;
				}
				temp.resize(ptr - pos + 1);
			}
		}
		if (output_struct_penalties == 1 || output_struct_penalties == 2) {
			if (struct_penalties1 != NONE && use_ss1) {
				for(i = pos; i <= ptr; ++i) {
					val = gap_penalty_mask1[i + fres - 2];
					if (val == gap_pos1)
						temp[i - pos] = '-';
					else
						temp[i - pos] = val;
				}
				temp.resize(ptr - pos + 1);
			}
		}
		if (output_struct_penalties == 0 || output_struct_penalties == 2) {
			if (struct_penalties2 == SECST && use_ss2) {
				for(i = pos; i <= ptr; ++i) {
					val=ss_mask2[i + fres - 2];
					if (val == gap_pos1)
						temp[i - pos] = '-';
					else
						temp[i - pos] = val;
				}
				temp.resize(ptr - pos + 1);
			}
		}
		if (output_struct_penalties == 1 || output_struct_penalties == 2) {
			if (struct_penalties2 != NONE && use_ss2) {
				for(i = pos; i <= ptr; ++i) {
					val = gap_penalty_mask2[i + fres - 2];
					if (val == gap_pos1)
						temp[i - pos] = '-';
					else
						temp[i - pos] = val;
				}
				temp.resize(ptr - pos + 1);
			}
		}

		for(ii = fseq; ii <= lseq; ++ii) {
			i = output_index[ii];
			print_seq_no[i] = 0;
			for(j = pos; j <= ptr; ++j) {
				if (j + fres - 1 <= seqlen_array[i])
					val = seq_array[i][j + fres - 1];
				else
					val = -3;
				if(val == -3 || val == 253)
					break;
				else if(val < 0 || val > max_aa)
					seq1[j] = '-';
				else {
					seq1[j] = amino_acid_codes[val];
					seq_no[i]++;
					print_seq_no[i] = 1;
				}
			}
			for(; j<=ptr; ++j)
				seq1[j] = '-';
			temp = seq1.substr(pos);
			temp.resize(ptr - pos + 1);
			result[ii - 1] = temp;
			order[ii - 1] = i;
		}

		for(i = pos;i <= ptr; ++i) {
			seq1[i] = ' ';
			ident = 0;
			for(j = 1; res_cat1[j - 1] != ""; ++j)
				catident1[j - 1] = 0;
			for(j = 1; res_cat2[j - 1] != ""; ++j)
				catident2[j - 1] = 0;
			for(j = fseq; j <= lseq; ++j) {
				if(seq_array[fseq][i] >= 0 && seq_array[fseq][i] <= max_aa) {
					if(seq_array[fseq][i] == seq_array[j][i])
						++ident;
					char idx = seq_array[j][i];
					if (idx >= amino_acid_codes.length())
						continue;
					for(k = 1; res_cat1[k - 1] != ""; k++) {
						for(l = 0; (c = res_cat1[k - 1][l]); l++) {
							if (amino_acid_codes[idx] == c) {
								catident1[k - 1]++;
								break;
							}
						}
					}
					for(k = 1; res_cat2[k - 1] != ""; k++) {
						for(l = 0;(c = res_cat2[k - 1][l]); l++) {
							if (amino_acid_codes[idx] == c) {
								catident2[k - 1]++;
								break;
							}
						}
					}
				}
			}
			if(ident == lseq - fseq + 1)
				seq1[i] = '*';
			else if (!dnaflag) {
				for(k = 1; res_cat1[k - 1] != ""; k++) {
					if (catident1[k - 1] == lseq - fseq + 1) {
						seq1[i] = ':';
						break;
					}
				}
				if(seq1[i] == ' ') {
					for(k = 1; res_cat2[k - 1] != ""; k++) {
						if (catident2[k - 1] == lseq - fseq + 1) {
							seq1[i] = '.';
							break;
						}
					}
				}
			}
		}
		temp = seq1.substr(pos);
		temp.resize(ptr - pos + 1);
	}

	seq1.clear();
	temp.clear();
	if (struct_penalties1 == SECST && use_ss1)
		ss_mask1.clear();
	if (struct_penalties2 == SECST && use_ss2)
		ss_mask2.clear();
}

void CMultAlignment::nbrf_out(ofstream& nbout, int fres, int len, int fseq, int lseq)
{
	//nothing
}

void CMultAlignment::gcg_out(ofstream& gcgout, int fres, int len, int fseq, int lseq)
{
	//nothing
}

void CMultAlignment::phylip_out(ofstream& phyout, int fres, int len, int fseq, int lseq)
{
	//nothing
}

void CMultAlignment::gde_out(ofstream& gdeout, int fres, int len, int fseq, int lseq)
{
	//nothing
}

int CMultAlignment::getargs(const string& inline1, vector<string>& args, int max)
{
	args.clear();
	istringstream iss(inline1);
	string token;
	while (getline(iss, token)) {
		size_t found = token.find_last_of('\t');
		if(found != string::npos)
			args.push_back(move(token.substr(0, found)));
		else
			args.push_back(move(token));
	}

	return args.size();
}

int CMultAlignment::read_user_matrix(const string& filename, short *usermat, short *xref)
{
	double f;
	int  numargs, farg;
	int i, j, k = 0;
	string codes, inline1;
	vector<string> args;
	char c1,c2;
	int ix1, ix = 0;
	int  maxres = 0;

	if (filename.length() == 0)
		return 0;

	ifstream fd(filename);
	if (fd.bad()) {
		cerr << "cannot open " << filename;
		return 0;
	}

	args.resize(NUMRES + 4);

	maxres = 0;
	while (getline(fd, inline1)) {
		if (inline1[0] == '\0' || inline1[0] == '#')
			continue;
/*
	read residue characters.
*/
		k = 0;
		for (j = 0; j < inline1.length(); ++j) {
			if (isalpha((int) inline1[j]))
				codes[k++] = inline1[j];
			if (k > NUMRES) {
				cerr << "too many entries in " << filename;
				return 0;
			}
		}
		codes[k] = '\0';
		break;
	}

	if (k == 0) {
		cerr << "wrong format in " << filename;
		return 0;
	}

/*
	cross-reference the residues
*/
	for (i = 0; i < NUMRES; ++i)
		xref[i] = -1;

	maxres = 0;
	for (i = 0; (c1 = codes[i]); ++i) {
		for (j = 0; (c2 = amino_acid_codes[j]); ++j)
			if (c1 == c2)
			{
				xref[i] = j;
				maxres++;
				break;
			}
		if (xref[i] == -1 && codes[i] != '*')
			cout << "residue " << codes[i] << " in " << filename << " not recognised";
	}


/*
	get the weights
*/

	ix = ix1 = 0;
	while (getline(fd, inline1)) {
		if (inline1[0] == '\n')
			continue;
		numargs = getargs(inline1, args, (k + 1));
		if (numargs == 0) {
			cerr << "wrong format in " << filename;
			return 0;
		}
		if (isalpha(args[0][0]))
			farg = 1;
		else
			farg = 0;
		for (i = 0; i <= ix ; ++i) {
			if (xref[i] != -1) {
				f = stof(args[i+farg]);
				usermat[ix1++] = (short)(f * 10.0);
			}
		}
		ix++;
	}
	if (ix != k + 1) {
		cerr << "wrong format in " << filename;
		return 0;
	}

	fd.close();

	maxres += 2;
	return maxres;
}

void CMultAlignment::fill_chartab()	/* Create translation and check table */
{
	register int i;
	register char c;
	chartab.resize(128);

	for(i = 0; (c = amino_acid_codes[i]); ++i)
		chartab[(int)c] = chartab[tolower(c)] = c;
}

int CMultAlignment::readseqs(int first_seq) /*first_seq is the #no. of the first seq. to read */
{	
	bool dnaflag1;
	int i;
	int no_seqs = numberofseqs;
	int l1;	
	string buffer;
	string line;
	string seq1, sname1, title;

	/* return the number of seqs. (zero here)*/
	max_aln_length = 0;

/* if this is a multiple alignment, or profile 1 - free any memory used
by previous alignments, then allocate memory for the new alignment */
	if(first_seq == 1) {
		max_names = 0;
		//free_aln(nseqs);
		alloc_aln(no_seqs);
	}

	for(i = first_seq; i <= first_seq + no_seqs - 1; ++i) {	 /* get the seqs now*/
		output_index[i] = i;	/* default output order */
		l1 = hpsequences[i - 1].length() - 1;
		seq1 = hpsequences[i - 1];
//		get_seq(sname1,&l1,title);
/* JULIE */
/*  Set max length of dynamically allocated arrays in prfalign.c */
		if (l1 > max_aln_length)
			max_aln_length = l1;

		seqlen_array[i]=l1;						 /* store the length */
		buffer = "sequence" + i;
		names[i] = buffer;//sname1);				  /*	 "	"  name	*/
		titles[i] = title;				  /*	 "	"  title  */

		if(!explicit_dnaflag)
		{
			dnaflag1 = check_dnaflag(seq1, l1); /* check DNA/Prot */
				if(i == 1)
					dnaflag = dnaflag1;
			}			/* type decided by first seq*/
		else
			dnaflag1 = dnaflag;

		alloc_seq(i, l1);

		if(dnaflag)
			n_encode(seq1, seq_array[i], l1); /* encode the sequence*/
		else					/* as ints  */
			p_encode(seq1, seq_array[i], l1);
		seq1.clear();
	}

	max_aln_length *= 2;

	for(i = first_seq; i <= first_seq + no_seqs - 1; ++i) {
		if(seqlen_array[i] > max_aln_length)
			max_aln_length = seqlen_array[i];
	}

/* look for a feature table / gap penalty mask (only if this is a profile) */
	if (profile_no > 0) {
		fin.clear();
		fin.seekg(0);

		struct_penalties = NONE;
		gap_penalty_mask.resize(max_aln_length + 1);
		sec_struct_mask.resize(max_aln_length + 1);
		ss_name.resize(MAXNAMES + 1);

		if (seqFormat == CLUSTAL) {
			get_clustal_ss(max_aln_length);
		}
		else if (seqFormat == GDE) {
			get_gde_ss(max_aln_length);
		}
		else if (seqFormat == EMBLSWISS) {
			get_embl_ss(max_aln_length);
		}
		else if (seqFormat == RSF) {
			get_rsf_ss(max_aln_length);
		}
	}

	for(i = first_seq; i <= first_seq + no_seqs - 1; ++i) {
		if(names[i].length() > max_names)
			max_names = names[i].length();
	}

	if(max_names < 10)
		max_names=10;

	return no_seqs;	 /* return the number of seqs. read in this call */
}

void CMultAlignment::make_p_ptrs(int *tptr, int *pl, int naseq, int l)
{
	int a[10];

	for (int i = 1; i <= wind_gap; ++i)
		a[i] = (int) pow((double)(max_aa + 1), (double)(i-1));

	int limit = (int) pow((double)(max_aa + 1),(double) ktup);
	for(int i = 1; i <= limit; ++i)
		pl[i] = 0;
	for(int i = 1; i <= l; ++i)
		tptr[i] = 0;

	for(int i = 1; i <= (l - ktup + 1); ++i) {
		int code = 0;
		bool flag = false;
		for(int j = 1; j <= ktup; ++j) {
			char residue = seq_array[naseq][i + j - 1];
			if((residue < 0) || (residue > max_aa)){
				flag = true;
				break;
			}
			code += ((residue) * a[j]);
		}
		if(flag)
			continue;
		++code;
		if(pl[code] != 0)
			tptr[i] = pl[code];
		pl[code] = i;
	}
}

void CMultAlignment::make_n_ptrs(int *tptr, int *pl, int naseq, int len)
{
	const int pot[] = { 0, 1, 4, 16, 64, 256, 1024, 4096 };

	int limit = (int) pow(4.0f, ktup);

	for(int i = 1; i <= limit; ++i)
		pl[i] = 0;
	for(int i = 1; i <= len; ++i)
		tptr[i] = 0;

	for(int i = 1; i <= len-ktup + 1; ++i) {
		int code = 0;
		bool flag = false;
		for(int j = 1; j <= ktup; ++j) {
			char residue = seq_array[naseq][i + j - 1];
			if((residue < 0) || (residue > 4)){
				flag = true;
				break;
			}
			code += residue * pot[j];  /* DES */
		}
		if(flag)
			continue;
		++code;
		if(pl[code] != 0)
			tptr[i] = pl[code];
		pl[code] = i;
	}
}

void CMultAlignment::des_quick_sort(int *array1, int *array2, int array_size)
/*  */
/* Quicksort routine, adapted from chapter 4, page 115 of software tools */
/* by Kernighan and Plauger, (1986) */
/* Sort the elements of array1 and sort the */
/* elements of array2 accordingly */
/*  */
{
	int p, pivlin;
	int i, j;
	int lst[50], ust[50];		 /* the maximum no. of elements must be*/
								/* < log(base2) of 50 */

	lst[1] = 1;
	ust[1] = array_size;
	p = 1;

	while(p > 0) {
		if(lst[p] >= ust[p])
			p--;
		else {
			i = lst[p] - 1;
			j = ust[p];
			pivlin = array1[j];
			while(i < j) {
				for(i = i + 1; array1[i] < pivlin; ++i)
					;
				for(j = j - 1; j > i; --j)
					if(array1[j] <= pivlin)
						break;
				if(i < j) {
					swap(array1[i], array1[j]);
					swap(array2[i], array2[j]);
				}
			}

			j = ust[p];

			swap(array1[i], array1[j]);
			swap(array2[i], array2[j]);

			if(i - lst[p] < ust[p] - i) {
				lst[p + 1] = lst[p];
				ust[p + 1] = i - 1;
				lst[p] = i + 1;
			}
			else {
				lst[p + 1] = i + 1;
				ust[p + 1] = ust[p];
				ust[p] = i - 1;
			}
			++p;
		}
	}

}

void CMultAlignment::put_frag(int fs,int v1,int v2,int flen)
{
	int end;
	accum[0][curr_frag] = fs;
	accum[1][curr_frag] = v1;
	accum[2][curr_frag] = v2;
	accum[3][curr_frag] = flen;

	if(!maxsf) {
		maxsf = 1;
		accum[4][curr_frag] = 0;
		return;
	}

		if(fs >= accum[0][maxsf]) {
		accum[4][curr_frag] = maxsf;
		maxsf = curr_frag;
		return;
	}
	else {
		next_frag = maxsf;
		for(;;) {
			end = next_frag;
			next_frag = accum[4][next_frag];
			if(fs >= accum[0][next_frag])
				break;
		}
		accum[4][curr_frag] = next_frag;
		accum[4][end] = curr_frag;
	}
}

bool CMultAlignment::frag_rel_pos(int a1, int b1, int a2, int b2)
{
	if(a1 - b1 == a2 - b2) {
		if(a2 < a1)
			return true;
	}
	else {
		if(a2 + ktup - 1 < a1 && b2 + ktup - 1 < b1)
			return true;
	}
	return false;
}

void CMultAlignment::pair_align(int seq_no,int l1,int l2)
{
	int pot[8], i, j, l, m, flag, limit, pos, tl1, vn1, vn2, flen, osptr, fs;
	int tv1, tv2, encrypt, subt1, subt2, rmndr;
	char residue;

	if(dnaflag) {
		for(i = 1; i <= ktup; ++i)
			pot[i] = (int) pow(4.0f, i - 1);
		limit = (int) pow(4.0f, ktup);
	}
	else {
		for (i = 1; i <= wind_gap; ++i)
			pot[i] = (int) pow((float)(max_aa + 1), i - 1);
		limit = (int) pow((float)(max_aa + 1), ktup);
	}

	tl1 = (l1 + l2) - 1;

	for(i = 1;i <= tl1; ++i) {
		slopes[i] = displ[i] = 0;
		diag_index[i] = i;
	}


/* increment diagonal score for each k_tuple match */

	for(i = 1; i <= limit; ++i) {
		vn1 = zzc[i];
		for(;;) {
			if(!vn1)
				break;
			vn2 = zzd[i];
			while(vn2 != 0) {
				osptr = vn1 - vn2 + l2;
				++displ[osptr];
				vn2 = zzb[vn2];
			}
			vn1 = zza[vn1];
		}
	}

/* choose the top SIGNIF diagonals */

	des_quick_sort(displ.get(), diag_index.get(), tl1);

	j = tl1 - signif + 1;
	if(j < 1)
		j = 1;

/* flag all diagonals within WINDOW of a top diagonal */

	for(i = tl1; i >= j; --i)
		if(displ[i] > 0) {
			pos = diag_index[i];
			l = (1 > pos-window) ? 1 : pos - window;
			m = (tl1 < pos + window) ? tl1 : pos + window;
			for(; l <= m; l++)
				slopes[l] = 1;
		}

	for(i = 1; i <= tl1; ++i)
		displ[i] = 0;


	curr_frag = maxsf = 0;

	for(i = 1; i <= (l1 - ktup + 1); ++i) {
		encrypt = flag = 0;
		for(j = 1; j <= ktup; ++j) {
			residue = seq_array[seq_no][i + j - 1];
			if(residue < 0 || residue > max_aa) {
				flag = true;
				break;
			}
			encrypt += residue * pot[j];
		}
		if(flag)
			continue;
		++encrypt;

		vn2 = zzd[encrypt];

		flag = true;
		for(;;) {
			if(!vn2) {
				flag = true;
				break;
			}
			osptr = i - vn2 + l2;
			if(slopes[osptr] != 1) {
				vn2 = zzb[vn2];
				continue;
			}
			flen = 0;
			fs = ktup;
			next_frag = maxsf;

		/*
		* A-loop
		*/

			for(;;) {
				if(!next_frag) {
					++curr_frag;
					if(curr_frag >= 2 * max_aln_length) {
						cout << "(Partial alignment)";
						vatend = 1;
						return;
					}
					displ[osptr] = curr_frag;
					put_frag(fs, i, vn2, flen);
				}
				else {
					tv1=accum[1][next_frag];
					tv2=accum[2][next_frag];
					if(frag_rel_pos(i, vn2, tv1, tv2)) {
						if(i - vn2 == accum[1][next_frag] - accum[2][next_frag]) {
							if(i > accum[1][next_frag] + (ktup - 1))
								fs = accum[0][next_frag] + ktup;
							else {
								rmndr = i - accum[1][next_frag];
								fs = accum[0][next_frag] + rmndr;
							}
							flen = next_frag;
							next_frag = 0;
							continue;
						}
						else {
							if(displ[osptr] == 0)
								subt1 = ktup;
							else {
								if(i > accum[1][displ[osptr]]+(ktup - 1))
									subt1 = accum[0][displ[osptr]] + ktup;
								else {
									rmndr = i - accum[1][displ[osptr]];
									subt1 = accum[0][displ[osptr]] + rmndr;
								}
							}
							subt2 = accum[0][next_frag] - wind_gap + ktup;
							if(subt2 > subt1) {
								flen = next_frag;
								fs = subt2;
							}
							else {
								flen = displ[osptr];
								fs = subt1;
							}
							next_frag = 0;
							continue;
						}
					}
					else {
						next_frag = accum[4][next_frag];
						continue;
					}
				}
				break;
			}
		/*
		* End of Aloop
		*/

			vn2 = zzb[vn2];
		}
	}
	vatend = 0;
}

void CMultAlignment::show_pair()
{
	int i, j, dsr;
	double calc_score;

	accum.clear();
	accum.resize(5);
	for (i = 0; i < 5; ++i) {
		vector<int> row(2 * max_aln_length + 1);
		accum[i] = row;
	}

	displ = make_unique<int[]>(2 * max_aln_length + 1);
	slopes.resize(2 * max_aln_length + 1);
	diag_index = make_unique<int[]>(2 * max_aln_length + 1);

	zza = make_unique<int[]>(max_aln_length + 1);
	zzb = make_unique<int[]>(max_aln_length + 1);

	zzc = make_unique<int[]>(max_aln_length + 1);
	zzd = make_unique<int[]>(max_aln_length + 1);

	if(dnaflag) {
		ktup = dna_ktup;
		window = dna_window;
		signif = dna_signif;
		wind_gap = dna_wind_gap;
	}
	else {
		ktup = prot_ktup;
		window = prot_window;
		signif = prot_signif;
		wind_gap = prot_wind_gap;
	}

	cout << "\n\n";

	for(i = 1; i <= nseqs; ++i) {
		if(dnaflag)
			make_n_ptrs(zza.get(), zzc.get(), i, seqlen_array[i]);
		else
			make_p_ptrs(zza.get(), zzc.get(), i, seqlen_array[i]);
			for(j = i + 1; j <= nseqs; ++j) {
				if(dnaflag)
					make_n_ptrs(zzb.get(), zzd.get(),j, seqlen_array[j]);
				else
					make_p_ptrs(zzb.get(), zzd.get(),j, seqlen_array[j]);
				pair_align(i, seqlen_array[i], seqlen_array[j]);
				if(!maxsf)
					calc_score = 0.0;
				else {
					calc_score = (double) accum[0][maxsf];
					if(percent) {
						dsr = (seqlen_array[i] < seqlen_array[j]) ?
							seqlen_array[i] : seqlen_array[j];
						calc_score = (calc_score / (double) dsr) * 100.0;
					}
				}
	/*
				tmat[i][j]=calc_score;
				tmat[j][i]=calc_score;
	*/

							tmat[i][j] = (100.0 - calc_score) / 100.0;
							tmat[j][i] = (100.0 - calc_score) / 100.0;
				if(calc_score > 0.1)
					cout << "Sequences (" << i << ":" << j << ") Aligned. Score: " << calc_score;
				else
					cout << "Sequences (" << i << ":" << j << ") Not Aligned.";
			}
		}

		accum.clear();
}

void CMultAlignment::distance_matrix_output(ofstream& ofile)
{
	int i,j;

	ofile << setw(6) << last_seq-first_seq+1;
	for(i = 1;i<=last_seq - first_seq + 1; ++i) {
		ofile << "\n " << setw(max_names) << names[i];
		for(j = 1; j <= last_seq - first_seq + 1; ++j) {
			ofile << setprecision(3) << tmat[i][j];
			if(j % 8 == 0) {
				if(j != last_seq - first_seq + 1)
					ofile << "\n";
				if(j != last_seq - first_seq + 1)
					ofile << "          ";
			}
		}
	}
}

void CMultAlignment::phylogenetic_tree(wstring& phylip_name,wstring& clustal_name,wstring& dist_name)
/*
	Calculate a tree using the distances in the nseqs*nseqs array tmat.
	This is the routine for getting the REAL trees after alignment.
*/
{
	string path;
	int i, j;
	int overspill = 0;
	int total_dists;
	vector<string> standard_tree;
	string lin2;

	if(isEmpty) {
		cerr << "You must load an alignment first";
		return;
	}

	if(nseqs <= 3) {
		cerr << "Alignment has only " << nseqs << " sequences";
		return;
	}
	first_seq = 1;
	last_seq = nseqs;

	get_path(seqname, path);

	if(output_tree_clustal) {
		if (clustal_name.length() > 0) {
			if(!open_explicit_file(clustal_phy_tree_file, clustal_name))
				return;
		}
	}

	if(output_tree_phylip) {
		if (phylip_name.length() > 0) {
			if(!open_explicit_file(phylip_phy_tree_file, phylip_name))
				return;
		}
	}

	if(output_tree_distances) {
		if (dist_name.length() > 0) {
			if(!open_explicit_file(distances_phy_tree_file, dist_name))
				return;
		}
	}

	boot_positions = make_unique<int[]>(seqlen_array[first_seq]+2);

	for(j = 1; j <= seqlen_array[first_seq]; ++j)
		boot_positions[j] = j;

	if(output_tree_clustal) {
		verbose = true;	  /* Turn on file output */
		if(dnaflag)
			overspill = dna_distance_matrix(clustal_phy_tree_file);
		else
			overspill = prot_distance_matrix(clustal_phy_tree_file);
	}

	if(output_tree_phylip) {
		verbose = false;	  /* Turn off file output */
		if(dnaflag)
			overspill = dna_distance_matrix(phylip_phy_tree_file);
		else
			overspill = prot_distance_matrix(phylip_phy_tree_file);
	}

	if(output_tree_distances) {
		verbose = false;	  /* Turn off file output */
		if(dnaflag)
			overspill = dna_distance_matrix(distances_phy_tree_file);
		else
			overspill = prot_distance_matrix(distances_phy_tree_file);
			distance_matrix_output(distances_phy_tree_file);
	}

/* check if any distances overflowed the distance corrections */
	if (overspill > 0) {
		total_dists = (nseqs * (nseqs - 1)) / 2;
	}

	if(output_tree_clustal)
		verbose = true;	  /* Turn on file output */

	standard_tree.clear();
	standard_tree.resize(nseqs + 1);
	for(i = 0; i < nseqs + 1; ++i) {
		string row;
		row.resize(nseqs + 1);
		standard_tree[i] = row;
	}

	if(output_tree_clustal || output_tree_phylip)
		nj_tree(standard_tree, clustal_phy_tree_file);

	if(output_tree_phylip)
		print_phylip_tree(standard_tree, phylip_phy_tree_file, 0);

/*
	print_tree(standard_tree,phy_tree_file);
*/
	tree_gaps.clear();
	standard_tree.clear();

	if(output_tree_clustal) {
		clustal_phy_tree_file.close();
		cout << "Phylogenetic tree file created:   [" << clustal_name.c_str() << "]";
}

	if(output_tree_phylip) {
		phylip_phy_tree_file.close();
		cout << "Phylogenetic tree file created:   [" << phylip_name.c_str() << "]";
	}

	if(output_tree_distances) {
		distances_phy_tree_file.close();
		cout << "Distance matrix  file  created:   [" << dist_name.c_str() << "]";
	}
}

void CMultAlignment::compare_tree(vector<string>& tree1, vector<string>& tree2, int *hits, int n)
{
	int i, j, k;
	int nhits1, nhits2;

	for(i = 1; i <= n - 3; ++i) {
		for(j = 1; j <= n - 3; ++j)  {
			nhits1 = 0;
			nhits2 = 0;
			for(k = 1; k <= n; k++) {
				if(tree1[i][k] == tree2[j][k])
					nhits1++;
				if(tree1[i][k] != tree2[j][k])
					nhits2++;
			}
			if((nhits1 == last_seq-first_seq + 1) || (nhits2 == last_seq-first_seq + 1))
				hits[i]++;
		}
	}
}

void CMultAlignment::print_tree(vector<string>& tree_description, ofstream& tree, int *totals)
{
	int row, col;

	tree << "\n";

	for(row = 1; row <= last_seq - first_seq - 2; row++)  {
		tree << " \n";
		for(col = 1; col <= last_seq - first_seq + 1; col++) {
			if(tree_description[row][col] == 0)
				tree << '*';
			else
				tree << '.';
		}
		if(totals[row] > 0)
			tree << setw(7) << totals[row];
	}
	tree << " \n";
	for(col = 1; col <= last_seq-first_seq + 1; col++)
		tree << setw(1) << tree_description[last_seq - first_seq - 1][col];
	tree << "\n";
}

unsigned long CMultAlignment::addrand(unsigned long r)
{
	const size_t m = 100000000;
	const size_t m1 = 10000;
	unsigned long j = rand();
	unsigned long a[55];

	j = (j + 1) % 55;

	int x = (j + 23) % 55;
	int y = (j + 54) % 55;
	a[j] = (a[x] + a[y]) % m;

	return ((a[j] / m1) * r) / m1;
}

void CMultAlignment::bootstrap_tree(wstring& phylip_name, wstring& clustal_name)
{
	int i,j;
	int ranno;
	string dummy, path;
	vector<string> sample_tree, standard_tree;
	int total_dists, overspill = 0, total_overspill = 0;
	int nfails = 0;

	if(isEmpty) {
		cerr << "You must load an alignment first";
		return;
	}

	if(nseqs <= 3) {
		cerr << "Alignment has only " << nseqs << " sequences";
		return;
	}

	if(!output_tree_clustal && !output_tree_phylip) {
		cerr << "You must select either clustal or phylip tree output format";
		return;
	}
	get_path(seqname, path);

	if (output_tree_clustal) {
		if (clustal_name.length() > 0) {
			if(!open_explicit_file(clustal_phy_tree_file, clustal_name))
				return;
		}
	}

	first_seq = 1;
	last_seq = nseqs;

	if (output_tree_phylip) {
		if (phylip_name.length() > 0) {
			if(!open_explicit_file(phylip_phy_tree_file, phylip_name))
				return;
		}
	}

	boot_totals = make_unique<int[]>(nseqs + 1);

	boot_positions = make_unique<int[]>(seqlen_array[first_seq] + 2);

	for(j = 1; j <= seqlen_array[first_seq]; ++j)  /* First select all positions for */
		boot_positions[j] = j;		/* the "standard" tree */

	if(output_tree_clustal) {
		verbose = true;	  /* Turn on file output */
		if(dnaflag)
			overspill = dna_distance_matrix(clustal_phy_tree_file);
		else
			overspill = prot_distance_matrix(clustal_phy_tree_file);
	}

	if(output_tree_phylip) {
		verbose = false;	  /* Turn off file output */
		if(dnaflag)
			overspill = dna_distance_matrix(phylip_phy_tree_file);
		else
			overspill = prot_distance_matrix(phylip_phy_tree_file);
	}

/* check if any distances overflowed the distance corrections */
	if (overspill > 0) {
		total_dists = (nseqs * (nseqs - 1)) / 2;
	}

	tree_gaps.clear();

	if (output_tree_clustal)
		verbose = true;	/* Turn on screen output */

	standard_tree.clear();
	standard_tree.resize(nseqs + 1);
	for(i = 0; i < nseqs + 1; ++i) {
		string row;
		row.resize(nseqs + 1);
		standard_tree[i] = row;
	}

/* compute the standard tree */

	if(output_tree_clustal || output_tree_phylip)
		nj_tree(standard_tree, clustal_phy_tree_file);

	if (output_tree_clustal)
		clustal_phy_tree_file << "\n\n\t\t\tBootstrap Confidence Limits\n\n";

/* save the left_branch and right_branch for phylip output */
	save_left_branch = make_unique<double[]>(nseqs + 2);
	save_right_branch = make_unique<double[]>(nseqs + 2);
	for (i = 1; i <= nseqs;i++) {
		save_left_branch[i] = left_branch[i];
		save_right_branch[i] = right_branch[i];
	}
/*
  The next line is a fossil from the days of using the cc ran()
	ran_factor = RAND_MAX / seqlen_array[first_seq];
*/

/* do not use the native cc ran()
	srand(boot_ran_seed);
*/
	addrandinit((unsigned long) boot_ran_seed);

	if (output_tree_clustal)
		clustal_phy_tree_file << "\n Random number generator seed = " << setw(7) << boot_ran_seed << '\n';

	if (output_tree_clustal) {
		clustal_phy_tree_file << "\n Number of bootstrap trials	= " << setw(7) << boot_ntrials << '\n';

		clustal_phy_tree_file << "\n\n Diagrammatic representation of the above tree: \n";
		clustal_phy_tree_file << "\n Each row represents 1 tree cycle;";
		clustal_phy_tree_file << " defining 2 groups.\n";
		clustal_phy_tree_file << "\n Each column is 1 sequence; ";
		clustal_phy_tree_file << "the stars in each line show 1 group; ";
		clustal_phy_tree_file << "\n the dots show the other\n";
		clustal_phy_tree_file << "\n Numbers show occurences in bootstrap samples.";
	}
/*
	print_tree(standard_tree, clustal_phy_tree_file, boot_totals);
*/
	verbose = false;						 /* Turn OFF screen output */

	sample_tree.clear();
	sample_tree.resize(nseqs + 1);
	for(i = 0; i < nseqs + 1; ++i) {
		string row;
		row.resize(nseqs + 1);
		sample_tree[i] = row;
	}

	total_overspill = 0;
	nfails = 0;
	for(i = 1; i <= boot_ntrials; ++i) {
		for(j = 1; j <= seqlen_array[first_seq]; ++j) { /* select alignment */
								/* positions for */
			ranno = addrand( (unsigned long) seqlen_array[1]) + 1;
			boot_positions[j] = ranno; 		 /* bootstrap sample */
		}
		if(output_tree_clustal) {
			if(dnaflag)
				overspill = dna_distance_matrix(clustal_phy_tree_file);
			else
				overspill = prot_distance_matrix(clustal_phy_tree_file);
		}

		if(output_tree_phylip) {
			if(dnaflag)
				overspill = dna_distance_matrix(phylip_phy_tree_file);
			else
				overspill = prot_distance_matrix(phylip_phy_tree_file);
		}

		if( overspill > 0) {
			total_overspill = total_overspill + overspill;
			nfails++;
		}

		tree_gaps.clear();

		if(output_tree_clustal || output_tree_phylip)
			nj_tree(sample_tree, clustal_phy_tree_file);

		compare_tree(standard_tree, sample_tree, boot_totals.get(), last_seq - first_seq + 1);
	}

/* check if any distances overflowed the distance corrections */
	if (nfails > 0) {
		total_dists = (nseqs * (nseqs - 1)) / 2;
	}

	sample_tree.clear();
/*
	fprintf(clustal_phy_tree_file,"\n\n Bootstrap totals for each group\n");
*/
	if (output_tree_clustal)
		print_tree(standard_tree, clustal_phy_tree_file, boot_totals.get());

	if(output_tree_phylip) {
		left_branch = make_unique<double[]>(nseqs + 2);
		right_branch = make_unique<double[]>(nseqs + 2);
		for (i = 1; i <= nseqs; ++i) {
			left_branch[i] = save_left_branch[i];
			right_branch[i] = save_right_branch[i];
		}
		print_phylip_tree(standard_tree, phylip_phy_tree_file, bootstrap_format);
	}

	standard_tree.clear();

	if (output_tree_clustal)
		clustal_phy_tree_file.close();

	if (output_tree_phylip)
		phylip_phy_tree_file.close();

	if (output_tree_clustal)
		cout << "Bootstrap output file completed       [" << clustal_name.c_str() << "]";
	if (output_tree_phylip)
		cout << "Bootstrap output file completed       [" << phylip_name.c_str() << "]";
}

void CMultAlignment::tree_gap_delete()	/* flag all positions in alignment that have a gap */
{			  /* in ANY sequence */
	tree_gaps.clear();
	tree_gaps.resize(max_aln_length + 1);

	for(int posn = 1; posn <= seqlen_array[first_seq]; ++posn) {
		tree_gaps[posn] = 0;
		for(int seqn = 1; seqn <= last_seq - first_seq + 1; ++seqn)  {
			if((seq_array[seqn + first_seq - 1][posn] == gap_pos1) ||
				(seq_array[seqn + first_seq - 1][posn] == gap_pos2)) {
				tree_gaps[posn] = 1;
				break;
			}
		}
	}
}

bool CMultAlignment::transition(int base1, int base2) /* TRUE if transition; else FALSE */
/*

	assumes that the bases of DNA sequences have been translated as
	a,A = 0;	c,C = 1;	g,G = 2;	t,T,u,U = 3;  N = 4;

	A <--> G  and  T <--> C  are transitions;  all others are transversions.

*/
{
	if((base1 == 0 && base2 == 2) || (base1 == 2 && base2 == 0))
		return true;												 /* A <--> G */
	if((base1 == 3 && base2 == 1) || (base1 == 1 && base2 == 3))
		return true;												 /* T <--> C */
	return false;
}

int CMultAlignment::dna_distance_matrix(ofstream& tree)
{
	int m, n;
	int j, i;
	int res1, res2;
	int overspill = 0;
	double p, q, e, a, b, k;

	tree_gap_delete();  /* flag positions with gaps (tree_gaps[i] = 1 ) */

	if(verbose) {
		tree << "\n";
		tree << "\n DIST   = percentage divergence (/100)";
		tree << "\n p      = rate of transition (A <-> G; C <-> T)";
		tree << "\n q      = rate of transversion";
		tree << "\n Length = number of sites used in comparison";
		tree << "\n";
		if(tossgaps) {
			tree << "\n All sites with gaps (in any sequence) deleted!";
			tree << "\n";
		}
		if(kimura) {
			tree << "\n Distances corrected by Kimura's 2 parameter model:";
			tree << "\n\n Kimura, M. (1980)";
			tree << " A simple method for estimating evolutionary ";
			tree << "rates of base";
			tree << "\n substitutions through comparative studies of ";
			tree << "nucleotide sequences.";
			tree << "\n J. Mol. Evol., 16, 111-120.";
			tree << "\n\n";
		}
	}

	for(m = 1; m < last_seq - first_seq + 1; ++m)	  /* for every pair of sequence */
	for(n = m + 1; n <= last_seq - first_seq + 1; ++n) {
		p = q = e = 0.0;
		tmat[m][n] = tmat[n][m] = 0.0;
		for(i = 1; i <= seqlen_array[first_seq]; ++i) {
			j = boot_positions[i];
						if(tossgaps && (tree_gaps[j] > 0) )
				goto skip;			 /* gap position */
			res1 = seq_array[m + first_seq - 1][j];
			res2 = seq_array[n + first_seq - 1][j];
			if(res1 == gap_pos1 || res1 == gap_pos2 ||
				res2 == gap_pos1 || res2 == gap_pos2)
				goto skip;			 /* gap in a seq*/
			e += 1.0;
			if(res1 != res2) {
				if(transition(res1, res2))
					p += 1.0;
				else
					q += 1.0;
			}
			skip:;
		}


	/* Kimura's 2 parameter correction for multiple substitutions */

		if(!kimura) {
			if (e == 0) {
				cout << "\n WARNING: sequences " << m << " and " << n << " are non-overlapping\n";
				k = 0.0;
				p = 0.0;
				q = 0.0;
			}
			else {
				k = (p + q) / e;
				if(p > 0.0)
					p /= e;
				else
					p = 0.0;
				if(q > 0.0)
					q /= e;
				else
					q = 0.0;
			}
			tmat[m][n] = tmat[n][m] = k;
			if(verbose) {						/* if screen output */
				tree << setw(4) << m << " vs" << setw(4) << n << ":  DIST = ";
				tree << setprecision(4) << k << "; p = " << setprecision(4) << p;
				tree << "; q = "<< setprecision(4) << q << "; length = " << setprecision(0) << e << '\n';
			}
		}
		else {
			if (e == 0) {
				cout << "\n WARNING: sequences " << m << " and " << n << " are non-overlapping\n";
				p = 0.0;
				q = 0.0;
			}
			else {
				if(p > 0.0)
					p /= e;
				else
					p = 0.0;
				if(q > 0.0)
					q /= e;
				else
					q = 0.0;
			}

			if((2.0 * p + q) == 1.0)
				a = 0.0;
			else
				a = 1.0 / (1.0 - (2.0 * p) - q);

			if(q == 0.5)
				b = 0.0;
			else
				b = 1.0 / (1.0 - (2.0 * q));

/* watch for values going off the scale for the correction. */
			if( (a <= 0.0) || (b <= 0.0) ) {
				overspill++;
				k = 3.5;  /* arbitrary high score */
			}
			else
				k = 0.5 * log(a) + 0.25 * log(b);
			tmat[m][n] = tmat[n][m] = k;
			if(verbose) {							/* if screen output */
				tree << setw(4) << m << " vs" << setw(4) << n << ":  DIST = ";
				tree << setprecision(4) << k << "; p = " << setprecision(4) << p;
				tree << "; q = "<< setprecision(4) << q << "; length = " << setprecision(0) << e << '\n';
			}

		}
	}
	return overspill;	/* return the number of off-scale values */
}

int CMultAlignment::prot_distance_matrix(ofstream& tree)
{
	int m, n;
	int j, i;
	int res1, res2;
	int overspill = 0;
	double p, e, k, table_entry;


	tree_gap_delete();  /* flag positions with gaps (tree_gaps[i] = 1 ) */

	if(verbose) {
		tree << "\n";
		tree << "\n DIST   = percentage divergence (/100)";
		tree << "\n Length = number of sites used in comparison";
		tree << "\n\n";
		if(tossgaps) {
			tree << "\n All sites with gaps (in any sequence) deleted";
			tree << "\n";
		}
		if(kimura) {
			tree << "\n Distances up tp 0.75 corrected by Kimura's empirical method:";
			tree << "\n\n Kimura, M. (1983)";
			tree << " The Neutral Theory of Molecular Evolution.";
			tree << "\n Page 75. Cambridge University Press, Cambridge, England.";
			tree << "\n\n";
		}
	}

	for(m = 1; m < nseqs; ++m)	  /* for every pair of sequence */
		for(n = m + 1; n <= nseqs; ++n) {
			p = e = 0.0;
			tmat[m][n] = tmat[n][m] = 0.0;
			for(i = 1; i <= seqlen_array[1]; ++i) {
				j = boot_positions[i];
				if(tossgaps && (tree_gaps[j] > 0) )
					goto skip; /* gap position */
				res1 = seq_array[m][j];
				res2 = seq_array[n][j];
				if( (res1 == gap_pos1) || (res1 == gap_pos2) ||
								(res2 == gap_pos1) || (res2 == gap_pos2))
										goto skip;	/* gap in a seq*/
				e += 1.0;
				if(res1 != res2)
					p += 1.0;
					skip:;
			}

			if(p <= 0.0)
				k = 0.0;
			else
				k = p / e;


			if(kimura) {
				if(k < 0.75) { /* use Kimura's formula */
					if(k > 0.0)
						k = -log(1.0 - k - (k * k / 5.0));
				}
				else {
					if(k > 0.930) {
						overspill++;
						k = 10.0; /* arbitrarily set to 1000% */
					}
					else {
						table_entry = (k * 1000.0) - 750.0;
						k = (double) dayhoff_pams[(int)table_entry];
						k /= 100.0;
					}
				}
			}

			tmat[m][n] = tmat[n][m] = k;
			if(verbose) {						 /* if screen output */
				tree << setw(4) << m << " vs" << setw(4) << n << ":  DIST = ";
				tree << setprecision(4) << k << ";  length = " << setprecision(0) << e << '\n';
			}
	}
	return overspill;
}

void CMultAlignment::guide_tree(int firstseq, int numseqs)
/*
	Routine for producing unrooted NJ trees from seperately aligned
	pairwise distances.  This produces the GUIDE DENDROGRAMS in
	PHYLIP format.
*/
{
	vector<string> standard_tree;
	int i;

	verbose = false;
	first_seq = firstseq;
	last_seq = first_seq + numseqs - 1;

	standard_tree.resize(last_seq - first_seq + 2);
	for(i = 0; i < last_seq - first_seq + 2; ++i) {
		string row;
		row.resize(last_seq - first_seq + 2);
		standard_tree[i] = row;
	}

	nj_tree(standard_tree, clustal_phy_tree_file);

	print_phylip_tree(standard_tree, phylip_phy_tree_file, 0);

	standard_tree.clear();
}

void CMultAlignment::free_aln(int nseqs)
{
	if(nseqs <= 0)
		return;

	seq_array.clear();
	names.clear();
	titles.clear();
	tmat.clear();
}

void CMultAlignment::alloc_seq(int seq_no, int length)
{
	seq_array[seq_no].clear();
	seq_array[seq_no].resize(length + 2);
}

void CMultAlignment::realloc_seq(int seq_no, int length)
{
	seq_array[seq_no].resize(length + 2);
}

void CMultAlignment::free_seq(int seq_no)
{
	seq_array[seq_no].clear();
}

string& CMultAlignment::blank_to_(string& str)
{
	for(int i = 0; i < str.length(); ++i)
		if(str[i] == ' ' ||
			str[i] == ';' ||
			str[i] == ',' ||
			str[i] == '(' ||
			str[i] == ')' ||
			str[i] == ':'
		)
			str[i] = '_';

	return str;
}

bool CMultAlignment::blankline(string& line)
{
	for(int i = 0; line[i] != '\n' && i < line.length(); ++i) {
		if(isdigit(line[i]) || isspace(line[i]) ||
				line[i] == '*' || line[i] == ':' ||
				line[i] == '.')
				continue;
			return false;
		}
	return true;
}
