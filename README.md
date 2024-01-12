
# S-CHAT Program README

## Overview
This chat program enables message exchange between two machines on the same network. It leverages the UDP protocol for communication and uses socket programming for inter-process communication (IPC). The program is designed to work on Linux systems.

## Dependencies
- **List Library**: A custom library for list operations included in the project.
- **Pthreads**: A POSIX threads library for multi-threading support. Note that only the binary is available for the pthreads library, as the source code is copyrighted by the University of British Columbia (UBC). The library is used with permission.

## To compile and run the chat program, follow these steps:

1. Clone the repository containing the chat program and its dependencies.
2. Navigate to the project directory and run the make command to create the executable file.
3. Execute the chat program using the following command: ./s-chat [transmit_port] [receiver_machine_name] [receive_port]
4. To end the chat on all machines, press !xx on the console.

## Functionalities
- **Message Exchange**: The program allows sending and receiving text messages between two machines on the same network.
- **Timestamps**: Each message is tagged with a timestamp indicating when it was sent.
- **Multi-Threading**: The program uses multiple threads for handling different aspects of the chat, such as reading console entries, transmitting messages, receiving messages, and printing messages to the console.

## Architecture
The program is structured around several key functions and threads:
- **readConsoleEntriesAndSendToServer**: Reads messages from the console and sends them to the server thread for processing.
- **readBufferAndTransmit**: Handles the transmission of messages over the network.
- **listenPortAndFeedBuffer**: Listens for incoming messages on a specified port and feeds them into a buffer.
- **readBufferAndPrint**: Reads messages from the buffer and prints them to the console.
- **server**: Manages message buffers and coordinates the sending and receiving of messages.

## List Library Overview
The List Library is a pivotal part of the chat program, providing a dynamic linked list implementation for efficient message management. Key features include:

1. Creation & Deletion: Facilitates the creation of new lists and deletion of existing ones, ensuring proper memory management.
2. Item Count & Access: Allows counting of items and access to the first, last, and current items in the list.
3. Navigation: Enables traversal through the list with functions to move to the next, previous, or any specific item.
4. Modification: Includes a variety of methods for adding or removing items at any position, with automatic adjustment of the current pointer.
5. Concatenation: Supports merging two lists into one, with the second list being absorbed into the first.
6. Searching: Provides a search mechanism that uses a comparator function to find items matching specific criteria.

This library underpins the server's ability to queue and process messages reliably, enhancing the chat program's functionality.

## Screenshot

![Screenshot of Chat Program](/Screenshots/img.png "Screenshot showing the chat program in action")

## Limitations and Known Issues
- The program is designed to work specifically on Linux systems.
- It requires both the source code and the binary of specific libraries (List Library and pthreads).
- The program uses UDP, which does not guarantee the delivery of messages.

## Acknowledgements
We extend our sincere gratitude to the University of Saskatchewan (USask) for their support and resources. A special thank you to Professor Dwight Makaroff for his invaluable mentorship and guidance throughout the development of this project. We also wish to express our appreciation to the University of British Columbia (UBC) for providing the UBC-Pthreads Library, which has been instrumental in the implementation of this program.

## Contact Information
For any queries or support, please contact at waleed8898@gmail.com.