#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

class Linereader{
        private:
                 string filename;
                 int count;
        public:
                string program();
                void count_lines ();
                int read_count ();
};
 void Linereader::count_lines()
 {
     ifstream input_data;
     input_data.open(filename.c_str());
     int count = 0;
     while (!input_data.eof())
        {
                string line;
                getline(input_data,line);
                count++;
        }
     input_data.close();
 }
 int Linereader::read_count()
 {
     return count;
 
 }
 
 int main(int argc, char** argv) {

    return 0;
}

