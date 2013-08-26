// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

// Test case for checker GlobalAccInCtor

// http://julipedia.meroh.net/2005/10/c-constructors-and-global-data.html
// http://www.parashift.com/c++-faq-lite/static-init-order.html
// http://stackoverflow.com/a/1597426
// http://stackoverflow.com/a/1271692

// The problem:
// A static object (or variable) is accessed before it is fully initialized.
// Object: Fully initialized by constructor.
//   static Foo x();
// Built-in type: Initialized by function call.
//   static int x = f();

// Case for object:
// 1. `class Integer` is declared and defined (may be defined elsewhere).
// 2. `class Foo` is declared. (Can be swapped with 1.)
// 3. `static Foo` object is declared (to call `Foo::Foo()`).
// 4. `static Integer Global_Integer` object is declared. (Can be swapped with 3.)
// 5. `Foo::Foo()` is defined. It uses `Global_Integer`.

// Case for built-in type:
// 1. `class Foo` is declared.
// 2. Function `int f()` is declared and defined (may be defined elsewhere). (Can be swapped with 1.)
// 3. Object `static Foo` is declared (to call `Foo::Foo()`).
// 4. Variable `static int x` is declared and initialized with `f()`. (Can be swapped with 3.)
// 5. `Foo::Foo()` is defined. It uses `x`.

// Problem:
// static global variable (object or built-in) accessed in constructor.

#include <iostream>
using std::cout;

int get_number(const int& v) { return v; }

class Integer {
private:
  int m_value;
public:
  Integer(int v) {
    m_value = v;
  }
  int get_value(void) const { return m_value; }
};

class Foo {
public:
  Foo(void);
private:
  int m_val;
  static int m_constant;
  static int m_function;
};

const static Foo Global_Foo_Before; // May print uninitialized values (zeros)

const static Integer Global_Integer(1);
const static int x_constant = 1;
const static int x_function = get_number(1);
int Foo::m_constant = 1;
int Foo::m_function = get_number(1);

const static Foo Global_Foo_After;

Foo::Foo(void) : m_val(Global_Integer.get_value()) // bug: static acc in ctor
{
  cout << "Non-member static class instance (direct):                   "
       << Global_Integer.get_value() << "\n"; // bug: static acc in ctor
  cout << "Non-member static class instance (initializer):              "
       << m_val << "\n";
  cout << "Non-member static built-in instance initialized by constant: "
       << x_constant << "\n"; // not bug?: x_constant initialized by constant
  cout << "Non-member static built-in instance initialized by function: "
       << x_function << "\n"; // bug: static acc in ctor
  cout << "Member static built-in instance initialized by constant:     "
       << m_constant << "\n"; // not bug?: m_constant initialized by constant
  cout << "Member static built-in instance initialized by function:     "
       << m_function << "\n"; // bug: static acc in ctor
}

int main(void)
{
  return 0;
}
