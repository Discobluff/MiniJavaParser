
class A { }

class B extends A {
    void g() { g((A)this); }
    void g(B b) { }
}
class Main { public static void main(String args[]) { } }
