package com.sefianti.jni;

import java.io.*;

public class KeyboardHook {
    private KeyEvents keyEvents;

    static {
        try {
            System.load(extractLibrary());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static String extractLibrary() throws IOException {
        String osName = System.getProperty("os.name").toLowerCase();
        String osArch = System.getProperty("os.arch").toLowerCase();
        if (osName.startsWith("windows")) {
            osName = "windows";
        }

        String resourcePath = String.format("/NATIVE/%s/%s/native.dll", osArch, osName);

        InputStream inputStream = KeyboardHook.class.getResourceAsStream(resourcePath);

        File tempFile = File.createTempFile("native", ".dll");
        OutputStream outputStream = new FileOutputStream(tempFile);

        byte[] buffer = new byte[1024];
        int length;
        while ((length = inputStream.read(buffer)) != -1) {
            outputStream.write(buffer, 0, length);
        }

        inputStream.close();
        outputStream.close();

        return tempFile.getAbsolutePath();
    }

    public KeyboardHook ( KeyEvents keyEvents ) {
        this.keyEvents = keyEvents;
        this.init();
    }

    private void dispatchKeyDown ( int keyCode ) {
        keyEvents.dispatchKeyDown(keyCode);
    }

    private void dispatchKeyUp ( int keyCode ) {
        keyEvents.dispatchKeyUp(keyCode);
    }

    private native void init();
    
    public native void destroy();
    
    public interface KeyEvents {
        public void dispatchKeyDown ( int keyCode );

        public void dispatchKeyUp ( int keyCode );
    }
}