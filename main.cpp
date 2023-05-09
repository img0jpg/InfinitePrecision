#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include <math.h>

using namespace std;

class InfinitePrecision
{
private:
    vector<int> digits;

public:
    // constructors
    InfinitePrecision()
    {
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
    InfinitePrecision(long long num)
    {
        while (num > 0)
        {
            digits.push_back(num % 10);
            num /= 10;
        }
        if (digits.empty())
            digits.push_back(0);
    }
    InfinitePrecision(const string &numberStr)
    {
        // Add the digits to the vector
        for (char c : numberStr)
        {
            digits.push_back(c - '0');
        }

        // Remove any leading zeros
        while (digits.size() > 1 && digits[0] == 0)
        {
            digits.erase(digits.begin());
        }
    }

    // Overloading Operators +, -, *, /, <, >, <=, >=, ==
    InfinitePrecision operator +(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        temp.add(other);
        return temp;
    }
    InfinitePrecision operator -(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        temp.subtract(other);
        return temp;
    }
    InfinitePrecision operator *(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        temp.multiply(other);
        return temp;
    }
    InfinitePrecision operator /(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        temp.divide(other);
        return temp;
    }
    InfinitePrecision operator %(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        temp.modulo(other);
        return temp;
    }
    bool operator ==(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        return temp.isEqual(other);
    }
    bool operator <(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        return temp.isLess(other);
    }
    bool operator >(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        return temp.isGreater(other);
    }
    bool operator <=(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        return temp.isLessOrEqual(other);
    }
    bool operator >=(const InfinitePrecision &other)
    {
        InfinitePrecision temp = *this;
        return temp.isGreaterOrEqual(other);
    }



    // Method to add another InfinitePrecision object to this one
    void add(const InfinitePrecision &other)
    {
        InfinitePrecision temp;
        int carry = 0;
        int i = digits.size() - 1;
        int j = other.digits.size() - 1;
        int sum = 0;
        temp.digits.clear();
        while (i >= 0 || j >= 0 || carry != 0)
        {
            sum = carry;
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
            temp.digits.push_back(sum % 10);
            carry = sum / 10;
        }
        digits.swap(temp.digits);
        reverse(digits.begin(), digits.end());
    }
    void subtract(const InfinitePrecision &other)
    {
        InfinitePrecision temp;
        int borrow = 0;
        int i = digits.size() - 1;
        int j = other.digits.size() - 1;
        temp.digits.clear();

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
            temp.digits.insert(temp.digits.begin(), diff);
        }

        // Remove any leading zeros
        while (digits.size() > 1 && digits[0] == 0)
        {
            temp.digits.erase(digits.begin());
        }

        digits.swap(temp.digits);
    }

    void multiply(const InfinitePrecision &other)
    {
        vector<int> result(digits.size() + other.digits.size() - 1, 0);
        // Perform the multiplication, with carries at each step
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            int carry = 0;
            for (int j = other.digits.size() - 1; j >= 0; j--)
            {
                int product = digits[i] * other.digits[j] + carry + result[i + j + 1];
                carry = product / 10;
                result[i + j + 1] = product % 10;
            }
            result[i] += carry;
        }

        // Remove any leading zeros from the result
        while (result.size() > 1 && result.front() == 0)
        {
            result.erase(result.begin());
        }

        digits.swap(result);
    }
    
    void divide(const InfinitePrecision &divisor)
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

        digits.swap(quotient.digits);
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
};

int main()
{
    // Test 1: Check if the constructor that takes an integer sets the value correctly
    InfinitePrecision num1(123456789);
    cout << "num1 expected: 123456789\n"
         << "num1 actual:   " << num1.toString() << "\n";
    assert(num1.toString() == "123456789");

    // Test 2: Check if the constructor that takes a string sets the value correctly
    InfinitePrecision num2("98765432101234567890");
    cout << "num2 expected: 98765432101234567890\n"
         << "num2 actual:   " << num2.toString() << "\n";
    assert(num2.toString() == "98765432101234567890");

    // Test 3: Check if the copy constructor creates a new object with the same value
    InfinitePrecision num3(num2);
    cout << "num3 expected: 98765432101234567890\n"
         << "num3 actual:   " << num3.toString() << "\n";
    assert(num3.toString() == num2.toString());

    // Test 4: Check if the addition operator works correctly
    InfinitePrecision num4("12345678901234567890");
    InfinitePrecision num5("98765432109876543210");
    cout << num4.toString() << " + " << num5.toString() << "\n";
    //num4.add(num5); this works - testing operator overloading
    num4 = (num4 + num5);
    cout << "num4 expected: 111111111011111111100\n"
         << "num4 actual:   " << num4.toString() << "\n";
    assert(num4.toString() == "111111111011111111100");

    // Test 5: Check if the subtraction operator works correctly
    InfinitePrecision num6("98765432109876543210");
    InfinitePrecision num7("12345678901234567890");
    cout << num6.toString() << " - " << num7.toString() << "\n";
    //num6.subtract(num7); this works - testing operator overloading
    num6 = (num6 - num7);
    cout << "num6 expected: 86419753208641975320\n"
         << "num6 actual:   " << num6.toString() << "\n";
    assert(num6.toString() == "86419753208641975320");

    // Test 6: Check if the multiplication operator works correctly
    InfinitePrecision num8("12345678901234567890");
    InfinitePrecision num9("98765432109876543210");
    cout << num8.toString() << " * " << num9.toString() << "\n";
    num8.multiply(num9);
    cout << "num8 expected: 121932631137021795745209395222020572100\n"
         << "num8 actual:   " << num8.toString() << "\n";
    assert(num8.toString() == "121932631137021795745209395222020572100");

    // Test 7: Check if the division operator works correctly
    InfinitePrecision num10("98765432109876543210");
    InfinitePrecision num11("12345678901234567890");
    cout << num10.toString() << " / " << num11.toString() << "\n";
    num10.divide(num11);
    cout << "num10 expected: 8\n"
         << "num10 actual:   " << num10.toString() << "\n";
    assert(num10.toString() == "8");

    // Test 8: Check if the modulo operator works correctly
    InfinitePrecision num12("98765432109876543210");
    InfinitePrecision num13("12345678901234567890");
    cout << num12.toString() << " % " << num13.toString() << "\n";
    num12.modulo(num13);
    cout << "num12 expected: 1977326720864197530\n"
         << "num12 actual:   " << num12.toString() << "\n";
    assert(num12.toString() == "1977326720864197530");

    return 0;
}
