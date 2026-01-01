class A { A() {} A(int x) {} }

class B extends A {
    void m() { B b = new B(0); }
}
class Main { public static void main(String args[]) { } }
