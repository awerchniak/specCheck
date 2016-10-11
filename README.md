# specCheck
## Background:
```
During the summer of 2015, I had recently completed my freshman year and was working at BES Design/Build,
LLC in Fairhope Alabama. As I had little engineering experience, when I first got there I was initially 
given more remedial tasks before moving onto learning the trade and working with engineers. One of the 
worst things I had to do was audit construction specification books by hand. These are 2000+ page 
documents, so as you imagine going through each page could get pretty tedious. Having recently taken my 
first collegeprogramming course, I realized that I could just write a program to do it for me instead 
(Luckily, I received PDF's of the books, which I could export as .txt files). On my own time (after 
work every day), I worked on this program, and after about a month, I came up with the working version 
that I now have. It's not pretty, and the user works entirely in the command line, but given that I had 
minimal programming background at the time I am proud of it. It brought my 4-6 hour auditing process down 
to just 20-30 seconds.
```
## Implementation:
```
specCheck is a relatively simple program. The problems I looked for in the spec books were Spec Writer 
Notes and misreferenced sections. specCheck parses the text buffer to first fill a correct table of sections 
from the document's table of contents, and then cross references it with every section mentioned. specCheck 
recognizes sections because they always appear as 6 digit numbers (sometimes with decimals afterward), and 
often appear in the form ## ## ##. Through various testing, this proved to be a good system. In addition to 
listing the errors in section references, specCheck's report includes any spec writer notes.
```
### Input:
Construction Specification Book, saved as a .txt file
### Output:
A .txt file listing all section discrepancies and all spec writer notes.
### Compiling:
cc -o specCheck specCheck.c
