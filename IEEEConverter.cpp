/*
* CS-3010 - Assignment 1
* Myo Aung
*/

#include <iostream>
#include <iomanip>

using namespace std;

class mantissa_s
{
 public:
    double actual;
    bool sign;
    char exponent;
    bool denormalized;
    int mantissa_bit;
};

class standardNum
{
 public:
    double num;
    int exponent;
};

void getNormalizeValue(double num, standardNum *sNa)
{
    sNa->num = num;
    sNa->exponent = 0;

    while (1)
    {
        if ((int)sNa->num == 1)
            break;
        else
        {
            if (sNa->num < 1)
            {
                sNa->num *= 2;
                sNa->exponent--;
            }
            else if (sNa->num > 1)
            {
                sNa->num /= 2;
                sNa->exponent++;
            }
        }
    }
}

void getBitfrom32BitFloating(double num, mantissa_s *mantissa)
{
    
    mantissa->actual = num;                                                         // store actual double value into struct
    mantissa->sign = !(num > -1);                                                   // if positive, then 0, if negative, then 1
    if (num < 0)                                                                    
        num = -num;

    standardNum sN;
    getNormalizeValue(num, &sN);

    // bias is the maximum value for signed bit representation
    unsigned char bias = ~0;                                                        // for signed 32 bit, maximum number is 127
    bias /= 2;
    mantissa->exponent = bias + sN.exponent;                                        // exponent is bias + loop_mormalize

    // to store if there any denormalized number
    mantissa->denormalized = mantissa->exponent != 0 ? 1 : 0;                       // 1 for normalize, 0 for denormalized
    double t_mantissa = sN.num - (int)sN.num;                                       // temp for mantissa
    char shift_value = 22;  
    mantissa->mantissa_bit = 0;                                                     // initialized
    for (int i = 0; i < 23; i++)                                                    // get mantissa
    {
        t_mantissa *= 2;
        if ((int)t_mantissa)
            mantissa->mantissa_bit = mantissa->mantissa_bit | 1 << shift_value;

        t_mantissa -= (int)t_mantissa;
        shift_value--;
    }
}

char *num2BinCharStr(int dNum, int representNum)
{
    char *store = new char[representNum + 1];
    store[representNum] = '\0';
    for (int i = 0; i < representNum; i++)
    {
        store[i] = (dNum & ((uint32_t)(1 << (representNum - 1)) >> i)) ? '1' : '0'; // iterate bit using reverse technique
    }
    return store;                                                                   // return dynamic memory pointer
}

int main()
{
    float set_ur_number_here;
    cout << "Enter a decimal number: ";
    cin >> set_ur_number_here;
    mantissa_s mantissa_struct;
    getBitfrom32BitFloating(set_ur_number_here, &mantissa_struct);

    char *exponentBit = num2BinCharStr(mantissa_struct.exponent, 8);
    char *mantissaBit = num2BinCharStr(mantissa_struct.mantissa_bit, 23);

    cout << "Actual Number: " << setprecision(32) << mantissa_struct.actual << endl;
    cout << "Sign: "; 
    if(mantissa_struct.sign == 1) {
        cout << "Negative" << endl;
    } else {
        cout << "Positive" << endl;
    }
    cout << "Exponent: " << exponentBit << endl;

    cout << "Type: ";
    if(mantissa_struct.denormalized == 1) {
        cout << "Normalize" << endl;
    } else {
        cout << "Denormalize" << endl;
    }
    
    cout << "Mantissa: " << mantissaBit << endl;
    cout << "Bit: " << mantissa_struct.sign << "|" << exponentBit << "|" << mantissaBit << endl;
    
    free(exponentBit);              // clean heap
    free(mantissaBit);              // clean heap

    return 0;
}