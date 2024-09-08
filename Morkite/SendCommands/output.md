// Servo with ID 2, what's your position? 


55 55 02 03 1C DE 

// My ID is 2, my position is 01F0
55 55 02 05 1C F0 01 EB 

// Servo ID 3, what's your position? 
55 55 03 03 1C DD 

// ID 3: My position is 01F6
55 55 03 05 1C F6 01 E4


// move to position 0? 
55 55 02 07 01 00 00 E8 03 0A 

// move to position 1000? 
55 55 02 07 01 E8 03 E8 03 1F 

// Report Position
55 55 02 03 1C DE 
// Position is 03E9
55 55 02 05 1C E9 03 F0



// MOV 00 00
55 55 02 07 01 00 00 E8 03 0A 

// Q POS?
55 55 02 03 1C DE 
// A POS = 00 00
55 55 02 05 1C 00 00 DC 

// MOV 03 E8
55 55 02 07 01 E8 03 E8 03 1F 

// Q POS?
55 55 02 03 1C DE 
// A POS = 03 EA
55 55 02 05 1C EA 03 EF





55 55 02 05 1C FE FF DF 

<MOV 03 E8>
55 55 02 07 01 DC 05 E8 03 29 
<POS ?>
55 55 02 03 1C DE 
<POS: 03 EB>
55 55 02 05 1C EB 03 EE 
