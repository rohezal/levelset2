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
		std::pair<int,int> getNextTask(int endx, int endy, int level);
		void nextTask(int endx, int endy, int level);
		void tagCell(int x, int y, int value);
		void addTask(int x, int y, int value);
		void drawPath(int posx, int posy, int endx, int endy);
		void debug() const;
		void exportToPng(const std::string& filename) const;
		void clear();

		inline bool validCords(int x, int y) const
        {
            return (x >= 0 && x < image[0].size() && y >= 0 && y < image.size() && path[y][x] < infinity);
        }

		inline std::pair<int,int> getMinimum(int posx, int posy) const
        {
			int minx = posx;
			int miny = posy;
			u_int32_t minimum = path[posy][posx];

			#pragma GCC unroll 10
            for(int a = -1; a <= 1; a++)
            {
				#pragma GCC unroll 5
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
            return std::pair<int,int>(minx,miny);
        }

		std::vector<std::vector<u_int16_t > >& getImage()
		{
			return image;
		}

		std::vector<std::vector<u_int32_t > >& getPath()
		{
			return path;
		}


	private:
        std::set<int> lookup;
        std::map<int,std::vector<std::pair<int,int> > > tasks;
		std::vector<std::vector<std::pair<int,int> > > data;
        std::vector<std::vector<u_int16_t > >image;
        std::vector<std::vector<u_int32_t > > path;
        uint32_t goal_distance = infinity;
};


