#include <iostream>
#include <sstream>
#include "eoclient.hpp"
#include "singleton.hpp"
#include <sys/types.h>
#include <windows.h>
#include <sys/timeb.h>
#include <string>
#include <fstream>

bool autoPot = true;
sf::Clock lastTick;
int refreshed = 0;


EOClient::EOClient(bool initialize)
{
	gioSettings = Gio::Settings::create("org.mt1.EOCmdr.Preferences");
	lastTick.restart();
	this->uptime_clock.restart();
	this->refresh_clock.restart();
	this->Reset();

	this->RegisterHandler(PacketFamily::Init, PacketAction::Init, INIT_INIT);

	if(initialize)
	{
		if(this->Connect())
		{
			this->RequestInit();
		}
	}
}

std::string gen_random(size_t length) {
    auto randchar = []() -> char
    {
        const char charset[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}
std::string gen_randomNum(size_t length) {
	auto randchar = []() -> char
	{
		const char charset[] =
		"1234567890";
		const size_t max_index = (sizeof(charset) - 1);
		return charset[ rand() % max_index ];
	};
	std::string str(length,0);
	std::generate_n( str.begin(), length, randchar );
	return str;
}

std::vector<std::string> EOClient::Explode(std::string str,  std::string delimiter)
{
	std::vector<std::string> arr;

	int strleng = str.length();
	int delleng = delimiter.length();
	if (delleng==0)
		return arr;//no change

	int i=0;
	int k=0;
	while( i<strleng )
	{
		int j=0;
		while (i+j<strleng && j<delleng && str[i+j]==delimiter[j])
			j++;
		if (j==delleng)//found delimiter
		{
			arr.push_back(  str.substr(k, i-k) );
			i+=delleng;
			k=i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(  str.substr(k, i-k) );
	return arr;
}

bool EOClient::Connect()
{
	Config &config = S::GetInstance().config;

	std::string address = std::string(gioSettings->get_string("ip"));
	unsigned short port = std::stoi(std::string(gioSettings->get_string("port")));

	printf("%s:%i", address.c_str(), port);
	sf::Socket::Status status = this->socket->connect(address, port);

	if(status != sf::Socket::Done)
	{
		puts("Socket: couldn't connect.");
		return false;
	}

	this->socket->setBlocking(false);
	this->connected = true;
	puts(" [OK] ------------------------------------------------");

	return true;
}

void EOClient::Disconnect()
{
	S &s = S::GetInstance();
	s.signalStateChanged.emit(EOClient::State::Uninitialized);
	this->socket->disconnect();
	this->Reset();
}

bool EOClient::Connected()
{
	return this->connected;
}

void EOClient::Send(PacketBuilder packet)
{
	if(this->send_buffer.empty())
	{
		PacketBuilder send_packet;

		send_packet.SetID(packet.GetID());
		if(this->state != State::Uninitialized)
		{
			int seq_byte = this->GenSequenceByte();
			if(seq_byte >= 253)
			{
				//unsigned short seq_byte_short = seq_byte;
#ifdef DEBUG
				//printf("Seq byte: %i\n", seq_byte_short);
#endif // DEBUG
				send_packet.AddShort(seq_byte);
			}
			else
			{
				send_packet.AddChar(seq_byte);
			}
		}
		send_packet.AddString(packet.Get().substr(4));
		std::string enc = this->processor.Encode(send_packet.Get());
		const char *data = enc.c_str();
		std::size_t sent = 0;

		sf::Socket::Status status = this->socket->send(data, enc.size(), sent);
		if(status == sf::Socket::Done)
		{

		}
		else if(status == sf::Socket::Partial)
		{
			this->send_buffer = enc.substr(sent);
		}
		else
		{
			puts("Socket: couldn't send data");
		}
	}
}

void EOClient::HandleData(std::string data)
{
	PacketReader reader(this->processor.Decode(data));
	std::map<PacketFamily, std::map<PacketAction, handler_func>>::iterator it;
	std::map<PacketAction, handler_func>::iterator it2;

	it = this->handlers.find(reader.Family());
	if(it != this->handlers.end())
	{
		it2 = this->handlers[reader.Family()].find(reader.Action());
		if(it2 != this->handlers[reader.Family()].end())
		{
			this->handlers[reader.Family()][reader.Action()](reader);
		}
	}
}

void EOClient::SendChatMessage(ChatMessage message){
	if(message.type == ChatType::Local)
		TalkPublic(message.message);
	if(message.type == ChatType::Global)
		TalkGlobal(message.message);
	if(message.type == ChatType::Private)
		TalkTell(message.receiver, message.message);
	if(message.type == ChatType::Guild)
		TalkGuild(message.message);
	if(message.type == ChatType::Party)
		TalkParty(message.message);
}

void EOClient::RegisterHandler(PacketFamily family, PacketAction action, handler_func func)
{
	this->handlers[family][action] = func;
}


void EOClient::DropItem(short itemID, int amount, int x, int y)
{
	PacketBuilder packet(PacketFamily::Item, PacketAction::Drop);
	packet.AddShort(itemID);
	packet.AddThree(amount);
	packet.AddChar(x);
	packet.AddChar(y);
	this->Send(packet);
}

void EOClient::UnregisterHandler(PacketFamily family, PacketAction action)
{
	std::map<PacketFamily, std::map<PacketAction, handler_func>>::iterator it;
	it = this->handlers.find(family);
	if(it != this->handlers.end())
	{
		this->handlers.erase(it);
	}
}


Direction EOClient::GetDirection(short x, short y){
	S &s = S::GetInstance();
	Direction targetDir = s.character.direction;
	if(x < s.character.x && y == s.character.y)
	{
		targetDir = Direction::Left;
	}
	if(x > s.character.x  && y == s.character.y)
	{
		targetDir = Direction::Right;
	}
	if(y < s.character.y && x == s.character.x)
	{
		targetDir = Direction::Up;
	}
	if(y > s.character.y && x == s.character.x)
	{
		targetDir = Direction::Down;
	}
	return targetDir;
}


void EOClient::Tick()
{
	S &s = S::GetInstance();
	Logger* logger = Logger::Instance();
	srand (time(NULL));
	if(this->connected)
	{
		char *databuff = new char[1024];
		std::size_t received = 0;

		std::string data;
		bool done = false;
		int oldlength;

		sf::Socket::Status status = this->socket->receive(databuff, 1024, received);

		if(status == sf::Socket::Status::Done)
		{
			std::string datastr(databuff);
			this->recv_buffer += datastr.substr(0, received);
		}
		else if(status == sf::Socket::Status::NotReady)
		{

		}
		else if(status == sf::Socket::Status::Disconnected)
		{
			S &s = S::GetInstance();
			s.signalStateChanged.emit(EOClient::State::Uninitialized);
			puts("Socket: disconnected");
			this->Reset();
		}
		else if(status == sf::Socket::Status::Error)
		{
			puts("Socket: error while receiving data");
			S &s = S::GetInstance();
			s.signalStateChanged.emit(EOClient::State::Uninitialized);
			this->Reset();
		}

		delete databuff;

		std::size_t datasize = 1;
		if(this->packet_state == EOClient::PacketState::ReadData)
		{
			datasize = this->length;
		}

		data = this->recv_buffer.substr(0, datasize);
		std::fill(this->recv_buffer.begin(), this->recv_buffer.begin() + datasize, '\0');
		this->recv_buffer.erase(0, datasize);

		while (data.length() > 0 && !done)
		{
			switch (this->packet_state)
			{
			case EOClient::PacketState::ReadLen1:
				this->raw_length[0] = data[0];
				data[0] = '\0';
				data.erase(0, 1);
				this->packet_state = EOClient::PacketState::ReadLen2;

				if (data.length() == 0)
				{
					break;
				}

			case EOClient::PacketState::ReadLen2:
				this->raw_length[1] = data[0];
				data[0] = '\0';
				data.erase(0, 1);
				this->length = PacketProcessor::Number(this->raw_length[0], this->raw_length[1]);
				this->packet_state = EOClient::PacketState::ReadData;

				if (data.length() == 0)
				{
					break;
				}

			case EOClient::PacketState::ReadData:
				oldlength = this->data.length();
				this->data += data.substr(0, this->length);
				std::fill(data.begin(), data.begin() + std::min<std::size_t>(data.length(), this->length), '\0');
				data.erase(0, this->length);
				this->length -= this->data.length() - oldlength;

				if (this->length == 0)
				{
					PacketReader reader(this->processor.Decode(this->data));

					if(s.config.GetValue("ShowDebug") == "on"){
						logger->Log("[Server] " + this->processor.GetFamilyName(reader.Family()) + ":" + this->processor.GetActionName(reader.Action()));
						printf("[Server] %s:%s\n", this->processor.GetFamilyName(reader.Family()).c_str(), this->processor.GetActionName(reader.Action()).c_str());
					}
					this->HandleData(this->data);
					std::fill((std::begin((this->data))), (std::end((this->data))), '\0');
					this->data.erase();
					this->packet_state = EOClient::PacketState::ReadLen1;

					done = true;
				}
				break;

			default:
				// If the code ever gets here, something is broken, so we just reset the client's state.
				// thanks Sausage
				this->Reset();
			}
		}

		if(!this->send_buffer.empty())
		{
			const char *to_send = this->send_buffer.c_str();
			std::size_t sent = 0;

			sf::Socket::Status status = this->socket->send(to_send, this->send_buffer.size(), sent);
			if(status == sf::Socket::Done)
			{
				std::fill((std::begin((data))), (std::end((data))), '\0');
				this->send_buffer.erase();
			}
			else if(status == sf::Socket::Partial)
			{
				std::fill(data.begin(), data.begin() + sent, '\0');
				this->send_buffer.erase(sent);
			}
		}
		//Game tick
		if(this->state == State::Playing){
			if(refresh_clock.getElapsedTime().asMilliseconds() >= 10000){
				refresh_clock.restart();
				RequestPlayerList();
			}
		}
	}
}


void EOClient::Reset()
{
	this->socket.reset();
	this->socket = unique_ptr<sf::TcpSocket>(new sf::TcpSocket());
	this->connected = false;
	std::fill((std::begin((this->send_buffer))), (std::end((this->send_buffer))), '\0');
	this->send_buffer.erase();
	std::fill((std::begin((this->recv_buffer))), (std::end((this->recv_buffer))), '\0');
	this->recv_buffer.erase();
	this->packet_state = EOClient::PacketState::ReadLen1;
	std::fill((std::begin((this->data))), (std::end((this->data))), '\0');
	this->data.erase();
	this->length = 0;
	this->raw_length[0] = 0;
	this->raw_length[1] = 0;
	this->seq_start = 0;
	this->seq = 0;
	this->session_id = 0;
	this->state = EOClient::State::Uninitialized;
	this->account.characters.clear();
}


void EOClient::InitSequenceByte(unsigned char s1, unsigned char s2)
{
	this->seq_start = std::uint32_t(s2 + s1 * 7);
	this->seq_start -= 13;
}

void EOClient::UpdateSequenceByte(unsigned short s1, unsigned char s2)
{
	this->seq_start = std::uint32_t(s1 - s2);
#ifdef DEBUG
	//printf("s1: %i, s2: %i, seq_start: %i, seq: %i\n", s1, s2, this->seq_start, this->seq);
#endif // DEBUG
}

int EOClient::GenSequenceByte()
{
	if(++this->seq >= 10)
		seq = 0;

	int ret = std::uint32_t(this->seq_start + this->seq);
#ifdef DEBUG
	//printf("seq_start: %i, seq: %i\n", this->seq_start, this->seq);
#endif // DEBUG

	return ret;
}

void EOClient::SetState(State state)
{
	S &s = S::GetInstance();
	this->state = state;
	s.signalStateChanged(state);
}

EOClient::State EOClient::GetState()
{
	return this->state;
}

void EOClient::RequestInit()
{
	PacketBuilder builder(PacketFamily::Init, PacketAction::Init);
	unsigned int challenge = 72000;
	builder.AddThree(challenge);
	builder.AddChar(2); // ?
	builder.AddChar(2); // ?
	builder.AddChar(28); // version
	builder.AddChar(2); // ?
	builder.AddChar(2); // ?
	builder.AddString(std::to_string(63948573164));//std::to_string(hddid + 3)); // HDD ID
	S&s = S::GetInstance();
	s.eoclient.Send(builder);
}

void EOClient::Initialize(PacketReader reader)
{
	printf("bildetilder");
	int s1 = reader.GetByte();
	int s2 = reader.GetByte();
	int emulti_d = reader.GetByte();
	int emulti_e = reader.GetByte();
	int id = reader.GetShort();
	reader.GetThree(); // hash response

	this->processor.SetEMulti(emulti_e, emulti_d);

	this->InitSequenceByte(s1, s2);
	this->session_id = id;

	this->state = EOClient::State::Initialized;
}

void EOClient::LoginRequest(std::string username, std::string password)
{
	PacketBuilder packet(PacketFamily::Login, PacketAction::Request);
	packet.AddBreakString(username);
	packet.AddBreakString(password);
	this->Send(packet);
}

void EOClient::LoginRequest(Glib::ustring username, Glib::ustring password)
{
	this->LoginRequest((std::string)username, (std::string)password);
}

void EOClient::RequestPlayerList(){
	PacketBuilder packet(PacketFamily::Players, PacketAction::Request);
	this->Send(packet);
}

void EOClient::AccountRequest(std::string username)
{
	PacketBuilder packet(PacketFamily::Account, PacketAction::Request);
	packet.AddString(username);
	this->Send(packet);
}

void EOClient::AccountCreate(std::string username, std::string password/*, std::string real_name, std::string location, std::string email*/)
{
	std::string computer = "DESKTOP-H8VUTY";

	PacketBuilder packet(PacketFamily::Account, PacketAction::Create);
	packet.AddShort(this->session_id);
	packet.AddByte(1); // ?
	packet.AddBreakString(username);
	packet.AddBreakString(password);
	packet.AddBreakString(gen_random(rand() % 20 + 3)); //real name
	packet.AddBreakString(gen_random(rand() % 20 + 3));//location
	packet.AddBreakString(gen_random(rand() % 20 + 3) + "@" + gen_random(rand() % 20 + 3) + "." + gen_random(rand() % 20 + 3));//email
	packet.AddBreakString(computer);//computer
	packet.AddString(gen_randomNum(11)); // HDD ID
	this->Send(packet);
}

void EOClient::CharacterRequest(){
	PacketBuilder packet(PacketFamily::Character, PacketAction::Request);
	packet.AddShort(this->session_id);
	packet.AddString("Wÿ");//what is this - idk ask vult
	this->Send(packet);
}

void EOClient::CreateCharacter(short gender,short hairstyle,short haircolor,short race, std::string name){
		this->CharacterRequest();
		Sleep(1);
		PacketBuilder packet( PacketFamily::Character, PacketAction::Create);
		packet.AddShort(this->session_id);//CREATEID
		packet.AddShort(gender);
		packet.AddShort(hairstyle);
		packet.AddShort(haircolor);
		packet.AddShort(race);
		packet.AddByte(255);//?
		packet.AddBreakString(name);
		this->Send(packet);
}

void EOClient::SelectCharacter(unsigned int id)
{
	PacketBuilder packet(PacketFamily::Welcome, PacketAction::Request);
	packet.AddInt(id);
	this->Send(packet);
}

void EOClient::TalkPublic(std::string message)
{
	PacketBuilder packet(PacketFamily::Talk, PacketAction::Report); //Change to Report for public speech
	packet.AddString(message);
	this->Send(packet);
	printf("Lcl: %s\n", message.c_str());
}

void EOClient::PartyRemove(unsigned short id){
	PacketBuilder packet(PacketFamily::Party, PacketAction::Remove);
	packet.AddShort(id);
	this->Send(packet);
}

void EOClient::TalkParty(std::string message)
{
	PacketBuilder packet(PacketFamily::Talk, PacketAction::Open); //Change to Report for public speech
	packet.AddString(message);
	this->Send(packet);
	printf("Party: %s\n", message.c_str());
}
void EOClient::OpenShop(short npcId){
	PacketBuilder packet(PacketFamily::Shop, PacketAction::Open);
	packet.AddShort(npcId);
	this->Send(packet);
}

void EOClient::BuyShop(short itemId, int amount){
	PacketBuilder packet2(PacketFamily::Shop, PacketAction::Buy);
	packet2.AddShort((unsigned short)itemId);
	packet2.AddInt(amount);
	this->Send(packet2);
}

void EOClient::PickUp(short uid){
	PacketBuilder packet(PacketFamily::Item, PacketAction::Get);
	packet.AddShort(uid);
	this->Send(packet);
}

void EOClient::TakeChest(int x, int y, int itemId){
	//PacketBuilder packet(PacketFamily::Chest, PacketAction::Open);
	//packet.AddChar((unsigned char)x);
	//packet.AddChar((unsigned char)y);
	//this->Send(packet);
	//Sleep(1000);
	PacketBuilder packet2(PacketFamily::Chest, PacketAction::Take);
	packet2.AddChar((unsigned char)x);
	packet2.AddChar((unsigned char)y);
	packet2.AddChar((unsigned char)itemId);
	this->Send(packet2);
}

//36 35 in eo, 37 36 in packet decoder
void EOClient::OpenDoor(int x, int y)
{
	PacketBuilder packet(PacketFamily::Door, PacketAction::Open);
	packet.AddChar((unsigned char)x);
	packet.AddChar((unsigned char)y);
	this->Send(packet);
}
void EOClient::TalkWPublic(std::wstring message)
{
	PacketBuilder packet(PacketFamily::Talk, PacketAction::Report); //Change to Report for public speech
	packet.AddWString(message);
	this->Send(packet);
}

void EOClient::TalkPublicOverride(std::string message)
{
	PacketBuilder packet(PacketFamily::Talk, PacketAction::Report); //Change to Report for public speech
	packet.AddString(message);
	this->Send(packet);
	printf("Lcl: %s\n", message.c_str());
}

void EOClient::TalkGuild(std::string message)
{
	PacketBuilder packet(PacketFamily::Talk, PacketAction::Request);
	packet.AddString(message);
	this->Send(packet);
	printf("Gld: %s\n", message.c_str());
}

void EOClient::PartyInvite(unsigned char type, unsigned short playerID)
{
	PacketBuilder packet(PacketFamily::Party, PacketAction::Request);
	packet.AddChar(type);
	packet.AddShort(playerID);
	this->Send(packet);
}

void EOClient::TalkGlobal(std::string message)
{
	PacketBuilder packet(PacketFamily::Talk, PacketAction::Message);
	packet.AddString(message);
	this->Send(packet);
	printf("Glb: %s\n", message.c_str());
}

void EOClient::UseItem(short itemId)
{
	PacketBuilder packet(PacketFamily::Item, PacketAction::Use);
	packet.AddShort(itemId);
	this->Send(packet);
}

void EOClient::TalkTell(std::string name, std::string message)
{
	PacketBuilder packet(PacketFamily::Talk, PacketAction::Tell);
	packet.AddBreakString(name);
	packet.AddString(message);
	this->Send(packet);
	printf("PM -> %s: %s\n", name.c_str(), message.c_str());
}

void EOClient::Face(Direction direction)
{
	S::GetInstance().character.direction = direction;

	PacketBuilder packet(PacketFamily::Face, PacketAction::Player);
	packet.AddChar((unsigned char)direction);
	this->Send(packet);
}

void EOClient::RefreshRequest()
{
	PacketBuilder packet(PacketFamily::Refresh, PacketAction::Request);
	this->Send(packet);
}
void EOClient::TradeRequest(short gameworld_id)
{
	PacketBuilder packet(PacketFamily::Trade, PacketAction::Request);
	packet.AddChar(138);
	packet.AddShort(gameworld_id);
	this->Send(packet);
}

void EOClient::TradeAccept(short gameworld_id)
{
	PacketBuilder packet(PacketFamily::Trade, PacketAction::Accept);
	packet.AddChar(138);
	packet.AddShort(gameworld_id);
	this->Send(packet);
}

void EOClient::TradeAdd(short item_id, int item_amount)
{
	PacketBuilder packet(PacketFamily::Trade, PacketAction::Add);
	packet.AddShort(item_id);
	packet.AddInt(item_amount);
	this->Send(packet);
}

void EOClient::TradeAgree()
{
	PacketBuilder packet(PacketFamily::Trade, PacketAction::Agree);
	packet.AddChar(true);
	this->Send(packet);
}

void EOClient::TradeClose()
{
	PacketBuilder packet(PacketFamily::Trade, PacketAction::Close);
	packet.AddChar(138);
	this->Send(packet);
}

void EOClient::AcceptPartyInvite(unsigned char type, unsigned short playerID){
	PacketBuilder packet(PacketFamily::Party, PacketAction::Accept);
	packet.AddChar(type);
	packet.AddShort(playerID);
	this->Send(packet);
}

void EOClient::Equip(unsigned short itemID, unsigned short subLoc){
	PacketBuilder packet(PacketFamily::Paperdoll, PacketAction::Add);
	packet.AddShort(itemID);
	packet.AddShort(subLoc);
	this->Send(packet);
}

int GetTimestamp()
{
	time_t rawtime;
	struct tm *realtime;
	struct _timeb timebuffer;
	int hour, minn, sec, msec;

	time ( &rawtime );
	realtime=localtime( &rawtime );
	_ftime( &timebuffer );
	hour = realtime->tm_hour;
	minn = realtime->tm_min;
	sec = realtime->tm_sec;
	msec = timebuffer.millitm;

	return hour*360000 + minn*6000 + sec*100 + msec/10;
}

void EOClient::Walk(Direction direction)
{
	S &s = S::GetInstance();


	s.character.direction = direction;

	int xoff[4] = { 0, -1, 0, 1 };
	int yoff[4] = { 1, 0, -1, 0 };

	PacketBuilder packet(PacketFamily::Walk, PacketAction::Player);
	packet.AddChar((unsigned char)direction);
	packet.AddThree(GetTimestamp());
	packet.AddChar(s.character.x + xoff[(unsigned char)direction]);
	packet.AddChar(s.character.y + yoff[(unsigned char)direction]);
	this->Send(packet);

	s.character.attacks = 0;
	if(direction == Direction::Up){
		s.character.newY = s.character.y - 1;
		s.character.newX = s.character.x;
	}
	if(direction == Direction::Down){
		s.character.newY = s.character.y + 1;
		s.character.newX = s.character.x;
	}
	if(direction == Direction::Left){
		s.character.newX = s.character.x - 1;
		s.character.newY = s.character.y;
	}
	if(direction == Direction::Right){
		s.character.newX = s.character.x + 1;
		s.character.newY = s.character.y;
	}
}

void EOClient::Attack(Direction direction)
{
	//S::GetInstance().character.direction = direction;
	S::GetInstance().character.attacks++;
	int ts = GetTimestamp();
	PacketBuilder packet(PacketFamily::Attack, PacketAction::Use);
	packet.AddChar((unsigned char)direction);
	packet.AddThree(ts);
	this->Send(packet);
}

void EOClient::Sit(SitAction sit_action)
{
	PacketBuilder packet(PacketFamily::Sit, PacketAction::Request);
	packet.AddChar((unsigned char)1/*(unsigned char)sit_action*/);
	this->Send(packet);
}

void EOClient::Christmas(){
	PacketBuilder   packet(PacketFamily::Quest, PacketAction::Use);
	packet.AddInt(51);
	this->Send(packet);
}
