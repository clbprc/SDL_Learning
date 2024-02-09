#include <SDL.h>
#undef main
#include <vector>
#include <algorithm>
#include <deque>

int main()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	auto window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 500, 500, 0);
	auto renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event e;

	enum Direction
	{
		s,	//down
		a,	//left
		d,	//right
		w,	//up
	};
	
	//snake body
	std::deque<SDL_Rect> rq;
	int size = 1;


	//candy
	std::vector<SDL_Rect> candies;

	for (int i = 0; i < 50; i++)//gen candy @ random 
	{
		candies.emplace_back(rand() % 100 * 10, rand() % 100 * 10, 10, 10);
		if (i <= 19)	//testing if doing this will recreate an apple when one is eaten - if not i will create a seperate loop that checks for a decrease in apples every 5s
			i++;
	}			

	bool running = true;
	int dir = 0;
	SDL_Rect head{ 250,250,10,10 };

	while (running == true)
	{
		//input check
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT) { running = false; }
			if (e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == SDLK_s) { dir = s; }
				if (e.key.keysym.sym == SDLK_a) { dir = a; }
				if (e.key.keysym.sym == SDLK_d) { dir = d; }
				if (e.key.keysym.sym == SDLK_w) { dir = w; }
			}
		}

		//moving
		switch (dir)
		{
		case s:
			head.y += 10; break;	//if S is pressed, the y value is increased by 10
		case w:
			head.y -= 10; break;	//if A is pressed, the 
		case a:
			head.x -= 10; break;
		case d:
			head.x += 10; break;
		}

		//check for collision [apple]
		std::for_each(candies.begin(), candies.end(), [&](auto& candy)
			{
				if (head.x == candy.x && head.y == candy.y)
				{
					size += 1;
					candy.x = - 10;
					candy.y = - 10;
				}
			});

		//check for collison [self]
		std::for_each(rq.begin(), rq.end(), [&](auto& snake_part)
			{
				if (head.x == snake_part.x && head.y == snake_part.y)
				{
					size = 1;
				}
			});
		//move forward
		rq.push_front(head);

		while (rq.size() > size)
			rq.pop_back();
		

		/*if (head.y == 1000 || head.y == -1000 || head.x == 1000 || head.x == -1000) //stop from leaving screen v1
		{
			head.y = 500;
			head.x = 500;
		}*/
		//clear window
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		//draw snake
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		std::for_each(rq.begin(), rq.end(), [&](auto& snake_body)
			{
				SDL_RenderFillRect(renderer, &snake_body);
			});
		//SDL_RenderFillRect(renderer, &head);
		
		//draw candy
		SDL_SetRenderDrawColor(renderer, 200, 70, 100, 255);
		std::for_each(candies.begin(), candies.end(), [&](auto& candy)
			{
				SDL_RenderFillRect(renderer, &candy);
			});


		//render screen
		SDL_RenderPresent(renderer);
		SDL_Delay(25);

	}
	
	return 0;
}