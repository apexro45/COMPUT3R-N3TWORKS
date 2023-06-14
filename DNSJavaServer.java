import java.net.*;

public class DNSJavaServer {
    public static void main(String[] args) throws Exception {
        DatagramSocket serverSocket = new DatagramSocket(9876);
        byte[] receiveData = new byte[1024];
        byte[] sendData;
        
        System.out.println("DNS Server started on port 9876.");

        while (true) {
            DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
            serverSocket.receive(receivePacket);
            
            String request = new String(receivePacket.getData()).trim();
            System.out.println("Received request: " + request);
            
            InetAddress clientIP = receivePacket.getAddress();
            int clientPort = receivePacket.getPort();
            
            String response = processRequest(request);
            sendData = response.getBytes();
            
            DatagramPacket sendPacket = new DatagramPacket(sendData, sendData.length, clientIP, clientPort);
            serverSocket.send(sendPacket);
            
            receiveData = new byte[1024];
        }
    }
    
    private static String processRequest(String request) {
        // Process the request and generate a response
        // This is a simplified example, you can implement your own logic here
        
        if (request.equals("example.com")) {
            return "192.168.0.1";
        } else {
            return "Invalid request";
        }
    }
}
