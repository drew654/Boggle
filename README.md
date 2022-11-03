# Boggle
It's Boggle, but in the command line.

Run program on Linux by using `./obj/main` in the root folder of the program, where `Collins_Scrabble_Words_2019.txt` and `Collins_Scrabble_Words_2019_with_definitions.txt` are located.

When solving the Boggle board, a thread is made for each of the dice in the board, recursively searching for every word in the board.
`boggle_thread_cursor_asciinema` is an asciinema recording of a Boggle board being solved (showing every half second) that has the output of `solution_PRIHTNFTHALECEMCNESERONZI.txt`.
<a href="https://asciinema.org/a/GaCdixxVbaGpHbX2JcUTiGsSg" target="_blank"><img src="https://asciinema.org/a/GaCdixxVbaGpHbX2JcUTiGsSg.svg" /></a>
