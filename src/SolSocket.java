import java.io.*;
import java.nio.*;
import java.net.*;

public class SolSocket {
    Socket socket;
    //InputStream input;
    //OutputStream output;
    SolSocket(Socket socket) {
        this.socket = socket;
    }
    SolSocket(String address, int port) throws IOException {
        socket = new Socket(address, port);
        //input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    }
    public int availableBytes() throws IOException {
        return socket.getInputStream().available();
    }
    public boolean hasData() throws IOException {
        return socket.getInputStream().available() > 0;
    }
    public int readByte() throws IOException {
        return socket.getInputStream().read();
    }
    public byte[] readByte(int number) throws IOException {
        byte[] bytes = new byte[number];
        socket.getInputStream().read(bytes);
        return bytes;
    }
    public byte[] readBytes() throws IOException {
        InputStream inputStream = socket.getInputStream();
        if(inputStream.available() == 0)
            return null;
        ByteArrayOutputStream byteArray = new ByteArrayOutputStream();
        while(inputStream.available() > 0) {
            byte[] bytes = new byte[inputStream.available()];
            inputStream.read(bytes);
            byteArray.write(bytes);
        }
        return byteArray.toByteArray();
    }
    public String readLine() throws IOException {
        InputStream inputStream = socket.getInputStream();
        if(inputStream.available() == 0)
            return null;
        ByteArrayOutputStream byteArray = new ByteArrayOutputStream();
        while(inputStream.available() > 0) {
            int b = inputStream.read();
            if(b == '\r') { inputStream.read(); break; }
            if(b == '\n') { break; }
            byteArray.write((byte)b);
        }
        return byteArray.toString();
    }
    public void writeByte(byte b) throws IOException {
        socket.getOutputStream().write(b);
    }
    public void writeBytes(byte[] b) throws IOException {
        socket.getOutputStream().write(b);
    }
    public boolean isClosed() { return socket.isClosed(); }
    public boolean isConnected() { return socket.isConnected(); }
    public void close() throws IOException { socket.close(); }
}
