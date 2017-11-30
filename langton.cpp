/*
writer: MatioMe
date: 21/05/2017

This code is a fast written code to experiment the drawings you can obtain
with more colors and other configurations than with the basic two color algorithm of the Langton ant.

see this page to learn more about SFML and how to install it on your system:
https://www.sfml-dev.org/tutorials/2.4/start-linux.php

compile and launch on linux with this command line:
g++ -c langton.cpp && g++ langton.o -o langton.app -lsfml-graphics -lsfml-window -lsfml-system && ./langton.app
*/

#define MAX_COLOR_QTY 30

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h> 
#include <time.h>


struct Position
{
  int x;
  int y;
  int orientation; //0:N 1:E 2:S 3:O
};

void turnLeft(Position &posAnt);
void turnRight(Position &posAnt);
void initColors(sf::Color *color);



int main()
{
	srand(time(0));

	int DIM_X=700, DIM_Y=700;

	int numberOfColor=0;
	int colorOrientation[MAX_COLOR_QTY]={0};

	//user interface:
  std::cout<<"\n            ##############################"<< std::endl;
  std::cout<<"            ## Multicolor Langton's Ant ##"<< std::endl;
  std::cout<<"            ##############################\n"<< std::endl;
  std::cout<<"                  '\\__\n                   (o )     ___\n                   <>(_)(_)(___)\n                     < < > >\n                     ' ' ` `"<<std::endl;
  std::cout<<"\n            ##############################"<< std::endl;
  std::cout<<"            ## Author: matioMe          ##"<< std::endl;
  std::cout<<"            ## Date: 21/05/2017         ##"<< std::endl;
  std::cout<<"            ## Version: 1.0             ##"<< std::endl;
  std::cout<<"            ##############################\n\n"<< std::endl;
  
  std::cout<<"         #####################################"<< std::endl;
  std::cout<<"         # website: https://www.mathware.eu/ #"<< std::endl;
  std::cout<<"         #####################################\n\n"<< std::endl;
  

  do
	{
		std::cout<<"Please, choose a number of color between 2 and 30: "<< std::endl;
		std::cin>>numberOfColor;

	}
	while(numberOfColor<2 || numberOfColor>30);

	numberOfColor--;

	bool randomize=0;

	for(int i=0; i<numberOfColor+1; i++)
	{
		char currentLetter;

		if(randomize!=1)
		{
			std::cout << "\n\nChoose color " << i+1 << " direction ('L' for left, 'R' for right)" << std::endl;
			if(i==0)
			{
				std::cout << "you can choose 'S' at any time to randomize the remaining directions" << std::endl;
			}
			std::cin>>currentLetter;

			if(currentLetter=='L' || currentLetter=='l' || currentLetter==0)
			{
				colorOrientation[i]=0;
			}
			if(currentLetter=='R' || currentLetter=='r' || currentLetter==1)
			{
				colorOrientation[i]=1;
			}
			if(currentLetter=='S' || currentLetter=='s')
			{
				randomize=1;
			}
			if(currentLetter!='S' && currentLetter!='R' && currentLetter!='L' && currentLetter!='s' && currentLetter!='r' && currentLetter!='l')
			{
				i--;
				std::cout<<"invalid value."<< std::endl;
			}

		}
		else
		{
			colorOrientation[i]=rand()%2;
		}
	}

	char chooseDim='n';
	std::cout<<"\n\ndo you want to choose screen size? (Y/n)"<< std::endl;
	std::cin>>chooseDim;
	
	if(chooseDim=='Y' || chooseDim=='y')
	{
		do
		{
			std::cout<<"\nX and Y square dimension? (10 min, 1024 max)"<< std::endl;
			std::cin>>DIM_Y;
			DIM_X=DIM_Y;
		}
		while(DIM_Y<100 || DIM_Y>1024);
	}

  int displaySpeed=100;
  std::cout<<"\n\ndo you want to choose display speed? (Y/n)"<< std::endl;
  std::cin>>chooseDim;
  
  if(chooseDim=='Y' || chooseDim=='y')
  {
    do
    {
      std::cout<<"\nChoose display speed (from 1 to 100)"<< std::endl;
      std::cin>>displaySpeed;
    }
    while(displaySpeed<1 || displaySpeed>100);
  }



	// Create 2 dimension tab for each pixel of the screen
	int colorOfPixel[DIM_X][DIM_Y]={0}; // init all the tab to 0 (black) actual color
	int SCREEN_SIZE=(DIM_X*DIM_Y);

	//setup défault color
	sf::Color color[MAX_COLOR_QTY];
	initColors(color);

	// create window
	sf::RenderWindow window(sf::VideoMode(DIM_X, DIM_Y), "test fourmi de langton");

	// Init ant position
	Position posAnt;
	posAnt.x=DIM_X/2;
	posAnt.y=DIM_Y/2;
	posAnt.orientation=0;

	// Create points on screen
	sf::VertexArray pxScreen(sf::Points, SCREEN_SIZE);
	int k=0;
	//!\ can'y have different DIM_X and DIM_Y with this methode
	for(int i=0; i<DIM_X; i++)
	{
	for(int j=0; j<DIM_Y; j++)
	{
	  k++;
	  pxScreen[k].position=sf::Vector2f(i,j);
	  pxScreen[k].color=sf::Color(0, 0, 0);
	}
	}

  //var main loop
  int pxColor=0;
  unsigned long int qtyMoves=0;

  // main loop
  while (window.isOpen())
  {
    //events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed) window.close();
    }

    //loop control
    if(posAnt.x!=0 && posAnt.y!=0 && posAnt.x!=DIM_X && posAnt.y!=DIM_Y)
    {
      //color control
      pxColor=colorOfPixel[posAnt.x][posAnt.y];

      //loading of colors
      if(pxColor < numberOfColor) colorOfPixel[posAnt.x][posAnt.y]++;
      else colorOfPixel[posAnt.x][posAnt.y]=0;

      //change of px orientation
      if(colorOrientation[pxColor] == 0) turnLeft(posAnt);
      else turnRight(posAnt);

      //display
      if(qtyMoves%(50*displaySpeed) == 0)
      {
        //wipe screen
        window.clear(sf::Color::Black);
        
        //display calc
        //!\ Same issue than before
        k=0;
        for(int i=0; i<DIM_X; i++)
        {
          for(int j=0; j<DIM_Y; j++)
          {
            k++;
            pxScreen[k].color=color[colorOfPixel[k%DIM_Y][k/DIM_Y]];
          }
        }

        window.draw(pxScreen); 
        window.display();
      }
    }

    if(posAnt.x!=DIM_X && posAnt.y!=DIM_Y)
    {
      qtyMoves++;
    }
  }
  std::cout << "number of iterations: " << qtyMoves << std::endl;
  std::cout << "Sequence played: " << std::endl;

  for (int i = 0; i < numberOfColor+1; i++)
  {
    if(colorOrientation[i]==0) std::cout<<'L';
    if(colorOrientation[i]==1) std::cout<<'R';
  }
  std::cout<<std::endl;

  return 0;
}


/******************
*    FONCTIONS    *
******************/


void turnLeft(Position &posAnt)
{
  switch(posAnt.orientation) //0:N 1:E 2:S 3:O
  {
    case 0:
    posAnt.x--;
    posAnt.orientation=3;
    break;

    case 1:
    posAnt.y--;
    posAnt.orientation=0;
    break;

    case 2:
    posAnt.x++;
    posAnt.orientation=1;
    break;

    case 3:
    posAnt.y++;
    posAnt.orientation=2;
    break;
    }
}

void turnRight(Position &posAnt)
{
  switch(posAnt.orientation) //0:N 1:E 2:S 3:O
  {
    case 0:
    posAnt.x++;
    posAnt.orientation=1;
    break;

    case 1:
    posAnt.y++;
    posAnt.orientation=2;
    break;

    case 2:
    posAnt.x--;
    posAnt.orientation=3;
    break;

    case 3:
    posAnt.y--;
    posAnt.orientation=0;
    break;
  }
}

void initColors(sf::Color *color)
{
  color[0].r=0;
  color[0].g=0;
  color[0].b=0;

  color[1].r=255;
  color[1].g=255;
  color[1].b=255;

  color[2].r=255;
  color[2].g=0;
  color[2].b=0;

  color[3].r=255;
  color[3].g=255;
  color[3].b=0;

  color[4].r=0;
  color[4].g=255;
  color[4].b=0;

  color[5].r=0;
  color[5].g=255;
  color[5].b=255;

  color[6].r=0;
  color[6].g=0;
  color[6].b=255;

  color[7].r=255;
  color[7].g=0;
  color[7].b=255;

  color[8].r=128;
  color[8].g=64;
  color[8].b=0;

  color[9].r=64;
  color[9].g=128;
  color[9].b=0;

  color[10].r=0;
  color[10].g=128;
  color[10].b=64;

  color[11].r=0;
  color[11].g=64;
  color[11].b=125;

  color[12].r=64;
  color[12].g=0;
  color[12].b=128;

  color[13].r=128;
  color[13].g=0;
  color[13].b=64;

  color[14].r=128;
  color[14].g=128;
  color[14].b=128;

  color[15].r=128;
  color[15].g=0;
  color[15].b=0;

  color[16].r=128;
  color[16].g=128;
  color[16].b=0;

  color[17].r=0;
  color[17].g=128;
  color[17].b=0;

  color[18].r=0;
  color[18].g=0;
  color[18].b=128;

  color[19].r=128;
  color[19].g=0;
  color[19].b=128;

  color[20].r=64;
  color[20].g=64;
  color[20].b=64;

  color[21].r=64;
  color[21].g=0;
  color[21].b=0;

  color[22].r=64;
  color[22].g=64;
  color[22].b=0;

  color[23].r=0;
  color[23].g=64;
  color[23].b=0;

  color[24].r=0;
  color[24].g=64;
  color[24].b=64;

  color[25].r=0;
  color[25].g=0;
  color[25].b=64;

  color[26].r=64;
  color[26].g=0;
  color[26].b=64;

  color[27].r=0;
  color[27].g=128;
  color[27].b=128;

  color[28].r=255;
  color[28].g=0;
  color[28].b=64;

  color[29].r=0;
  color[29].g=255;
  color[29].b=64;
}



