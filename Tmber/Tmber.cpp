//Підключаємо важливі бібліотеки
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>



//Спрощуємо використання кода за допомогою "using namespace",
//визначающего простір імен
using namespace sf;

//Обявляємо функцію
void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//Де знаходиться герой/гілки?
//Зліва чи справа
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];


// Start
int main()

{
	//Створюэмо обэкт VideoMode
	VideoMode vm(1928, 1080);
	//Створюємо і відкриваємо вікно для гри в повноекранному режимі
	RenderWindow window(vm, "Timber!", Style::Fullscreen);

	//Створюэмо текстру для зберігання зображення в GPU
	Texture textureBackground;

	//Завантажуємо текстуру
	textureBackground.loadFromFile("graphics/background.png");

	//Створюємо спрайт
	Sprite spriteBackground;

	//Привязуємо текстуру до спрайта
	spriteBackground.setTexture(textureBackground);

	//Ставимо позицію spriteBackground, щоб вона покрила весь екран 
	spriteBackground.setPosition(0, 0);

	//Створюємo спрайт дерева
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	//Створюємо бджолу
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);	

	//Рухається бжлола чи ні?
	bool beeActive = false;

	//З якою швидікостю може рухатись бджола?
	float beeSpeed = 0.0f;

	//Cтворюємо три спрайта хмаринок із однієї текстури
	Texture textureCloud;

	//Завантажуэмо одну нову текстуру
	textureCloud.loadFromFile("graphics/cloud.png");

	//Три нових спрайта з однієї текстури
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);	
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	//Позиціонуємо хмаринки на різних висотах
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 259);
	spriteCloud3.setPosition(0, 500);

	//Виясняємо, чи рухається хмаринка в даний момент
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//З якою швидкістю рухається кожна хмаринка
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//Змінні для управління часом
	Clock clock;

	//Шкала часу
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920.f / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//Відстежуємо стан гри (на паузі чи ні)
	bool paused = true;

	//Відмальовуємо шрифт 
	int score = 0;

	Text messageText;
	Text scoreText;

	//Вибираємо шривфт 
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//Встановлювємо шрифт  повідомлення
	messageText.setFont(font);
	scoreText.setFont(font);

	//Встановлюємо текст для повідомлення
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0");

	//Встановлюємо розмір повідомлення
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Вибираємо колір 
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Позиціонуємо текст
	FloatRect textRect = messageText.getLocalBounds();

	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	//Готуємо пять гілок
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	//Розміщуємо текстури для кожного спрайта
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);

		//Задаємо точку привязки спрайта
		//Це дозволить нам обертати його, не змінюючи його положення
		branches[i].setOrigin(220, 20);
	}

	//Підготовка ігрового персонажу 
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(589, 720);

	//Гравець починає з лівої сторони
	side playerSide = side::LEFT;

	//Підготовка памятника
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//Підготовка сокири
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spritAxe;
	spritAxe.setTexture(textureAxe);
	spritAxe.setPosition(700, 830);

	//Вирівнюємо сокиру відносно дерева
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Підготовка відлітаючого куска дерева
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	//Деякі інші корисні змінні звязані з деревом
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//Керування вводом ігравця
	bool acceptInput = false;

	// Підготовка звуків
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	// Час вийшов
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);



	while (window.isOpen())
	{
		/*
		********************************************
			      Обробка введення ігрока
		********************************************
		*/

		Event event;
		
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				// Знову "слухаємо" натискання клавіш
				acceptInput = true;

				// Сховати сокиру
				spritAxe.setPosition(2000, spritAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Старт ігри (тільки коли зараз пауза)
		if (Keyboard::isKeyPressed(Keyboard::Return) && paused)
		{
			paused = false;

			// Скидаємо час та рахунок тільки при старті нової гри
			score = 0;
			timeRemaining = 6.0f;
			timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			// Скидаємо таймер, щоб dt не включав час паузи
			clock.restart();

			//Забираємо усі гілки
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			//Ховаємо памятник
			spriteRIP.setPosition(675, 2000);

			//Переміщуємо гравця на початкову позицію
			spritePlayer.setPosition(580, 720);

			acceptInput = true;
		}
		// Обертаємо елементи управління персонажем,
		// щоб переконатися, що ми приймаємо введення
		if (acceptInput)
		{
			// Спочатку обробляємо натискання клавіші зі стрілкою вправо
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				// Переконуємося, що персонаж знаходиться праворуч
				playerSide = side::RIGHT;

				score++;

				// Додаємо додатковий час
				timeRemaining += (2 / score) + .15;

				spritAxe.setPosition(AXE_POSITION_RIGHT, spritAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				// Оновлюємо гілки
				updateBranches(score);

				// Відлітаючий кусок дерева в ліву сторону 
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				// Відтворення звуку удару
				chop.play();

			}

			// Обробка натискання клавіші зі стрілкою вліво
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				// Переконуємося, що персонаж знаходиться ліворуч
				playerSide = side::LEFT;

				score++;

				// Додаємо додатковий час
				timeRemaining += (2 / score) + .15;

				spritAxe.setPosition(AXE_POSITION_LEFT, spritAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				// Оновлюємо гілки
				updateBranches(score);

				// Відлітаючий кусок дерева в ліву сторону
				spriteLog.setPosition(810, 720);

				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;

				// Відтворення звуку удару
				chop.play();

			}
		}

		/*
		********************************************
				  Обновлення сцени
		********************************************
		*/

		if (!paused) {

		

		//Зміна часу
		Time dt = clock.restart();

		//Зменшуємо залишок часу
		timeRemaining -= dt.asSeconds();
		//Міняємо розмір шкали часу
		timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

		if (timeRemaining <= 0.0f) {
			paused = true;

			//Показуємо повідомлення про те що час вийшов
			messageText.setString("Out of time!");

			//Перераховуємо позицію тексту на основі його нового розміру
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

			messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			// Відтворюємо звук закінчення часу
			outOfTime.play();
		}

		//Галаштування бджоли
		if (!beeActive)
		{

			//Задаємo скороість бджоли
			srand((int)time(0));
			beeSpeed = (rand() % 200) + 200;

			//Задаємо висоту польоту бджоли 
			srand((int)time(0) * 10);
			float height = (rand() % 500) + 500;
			spriteBee.setPosition(2000, height);
			beeActive = true;
		}
		else
			//Рухаємо бджолу
		{
			spriteBee.setPosition(
				spriteBee.getPosition().x -
				(beeSpeed * dt.asSeconds()),
				spriteBee.getPosition().y);

			//Перевіряємо, чи бджола вилетіла за межі екрану
			if (spriteBee.getPosition().x < -100)
			{
				//Готуємо бджолу до появи як нової в наступному кадрі
				beeActive = false;
			}
		}

		//Керуємо хмаринками 
		//Хмаринка 1
		if (!cloud1Active)
		{

			//Задаэмо скороість хмаринки
			srand((int)time(0) * 10);
			cloud1Speed = (rand() % 200);

			//Задаємо висоту польоту хмаринки
			srand((int)time(0) * 10);
			float height = (rand() % 150);
			spriteCloud1.setPosition(-200, height);
			cloud1Active = true;

		}
		else
		{
			spriteCloud1.setPosition(
				spriteCloud1.getPosition().x +
				(cloud1Speed * dt.asSeconds()),
				spriteCloud1.getPosition().y);

			//Перевіряємо, чи хмаринка вилетіла за межі екрану
			if (spriteCloud1.getPosition().x > 1920)
			{
			//Готуємо хмаринку до появи як нової в наступному кадрі
				cloud1Active = false;
			}
		}
		//Хмаринка 2
		if (!cloud2Active)
		{
			//Задаэмо скороість хмаринки
			srand((int)time(0) * 20);
			cloud2Speed = (rand() % 200);

			//Задаємо висоту польоту хмаринки
			srand((int)time(0) * 20);
			float height = (rand() % 300) - 150;
			spriteCloud2.setPosition(-200, height);
			cloud2Active = true;
		}
		else
		{
			spriteCloud2.setPosition(
				spriteCloud2.getPosition().x +
				(cloud2Speed * dt.asSeconds()),
				spriteCloud2.getPosition().y);

			//Перевіряємо, чи хмаринка вилетіла за межі екрану
			if (spriteCloud2.getPosition().x > 1920)
			{
				//Готуємо хмаринку до появи як нової в наступному кадрі
				cloud2Active = false;
			}
		}

		//Хмаринка 3
		if (!cloud3Active)
		{
			//Задаємo скороість хмаринки
			srand((int)time(0) * 30);
			cloud3Speed = (rand() % 200);

			//Задаємо висоту польоту хмаринки
			srand((int)time(0) * 30);
			float height = (rand() % 450) - 150;
			spriteCloud3.setPosition(-200, height);
			cloud3Active = true;
		}
		else
		{
			spriteCloud3.setPosition(
				spriteCloud3.getPosition().x +
				(cloud3Speed * dt.asSeconds()),
				spriteCloud3.getPosition().y);
			//Перевіряємо, чи хмаринка вилетіла за межі екрану
			if (spriteCloud3.getPosition().x > 1920)
			{
				//Готуємо хмаринку до появи як нової в наступному кадрі
				cloud3Active = false;
			}
		}

		//Обновляємо тест рахунку
		std::stringstream ss;
		ss << "Score = " << score;
		scoreText.setString(ss.str());
		//ss << "Score: " << score; scoreText.setString(ss.str());

		//Оновлення спрайтів гілок
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			float height = i * 150;

			if (branchPositions[i] == side::LEFT)
			{
				//Рухаємо гілку в ліву сторону
				branches[i].setPosition(610, height);

				//Перевертаємо спрайт в інший бік
				branches[i].setRotation(180);
			}
			else if (branchPositions[i] == side::RIGHT)
			{
				//Переміщуємо спрайт у правий бік
				branches[i].setPosition(1330, height);

				//Перевертаємо спрайт в інший бік
				branches[i].setRotation(0);
				
			}
			else {
				//Приховуємо гілку
				branches[i].setPosition(3000, height);
			}
		}
		//Обновляємо відлітаючий кусок дерева
		if (logActive)
		{
			spriteLog.setPosition(
				spriteLog.getPosition().x +
				(logSpeedX * dt.asSeconds()),

				spriteLog.getPosition().y +
				(logSpeedY * dt.asSeconds()));

			// Чи досягла колода правого краю?
			if (spriteLog.getPosition().x < -100 ||
				spriteLog.getPosition().x > 2000)
			{
				// Готуємо його до появи як нової колоди в наступному кадрі
				logActive = false;
				spriteLog.setPosition(810, 720);
			}
		}

		// Чи був персонаж розчавлений гілкою?
		if (branchPositions[5] == playerSide)
		{
			// Смерть
			paused = true;
			acceptInput = false;

			// Відображення надгробка
			spriteRIP.setPosition(525, 760);

			// Приховуємо персонажа 
			spritePlayer.setPosition(2000, 660);

			// Змінюємо текст повідомлення
			messageText.setString("CRUSHED!");

			// Центруємо повідомлення
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);

			messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

			// Відтворюємо звук, що означає загибель персонажа
			death.play();
		}

		}  // К інекць блока if(!paused)

		/*
		********************************************
				  Відмальовування сцени
		********************************************
		*/

		//Очищаэмо все з попереднього кадру
		window.clear();

		//Відмалюємо тут нашу ігрову сцену
		window.draw(spriteBackground);

		//Відмальовуємо хмаринки
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		//Відмальовування гілок
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}

		//Відмальовуємо дерево
		window.draw(spriteTree);

		//Відмальовуємо перонажа
		window.draw(spritePlayer);

		//Відмальовуємо сокиру
		window.draw(spritAxe);

		//Відмальовуємо відлітаючий кусок дерева
		window.draw(spriteLog);

		//Відмальовуємо памятник
		window.draw(spriteRIP);

		//Відмальовуємо бджолу
		window.draw(spriteBee);

		//Відмальовуємо рахунок
		window.draw(scoreText);

		//Відмальовуємо шкалу часу
		window.draw(timeBar);

		if (paused) 
		{
		//Відмальовуємо повідомлення 
			window.draw(messageText);

		}

		//Відамлюємо все, що було намальовано
		window.display();
	}

	return 0;
}

//Визначення функції
void updateBranches(int seed)
{
	//Перемістіть усі гілки вниз на одне місце
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}
	//Створюємо нову гілку 0
	//LEFT, RIGHT или NONE
	srand((int)time(0)+seed);
	int r = (rand() % 5);

	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
	}

}

//// 168ст 