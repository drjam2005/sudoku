#include "raylib.h"
#include <cmath>
#include <time.h>
#include <set>

#define WIDTH 800
#define HEIGHT 600

struct Cell {
	size_t number;
};

struct Grid {
	Cell data[3][3];

	bool isValidGrid() {
		std::set<size_t> nums = {};
		for(size_t i = 0; i < 3; ++i){
			for(size_t j = 0; j < 3; ++j){
				if(data[i][j].number == 0)
					continue;
				if(nums.contains(data[i][j].number)){
					return false;
				}else
					nums.insert(data[i][j].number);
			}
		}

		return true;
	}
	
	bool isCompleteGrid(){
		std::set<size_t> nums = {
			1, 2, 3, 4, 5, 6, 7, 8, 9
		};
		for(size_t i = 0; i < 3; ++i){
			for(size_t j = 0; j < 3; ++j){
				if(nums.contains(data[i][j].number))
					nums.erase(data[i][j].number);
			}
		}
		return nums.empty();
	}
};

class Board {
public:
	Grid board[3][3];


	bool isValidBoard() {
		for(size_t gridRow = 0; gridRow < 3; ++gridRow){
			for(size_t gridCol = 0; gridCol < 3; ++gridCol){
				if(!board[gridRow][gridCol].isValidGrid())
					return false;
			}
		}

		// check all rows
		for(size_t r = 0; r < 9; ++r){
			std::set<size_t> nums = {};

			for(size_t c = 0; c < 9; ++c){
				size_t gridRow = r / 3;
				size_t gridCol = c / 3;
				size_t cellRow = r % 3;
				size_t cellCol = c % 3;

				size_t num = board[gridRow][gridCol].data[cellRow][cellCol].number;

				if(num == 0)
					continue;

				if(nums.contains(num))
					return false;

				nums.insert(num);
			}
		}

		// check all columns
		for(size_t c = 0; c < 9; ++c){
			std::set<size_t> nums = {};

			for(size_t r = 0; r < 9; ++r){
				size_t gridRow = r / 3;
				size_t gridCol = c / 3;
				size_t cellRow = r % 3;
				size_t cellCol = c % 3;

				size_t num = board[gridRow][gridCol].data[cellRow][cellCol].number;

				if(num == 0)
					continue;

				if(nums.contains(num))
					return false;

				nums.insert(num);
			}
		}

		return true;
	}
};

class Game {
	Board board;
	Vector2 selectedGrid = { -1.0f, -1.0f };
	Vector2 selectedCell = { -1.0f, -1.0f };
public:
	Game(Board& board) : board(board) {}
	void Render(){
		DrawRectangle(195, 95, 405, 405, LIGHTGRAY);
		if(selectedGrid.x >= 0){
			HighLightCell(selectedGrid.x, selectedGrid.y, selectedCell.x, selectedCell.y);
			char character = GetCharPressed();
			int num = TextToInteger(&character);
			if(num){
				board.board[(int)selectedGrid.x][(int)selectedGrid.y].data[(int)selectedCell.x][(int)selectedCell.y].number = num;
				selectedGrid.x = -1;
			}
		}
		for(size_t i = 1; i <= 3; ++i){
			DrawLine(195, 95+(i*(45*3)), 195+405, 95+(i*(45*3)), BLACK);
			DrawLine(195+(i*(45*3)), 95, 195+(i*(45*3)), 95+405, BLACK);
		}

		for(int r = 0; r < 3; ++r){
			for(int c = 0; c < 3; ++c){
				Grid& grid = board.board[r][c];
				for(int ir = 0; ir < 3; ++ir){
					for(int ic = 0; ic < 3; ++ic){
						DrawCell(grid.data[ir][ic].number, r, c, ir, ic);
					}
				}
			}
		}

		DrawText(TextFormat("%s", board.isValidBoard() ? "YES":"NO"), 20, 500, 20, GREEN);

		if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
			Vector2 mousePos = GetMousePosition();
			Vector2 boardPos = {
				mousePos.x - 195,
				mousePos.y - 95
			};

			selectedGrid = {
				std::floor(boardPos.x/135.0f),
				std::floor(boardPos.y/135.0f)
			};

			selectedCell  = {
				std::fmod(std::floor(boardPos.x/45.0f), 3.0f),
				std::fmod(std::floor(boardPos.y/45.0f), 3.0f)
			};
		}

		if(IsKeyPressed(KEY_SPACE)){
			GenerateFullBoard();
		}

		
	}
private:
	void DrawCell(size_t number, size_t GRow, size_t GCol, size_t row, size_t col){
		DrawText(TextFormat("%u", number),
				195 + (GRow*135) + (row * 45) + 10,
				95 +  (GCol*135) + (col * 45) + 10,
			20, BLACK);
	}
	void HighLightCell(size_t GRow, size_t GCol, size_t row, size_t col){
		DrawRectangle(
				195 + (GRow*135) + (row * 45),
				95 +  (GCol*135) + (col * 45),
				45, 45,
				WHITE
			);
			
	}

	void GenerateFullBoard(){
		srand(time(0));
		// holy naive approach
		for(size_t c = 0; c < 3; ++c){
			for(size_t r = 0; r < 3; ++r){
				for(size_t ic = 0; ic < 3; ++ic){
					for(size_t ir = 0; ir < 3; ++ir){
						int num;
						do {
							num = rand() % 9 + 1;
							board.board[c][r].data[ic][ir].number = num;
						}while(!board.isValidBoard());
					}
				}
			}
		}

	}
};


int main(){
	Board sudoku = {{
		{
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }},
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }},
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }}
		},
		{
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }},
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }},
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }}
		},
		{
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }},
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }},
			{{ {{0},{0},{0}}, {{0},{0},{0}}, {{0},{0},{0}} }}
		}
	}};
	

	Game game(sudoku);

  	InitWindow(WIDTH, HEIGHT, "Sudoku");
  	SetTargetFPS(60);
  	while(!WindowShouldClose()){
  		BeginDrawing();
  		ClearBackground(BLACK);
  
		game.Render();

  		EndDrawing();
  	}
  	CloseWindow();
}
