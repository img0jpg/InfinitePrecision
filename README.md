# InfinitePrecision
* Integers are usually stored in 32 bit
  * -2bn .. +2bn-1
* Long Double is usually stored in 80 bits
  * About 18 digits of precision
    
### **I want to do MATH with a lot more precision! At least hundreds of digits**
* Write a class that stores a number in (high) precision

  Precision should expand as needed up to a class-wide hard-coded limit

  v1 - integer, positives only

  v2 - integer, handle negative numbers (you may want to design this before you implement v1)
         Note that this is perhaps the hardest part of the project
         
  v3 - fractional arithmetic ( 500.324523222 )
* Implement:
    * initialize from string, long, copy
    * implement add, subtract, times, divide, mod, and comparison operators (<, etc)
        
        can be separate methods, but then do the operator overloading
    * return string/long
 * (recommended)
    * implement a "state()" function that returns the internal state as a string for debug
    * implement ++ and --
* Test cases
  
  *yup, lots*
* main() / driver
  * A simple calculator
  * read lines from user
  * M + N, M - N, etc -- output the result
-------
For the Record:

My baseline implementation of v2 is a few hundred lines of code. The basics are about 20 lines per method -- but design and forethought are KEY. I cannot imagine this being done without design thinking up front.

Points:

75 - functioning (completeness, correctness)

75 - cleanliness (design and style -- I will EXPECT design notes in your pseudocode)

50 - richness of testing
