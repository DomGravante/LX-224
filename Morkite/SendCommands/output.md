
55 55 FE 04 0D 02 EE      # Set IDs
55 55 02 03 1C DE         # Read Position: Command
55 55 02 05 1C F0 01 EB   # Read Position: Response




# Prove you can communicate with the servo!

## Success on

00:18:53.380 -> 55 55 03 03 1C DD 55 55 03 05 1C EF 01 EB RX1 Data
00:18:53.420 -> 55 55 03 03 1C DD 55 55 03 05 1C EF 01 EB RX2 Data
00:18:54.388 ->
00:18:54.837 -> 55 55 03 03 1C DD 55 55 03 05 1C F0 01 EA RX1 Data
00:18:54.837 -> 55 55 03 03 1C DD 55 55 03 05 1C F0 01 EA RX2 Data

## Response split up between commands

55 55 03 03 1C DD # Get Position
55 55 03 05 1C EF 01 EB # Return Position
55 55 03 05 1C F0 01 EA # Return position, a bit later

## RESPONSE Broken Down

0 55 HEADER1
1 55 HEADER2
2 03 ID
3 05 DATALEN
4 1C CMD
5 EF Pos1 (sometimes F0)
6 01 Pos2
7 EB Checksum (Sometimes EA)

## Position Analysis

So our position bytes are:

EF Lower 8 bits of position
01 Higher 8 bits of position

This means that our position is:
- 01EF
- 0000000111101111

- What does this mean? 