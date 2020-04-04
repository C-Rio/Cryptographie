#ifndef PERMUTATION_H
#define PERMUTATION_H 

#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;

const int P10[11] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8[9] = {6, 3, 7, 4, 8, 5, 10, 9};
const int IP[9] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_R[9] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[9] = {4, 1, 2, 3, 2, 3, 4, 1};
const int P4[5] = {2, 4, 3, 1};

const int S0[17] = {1, 0, 3, 2,
					3, 2, 1, 0,
					0, 2, 1, 3,
					3, 1, 3, 2};

const int S1[17] = {0, 1, 2, 3,
					2, 0, 1, 3,
					3, 0, 1, 0,
					2, 1, 0, 3};

class permutation
{
  public:
	bitset<10> do_P10(bitset<10>);
	bitset<8> do_P8(bitset<10>);
	bitset<4> do_P4(bitset<4>);
	bitset<8> do_IP(bitset<8>);
	bitset<8> do_IP_R(bitset<8>);
	bitset<8> do_EP(bitset<4>);
	void circular_left_shift(bitset<10> &);
};

#endif