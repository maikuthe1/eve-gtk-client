#include "atlas.hpp"

Atlas::Atlas(){
	
}

Atlas::BitmapInfo Atlas::GetSprite(std::string fileName, int index){
	index += 100;
	auto it = cache.find(fileName);
	std::map<unsigned int, BitmapInfo> imageList;
	if(it != cache.end())
	{
		imageList = it->second;
		auto it2 = imageList.find(index);
		BitmapInfo bmpInfo;
		if(it2 != imageList.end()){
			bmpInfo = it2->second;
			return bmpInfo;
		}
	}
	
	std::string prefix = "gfx/" + fileName;
	cio::stream file(prefix.c_str(), cio::stream::mode_read);
	Atlas::BitmapInfo sprite;
	sprite.size = 0;

	if (!file){
		printf("\n\nCouldn't load the file");
		// Log me
	}

	pe_reader pe(std::move(file));

	if (!pe.read_header()){
		printf("\n\nCouldn't read the headetr");
		// Log me
	}

	std::map<int, pe_reader::BitmapInfo> table = pe.read_bitmap_table();
	pe_reader::BitmapInfo info = table[index];
	
	u_ptr<char[]> buf(new char[info.size]);
	pe.read_resource(buf.get(), info.start, info.size);
	dib_reader dib(buf.get(), info.size);
	dib.start();
	printf("Info for %s %i: Header length: %i, Palette length: %i, BPP: %i, Stride: %i, Compression: %i", fileName.c_str(), index, dib.header_size(), dib.palette_size(), dib.bpp(), dib.stride(), (int)dib.compression());
	unsigned char fixedHeader[14] = {0x42, 0x4D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00};
	sprite.size = sizeof(fixedHeader) + dib.image_size() + dib.header_size();
	sprite.width = info.width;
	sprite.height = info.height;
	sprite.bytes = new unsigned char[sizeof(fixedHeader) + dib.image_size() + dib.header_size()];
	sprite.rawSize = dib.image_size() + dib.header_size();
	sprite.rawBytes = new unsigned char[dib.image_size() + dib.header_size()];
	for(int i = 0; i < sizeof(fixedHeader); i++){
		sprite.bytes[i] = fixedHeader[i];
	}
	for(int i = 0; i < dib.image_size() + dib.header_size(); i++){
		sprite.bytes[i + sizeof(fixedHeader)] = buf.get()[i];
		sprite.rawBytes[i] = buf.get()[i];
	}
	sprite.stride = dib.stride();
	cache[fileName][index] = sprite;
	//std::string out = std::to_string(index) + ".bmp";
	//std::ofstream stream(out.c_str(), std::ios::binary | std::ios::out);
	//stream.write((char*)sprite.bytes, sprite.size);
	//stream.close();
	return sprite;
}

Gtk::Image* Atlas::GetSubSprite(std::string fileName, int index, int x, int y, int width, int height){
	//Cairo::RefPtr<Cairo::ImageSurface> imSur = Cairo::ImageSurface::create(sprite.bytes, Cairo::FORMAT_RGB24, sprite.width, sprite.height, Cairo::ImageSurface::format_stride_for_width(Cairo::FORMAT_RGB24, sprite.width));
	Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
	Atlas::BitmapInfo sprite = GetSprite(fileName, index);
	loader->write(sprite.bytes, sprite.size);
	loader->close();
	Glib::RefPtr<Gdk::Pixbuf> pixBuf = loader->get_pixbuf();
	Gtk::Image *image = Gtk::manage(new Gtk::Image(Gdk::Pixbuf::create_subpixbuf(loader->get_pixbuf(), x, y, width, height)));
	return image;
}