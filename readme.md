# Question 1:
  write a program that needs to keep track of the
publications based on research conducted by an institute. Such records
are often required by ranking agencies. A single publication consists of
the following:
(a) Title
(b) Venue of publication
(c) A number of authors
(d) Each author has some affiliation. At least one author needs to have
the institute’s affiliation.
(e) Optionally a Digital Object Identifier, which gives a URL
(f) Year of publication
Write a program to design the appropriate data structure.
 # Question 2:

 The publication data is often given in the form of a bib file (a sample bib
file is attached). Write a program to parse this bib file, and construct
the appropriate data structure based on the content. Assume that our
institute’s affiliation is “IIIT-Delhi”. The way the data should be stored

should allow an easy search whenever a particular author name’s publica-
tions are requested. In case an invalid bib file is given, it should raise an

assert statement saying it is invalid. Cases of invalid bib files are:
(a) Formatting mistake, with incorrect braces or commas
(b) Same author repeated twice in any bib entry
(c) No author belonging to the institute’s affiliation.
You can separately take the list of faculty from a csv file.


 # Question 3:

 Write a different main function that takes the name of any one or more
author as an input via arguments, and show the list of their publications.
Then, calculate and print the average number of co-authors for each paper
by this author.

# build:

 make

 ./debug1  # for question 2

 ./debug2  # for question 3



# The output will be generated fpr the abve programs.


# THIS REPO IS BASICALLY UNDERSTANDING PURPOSES OF ANY THIRD PARTY TO HOW HEADER FILE IS CREATED USING LIBRARIES AND TESTING THAT FILE USING TEST.CPP.


 N.B: I have taken help of some online refernces like: 
 # https://www.geeksforgeeks.org/templates-cpp/ (GEEKFORGEEKS)

