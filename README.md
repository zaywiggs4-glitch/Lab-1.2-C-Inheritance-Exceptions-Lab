# Lab-1.2-C-Inheritance-Exceptions-Lab

# Design Choices

I made a Person base class that Student and Instructor both inherit from, 
since they share a lot of the same stuff, id, name, email. Person checks 
that data is valid in its constructor, and then Student and Instructor add 
their own fields on top (grad year and courses for Student, office and 
teaches for Instructor) and override the to_json, to_yaml, and csv methods 
from the Serializable interface so each type knows how to serialize itself.

In main.cpp I used regular pointers with new and delete instead of 
unique_ptr, mostly so the memory management isn't hidden behind a smart 
pointer, I wanted it to be obvious where objects get created and cleaned up.

For the extra challenges, I added a from_json static method to both classes 
so you can go the other direction and turn a JSON object back into an actual 
Student or Instructor. I also wrote a CSV writer that can take a list with 
both Students and Instructors mixed together and write them to one CSV file 
using the same header row, it just leaves the fields blank that don't apply 
to whichever type it's writing.

# Where Exceptions Get Thrown

All the checking happens in the constructors, before anything actually gets 
saved to the object's variables. That way if a ValidationError gets thrown, 
you don't end up with a half finished object sitting around.

- Person throws if the id is 0 or negative, if the name is empty, or if the 
  email is missing an @ or a . in the right spot
- Student throws if grad_year is earlier than 2000
- Instructor throws if office is left empty
- from_json throws on both classes if a required field is missing from the 
  JSON before it even tries building the object

# How the CSV Escaping Works

csv_escape just checks if a string has a comma, a quote mark, or a newline 
in it. If none of those are there, it leaves the string alone. If one is, it 
wraps the whole thing in quotes and doubles up any quote marks inside it (so 
a " turns into ""), which is just how CSV normally deals with quotes showing 
up inside a quoted field.