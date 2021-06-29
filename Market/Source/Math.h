/**
* Title: Market
* Author: Tonia Sanzo
* Date: 6/28/21
*
* Class with different math functions
*/
#pragma once
#include "PCH.h"


using namespace MATH
{
    int getGCD(int a, int b)
    {
        int r = a % b;
        if (!r)
        {
            return b;
        }
        getGCD(b, r);
    }
}