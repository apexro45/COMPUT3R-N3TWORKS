#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define WINDOW_SIZE 4
#define MAX_PACKET_LENGTH 100

// Function to simulate sending a packet
void sendPacket(char packet[]) {
    printf("Sending packet: %s\n", packet);
}

// Function to simulate receiving an acknowledgment
void receiveAcknowledgment(char packet[]) {
    printf("Received acknowledgment: %s\n", packet);
}

int main() {
    char senderBuffer[WINDOW_SIZE][MAX_PACKET_LENGTH];
    int base = 0;
    int nextSeqNum = 0;
    bool isWindowFull = false;

    int totalPackets;
    printf("Enter the total number of packets to send: ");
    scanf("%d", &totalPackets);
    getchar(); // To consume the newline character after the integer input

    printf("Sliding Window Protocol Simulation\n");

    while (base < totalPackets) {
        // Send packets if the window is not full
        while (nextSeqNum < base + WINDOW_SIZE && nextSeqNum < totalPackets) {
            char packet[MAX_PACKET_LENGTH];
            printf("Enter packet %d: ", nextSeqNum);
            fgets(packet, sizeof(packet), stdin);
            packet[strcspn(packet, "\n")] = '\0'; // Remove trailing newline character

            sendPacket(packet);
            strcpy(senderBuffer[nextSeqNum % WINDOW_SIZE], packet);
            nextSeqNum++;
        }

        // Simulate receiving an acknowledgment automatically
        if (base < totalPackets) {
            receiveAcknowledgment(senderBuffer[base % WINDOW_SIZE]);
            base++;
            printf("Acknowledgment accepted.\n");
        }
    }

    return 0;
}
