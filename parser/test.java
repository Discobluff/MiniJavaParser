class A {
    int a;
    B b;
    A() {}
}

class B extends A {
    int b;
    B() {}
}

class Main {
    public static void main(String args[]) {
        A a = new A();
        boolean ok = (B)a.b == null;

        /*types primitifs*/
ok = ok && (int)1 == 1 && (int)1 == 1;
    }
}