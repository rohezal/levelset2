#include "levelset.h"


Container::Container(const png::image<png::gray_pixel_16>& _image)
{
    for (int a = 0; a < _image.get_height(); a++)
    {
        image.push_back(std::vector<u_int16_t>());
        image.back().reserve(_image.get_width());

        path.push_back(std::vector<u_int32_t>());
        path.back().reserve(_image.get_width());

        for (int b = 0; b < _image.get_width(); b++)
        {
            image[a].push_back(_image[a][b]);
            if(image[a][b] == 0)
            {
                path[a].push_back(infinity);
            }
            else
            {
                path[a].push_back(max_value);
            }
        }

    }
}


void Container::findWay(int posx, int posy, int endx, int endy, int level)
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

std::pair<int,int> Container::getNextTask(int endx, int endy, int level)
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

void Container::tagCell(int x, int y, int value)
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


void Container::addTask(int x, int y, int value)
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

void Container::exportToPng(const std::string& filename) const
{
	if(path_image.size() == 0)
	{
		png::image< png::gray_pixel_16 > pngimage(path.size(), path[0].size());

		for (int a = 0; a < pngimage.get_height(); a++)
		{
			for (int b = 0; b < pngimage.get_width(); b++)
			{
				uint32_t temp = path[a][b];
				temp = temp > 65000 ? 65000 : temp;
				pngimage[a][b] = temp;
			}
		}
		pngimage.write(filename);
	}

	else
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
				temp = path_image[a][b];
				pngimage_way[a][b] = temp;
			}
		}
		pngimage.write(filename);
		pngimage_way.write(std::string("way")+filename);
	}




}

void Container::clear()
{
	path_image.clear();
	path.clear();
	tasks.clear();
}

std::pair<int, int> Container::getFreePixelAfter(int x, int y)
{
	std::pair<int,int> result;
	result.first = -1;
	result.second = -1;
	for(int a = y; a < image.size(); a++)
	{
		for(int b = x+1; b < image[0].size(); b++)
		{
			if(image[a][b] > 0)
			{
				result.second = a;
				result.first = b;
				return result;
			}
		}
	}
}

void Container::drawPath(int posx, int posy, int endx, int endy)
{
	if(posx == endx && posy == endy)
	{
		return;
	}

	path_image = image;

	while (!(posx == endx && posy == endy))
	{
		this->path_image[posy][posx] = 256*128;
		std::pair<int,int> min = getMinimum(posx,posy);

		posx = min.first;
		posy = min.second;
	}

	//drawPath(min.first,min.second,endx,endy);

}
