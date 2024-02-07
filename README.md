# Lights out
This is a small command-line game inspired by the Linear Algebra lecture I took at ETHZ. I wanted to make this game to get familiar with the C programming language and learn about some interesting correlations between Linear Algebra and the game. For Reference one can consult the Mathematics Magazine, Vol 71, Nr 4, published by Mathematical Association of America in 1998. The article is called *Turning Lights Out with Linear Algebra* - Marlow Anderson and Todd Feil.
# Game
The game consists of a 5 x 5 grid with values 1 (0) representing lights on (off). If one chooses to turn a light on (off) the lights of the 4 neighbours get flipped aswell. The goal is to turn off all the lights given an initial state. The initial state can either be constructed s. t. a optimal solution always exist or if one randomly chooses the grid with 1's and 0's there might not exist a solution at all.
# Linear Algebra connection
The grid can be represented as a single vector **b** constisting of 25 values either 1 or 0 representing lights on or off. There are exactly 25 different lights that can be flipped (respecting the rules of turning lights on or off). Each such light flipping can be represented by a vector of 25 values with exactly 4 ones in the correct locations and zeros otherwise. If a solution to the problem exist there must exist a unique way to pick some of the 25 vectors adding them up modulo 2 that gives us the vector **b**.
This is basically just solving a linear system of equation where the matrix **A** is 25 x 25 with the 25 different lights vectors as column, and we want to solve for **Ax = b** where **x** is the vector that tells us which of the columns from **A** one has to pick to get **b**. In other terms which lights one has to turn flip in order to get the initial grid. There is a small caveat that is we are not in the vector field over the real numbers but instead we are in the binary vector field where the arithmetic operation are the corresponding binary operation. But solving such a linear system of equation can be done through Gaussian Elimination isomorph to how we do it over the standard vector field.
# Todo
- Seperating the code
- Different frontend
- Minimal moves counter
- Other modes
