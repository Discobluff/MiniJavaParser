public class dangling_else {
    public int test(){
        if (2 > 1) if (true) if (false) return 1; else return 0;

        return 0;
    }    
}
