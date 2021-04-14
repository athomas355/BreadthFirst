BreadthFirst Traversal

What it Does:
The BreadthFirst Traversal is a linux utility that outputs file names from a given directory in breadth-first order. 
There are also other methods that alter the output from just the filenames to other information about the file and then its filename.

How to Use it:
bt [OPTIONS] [directory] [directory...]

OPTIONS:
-h	Help Message
-L	Follow SymLinks (This doesn't work in this version)
-d	Show the time of last modification
-g	print group name assoicated with file
-i	print the number of links to file in inode table
-p	print the permission bits of file
-s	prints the size of file
-t	prints the type of file 
-u	print user naem associated with file
-l	option as if -tpiugs were all chosen

How it Does it:
Psedocode for BreadthFirst:
breadthfirst(root) {
	enqueue(root);
	while(!empty(queue)) {
		next = dequeu(queue);
		for each node directly below next {
			visit(node);
			if(is_a_directory(node)) {
				enqueue(node);
			}
		}	
	}
}

How to Build It:
	1. make
	2. make install (only if you have root privileges)
	3. make clean

Known Bugs:
	-Symlinks just don't work. There are two different stat calls, stat and lstat. stat is for looking at the information of a file. lstat is looking at the link itself.
	When we looked at a symlink that pointed to a directory. It would know that the Symlink was a link but it was not able to determine that it was a directory.

Authors: Ashish Thomas

Github: https://github.com/athomas355/BreadthFirst.git
	git@github.com:athomas355/BreadthFirst.git
