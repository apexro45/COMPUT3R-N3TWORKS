import java.net.*;

public class DNSJavaClient {
    public static void main(String[] args) throws Exception {
        DatagramSocket clientSocket = new DatagramSocket();
        
        InetAddress serverIP = InetAddress.getByName("localhost");
        int serverPort = 9876;
        
        String domain = "example.com";
        
        byte[] sendData = domain.getBytes();
        byte[] receiveData = new byte[1024];
        
        DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, serverIP, serverPort);
        clientSocket.send(sendPacket);
        
        DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
        clientSocket.receive(receivePacket);
        
        String response = new String(receivePacket.getData()).trim();
        
        System.out.println("Response from DNS server: " + response);
        
        clientSocket.close();
    }
}
