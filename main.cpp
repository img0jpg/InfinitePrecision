#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

void tests();
void instructions();

class InfinitePrecision {
private:
  vector<int> digits;

public:
  // constructors
  InfinitePrecision() { digits.insert(digits.begin(), 1); }
  InfinitePrecision(int value) {
    while (value != 0) {
      digits.insert(digits.begin(), value % 10);
      value /= 10;
    }
  }
  InfinitePrecision(long long num) {
    while (num > 0) {
      digits.push_back(num % 10);
      num /= 10;
    }
    if (digits.empty())
      digits.push_back(0);
  }
  InfinitePrecision(const string &numberStr) {
    // Add the digits to the vector
    for (char c : numberStr) {
      digits.push_back(c - '0');
    }

    // Remove any leading zeros
    while (digits.size() > 1 && digits[0] == 0) {
      digits.erase(digits.begin());
    }
  }


  // Overloading Operators +, -, *, /, <, >, <=, >=, ==
  InfinitePrecision operator+(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    temp.add(other);
    return temp;
  }
  InfinitePrecision operator-(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    temp.subtract(other);
    return temp;
  }
  InfinitePrecision operator*(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    temp.multiply(other);
    return temp;
  }
  InfinitePrecision operator/(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    temp.divide(other);
    return temp;
  }
  InfinitePrecision operator%(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    temp.modulo(other);
    return temp;
  }
  bool operator==(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    return temp.isEqual(other);
  }
  bool operator<(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    return temp.isLess(other);
  }
  bool operator>(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    return temp.isGreater(other);
  }
  bool operator<=(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    return temp.isLessOrEqual(other);
  }
  bool operator>=(const InfinitePrecision &other) {
    InfinitePrecision temp = *this;
    return temp.isGreaterOrEqual(other);
  }
  InfinitePrecision& operator=(const InfinitePrecision &other) 
  {
    if (this != &other) 
    {
      digits = other.digits;
    }
    return *this;
  }
  InfinitePrecision& operator=(const string &other) 
  {
    InfinitePrecision temp(other);
      digits = temp.digits;
    return *this;
  }
  // Method to add another InfinitePrecision object to this one
  void add(const InfinitePrecision &other) {
    InfinitePrecision temp;
    int carry = 0;
    int i = digits.size() - 1;
    int j = other.digits.size() - 1;
    int sum = 0;
    temp.digits.clear();
    while (i >= 0 || j >= 0 || carry != 0) {
      sum = carry;
      if (i >= 0) {
        sum += digits[i];
        i--;
      }
      if (j >= 0) {
        sum += other.digits[j];
        j--;
      }
      temp.digits.push_back(sum % 10);
      carry = sum / 10;
    }
    digits = temp.digits;
    reverse(digits.begin(), digits.end());
  }
  void subtract(const InfinitePrecision &other) {
    InfinitePrecision temp;
    int borrow = 0;
    int i = digits.size() - 1;
    int j = other.digits.size() - 1;
    temp.digits.clear();

    while (i >= 0 || j >= 0) {
      int diff = borrow;
      if (i >= 0) {
        diff += digits[i];
        i--;  
      }
      if (j >= 0) {
        diff -= other.digits[j];
        j--;
      }
      if (diff < 0) {
        diff += 10;
        borrow = -1;
      } else {
        borrow = 0;
      }
      temp.digits.insert(temp.digits.begin(), diff);
    }

    digits = temp.digits;
  }

  void multiply(const InfinitePrecision &other) {
    int vectorIndex;
    InfinitePrecision result(0);
    result.digits.resize((digits.size() + other.digits.size() - 1), 0);
    InfinitePrecision temp(result);
    // Perform the multiplication, with carries at each step
    for (int i = digits.size() - 1; i >= 0; i--) {
      for (int j = other.digits.size() - 1; j >= 0; j--) {
        vectorIndex = i + j;
        int product = digits[i] * other.digits[j];
        temp.addToVector(vectorIndex, product);
      }
      result = (result + temp);
      temp = (temp - temp);
    }

    digits = result.digits;
    // Remove any leading zeros from the result
    this->trim();
  }

  void divide(const InfinitePrecision &divisor) {
    if (divisor.digits.size() == 1 && divisor.digits[0] == 0) {
      throw invalid_argument("division by zero");
    }
    if(digits.size() < divisor.digits.size())
    {
      digits.clear();
      digits.push_back(0);
      return;
    }
    InfinitePrecision quotient(0);
    InfinitePrecision remainder(0);

    for (int i = digits.size() - 1; i >= 0; i--) {
      remainder.digits.insert(remainder.digits.begin(), digits[i]);
      int quotientDigit = 0;
      while (remainder.isGreaterOrEqual(divisor)) {
        remainder = (remainder - divisor);
        quotientDigit++;
      }
      quotient.digits.insert(quotient.digits.end(), quotientDigit);
    }

    digits = quotient.digits;

    // Remove any leading zeros in the result
    this->trim();
  }

  void modulo(const InfinitePrecision &other) {
    if (other.digits.size() == 1 && other.digits[0] == 0) {
      throw invalid_argument("division by zero");
    }

    InfinitePrecision result(0);
    InfinitePrecision temp(0);
    temp.digits = digits;
    result.digits = digits;

    temp = (temp / other);
    temp = (temp * other);
    result = (result - temp);

    digits = result.digits;

    this->trim();
  }

  bool isEqual(const InfinitePrecision &other) const {
    // If the number of digits is different, the numbers are not equal
    if (digits.size() != other.digits.size()) {
      return false;
    }

    // Compare each digit to see if they are equal
    for (int i = 0; i < digits.size(); i++) {
      if (digits[i] != other.digits[i]) {
        return false;
      }
    }
    // If we made it through the loop, the numbers are equal
    return true;
  }

  bool isGreater(const InfinitePrecision &other) const {
    // If this number has more digits than the other number, it is greater
    if (digits.size() > other.digits.size()) {
      return true;
    }

    // If the other number has more digits than this number, it is greater
    if (digits.size() < other.digits.size()) {
      return false;
    }

    // Compare each digit from left to right to see which number is greater
    for (int i = 0; i < digits.size(); i++) {
      if (digits[i] > other.digits[i]) {
        return true;
      }
      if (digits[i] < other.digits[i]) {
        return false;
      }
    }
    return false;
  }

  bool isLess(const InfinitePrecision &other) const {
    // If this number has fewer digits than the other number, it is less
    if (digits.size() < other.digits.size()) {
      return true;
    }
    // If the other number has fewer digits than this number, it is less
    if (digits.size() > other.digits.size()) {
      return false;
    }
    // Compare each digit from left to right to see which number is less
    for (int i = 0; i < digits.size(); i++) {
      if (digits[i] < other.digits[i]) {
        return true;
      }
      if (digits[i] > other.digits[i]) {
        return false;
      }
    }
    return false;
  }

  bool isGreaterOrEqual(const InfinitePrecision &other) const {
    return isEqual(other) || isGreater(other);
  }

  bool isLessOrEqual(const InfinitePrecision &other) const {
    return isEqual(other) || isLess(other);
  }
  // HELPER FUNCTIONS

  // Remove any leading zeros in digits vector
  void trim() {
    while (digits.size() > 1 && digits[0] == 0) {
      digits.erase(digits.begin());
    }
  }

  // Method to add an int to a specific index and handles carry
  void addToVector(int index, int adder) {
    digits[index] += adder;
    if (digits[index] >= 10) {
      adder = digits[index];
      digits[index] %= 10;
      adder = int(adder / 10);
      digits[index - 1] += adder;
    }
  }

  // Method to convert the InfinitePrecision object to a string
  string toString() const {
    string result;
    for (int i = 0; i < digits.size(); i++) {
      result += to_string(digits[i]);
    }
    return result;
  }
};

int main() {
  InfinitePrecision num1(0);
  InfinitePrecision num2(0);
  InfinitePrecision solution(0);

  string x, y;
  char operatr;
 
  tests();
  instructions();
  cout << "X: ";
  cin >> x;
  cout << "Y: ";
  cin >> y;

  num1 = x;
  num2 = y;
  
  cout << "Operator: ";
  cin >> operatr;
  
  
  switch (operatr)
  {
    case '+':
      solution = num1 + num2;
      break;
    case '-':
      solution = num1 - num2;
      break;
    case '*':
      solution = num1 * num2;
      break;
    case '/':
      solution = num1 / num2;
      break;
    default:
      cout << "Input error! Please try again." << endl;
      return 0;
  }
  
  cout << num1.toString() << " " << operatr << " " << num2.toString() 
    << " = " << solution.toString() << "\n\n";
  
  return 0;
}

void tests() { // Test 1: Check if the constructor that takes an integer sets
               // the value co>
  InfinitePrecision num1(123456789);
  cout << "num1 expected: 123456789\n"
       << "num1 actual:   " << num1.toString() << "\n\n";
  assert(num1.toString() == "123456789");

  // Test 2: Check if the constructor that takes a string sets the value corr>
  InfinitePrecision num2("98765432101234567890");
  cout << "num2 expected: 98765432101234567890\n"
       << "num2 actual:   " << num2.toString() << "\n\n";
  assert(num2.toString() == "98765432101234567890");

  // Test 3: Check if the copy constructor creates a new object with the same>
  InfinitePrecision num3(num2);
  cout << "num3 expected: 98765432101234567890\n"
       << "num3 actual:   " << num3.toString() << "\n\n";
  assert(num3.toString() == num2.toString());

  // Test 4: Check if the addition operator works correctly
  InfinitePrecision num4("12345678901234567890");
  InfinitePrecision num5("98765432109876543210");
  cout << num4.toString() << " + " << num5.toString() << "\n";

  // num4.add(num5); this works - testing operator overloading
  num4 = (num4 + num5);
  cout << "num4 expected: 111111111011111111100\n"
       << "num4 actual:   " << num4.toString() << "\n\n";
  assert(num4.toString() == "111111111011111111100");

  // Test 5: Check if the subtraction operator works correctly
  InfinitePrecision num6("98765432109876543210");
  InfinitePrecision num7("12345678901234567890");
  cout << num6.toString() << " - " << num7.toString() << "\n";
  // num6.subtract(num7); this works - testing operator overloading
  num6 = (num6 - num7);
  cout << "num6 expected: 86419753208641975320\n"
       << "num6 actual:   " << num6.toString() << "\n\n";
  assert(num6.toString() == "86419753208641975320");

  // Test 6: Check if the multiplication operator works correctly
  InfinitePrecision num8("12345678901234567890");
  InfinitePrecision num9("98765432109876543210");
  cout << num8.toString() << " * " << num9.toString() << "\n";
  // num8.multiply(num9); this works - testing operator overloading
  num8 = (num8 * num9);
  cout << "num8 expected: 1219326311370217952237463801111263526900\n"
       << "num8 actual:   " << num8.toString() << "\n\n";
  assert(num8.toString() == "1219326311370217952237463801111263526900");

  // Test 7: Check if the division operator works correctly
  InfinitePrecision num10("98765432109876543210");
  InfinitePrecision num11("12345678901234567890");
  cout << num10.toString() << " / " << num11.toString() << "\n";
  // num10.divide(num11); this works, testing operator overloading
  num10 = (num10 / num11);
  cout << "num10 expected: 8\n"
       << "num10 actual:   " << num10.toString() << "\n\n";
  assert(num10.toString() == "8");

  // Test 8: Check if the modulo operator works correctly
  InfinitePrecision num12("98765432109876543210");
  InfinitePrecision num13("12345678901234567890");
  cout << num12.toString() << " % " << num13.toString() << "\n\n";
  num12.modulo(num13);
  cout << "num12 expected: 900000000090\n"
       << "num12 actual:   " << num12.toString() << "\n";
  assert(num12.toString() == "900000000090");

  cout << "all checks passed!\n\n";
}

void instructions() {
  cout << "\n\nWelcome to Bruce's Infinitely Precise Calculator!\n"
       << "(that only accepts positive integers)\n"
       << "--------------------------------------------------------------------"
          "--------\n";
  cout << "To use this calculator, input numbers in the form of the following "
          "command:\n"
       << "X (+) Y = ?\n"
       << "where we provide the '?'!\n";
  cout << "Of course you can use whatever numbers you want in place of X or Y\n"
       << "and you can replace the (+) operator with any of your favorite "
          "mathematical operators!\n";
  cout << "Such as:\n"
       << " - : for subtraction\n"
       << " + : for addition\n"
       << " * : for multiplication\n"
       << " / : for division\n"
       << " % : for getting the remainder\n\n";
  cout << "Follow this format for a fun and informative function!\n\n";
}