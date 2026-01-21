class A { A() {} }

class B { void m() { A a = (B)(A)new A(); } }
class Main { public static void main(String args[]) { } }
