#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <png++/png.hpp>
#include <string>
#include <set>
#include <map>

const uint32_t infinity = (256*256*255+254);
const uint32_t max_value = infinity-1;

class Container
{
    public:
        Container(const png::image<png::gray_pixel_16>& _image);


		void findWay(int posx, int posy, int endx, int endy, int level = 0);

		/*
        void findWay(int posx, int posy, int endx, int endy, int level = 0)
        {
            std::pair<int,int> nextcell;

            if(level == 0)
            {
                path[posy][posx] = 0;
                nextcell.first = posx;
                nextcell.second = posy;
            }

            do
            {

                posx = nextcell.first;
                posy = nextcell.second;

                const uint32_t current_value = path[posy][posx];

                if(current_value > goal_distance)
                {
                    //std::cout << current_value << std::endl;

                    if(tasks.size() > 0)
                    {
                        nextcell = getNextTask(endx,endy, current_value+1);
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }

                if(posx == endx && posy == endy)
                {
                    goal_distance = current_value;
                    if(tasks.size() > 0)
                    {
                        nextcell = getNextTask(endx,endy, current_value+1);
                        continue;
                    }
                    else
                    {
                        break;
                    }
                }

                if(validCords(posx,posy) && validCords(endx,endy))
                {
                    //std::cout << "(" << posy << "," << posx << "): " << path[posy][posx] << std::endl;
                    for(int a = -1; a <= 1; a++)
                    {
                        for(int b = -1; b <= 1; b++)
                        {
                            int nposx = posx+b;
                            int nposy = posy+a;
                            tagCell(nposx,nposy,current_value+1);
                        }
                    }
                }
                if(tasks.size() > 0)
                {
                   nextcell = getNextTask(endx,endy, current_value+1);
                }


            }
            while (tasks.size() > 0);


        }
		*/

		std::pair<int,int> getNextTask(int endx, int endy, int level);

		/*
        std::pair<int,int> getNextTask(int endx, int endy, int level)
        {
            int key = (*(tasks.begin())).first;
            std::vector<std::pair<int,int> >& taskvector = tasks[key];

            if(taskvector.size() == 0)
            {
                    std::cout << "ERROR! VECTOR EMPTY FROM MAP IN nextTask" << std::endl;
            }

            std::pair<int,int> cell = taskvector.back();

            taskvector.pop_back();

            if(taskvector.size() == 0)
            {
                tasks.erase(key);
            }


            return cell;
        }
		*/


		void nextTask(int endx, int endy, int level);
		/*
        void nextTask(int endx, int endy, int level)
        {
            int key = (*(tasks.begin())).first;
            std::vector<std::pair<int,int> >& taskvector = tasks[key];

            if(taskvector.size() == 0)
            {
                    std::cout << "ERROR! VECTOR EMPTY FROM MAP IN nextTask" << std::endl;
            }

            std::pair<int,int> cell = taskvector.back();

            taskvector.pop_back();

            if(taskvector.size() == 0)
            {
                tasks.erase(key);
            }


            findWay(cell.first, cell.second, endx, endy, level);
        }
		*/

		void tagCell(int x, int y, int value);

		/*
		void tagCell(int x, int y, int value)
		{
			if(validCords(x,y))
			{
				if(path[y][x] > value && path[y][x] < infinity)
				{
					path[y][x] = value;
					addTask(x,y,value);
				}
			}
		}
		 */


		void addTask(int x, int y, int value);
		/*
        void addTask(int x, int y, int value)
        {
            //lookup.insert(value);
            //data[value].push_back(std::pair<int,int>(x,y));

            std::map<int,std::vector< std::pair<int,int> > >::iterator i = tasks.find(value);
            if (i == tasks.end())
            {
                std::vector<std::pair<int,int> > temp;
                temp.push_back(std::pair<int,int>(x,y));
                tasks[value] = temp;
            }
            else
            {
                tasks[value].push_back(std::pair<int,int>(x,y));
            }
        }
		*/

		inline bool validCords(int x, int y) const
        {
            return (x >= 0 && x < image[0].size() && y >= 0 && y < image.size() && path[y][x] < infinity);
        }

		void exportToPng(const std::string& filename) const;

		/*
		void exportToPng(const std::string& filename) const
        {
            png::image< png::gray_pixel_16 > pngimage(path.size(), path[0].size());
            png::image< png::gray_pixel_16 > pngimage_way(path.size(), path[0].size());

            for (int a = 0; a < pngimage.get_height(); a++)
            {
                for (int b = 0; b < pngimage.get_width(); b++)
                {
                    uint32_t temp = path[a][b];
                    temp = temp > 65000 ? 65000 : temp;
                    pngimage[a][b] = temp;
                    temp = image[a][b];
                    pngimage_way[a][b] = temp;
                }
            }
            pngimage.write(filename);
            pngimage_way.write(std::string("way")+filename);
		}*/

		inline std::pair<int,int> getMinimum(int posx, int posy) const
        {
            int minx = posx;
            int miny = posy;
            u_int32_t minimum = path[posy][posx];
            u_int32_t oldminimum = path[posy][posx];

            for(int a = -1; a <= 1; a++)
            {
                for(int b = -1; b <= 1; b++)
                {
                    int nposx = posx+b;
                    int nposy = posy+a;
                    if(validCords(nposx,nposy))
                    {
                        if(path[nposy][nposx] < minimum)
                        {
                            miny = nposy;
                            minx = nposx;
                            minimum = path[nposy][nposx];
                        }
                    }
                }
            }

            //std::cout << "(" << miny << "," << minx << "): " << minimum << "<" << oldminimum << std::endl;

            return std::pair<int,int>(minx,miny);
        }

		void drawPath(int posx, int posy, int endx, int endy);

		/*
		void drawPath(int posx, int posy, int endx, int endy)
        {
            if(posx == endx && posy == endy)
            {
                return;
            }

            this->image[posy][posx] = 256*128;
            std::pair<int,int> min = getMinimum(posx,posy);

            drawPath(min.first,min.second,endx,endy);

		}*/

		void debug() const;

		/*
		void debug()
        {
            int counter = 0;

            for(size_t i = 0; i < data.size(); i++)
            {
                if(data[i].size() > 0)
                {
                    counter++;
                    std::cout << "Container " << i << ": " << data[i].size() << std::endl;
                }
            }
            std::cout << "Filled containers" << " " << ": " << counter << std::endl;
        }
		*/

        std::set<int> lookup;
        std::map<int,std::vector<std::pair<int,int> > > tasks;
		std::vector<std::vector<std::pair<int,int> > > data;
        std::vector<std::vector<u_int16_t > >image;
        std::vector<std::vector<u_int32_t > > path;
        uint32_t goal_distance = infinity;
};


