#include <iostream>
#include <sstream>
#include<vector>
#include <algorithm>

class LongInt 
{
    public:
        std::vector <int> digits;
        int size;

        LongInt(std::vector <int> localDigits)
        {
            digits = localDigits;
            size = digits.size();
        }

        std::string toString()
        {
            std::string result = "";
            for(int i = 0; i < size; i++)
            {
                result.append(std::to_string (digits[i]));
            }
            return result;
        }

        long long int toInt()
        {
            std::string result = "";
            for(int i = 0; i < size; i++)
            {
                result.append(std::to_string (digits[i]));
            }

            char* endptr = NULL;
            return std::strtoll(result.c_str(), &endptr, 10);
        }

};


LongInt summation(LongInt max, LongInt min)
{
    std::vector <int> sum;
    int carry = 0;

    if(max.size < min.size)
    {
        std::swap(max, min);
    }

    for(int i = 0; i < min.size; i++)
    {
        int currentSum = min.digits[min.size - 1 - i] + max.digits[max.size - 1 - i] + carry;
        carry = 0;
        //if currentsum is greater than 9 currentsum - 10, carry = 1
        if(currentSum > 9)
        {
            currentSum = currentSum - 10;
            carry = 1;
        }
        sum.push_back(currentSum);
    }

    //from the first index min does not exist, min[min.size] to the last index that max exists max[max.size - 1]
    for(int i = min.size; i < max.size; i++)
    {
        int currentSum = max.digits[max.size - 1 - i] + carry;
        carry = 0;

        if(currentSum > 9)
        {
            currentSum = currentSum - 10;
            carry = 1;
        }
        sum.push_back(currentSum);
    }

    if(carry != 0)
    {
        sum.push_back(carry);
    }

    //answer is reversed
    reverse(sum.begin(), sum.end());
    LongInt returnValue(sum);
    return returnValue;
}

LongInt naiveMultiply(LongInt min, LongInt max)
{
    //add larger number to 0 smaller number of times, i.e. 3*256 is 0 + 256 + 256 + 256
    std::vector <int> product;
    product.push_back(0);
    LongInt returnValue(product);

    if(max.size < min.size)
    {
        std::swap(max, min);
    }

    if(min.size == 1)
    {
        if(min.digits[0] == 0 || max.digits[0] == 0)
        {
            return returnValue;
        }
    }

    for(int i = 0; i < min.size; i++)
    {
        //add trailing zeros
        std::vector <int> currentVector;
        currentVector.push_back(0);
        LongInt currentSum(currentVector);
        int currentValue = min.digits[min.size - 1 - i];
        for(int j = 0; j < currentValue; j++)
        {
            currentSum = summation(currentSum, max);
        }
        for(int j = 0; j < i; j++)
        {
            std::vector <int> currentVector = currentSum.digits;
            currentVector.push_back(0);
            LongInt placeHolder(currentVector);
            currentSum = placeHolder;
        }

        returnValue = summation(returnValue, currentSum);
    }

    return returnValue;
}

LongInt subtraction(LongInt max, LongInt min)
{
    /*
    for each number right to left
    difference = max[i] - min[i]
    if difference < 0
        max[i-1]--
        differnce += 10
    Result.digit[i] = difference
    */

    LongInt maxCopy = max;
    std::vector <int> resultVector;

    for(int i = 0; i < min.size; i++)
    {
        int difference = maxCopy.digits[maxCopy.size - 1 - i] - min.digits[min.size - 1 - i];
        if(difference < 0)
        {
            maxCopy.digits[maxCopy.size - i - 2] = maxCopy.digits[maxCopy.size - i - 2] - 1;
            difference += 10;
        }

        resultVector.push_back(difference);
    }

    for(int i = min.size; i < max.size; i++)
    {
        int difference = maxCopy.digits[maxCopy.size - 1 - i];
//        std::cout << std::to_string(maxCopy.digits[maxCopy.size - 1 - i]) << std::endl;

        if(difference < 0)
        {
            maxCopy.digits[maxCopy.size - i - 2] = maxCopy.digits[maxCopy.size - i - 2] - 1;
            difference += 10;
        }
        resultVector.push_back(difference);
    }

    reverse(resultVector.begin(), resultVector.end());
    LongInt returnValue(resultVector);
    return returnValue;
}

LongInt split(LongInt input, int start, int end)
{
    std::vector <int> resultVector;

    for(int i = start; i < end; i++)
    {
        resultVector.push_back(input.digits[i]);
    }

    LongInt returnValue(resultVector);
    return returnValue;
}

std::vector <int> toVector(long long int input)
{
    //convert to string
    //iterate through string char by char, convert to int and push to vector
    //return vector

    std::vector <int> resultVector;
    std::string stringified = std::to_string(input);

    for(int i = 0; i < stringified.length(); i++)
    {
        int currentInt = stringified[i] - '0';

        resultVector.push_back(currentInt);
    }
    return resultVector;
}

LongInt makeEven(LongInt input)
{
    if(input.size % 2 != 0)
    {
        //add leading 0
        std::vector <int> placeholder = input.digits;
        placeholder.insert(placeholder.begin(), 0);
        LongInt returnValue(placeholder);
        return returnValue;
    }
    else
    {
        return input;
    }
}

LongInt increaseLength(LongInt input, int size)
{
    std::vector <int> placeholder = input.digits;

    for(int i = 0; i < size; i++)
    {
        placeholder.insert(placeholder.begin(), 0);
    }

    LongInt returnValue(placeholder);
    return returnValue;
}

LongInt karatsubaMultiply(LongInt min, LongInt max)
{
//    std::cout << "size " << std::to_string(min.size) << std::endl;

    if(max.size < min.size)
    {
        std::swap(max, min);
    }

    max = makeEven(max);
    min = increaseLength(min, max.size - min.size);
    //base case is less then 6 digits, use long long int multiplication
    if(min.size < 6 && max.size < 6)
    {
//        std::cout << "base case" << std::endl;

        long long int currentResult = min.toInt() * max.toInt();
        //turn into LongInt
        std::vector <int> resultVector = toVector(currentResult);
        LongInt longIntCurrentResult(resultVector);
        return longIntCurrentResult;
    }

    //find split by dividing smaller number in two and rounding down
    //split each digit sequence at point
    //identify z0, z1, z2
    //recursively solve each and combine

    int splitValue = min.size / 2;

    LongInt minLow = split(min, splitValue, min.size);
    LongInt minHigh = split(min, 0, splitValue);
    LongInt maxLow = split(max, splitValue, max.size);
    LongInt maxHigh = split(max, 0, splitValue);

    LongInt z0 = karatsubaMultiply(minLow, maxLow);
    LongInt z1 = karatsubaMultiply(summation(minLow, minHigh), summation(maxLow, maxHigh));
    LongInt z2 = karatsubaMultiply(minHigh, maxHigh);
    LongInt z3 = z2;
    LongInt z4 = subtraction(subtraction(z1, z2), z0);

//    std::cout << z3.toString() << std::endl;

    std::vector <int> z3Vector = z3.digits;
    for(int i = 0; i < splitValue * 2; i++)
    {
        z3Vector.push_back(0);
    }

    LongInt placeHolder(z3Vector);
    z3 = placeHolder;

//    std::cout << z3.toString() << std::endl;

    std::vector <int> z4Vector = z4.digits;

    for(int i = 0; i < splitValue; i++)
    {
        z4Vector.push_back(0);
    }

    LongInt placeHolderz4(z4Vector);
    z4 = placeHolderz4;

    LongInt result = summation(z3, z4);

    result = summation(result, z0);

    return result;
}

std::vector <int> stringToVector(std::string stringified)
{
    //convert to string
    //iterate through string char by char, convert to int and push to vector
    //return vector

    std::vector <int> resultVector;

    for(int i = 0; i < stringified.length(); i++)
    {
        int currentInt = stringified[i] - '0';

        resultVector.push_back(currentInt);
    }
    return resultVector;
}

int main()
{
    //take input from keyboard
    std::vector <int> valueOneVector;
    std::vector <int> valueTwoVector;
    std::string inputOne;
    std::string inputTwo;

    std::cout << "please enter input one" << std::endl;
    std::cin >> inputOne;

    std::cout << "please enter input two" << std::endl;
    std::cin >> inputTwo;

    valueOneVector = stringToVector(inputOne);
    valueTwoVector = stringToVector(inputTwo);
    LongInt valueOne(valueOneVector);
    LongInt valueTwo(valueTwoVector);

    std::cout << "using naive multiplication " << std::endl << naiveMultiply(valueOne, valueTwo).toString() << std::endl;
    std::cout << "using karatsuba multiplication " << std::endl << karatsubaMultiply(valueOne, valueTwo).toString() << std::endl;
//    int splitValue = valueOne.size / 2;
//    std::cout << split(valueOne, 0, splitValue).toString() << std::endl;
//    std::cout << split(valueOne, splitValue, valueOne.size).toString() << std::endl;

    return 0;
}
