
import com.sefianti.jni.KeyboardHook;
import com.sefianti.jni.KeyboardHook.KeyEvents;
public class Main {
    public static void main(String[] args) {
        KeyEvents test = new Test1();
        KeyboardHook hook = new KeyboardHook(test);
    }

    private static class Test1 implements KeyEvents {

        public void dispatchKeyDown ( int keyCode ) {
            System.out.println("down " + keyCode);
        }

        public void dispatchKeyUp ( int keyCode ) {
            System.out.println("keyUp " + keyCode);
        }
    }
}