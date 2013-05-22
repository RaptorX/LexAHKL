***

Autohotkey L Lexer
------------------------

This is a Lexer for autohotkey L that has been written from scratch 
to match Autohotkeys syntax as much as possible.

Features
------------------------

1. All bugs found in the previous versions of AHK Lexers have been fixed
2. Folding
3. More accurate reproduction of Autohotkey syntax
4. Lexing of Object Syntax
5. Intelligent highlighting of Special Command Parameters
6. Two user defined keyword lists available
7. Extremely customizable coloring. You can change the following colors to the lexer:
	* Documentation Comments (See test file for information of what this is)
	* Line Comments
	* Block Comments
	* Strings
	* String Block
	* String Block Options
	* String Block Line Comments
	* Directives
	* Commands
	* Special Command Parameters
	* Control Flow Keywords (if, else, break, continue and so on)
	* Keyboard/Mouse Keys
	* Labels
	* Hotkeys
	* Hotstrings
	* Hotstring Options
	* Decimal Numbers
	* Hexadecimal Numbers
	* User Created Variables
	* Builtin Variables
	* Variable References (in expressions)
	* Objects
	* User Functions
	* Builtin Functions
	* Escape Sequences
	* Two user defined keyword lists <-- you can highlight custom words if you want 

As you can see this Lexer is extremely customizable so i will make a few notes on that.

If you dont want to color a few things then just dont assign any color to them. Also a few things might look like 'duplicates' but they arent. I made it like that to give the programmer all the flexibility you need for making things your way. While you might prefer the line comments and the string block comments to look the same way there are other people who might not want that.

License:
------------------------
Copyright (C) 2013 Isaias Baez


This program is free software: you can redistribute it and/or  
modify it under the terms of the GNU General Public License as  
published by the Free Software Foundation, either version 3 of  
the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,  
but **WITHOUT ANY WARRANTY**; without even the implied warranty of  
**MERCHANTABILITY** or **FITNESS FOR A PARTICULAR PURPOSE**.  See the  
GNU General Public License for more details.

You should have received a copy of the GNU General Public License  
along with this program. If not, see <<http://www.gnu.org/licenses/gpl-3.0.txt>>

Isaias Baez  
Emden, Germany  
<<graptorx@gmail.com>>

***