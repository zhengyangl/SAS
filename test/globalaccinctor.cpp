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

class Integer {
  int m_value;
  bool m_initialized; // initialized to 0 in static instance before ctor call
public:
  Integer(int v) {
    m_value = v;
    m_initialized = true;
  }
  int get_value(void) const { return m_value; }
  bool get_initialized(void) const { return m_initialized; }
};

class Foo {
public:
  Foo(void);
private:
  int m_val;
};

int f() { return 5; }

const static Foo Global_Foo_Before; // May print "0" (unitialized value)
const static Integer Global_Integer(5);
const static int x = f();
const static int y = 6;
const static Foo Global_Foo_After;

Foo::Foo(void) : m_val(Global_Integer.get_value()) // bug: global acc in ctor
{
  cout << Global_Integer.get_value() << "\n"; // bug: global acc in ctor
  cout << Global_Integer.get_initialized() << "\n";
  cout << x << "\n"; // bug: global acc in ctor
  cout << y << "\n"; // not bug: y initialized by constant
  cout << m_val << "\n";
}

int main(void)
{
  return 0;
}
