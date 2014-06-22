Concordance
===========
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

— How to install —

Do the following steps to set your compiler (if needed):
- edit CC lines in the Makefile to reflect your compiler and optimizer
To compile:
- run make

To try some test cases do the following steps:
- copy the cpp files in test/Parser or test/Prefix_tree into the main directory
- run
	make TEST=1 PRG=prefix_tree_test
 or
	make TEST=1 PRG=parser_test
to compile the files depending on which test you would like to execute.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

— How to run the program —

Use:

	 ./concordance -h
to print a help message.

In test/ there is an example file. 
You may want to try:

		./concordance -i test/test_input.txt

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

— About the program —

Given a text file in stdin, the program prints out a “concordance”.
A concordance is a bit like an index: It’s an alphabetical list of all the words present in a body of text,
complete with citations of where each word appears (e.g., the line, page or sentence number). 
To sort the words the program uses and internal data structure that can be chosen by the user.
The current implementation allows the user to choose two different data structures:

1) A Prefix Tree;

2) A Red Black Tree.

These data structures have different characteristics in terms of space/time.
For example, a prefix-tree performs operations in O(k) where “k” is maximum length of the string in the text, while a rb-tree performs operations in O(log n) time where “n” is the number of string in the text.

For any questions, feel free to write at: campe8@nmsu.edu.

