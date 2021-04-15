// design a musical jukebox using object oriented principles
// here we simulate a media player program on a computer that play mp3

// this is what the jukebox can do:
// play a song 
// play a album
// create a play list
// play a play list
// queue a new song to the play list
// play next song
// play previous song

#include <vector>
#include <string>
#include <list>
#include <set>
using namespace std;

// song tracks the file location 
struct Song; 
struct Album; // a list of songs

// play list keeps a state of the current song
struct PlayList
{
    PlayList(const string& name, const vector<Song>& songs);
    PlayList(const Album& album);
    
    const Song& getCurrentSong() const;

    // this wraps around the play list
    const Song& getNextSong() const;
    const Song& getPrevSong() const;

    string name;
    vector<Song> songs;
    size_t idx = 0;
};

// player is responsible for playing the song with different setting
// such as volumn, effects etc
struct Player
{
    Player();

    void play(const Song& s);
    void next();
    void prev();
    void changeSetting();

    PlayList& pl;
};

struct Jukebox // storage of albums and play lists
{
    Jukebox();

    // storage of songs
    set<Album> albums;
    set<PlayList> lists;

    // interactive component with a user
    Player p;
};


int main(int argc, char* argv[])
{

    return 0;
}
