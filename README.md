# Linked List Implementation

## Project Overview
This repository contains the implementation of a linked list as specified for the assignment. The linked list supports various operations such as insertion, deletion, reversing, and more, as detailed in the provided `clist.h` header file.

## Building the Code

To compile the code, ensure that you have a C compiler such as `gcc` installed. Navigate to the directory containing the source files and run the following command:

```bash
gcc -o linked_list clist_test.c clist.c clist.h -Wall -Wextra -fsanitize=address
```

This command will compile the source files main.c and clist.c into an executable named linked_list. It enables all compiler warnings with -Wall and -Wextra, and includes the AddressSanitizer library with -fsanitize=address to detect memory leaks and other memory-related issues.



### Running the Code

After building, you can run the program by executing:

```bash
./linked_list
```

This will execute the linked list program and trigger any tests or operations defined in main.c.



### Testing
The repository includes a series of automated tests to ensure each function operates as expected. These tests can be reviewed and run to validate the functionality of the linked list operations.


## Contributing
Feel free to fork the repository and submit pull requests. You can also open issues to discuss potential changes or report bugs.

## License
This project is licensed under the MIT License - see the LICENSE.md file for details.

## Author
- [AHMED MOHAMED](mailto:ahmdmshazly@cmu.edu)


-------------------------

***Note: This README is a part of a submission to an academic course.***


