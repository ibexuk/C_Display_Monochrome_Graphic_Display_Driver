
######################################################################
##### Open sourced by IBEX, an electronic product design company #####
#####    based in South East England.  http://www.ibexuk.com     #####
######################################################################

This project was previously available from our http://www.embedded-code.com source code store (owned by IBEX UK Ltd) and is a fully packaged and documented project.  After several years selling these source code projects to developers we took the decision to finally open source all of the projects hosted on the site.

The projects original page on the embedded-code.com web site can be found at: http://www.embedded-code.com/source-code/displays/monochrome-screens/monochrome-screen-driver

---------------------------------------------------------------------------------
----- UPDATES MADE TO THE SOURCE CODE BUT NOT ADDED TO THE TECHNICAL MANUAL -----
---------------------------------------------------------------------------------
The following updates we're made for a projec we worked on but we didn't get to updating the documentation with them as we moved onto a new but seperate version of the display driver (see other "Universal Display Driver" GitHub project). To use the driver exactly as it is documented in the embedded-code technical manual use the original commit here on GitHub. The following are our notes of the changes made for the V1.04 commit:


The old bitmap file based font handling has been deleted (it was simple but a pain to design fonts).  Display.c and .h are inteded to be the new LCD driver files once all orientations are tested.

Defines have changed - manual not updated.

Moving forward with this driver should be very straightforward.

Whilst we've been careful it is possible to display scrolling text may have been damaged, although hopefully not.

Note that for the most recent project we've had to use the manager PSV functions of the compiler by altering the main CONSTANT define and declaring our arrays with a special attribute marker.  This is C30 compiler specification deal with its 16 bit pointer normal limitation.

Several parts of the display_bitmap function are commented out.  They can be deleted, but have been left for now to see how it used to work in case there are any issues to solve.

display_write_bitmap_byte has been updated in all of the screen driver files also to include the new flags byte - just need to update documentation

INVERT_USER_X_Y_COORDS added, so user can always work in X,Y.  Recomend adding defines for orientation to corect X,Y so 0,0 is always bottom left.

New features:- European character sets built in.  Text display functions simplified and much faster.  All fonts now proportional by design with reduced font lookup tables space requriement, unicode support possible (the display__string functions would just need to accept a word rather than a byte based string - everything else ready to go).  Supports the BitFontCreator Pro software package (not free) which makes creating new fonts and adjusting existing fonts a breeze.  Minimum width text width option to ensure previous text is completly overwritten without adding leading and trailing spaces or drawing blank box first.  Draw empty block option added - turn all pixels on or clear all pixels in a specified rectangle.

Invert block pixels - simply invert the existing display pixels in a specified rectangle

------------------------------------------------------------
----- V2.3.3 Bit Font Creator Pro Creating Fonts Notes -----
------------------------------------------------------------

Create a new file (doesn't matter what you choose as next step will overwrite size etc settings anyway)

Press 'Step 1 Import Font' button 
Select the font and style.  The size you select will determine the pixel size of the font.
Re-import until you get the character size you want.

Press 'Step 2 Edit Characters Table' button
The font you have selected is likely to provide characters in many character sets. Select the character set you want to use.
For UK only: Encoding = ASCII+, press 'ASCII'  for 0x20 - 0x7F
For European Encoding = ASCII+, press ANSI/ISO8859-1 for 0x20 to 0xFF).
(The option you select here will discard other characters and this is not reversible)
Notes:
You can't stop it not including characters before 0x20 - get over it!
The 'Append Char...' button only works for Unicode and allows you to specify a range of character codes to append to the end of the existing table (it isn't for setting the start and end characters)

Save the file

Remove any whitespace on the left and right of every character
Holding the CTRL key causes add and subtracts to occur on the right side.

Now go through all the characters and see how many unused pixels there are top and bottom.
Notes:
Typically a row or 2 at the bottom can be removed as the few characters that use it can be moved up.

The grey line shows the fonts base line, to help you see under line parts of characters.

If there are any characters that you'd like to move slightly or left right white space you would like to remove use the buttons on the left to do this.  (Save before trying anything risky).

Holding the CTRL key causes add and subtracts to occur on the right side.

Press 'Step 3 Change Font Height' button
This command literally clips or adds.  Once you use clip those pixels are gone - if you add later on the added rows will be blank (i.e. if you over clip you can't undo by using add, so get it right first time, or do sucessive clips.
This step may be necessary as when discarding characters we don't want (unicode, european etc) there may now be unused rows that we can get rid of. (Take a look at a capital letter with a top accent and a lowercase 'y' to quickly see if there is likely to be an unused row). Save before trying anything risky.

Press 'Step 4 Configure Data Format'
(This will depend on screen orientation)
Bit Order = Big Endian
Scan Based = Row / Column (depending on screen orientation - 0º=Row, 90º=Column)
Scan Prefered = [Same as Scan Based option]
Data Packed = No
Data Length = 8 bits

Press 'Step 5 Export Bitmap Data' button
Data format = C File (*.c)

Note for Embedded-Code Display Driver
You need to create both row or column based outputs (depending on orientation)


To Edit The Character Table
Use the "Edit Table..." button at the bottom right of the characters table - this will let you alter the table without it being recreated by the software causing previous edits to be lost.
