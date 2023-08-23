# Boggle
It's Boggle, but in the command line.

Run program on Linux by using `./boggle` in the root folder of the program, where `Collins_Scrabble_Words_2019_with_definitions.txt` is located.

On the main title screen, there are options to play the game, view the rules, solve an existing board inputted by the user, define a word, and exit the program.

![image](https://user-images.githubusercontent.com/55492673/211369859-0a610021-6d31-4f29-8ae1-11486fa02322.png)

When playing the game, the player types in a word they have found on the Boggle board and that word is added to the list of inputted words. Because the game does not know where the word was found, it has to find it in the board to check if it is a correct word. The game not knowing what string of letters chosen is a problem always present in a command line implementation of Boggle, but I wanted to do this anyways. Once a word is in the list of inputted words, it has a separate set of threads looking for those words in the board so that the player can keep typing and adding new words. Once the word has been found in the board or to not be in the board by the game, it sorts that word accordingly and the display is updated with the next word inputted by the player. The game clock refreshes its display for each input of a new word so that the player is not cut off when typing. The clock is always counting in the background.

![image](https://user-images.githubusercontent.com/55492673/211370438-9bc4a465-9210-4a69-bf62-400c5dcc8a6e.png)

When the timer runs out, the game finishes sorting the inputted words while not accepting new inputted words from the player.

![image](https://user-images.githubusercontent.com/55492673/211370581-a3e27be3-429d-42e2-a2bd-29533a88ce0c.png)

At the end of sorting words, the game displays the player's final score and gives the option to solve the game, finding every word in every possible location on the board.

![image](https://user-images.githubusercontent.com/55492673/211370700-672ef08c-b5e1-40bb-9526-0e22c3787a28.png)

When solving the Boggle board, a thread is made for each of the dice in the board, recursively searching for every word in the board.

![image](https://user-images.githubusercontent.com/55492673/211370787-650f0bb3-40d9-448b-9115-7d036729e275.png)

At the end of finding every word in the board, the solution to that game is outputted as `solution_<board name>.txt`. In this case, the board is called `ENETHOTITMRVNTONEEBARSEYC` and its solution file name is called `solution_ENETHOTITMRVNTONEEBARSEYC.txt`. `ENETHOTITMRVNTONEEBARSEYC` is what can be inputted by the player for the "solve board" option of the title screen to solve that board without playing through the game.

![image](https://user-images.githubusercontent.com/55492673/211376339-79c7991a-63ca-4264-8a16-07d44e42c8b0.png)

Here is a [demo video](https://youtu.be/SqJSmkqrkHI) of the game running.
