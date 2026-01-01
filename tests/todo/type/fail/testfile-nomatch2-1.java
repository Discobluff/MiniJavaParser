
class A { }

class B extends A {
    void g() { g((A)this, (A)this); }
    void g(A a, B b) { }
}
class Main { public static void main(String args[]) { } }
