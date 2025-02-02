﻿#include "../header/Game.h"
#include "../header/raygui.h"

Game::Game()
{
	charAnim = std::vector<std::vector<Texture2D*>>(4, std::vector<Texture2D*>(4));
	for (int i = 0; i < 4; i++)
	{
		charAnim[0][i] = &TextureHolder::getHolder().get((Textures::ID)(i + 17));
		charAnim[1][i] = &TextureHolder::getHolder().get((Textures::ID)(i + 21));
		charAnim[2][i] = &TextureHolder::getHolder().get((Textures::ID)(i + 25));
		charAnim[3][i] = &TextureHolder::getHolder().get((Textures::ID)(i + 29));
	}
	gameRight = &TextureHolder::getHolder().get(Textures::GAMERIGHT);
	restart_button = &TextureHolder::getHolder().get(Textures::RESTART_BTN);
	pause_button = &TextureHolder::getHolder().get(Textures::PAUSE_BTN);
	music_button = &TextureHolder::getHolder().get(Textures::MUSIC_BTN);
	resume_button = &TextureHolder::getHolder().get(Textures::RESUME_BTN);
	home_button = &TextureHolder::getHolder().get(Textures::HOME_BTN);
	next_button = &TextureHolder::getHolder().get(Textures::NEXT_BTN);
	blurImage = &TextureHolder::getHolder().get(Textures::BLUR_BG);
	pauseMenu = &TextureHolder::getHolder().get(Textures::PAUSE_MENU);
	victoryMenu = &TextureHolder::getHolder().get(Textures::VICTORY_MENU);
	loseMenu = &TextureHolder::getHolder().get(Textures::LOSE_MENU);
	backButton = nextButton = false;
	startTime = 0;
	lastBestScore = lastBestLevel = 0;
	lastBestTime = 0;
	pauseState = false;
	isWin = false;
	isLose = false;
}

Game::~Game()
{
	
}

Screen Game::update()
{
	// Stopping event in game
	if (isWin || pauseState)
	{
		startTime = 0;
		curPlayer.timeIncrease(playTime);
		playTime = 0;
		return GAME;
	}

	if (isLose) {
		if (IsKeyDown(KEY_ENTER)) {
			curPlayer = Player(curPlayer.getName());
			curPlayer.setBestLevel(lastBestLevel);
			curPlayer.setBestScore(lastBestScore);
			curPlayer.setBestTime(lastBestTime);
			sortListPlayer();
			startTime = 0;
			playTime = 0;
			allLane.clear();
			isLose = false;
			return HOME;
		}
		return GAME;
	}

	// Create next level
	if (nextButton)
	{
		curPlayer.setCurdirection(0);
		curPlayer.setScreenRec({ 426, 0, 44, 59 });
		curPlayer.setPreTime(curPlayer.getTime());
		nextButton = false;
		int level = curPlayer.getLevel() + 1;
		curPlayer.setLevel(level);
		allLane.clear();
		allLane = random(level);
		listPlayer[curPlayer.getPosInList()] = curPlayer;
	}

	if (backButton)
	{
		startTime = 0;
		curPlayer.timeIncrease(playTime);
		listPlayer[curPlayer.getPosInList()] = curPlayer;
		backButton = false;
		return HOME;
	}
	
	// Create new level 1
	if (!allLane.size())
	{
		/*for (int i = 0; i < listPlayer.size(); i++) {
			std::cout << "Name: " << listPlayer[i].getName() << std::endl
				<< "Level: " << listPlayer[i].getLevel() << std::endl
				<< "Time: " << listPlayer[i].getTime() << std::endl
				<< "Score: " << listPlayer[i].getScore() << std::endl
				<< "Best Level: " << listPlayer[i].getBestLevel() << std::endl
				<< "Best Time: " << listPlayer[i].getBestTime() << std::endl
				<< "Best Score: " << listPlayer[i].getBestScore() << std::endl;
		}*/
		allLane = random(1);
		curPlayer.setScreenRec({ 426, 0, 44, 59 });
		curPlayer.setTime(0);
		curPlayer.setPreTime(0);
		curPlayer.setCurdirection(0);
		curPlayer.setLevel(1);
		curPlayer.setScore(0);
	}

	// Count Time
	if (startTime == 0) startTime = GetTime();
	playTime = GetTime() - startTime;

	// Lose Function 
	isLose = checkLose();

	// Win function
	isWin = checkWin();

	// Event Keyboard
	curPlayer.eventKeyboard();

	return GAME;
}

void Game::draw()
{
	renderAllLane(isWin, pauseState, isLose);	// Drawing lanes

	drawPlayerState();					// Drawing current detail of player

	drawRightMenu();					// Drawing detail menu

	drawButtons();						// Drawing buttons

	if (pauseState == true)   drawPauseMenu();  // Drawing paused state
	
	if (isWin == true)	drawVictoryMenu();		// Drawing passed menu

	if (isLose == true) drawLoseMenu();			// Drawing lose menu
}

void Game::drawRightMenu() {
	DrawTexture(*gameRight, 961, 0, WHITE);
	DrawText(curPlayer.getName().c_str(), 1150, 185, 33, BROWN);
	DrawText(TextFormat("%i", curPlayer.getLevel()), 1150, 100, 33, BROWN);
	DrawText(TextFormat("%i", curPlayer.getScore()), 1150, 355, 33, BROWN);
	DrawText(TextFormat("%.2lf", curPlayer.getTime() + playTime), 1150, 270, 33, BROWN);
}

void Game::drawButtons() {
	Vector2 getMouse = GetMousePosition();
	int restartX = 1000, restartY = 600, pauseX = 1100, pauseY = 600, musicX = 1200, musicY = 600;
	/*if (getMouse.x >= restartX && getMouse.x <= restartX + restart_button->width && getMouse.y >= restartY && getMouse.y <= restartY + restart_button->height)
	{
		DrawTexture(*restart_button, restartX, restartY, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
		}
	}
	else
	{
		DrawTexture(*restart_button, restartX, restartY, BROWN);
	}*/

	if (getMouse.x >= pauseX && getMouse.x <= pauseX + pause_button->width && getMouse.y >= pauseY && getMouse.y <= pauseY + pause_button->height)
	{
		DrawTexture(*pause_button, pauseX, pauseY, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
		{
			pauseState = true;
		}
	}
	else
	{
		DrawTexture(*pause_button, pauseX, pauseY, BROWN);
	}

	/*if (getMouse.x >= musicX && getMouse.x <= musicX + music_button->width && getMouse.y >= musicY && getMouse.y <= musicY + music_button->height)
	{
		DrawTexture(*music_button, musicX, musicY, RED);
	}
	else
		DrawTexture(*music_button, musicX, musicY, BROWN);*/
}

void Game::drawPauseMenu() {
	float pauseMenuX = 640 - pauseMenu->width / 2;
	float pauseMenuY = 360 - pauseMenu->height / 2;
	DrawTexture(*blurImage, 0, 0, CLITERAL(Color){ 255, 255, 255, 200 });
	DrawTexture(*pauseMenu, pauseMenuX, pauseMenuY, RAYWHITE);
	float resumeButtonX = pauseMenuX + pauseMenu->width / 4 - resume_button->width / 2;
	float resumeButtonY = pauseMenuY + 170;
	float homeButtonX = pauseMenuX + 3 * pauseMenu->width / 4 - home_button->width / 2;
	float homeButtonY = pauseMenuY + 170;
	Vector2 mousePos = GetMousePosition();
	if (mousePos.x >= resumeButtonX && mousePos.x <= resumeButtonX + resume_button->width && mousePos.y >= resumeButtonY && mousePos.y <= resumeButtonY + resume_button->height) {
		DrawTexture(*resume_button, resumeButtonX, resumeButtonY, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			pauseState = false;
		}
	}
	else DrawTexture(*resume_button, resumeButtonX, resumeButtonY, RAYWHITE);

	if (mousePos.x >= homeButtonX && mousePos.x <= homeButtonX + home_button->width && mousePos.y >= homeButtonY && mousePos.y <= homeButtonY + home_button->height) {
		DrawTexture(*home_button, homeButtonX, homeButtonY, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			pauseState = false;
			backButton = true;
		}
	}
	else DrawTexture(*home_button, homeButtonX, homeButtonY, RAYWHITE);
}

void Game::drawVictoryMenu() {
	float victoryMenuX = 640 - victoryMenu->width / 2;
	float victoryMenuY = 360 - victoryMenu->height / 2;
	DrawTexture(*blurImage, 0, 0, CLITERAL(Color){ 255, 255, 255, 200 });
	DrawTexture(*victoryMenu, victoryMenuX, victoryMenuY, RAYWHITE);
	float nextButtonX = victoryMenuX + victoryMenu->width / 4 - next_button->width / 2;
	float nextButtonY = victoryMenuY + 150;
	float homeButtonX = victoryMenuX + 3 * victoryMenu->width / 4 - home_button->width / 2;
	float homeButtonY = victoryMenuY + 150;
	Vector2 mousePos = GetMousePosition();
	if (mousePos.x >= nextButtonX && mousePos.x <= nextButtonX + next_button->width && mousePos.y >= nextButtonY && mousePos.y <= nextButtonY + next_button->height) {
		DrawTexture(*next_button, nextButtonX, nextButtonY, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			isWin = false;
			nextButton = true;
			pauseState = false;
		}
	}
	else DrawTexture(*next_button, nextButtonX, nextButtonY, RAYWHITE);

	if (mousePos.x >= homeButtonX && mousePos.x <= homeButtonX + home_button->width && mousePos.y >= homeButtonY && mousePos.y <= homeButtonY + home_button->height) {
		DrawTexture(*home_button, homeButtonX, homeButtonY, RED);
		if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			isWin = false;
			nextButton = true;
			pauseState = false;
			backButton = true;
		}
	}
	else DrawTexture(*home_button, homeButtonX, homeButtonY, RAYWHITE);

	DrawText(TextFormat("Score:        %i", curPlayer.getScore()), victoryMenuX + 150, victoryMenuY + 330, 55, DARKGRAY);
}

void Game::drawLoseMenu() {
	float loseMenuX = 640 - victoryMenu->width / 2;
	float loseMenuY = 360 - victoryMenu->height / 2;
	DrawTexture(*blurImage, 0, 0, CLITERAL(Color){ 255, 255, 255, 200 });
	DrawTexture(*loseMenu, loseMenuX, loseMenuY, RAYWHITE);
	DrawText(TextFormat("Score :        %i", curPlayer.getScore()), loseMenuX + 210, loseMenuY + 160, 40, DARKGRAY);
	if (curPlayer.getScore() == curPlayer.getBestScore()) {
		DrawText(TextFormat("Rank :        %i", curPlayer.getPosInList() + 1), loseMenuX + 210, loseMenuY + 240, 40, DARKGRAY);
	}
	else {
		DrawText(TextFormat("Best Score :        %i", curPlayer.getBestScore()), loseMenuX + 210, loseMenuY + 240, 40, DARKGRAY);
	}
	DrawText("PRESS ENTER TO RETURN HOME", loseMenuX + 54, loseMenuY + 350, 40, CLITERAL(Color){80, 80, 80, 200});
}

void Game::drawPlayerState()
{
	curPlayer.render(charAnim);
}

bool Game::checkLose()
{
	for (int i = 0; i < (int)allLane.size(); i++)
	{
		for (int j = 0; j < (int)allLane[i].getObstacles().size(); ++j)
		{
			if (CheckCollision(curPlayer, allLane[i].getObstacles()[j]))
			{
				startTime = 0;
				playTime = 0;
				listPlayer[curPlayer.getPosInList()].setScore(0);
				listPlayer[curPlayer.getPosInList()].setLevel(1);
				listPlayer[curPlayer.getPosInList()].setPreTime(0);
				listPlayer[curPlayer.getPosInList()].setTime(0);
				lastBestScore = curPlayer.getBestScore();
				lastBestLevel = curPlayer.getBestLevel();
				lastBestTime = curPlayer.getBestTime();
				sortListPlayer();
				return true;
			}
		}
	}
	return false;
}

bool Game::checkWin()
{
	if (curPlayer.getScreenRec().y > allLane[allLane.size() - 1].getScreenPos().y)
	{
		startTime = 0;
		curPlayer.timeIncrease(playTime);
		playTime = 0;
		curPlayer.addScore(curPlayer.calScore());
		/*std::cout << "Score here: " << curPlayer.getScore() << std::endl;
		std::cout << "Best: " << curPlayer.getBestScore() << std::endl;*/
		curPlayer.updateBestScore();
		/*std::cout << "Score here: " << curPlayer.getScore() << std::endl;
		std::cout << "Best: " << curPlayer.getBestScore() << std::endl;*/
		listPlayer[curPlayer.getPosInList()].setScore(curPlayer.getScore());
		listPlayer[curPlayer.getPosInList()].setLevel(curPlayer.getLevel() + 1);
		listPlayer[curPlayer.getPosInList()].setPreTime(curPlayer.getTime());
		listPlayer[curPlayer.getPosInList()].setTime(curPlayer.getTime());
		listPlayer[curPlayer.getPosInList()].updateBestScore();
		/*for (int i = 0; i < listPlayer.size(); i++) {
			std::cout << "Name: " << listPlayer[i].getName() << std::endl
				<< "Level: " << listPlayer[i].getLevel() << std::endl
				<< "Time: " << listPlayer[i].getTime() << std::endl
				<< "Score: " << listPlayer[i].getScore() << std::endl
				<< "Best Level: " << listPlayer[i].getBestLevel() << std::endl
				<< "Best Time: " << listPlayer[i].getBestTime() << std::endl
				<< "Best Score: " << listPlayer[i].getBestScore() << std::endl;
		}*/
		sortListPlayer();
		/*for (int i = 0; i < listPlayer.size(); i++) {
			std::cout << "Name: " << listPlayer[i].getName() << std::endl
				<< "Level: " << listPlayer[i].getLevel() << std::endl
				<< "Time: " << listPlayer[i].getTime() << std::endl
				<< "Score: " << listPlayer[i].getScore() << std::endl
				<< "Best Level: " << listPlayer[i].getBestLevel() << std::endl
				<< "Best Time: " << listPlayer[i].getBestTime() << std::endl
				<< "Best Score: " << listPlayer[i].getBestScore() << std::endl;
		}*/
		return true;
	}
	return false;
}

bool CheckCollision(Player& p, Obstacle  &ob)
{
	Rectangle temp1 = ob.getScreenRec();
	Rectangle temp2 = curPlayer.getScreenRec();
	temp1.y = allLane[ob.getInLane()].getScreenPos().y + 15;
	temp2.y += temp2.height * 4 / 5;
	temp2.height /= 5;

	temp1.width = ob.getScreenRec().width;
	temp1.height = ob.getScreenRec().height;

	temp1.y += temp1.height * 0.5;
	temp1.height /= 2;
	if (CheckCollisionRecs(temp2, temp1))
	{
		return true;
	}
	return false;
}

