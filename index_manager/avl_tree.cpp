
#include "pch.h"
#include "framework.h"
#include <bits/bits-stdc++.h>
#include "avl_tree.h"

using namespace std;

string AvlTreeNameSpace::convertToBinary(int num)
{
    string res = "";
    for (int i = 0; i <= 30; i++)
    {
        res += to_string(min(num & (1 << i), 1));
    }
    return res;
}

int AvlTreeNameSpace::binaryToInt(string binary)
{
    int num = 0;
    for (int i = 0; i < binary.size(); i++) {
        num += (binary[i] - '0') * (1 << i);
    }
    return num;
}
