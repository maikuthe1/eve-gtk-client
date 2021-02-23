#ifndef ATLAS_H
#define ATLAS_H
#include "../sausage/dib_reader.hpp"
#include "../sausage/pe_reader.hpp"
#include "../sausage/util/cio/cio.hpp"
#include "../sausage/util/int_pack.hpp"
#include "../sausage/common.hpp"
#include <gdkmm-3.0/gdkmm.h>
#include <glibmm-2.4/glibmm.h>
#include <gtkmm-3.0/gtkmm.h>
#include <fstream>

class Atlas {
public:
	struct BitmapInfo
	{
		size_t size;
		size_t rawSize;
		int width;
		int height;
		int stride;
		unsigned char* bytes;
		unsigned char* rawBytes;
	};
protected:
	std::map<std::string, std::map<unsigned int, BitmapInfo>> cache;
	
public:
	Atlas();
	Atlas::BitmapInfo GetSprite(std::string fileName, int index);
	Gtk::Image* GetSubSprite(std::string fileName, int index, int x, int y, int width, int height);
};


#endif