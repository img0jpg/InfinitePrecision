#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <math.h>

using namespace std;

class InfinitePrecision
{
private:
    vector<int> digits;

public:
    //constructors
    InfinitePrecision() {
        digits.insert(digits.begin(), 1);
    }
    InfinitePrecision(int value)
    {
        while (value != 0)
        {
            digits.insert(digits.begin(), value % 10);
            value /= 10;
        }
    }
    InfinitePrecision(long long num) {
        while (num > 0) {
            digits.push_back(num % 10);
            num /= 10;
        }
        if (digits.empty()) digits.push_back(0);
    }
    InfinitePrecision(const string& numberStr) {
    // Add the digits to the vector
    for (char c : numberStr) {
        digits.push_back(c - '0');
    }

    // Remove any leading zeros
    while (digits.size() > 1 && digits[0] == 0) {
        digits.erase(digits.begin());
    }
}

    // Method to add another InfinitePrecision object to this one
    void add(const InfinitePrecision &other)
    {
        int carry = 0;
        int i = digits.size() - 1;
        int j = other.digits.size() - 1;

        while (i >= 0 || j >= 0 || carry != 0)
        {
            int sum = carry;
            if (i >= 0)
            {
                sum += digits[i];
                i--;
            }
            if (j >= 0)
            {
                sum += other.digits[j];
                j--;
            }
            digits.insert(digits.begin(), sum % 10);
            carry = sum / 10;
        }
    }
    void subtract(const InfinitePrecision &other)
    {
        int borrow = 0;
        int i = digits.size() - 1;
        int j = other.digits.size() - 1;

        while (i >= 0 || j >= 0)
        {
            int diff = borrow;
            if (i >= 0)
            {
                diff += digits[i];
                i--;
            }
            if (j >= 0)
            {
                diff -= other.digits[j];
                j--;
            }
            if (diff < 0)
            {
                diff += 10;
                borrow = -1;
            }
            else
            {
                borrow = 0;
            }
            digits.insert(digits.begin(), diff);
        }

        // Remove any leading zeros
        while (digits.size() > 1 && digits[0] == 0)
        {
            digits.erase(digits.begin());
        }
    }
    void multiply(const InfinitePrecision &other)
    {
        vector<int> result(digits.size() + other.digits.size() - 1, 0);
        int carry = 0;

        for (int i = digits.size() - 1; i >= 0; i--)
        {
            carry = 0;
            for (int j = other.digits.size() - 1; j >= 0; j--)
            {
                int product = digits[i] * other.digits[j] + carry + result[i + j + 1];
                result[i + j + 1] = product % 10;
                carry = product / 10;
            }
            result[i] += carry;
        }

        // Remove any leading zeros in the result
        while (result.size() > 1 && result[0] == 0)
        {
            result.erase(result.begin());
        }

        digits = result;
    }
    InfinitePrecision divide(const InfinitePrecision &divisor) const
    {
        if (divisor.digits.size() == 1 && divisor.digits[0] == 0)
        {
            throw invalid_argument("division by zero");
        }
        InfinitePrecision quotient(0);
        InfinitePrecision remainder(0);

        for (int i = digits.size() - 1; i >= 0; i--)
        {
            remainder.digits.insert(remainder.digits.begin(), digits[i]);
            int quotientDigit = 0;
            while (remainder.isGreaterOrEqual(divisor))
            {
                remainder.subtract(divisor);
                quotientDigit++;
            }
            quotient.digits.insert(quotient.digits.begin(), quotientDigit);
        }

        // Remove any leading zeros in the quotient
        while (quotient.digits.size() > 1 && quotient.digits[0] == 0)
        {
            quotient.digits.erase(quotient.digits.begin());
        }

        return quotient;
    }

    void modulo(const InfinitePrecision &other)
    {
        InfinitePrecision quotient(0);
        InfinitePrecision remainder(0);

        for (int i = digits.size() - 1; i >= 0; i--)
        {
            remainder.multiply(10);
            remainder.add(digits[i]);
            int x = 0;
            while (other.isLessOrEqual(remainder))
            {
                remainder.subtract(other);
                x++;
            }
            quotient.multiply(10);
            quotient.add(x);
        }

        digits = remainder.digits;
    }

    // Method to check if this InfinitePrecision object is equal to another InfinitePrecision object
    bool isEqual(const InfinitePrecision &other) const
    {
        // If the number of digits is different, the numbers are not equal
        if (digits.size() != other.digits.size())
        {
            return false;
        }

        // Compare each digit to see if they are equal
        for (int i = 0; i < digits.size(); i++)
        {
            if (digits[i] != other.digits[i])
            {
                return false;
            }
        }

        // If we made it through the loop, the numbers are equal
        return true;
    }

    // Method to check if this InfinitePrecision object is greater than another InfinitePrecision object
    bool isGreater(const InfinitePrecision &other) const
    {
        // If this number has more digits than the other number, it is greater
        if (digits.size() > other.digits.size())
        {
            return true;
        }

        // If the other number has more digits than this number, it is greater
        if (digits.size() < other.digits.size())
        {
            return false;
        }

        // Compare each digit from left to right to see which number is greater
        for (int i = 0; i < digits.size(); i++)
        {
            if (digits[i] > other.digits[i])
            {
                return true;
            }
            if (digits[i] < other.digits[i])
            {
                return false;
            }
        }

        // If we made it through the loop, the numbers are equal, so this number is not greater than the other number
        return false;
    }

    // Method to check if this InfinitePrecision object is less than another InfinitePrecision object
    bool isLess(const InfinitePrecision &other) const
    {
        // If this number has fewer digits than the other number, it is less
        if (digits.size() < other.digits.size())
        {
            return true;
        }

        // If the other number has fewer digits than this number, it is less
        if (digits.size() > other.digits.size())
        {
            return false;
        }

        // Compare each digit from left to right to see which number is less
        for (int i = 0; i < digits.size(); i++)
        {
            if (digits[i] < other.digits[i])
            {
                return true;
            }
            if (digits[i] > other.digits[i])
            {
                return false;
            }
        }

        // If we made it through the loop, the numbers are equal, so this number is not less than the other number
        return false;
    }

    // Method to check if this InfinitePrecision object is greater than or equal to another InfinitePrecision object
    bool isGreaterOrEqual(const InfinitePrecision &other) const
    {
        // If this number is equal to or greater than the other number, it is greater than or equal to it
        return isEqual(other) || isGreater(other);
    }

    // Method to check if this InfinitePrecision object is less than or equal to another InfinitePrecision object
    bool isLessOrEqual(const InfinitePrecision &other) const
    {
        // If this number is equal to or less than the other number, it is less than or equal to it
        return isEqual(other) || isLess(other);
    }

    // Method to convert the InfinitePrecision object to a string
    string toString() const
    {
        string result;
        for (int i = 0; i < digits.size(); i++)
        {
            result += to_string(digits[i]);
        }
        return result;
    }

    string state() const
    {
        string result = "Internal state:\n";
        for (int i = 0; i < digits.size(); i++)
        {
            result += "digits[" + to_string(i) + "] = " + to_string(digits[i]) + "\n";
        }
        return result;
    }
};
int main()
{
    // Test 1: Check if the constructor that takes an integer sets the value correctly
    InfinitePrecision num1(123456789);
    assert(num1.toString() == "123456789");
    cout << num1.state();

    // Test 2: Check if the constructor that takes a string sets the value correctly
    InfinitePrecision num2("98765432101234567890");
    assert(num2.toString() == "98765432101234567890");
    cout << num2.state();
    // Test 3: Check if the copy constructor creates a new object with the same value
    InfinitePrecision num3(num2);
    assert(num3.toString() == num2.toString());
    cout << num3.state();

    // Test 4: Check if the addition operator works correctly
    InfinitePrecision num4("12345678901234567890");
    InfinitePrecision num5("98765432109876543210");
    num4.add(num5);
    assert(num4.toString() == "111111111011111111100");
    cout << num4.state();

    // Test 5: Check if the subtraction operator works correctly
    InfinitePrecision num6("98765432109876543210");
    InfinitePrecision num7("12345678901234567890");
    num6.subtract(num7);
    assert(num6.toString() == "86419753208641975320");
    cout << num6.state();

    // Test 6: Check if the multiplication operator works correctly
    InfinitePrecision num8("12345678901234567890");
    InfinitePrecision num9("98765432109876543210");
    num8.multiply(num9);
    assert(num8.toString() == "121932631137021795745209395222020572100");
    cout << num8.state();

    // Test 7: Check if the division operator works correctly
    InfinitePrecision num10("98765432109876543210");
    InfinitePrecision num11("12345678901234567890");
    num10.divide(num11);
    assert(num10.toString() == "8");
    cout << num10.state();


    // Test 8: Check if the modulo operator works correctly
    InfinitePrecision num12("98765432109876543210");
    InfinitePrecision num13("12345678901234567890");
    num12.modulo(num13);
    assert(num12.toString() == "1977326720864197530");
    cout << num12.state();

    return 0;
}